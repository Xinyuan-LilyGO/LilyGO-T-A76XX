/**
 * @file      LBSExample.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-10-26
 * @note      Not support T-SIM7672
 */

#include "utilities.h"


// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

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

    // Set ring pin input
    pinMode(MODEM_RING_PIN, INPUT_PULLUP);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

    while (!modem.testAT()) {
        delay(10);
    }

    // Wait PB DONE
    delay(10000);

}

void loop()
{
    // You need to wait for the network to be activated before you can use the LBS function. 
    // By default, the network is automatically activated after the modem is started.
    float lat      = 0;
    float lon      = 0;
    float accuracy = 0;
    int   year     = 0;
    int   month    = 0;
    int   day      = 0;
    int   hour     = 0;
    int   min      = 0;
    int   sec      = 0;
    for (;;) {
        Serial.println("Requesting current GSM location");
        if (modem.getGsmLocation(&lat, &lon, &accuracy, &year, &month, &day, &hour,
                                 &min, &sec)) {
            Serial.print("Latitude:"); Serial.println(lat, 8);
            Serial.print("Longitude:"); Serial.println(lon, 8);
            Serial.print("Accuracy:"); Serial.println(accuracy);
            Serial.print("Year:"); Serial.print(year);
            Serial.print("\tMonth:"); Serial.print(month);
            Serial.print("\tDay:"); Serial.println(day);
            Serial.print("Hour:"); Serial.print(hour);
            Serial.print("\tMinute:"); Serial.print(min);
            Serial.print("\tSecond:"); Serial.println(sec);
            break;
        } else {
            DBG("Couldn't get GSM location, retrying in 15s.");
            delay(15000L);
        }
    }
    Serial.println("Retrieving GSM location again as a string");
    String location = modem.getGsmLocation();
    Serial.print("GSM Based Location String:");
    Serial.println(location);

    while (1) {
        if (SerialAT.available()) {
            Serial.write(SerialAT.read());
        }
        if (Serial.available()) {
            SerialAT.write(Serial.read());
        }
    }
}
