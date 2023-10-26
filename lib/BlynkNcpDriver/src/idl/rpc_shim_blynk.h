
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_SHIM_BLYNK_H
#define RPC_SHIM_BLYNK_H

#ifdef __cplusplus
extern "C" {
#endif


static inline
bool rpc_blynk_getNcpVersion(const char** ver) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_GETNCPVERSION);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readString(&_rsp_buff, ver);
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
bool rpc_blynk_setVendorPrefix(const char* vendor) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETVENDORPREFIX);
  MessageWriter_writeString(vendor);
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
bool rpc_blynk_setVendorServer(const char* host) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETVENDORSERVER);
  MessageWriter_writeString(host);
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
bool rpc_blynk_setFirmwareInfo(const char* type, const char* version, const char* build, const char* blynk) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETFIRMWAREINFO);
  MessageWriter_writeString(type);
  MessageWriter_writeString(version);
  MessageWriter_writeString(build);
  MessageWriter_writeString(blynk);
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
bool rpc_blynk_initialize(const char* templateID, const char* templateName) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_INITIALIZE);
  MessageWriter_writeString(templateID);
  MessageWriter_writeString(templateName);
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
bool rpc_blynk_getHotspotName(const char** hotspot) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_GETHOTSPOTNAME);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readString(&_rsp_buff, hotspot);
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
bool rpc_blynk_isConfigured(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_ISCONFIGURED);
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
bool rpc_blynk_configStart(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_CONFIGSTART);
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
bool rpc_blynk_configStop(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_CONFIGSTOP);
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
bool rpc_blynk_configReset(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_CONFIGRESET);
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
bool rpc_blynk_setConfigTimeout(uint16_t timeout) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETCONFIGTIMEOUT);
  MessageWriter_writeUInt16(timeout);
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
bool rpc_blynk_setConfigSkipLimit(uint8_t count) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETCONFIGSKIPLIMIT);
  MessageWriter_writeUInt8(count);
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
bool rpc_blynk_setTime(int64_t time) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_SETTIME);
  MessageWriter_writeInt64(time);
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
bool rpc_blynk_getTime(const char** iso8601, int64_t* time, int16_t* offset, const char** tz_abbr, uint8_t* dst_status) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_GETTIME);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readString(&_rsp_buff, iso8601);
    MessageBuffer_readInt64(&_rsp_buff, time);
    MessageBuffer_readInt16(&_rsp_buff, offset);
    MessageBuffer_readString(&_rsp_buff, tz_abbr);
    MessageBuffer_readUInt8(&_rsp_buff, dst_status);
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
bool rpc_blynk_getLocationInfo(const char** lat, const char** lon, const char** olson_id, const char** posix_tz) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_GETLOCATIONINFO);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readString(&_rsp_buff, lat);
    MessageBuffer_readString(&_rsp_buff, lon);
    MessageBuffer_readString(&_rsp_buff, olson_id);
    MessageBuffer_readString(&_rsp_buff, posix_tz);
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
bool rpc_blynk_otaUpdateStart(uint16_t chunk) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_OTAUPDATESTART);
  MessageWriter_writeUInt16(chunk);
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
bool rpc_blynk_otaUpdateGetCRC32(uint32_t* crc) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_OTAUPDATEGETCRC32);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readUInt32(&_rsp_buff, crc);
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
bool rpc_blynk_otaUpdateGetMD5(buffer_t* digest) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_OTAUPDATEGETMD5);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readBinary(&_rsp_buff, digest);
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
bool rpc_blynk_otaUpdateGetSHA256(buffer_t* digest) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_OTAUPDATEGETSHA256);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readBinary(&_rsp_buff, digest);
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
uint8_t rpc_blynk_otaUpdatePrefetch(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  uint8_t _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_OTAUPDATEPREFETCH);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 30000);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readUInt8(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
bool rpc_blynk_factoryReset(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  bool _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_FACTORYRESET);
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
uint8_t rpc_blynk_factoryTestWiFiAP(uint16_t channel) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  uint8_t _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_FACTORYTESTWIFIAP);
  MessageWriter_writeUInt16(channel);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 5000);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readUInt8(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
uint8_t rpc_blynk_factoryTestWiFi(const char* ssid, const char* pass, int16_t* rssi) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  uint8_t _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_FACTORYTESTWIFI);
  MessageWriter_writeString(ssid);
  MessageWriter_writeString(pass);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 15000);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readInt16(&_rsp_buff, rssi);
    MessageBuffer_readUInt8(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
uint8_t rpc_blynk_factoryTestConnect(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  uint8_t _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_FACTORYTESTCONNECT);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, 10000);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readUInt8(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
uint8_t rpc_blynk_getState(void) {
  RpcStatus _rpc_res;
  /* Prepare return value */
  uint8_t _rpc_ret_val;
  memset(&_rpc_ret_val, 0, sizeof(_rpc_ret_val));

  /* Send request */
  const uint16_t _rpc_seq = MessageWriter_beginInvoke(RPC_UID_BLYNK_GETSTATE);
  MessageWriter_end();

  /* Wait response */
  MessageBuffer _rsp_buff;
  MessageBuffer_init(&_rsp_buff, NULL, 0);
  _rpc_res = rpc_wait_result(_rpc_seq, &_rsp_buff, RPC_TIMEOUT_DEFAULT);
  if (_rpc_res == RPC_STATUS_OK) {
    /* Deserialize outputs */
    MessageBuffer_readUInt8(&_rsp_buff, &_rpc_ret_val);
  }
  if (MessageBuffer_getError(&_rsp_buff) || MessageBuffer_availableToRead(&_rsp_buff)) {
    rpc_set_status(_rpc_res = RPC_STATUS_ERROR_RETS_R);
    return _rpc_ret_val;
  }

  rpc_set_status(_rpc_res);
  return _rpc_ret_val;
}


static inline
void rpc_blynk_virtualWrite(uint16_t vpin, buffer_t value) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_VIRTUALWRITE);
  MessageWriter_writeUInt16(vpin);
  MessageWriter_writeBinary(value);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_setProperty(uint16_t vpin, const char* property, buffer_t value) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_SETPROPERTY);
  MessageWriter_writeUInt16(vpin);
  MessageWriter_writeString(property);
  MessageWriter_writeBinary(value);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_syncAll(void) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_SYNCALL);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_syncVirtual(buffer_t vpins) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_SYNCVIRTUAL);
  MessageWriter_writeBinary(vpins);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_logEvent(const char* event_code, const char* description) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_LOGEVENT);
  MessageWriter_writeString(event_code);
  MessageWriter_writeString(description);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_resolveEvent(const char* event_code) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_RESOLVEEVENT);
  MessageWriter_writeString(event_code);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_resolveAllEvents(const char* event_code) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_RESOLVEALLEVENTS);
  MessageWriter_writeString(event_code);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_beginGroup(int64_t timestamp) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_BEGINGROUP);
  MessageWriter_writeInt64(timestamp);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_endGroup(void) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_ENDGROUP);
  MessageWriter_end();

  /* Oneway => skip response */
}


static inline
void rpc_blynk_setMetadata(const char* field, const char* value) {
  /* Send request */
  MessageWriter_beginOneway(RPC_UID_BLYNK_SETMETADATA);
  MessageWriter_writeString(field);
  MessageWriter_writeString(value);
  MessageWriter_end();

  /* Oneway => skip response */
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_SHIM_BLYNK_H */
