#include "MessageBuffer.h"
#include "MessageWriter.h"
#include "BlynkRpcUartFraming.h"
#include "BlynkRpc.h"

extern uint32_t _rpc_last_tx_time;

size_t MessageWriter_writeString(const char* value) {
    if (!value) {
        return MessageWriter_write("", 1);
    }
    size_t length = strlen(value) + 1;
    return MessageWriter_write(value, length);
}

size_t MessageWriter_writeBinary(buffer_t value) {
    size_t len = sizeof(uint16_t) + value.length;
    /*if (MessageBuffer_availableToWrite(self) < len) {
        self->_error = true;
        return 0;
    }*/
    MessageWriter_writeUInt16(value.length);
    MessageWriter_write(value.data, value.length);
    return len;
}

size_t MessageWriter_writeBool(bool value) {
    return MessageWriter_writeUInt8(value ? 1 : 0);
}

size_t MessageWriter_writeInt8(const int8_t value) {
    return MessageWriter_write(&value, sizeof(int8_t));
}

size_t MessageWriter_writeInt16(const int16_t value) {
    return MessageWriter_write(&value, sizeof(int16_t));
}

size_t MessageWriter_writeInt32(const int32_t value) {
    return MessageWriter_write(&value, sizeof(int32_t));
}

size_t MessageWriter_writeInt64(const int64_t value) {
    return MessageWriter_write(&value, sizeof(int64_t));
}

size_t MessageWriter_writeUInt8(const uint8_t value) {
    return MessageWriter_write(&value, sizeof(uint8_t));
}

size_t MessageWriter_writeUInt16(const uint16_t value) {
    return MessageWriter_write(&value, sizeof(uint16_t));
}

size_t MessageWriter_writeUInt32(const uint32_t value) {
    return MessageWriter_write(&value, sizeof(uint32_t));
}

size_t MessageWriter_writeUInt64(const uint64_t value) {
    return MessageWriter_write(&value, sizeof(uint64_t));
}

size_t MessageWriter_writeFloat(const float value) {
    return MessageWriter_write(&value, sizeof(float));
}

uint16_t MessageWriter_beginInvoke(uint16_t uid) {
    uint16_t seq = rpc_next_seq();
    MessageWriter_begin();
    MessageWriter_writeUInt16(RPC_OP_INVOKE);
    MessageWriter_writeUInt16(uid);
    MessageWriter_writeUInt16(seq);
    return seq;
}

void MessageWriter_beginOneway(uint16_t uid) {
    MessageWriter_begin();
    MessageWriter_writeUInt16(RPC_OP_ONEWAY);
    MessageWriter_writeUInt16(uid);
}

void MessageWriter_beginResult(uint16_t seq, uint8_t status) {
    MessageWriter_begin();
    MessageWriter_writeUInt16(RPC_OP_RESULT);
    MessageWriter_writeUInt16(seq);
    MessageWriter_writeUInt8(status);
}

void MessageWriter_sendResultStatus(uint16_t seq, uint8_t status) {
    MessageWriter_beginResult(seq, status);
    MessageWriter_end();
}

size_t MessageWriter_begin(void) {
    RpcUartFraming_beginPacket();
    return 1;
}
size_t MessageWriter_end(void) {
    RpcUartFraming_endPacket();
    _rpc_last_tx_time = rpc_system_millis();
    return 1;
}

size_t MessageWriter_write(const void* data, size_t size) {
    /*if (!data || MessageBuffer_availableToWrite(self) < size) {
        self->_error = true;
        return 0;
    }
    memcpy(self->_buffer + self->_wpos, data, size);
    self->_wpos += size;*/

    return RpcUartFraming_write((const uint8_t*)data, size);
}
