
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_SHIM_NCP_H
#define RPC_SHIM_NCP_H

#ifdef __cplusplus
extern "C" {
#endif


static inline
RpcStatus rpc_ncp_ping(void) {
  RpcStatus _rpc_res;
  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_NCP_PING);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 100);

  rpc_set_status(_rpc_res);
  return _rpc_res;
}


static inline
bool rpc_ncp_reboot(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_NCP_REBOOT);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readBool(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
bool rpc_ncp_hasUID(uint16_t uid) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_NCP_HASUID);
  MessageWriter_writeUInt16(uid);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readBool(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_SHIM_NCP_H */
