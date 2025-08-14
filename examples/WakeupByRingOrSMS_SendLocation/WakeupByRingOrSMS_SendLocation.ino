/**
 * @file      WakeupByRingOrSMS_SendLocation.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-08-14
 * @note      Only applicable to A7670X, A7608X series,SIM7600 series modules,
 *            SIM7670G - `SIM7670G-MNGV 2374B04` version supports SMS function,but not support voice function
 *            SIM7000G - RI pin not connected to esp ,this examples can't run
 *            but it requires the operator base station to support SMS Over SGS service to send,
 *            otherwise it will be invalid
 * 
 *             When a "sleep" text message is received, the device will continue to sleep.
 *             When a "get gps" message is received, the device will wake up to obtain the location and send it to the 
 *             specified number. 
* */


#include "utilities.h"
#include <driver/gpio.h>

#ifdef LILYGO_SIM7000G
#error "SIM7000G - RI pin not connected to esp ,this examples can't run"
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>

#define SMS_TARGET          "+86xxxxxxx" //Change the number you want to send sms message

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

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
uint8_t fixMode   = 0;

void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
    }
}

bool loopGPS()
{
    Serial.println("Requesting current GPS/GNSS/GLONASS location");
    while (1) {
        if (modem.getGPS(&fixMode, &lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
                         &year2, &month2, &day2, &hour2, &min2, &sec2)) {

            Serial.print("FixMode:"); Serial.println(fixMode);
            Serial.print("Latitude:"); Serial.print(lat2, 6); Serial.print("\tLongitude:"); Serial.println(lon2, 6);
            Serial.print("Speed:"); Serial.print(speed2); Serial.print("\tAltitude:"); Serial.println(alt2);
            Serial.print("Visible Satellites:"); Serial.print(vsat2); Serial.print("\tUsed Satellites:"); Serial.println(usat2);
            Serial.print("Accuracy:"); Serial.println(accuracy2);

            Serial.print("Year:"); Serial.print(year2);
            Serial.print("\tMonth:"); Serial.print(month2);
            Serial.print("\tDay:"); Serial.println(day2);

            Serial.print("Hour:"); Serial.print(hour2);
            Serial.print("\tMinute:"); Serial.print(min2);
            Serial.print("\tSecond:"); Serial.println(sec2);
            return true;
        }
        delay(5000);
    }
    return false;
}

String getUnReadSMS()
{
    // Set SMS system into text mode
    modem.sendAT("+CMGF=1");
    modem.waitResponse(10000);

    modem.sendAT("+CMGL=\"REC UNREAD\"");
    String sms, target;
    if (modem.waitResponse(10000UL, sms) == 1) {
        int cmglIndex = sms.indexOf("+CMGL:");
        if (cmglIndex != -1) {
            int lineEnd = sms.indexOf("\r\n", cmglIndex);
            if (lineEnd != -1) {
                int contentStart = lineEnd + 2;
                int contentEnd = sms.indexOf("\r\n", contentStart);

                if (contentEnd != -1) {
                    target = sms.substring(contentStart, contentEnd);
                } else {
                    target = sms.substring(contentStart);
                }
            }
            return target;
        }
    }
    return "";
}


