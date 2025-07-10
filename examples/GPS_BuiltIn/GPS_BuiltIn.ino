/**
 * @file      GPS_BuiltIn.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-06-28
 * @note      GPS only supports A7670X/A7608X/SIM7000G/SIM7600 series (excluding A7670G and other versions that do not support positioning).
 */
#include "utilities.h"

#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial


// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS


#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

void setup()
{
    Serial.begin(115200);
#ifdef BOARD_POWERON_PIN
    /* Set Power control pin output
    * * @note      Known issues, ESP32 (V1.2) version of T-A7670, T-A7608,
    *            when using battery power supply mode, BOARD_POWERON_PIN (IO12) must be set to high level after esp32 starts, otherwise a reset will occur.
    * */
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    // Set modem reset pin ,reset modem
#ifdef MODEM_RESET_PIN
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
#endif

    // Pull down DTR to ensure the modem is not in sleep state
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    // Turn on modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(1000);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

    int retry = 0;
    while (!modem.testAT(1000)) {
        Serial.println(".");
        if (retry++ > 10) {
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_PWRKEY_PIN, HIGH);
            delay(1000);
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            retry = 0;
        }
    }
    Serial.println();
    delay(200);

    String modemName = "UNKOWN";
    while (1) {
        modemName = modem.getModemName();
        if (modemName == "UNKOWN") {
            Serial.println("Unable to obtain module information normally, try again");
            delay(1000);
        } else if (modemName.startsWith("A7670G")) {
            while (1) {
                Serial.println("A7670G does not support built-in GPS function, please run examples/GPSShield");
                delay(1000);
            }
        } else {
            Serial.print("Model Name:");
            Serial.println(modemName);
            break;
        }
        delay(5000);
    }

    modem.sendAT("+SIMCOMATI");
    modem.waitResponse();

    /*
    *   A7608 B08 firmware has GPS positioning problems. If it is B08 version, you need to upgrade the A7608 firmware.
    *
    *   Manufacturer: INCORPORATED
    *   Model: A7608SA-H
    *   Revision: A50C4B08A7600M7
    *   A7600M7_B08V02_220929
    *   QCN:
    *   IMEI: xxxxxxxxxxxxx
    *   MEID:
    *   +GCAP: +CGSM,+FCLASS,+DS
    *   DeviceInfo:
    *
    *   +CGNSSINFO: 2,04,00,21.xxxxx,N,114.xxxxxxxx,E,020924,094145.00,-34.0,1.403,,6.9,6.8,1.0,03
    */


    Serial.println("Enabling GPS/GNSS/GLONASS");
    while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
        Serial.print(".");
    }
    Serial.println();
    Serial.println("GPS Enabled");

    // Set GPS Baud to 115200
    modem.setGPSBaud(115200);

}

void loop()
{
    float lat2      = 0;
    float lon2      = 0;
    float speed2    = 0;
    float alt2      = 0;
    int   vsat2     = 0;
    int   usat2     = 0;
    float accuracy2 = 0;
    int   year2     = 0;
    int   month2    = 0;
    int   day2      = 0;
    int   hour2     = 0;
    int   min2      = 0;
    int   sec2      = 0;
    uint8_t    fixMode   = 0;
    for (;;) {
        Serial.println("Requesting current GPS/GNSS/GLONASS location");
        if (modem.getGPS(&fixMode, &lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
                         &year2, &month2, &day2, &hour2, &min2, &sec2)) {

            Serial.print("FixMode:"); Serial.println(fixMode);
            Serial.print("Latitude:"); Serial.print(lat2, 6); Serial.print("\tLongitude:"); Serial.println(lon2, 6);
            Serial.print("Speed:"); Serial.print(speed2); Serial.print("\tAltitude:"); Serial.println(alt2);
            Serial.print("Visible Satellites:"); Serial.print(vsat2);

            // GPS_BuiltIn cannot get the number of satellites in use, so it always returns 0
            Serial.print("\tUsed Satellites:"); Serial.println(usat2);
            Serial.print("Accuracy:"); Serial.println(accuracy2);

            Serial.print("Year:"); Serial.print(year2);
            Serial.print("\tMonth:"); Serial.print(month2);
            Serial.print("\tDay:"); Serial.println(day2);

            Serial.print("Hour:"); Serial.print(hour2);
            Serial.print("\tMinute:"); Serial.print(min2);
            Serial.print("\tSecond:"); Serial.println(sec2);
            break;
        } else {
            Serial.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 15s.");
            delay(15000L);
        }
    }
    Serial.println("Retrieving GPS/GNSS/GLONASS location again as a string");
    String gps_raw = modem.getGPSraw();
    Serial.print("GPS/GNSS Based Location String:");
    Serial.println(gps_raw);
    Serial.println("Disabling GPS");

    modem.disableGPS();

    while (1) {
        if (SerialAT.available()) {
            Serial.write(SerialAT.read());
        }
        if (Serial.available()) {
            SerialAT.write(Serial.read());
        }
        delay(1);
    }
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif

/*
SIM7600 Version OK 20250709
AT+SIMCOMATI
Manufacturer: SIMCOM INCORPORATED
Model: SIMCOM_SIM7600G-H
Revision: LE20B04SIM7600G22
QCN: 
IMEI: xxxxxxxxxxxx
MEID: 
+GCAP: +CGSM
DeviceInfo: 173,170
*/
