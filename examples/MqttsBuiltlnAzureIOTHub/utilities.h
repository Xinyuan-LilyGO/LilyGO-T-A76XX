/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2025-04-30
 *
 */

#pragma once

// Note:

// When using ArduinoIDE, you must select a corresponding board type.
// If you don’t know which board type you have, please click on the link to view it.
// 使用ArduinoIDE ,必须选择一个对应的板型 ,如果你不知道你的板型是哪种，请点击链接进行查看

// The model name with S3 after it is the ESP32-S3 version, otherwise it is the ESP32 version
// 型号名称后面带S3的为ESP32-S3版本，否则是ESP32版本


// Products Link:https://www.lilygo.cc/products/t-sim-a7670e
// #define LILYGO_T_A7670

// Products Link: https://lilygo.cc/products/t-call-v1-4
// #define LILYGO_T_CALL_A7670_V1_0

// Products Link: https://lilygo.cc/products/t-sim-7670g-s3
// #define LILYGO_T_SIM7670G_S3

// Products Link: https://lilygo.cc/products/t-a7608e-h?variant=42860532433077
// #define LILYGO_T_A7608X

// Products Link: https://lilygo.cc/products/t-a7608e-h?variant=43932699033781
// #define LILYGO_T_A7608X_S3

// Products Link: https://lilygo.cc/products/t-sim7000g
// #define LILYGO_SIM7000G

// Products Link: https://lilygo.cc/products/a-t-pcie?variant=42335922094261
// #define LILYGO_T_PCIE_A767X

// Products Link: https://lilygo.cc/products/a-t-pcie?variant=42335921897653
// #define LILYGO_T_PCIE_SIM7000G

// Products Link: https://lilygo.cc/products/a-t-pcie?variant=42335921995957
// #define LILYGO_T_PCIE_SIM7080G

// Products Link: ......
// #define LILYGO_T_PCIE_SIM7670G

// Products Link: https://lilygo.cc/products/t-eth-elite-1?variant=44498205049013
// #define LILYGO_T_ETH_ELITE_A7670X

// https://lilygo.cc/products/t-sim7600
// #define LILYGO_SIM7600X

// Products Link: ......
// #define LILYGO_T_RELAY_S3_SIMSHIELD


// SIMCOM standard interface series
// #define LILYGO_SIM7000G_S3_STAN
// #define LILYGO_SIM7080G_S3_STAN
// #define LILYGO_SIM7670G_S3_STAN
// #define LILYGO_A7670X_S3_STAN



#if defined(LILYGO_T_A7670)

    #define MODEM_BAUDRATE                      (115200)
    #define MODEM_DTR_PIN                       (25)
    #define MODEM_TX_PIN                        (26)
    #define MODEM_RX_PIN                        (27)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (4)
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
    #define MODEM_GPS_ENABLE_LEVEL              (-1)

    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif

    // It is only available in V1.4 version. In other versions, IO36 is not connected.
    #define BOARD_SOLAR_ADC_PIN                 (36)


    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (23)
    #define SIMSHIELD_MISO                      (19)
    #define SIMSHIELD_SCK                       (18)
    #define SIMSHIELD_SD_CS                     (32)
    #define SIMSHIELD_RADIO_BUSY                (39)
    #define SIMSHIELD_RADIO_CS                  (5)
    #define SIMSHIELD_RADIO_IRQ                 (34)
    #define SIMSHIELD_RADIO_RST                 (15)
    #define SIMSHIELD_RS_RX                     (13)
    #define SIMSHIELD_RS_TX                     (14)
    #define SIMSHIELD_SDA                       (21)
    #define SIMSHIELD_SCL                       (22)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-A7670 ESP32 Version"

#elif defined(LILYGO_T_CALL_A7670_V1_0)
    #define MODEM_BAUDRATE                      (115200)
    #define MODEM_DTR_PIN                       (14)
    #define MODEM_TX_PIN                        (26)
    #define MODEM_RX_PIN                        (25)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (4)
    #define BOARD_LED_PIN                       (12)
    #define LED_ON                              HIGH
    #define MODEM_RING_PIN                      (13)
    #define MODEM_RESET_PIN                     (27)
    #define MODEM_RESET_LEVEL                   LOW
    #define SerialAT                            Serial1


    #define MODEM_GPS_ENABLE_GPIO               (-1)
    #define MODEM_GPS_ENABLE_LEVEL              (-1)


    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif

    #define PRODUCT_MODEL_NAME                  "LilyGo-T-Call A7670 V1.0"

