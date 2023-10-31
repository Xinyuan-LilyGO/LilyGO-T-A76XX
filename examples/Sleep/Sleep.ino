/**
 * @file      Sleep.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-05-24
 *
 */
#include "utilities.h"
#include <driver/gpio.h>

#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>

#define uS_TO_S_FACTOR      1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP       30          /* Time ESP32 will go to sleep (in seconds) */


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

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    // Turn on DC boost to power on the modem
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, LOW);

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) {
        /*
        BOARD_PWRKEY_PIN IO:4 The power-on signal of the modulator must be given to it,
        otherwise the modulator will not reply when the command is sent
        */
        pinMode(BOARD_PWRKEY_PIN, OUTPUT);
        digitalWrite(BOARD_PWRKEY_PIN, LOW);
        delay(100);
        digitalWrite(BOARD_PWRKEY_PIN, HIGH);
        //Ton >= 100 <= 500
        delay(100);
        digitalWrite(BOARD_PWRKEY_PIN, LOW);


    } else {
        Serial.println("Wakeup modem !");

        // Need to cancel GPIO hold if wake from sleep
        gpio_hold_dis((gpio_num_t )MODEM_DTR_PIN);

        // Pull down DTR to wake up MODEM
        pinMode(MODEM_DTR_PIN, OUTPUT);
        digitalWrite(MODEM_DTR_PIN, LOW);
        delay(2000);
        modem.sleepEnable(false);
    }


    Serial.println("Check modem online .");
    while (!modem.testAT()) {
        Serial.print("."); delay(500);
    }
    Serial.println("Modem is online !");

    delay(5000);


    Serial.println("Enter modem sleep mode!");

    // Pull up DTR to put the modem into sleep
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, HIGH);
    // Set DTR to keep at high level, if not set, DTR will be invalid after ESP32 goes to sleep.
    gpio_hold_en((gpio_num_t )MODEM_DTR_PIN);
    gpio_deep_sleep_hold_en();


    if (modem.sleepEnable(true) != true) {
        Serial.println("modem sleep failed!");
    } else {
        Serial.println("Modem enter sleep modem!");
    }

    delay(5000);

    Serial.println("Check modem response .");
    while (modem.testAT()) {
        Serial.print("."); delay(500);
    }
    Serial.println("Modem is not respone ,modem has sleep !");

    delay(5000);

    Serial.println("Enter esp32 goto deepsleep!");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop()
{
}

