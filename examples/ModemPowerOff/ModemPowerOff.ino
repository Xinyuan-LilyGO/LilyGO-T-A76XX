/**
 * @file      ModemPowerOff.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-04-11
 *
 */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include "utilities.h"
#include <TinyGsmClient.h>
#include "Arduino.h"

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


void setup()
{
    Serial.begin(115200); // Set console baud rate

    Serial.println("Start Sketch");

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    // Set Power control pin output
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, LOW);
#endif

}



void loopAT(uint32_t delay_ms)
{
    uint32_t interval = millis() + delay_ms;
    while (millis() < interval) {
        while (SerialAT.available()) {
            Serial.write(SerialAT.read());
        }
        while (Serial.available()) {
            SerialAT.write(Serial.read());
        }
        delay(1);
    }
}


void loop()
{
    int i = 10;
    while (i--) {
        Serial.printf("Turn on the modem after  %d seconds\n", i); delay(1000);
    }

    // Turn on the modem power supply
#ifdef BOARD_POWERON_PIN
    digitalWrite(BOARD_POWERON_PIN, HIGH);
    delay(100);
#endif

    // Set modem reset pin ,reset modem
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

    // PowerKey Control modem power on
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Test modem connected 
    while (!modem.testAT()) {
        delay(1);
    }

    Serial.println("Modem has power on!");

    // 20s
    loopAT(20000);

    i = 5;
    while (i--) {
        Serial.printf("Turn off the modem after  %d seconds\n", i);
        delay(1000);
    }

    // AT Command send poweroff cmd
    Serial.println("Send power off!");
    modem.poweroff();

    delay(10000);

    // Turn off the modem power supply
#ifdef BOARD_POWERON_PIN
    digitalWrite(BOARD_POWERON_PIN, LOW);
#endif

    loopAT(10000);
}