#elif defined(LILYGO_T_CALL_A7670_V1_1)

    #define MODEM_BAUDRATE                      (115200)
    #define MODEM_DTR_PIN                       (32)
    #define MODEM_TX_PIN                        (27)
    #define MODEM_RX_PIN                        (26)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (4)
    #define BOARD_LED_PIN                       (13)
    #define LED_ON                              HIGH
    // There is no modem power control, the LED Pin is used as a power indicator here.
    #define MODEM_RING_PIN                      (33)
    #define MODEM_RESET_PIN                     (5)
    #define MODEM_RESET_LEVEL                   LOW
    #define SerialAT                            Serial1

    #define MODEM_GPS_ENABLE_GPIO               (-1)
    #define MODEM_GPS_ENABLE_LEVEL              (-1)

    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif

    #define PRODUCT_MODEL_NAME                  "LilyGo-T-Call A7670 V1.1"

#elif defined(LILYGO_T_SIM7670G_S3)
    #define MODEM_BAUDRATE                      (115200)
    #define MODEM_DTR_PIN                       (9)
    #define MODEM_TX_PIN                        (11)
    #define MODEM_RX_PIN                        (10)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (18)
    #define BOARD_LED_PIN                       (12)
    #define LED_ON                              (LOW)
    // There is no modem power control, the LED Pin is used as a power indicator here.
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

    #ifndef TINY_GSM_MODEM_SIM7670G
        #define TINY_GSM_MODEM_SIM7670G
    #endif

    #define MODEM_GPS_ENABLE_GPIO               (4)
    #define MODEM_GPS_ENABLE_LEVEL              (1)

    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (15)
    #define SIMSHIELD_MISO                      (7)
    #define SIMSHIELD_SCK                       (16)
    #define SIMSHIELD_SD_CS                     (46)
    #define SIMSHIELD_RADIO_BUSY                (38)
    #define SIMSHIELD_RADIO_CS                  (39)
    #define SIMSHIELD_RADIO_IRQ                 (6)
    #define SIMSHIELD_RADIO_RST                 (40)
    #define SIMSHIELD_RS_RX                     (41)
    #define SIMSHIELD_RS_TX                     (42)
    #define SIMSHIELD_SDA                       (2)
    #define SIMSHIELD_SCL                       (1)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7670G-S3"

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

    // only version v1.1 or V2  has solar adc pin
    #define BOARD_SOLAR_ADC_PIN                 (34)


    // 127 is defined in GSM as the AUXVDD index
    #define MODEM_GPS_ENABLE_GPIO               (127)
    #define MODEM_GPS_ENABLE_LEVEL              (1)

    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (23)
    #define SIMSHIELD_MISO                      (19)
    #define SIMSHIELD_SCK                       (18)
    #define SIMSHIELD_SD_CS                     (32)
    #define SIMSHIELD_RADIO_BUSY                (39)
    #define SIMSHIELD_RADIO_CS                  (5)
    #define SIMSHIELD_RADIO_IRQ                 (34)
    #define SIMSHIELD_RADIO_RST                 (15)
    #define SIMSHIELD_RS_RX                     (13)
    #define SIMSHIELD_RS_TX                     (14)
    #define SIMSHIELD_SDA                       (21)
    #define SIMSHIELD_SCL                       (22)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-A7608X ESP32 Version"

