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
 *            T-A7670x :  Only version V1.3 has the resistor divider connected to the solar input, other versions IO38 is not connected
 * @note      Onlu support T-A7670 ,T-A7608X, T-SIM7672G board , not support T-Call A7670
 */
#include <esp_adc_cal.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "utilities.h"

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
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif


    //Connect to the WiFi network
    connectToWiFi(networkName, networkPswd);

}

void loop()
{
    //only send data when connected
    if (connected) {
        if (millis() - timeStamp > 1000) {
            timeStamp = millis();
            esp_adc_cal_characteristics_t adc_chars;
            esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
            uint16_t battery_voltage = esp_adc_cal_raw_to_voltage(analogRead(BOARD_BAT_ADC_PIN), &adc_chars) * 2;

#ifdef BOARD_SOLAR_ADC_PIN
            uint16_t solar_voltage = esp_adc_cal_raw_to_voltage(analogRead(BOARD_SOLAR_ADC_PIN), &adc_chars) * 2;
#else
            uint16_t solar_voltage = 0;
#endif

            snprintf(buf, 256, "Battery:%umV \tSolar:%umV", battery_voltage, solar_voltage);

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
