/**
 * @file      SimHatRelay.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-10-17
 *
 */

#include <Arduino.h>

// T-A7670X SIM-Hat Pin
#define RELAY1_PIN     18
#define RELAY2_PIN     19
#define RELAY3_PIN     32

 ///T-A7608-ESP32S3 SIM-Hat Pin
// #define RELAY1_PIN     40
// #define RELAY2_PIN     41
// #define RELAY3_PIN     5
  

void setup(void)
{
    Serial.begin(115200);

    while (!Serial) {
        delay(1);
    }

    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);

    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    digitalWrite(RELAY3_PIN, LOW);
}

void loop(void)
{
    digitalWrite(RELAY1_PIN, HIGH);
    delay(500);
    digitalWrite(RELAY2_PIN, HIGH);
    delay(500);
    digitalWrite(RELAY3_PIN, HIGH);
    delay(500);
    digitalWrite(RELAY1_PIN, LOW);
    delay(500);
    digitalWrite(RELAY2_PIN, LOW);
    delay(500);
    digitalWrite(RELAY3_PIN, LOW);
    delay(500);
}