#elif defined(LILYGO_T_A7608X_S3)

    #define MODEM_BAUDRATE                      (115200)
    #define MODEM_DTR_PIN                       (7)
    #define MODEM_TX_PIN                        (17)
    #define MODEM_RX_PIN                        (18)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (15)
    #define BOARD_BAT_ADC_PIN                   (4)
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

    // only version v1.1 has solar adc pin
    #define BOARD_SOLAR_ADC_PIN                 (3)

    // 127 is defined in GSM as the AUXVDD index
    #define MODEM_GPS_ENABLE_GPIO               (127)
    #define MODEM_GPS_ENABLE_LEVEL              (1)

    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (11)
    #define SIMSHIELD_MISO                      (10)
    #define SIMSHIELD_SCK                       (12)
    #define SIMSHIELD_SD_CS                     (45)
    #define SIMSHIELD_RADIO_BUSY                (38)
    #define SIMSHIELD_RADIO_CS                  (39)
    #define SIMSHIELD_RADIO_IRQ                 (9)
    #define SIMSHIELD_RADIO_RST                 (40)
    #define SIMSHIELD_RS_RX                     (41)
    #define SIMSHIELD_RS_TX                     (42)
    #define SIMSHIELD_SDA                       (2)
    #define SIMSHIELD_SCL                       (1)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-A7608X-S3"

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
    #define MODEM_GPS_ENABLE_LEVEL              (1)

    #define PRODUCT_MODEL_NAME                  "LilyGo-A7608X-DC-S3"

#elif defined(LILYGO_SIM7000G)

    #define MODEM_DTR_PIN                       (25)
    #define MODEM_RX_PIN                        (26)
    #define MODEM_TX_PIN                        (27)

    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (4)
    #define BOARD_LED_PIN                       (12)
    #define LED_ON                              (LOW)

    #define BOARD_MISO_PIN                      (2)
    #define BOARD_MOSI_PIN                      (15)
    #define BOARD_SCK_PIN                       (14)
    #define BOARD_SD_CS_PIN                     (13)
    
    #define BOARD_BAT_ADC_PIN                   (35)
    #define BOARD_SOLAR_ADC_PIN                 (36)
    
    #define SerialAT                            Serial1

    #ifndef TINY_GSM_MODEM_SIM7000SSL
        #define TINY_GSM_MODEM_SIM7000SSL
    #endif

    // 127 is defined in GSM as the AUXVDD index
    #define MODEM_GPS_ENABLE_GPIO               (48)
    #define MODEM_GPS_ENABLE_LEVEL              (1)
    
    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (23)
    #define SIMSHIELD_MISO                      (19)
    #define SIMSHIELD_SCK                       (18)
    #define SIMSHIELD_SD_CS                     (32)
    #define SIMSHIELD_RADIO_BUSY                (39)
    #define SIMSHIELD_RADIO_CS                  (5)
    #define SIMSHIELD_RADIO_IRQ                 (34)
    #define SIMSHIELD_RADIO_RST                 (15)
    #define SIMSHIELD_RS_RX                     (13)
    #define SIMSHIELD_RS_TX                     (14)
    #define SIMSHIELD_SDA                       (21)
    #define SIMSHIELD_SCL                       (22)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7000G ESP32 Version"

#elif defined(LILYGO_SIM7600X)

    #define MODEM_DTR_PIN                       (32)
    #define MODEM_RX_PIN                        (26)
    #define MODEM_TX_PIN                        (27)
    #define MODEM_FLIGHT_PIN                    (25)
    #define MODEM_STATUS_PIN                    (34)

    #define BOARD_MISO_PIN                      (2)
    #define BOARD_MOSI_PIN                      (15)
    #define BOARD_SCK_PIN                       (14)
    #define BOARD_SD_CS_PIN                     (13)

    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (4)
    #define MODEM_RING_PIN                      (33)
    #define BOARD_LED_PIN                       (12)
    #define LED_ON                              (HIGH)

    #define BOARD_BAT_ADC_PIN                   (35)
    #define BOARD_SOLAR_ADC_PIN                 (36)

    #define SerialAT                            Serial1

    #ifndef TINY_GSM_MODEM_SIM7600
        #define TINY_GSM_MODEM_SIM7600
    #endif

    // 127 is defined in GSM as the AUXVDD index
    #define MODEM_GPS_ENABLE_GPIO               (127)
    #define MODEM_GPS_ENABLE_LEVEL              (1)

    // T-SIM7600 startup time needs to wait
    #define MODEM_START_WAIT_MS                 (15000)



    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (23)
    #define SIMSHIELD_MISO                      (19)
    #define SIMSHIELD_SCK                       (18)
    #define SIMSHIELD_SD_CS                     (32)
    #define SIMSHIELD_RADIO_BUSY                (39)
    #define SIMSHIELD_RADIO_CS                  (5)
    #define SIMSHIELD_RADIO_IRQ                 (34)
    #define SIMSHIELD_RADIO_RST                 (14)
    #define SIMSHIELD_RS_RX                     (12)
    #define SIMSHIELD_RS_TX                     (13)
    #define SIMSHIELD_SDA                       (21)
    #define SIMSHIELD_SCL                       (22)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7600X ESP32 Version"

