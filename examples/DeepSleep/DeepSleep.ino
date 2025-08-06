/**
 * @file      DeepSleep.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-16
 * @record    T-A7608-S3 : https://youtu.be/5G4COjtKsFU
 *
 * !The following test parameters are all obtained by testing at a voltage of 4.2V using a battery holder. Instrument: VICTOR 8246A
 *
 * T-A7608-S3 DeepSleep ~ 368 uA
 * T-A7608-ESP32  DeepSleep ~ 240 uA
 * T-A7670-ESP32  DeepSleep ~ 157 uA
 * T-SIM7600-ESP32 DeepSleep ~ 200 uA
 * T-SIM7000-ESP32 DeepSleep ~ 500 uA
 * T-SIM7080G-S3-Standard DeepSleep Current dynamic changes Min:60uA , Max186uA ,Avg:128uA
 * T-SIM7000G-S3-Standard DeepSleep Current dynamic changes Min:59uA , Max273uA ,Avg:166uA
 * T-SIM7670G-S3-Standard DeepSleep Current dynamic changes Min:64uA , Max201uA ,Avg:147uA
 * T-A7670X-S3-Standard DeepSleep Current dynamic changes Min:63uA , Max288uA ,Avg:181uA
 * T-A7670G-S3-Standard + L76K GPS Module DeepSleep Current dynamic changes Min:282uA , Max334uA ,Avg:314uA
 * 
 */

#include "utilities.h"
#include <driver/gpio.h>

// If use the A7670G + L76K GPS module,
// Need to set the GPS to sleep mode, otherwise the sleep current will not drop.
// #define USING_L76K_MODULE

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

#ifdef BOARD_LED_PIN
    pinMode(BOARD_LED_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, LED_ON);
#endif

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Serial.println("Wakeup timer");
        int i = 30;
        while (i > 0) {
            Serial.printf("Modem will start in %d seconds\n", i);
            Serial.flush();
            delay(1000);
            i--;
        }
        Serial.println("TurnON Modem!");
    }

#ifdef BOARD_POWERON_PIN
    /* Set Power control pin output
    * * @note      Known issues, ESP32 (V1.2) version of T-A7670, T-A7608,
    *            when using battery power supply mode, BOARD_POWERON_PIN (IO12) must be set to high level after esp32 starts, otherwise a reset will occur.
    * */
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    delay(2000);
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

    // Pull down DTR to ensure the modem is not in sleep state
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    // Turn on the modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(MODEM_POWERON_PULSE_WIDTH_MS);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Delay sometime ...
    delay(10000);

    Serial.println("Check modem online .");
    while (!modem.testAT()) {
        Serial.print("."); delay(500);
    }
    Serial.println("Modem is online !");

    delay(5000);


    Serial.println("Enter modem power off!");

    if (modem.poweroff()) {
        Serial.println("Modem enter power off modem!");
    } else {
        Serial.println("modem power off failed!");
    }

    delay(5000);

    Serial.println("Check modem response .");
    while (modem.testAT()) {
        Serial.print("."); delay(500);
    }
    Serial.println("Modem is not response ,modem has power off !");

    delay(5000);

#ifdef BOARD_LED_PIN
    digitalWrite(BOARD_LED_PIN, !LED_ON);
#endif

#ifdef BOARD_POWERON_PIN
    // Turn on DC boost to power off the modem
    digitalWrite(BOARD_POWERON_PIN, LOW);
#endif

#ifdef MODEM_RESET_PIN
    // Keep it low during the sleep period. If the module uses GPIO5 as reset,
    // there will be a pulse when waking up from sleep that will cause the module to start directly.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/85
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    gpio_hold_en((gpio_num_t)MODEM_RESET_PIN);
    gpio_deep_sleep_hold_en();
#endif


#ifdef USING_L76K_MODULE
    pinMode(GPS_SHIELD_WAKEUP_PIN, OUTPUT);
    digitalWrite(GPS_SHIELD_WAKEUP_PIN, LOW);
    gpio_hold_en((gpio_num_t) GPS_SHIELD_WAKEUP_PIN);
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