void setup()
{
    Serial.begin(115200); // Set console baud rate

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef BOARD_POWERON_PIN
    /* Set Power control pin output
    * * @note      Known issues, ESP32 (V1.2) version of T-A7670, T-A7608,
    *            when using battery power supply mode, BOARD_POWERON_PIN (IO12) must be set to high level after esp32 starts, otherwise a reset will occur.
    * */
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    print_wakeup_reason();

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0) {

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

#ifdef MODEM_FLIGHT_PIN
        // If there is an airplane mode control, you need to exit airplane mode
        pinMode(MODEM_FLIGHT_PIN, OUTPUT);
        digitalWrite(MODEM_FLIGHT_PIN, HIGH);
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


    } else {
        Serial.println("Released modem hold pin!");

        // Need to cancel GPIO hold if wake from sleep
        gpio_hold_dis((gpio_num_t )MODEM_DTR_PIN);

#ifdef BOARD_POWERON_PIN
        // Need to cancel GPIO hold if wake from sleep
        gpio_hold_dis((gpio_num_t )BOARD_POWERON_PIN);
#endif

#ifdef MODEM_FLIGHT_PIN
        // Need to cancel GPIO hold if wake from sleep
        gpio_hold_dis((gpio_num_t )MODEM_FLIGHT_PIN);
#endif
    }

    Serial.println("Check modem online .");
    int retry = 0;
    while (!modem.testAT()) {
        if (retry++ >= 15) {
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_PWRKEY_PIN, HIGH);
            delay(MODEM_POWERON_PULSE_WIDTH_MS);
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            Serial.println("Start modem");
            retry = 0;
        }
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Modem is online !");

    delay(5000);

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0) {
        // Wait PB DONE
        Serial.println("Wait SMS Done.");
        if (!modem.waitResponse(100000UL, "SMS DONE")) {
            Serial.println("Can't wait from sms register ....");
            return;
        }
        Serial.println("Modem has registered to network");
        delay(1000);

    } else {

        String sms = getUnReadSMS();


        // When receiving the get gps SMS, get the location information and send it to the specified number
        if (sms.startsWith("get gps")) {

            String modemName = "UNKNOWN";
            while (1) {
                modemName = modem.getModemName();
                if (modemName == "UNKNOWN") {
                    Serial.println("Unable to obtain module information normally, try again");
                    delay(1000);
                } else if (modemName.startsWith("A7670E-FASE") || modemName.startsWith("A7670SA-FASE")) {
                    Serial.println("Modem support built-in GPS function, keep running");
                    break;
                } else if (modemName.startsWith("A7670E-LNXY-UBL")
                           || modemName.startsWith("A7670SA-LASE")
                           || modemName.startsWith("A7670SA-LASC")
                           ||  modemName.startsWith("A7670G-LLSE")
                           ||  modemName.startsWith("A7670G-LABE")
                           ||  modemName.startsWith("A7670E-LASE ")) {
                    while (1) {
                        Serial.println("The modem does not have built-in GPS function.");
                        delay(1000);
                    }
                } else {
                    Serial.print("Model Name:");
                    Serial.println(modemName);
                    break;
                }
            }

            Serial.println("Enabling GPS/GNSS/GLONASS");
            while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
                Serial.print(".");
                delay(3000);
            }
            Serial.println();
            Serial.println("GPS Enabled");

            // Set GPS Baud to 115200
            modem.setGPSBaud(115200);

            if (loopGPS() == true) {

                Serial.printf("The location was obtained successfully and sent to the %s number.\n", SMS_TARGET);

                String msg_str = "Longitude:" + String(lon2, 6) + " ";
                msg_str += "Latitude:" + String(lat2, 6) + "\n";
                msg_str += "UTC Date:" + String(year2) + "/";
                msg_str +=  String(month2) + "/";
                msg_str +=  String(day2) + " \n";
                msg_str += "UTC Time:" + String(hour2) + ":";
                msg_str += String(min2) + ":";
                msg_str += String(sec2);
                msg_str += "\n";

                Serial.print("MESSAGE:"); Serial.println(msg_str);
                bool res = modem.sendSMS(SMS_TARGET, msg_str);
                Serial.print("Send sms message ");
                Serial.println(res ? "OK" : "fail");
            }
            // Continue sleeping when sleep is received
        } else if (sms.startsWith("sleep")) {
            Serial.println("goto sleep");
        }
    }


#ifdef MODEM_FLIGHT_PIN
    // Hold on flight pin
    gpio_hold_en((gpio_num_t )MODEM_FLIGHT_PIN);
    gpio_deep_sleep_hold_en();
#endif

#ifdef BOARD_POWERON_PIN
    Serial.println("Set modem power pin to hold on");
    // Hold on power pin
    gpio_hold_en((gpio_num_t )BOARD_POWERON_PIN);
    gpio_deep_sleep_hold_en();
#endif

#ifdef MODEM_RESET_PIN
    // Keep it low during the sleep period. If the module uses GPIO5 as reset,
    // there will be a pulse when waking up from sleep that will cause the module to start directly.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/85
    Serial.println("Set modem reset pin to hold on");
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    gpio_hold_en((gpio_num_t)MODEM_RESET_PIN);
    gpio_deep_sleep_hold_en();

#endif

    pinMode(MODEM_RING_PIN, INPUT_PULLUP);
    gpio_pullup_en((gpio_num_t)MODEM_RING_PIN);

    Serial.println("Ring coming...wait ring stop...");
    while (digitalRead(MODEM_RING_PIN) == LOW) {
        Serial.println("Wait rinng to stop..."); delay(5000);
    }

    Serial.println("Now your can send message or call this modem phone number wake up ESP32");
    Serial.println("Enter esp32 goto deepsleep!");
    Serial.flush();
    delay(1000);
    esp_sleep_enable_ext0_wakeup(((gpio_num_t)MODEM_RING_PIN), 0);
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
