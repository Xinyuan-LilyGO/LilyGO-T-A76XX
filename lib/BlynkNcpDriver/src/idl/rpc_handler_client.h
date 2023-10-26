
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_HANDLER_CLIENT_H
#define RPC_HANDLER_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif


void rpc_client_blynkVPinChange_impl(uint16_t vpin, buffer_t param);

static
void rpc_client_blynkVPinChange_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint16_t vpin; MessageBuffer_readUInt16(_rpc_buff, &vpin);
  buffer_t param; MessageBuffer_readBinary(_rpc_buff, &param);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_client_blynkVPinChange_impl(vpin, param);

  /* Oneway => skip response */
}


void rpc_client_blynkStateChange_impl(uint8_t state);

static
void rpc_client_blynkStateChange_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint8_t state; MessageBuffer_readUInt8(_rpc_buff, &state);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_client_blynkStateChange_impl(state);

  /* Oneway => skip response */
}


void rpc_client_processEvent_impl(uint8_t event);

static
void rpc_client_processEvent_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint8_t event; MessageBuffer_readUInt8(_rpc_buff, &event);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_client_processEvent_impl(event);

  /* Oneway => skip response */
}


bool rpc_client_otaUpdateAvailable_impl(const char* filename, uint32_t filesize, const char* fw_type, const char* fw_ver, const char* fw_build);

static
void rpc_client_otaUpdateAvailable_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* filename; MessageBuffer_readString(_rpc_buff, &filename);
  uint32_t filesize; MessageBuffer_readUInt32(_rpc_buff, &filesize);
  const char* fw_type; MessageBuffer_readString(_rpc_buff, &fw_type);
  const char* fw_ver; MessageBuffer_readString(_rpc_buff, &fw_ver);
  const char* fw_build; MessageBuffer_readString(_rpc_buff, &fw_build);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_client_otaUpdateAvailable_impl(filename, filesize, fw_type, fw_ver, fw_build);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_client_otaUpdateWrite_impl(uint32_t offset, buffer_t chunk, uint32_t crc32);

static
void rpc_client_otaUpdateWrite_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint32_t offset; MessageBuffer_readUInt32(_rpc_buff, &offset);
  buffer_t chunk; MessageBuffer_readBinary(_rpc_buff, &chunk);
  uint32_t crc32; MessageBuffer_readUInt32(_rpc_buff, &crc32);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_client_otaUpdateWrite_impl(offset, chunk, crc32);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_client_otaUpdateFinish_impl(void);

static
void rpc_client_otaUpdateFinish_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_client_otaUpdateFinish_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


void rpc_client_otaUpdateCancel_impl(void);

static
void rpc_client_otaUpdateCancel_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  rpc_client_otaUpdateCancel_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_end();
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_HANDLER_CLIENT_H */
