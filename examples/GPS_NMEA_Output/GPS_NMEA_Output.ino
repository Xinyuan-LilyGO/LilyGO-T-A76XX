/**
 * @file      GPS_NMEA_Output.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-08-13
 * @note      GPS only supports A7670X/A7608X/SIM7000G/SIM7600 series (excluding A7670G and other versions that do not support positioning).
 */
#include "utilities.h"

#ifdef TINY_GSM_MODEM_SIM7080
#error "SIM7080G NOT SUPPORT GPS NMEA OUTPUT TO SERIAL,PLEASE USE THE GPS_BuiltIn EXAMPLE"
#endif

#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>
#include <TinyGPSPlus.h>
#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


uint32_t check_interval = 0;


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
    delay(MODEM_POWERON_PULSE_WIDTH_MS);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

    /*
    *   During testing, it was found that there may be a power outage.
        Add a loop detection here. When the GPS timeout does not start,
        resend the AT to check if the modem is online
    * */
    Serial.print("Modem starting...");
    int retry = 0;
    while (!modem.testAT(1000)) {
        Serial.println(".");
        if (retry++ > 30) {
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_PWRKEY_PIN, HIGH);
            delay(1000);    //Ton = 1000ms ,Min = 500ms, Max 2000ms
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

    retry = 15;
    Serial.println("Enabling GPS/GNSS/GLONASS");
    while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
        Serial.print(".");
        if (retry-- <= 0) {
            Serial.println("GPS startup failed. Please check whether the board you ordered contains GPS function."); delay(1000);
        }
    }

    Serial.println();
    Serial.println("GPS Enabled");

    modem.setGPSBaud(115200);

#if defined(TINY_GSM_MODEM_A7670) || defined(TINY_GSM_MODEM_A7608)
    modem.setGPSMode(GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS);
#elif defined(TINY_GSM_MODEM_SIM7670G)
    modem.setGPSMode(GNSS_MODE_GPS_GLONASS_BDS);
#elif defined(TINY_GSM_MODEM_SIM7600)
    modem.setGPSMode(GNSS_MODE_ALL);
#endif

    modem.configNMEASentence(1, 1, 1, 1, 1, 1);

    modem.setGPSOutputRate(1);

    modem.enableNMEA();

    Serial.println("Next you should see NMEA sentences in the serial monitor");

}

void loop()
{
    if (millis() > check_interval) {
        if (modem.isEnableGPS() == false) {
            Serial.println("Restart GPS!");
            modem.enableGPS();
            delay(3000);
        }
        check_interval = millis() + 15000;
    }

    while (SerialAT.available()) {
        int  ch = SerialAT.read();
        Serial.write(ch);
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
