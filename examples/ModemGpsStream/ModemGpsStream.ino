/**
 * @file      ModemGpsStream.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-07-12
 * @note
 *      This sketch is only applicable to the
 *      1. T-A7670X-S3-Standard
 *      2. T-SIM7670G-S3-Standard
 *      Other models are not supported
 */
#include "utilities.h"


#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1
#define SerialGPS   Serial2

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS
// Decode GPS stream, if not defined then output NMEA
#define DECODER_GPS_STREAM


#include <TinyGsmClient.h>
#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

bool external_gps_module = false;

bool pps_signal = false;


#ifdef DECODER_GPS_STREAM
#include <TinyGPSPlus.h>
TinyGPSPlus gps;
static void printFloat(float val, bool valid, int len, int prec)
{
    if (!valid) {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    } else {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
}

static void printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (!d.isValid()) {
        Serial.print(F("********** "));
    } else {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        Serial.print(sz);
    }

    if (!t.isValid()) {
        Serial.print(F("******** "));
    } else {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        Serial.print(sz);
    }

    printInt(d.age(), d.isValid(), 5);
}

static void printStr(const char *str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i)
        Serial.print(i < slen ? str[i] : ' ');
}

void printGPS()
{
    static uint32_t interval = 0;
    static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

    if (millis() < interval) {
        return;
    }

    if (millis() > 10000 && gps.charsProcessed() < 10) {
        Serial.println(F("No GPS data received,Please confirm that the GPS module exists"));
        return;
    }

    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
    printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

    unsigned long distanceKmToLondon =
        (unsigned long)TinyGPSPlus::distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            LONDON_LAT,
            LONDON_LON) / 1000;
    printInt(distanceKmToLondon, gps.location.isValid(), 9);

    double courseToLondon =
        TinyGPSPlus::courseTo(
            gps.location.lat(),
            gps.location.lng(),
            LONDON_LAT,
            LONDON_LON);

    printFloat(courseToLondon, gps.location.isValid(), 7, 2);

    const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

    printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

    printInt(gps.charsProcessed(), true, 6);
    printInt(gps.sentencesWithFix(), true, 10);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();

    interval = millis() + 1000;
}

#endif

void setup()
{
    Serial.begin(115200);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    // Turn on DC boost to power on the modem
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

    // Pull down DTR to ensure the modem is not in sleep state
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    // Turn on modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);

    Serial.print("Modem starting...");
    while (!modem.testAT(5000)) {
        Serial.println(".");
        digitalWrite(BOARD_PWRKEY_PIN, LOW);
        delay(100);
        digitalWrite(BOARD_PWRKEY_PIN, HIGH);
        delay(MODEM_POWERON_PULSE_WIDTH_MS);
        digitalWrite(BOARD_PWRKEY_PIN, LOW);
    }
    Serial.println();

    delay(2000);

    String modemName = "UNKNOWN";
    while (1) {
        modemName = modem.getModemName();
        if (modemName == "UNKNOWN") {
            Serial.println("Unable to obtain module information normally, try again");
            delay(1000);
        } else if (modemName.startsWith("A7670E-FASE") || modemName.startsWith("A7670SA-FASE")) {
            Serial.println("Modem support built-in GPS function, keep running");
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
        delay(5000);
    }



    if (!external_gps_module) {
        int retry = 15;
        Serial.println("Start the Modem GPS module and use the default 115200 baud rate for communication");
        while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO, MODEM_GPS_ENABLE_LEVEL)) {
            Serial.print(".");
            if (retry-- <= 0) {
                Serial.println("GPS startup failed. Please check whether the board you ordered contains GPS function.");
                delay(1000);
            }
            delay(2000);
        }
        Serial.println();

        Serial.println("GPS Enabled");

        // Set the communication baud rate
        modem.setGPSBaud(115200);

        // If you have a GPS backup battery connected, you can try a hot start.
        modem.gpsHotStart();

#if defined(TINY_GSM_MODEM_A7670) || defined(TINY_GSM_MODEM_A7608)
        modem.setGPSMode(GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS);
#elif defined(TINY_GSM_MODEM_SIM7670G)
        modem.setGPSMode(GNSS_MODE_GPS_GLONASS_BDS);
#elif defined(TINY_GSM_MODEM_SIM7600)
        modem.setGPSMode(GNSS_MODE_ALL);
#elif defined(TINY_GSM_MODEM_SIM7000SSL) || defined(TINY_GSM_MODEM_SIM7000)
        modem.setGPSMode(GNSS_MODE_ALL);
#endif

#if defined(TINY_GSM_MODEM_SIM7000SSL) || defined(TINY_GSM_MODEM_SIM7000)
        // Redirect SIM70XX modem GPS NMEA to UART for ESP processing
        modem.configGNSS_OutputPort(NMEA_TO_UART3_PORT);
#endif

        modem.configNMEASentence(NMEA_GPGGA | NMEA_GPGSA | NMEA_GPGSV | NMEA_GPRMC);

        // Modem gps baudrate is 115200
        SerialGPS.begin(115200, SERIAL_8N1, MODEM_GPS_TX_PIN, MODEM_GPS_RX_PIN);
    } else {

        Serial.println("Start the external GPS module and use the default 9600 baud rate for communication");

        // External GPS module wake-up pin, high level working, low level sleep
        pinMode(GPS_SHIELD_WAKEUP_PIN, OUTPUT);
        digitalWrite(GPS_SHIELD_WAKEUP_PIN, HIGH);

        // External GPS module baudrate is 115200
        SerialGPS.begin(9600, SERIAL_8N1, MODEM_GPS_TX_PIN, MODEM_GPS_RX_PIN);
    }



    Serial.println("Next you should see NMEA sentences in the serial monitor");

    // PPS is only for SIM7670G/A7670X series. SIM7000G/SIM7080G/SIM7600X does not have PPS.
    // Register the PPS pin as an interrupt and test the PPS signal
    pinMode(MODEM_GPS_PPS_PIN, INPUT);
    attachInterrupt(MODEM_GPS_PPS_PIN, []() {
        pps_signal = true;
    }, RISING);

#ifdef DECODER_GPS_STREAM
    Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
    Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
    Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
#endif

}

void loop()
{
    if (pps_signal) {
        pps_signal = false;
        Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>PPS");
    }

    while (SerialGPS.available()) {
        int  ch = SerialGPS.read();
#ifdef DECODER_GPS_STREAM
        gps.encode(ch);
#else
        Serial.write(ch);
#endif
    }

#ifdef DECODER_GPS_STREAM
    printGPS();
#endif

    while (SerialAT.available()) {
        int  ch = SerialAT.read();
        Serial.write(ch);
    }

    while (Serial.available()) {
        SerialAT.write(Serial.read());
    }
}


#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
