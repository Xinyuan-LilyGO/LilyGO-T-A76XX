/**
 * @file      GPS_NMEA_Output.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-08-13
 *
 */
#include "utilities.h"

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

void displayInfo();

void setup()
{
    Serial.begin(115200);
    // Turn on DC boost to power on the modem
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    // Set modem reset pin ,reset modem
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

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

    /*
    *   During testing, it was found that there may be a power outage.
        Add a loop detection here. When the GPS timeout does not start,
        resend the AT to check if the modem is online
    * */
    Serial.print("Modem starting...");
    int retry = 0;
    while (!modem.testAT(1000)) {
        Serial.println(".");
        if (retry++ > 10) {
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


    Serial.println("Before running, please check whether the version you purchased supports the GPS function. Regardless of whether the PCB contains an antenna connector, the A7670G does not include a built-in GPS function. If the A7670G you purchased claims to support GPS, then please run examples/GPSShield\n\n");
    Serial.println("Before running, please check whether the version you purchased supports the GPS function. Regardless of whether the PCB contains an antenna connector, the A7670G does not include a built-in GPS function. If the A7670G you purchased claims to support GPS, then please run examples/GPSShield\n\n");
    Serial.println("Before running, please check whether the version you purchased supports the GPS function. Regardless of whether the PCB contains an antenna connector, the A7670G does not include a built-in GPS function. If the A7670G you purchased claims to support GPS, then please run examples/GPSShield\n\n");


    retry = 15;
    Serial.println("Enabling GPS/GNSS/GLONASS");
    while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO)) {
        Serial.print(".");
        if (retry-- <= 0) {
            Serial.println("GPS startup failed. Please check whether the board you ordered contains GPS function."); delay(1000);
        }
    }

    Serial.println();
    Serial.println("GPS Enabled");

    modem.setGPSBaud(115200);

    modem.setGPSMode(3);    //GPS + BD

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

