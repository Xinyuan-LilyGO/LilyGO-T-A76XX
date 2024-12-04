/**
 * @file      GPSShield.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-05-22
 * @note      This sketch is only suitable for boards without GPS function inside the modem, such as A7670G
 *            Works only with externally mounted GPS modules.
 *            If the purchased board includes a GPS extension module it will work,
 *            otherwise this sketch will have no effect
 * @note      Onlu support T-A7670 board , not support T-Call A7670 , T-SIM7672 , T-PCIE
 */

#ifndef SerialAT
#define SerialAT Serial1
#endif

#ifndef SerialGPS
#define SerialGPS Serial2
#endif


#define BOARD_MODEM_DTR_PIN                 25
#define BOARD_MODEM_TX_PIN                  26
#define BOARD_MODEM_RX_PIN                  27
#define BOARD_MODEM_PWR_PIN                 4
#define BOARD_ADC_PIN                       35
#define BOARD_POWER_ON_PIN                  12
#define BOARD_MODEM_RI_PIN                  33
#define BOARD_RST_PIN                       5
#define BOARD_SDCARD_MISO                   2
#define BOARD_SDCARD_MOSI                   15
#define BOARD_SDCARD_SCLK                   14
#define BOARD_SDCARD_CS                     13

#define BOARD_GPS_TX_PIN                    21
#define BOARD_GPS_RX_PIN                    22
#define BOARD_GPS_PPS_PIN                   23
#define BOARD_GPS_WAKEUP_PIN                19


#include <Arduino.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;


void setup()
{
    pinMode(BOARD_POWER_ON_PIN, OUTPUT);
    digitalWrite(BOARD_POWER_ON_PIN, HIGH);

    pinMode(BOARD_RST_PIN, OUTPUT);
    digitalWrite(BOARD_RST_PIN, LOW);

    pinMode(BOARD_MODEM_PWR_PIN, OUTPUT);
    digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_MODEM_PWR_PIN, HIGH);
    delay(1000);
    digitalWrite(BOARD_MODEM_PWR_PIN, LOW);

    Serial.begin(115200);
    //Modem Serial port
    SerialAT.begin(115200, SERIAL_8N1, BOARD_MODEM_RX_PIN, BOARD_MODEM_TX_PIN);
    //GPS Serial port
    SerialGPS.begin(9600, SERIAL_8N1, BOARD_GPS_RX_PIN, BOARD_GPS_TX_PIN);

    // Ref issue : https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/56#issuecomment-1672628977
    Serial.println("This sketch is only suitable for boards without GPS function inside the modem, such as A7670G");

    Serial.println("Works only with externally mounted GPS modules. If the purchased board includes a GPS extension module it will work, otherwise this sketch will have no effect");

    Serial.println("If the purchased modem supports GPS functionality, please use examples/TinyGSM_Net_GNSS");

    delay(2000);

}

void loop()
{
    while (SerialGPS.available()) {
        int c = SerialGPS.read();
        if (gps.encode(c)) {
            displayInfo();
        }
    }

    if (millis() > 30000 && gps.charsProcessed() < 10) {
        Serial.println(F("No GPS detected: check wiring."));
        delay(1000);
    }

    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }
    delay(1);
}


void displayInfo()
{
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}