#elif defined(LILYGO_SIM7000G_S3_STAN) || defined(LILYGO_SIM7080G_S3_STAN) \
    || defined(LILYGO_SIM7670G_S3_STAN) || defined(LILYGO_A7670X_S3_STAN)   \
    || defined(LILYGO_SIM7600X_S3_STAN)

    #define MODEM_BAUDRATE                      (115200)

    // Default I2C communication pin, cannot be changed
    #define BOARD_SDA_PIN                       (3)
    #define BOARD_SCL_PIN                       (2)

    #define MODEM_DTR_PIN                       (7)
    #define MODEM_TX_PIN                        (4) 
    #define MODEM_RX_PIN                        (5) 
    #define MODEM_RING_PIN                      (6)

    /*
    * GPS communication pin. If the modem has GPS function, 
    * NMEA data can be read through this IO. 
    * If an external GPS module is used, the same GPIO is used for communication.
    * */
    #define MODEM_GPS_RX_PIN                    (48)
    #define MODEM_GPS_TX_PIN                    (45)
    #define MODEM_GPS_PPS_PIN                   (17)

    // This IO is only used when using an external GPS module, such as A7670G+L76K GPS
    #define GPS_SHIELD_WAKEUP_PIN               (0)

    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN                    (46)

    // Analog pins are already connected to the ADC voltage divider circuit and cannot be used for other purposes
    #define BOARD_BAT_ADC_PIN                   (8)
    #define BOARD_SOLAR_ADC_PIN                 (18)

    // SD Socket pins
    #define BOARD_MISO_PIN                      (13)
    #define BOARD_MOSI_PIN                      (11)
    #define BOARD_SCK_PIN                       (12)
    #define BOARD_SD_CS_PIN                     (10)

    // Enable / disable power save mode (1 disabled, 0 enabled)
    #define BOARD_POWER_SAVE_MODE_PIN           (42)

    // CAMERA_XXX_PIN is routed to the pin header and camera connector
    #define CAMERA_PWDN_PIN                     (-1)
    #define CAMERA_RESET_PIN                    (-1)
    #define CAMERA_XCLK_PIN                     (21)
    #define CAMERA_SIOD_PIN                     (1)
    #define CAMERA_SIOC_PIN                     (41)
    #define CAMERA_VSYNC_PIN                    (9)
    #define CAMERA_HREF_PIN                     (14)
    #define CAMERA_PCLK_PIN                     (37)
    #define CAMERA_Y9_PIN                       (40)
    #define CAMERA_Y8_PIN                       (39)
    #define CAMERA_Y7_PIN                       (38)
    #define CAMERA_Y6_PIN                       (36)
    #define CAMERA_Y5_PIN                       (16)
    #define CAMERA_Y4_PIN                       (47)
    #define CAMERA_Y3_PIN                       (15)
    #define CAMERA_Y2_PIN                       (35)

    #define SerialAT                            Serial1
   
    
    // This board has connected the ADC circuit to the modem, and the battery voltage can be read via AT+CBC
    #define MODEM_CONNECTED_ADC_PIN

    #ifdef LILYGO_SIM7000G_S3_STAN
        // Modem model:SIM7000G
        #define TINY_GSM_MODEM_SIM7000SSL
        // GPS antenna power control GPIO, this GPIO is the modem GPIO
        #define MODEM_GPS_ENABLE_GPIO               (48)
        #define MODEM_GPS_ENABLE_LEVEL              (1)

        #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7000G-S3-Standard"
    #endif

    #ifdef LILYGO_SIM7080G_S3_STAN
        // Modem model:SIM7080G
        #define TINY_GSM_MODEM_SIM7080
        // GPS antenna power control GPIO, this GPIO is the modem GPIO
        #define MODEM_GPS_ENABLE_GPIO               (5)
        #define MODEM_GPS_ENABLE_LEVEL              (1)

        #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7080G-S3-Standard"
    #endif

    #ifdef LILYGO_SIM7670G_S3_STAN
        // Modem model:SIM7670G
        #define TINY_GSM_MODEM_SIM7670G
        // GPS antenna power control GPIO, this GPIO is the modem GPIO
        #define MODEM_GPS_ENABLE_GPIO               (1)
        #define MODEM_GPS_ENABLE_LEVEL              (1)
        #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7670G-S3-Standard"
    #endif

    #ifdef LILYGO_A7670X_S3_STAN
        // Modem model:A7670G/A7670E/A7670SA
        #define TINY_GSM_MODEM_A7670
        // GPS antenna power control GPIO, this GPIO is the modem GPIO
        #define MODEM_GPS_ENABLE_GPIO               (1)
        #define MODEM_GPS_ENABLE_LEVEL              (1)
        #define MODEM_AUDIO_PA_ENABLE_GPIO          (3)
        #define MODEM_AUDIO_PA_ENABLE_LEVEL         (1)
        #define PRODUCT_MODEL_NAME                  "LilyGo-A7670X-S3-Standard"
    #endif

    #ifdef LILYGO_SIM7600X_S3_STAN
        // Modem model:SIM7600E/A/SA/G
        #define TINY_GSM_MODEM_SIM7600
        // MODEM aux power supply
        #define MODEM_GPS_ENABLE_GPIO               (127)
        #define MODEM_GPS_ENABLE_LEVEL              (1)
        #define MODEM_AUDIO_PA_ENABLE_GPIO          (77)
        #define MODEM_AUDIO_PA_ENABLE_LEVEL         (1)
        #define PRODUCT_MODEL_NAME                  "LilyGo-SIM7600X-S3-Standard"
    #endif

