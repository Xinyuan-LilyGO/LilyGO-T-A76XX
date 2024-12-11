/**
 * @file      SecureClient.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-12-11
 * @note      Secure Client support A7670X/A7608X/SIM7670G
 * 
 * 
 * Tested on the following firmware versions
 * Model: SIM7670G-MNGV
 * Revision: 2374B05SIM767XM5A_M
 * 
 * Model: A7608SA-H
 * Revision: A50C4B12A7600M7 , A7600M7_B12V01_240315
 * 
 * Model: A7670SA-FASE
 * Revision: A011B07A7670M7_F,A7670M7_B07V01_240927
 */

#include "utilities.h"


#ifdef TINY_GSM_MODEM_A7670
#undef TINY_GSM_MODEM_A7670
#endif

#ifdef TINY_GSM_MODEM_A7608
#undef TINY_GSM_MODEM_A7608
#endif

// Use SecureClient
#ifndef TINY_GSM_MODEM_A76XXSSL
#define TINY_GSM_MODEM_A76XXSSL //Support A7670X/A7608X/SIM7670G
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG Serial

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

TinyGsmClientSecure client(modem);

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
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

    // Turn on modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(1000);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    Serial.println("Start modem...");
    delay(3000);

    // Check if the modem is online
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

    delay(5000);

    String ipAddress = modem.getLocalIP();
    Serial.print("Network IP:"); Serial.println(ipAddress);
}

const char *request_url[] = {
    "httpbin.org",
    "vsh.pp.ua",
    "ipapi.co",
    "ip-api.com"
};

const char *request_resource[] = {
    "/get",
    "/TinyGSM/logo.txt",
    "/timezone",
    "/json/23.158.104.183"
};

uint16_t request_server_port[] = {
    443,    //https://httpbin.org/get
    443,    //https://vsh.pp.ua/TinyGSM/logo.txt
    443,    //https://ipapi.co/timezone
    80      //http://ip-api.com/json/23.158.104.183 , "SSL unavailable for this endpoint, order a key at https://members.ip-api.com/"
};

void loop()
{
    for (int i = 0; i < sizeof(request_url) / sizeof(request_url[0]); ++i) {
        // Make https  request
        HttpClient    http(client, request_url[i], request_server_port[i]);

        Serial.print("Request URL : ");
        Serial.print(request_url[i]);
        Serial.print(" Port:");
        Serial.println(request_server_port[i]);

        Serial.print(F("Performing HTTPS GET request... "));
        http.connectionKeepAlive();  // Currently, this is needed for HTTPS
        int err = http.get(request_resource[i]);
        if (err != 0) {
            Serial.println(F("failed to connect"));
            continue;
        }

        int status = http.responseStatusCode();
        Serial.print(F("Response status code: "));
        Serial.println(status);
        if (!status) {
            continue;
        }

        Serial.println(F("Response Headers:"));
        while (http.headerAvailable()) {
            String headerName  = http.readHeaderName();
            String headerValue = http.readHeaderValue();
            Serial.println("    " + headerName + " : " + headerValue);
        }

        int length = http.contentLength();
        if (length >= 0) {
            Serial.print(F("Content length is: "));
            Serial.println(length);
        }
        if (http.isResponseChunked()) {
            Serial.println(F("The response is chunked"));
        }

        String body = http.responseBody();
        Serial.println(F("Response:"));
        Serial.println(body);

        Serial.print(F("Body length is: "));
        Serial.println(body.length());

        http.stop();
        Serial.println(F("Server disconnected"));

    }

    Serial.println("All test done ..");

    // Disconnect network
    // modem.setNetworkDeactivate();

    while (true) {
        if (SerialAT.available()) {
            Serial.write(SerialAT.read());
        }
        if (Serial.available()) {
            SerialAT.write(Serial.read());
        }
        delay(1);
    }
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
