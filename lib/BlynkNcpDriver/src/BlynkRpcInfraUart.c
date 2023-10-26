
#include "BlynkRpc.h"
#include "BlynkRpcUartFraming.h"

static uint8_t  inputData[RPC_INPUT_BUFFER];
static unsigned inputDataLen;

bool rpc_recv_msg(MessageBuffer* buff, uint32_t timeout)
{
  MessageBuffer_reset(buff);
  const uint32_t tstart = rpc_system_millis();
  do {
    while (RpcUartFraming_available()) {
      // TODO: boundary check
      inputData[inputDataLen++] = RpcUartFraming_read();
    }

    if (RpcUartFraming_finishedPacket()) {
      unsigned packetSize = inputDataLen;
      inputDataLen = 0;

      //TRACE_HEX(">>", inputData, packetSize);
      if (RpcUartFraming_checkPacketCRC()) {
        MessageBuffer_setBuffer(buff, inputData, sizeof(inputData));
        MessageBuffer_setWritten(buff, packetSize);
        return true;
      } else {
        //LOG("NCP message CRC error");
      }
    }
  } while (rpc_system_millis() - tstart < timeout);
  return false;
}
