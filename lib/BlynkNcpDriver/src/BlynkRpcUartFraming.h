#ifndef BLYNK_RPC_UART_FRAMING_H
#define BLYNK_RPC_UART_FRAMING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void    RpcUartFraming_init(void);

void    RpcUartFraming_beginPacket(void);
size_t  RpcUartFraming_write(const uint8_t *buffer, size_t size);
void    RpcUartFraming_endPacket(void);

int     RpcUartFraming_available(void);
int     RpcUartFraming_read(void);
bool    RpcUartFraming_finishedPacket(void);
bool    RpcUartFraming_checkPacketCRC(void);

#ifdef __cplusplus
}
#endif

#endif
