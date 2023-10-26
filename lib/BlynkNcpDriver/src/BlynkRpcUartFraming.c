
#include <string.h>

#include "BlynkRpc.h"
#include "BlynkRpcUartFraming.h"
#include "BlynkRpcCRC8.h"

typedef enum {
  BEG   = 0xAA,
  END   = 0xBB,
  ESC   = 0xCC,
  XON   = 0x11,
  XOFF  = 0x13
} SpecialByte;

typedef enum {
  STATE_BEG,
  STATE_DATA,
  STATE_ESC,
  STATE_END
} RpcUartFramingState;

typedef struct {
  uint8_t data[32];
  uint8_t read;
  uint8_t write;
  uint8_t count;
} RpcUartInputBuffer;

typedef struct {
  RpcUartInputBuffer  buffer;
  uint8_t   state;
  uint8_t   rcrc;
  uint8_t   wcrc;
  bool      escapeXonXoff;
} RpcUartFraming;

static RpcUartFraming _self;

void RpcUartFraming_init(void) {
  memset(&_self, 0, sizeof(_self));
  _self.state = STATE_BEG;
  _self.escapeXonXoff = true;
}

static
bool RpcUartFraming_hasPacketData(void) {
  return _self.buffer.count > 1;
}

static
void RpcUartFraming_processInput(void) {
  if (_self.state == STATE_END && _self.buffer.count > 0) { return; }

  while (rpc_uart_available() && _self.buffer.count < sizeof(_self.buffer.data)) {
    uint8_t data = rpc_uart_read();
    switch (_self.state) {
      case STATE_BEG:
        if (data == BEG) {
          _self.state = STATE_DATA;
        }
        break;
      case STATE_DATA:
        if (data == ESC) {
          _self.state = STATE_ESC;
        } else if (data == END) {
          _self.state = STATE_END;
          return;
        } else {
          _self.buffer.data[_self.buffer.write++] = data;
          _self.buffer.write %= sizeof(_self.buffer.data);
          _self.buffer.count++;
        }
        break;
      case STATE_ESC:
        data ^= 0xFF;
        _self.buffer.data[_self.buffer.write++] = data;
        _self.buffer.write %= sizeof(_self.buffer.data);
        _self.buffer.count++;
        _self.state = STATE_DATA;
        break;
      case STATE_END:
        if (data == BEG) {
          _self.state = STATE_DATA;
        }
        break;
    }
  }
}

static
size_t RpcUartFraming_writeByte(uint8_t data) {
  rpc_crc8_update(&_self.wcrc, data);
  if (data == BEG || data == END || data == ESC ||
      (_self.escapeXonXoff && (data == XON || data == XOFF))
  ) {
    rpc_uart_write(ESC);
    data ^= 0xFF;
  }
  return rpc_uart_write(data);
}


void RpcUartFraming_beginPacket(void) {
  rpc_crc8_reset(&_self.wcrc);
  rpc_uart_write(BEG);
}

void RpcUartFraming_endPacket(void) {
  RpcUartFraming_writeByte(_self.wcrc);
  rpc_uart_write(END);
  rpc_uart_flush();
}

bool RpcUartFraming_finishedPacket(void) {
  return _self.state == STATE_END && !RpcUartFraming_hasPacketData();
}

bool RpcUartFraming_checkPacketCRC(void) {
  if (!RpcUartFraming_finishedPacket()) { return false; }
  const uint8_t expected = _self.rcrc;
  const uint8_t actual = _self.buffer.data[_self.buffer.read++];
  _self.buffer.read %= sizeof(_self.buffer.data);
  _self.buffer.count--;
  rpc_crc8_reset(&_self.rcrc);
  _self.state = STATE_BEG;
  return actual == expected;
}

size_t RpcUartFraming_write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  while (size--) {
    if (RpcUartFraming_writeByte(*buffer++)) n++;
    else break;
  }
  return n;
}

int RpcUartFraming_available(void) {
  RpcUartFraming_processInput();
  return RpcUartFraming_hasPacketData() ? (_self.buffer.count - 1) : 0;
}

int RpcUartFraming_read(void) {
  RpcUartFraming_processInput();
  if (_self.buffer.count <= 1) {
    return -1;
  }
  uint8_t data = _self.buffer.data[_self.buffer.read++];
  _self.buffer.read %= sizeof(_self.buffer.data);
  _self.buffer.count--;

  rpc_crc8_update(&_self.rcrc, data);
  return data;
}
