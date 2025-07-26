/**
 * @file      SerialRS485.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-03
 * @note      Simple demonstration of how to use Serial2
 */
#include "Arduino.h"
#include "utilities.h"

#define SerialRS485     Serial2

// #define USING_SIMSHIELD //Define this line to use the SimShield pinout

#ifdef USING_SIMSHIELD
#define RS485_RX_PIN    SIMSHIELD_RS_RX  // GPIO34 only input 
#define RS485_TX_PIN    SIMSHIELD_RS_TX  // GPIO32 can output 
#else /*USING_SIMSHIELD*/
// You can freely choose unused GPIO as RS485 TX, RX
#if CONFIG_IDF_TARGET_ESP32S3
#define RS485_RX_PIN    15
#define RS485_TX_PIN    16
#else /*CONFIG_IDF_TARGET_ESP32S3*/
// GPIO above ESP32 GPIO33 can only be used as input, ESP32S3 has no restrictions
#define RS485_RX_PIN    34  // GPIO34 only input 
#define RS485_TX_PIN    32  // GPIO32 can output 
#endif
#endif /*USING_SIMSHIELD*/

void setup()
{
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
