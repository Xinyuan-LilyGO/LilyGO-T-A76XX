
#include "BlynkRpc.h"

static RpcStatus _rpc_status;
static uint16_t  _rpc_seq_id;

static uint32_t _rpc_timeout_override = 0;
uint32_t _rpc_last_rx_time = 0;
uint32_t _rpc_last_tx_time = 0;

RpcStatus rpc_get_status(void) {
  return _rpc_status;
}

uint32_t rpc_get_last_rx(void) {
  return rpc_system_millis() - _rpc_last_rx_time;
}

uint32_t rpc_get_last_tx(void) {
  return rpc_system_millis() - _rpc_last_tx_time;
}

void rpc_set_timeout(uint32_t ms) {
  _rpc_timeout_override = ms;
}

void rpc_set_status(RpcStatus status) {
  _rpc_status = status;
}

uint16_t rpc_next_seq(void) {
  return ++_rpc_seq_id;
}

const char* rpc_get_status_str(RpcStatus status) {
  switch (status) {
    case RPC_STATUS_OK             : return "OK";
    case RPC_STATUS_ERROR_GENERIC  : return "Generic error";
    case RPC_STATUS_ERROR_TIMEOUT  : return "Timeout";
    case RPC_STATUS_ERROR_MEMORY   : return "Memory failed";
    case RPC_STATUS_ERROR_UID      : return "Function not found";
    case RPC_STATUS_ERROR_ARGS_W   : return "Args writing failed";
    case RPC_STATUS_ERROR_ARGS_R   : return "Args reading failed";
    case RPC_STATUS_ERROR_RETS_W   : return "Rets writing failed";
    case RPC_STATUS_ERROR_RETS_R   : return "Rets reading failed";
  }
  return "unknown";
}

static
bool rpc_handle_msg(MessageBuffer* buff)
{
  uint16_t op;
  MessageBuffer_readUInt16(buff, &op);
  if (op == RPC_OP_INVOKE || op == RPC_OP_ONEWAY) {
      uint16_t id;
      MessageBuffer_readUInt16(buff, &id);
      rpc_invoke_handler(id, buff);
  } else {
      return false;
  }

  return true;
}

RpcStatus rpc_wait_result(uint16_t expected_seq, MessageBuffer* buff, uint32_t timeout)
{
  if (_rpc_timeout_override) {
    timeout = _rpc_timeout_override;
  }
  uint8_t status = RPC_STATUS_ERROR_TIMEOUT;
  const uint32_t tstart = rpc_system_millis();
  const uint32_t ttotal = timeout;
  while (rpc_recv_msg(buff, timeout)) {
    _rpc_last_rx_time = rpc_system_millis();
    const uint32_t elapsed = _rpc_last_rx_time - tstart;
    timeout = (ttotal > elapsed) ? (ttotal - elapsed) : 0;

    uint16_t op = 0;
    MessageBuffer_readUInt16(buff, &op);
    if (op == RPC_OP_RESULT) {
      uint16_t seq = 0;
      MessageBuffer_readUInt16(buff, &seq);
      if (seq == expected_seq) {
        MessageBuffer_readUInt8(buff, &status);
        break;
      } else {
        // not our reply => skip it
        continue;
      }
    }
    // process unexpected messages
    MessageBuffer_rewind(buff);
    rpc_handle_msg(buff);
  }
  return (RpcStatus)status;
}

void rpc_run(void) {
  MessageBuffer buff;
  MessageBuffer_init(&buff, NULL, 0);
  while (rpc_recv_msg(&buff, 0)) {
    _rpc_last_rx_time = rpc_system_millis();
    rpc_handle_msg(&buff);
  }
}
