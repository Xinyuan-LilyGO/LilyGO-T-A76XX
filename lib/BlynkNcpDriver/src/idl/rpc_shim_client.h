
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_SHIM_CLIENT_H
#define RPC_SHIM_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif


static inline
void rpc_client_blynkVPinChange(uint16_t vpin, buffer_t param) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_CLIENT_BLYNKVPINCHANGE);
  MessageWriter_writeUInt16(vpin);
  MessageWriter_writeBinary(param);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_client_blynkStateChange(uint8_t state) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_CLIENT_BLYNKSTATECHANGE);
  MessageWriter_writeUInt8(state);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_client_processEvent(uint8_t event) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_CLIENT_PROCESSEVENT);
  MessageWriter_writeUInt8(event);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
bool rpc_client_otaUpdateAvailable(const char* filename, uint32_t filesize, const char* fw_type, const char* fw_ver, const char* fw_build) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_CLIENT_OTAUPDATEAVAILABLE);
  MessageWriter_writeString(filename);
  MessageWriter_writeUInt32(filesize);
  MessageWriter_writeString(fw_type);
  MessageWriter_writeString(fw_ver);
  MessageWriter_writeString(fw_build);
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
bool rpc_client_otaUpdateWrite(uint32_t offset, buffer_t chunk, uint32_t crc32) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_CLIENT_OTAUPDATEWRITE);
  MessageWriter_writeUInt32(offset);
  MessageWriter_writeBinary(chunk);
  MessageWriter_writeUInt32(crc32);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 5000);
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
bool rpc_client_otaUpdateFinish(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_CLIENT_OTAUPDATEFINISH);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 5000);
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
void rpc_client_otaUpdateCancel(void) {
  RpcStatus _rpc_res;
  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_CLIENT_OTAUPDATECANCEL);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);

  rpc_set_status(_rpc_res);
  return;
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_SHIM_CLIENT_H */
