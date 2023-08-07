/**
 * @file      readBattery.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-08-07
 * @note      Reading the battery voltage information is only applicable to the V1.2 version.
 *            The V1.1 version does not have a battery voltage divider.
 *            If V1.1 needs to be read, then you need to add a 100K 1% voltage divider resistor between the battery and GND
 *            When connected to the USB, the battery voltage data read is not the real battery voltage, so the battery
 *            voltage is sent to the UDP Server through UDP. When using it, please disconnect the USBC
 */
#include <esp_adc_cal.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>


#define ADC_PIN             35
#define POWER_ENABLE_PIN    12


// WiFi network name and password:
const char *networkName = "your-ssid";
const char *networkPswd = "your-password";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char *udpAddress = "192.168.36.14";
const int udpPort = 3333;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

int vref = 1100;
uint32_t timeStamp = 0;


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

    pinMode(POWER_ENABLE_PIN, OUTPUT);
    digitalWrite(POWER_ENABLE_PIN, HIGH);


    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);    //Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
        vref = adc_chars.vref;
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    } else {
        Serial.println("Default Vref: 1100mV");
    }

    //Connect to the WiFi network
    connectToWiFi(networkName, networkPswd);

}

void loop()
{
    //only send data when connected
    if (connected) {
        if (millis() - timeStamp > 1000) {
            timeStamp = millis();
            uint16_t v = analogRead(ADC_PIN);
            float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
            String voltage = "Voltage :" + String(battery_voltage) + "V\n";

            //    When connected to the USB, the battery voltage data read is not the real battery voltage,
            // so the battery voltage is sent to the UDP Server through UDP. When using it, please disconnect the USBC
            Serial.println(voltage);

            //Send a packet
            udp.beginPacket(udpAddress, udpPort);
            udp.printf(voltage.c_str());
            udp.endPacket();
        }

    }
}
