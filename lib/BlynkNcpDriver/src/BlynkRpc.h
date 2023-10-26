#ifndef BLYNK_RPC_H
#define BLYNK_RPC_H

#include <stdint.h>
#include "BlynkRpcConfig.h"
#include "MessageBuffer.h"
#include "MessageWriter.h"

#define BLYNK_RPC_LIB_VERSION "0.6.3"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: RpcOpTypes are not completely random.
 * Avoid using "special bytes" of an underlying transport.
 * Specificaly for UART: `11`, `13`, `AA`, `BB`, `CC` are reserved.
 * RSVD items should also be unused.
 */

typedef enum RpcOpType {
  RPC_OP_INVOKE = 0x67BC,
  RPC_OP_RESULT = 0xA512,
  RPC_OP_ONEWAY = 0x1CF3,

  RPC_OP_RSVD0  = 0xD674,
  RPC_OP_RSVD1  = 0x56E1,
  RPC_OP_RSVD2  = 0xD8AB,
  RPC_OP_RSVD3  = 0x3CAE,
  RPC_OP_RSVD4  = 0xED0B,
  RPC_OP_RSVD5  = 0x1FB5,
} RpcOpType;

typedef enum RpcStatus {
  RPC_STATUS_OK             = 0x00,
  RPC_STATUS_ERROR_GENERIC  = 0x10,
  RPC_STATUS_ERROR_TIMEOUT  = 0x11,
  RPC_STATUS_ERROR_MEMORY   = 0x12,
  RPC_STATUS_ERROR_UID      = 0x13,
  RPC_STATUS_ERROR_ARGS_W   = 0x14,
  RPC_STATUS_ERROR_ARGS_R   = 0x15,
  RPC_STATUS_ERROR_RETS_W   = 0x16,
  RPC_STATUS_ERROR_RETS_R   = 0x17,
} RpcStatus;

typedef enum RpcUID {
  /* System */
  RPC_UID_NCP_PING                  = 0x0101,
  RPC_UID_NCP_REBOOT                = 0x0102,
  RPC_UID_NCP_HASUID                = 0x0103,

  RPC_UID_MCU_PING                  = 0x0181,
  RPC_UID_MCU_REBOOT                = 0x0182,
  RPC_UID_MCU_HASUID                = 0x0183,

  /* Hardware */
  RPC_UID_HW_SETUARTBAUDRATE        = 0x0201,

  RPC_UID_HW_INITUSERBUTTON         = 0x0211,

  RPC_UID_HW_INITLED                = 0x0221,
  RPC_UID_HW_INITRGB                = 0x0222,
  RPC_UID_HW_INITARGB               = 0x0223,
  RPC_UID_HW_SETLEDBRIGHTNESS       = 0x0224,

  RPC_UID_HW_GETWIFIMAC             = 0x0231,
  RPC_UID_HW_GETETHERNETMAC         = 0x0232,

  /* Blynk */
  RPC_UID_BLYNK_INITIALIZE          = 0x0301,
  RPC_UID_BLYNK_SETVENDORPREFIX     = 0x0302,
  RPC_UID_BLYNK_SETVENDORSERVER     = 0x0303,
  RPC_UID_BLYNK_SETFIRMWAREINFO     = 0x0304,
  RPC_UID_BLYNK_GETNCPVERSION       = 0x0305,
  RPC_UID_BLYNK_GETSTATE            = 0x0306,
  RPC_UID_BLYNK_GETHOTSPOTNAME      = 0x0307,

  RPC_UID_BLYNK_VIRTUALWRITE        = 0x0311,
  RPC_UID_BLYNK_SYNCALL             = 0x0312,
  RPC_UID_BLYNK_SYNCVIRTUAL         = 0x0313,
  RPC_UID_BLYNK_SETPROPERTY         = 0x0314,
  RPC_UID_BLYNK_BEGINGROUP          = 0x0315,
  RPC_UID_BLYNK_ENDGROUP            = 0x0316,

  RPC_UID_BLYNK_LOGEVENT            = 0x0317,
  RPC_UID_BLYNK_RESOLVEEVENT        = 0x0318,
  RPC_UID_BLYNK_RESOLVEALLEVENTS    = 0x0319,

  RPC_UID_BLYNK_SETMETADATA         = 0x031A,

  RPC_UID_BLYNK_CONFIGSTART         = 0x0321,
  RPC_UID_BLYNK_CONFIGSTOP          = 0x0322,
  RPC_UID_BLYNK_CONFIGRESET         = 0x0323,
  RPC_UID_BLYNK_ISCONFIGURED        = 0x0324,
  RPC_UID_BLYNK_SETCONFIGTIMEOUT    = 0x0325,
  RPC_UID_BLYNK_SETCONFIGSKIPLIMIT  = 0x0326,

  RPC_UID_BLYNK_FACTORYRESET        = 0x0331,
  RPC_UID_BLYNK_FACTORYTESTWIFI     = 0x0332,
  RPC_UID_BLYNK_FACTORYTESTWIFIAP   = 0x0333,
  RPC_UID_BLYNK_FACTORYTESTCONNECT  = 0x033A,

  RPC_UID_BLYNK_OTAUPDATESTART      = 0x0341,
  RPC_UID_BLYNK_OTAUPDATEGETCRC32   = 0x0342,
  RPC_UID_BLYNK_OTAUPDATEGETMD5     = 0x0343,
  RPC_UID_BLYNK_OTAUPDATEGETSHA256  = 0x0344,
  RPC_UID_BLYNK_OTAUPDATEPREFETCH   = 0x0345,

  RPC_UID_BLYNK_SETTIME             = 0x0351,
  RPC_UID_BLYNK_GETTIME             = 0x0352,
  RPC_UID_BLYNK_GETLOCATIONINFO     = 0x0353,

  /* Client API */
  RPC_UID_CLIENT_BLYNKSTATECHANGE   = 0x0411,
  RPC_UID_CLIENT_BLYNKVPINCHANGE    = 0x0412,

  RPC_UID_CLIENT_PROCESSEVENT       = 0x0421,

  RPC_UID_CLIENT_OTAUPDATEAVAILABLE = 0x0441,
  RPC_UID_CLIENT_OTAUPDATEWRITE     = 0x0442,
  RPC_UID_CLIENT_OTAUPDATEFINISH    = 0x0443,
  RPC_UID_CLIENT_OTAUPDATECANCEL    = 0x0444,
} RpcUID;

