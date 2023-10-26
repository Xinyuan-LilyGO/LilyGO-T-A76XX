
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_HANDLER_BLYNK_H
#define RPC_HANDLER_BLYNK_H

#ifdef __cplusplus
extern "C" {
#endif


bool rpc_blynk_getNcpVersion_impl(const char** ver);

static
void rpc_blynk_getNcpVersion_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* ver; memset(&ver, 0, sizeof(ver)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_getNcpVersion_impl(&ver);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(ver);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setVendorPrefix_impl(const char* vendor);

static
void rpc_blynk_setVendorPrefix_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* vendor; MessageBuffer_readString(_rpc_buff, &vendor);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setVendorPrefix_impl(vendor);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setVendorServer_impl(const char* host);

static
void rpc_blynk_setVendorServer_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* host; MessageBuffer_readString(_rpc_buff, &host);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setVendorServer_impl(host);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setFirmwareInfo_impl(const char* type, const char* version, const char* build, const char* blynk);

static
void rpc_blynk_setFirmwareInfo_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* type; MessageBuffer_readString(_rpc_buff, &type);
  const char* version; MessageBuffer_readString(_rpc_buff, &version);
  const char* build; MessageBuffer_readString(_rpc_buff, &build);
  const char* blynk; MessageBuffer_readString(_rpc_buff, &blynk);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setFirmwareInfo_impl(type, version, build, blynk);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_initialize_impl(const char* templateID, const char* templateName);

static
void rpc_blynk_initialize_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* templateID; MessageBuffer_readString(_rpc_buff, &templateID);
  const char* templateName; MessageBuffer_readString(_rpc_buff, &templateName);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_initialize_impl(templateID, templateName);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_getHotspotName_impl(const char** hotspot);

static
void rpc_blynk_getHotspotName_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* hotspot; memset(&hotspot, 0, sizeof(hotspot)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_getHotspotName_impl(&hotspot);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(hotspot);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_isConfigured_impl(void);

static
void rpc_blynk_isConfigured_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_isConfigured_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_configStart_impl(void);

static
void rpc_blynk_configStart_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_configStart_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_configStop_impl(void);

static
void rpc_blynk_configStop_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_configStop_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_configReset_impl(void);

static
void rpc_blynk_configReset_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_configReset_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setConfigTimeout_impl(uint16_t timeout);

static
void rpc_blynk_setConfigTimeout_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t timeout; MessageBuffer_readUInt16(_rpc_buff, &timeout);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setConfigTimeout_impl(timeout);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setConfigSkipLimit_impl(uint8_t count);

static
void rpc_blynk_setConfigSkipLimit_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint8_t count; MessageBuffer_readUInt8(_rpc_buff, &count);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setConfigSkipLimit_impl(count);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_setTime_impl(int64_t time);

static
void rpc_blynk_setTime_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  int64_t time; MessageBuffer_readInt64(_rpc_buff, &time);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_setTime_impl(time);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_getTime_impl(const char** iso8601, int64_t* time, int16_t* offset, const char** tz_abbr, uint8_t* dst_status);

static
void rpc_blynk_getTime_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* iso8601; memset(&iso8601, 0, sizeof(iso8601)); /* output */
  int64_t time; memset(&time, 0, sizeof(time)); /* output */
  int16_t offset; memset(&offset, 0, sizeof(offset)); /* output */
  const char* tz_abbr; memset(&tz_abbr, 0, sizeof(tz_abbr)); /* output */
  uint8_t dst_status; memset(&dst_status, 0, sizeof(dst_status)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_getTime_impl(&iso8601, &time, &offset, &tz_abbr, &dst_status);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(iso8601);
  MessageWriter_writeInt64(time);
  MessageWriter_writeInt16(offset);
  MessageWriter_writeString(tz_abbr);
  MessageWriter_writeUInt8(dst_status);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_getLocationInfo_impl(const char** lat, const char** lon, const char** olson_id, const char** posix_tz);

static
void rpc_blynk_getLocationInfo_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* lat; memset(&lat, 0, sizeof(lat)); /* output */
  const char* lon; memset(&lon, 0, sizeof(lon)); /* output */
  const char* olson_id; memset(&olson_id, 0, sizeof(olson_id)); /* output */
  const char* posix_tz; memset(&posix_tz, 0, sizeof(posix_tz)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_getLocationInfo_impl(&lat, &lon, &olson_id, &posix_tz);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(lat);
  MessageWriter_writeString(lon);
  MessageWriter_writeString(olson_id);
  MessageWriter_writeString(posix_tz);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_otaUpdateStart_impl(uint16_t chunk);

static
void rpc_blynk_otaUpdateStart_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t chunk; MessageBuffer_readUInt16(_rpc_buff, &chunk);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_otaUpdateStart_impl(chunk);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_otaUpdateGetCRC32_impl(uint32_t* crc);

static
void rpc_blynk_otaUpdateGetCRC32_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint32_t crc; memset(&crc, 0, sizeof(crc)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_otaUpdateGetCRC32_impl(&crc);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeUInt32(crc);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_otaUpdateGetMD5_impl(buffer_t* digest);

static
void rpc_blynk_otaUpdateGetMD5_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  buffer_t digest; memset(&digest, 0, sizeof(digest)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_otaUpdateGetMD5_impl(&digest);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBinary(digest);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_otaUpdateGetSHA256_impl(buffer_t* digest);

static
void rpc_blynk_otaUpdateGetSHA256_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  buffer_t digest; memset(&digest, 0, sizeof(digest)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_otaUpdateGetSHA256_impl(&digest);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBinary(digest);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


uint8_t rpc_blynk_otaUpdatePrefetch_impl(void);

static
void rpc_blynk_otaUpdatePrefetch_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  uint8_t _rpc_ret_val = rpc_blynk_otaUpdatePrefetch_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeUInt8(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_blynk_factoryReset_impl(void);

static
void rpc_blynk_factoryReset_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  bool _rpc_ret_val = rpc_blynk_factoryReset_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


uint8_t rpc_blynk_factoryTestWiFiAP_impl(uint16_t channel);

static
void rpc_blynk_factoryTestWiFiAP_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t channel; MessageBuffer_readUInt16(_rpc_buff, &channel);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  uint8_t _rpc_ret_val = rpc_blynk_factoryTestWiFiAP_impl(channel);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeUInt8(_rpc_ret_val);
  MessageWriter_end();
}


uint8_t rpc_blynk_factoryTestWiFi_impl(const char* ssid, const char* pass, int16_t* rssi);

static
void rpc_blynk_factoryTestWiFi_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* ssid; MessageBuffer_readString(_rpc_buff, &ssid);
  const char* pass; MessageBuffer_readString(_rpc_buff, &pass);
  int16_t rssi; memset(&rssi, 0, sizeof(rssi)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  uint8_t _rpc_ret_val = rpc_blynk_factoryTestWiFi_impl(ssid, pass, &rssi);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeInt16(rssi);
  MessageWriter_writeUInt8(_rpc_ret_val);
  MessageWriter_end();
}


uint8_t rpc_blynk_factoryTestConnect_impl(void);

static
void rpc_blynk_factoryTestConnect_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  uint8_t _rpc_ret_val = rpc_blynk_factoryTestConnect_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeUInt8(_rpc_ret_val);
  MessageWriter_end();
}


uint8_t rpc_blynk_getState_impl(void);

static
void rpc_blynk_getState_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  (void)_rpc_buff;

  /* Call the actual function */
  uint8_t _rpc_ret_val = rpc_blynk_getState_impl();

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeUInt8(_rpc_ret_val);
  MessageWriter_end();
}


void rpc_blynk_virtualWrite_impl(uint16_t vpin, buffer_t value);

static
void rpc_blynk_virtualWrite_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint16_t vpin; MessageBuffer_readUInt16(_rpc_buff, &vpin);
  buffer_t value; MessageBuffer_readBinary(_rpc_buff, &value);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_virtualWrite_impl(vpin, value);

  /* Oneway => skip response */
}


void rpc_blynk_setProperty_impl(uint16_t vpin, const char* property, buffer_t value);

static
void rpc_blynk_setProperty_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  uint16_t vpin; MessageBuffer_readUInt16(_rpc_buff, &vpin);
  const char* property; MessageBuffer_readString(_rpc_buff, &property);
  buffer_t value; MessageBuffer_readBinary(_rpc_buff, &value);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_setProperty_impl(vpin, property, value);

  /* Oneway => skip response */
}


void rpc_blynk_syncAll_impl(void);

static
void rpc_blynk_syncAll_handler(MessageBuffer* _rpc_buff) {
  (void)_rpc_buff;

  /* Call the actual function */
  rpc_blynk_syncAll_impl();

  /* Oneway => skip response */
}


void rpc_blynk_syncVirtual_impl(buffer_t vpins);

static
void rpc_blynk_syncVirtual_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  buffer_t vpins; MessageBuffer_readBinary(_rpc_buff, &vpins);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_syncVirtual_impl(vpins);

  /* Oneway => skip response */
}


void rpc_blynk_logEvent_impl(const char* event_code, const char* description);

static
void rpc_blynk_logEvent_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  const char* event_code; MessageBuffer_readString(_rpc_buff, &event_code);
  const char* description; MessageBuffer_readString(_rpc_buff, &description);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_logEvent_impl(event_code, description);

  /* Oneway => skip response */
}


void rpc_blynk_resolveEvent_impl(const char* event_code);

static
void rpc_blynk_resolveEvent_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  const char* event_code; MessageBuffer_readString(_rpc_buff, &event_code);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_resolveEvent_impl(event_code);

  /* Oneway => skip response */
}


void rpc_blynk_resolveAllEvents_impl(const char* event_code);

static
void rpc_blynk_resolveAllEvents_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  const char* event_code; MessageBuffer_readString(_rpc_buff, &event_code);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_resolveAllEvents_impl(event_code);

  /* Oneway => skip response */
}


void rpc_blynk_beginGroup_impl(int64_t timestamp);

static
void rpc_blynk_beginGroup_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  int64_t timestamp; MessageBuffer_readInt64(_rpc_buff, &timestamp);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_beginGroup_impl(timestamp);

  /* Oneway => skip response */
}


void rpc_blynk_endGroup_impl(void);

static
void rpc_blynk_endGroup_handler(MessageBuffer* _rpc_buff) {
  (void)_rpc_buff;

  /* Call the actual function */
  rpc_blynk_endGroup_impl();

  /* Oneway => skip response */
}


void rpc_blynk_setMetadata_impl(const char* field, const char* value);

static
void rpc_blynk_setMetadata_handler(MessageBuffer* _rpc_buff) {
  /* Deserialize arguments */
  const char* field; MessageBuffer_readString(_rpc_buff, &field);
  const char* value; MessageBuffer_readString(_rpc_buff, &value);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    return;
  }

  /* Call the actual function */
  rpc_blynk_setMetadata_impl(field, value);

  /* Oneway => skip response */
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_HANDLER_BLYNK_H */
