/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-15
 *
 */

#pragma once

// Note:
// When using ArduinoIDE, you must select a corresponding board type.
// If you don’t know which board type you have, please click on the link to view it.
// 使用ArduinoIDE ,必须选择一个对应的板型 ,如果你不知道你的板型是哪种，请点击链接进行查看

// Products Link:https://www.lilygo.cc/products/t-sim-a7670e
// #define LILYGO_T_A7670

// #define LILYGO_T_CALL_A7670

// #define LILYGO_T_SIM7672G_S3

// #define LILYGO_T_A7608X

// #define LILYGO_T_A7608X_S3

// #define LILYGO_T_A7608X_DC_S3





#if defined(LILYGO_T_A7670)

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (25)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (27)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_ADC_PIN                       (35)
// The modem power switch must be set to HIGH for the modem to supply power.
#define BOARD_POWERON_PIN                   (12)
#define MODEM_RING_PIN                      (33)
#define MODEM_RESET_PIN                     (5)
#define BOARD_MISO_PIN                      (2)
#define BOARD_MOSI_PIN                      (15)
#define BOARD_SCK_PIN                       (14)
#define BOARD_SD_CS_PIN                     (13)
#define BOARD_BAT_ADC_PIN                   (35)
#define MODEM_RESET_LEVEL                   HIGH
#define SerialAT                            Serial1

#define MODEM_GPS_ENABLE_GPIO               (-1)


#ifndef TINY_GSM_MODEM_A7670
#define TINY_GSM_MODEM_A7670
#endif

#elif defined(LILYGO_T_CALL_A7670)
#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (14)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (25)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_LED_PIN                       (12)
// There is no modem power control, the LED Pin is used as a power indicator here.
#define BOARD_POWERON_PIN                   (BOARD_LED_PIN)
#define MODEM_RING_PIN                      (13)
#define MODEM_RESET_PIN                     (27)
#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1


#define MODEM_GPS_ENABLE_GPIO               (-1)


#ifndef TINY_GSM_MODEM_A7670
#define TINY_GSM_MODEM_A7670
#endif

#elif defined(LILYGO_T_SIM7672G_S3)
#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (9)
#define MODEM_TX_PIN                        (11)
#define MODEM_RX_PIN                        (10)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (18)
#define BOARD_LED_PIN                       (12)
// There is no modem power control, the LED Pin is used as a power indicator here.
#define BOARD_POWERON_PIN                   (BOARD_LED_PIN)
#define MODEM_RING_PIN                      (3)
#define MODEM_RESET_PIN                     (17)
#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1

#define BOARD_BAT_ADC_PIN                   (4)
#define BOARD_SOLAR_ADC_PIN                 (5)
#define BOARD_MISO_PIN                      (47)
#define BOARD_MOSI_PIN                      (14)
#define BOARD_SCK_PIN                       (21)
#define BOARD_SD_CS_PIN                     (13)

#ifndef TINY_GSM_MODEM_SIM7672
#define TINY_GSM_MODEM_SIM7672
#endif

#define MODEM_GPS_ENABLE_GPIO               (4)


#elif defined(LILYGO_T_A7608X)

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (25)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (27)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_BAT_ADC_PIN                   (35)
// The modem power switch must be set to HIGH for the modem to supply power.
#define BOARD_POWERON_PIN                   (12)
#define MODEM_RING_PIN                      (33)
#define MODEM_RESET_PIN                     (5)
#define BOARD_MISO_PIN                      (2)
#define BOARD_MOSI_PIN                      (15)
#define BOARD_SCK_PIN                       (14)
#define BOARD_SD_CS_PIN                     (13)

#define MODEM_RESET_LEVEL                   HIGH
#define SerialAT                            Serial1


#ifndef TINY_GSM_MODEM_A7608
#define TINY_GSM_MODEM_A7608
#endif

// 127 is defined in GSM as the AUXVDD index
#define MODEM_GPS_ENABLE_GPIO               (127)

#elif defined(LILYGO_T_A7608X_S3)

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (7)
#define MODEM_TX_PIN                        (17)
#define MODEM_RX_PIN                        (18)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (15)
#define BOARD_BAT_ADC_PIN                   (35)
// The modem power switch must be set to HIGH for the modem to supply power.
#define BOARD_POWERON_PIN                   (12)
#define MODEM_RING_PIN                      (6)
#define MODEM_RESET_PIN                     (16)
#define BOARD_MISO_PIN                      (47)
#define BOARD_MOSI_PIN                      (14)
#define BOARD_SCK_PIN                       (21)
#define BOARD_SD_CS_PIN                     (13)

#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1

#ifndef TINY_GSM_MODEM_A7608
#define TINY_GSM_MODEM_A7608
#endif

// 127 is defined in GSM as the AUXVDD index
#define MODEM_GPS_ENABLE_GPIO               (127)

#elif defined(LILYGO_T_A7608X_DC_S3)

#define MODEM_DTR_PIN                       (5)
#define MODEM_RX_PIN                        (42)
#define MODEM_TX_PIN                        (41)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (38)
#define MODEM_RING_PIN                      (6)
#define MODEM_RESET_PIN                     (40)
#define MODEM_RTS_PIN                       (1)
#define MODEM_CTS_PIN                       (2)

#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1

#ifndef TINY_GSM_MODEM_A7608
#define TINY_GSM_MODEM_A7608
#endif

// 127 is defined in GSM as the AUXVDD index
#define MODEM_GPS_ENABLE_GPIO               (127)

#else
#error "Use ArduinoIDE, please open the macro definition corresponding to the board above <utilities.h>"
#endif









