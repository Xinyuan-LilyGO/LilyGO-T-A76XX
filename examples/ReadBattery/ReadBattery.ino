/**
 * @file      ReadBattery.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-08-07
 * @note      Reading the battery voltage information is only applicable to the V1.2 version.
 *            T-A7670x :  The V1.1 version does not have a battery voltage divider.
 *                         If V1.1 needs to be read, then you need to add a 100K 1% voltage divider resistor between the battery and GND
 *                         When connected to the USB, the battery voltage data read is not the real battery voltage, so the battery
 *                         voltage is sent to the UDP Server through UDP. When using it, please disconnect the USB-C
 *            T-A7670x :  Only version V1.4 has the resistor divider connected to the solar input, other versions IO38 is not connected
 *            T-SIM7600 series ：When the USB is connected, the battery mV is 0 because the battery voltage cannot be read when the USB is plugged in.
 *
 * @note      Only support T-A7670 ,T-A7608X, T-SIM7672G board , not support T-Call A7670 , T-PCIE-A7670
 *
 *            The AT+CBC command only supports the following versions , Other versions cannot read it because the hardware is not connected.
 *              1. T-A7670X-S3-Standard
 *              2. T-SIM7000G-S3-Standard
 *              3. T-SIM7080G-S3-Standard
 *              4. T-SIM7670G-S3-Standard
 *
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp32-hal-adc.h>
#include "utilities.h"


#ifndef BOARD_BAT_ADC_PIN
#error "No support this board"
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon
/*
* Only T-A767X-ESP32S3 version. Other versions cannot use AT+CBC to read the battery
* voltage because the hardware is not connected.
* */
#ifdef MODEM_CONNECTED_ADC_PIN
#include <TinyGsmClient.h>
#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
#endif

// WiFi network name and password:
const char *networkName = "your-ssid";
const char *networkPswd = "your-password";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char *udpAddress = "192.168.36.132";
const int udpPort = 3336;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

uint32_t timeStamp = 0;
char buf[256];

void connectToWiFi(const char *ssid, const char *pwd)
{
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    //register event handler
    WiFi.onEvent(WiFiEvent);

    //Initiate connection
    WiFi.begin(ssid, pwd);

    Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event)
{
    switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        //When connected set
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());
        //initializes the UDP state
        //This initializes the transfer buffer
        udp.begin(WiFi.localIP(), udpPort);
        connected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        connected = false;
        break;
    default: break;
    }
}


void setup()
{
    Serial.begin(115200); // Set console baud rate

#ifdef BOARD_POWERON_PIN
    /* Set Power control pin output
    * * @note      Known issues, ESP32 (V1.2) version of T-A7670, T-A7608,
    *            when using battery power supply mode, BOARD_POWERON_PIN (IO12) must be set to high level after esp32 starts, otherwise a reset will occur.
    * */
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif


    /*
    * Only T-A767X-ESP32S3 version. Other versions cannot use AT+CBC to read the battery
    * voltage because the hardware is not connected.
    * */
#ifdef MODEM_CONNECTED_ADC_PIN
    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

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

    Serial.println("Start modem...");
    delay(3000);

    int retry = 0;
    while (!modem.testAT(1000)) {
        Serial.println(".");
        if (retry++ > 30) {
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_PWRKEY_PIN, HIGH);
            delay(MODEM_POWERON_PULSE_WIDTH_MS);
            digitalWrite(BOARD_PWRKEY_PIN, LOW);
            retry = 0;
        }
    }
    Serial.println();
    delay(200);


#endif

    //Connect to the WiFi network
    connectToWiFi(networkName, networkPswd);


    //adc setting start

    // You don't need to set it, because the values ​​are all default. The current version is Arduino 3.0.4, and the subsequent versions are uncertain.

    analogSetAttenuation(ADC_11db);

    analogReadResolution(12);

#if CONFIG_IDF_TARGET_ESP32
    analogSetWidth(12);
#endif

    //adc setting end
}

void loop()
{
    //only send data when connected
    if (connected) {
        if (millis() - timeStamp > 1000) {
            timeStamp = millis();

            uint32_t battery_voltage = analogReadMilliVolts(BOARD_BAT_ADC_PIN);
            battery_voltage *= 2;   //The hardware voltage divider resistor is half of the actual voltage, multiply it by 2 to get the true voltage

#ifdef BOARD_SOLAR_ADC_PIN
            uint32_t solar_voltage = analogReadMilliVolts(BOARD_SOLAR_ADC_PIN);
            solar_voltage *= 2;     //The hardware voltage divider resistor is half of the actual voltage, multiply it by 2 to get the true voltage
            snprintf(buf, 256, "Battery:%umV \tSolar:%umV", battery_voltage, solar_voltage);
#else
            snprintf(buf, 256, "Battery:%umV ", battery_voltage);
#endif



            /*
            * modem.getBattVoltage() See sketch title
            * */
#ifdef MODEM_CONNECTED_ADC_PIN
            uint16_t modem_voltage = modem.getBattVoltage();
            Serial.printf("Modem voltage:%u mV\n", modem_voltage);
            snprintf(buf, 256, "%s Modem voltage:%u mV\n", buf, modem_voltage);
#endif

            // When connected to the USB, the battery voltage data read is not the real battery voltage,
            // so the battery voltage is sent to the UDP Server through UDP. When using it, please disconnect the USBC
            Serial.println(buf);

            //Send a packet
            udp.beginPacket(udpAddress, udpPort);
            udp.println(buf);
            udp.endPacket();
        }
    }
}
