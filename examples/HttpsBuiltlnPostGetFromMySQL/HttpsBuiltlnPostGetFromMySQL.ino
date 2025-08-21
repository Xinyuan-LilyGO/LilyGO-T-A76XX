/**
 * @file      HttpsBuiltlnPostGetFromMySQL.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2025-08-21
 * @note
 * * Example is suitable for A7670X/A7608X/SIM7670G/SIM7000G/SIM7080G/SIM7600 series
 * * This example connects to a self-hosted server at lewishe.pro to insert data into a MySQL database, 
 * * then sends a GET request to the server to retrieve the injected data using a unique identifier. 
 * * This server is a simple HTTPS API for users to test. Please do not maliciously attack the server.
 * 
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 */
// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

#include "utilities.h"
#include <TinyGsmClient.h>
#include <cJSON.h>
#include <WiFi.h>
#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#include "examplePayload.h"

// It depends on the operator whether to set up an APN. If some operators do not set up an APN,
// they will be rejected when registering for the network. You need to ask the local operator for the specific APN.
// APNs from other operators are welcome to submit PRs for filling.
// #define NETWORK_APN     "CHN-CT"             //CHN-CT: China Telecom

const char *server_url =  "https://lewishe.pro";


char *create_json_payload(const char* api_key, const char* data_str);


void setup()
{
    Serial.begin(115200); // Set console baud rate

    Serial.println("Start Sketch");

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

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

    // Turn on the modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(MODEM_POWERON_PULSE_WIDTH_MS);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Check if the modem is online
    Serial.println("Start modem...");

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

#ifdef TINY_GSM_MODEM_HAS_NETWORK_MODE
    if (!modem.setNetworkMode(MODEM_NETWORK_AUTO)) {
        Serial.println("Set network mode failed!");
    }
    String mode = modem.getNetworkModeString();
    Serial.print("Current network mode : ");
    Serial.println(mode);
#endif

#ifdef TINY_GSM_MODEM_HAS_PREFERRED_MODE
    if (!modem.setPreferredMode(MODEM_PREFERRED_CATM_NBIOT)) {
        Serial.println("Set network preferred failed!");
    }
    String prefMode = modem.getPreferredModeString();
    Serial.print("Current preferred mode : ");
    Serial.println(prefMode);
#endif



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

    delay(5000);

    String ipAddress = modem.getLocalIP();
    Serial.print("Network IP:"); Serial.println(ipAddress);


    // Print modem software version
    String res;
    modem.sendAT("+SIMCOMATI");
    modem.waitResponse(10000UL, res);
    Serial.println(res);

    Serial.println("Please make sure you are using the latest released version of the firmware. Find the latest version here: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/docs/update_fw.md");
    Serial.println("If you still have problems with the latest firmware, please open an issue. Otherwise, please do not create meaningless issues.");


    // Use ESP MAC as unique identifier
    String macString = WiFi.macAddress();
    macString.replace(":", "");


    // Initialize HTTPS
    modem.https_begin();

    char url_buffer[256];

    // Set the post request url
    snprintf(url_buffer, 256, "%s/api/insert", server_url);

    if (!modem.https_set_url(url_buffer)) {
        Serial.println("Failed to set the URL. Please check the validity of the URL!");
        return;
    }

    modem.https_set_content_type("application/json");
    modem.https_set_user_agent("TinyGSM/LilyGo-Sim-Series");

    // Pack 2545 bytes of data into JSON and send it to the database
    // macString as a unique identifier
    char *json = create_json_payload(macString.c_str(), examplePayload);
    int httpCode = modem.https_post(json);
    if (httpCode != 200) {
        Serial.print("HTTP post failed ! error code = ");
        Serial.println(httpCode);
        free(json);
        return;
    }   

    // Freeing memory
    free(json);

    // Get HTTPS header information
    String header = modem.https_header();
    Serial.print("HTTP Header : ");
    Serial.println(header);

    // Get HTTPS response
    String body = modem.https_body();
    Serial.print("HTTP body : ");
    Serial.println(body);

    Serial.println("=============================================");

    // Set the get request url
    snprintf(url_buffer, 256, "%s/api/get-last?api_key=%s", server_url, macString.c_str());
    Serial.println("Set request url");
    if (!modem.https_set_url(url_buffer)) {
        Serial.println("Failed to set the URL. Please check the validity of the URL!");
        return;
    }

    httpCode = modem.https_get();
    if (httpCode != 200) {
        Serial.print("HTTP get failed ! error code = ");
        Serial.println(httpCode);
        // Disconnect http server
        modem.https_end();
        return;
    }

    // Get HTTPS header information
    header = modem.https_header();
    Serial.print("HTTP Header : ");
    Serial.println(header);

    delay(1000);

    // Get HTTPS response
    body = modem.https_body();
    Serial.print("HTTP body : ");
    Serial.println(body);

    delay(3000);

    // Disconnect http server
    modem.https_end();
}

void loop()
{
    // Debug AT
    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }
    delay(1);
}

char *create_json_payload(const char* api_key, const char* data_str)
{
    cJSON* root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }
    cJSON_AddStringToObject(root, "api_key", api_key);
    cJSON_AddStringToObject(root, "data", data_str);
    char *json_str = cJSON_Print(root);
    cJSON_Delete(root);
    return json_str;
}


#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif

/*
SIM7600 Version OK 20250709
AT+SIMCOMATI
Manufacturer: SIMCOM INCORPORATED
Model: SIMCOM_SIM7600G-H
Revision: LE20B04SIM7600G22
QCN:
IMEI: xxxxxxxxxxxx
MEID:
+GCAP: +CGSM
DeviceInfo: 173,170

SIM7000G    # 2025/07/10:OK!
Revision:1529B11SIM7000G
CSUB:V01
APRev:1529B11SIM7000,V01
QCN:MDM9206_TX3.0.SIM7000G_P1.03C_20240911

Revision:1529B11SIM7000G
CSUB:V01
APRev:1529B11SIM7000,V01
QCN:MDM9206_TX3.0.SIM7000G_P1.02_20180726


SIM7080G    # 2025/08/07:OK!
Revision:1951B16SIM7080
CSUB:B16V01
APRev:1951B16SIM7080,B16V01
QCN:SIM7080G_P1.03_20210823

A7670E-FASE  # 20250821
Manufacturer: INCORPORATED
Model: A7670E-FASE
Revision: A011B07A7670M7_F
A7670M7_B07V01_240927

*/
