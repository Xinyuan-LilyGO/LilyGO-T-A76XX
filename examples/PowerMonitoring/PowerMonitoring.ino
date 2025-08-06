/**
 * @file      PowerMonitoring.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-08-23
 * @note      Reading the battery voltage information is only applicable to the V1.2 version.
 *            T-A7670x :  The V1.1 version does not have a battery voltage divider.
 *                         If V1.1 needs to be read, then you need to add a 100K 1% voltage divider resistor between the battery and GND
 *                         When connected to the USB, the battery voltage data read is not the real battery voltage, so the battery
 *                         voltage is sent to the UDP Server through UDP. When using it, please disconnect the USB-C
 *            T-A7670x :  Only version V1.4 has the resistor divider connected to the solar input, other versions IO38 is not connected
 * @note      Only support T-A7670 ,T-A7608X, T-SIM7672G board , not support T-Call A7670 , T-PCIE-A7670
 */

#include "utilities.h"

#ifndef BOARD_BAT_ADC_PIN
#error "No support this board"
#endif

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS


#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define LOW_VOLTAGE_LEVEL       3600            //Sleep shutdown voltage
#define WARN_VOLTAGE_LEVEL      3700            //Warning voltage

#define SLEEP_MINUTE            60              //Sleep time ,unit / minute

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom

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


#ifdef BOARD_BAT_ADC_PIN
#include <vector>
#include <algorithm>
#include <numeric>
// Calculate the average power data
uint32_t getBatteryVoltage()
{
    std::vector<uint32_t> data;
    for (int i = 0; i < 30; ++i) {
        uint32_t val = analogReadMilliVolts(BOARD_BAT_ADC_PIN);
        // Serial.printf("analogReadMilliVolts : %u mv \n", val * 2);
        data.push_back(val);
        delay(30);
    }
    std::sort(data.begin(), data.end());
    data.erase(data.begin());
    data.pop_back();
    int sum = std::accumulate(data.begin(), data.end(), 0);
    double average = static_cast<double>(sum) / data.size();
    return  average * 2;
}
#endif



void setup()
{
    Serial.begin(115200);
    // Turn on DC boost to power on the modem
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif


#ifdef BOARD_BAT_ADC_PIN
    uint32_t battery_voltage_mv = getBatteryVoltage();

// SIM7670G usb connected battery voltage is zero
#ifdef TINY_GSM_MODEM_SIM7672
    if (battery_voltage_mv == 0) {
        Serial.println("USB may already be connected...Skip check");
    } else
#endif

        // If the battery level is lower than 3.6V, the system will continue to sleep and wake up after one hour to continue testing.
        if (battery_voltage_mv < LOW_VOLTAGE_LEVEL) {
            Serial.printf("Battery voltage is too low ,%u mv, entering sleep mode\n", battery_voltage_mv);
            deep_sleep(SLEEP_MINUTE * 60 * 1000); //60 minute
        }
    Serial.printf("Battery voltage is %u mv\n", battery_voltage_mv);
#endif


    // Set modem reset pin ,reset modem
#ifdef MODEM_RESET_PIN
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
#endif

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

}

uint32_t voltage_interval = 0;



void loop()
{


    if (millis() > voltage_interval) {

        // Check the battery voltage every 30 seconds
        voltage_interval = millis() + 30000;

        uint32_t battery_voltage_mv = getBatteryVoltage();


        // If the battery level is lower than 3.6V, the system will continue to sleep and wake up after one hour to continue testing.

        if (battery_voltage_mv < LOW_VOLTAGE_LEVEL) {

            Serial.printf("Battery voltage is too low ,%u mv, entering sleep mode\n", battery_voltage_mv);

            // Turn off the modem
            modem.poweroff();

            // Sleep esp32
            deep_sleep(SLEEP_MINUTE * 60 * 1000); //60 minute

        } else if (battery_voltage_mv < WARN_VOLTAGE_LEVEL) {

            Serial.println("Battery voltage reaches the warning voltage");

        }

    }

    // Do what you need to do here





    // Light sleep for one second to reduce running power consumption
    light_sleep(1000);
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