#elif defined(LILYGO_SIM7080G_S3)

    #define SerialAT                    Serial1
    #define MODEM_BAUDRATE              (115200)

    #define PWDN_GPIO_NUM               (-1)
    #define RESET_GPIO_NUM              (18)
    #define XCLK_GPIO_NUM               (8)
    #define SIOD_GPIO_NUM               (2)
    #define SIOC_GPIO_NUM               (1)
    #define VSYNC_GPIO_NUM              (16)
    #define HREF_GPIO_NUM               (17)
    #define PCLK_GPIO_NUM               (12)
    #define Y9_GPIO_NUM                 (9)
    #define Y8_GPIO_NUM                 (10)
    #define Y7_GPIO_NUM                 (11)
    #define Y6_GPIO_NUM                 (13)
    #define Y5_GPIO_NUM                 (21)
    #define Y4_GPIO_NUM                 (48)
    #define Y3_GPIO_NUM                 (47)
    #define Y2_GPIO_NUM                 (14)

    #define I2C_SDA                     (15)
    #define I2C_SCL                     (7)

    #define PMU_INPUT_PIN               (6)

    #define BUTTON_CONUT                (1)
    #define USER_BUTTON_PIN             (0)
    #define BUTTON_ARRAY                {USER_BUTTON_PIN}

    #define BOARD_PWRKEY_PIN            (41)
    #define MODEM_DTR_PIN               (42)
    #define MODEM_RING_PIN              (3)
    #define MODEM_RX_PIN                (4)
    #define MODEM_TX_PIN                (5)

    #define SDMMC_CMD                   (39)
    #define SDMMC_CLK                   (38)
    #define SDMMC_DATA                  (40)

    #ifndef TINY_GSM_MODEM_SIM7080G
        #define TINY_GSM_MODEM_SIM7080G
    #endif

    //! The following pins are for SimShield and need to be used with SimShield
    //! 以下引脚针对SimShield,需要搭配SimShield 
    #define SIMSHIELD_MOSI                      (11)
    #define SIMSHIELD_MISO                      (13)
    #define SIMSHIELD_SCK                       (12)
    #define SIMSHIELD_SD_CS                     (21)
    #define SIMSHIELD_RADIO_BUSY                (48)
    #define SIMSHIELD_RADIO_CS                  (45)
    #define SIMSHIELD_RADIO_IRQ                 (8)
    #define SIMSHIELD_RADIO_RST                 (47)
    #define SIMSHIELD_RS_RX                     (2)
    #define SIMSHIELD_RS_TX                     (1)
    #define SIMSHIELD_SDA                       (44)
    #define SIMSHIELD_SCL                       (43)
    #define SerialRS485                         Serial2

    #define PRODUCT_MODEL_NAME                  "LilyGo-T-SIM7080G-S3-PMU"

