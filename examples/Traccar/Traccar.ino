/**
 * @file      Traccar.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-03-25
 * @note      This demo uploads the location obtained by the device to [traccar](https://www.traccar.org/).
 *            For more information about [traccar](https://www.traccar.org/), please visit the official website.
 *            This demo only demonstrates uploading the location.
 */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// If defined, cancel shallow sleep and use delay to replace shallow sleep
// #define DEBUG_SKETCH

#define REPORT_LOCATION_RATE_SECOND     20

#include "utilities.h"
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom

String modemName = "UNKOWN";
const char *client_id = "yout tarrcar device id";
const char *request_url = "https://your_tarrcar_server.com";
const char *post_format = "deviceid=%s&lat=%.7f&lon=%.7f&speed=%.2f&altitude=%.2f&batt=%u";

// http://demo.traccar.org:5055/?deviceid=12345&lat=48.8566&lon=2.3522&timestamp=2021-01-01T00:00:00Z&speed=15&bearing=270&altitude=35&accuracy=10&hdop=0.8&batt=75

void light_sleep_delay(uint32_t ms)
{
#ifdef DEBUG_SKETCH
    delay(ms);
#else
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_light_sleep_start();
#endif
}


bool post_location(GPSInfo &info)
{
    // Initialize HTTPS
    modem.https_begin();

    char post_buffer[128];

    // Do not calculate the power consumption, fixed at 100%
    uint8_t battery_percent = 100;
    snprintf(post_buffer, sizeof(post_buffer), post_format, client_id, info.latitude, info.longitude, info.second, info.altitude, battery_percent);

    Serial.print("Request url:"); Serial.println(request_url);
    Serial.print("Post body  :"); Serial.println(post_buffer);

    // Set Post URL
    if (!modem.https_set_url(request_url)) {
        Serial.println("Failed to set the URL. Please check the validity of the URL!");
        return false;
    }
    modem.https_set_user_agent("TinyGSM/LilyGo-A76XX");
    int httpCode = modem.https_post(post_buffer);
    if (httpCode != 200) {
        Serial.print("HTTP post failed ! error code = ");
        Serial.println(httpCode);
        modem.https_end();
        return false;
    }

    modem.https_end();
    return true;
}



bool loopGPS(GPSInfo &info)
{
    bool rlst = modem.getGPS_Ex(info);

    if (rlst) {
        Serial.println();
        Serial.print("FixMode:");
        Serial.println(info.isFix);
        Serial.print("Latitude:");
        Serial.println(info.latitude, 6);
        Serial.print("Longitude:");
        Serial.println(info.longitude, 6);
        Serial.print("Speed:");
        Serial.println(info.speed);
        Serial.print("Altitude:");
        Serial.println(info.altitude);
        Serial.println("Visible Satellites:");
        Serial.print(" GPS Satellites:");
        Serial.println(info.gps_satellite_num);
        Serial.print(" BEIDOU Satellites:");
        Serial.println(info.beidou_satellite_num);
        Serial.print(" GLONASS Satellites:");
        Serial.println(info.glonass_satellite_num);
        Serial.print(" GALILEO Satellites:");
        Serial.println(info.galileo_satellite_num);
        Serial.println("Date Time:");
        Serial.print("Year:");
        Serial.print(info.year);
        Serial.print(" Month:");
        Serial.print(info.month);
        Serial.print(" Day:");
        Serial.println(info.day);
        Serial.print("Hour:");
        Serial.print(info.hour);
        Serial.print(" Minute:");
        Serial.print(info.minute);
        Serial.print(" Second:");
        Serial.println(info.second);
        Serial.print("Course:");
        Serial.println(info.course);
        Serial.print("PDOP:");
        Serial.println(info.PDOP);
        Serial.print("HDOP:");
        Serial.println(info.HDOP);
        Serial.print("VDOP:");
        Serial.println(info.VDOP);
        String gps_raw = modem.getGPSraw();
        Serial.print("GPS/GNSS Based Location String:");
        Serial.println(gps_raw);
    }
    return rlst;
}

void modem_enter_sleep(uint32_t ms)
{
    Serial.println("Enter modem sleep mode!");
#ifdef BOARD_LED_PIN
    digitalWrite(BOARD_LED_PIN, LOW);
#endif
    // Pull up DTR to put the modem into sleep
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, HIGH);

    if (!modem.sleepEnable(true)) {
        Serial.println("modem sleep failed!");
    } else {
        Serial.println("Modem enter sleep modem!");
    }

    light_sleep_delay(ms);

    // Pull down DTR to wake up MODEM
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

#ifdef BOARD_LED_PIN
    digitalWrite(BOARD_LED_PIN, HIGH);
#endif

    // Wait modem wakeup
    light_sleep_delay(500);
}


