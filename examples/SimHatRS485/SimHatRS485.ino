/**
 * @file      SimHatRS485.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-05-09
 * @note      A7670G + external L76K GPS module solution is not supported.
 *            SIM7600 MODEM DTR PIN ,GPIO conflict, cannot be used
 *            SIM7670G RESET PIN,GPIO conflict, cannot be used
 */
#include "Arduino.h"
#include "utilities.h"

#define SerialRS485     Serial2

#if defined(LILYGO_T_A7670)  || defined(LILYGO_SIM7600X) || \
    defined(LILYGO_SIM7000G) || defined(LILYGO_T_A7608X)

    #define CAN_RX_PIN          21
    #define CAN_TX_PIN          22
    #define CAN_ENABLE_PIN      0
    #define RS485_TX_PIN        19
    #define RS485_RX_PIN        18
    #define RS485_ENABLE_PIN    23  
    #define RELAY_PIN           32  //SIM7600 MODEM DTR PIN ,GPIO conflict, cannot be used

#elif defined(LILYGO_T_A7608X_S3)

    #define CAN_RX_PIN          2
    #define CAN_TX_PIN          1
    #define CAN_ENABLE_PIN      0
    #define RS485_TX_PIN        41
    #define RS485_RX_PIN        40
    #define RS485_ENABLE_PIN    42
    #define RELAY_PIN           5

#elif defined(LILYGO_T_SIM7670G_S3)

    #define CAN_RX_PIN          2
    #define CAN_TX_PIN          1
    #define CAN_ENABLE_PIN      48
    #define RS485_TX_PIN        41
    #define RS485_RX_PIN        40
    #define RS485_ENABLE_PIN    42
    #define RELAY_PIN           -1 //SIM7670G RESET PIN,GPIO conflict, cannot be used

#else
    #error "Not support this board"
#endif

void setup()
{
    pinMode(RS485_ENABLE_PIN, OUTPUT);
    digitalWrite(RS485_ENABLE_PIN, HIGH);

    Serial.begin(115200); // Set console baud rate

    // Modem use Serial1
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    // RS485 use Serial2
    SerialRS485.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

}

void loop()
{
    if (SerialRS485.available()) {
        Serial.write(SerialRS485.read());
    }
    if (Serial.available()) {
        SerialRS485.write(Serial.read());
    }
    delay(1);
}