#elif defined(LILYGO_T_RELAY_S3_SIMSHIELD)

    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif

    #define BOARD_PWRKEY_PIN                    46
    #define MODEM_RX_PIN                        9
    #define MODEM_TX_PIN                        3
    #define MODEM_DTR_PIN                       11
    #define MODEM_RING_PIN                      13

    #define SerialAT                            Serial1
    #define PRODUCT_MODEL_NAME                  "LilyGo-T-RelayShield"

#elif defined(LILYGO_T_PCIE_A767X)

    #define LILYGO_T_PCIE

    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif

    // Modem GPIO 4 control gps enable
    #define MODEM_GPS_ENABLE_GPIO               (4)
    #define MODEM_GPS_ENABLE_LEVEL              (0)
    #define PRODUCT_MODEL_NAME                  "LilyGo-T-PCIE-A7670X"

#elif defined(LILYGO_T_PCIE_SIM7000G)

    #define LILYGO_T_PCIE

    #ifndef TINY_GSM_MODEM_SIM7000SSL
        #define TINY_GSM_MODEM_SIM7000SSL
    #endif

    // Modem Physical pins 48 control gps enable
    #define MODEM_GPS_ENABLE_GPIO               (48)
    #define MODEM_GPS_ENABLE_LEVEL              (1)
    
    #define PRODUCT_MODEL_NAME                  "LilyGo-T-PCIE-SIM7000G"

#elif defined(LILYGO_T_PCIE_SIM7080G)

    #define LILYGO_T_PCIE

    #ifndef TINY_GSM_MODEM_SIM7080
        #define TINY_GSM_MODEM_SIM7080
    #endif

    #define PRODUCT_MODEL_NAME                  "LilyGo-T-PCIE-SIM7080G"

#elif defined(LILYGO_T_PCIE_SIM7600X)

    #define LILYGO_T_PCIE

    #ifndef TINY_GSM_MODEM_SIM7600
        #define TINY_GSM_MODEM_SIM7600
    #endif

    #define PRODUCT_MODEL_NAME                  "LilyGo-T-PCIE-SIM7600X"

#elif defined(LILYGO_T_PCIE_SIM7670G)

    #define LILYGO_T_PCIE

    #ifndef TINY_GSM_MODEM_SIM7670G
        #define TINY_GSM_MODEM_SIM7670G
    #endif
    // Modem GPIO 4 control gps enable
    #define MODEM_GPS_ENABLE_GPIO               (4)
    #define MODEM_GPS_ENABLE_LEVEL              (1)
    #define PRODUCT_MODEL_NAME                  "LilyGo-T-PCIE-SIM7670G"


#elif defined(LILYGO_T_ETH_ELITE_A7670X)

    #define LILYGO_T_ETH_ELITE

    #ifndef TINY_GSM_MODEM_A7670
        #define TINY_GSM_MODEM_A7670
    #endif


    // Modem GPIO 4 control gps enable
    #define MODEM_GPS_ENABLE_GPIO               (4)
    #define MODEM_GPS_ENABLE_LEVEL              (0)
    #define PRODUCT_MODEL_NAME                  "LilyGo-T-ETH-Elite-A7670X"

