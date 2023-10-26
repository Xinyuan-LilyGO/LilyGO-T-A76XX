#ifndef BLYNK_RPC_CLIENT_H
#define BLYNK_RPC_CLIENT_H

#include "BlynkRpc.h"

/*
 * Shims
 */

#include "idl/rpc_shim_ncp.h"
#include "idl/rpc_shim_hw.h"
#include "idl/rpc_shim_blynk.h"

#include "idl/rpc_handler_mcu.h"
#include "idl/rpc_handler_client.h"

rpc_handler_t rpc_find_uid_handler(uint16_t uid) {
  switch (uid) {
  case RPC_UID_MCU_PING:                  return rpc_mcu_ping_handler;
  case RPC_UID_MCU_REBOOT:                return rpc_mcu_reboot_handler;
  case RPC_UID_MCU_HASUID:                return rpc_mcu_hasUID_handler;

  case RPC_UID_CLIENT_BLYNKSTATECHANGE:   return rpc_client_blynkStateChange_handler;
  case RPC_UID_CLIENT_BLYNKVPINCHANGE:    return rpc_client_blynkVPinChange_handler;

  case RPC_UID_CLIENT_PROCESSEVENT:       return rpc_client_processEvent_handler;
  case RPC_UID_CLIENT_OTAUPDATEAVAILABLE: return rpc_client_otaUpdateAvailable_handler;
  case RPC_UID_CLIENT_OTAUPDATEWRITE:     return rpc_client_otaUpdateWrite_handler;
  case RPC_UID_CLIENT_OTAUPDATEFINISH:    return rpc_client_otaUpdateFinish_handler;
  case RPC_UID_CLIENT_OTAUPDATECANCEL:    return rpc_client_otaUpdateCancel_handler;

  default: return NULL;
  }
}

RpcStatus rpc_invoke_handler(uint16_t uid, MessageBuffer* buff) {
  rpc_handler_t handler = rpc_find_uid_handler(uid);
  if (handler) {
    handler(buff);
    return RPC_STATUS_OK;
  } else {
    return RPC_STATUS_ERROR_UID;
  }
}

bool rpc_mcu_hasUID_impl(uint16_t uid) {
  return rpc_find_uid_handler(uid) != NULL;
}

#endif /* BLYNK_RPC_CLIENT_H */
