#if !defined(RPC_DISABLE_WEAK_IMPL)

#include "BlynkRpc.h"

RPC_ATTR_WEAK
bool rpc_mcu_reboot_impl(void) {
  return false;
}

RPC_ATTR_WEAK
void rpc_client_blynkVPinChange_impl(uint16_t vpin, buffer_t param) {

}

RPC_ATTR_WEAK
void rpc_client_blynkStateChange_impl(uint8_t state) {

}

RPC_ATTR_WEAK
void rpc_client_processEvent_impl(uint8_t event) {

}

RPC_ATTR_WEAK
bool rpc_client_otaUpdateAvailable_impl(const char* filename, uint32_t filesize, const char* fw_type, const char* fw_ver, const char* fw_build) {
  return false;
}

RPC_ATTR_WEAK
bool rpc_client_otaUpdateWrite_impl(uint32_t offset, buffer_t chunk, uint32_t crc32) {
  return false;
}

RPC_ATTR_WEAK
bool rpc_client_otaUpdateFinish_impl(void) {
  return false;
}

RPC_ATTR_WEAK
void rpc_client_otaUpdateCancel_impl(void) {

}

#endif /* !defined(RPC_DISABLE_WEAK_IMPL) */