#else
    #error "Use ArduinoIDE, please open the macro definition corresponding to the board above <utilities.h>"
#endif


#ifdef LILYGO_T_PCIE

    #define MODEM_DTR_PIN           (32)
    #define MODEM_RX_PIN            (26)
    #define MODEM_TX_PIN            (27)
    // The modem power switch must be set to HIGH for the modem to supply power.
    #define BOARD_POWERON_PIN       (25)
    // The modem boot pin needs to follow the startup sequence.
    #define BOARD_PWRKEY_PIN        (4)
    #define MODEM_RING_PIN          (33)
    #define BOARD_LED_PIN           (12)
    #define PMU_IRQ                 (35)

    #define LED_ON                  (LOW)

    #define SerialAT                Serial1

    #ifndef MODEM_GPS_ENABLE_GPIO
        #define MODEM_GPS_ENABLE_GPIO               (-1)
    #endif
    #ifndef MODEM_GPS_ENABLE_LEVEL
        #define MODEM_GPS_ENABLE_LEVEL              (-1)
    #endif
    
#endif

#ifdef LILYGO_T_ETH_ELITE

    #define ETH_MISO_PIN             (47)
    #define ETH_MOSI_PIN             (21)
    #define ETH_SCLK_PIN             (48)
    #define ETH_CS_PIN               (45)
    #define ETH_INT_PIN              (14)
    #define ETH_RST_PIN              (-1)
    #define ETH_ADDR                 (1)

    #define SPI_MISO_PIN             (9)
    #define SPI_MOSI_PIN             (11)
    #define SPI_SCLK_PIN             (10)

    #define SD_MISO_PIN              (SPI_MISO_PIN)
    #define SD_MOSI_PIN              (SPI_MOSI_PIN)
    #define SD_SCLK_PIN              (SPI_SCLK_PIN)
    #define SD_CS_PIN                (12)

    #define BOARD_SDA_PIN             (17)
    #define BOARD_SCL_PIN             (18)

    #define ADC_BUTTONS_PIN           (7)
    #define MODEM_RX_PIN              (4)
    #define MODEM_TX_PIN              (6)
    #define MODEM_DTR_PIN             (5)
    #define MODEM_RING_PIN            (1)
    #define BOARD_PWRKEY_PIN          (3)

    #define GPS_RX_PIN                (39)
    #define GPS_TX_PIN                (42)

    #define BOARD_LED_PIN             (38)
    #define LED_ON                    (HIGH)

    #define SerialAT                Serial1

    #ifndef MODEM_GPS_ENABLE_GPIO
        #define MODEM_GPS_ENABLE_GPIO               (-1)
    #endif
    #ifndef MODEM_GPS_ENABLE_LEVEL
        #define MODEM_GPS_ENABLE_LEVEL              (-1)
    #endif
    
#endif


#ifndef MODEM_START_WAIT_MS
    #define MODEM_START_WAIT_MS             3000
#endif

#if defined(TINY_GSM_MODEM_SIM7670G) || defined(TINY_GSM_MODEM_A7670) || defined(TINY_GSM_MODEM_A7608)
    #define MODEM_REG_SMS_ONLY
#endif

// T-Call-A767X V1.0 and V1.1 have different Pinmaps and cannot be used universally , Invalid version
// #define LILYGO_T_CALL_A7670_V1_1
// #define LILYGO_T_A7608X_DC_S3

// Power on/off sequence
#if defined(TINY_GSM_MODEM_A7670)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (100)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (3000)
#elif defined(TINY_GSM_MODEM_SIM7670G)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (100)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (3000)
#elif defined(TINY_GSM_MODEM_SIM7600)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (500)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (3000)
#elif defined(TINY_GSM_MODEM_SIM7080)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (1000)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (1300)
#elif defined(TINY_GSM_MODEM_A7608)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (1000)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (3000)
#elif defined(TINY_GSM_MODEM_SIM7000SSL) || defined(TINY_GSM_MODEM_SIM7000)
    #define MODEM_POWERON_PULSE_WIDTH_MS      (1000)
    #define MODEM_POWEROFF_PULSE_WIDTH_MS     (1300)
#endif

