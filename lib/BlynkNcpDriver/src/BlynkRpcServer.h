#ifndef BLYNK_RPC_SERVER_H
#define BLYNK_RPC_SERVER_H

#include "BlynkRpc.h"

/*
 * Shims
 */

#include "idl/rpc_handler_ncp.h"
#include "idl/rpc_handler_hw.h"
#include "idl/rpc_handler_blynk.h"

#include "idl/rpc_shim_mcu.h"
#include "idl/rpc_shim_client.h"

rpc_handler_t rpc_find_uid_handler(uint16_t uid) {
  switch (uid) {
  case RPC_UID_NCP_PING:                return rpc_ncp_ping_handler;
  case RPC_UID_NCP_REBOOT:              return rpc_ncp_reboot_handler;
  case RPC_UID_NCP_HASUID:              return rpc_ncp_hasUID_handler;

  case RPC_UID_HW_SETUARTBAUDRATE:      return rpc_hw_setUartBaudRate_handler;
  case RPC_UID_HW_INITUSERBUTTON:       return rpc_hw_initUserButton_handler;
  case RPC_UID_HW_INITLED:              return rpc_hw_initLED_handler;
  case RPC_UID_HW_INITRGB:              return rpc_hw_initRGB_handler;
  case RPC_UID_HW_INITARGB:             return rpc_hw_initARGB_handler;
  case RPC_UID_HW_SETLEDBRIGHTNESS:     return rpc_hw_setLedBrightness_handler;
  case RPC_UID_HW_GETWIFIMAC:           return rpc_hw_getWiFiMAC_handler;
  case RPC_UID_HW_GETETHERNETMAC:       return rpc_hw_getEthernetMAC_handler;

  case RPC_UID_BLYNK_INITIALIZE:        return rpc_blynk_initialize_handler;
  case RPC_UID_BLYNK_SETVENDORPREFIX:   return rpc_blynk_setVendorPrefix_handler;
  case RPC_UID_BLYNK_SETVENDORSERVER:   return rpc_blynk_setVendorServer_handler;
  case RPC_UID_BLYNK_SETFIRMWAREINFO:   return rpc_blynk_setFirmwareInfo_handler;
  case RPC_UID_BLYNK_GETNCPVERSION:     return rpc_blynk_getNcpVersion_handler;
  case RPC_UID_BLYNK_GETSTATE:          return rpc_blynk_getState_handler;
  case RPC_UID_BLYNK_GETHOTSPOTNAME:    return rpc_blynk_getHotspotName_handler;

  case RPC_UID_BLYNK_VIRTUALWRITE:      return rpc_blynk_virtualWrite_handler;
  case RPC_UID_BLYNK_SYNCALL:           return rpc_blynk_syncAll_handler;
  case RPC_UID_BLYNK_SYNCVIRTUAL:       return rpc_blynk_syncVirtual_handler;
  case RPC_UID_BLYNK_SETPROPERTY:       return rpc_blynk_setProperty_handler;
  case RPC_UID_BLYNK_BEGINGROUP:        return rpc_blynk_beginGroup_handler;
  case RPC_UID_BLYNK_ENDGROUP:          return rpc_blynk_endGroup_handler;

  case RPC_UID_BLYNK_LOGEVENT:          return rpc_blynk_logEvent_handler;
  case RPC_UID_BLYNK_RESOLVEEVENT:      return rpc_blynk_resolveEvent_handler;
  case RPC_UID_BLYNK_RESOLVEALLEVENTS:  return rpc_blynk_resolveAllEvents_handler;

  case RPC_UID_BLYNK_SETMETADATA:       return rpc_blynk_setMetadata_handler;

  case RPC_UID_BLYNK_CONFIGSTART:       return rpc_blynk_configStart_handler;
  case RPC_UID_BLYNK_CONFIGSTOP:        return rpc_blynk_configStop_handler;
  case RPC_UID_BLYNK_CONFIGRESET:       return rpc_blynk_configReset_handler;
  case RPC_UID_BLYNK_ISCONFIGURED:      return rpc_blynk_isConfigured_handler;
  case RPC_UID_BLYNK_SETCONFIGTIMEOUT:   return rpc_blynk_setConfigTimeout_handler;
  case RPC_UID_BLYNK_SETCONFIGSKIPLIMIT: return rpc_blynk_setConfigSkipLimit_handler;

  case RPC_UID_BLYNK_FACTORYRESET:       return rpc_blynk_factoryReset_handler;
  case RPC_UID_BLYNK_FACTORYTESTWIFI:    return rpc_blynk_factoryTestWiFi_handler;
  case RPC_UID_BLYNK_FACTORYTESTWIFIAP:  return rpc_blynk_factoryTestWiFiAP_handler;
  case RPC_UID_BLYNK_FACTORYTESTCONNECT: return rpc_blynk_factoryTestConnect_handler;

  case RPC_UID_BLYNK_OTAUPDATESTART:     return rpc_blynk_otaUpdateStart_handler;
  case RPC_UID_BLYNK_OTAUPDATEGETCRC32:  return rpc_blynk_otaUpdateGetCRC32_handler;
  case RPC_UID_BLYNK_OTAUPDATEGETMD5:    return rpc_blynk_otaUpdateGetMD5_handler;
  case RPC_UID_BLYNK_OTAUPDATEGETSHA256: return rpc_blynk_otaUpdateGetSHA256_handler;
  case RPC_UID_BLYNK_OTAUPDATEPREFETCH:  return rpc_blynk_otaUpdatePrefetch_handler;

  case RPC_UID_BLYNK_SETTIME:            return rpc_blynk_setTime_handler;
  case RPC_UID_BLYNK_GETTIME:            return rpc_blynk_getTime_handler;
  case RPC_UID_BLYNK_GETLOCATIONINFO:    return rpc_blynk_getLocationInfo_handler;

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

bool rpc_ncp_hasUID_impl(uint16_t uid) {
  return rpc_find_uid_handler(uid) != NULL;
}

#endif /* BLYNK_RPC_SERVER_H */
