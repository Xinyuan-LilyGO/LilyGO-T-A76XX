
/* This file is auto-generated. DO NOT EDIT. */

#ifndef RPC_HANDLER_HW_H
#define RPC_HANDLER_HW_H

#ifdef __cplusplus
extern "C" {
#endif


bool rpc_hw_setUartBaudRate_impl(uint32_t baud);

static
void rpc_hw_setUartBaudRate_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint32_t baud; MessageBuffer_readUInt32(_rpc_buff, &baud);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_setUartBaudRate_impl(baud);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_initUserButton_impl(uint16_t gpio, bool active_low);

static
void rpc_hw_initUserButton_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t gpio; MessageBuffer_readUInt16(_rpc_buff, &gpio);
  bool active_low; MessageBuffer_readBool(_rpc_buff, &active_low);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_initUserButton_impl(gpio, active_low);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_initLED_impl(uint16_t gpio, bool active_low);

static
void rpc_hw_initLED_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t gpio; MessageBuffer_readUInt16(_rpc_buff, &gpio);
  bool active_low; MessageBuffer_readBool(_rpc_buff, &active_low);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_initLED_impl(gpio, active_low);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_initRGB_impl(uint16_t gpio_r, uint16_t gpio_g, uint16_t gpio_b, bool common_anode);

static
void rpc_hw_initRGB_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t gpio_r; MessageBuffer_readUInt16(_rpc_buff, &gpio_r);
  uint16_t gpio_g; MessageBuffer_readUInt16(_rpc_buff, &gpio_g);
  uint16_t gpio_b; MessageBuffer_readUInt16(_rpc_buff, &gpio_b);
  bool common_anode; MessageBuffer_readBool(_rpc_buff, &common_anode);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_initRGB_impl(gpio_r, gpio_g, gpio_b, common_anode);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_initARGB_impl(uint16_t gpio, uint8_t mode, uint8_t count);

static
void rpc_hw_initARGB_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint16_t gpio; MessageBuffer_readUInt16(_rpc_buff, &gpio);
  uint8_t mode; MessageBuffer_readUInt8(_rpc_buff, &mode);
  uint8_t count; MessageBuffer_readUInt8(_rpc_buff, &count);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_initARGB_impl(gpio, mode, count);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_setLedBrightness_impl(uint8_t value);

static
void rpc_hw_setLedBrightness_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  uint8_t value; MessageBuffer_readUInt8(_rpc_buff, &value);

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_setLedBrightness_impl(value);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_getWiFiMAC_impl(const char** mac);

static
void rpc_hw_getWiFiMAC_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* mac; memset(&mac, 0, sizeof(mac)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_getWiFiMAC_impl(&mac);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(mac);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}


bool rpc_hw_getEthernetMAC_impl(const char** mac);

static
void rpc_hw_getEthernetMAC_handler(MessageBuffer* _rpc_buff) {
  uint16_t _rpc_seq;
  MessageBuffer_readUInt16(_rpc_buff, &_rpc_seq);
  /* Deserialize arguments */
  const char* mac; memset(&mac, 0, sizeof(mac)); /* output */

  if (MessageBuffer_getError(_rpc_buff) || MessageBuffer_availableToRead(_rpc_buff)) {
    MessageWriter_sendResultStatus(_rpc_seq, RPC_STATUS_ERROR_ARGS_R);
    return;
  }

  /* Call the actual function */
  bool _rpc_ret_val = rpc_hw_getEthernetMAC_impl(&mac);

  /* Send response */
  MessageWriter_beginResult(_rpc_seq, RPC_STATUS_OK);
  MessageWriter_writeString(mac);
  MessageWriter_writeBool(_rpc_ret_val);
  MessageWriter_end();
}

#ifdef __cplusplus
}
#endif

#endif /* RPC_HANDLER_HW_H */