typedef enum RpcBlynkState {
  BLYNK_STATE_UNKNOWN           = 0,

  BLYNK_STATE_IDLE              = 1,
  BLYNK_STATE_CONFIG            = 2,
  BLYNK_STATE_CONNECTING_NET    = 3,
  BLYNK_STATE_CONNECTING_CLOUD  = 4,
  BLYNK_STATE_CONNECTED         = 5,

  BLYNK_STATE_NOT_INITIALIZED   = 10,
  BLYNK_STATE_OTA_UPGRADE       = 11,
  BLYNK_STATE_ERROR             = 12,
} RpcBlynkState;

typedef enum RpcEvent {
  /* Graceful reboot of the NCP */
  RPC_EVENT_NCP_REBOOTING       = 1,

  /* User button events, relevant only
   * if rpc_hw_initUserButton() is used */
  RPC_EVENT_HW_USER_CLICK       = 10,
  RPC_EVENT_HW_USER_DBLCLICK    = 11,
  RPC_EVENT_HW_USER_LONGPRESS   = 12,
  RPC_EVENT_HW_USER_LONGRELEASE = 13,

  RPC_EVENT_HW_USER_RESET_START  = 16,
  RPC_EVENT_HW_USER_RESET_CANCEL = 17,
  RPC_EVENT_HW_USER_RESET_DONE   = 18,

  /* Happens each time the device is provisioned
   * and connected to the cloud */
  RPC_EVENT_BLYNK_PROVISIONED   = 20,
  /* Time sync is requested during prolonged offline periods */
  RPC_EVENT_BLYNK_TIME_SYNC     = 21,
  /* Happens when time or location is changed */
  RPC_EVENT_BLYNK_TIME_CHANGED  = 22,
} RpcEvent;

typedef enum RpcOtaPrefetchStatus {
  RPC_OTA_PREFETCH_OK               = 1,
  RPC_OTA_PREFETCH_GENERIC_ERROR    = 2,
  RPC_OTA_PREFETCH_CONNECT_FAILED   = 3,
  RPC_OTA_PREFETCH_REQUEST_FAILED   = 4,
  RPC_OTA_PREFETCH_DOWNLOAD_TIMEOUT = 5,
  RPC_OTA_PREFETCH_FS_NO_SPACE      = 6,
  RPC_OTA_PREFETCH_FS_WRITE_FAILED  = 7,
  RPC_OTA_PREFETCH_FINALIZE_FAILED  = 8,
} RpcOtaPrefetchStatus;

typedef enum RpcFactoryTestStatus {
  RPC_FACTORY_TEST_OK           = 1,
  RPC_FACTORY_TEST_INVALID_ARGS = 2,
  RPC_FACTORY_TEST_SYSTEM_FAIL  = 3,
  RPC_FACTORY_TEST_WIFI_FAIL    = 4,
  RPC_FACTORY_TEST_LOW_RSSI     = 5,
  RPC_FACTORY_TEST_INET_FAIL    = 6,
} RpcFactoryTestStatus;

#define RPC_ATTR_WEAK           __attribute__ ((weak))
#define RPC_ATTR_DEPRECATED     __attribute__ ((deprecated))

typedef void (*rpc_handler_t)(MessageBuffer*);

void          rpc_set_status(RpcStatus status);
RpcStatus     rpc_get_status(void);
const char*   rpc_get_status_str(RpcStatus status);
void          rpc_set_timeout(uint32_t ms);
uint32_t      rpc_get_last_rx(void);
uint32_t      rpc_get_last_tx(void);
uint16_t      rpc_next_seq(void);
RpcStatus     rpc_invoke_handler(uint16_t id, MessageBuffer* buff);
bool          rpc_recv_msg(MessageBuffer* buff, uint32_t timeout);
RpcStatus     rpc_wait_result(uint16_t expected_seq, MessageBuffer* buff, uint32_t timeout);
void          rpc_run(void);

uint32_t      rpc_system_millis(void);
int           rpc_uart_available(void);
int           rpc_uart_read(void);
size_t        rpc_uart_write(uint8_t data);
void          rpc_uart_flush(void);

#ifdef __cplusplus
}
#endif

#endif /* BLYNK_RPC_H */
