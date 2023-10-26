/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-10-26
 *
 */

#pragma once


// #define LILYGO_T_A7670
// #define LILYGO_T_CALL_A7670

#if defined(LILYGO_T_A7670)

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (25)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (27)
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_ADC_PIN                       (35)
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


#elif defined(LILYGO_T_CALL_A7670)
#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (14)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (25)
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_ADC_PIN                       (35)
#define BOARD_LED_PIN                       (12)
#define BOARD_POWERON_PIN                   (BOARD_LED_PIN)
#define MODEM_RING_PIN                      (13)
#define MODEM_RESET_PIN                     (27)
#define MODEM_RESET_LEVEL                   LOW
#define SerialAT                            Serial1

#else
#error "Use ArduinoIDE, please open the macro definition corresponding to the board above <utilities.h>"
#endif









