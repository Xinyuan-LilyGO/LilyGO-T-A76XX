/**
 * @file      ModemSleep.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-05-24
 * @record    https://youtu.be/2cjNsYcU6TU
 * @note      T-A7608 & T-A7608-S3 & T-A7670x VBUS of the modem is connected to VBUS.
 *            When using USB power supply, the modem cannot be set to sleep mode. Please see README for details.
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
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) {

#ifdef MODEM_RESET_PIN
        // Release reset GPIO hold
        gpio_hold_dis((gpio_num_t)MODEM_RESET_PIN);

        // Set modem reset pin ,reset modem
        // The module will also be started during reset.
        Serial.println("Set Reset Pin.");
        pinMode(MODEM_RESET_PIN, OUTPUT);
        digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
        digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
        digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
#endif

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

#ifdef BOARD_POWERON_PIN
        // Need to cancel GPIO hold if wake from sleep
        gpio_hold_dis((gpio_num_t )BOARD_POWERON_PIN);
#endif

        // Pull down DTR to wake up MODEM
        pinMode(MODEM_DTR_PIN, OUTPUT);
        digitalWrite(MODEM_DTR_PIN, LOW);
        delay(2000);
        modem.sleepEnable(false);

        // Delay sometime ...
        delay(10000);
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

    // If it doesn't sleep, please see README to remove the resistor, which is only needed when USB-C is used for power supply.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/examples/ModemSleep
    Serial.println("Check modem response .");
    while (modem.testAT()) {
        Serial.print("."); delay(500);
    }
    Serial.println("Modem is not response ,modem has sleep !");

    delay(5000);

#ifdef BOARD_POWERON_PIN
    gpio_hold_en((gpio_num_t )BOARD_POWERON_PIN);
    gpio_deep_sleep_hold_en();
#endif

#ifdef MODEM_RESET_PIN
    // Keep it low during the sleep period. If the module uses GPIO5 as reset, 
    // there will be a pulse when waking up from sleep that will cause the module to start directly.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/85
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    gpio_hold_en((gpio_num_t)MODEM_RESET_PIN);
    gpio_deep_sleep_hold_en();
#endif


    Serial.println("Enter esp32 goto deepsleep!");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop()
{
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
