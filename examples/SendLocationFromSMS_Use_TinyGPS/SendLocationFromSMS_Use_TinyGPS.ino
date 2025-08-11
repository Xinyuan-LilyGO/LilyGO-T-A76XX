/**
 * @file      SendLocationFromSMS_Use_TinyGPS.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-08-30
 * @note      SIM7670G does not support SMS and voice functions
 * *!         important  important  important
 * *!         important  important  important
 * *!         important  important  important
 * *!         important  important  important
 * *          New users please use GPS_BuiltIn https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/examples/GPS_BuiltIn
 * *          This example explanation can be found here. Please do not ask invalid questions.
 * *          https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/118
 */

#include "utilities.h"

#ifdef LILYGO_SIM7000G
#warning "SIM7000G SMS function, the network access mode must be GSM, not NB-IOT"
#endif

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS


#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


#include <TinyGPSPlus.h>
TinyGPSPlus gps;

uint32_t last_check = 0;

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom


#define SMS_TARGET  "+86xxxxxxxxxx" //Change the number you want to send sms message



void light_sleep(uint32_t ms)
{
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
}

void deep_sleep(uint32_t ms)
{
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_deep_sleep_start();
}

void smartDelay(unsigned long ms)
{
    int ch = 0;
    unsigned long start = millis();
    do {
        while (SerialAT.available()) {
            ch = SerialAT.read();
            Serial.write(ch);
            gps.encode(ch);
        }
    } while (millis() - start < ms);
}



bool loopGPS()
{
    smartDelay(1000);

    if ((gps.charsProcessed() < 50) && ((millis() - last_check) > 10000UL)) {
        Serial.println("ERROR:GPS NO DATA!"); delay(1000);
    }

    if (gps.date.isValid() && gps.location.isValid() && gps.time.isValid()) {
        if (gps.date.year() > 2000) {

            Serial.println("The location was obtained successfully, and GPS positioning was turned off.");
            /*
            * After obtaining the geographic location,
            * turn off NMEA sentence output, otherwise it will affect AT interaction
            * */
            modem.disableNMEA();

            // Turn Off GPS
            modem.disableGPS();
            return true;
        }
    }
    return false;
}


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

#ifdef MODEM_FLIGHT_PIN
    // If there is an airplane mode control, you need to exit airplane mode
    pinMode(MODEM_FLIGHT_PIN, OUTPUT);
    digitalWrite(MODEM_FLIGHT_PIN, HIGH);
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

#ifdef MODEM_RING_PIN
    // Set ring pin input
    pinMode(MODEM_RING_PIN, INPUT_PULLUP);
#endif

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

    while (!modem.testAT()) {
        delay(10);
    }



#ifdef NETWORK_APN
    Serial.printf("Set network apn : %s\n", NETWORK_APN);
    if (!modem.setNetworkAPN(NETWORK_APN)) {
        Serial.println("Set network apn error !");
    }
#endif


    // Check network registration status and network signal status
    int16_t sq ;
    Serial.print("Wait for the modem to register with the network.");
    RegStatus status = REG_NO_RESULT;
    while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED) {
        status = modem.getRegistrationStatus();
        switch (status) {
        case REG_UNREGISTERED:
        case REG_SEARCHING:
            sq = modem.getSignalQuality();
            Serial.printf("[%lu] Signal Quality:%d\n", millis() / 1000, sq);
            delay(1000);
            break;
        case REG_DENIED:
            Serial.println("Network registration was rejected, please check if the APN is correct");
            return ;
        case REG_OK_HOME:
            Serial.println("Online registration successful");
            break;
        case REG_OK_ROAMING:
            Serial.println("Network registration successful, currently in roaming mode");
            break;
        default:
            Serial.printf("Registration Status:%d\n", status);
            delay(1000);
            break;
        }
    }
    Serial.println();

    Serial.printf("Registration Status:%d\n", status);
    delay(1000);



    Serial.println("Enabling GPS/GNSS/GLONASS");
    while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
        Serial.print(".");
    }
    Serial.println();
    Serial.println("GPS Enabled");


    /*
    *   Enable GPS OUTPUT NMEA parse from TinyGPS Library
    * * */
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

    last_check = millis();
}


void loop()
{
    // New users please use GPS_BuiltIn https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/examples/GPS_BuiltIn
    // This example explanation can be found here. Please do not ask invalid questions.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/118
    if (loopGPS() == true) {

        Serial.printf("The location was obtained successfully and sent to the %s number.\n", SMS_TARGET);

        double  lon2 = gps.location.lng();
        double  lat2 = gps.location.lat();
        uint16_t  year2 = gps.date.year();
        uint8_t   month2 = gps.date.month();
        uint8_t   day2 = gps.date.day();
        uint8_t   hour2 = gps.time.hour();
        uint8_t   min2 = gps.time.minute();
        uint8_t   sec2 = gps.time.second();
        double  speed = gps.speed.value();

        String msg_str = "Longitude:" + String(lon2, 6) + " ";
        msg_str += "Latitude:" + String(lat2, 6) + "\n";
        msg_str += "UTC Date:" + String(year2) + "/";
        msg_str +=  String(month2) + "/";
        msg_str +=  String(day2) + " \n";
        msg_str += "UTC Time:" + String(hour2) + ":";
        msg_str += String(min2) + ":";
        msg_str += String(sec2);
        msg_str += "\n";
        msg_str += "Speed:";
        msg_str += String(speed);
        msg_str += "Km/H\n";

        Serial.print("MESSAGE:"); Serial.println(msg_str);

        bool res = modem.sendSMS(SMS_TARGET, msg_str);
        Serial.print("Send sms message ");
        Serial.println(res ? "OK" : "fail");

        // Wait print done
        delay(3000);

        // Deep sleep, wake up every 60 seconds for positioning
        deep_sleep(60 * 1000); //60 Second
    }

    // Light sleep for one second to reduce running power consumption
    light_sleep(5000);
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
