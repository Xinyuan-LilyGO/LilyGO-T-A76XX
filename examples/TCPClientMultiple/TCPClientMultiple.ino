/**
 * @file      TCPClientMultiple.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-02-07
 * @note      The example demonstrates multiple socket connections. The problem comes from https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/223#issuecomment-2639376887
 */
#warning "SIM7600 has not been tested yet."
#include "utilities.h"

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

// The first parameter is the TinyGsm object, the second is the socket index number to be used, and a maximum of 10 sockets can be used
TinyGsmClient client1(modem, 0);
TinyGsmClient client2(modem, 1);
TinyGsmClient client3(modem, 2);
TinyGsmClient client4(modem, 3);
TinyGsmClient client5(modem, 4);
TinyGsmClient *client[] = {&client1, &client2, &client3, &client4, &client5};


const char *url[]  = {
    "httpbin.org",
    "httpbin.org",
    "httpbin.org",
    "httpbin.org",
    "httpbin.org",
};

const char *resource[] = {
    "/get",
    "/delete",
    "/patch",
    "/post",
    "/put",
};

const char *method[] = {
    "GET ",
    "DELETE ",
    "PATCH ",
    "POST ",
    "PUT ",
};

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

    // Set modem reset
#ifdef MODEM_RESET_PIN
    pinMode(MODEM_RESET_PIN, OUTPUT);
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
    delay(1000);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

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
        delay(1000);
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

#ifdef MODEM_REG_SMS_ONLY
    while (status == REG_SMS_ONLY) {
        Serial.println("Registered for \"SMS only\", home network (applicable only when E-UTRAN), this type of registration cannot access the network. Please check the APN settings and ask the operator for the correct APN information and the balance and package of the SIM card. If you still cannot connect, please replace the SIM card and test again. Related ISSUE: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/307#issuecomment-3034800353");
        delay(5000);
    }
#endif

    Serial.printf("Registration Status:%d\n", status);
    delay(1000);

    String ueInfo;
    if (modem.getSystemInformation(ueInfo)) {
        Serial.print("Inquiring UE system information:");
        Serial.println(ueInfo);
    }

    if (!modem.setNetworkActive()) {
        Serial.println("Enable network failed!");
    }

    String ipAddress = modem.getLocalIP();
    Serial.print("Network IP:"); Serial.println(ipAddress);

    while (!modem.waitForNetwork(1000, true)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
    Serial.println("Network is connected!");
}



void loop()
{
    for (int i = 0; i < sizeof(url) / sizeof(url[0]); ++i) {
        Serial.printf("Try connect to %s \n", url[i]);
        if (client[i]->connect(url[i], 80)  == 1) {
            Serial.printf("client %d connect success\n", i);
        } else {
            Serial.printf("client %d connect failed\n", i);
        }
    }
    for (int i = 0; i < sizeof(url) / sizeof(url[0]); ++i) {
        if (!client[i]->connected()) {
            Serial.printf("socket %d is disconnected!", i);
            continue;
        }
        Serial.printf("\n\nTry get request %s!\n============================\n\n", resource[i]);
        client[i]->print(String(method[i]) + resource[i] + " HTTP/1.0\r\n");
        client[i]->print(String("Host: ") + url[i] + "\r\n");
        client[i]->print("Connection: close\r\n\r\n");
        // Wait for data to arrive
        uint32_t startS = millis();
        while (client[i]->connected() && !client[i]->available() &&
                millis() - startS < 30000L) {
            delay(100);
        };
        while (client[i]->connected() && millis() - startS < 10000L) {
            while (client[i]->available()) {
                int c = client[i]->read();
                Serial.write(c);
                startS = millis();
            }
        }
        Serial.println();
        Serial.println("Request done!\n\n============================\n\n");
    }
    Serial.println("All connect test done!");
    for (int i = 0; i < sizeof(url) / sizeof(url[0]); ++i) {
        // Generally, after the request is completed, the socket will be automatically closed.
        // Here you can manually query the socket that has not been closed.
        if (client[i]->connected()) {
            client[i]->stop();
        }
        Serial.printf("stop client %d connect\n", i);
    }
    while (1) {
        delay(1000);
    }
}


#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