void setup()
{
    Serial.begin(115200); // Set console baud rate
    Serial.println("Start Sketch");

#ifdef BOARD_LED_PIN
    pinMode(BOARD_LED_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, LOW);
#endif

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef BOARD_POWERON_PIN
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

    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Check if the modem is online
    Serial.println("Start modem...");

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

    while (1) {
        modemName = modem.getModemName();
        if (modemName == "UNKOWN") {
            Serial.println("Unable to obtain module information normally, try again");
            light_sleep_delay(1000);
        } else if (modemName.startsWith("A7670G")) {
            while (1) {
                Serial.println("A7670G does not support built-in GPS function, please "
                               "run examples/GPSShield");
                light_sleep_delay(1000);
            }
        } else {
            Serial.print("Model Name:");
            Serial.println(modemName);
            break;
        }
        light_sleep_delay(5000);
    }

    // Check if SIM card is online
    SimStatus sim = SIM_ERROR;
    while (sim != SIM_READY) {
        sim = modem.getSimStatus();
        switch (sim) {
        case SIM_READY:
            Serial.println("SIM card online");
            break;
        case SIM_LOCKED:
            Serial.println("The SIM card is locked. Please unlock the SIM card first.");
            // const char *SIMCARD_PIN_CODE = "123456";
            // modem.simUnlock(SIMCARD_PIN_CODE);
            break;
        default:
            break;
        }
        light_sleep_delay(1000);
    }

    //SIM7672G Can't set network mode
#ifndef TINY_GSM_MODEM_SIM7672
    if (!modem.setNetworkMode(MODEM_NETWORK_AUTO)) {
        Serial.println("Set network mode failed!");
    }
    String mode = modem.getNetworkModes();
    Serial.print("Current network mode : ");
    Serial.println(mode);
#endif

#ifdef NETWORK_APN
    Serial.printf("Set network apn : %s\n", NETWORK_APN);
    modem.sendAT(GF("+CGDCONT=1,\"IP\",\""), NETWORK_APN, "\"");
    if (modem.waitResponse() != 1) {
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
            light_sleep_delay(1000);
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
            light_sleep_delay(1000);
            break;
        }
    }
    Serial.println();


    Serial.printf("Registration Status:%d\n", status);
    light_sleep_delay(1000);

    String ueInfo;
    if (modem.getSystemInformation(ueInfo)) {
        Serial.print("Inquiring UE system information:");
        Serial.println(ueInfo);
    }

    if (!modem.setNetworkActive()) {
        Serial.println("Enable network failed!");
    }

    light_sleep_delay(5000);

    String ipAddress = modem.getLocalIP();
    Serial.print("Network IP:"); Serial.println(ipAddress);


    modem.sendAT("+SIMCOMATI");
    modem.waitResponse();

    Serial.println("Enabling GPS/GNSS/GLONASS");
    while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
        Serial.print(".");
        light_sleep_delay(500);
    }
    Serial.println();
    Serial.println("GPS Enabled");


    // Set GPS Baud to 115200
    modem.setGPSBaud(115200);

    /*
    * Model: A76XX
    * 1 - GPS L1 + SBAS + QZSS
    * 2 - BDS B1
    * 3 - GPS + GLONASS + GALILEO + SBAS + QZSS
    * 4 - GPS + BDS + GALILEO + SBAS + QZSS.
    *
    * Model: SIM7670G
    * 1  -  GPS
    * 3  -  GPS + GLONASS
    * 5  -  GPS + GALILEO
    * 9  -  GPS + BDS
    * 13 -  GPS + GALILEO + BDS
    * 15 -  GPS + GLONASS + GALILEO + BDS
    * */

    uint8_t gpsMode = 0;
    if (modemName.startsWith("A767")) {
        Serial.println("Set A76XX GNSS MODE = GPS + BDS + GALILEO + SBAS + QZSS");
        gpsMode = 4;
    } else {
        gpsMode = 15;
        Serial.println("Set SIM7670G GNSS MODE = GPS + GLONASS + GALILEO + BDS");
    }
    modem.setGPSMode(gpsMode);


#ifndef TINY_GSM_MODEM_SIM7672
    // GPS acceleration only supports A7670X/A7608X (excluding A7670G and other versions that do not support positioning).
    // SIM7670G does not support GPS acceleration function
    Serial.println("GPS acceleration is enabled");
    if (!modem.enableAGPS()) {
        Serial.println(" failed !!!");
    } else {
        Serial.println(" success!!!");
    }
#endif

}


void loop()
{
    GPSInfo info;
    bool rlst = loopGPS(info);
    if (!rlst) {
        // If positioning is not successful, set ESP to enter light sleep mode to save power consumption
        light_sleep_delay(15000);
    } else {
        rlst = post_location(info);
        if (rlst) {
            // If the positioning is successful and the location is sent successfully,
            // the ESP and modem are set to sleep mode. The sleep mode consumes about 2~3mA
            // For power consumption records, please see README
            modem_enter_sleep(REPORT_LOCATION_RATE_SECOND * 1000);
        } else {
            // If the positioning is successful, if the sending of the position fails,
            // set the ESP to sleep mode and wait for the next sending
            light_sleep_delay(REPORT_LOCATION_RATE_SECOND * 1000);
        }
    }
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
