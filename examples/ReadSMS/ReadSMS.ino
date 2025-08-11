/**
 * @file      ReadSMS.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-08-29
 * @note      SIM7670G does not support SMS and voice functions
 *            `A7670E-LNXY-UBL` this version does not support voice and SMS functions.
 * ! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual
 * 
 */

#include "utilities.h"

#ifdef TINY_GSM_MODEM_SIM7080
#error "This modem not sms function"
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


// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom

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

    String modemName = "UNKOWN";
    while (1) {
        modemName = modem.getModemName();
        if (modemName == "UNKOWN") {
            Serial.println("Unable to obtain module information normally, try again");
            delay(1000);
        } else if (modemName.startsWith("SIM7670")) {
            while (1) {
                Serial.println("SIM7670 does not support SMS Function");
                delay(1000);
            }
        } else {
            Serial.print("Model Name:");
            Serial.println(modemName);
            break;
        }
        delay(5000);
    }

    // Wait PB DONE
    Serial.println("Wait SMS Done.");
    if (!modem.waitResponse(100000UL, "SMS DONE")) {
        Serial.println("Can't wait from sms register ....");
        return;
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


    //! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual
    //! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual
    //! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual
    //! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual
    //! Only read data, not decode data. For detailed SMS operation, please refer to A76XX_Series_AT_Command_Manual

    // A76XX_Series_AT_Command_Manual_V1.12.pdf : https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/datasheet/A76XX/A76XX_Series_AT_Command_Manual_V1.12.pdf
    // A76XX Series_SMS_Application Note_V3.00.pdf : https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/datasheet/A76XX/A76XX%20Series_SMS_Application%20Note_V3.00.pdf

    String data;
    int8_t res;

    // Set SMS system into text mode
    modem.sendAT("+CMGF=1");
    modem.waitResponse(10000);

    // Listing all SMS messages.
    modem.sendAT("+CMGL=\"ALL\"");
    res = modem.waitResponse(10000UL, data);
    if (res == 1) {
        Serial.print("ALL MSG Data:");
        data.replace("\r\nOK\r\n", "");
        data.replace("\rOK\r", "");
        data.trim();
        Serial.println(data);
    } else {
        Serial.print("Read all message failed");
    }

    Serial.println("==================================");

    delay(3000);
    data = "";
    // Reading the message again changes the status to "READ" from "UNREAD"
    modem.sendAT("+CMGR=1");    //Index 1 is the latest message
    res = modem.waitResponse(1000, data);
    if (res == 1) {
        Serial.print("Last Data:");
        data.replace("\r\nOK\r\n", "");
        data.replace("\rOK\r", "");
        data.trim();
        Serial.println(data);
    } else {
        Serial.print("Read message failed");
    }

    Serial.println("==================================");

    delay(3000);
    data = "";
    // Read the second to last SMS message
    modem.sendAT("+CMGR=2");    //Index 2 is the latest message
    res = modem.waitResponse(1000, data);
    if (res == 1) {
        Serial.print("Sec 2 Data:");
        data.replace("\r\nOK\r\n", "");
        data.replace("\rOK\r", "");
        data.trim();
        Serial.println(data);
    } else {
        Serial.print("Read message failed");
    }

    Serial.println("==================================");

    // Delete an SMS message.
    /*
    modem.sendAT("+CMGD=1");  //Index 1 is the latest message
    res = modem.waitResponse(1000UL, data);
    if (res == 1) {
        Serial.print("Delete message success");
    } else {
        Serial.print("Delete message failed");
    }
    */

}

void loop()
{

    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }
    delay(1);
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
