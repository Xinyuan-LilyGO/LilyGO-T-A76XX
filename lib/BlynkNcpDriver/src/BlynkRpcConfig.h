#ifndef BLYNK_RPC_CONFIG_H
#define BLYNK_RPC_CONFIG_H

#if !defined(RPC_TIMEOUT_DEFAULT)
  #define RPC_TIMEOUT_DEFAULT     1000
#endif

#if defined(RPC_INPUT_BUFFER)
  // Use the specified value
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  #define RPC_INPUT_BUFFER        256
#elif defined(LINUX) || defined(ESP32)
  #define RPC_INPUT_BUFFER        4096
#else
  #define RPC_INPUT_BUFFER        2048
#endif

#if defined(RPC_ENABLE_SMALL_CRC8)
  // Use the specified value
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  #define RPC_ENABLE_SMALL_CRC8   1
#else
  #define RPC_ENABLE_SMALL_CRC8   0
#endif

#endif /* BLYNK_RPC_CONFIG_H */

