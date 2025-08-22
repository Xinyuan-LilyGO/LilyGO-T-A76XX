/**
 * @file      MqttsBuiltlnAzureIOTHub.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2025-08-22
 * @note
 * * The example test is on A7670G. Other models should have the same behavior. If you encounter any errors, please file an issue.
 * * Connect to Azure IOT Hub. Please fill in the correct device name and SAS token before connecting.
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 */

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

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

#define IOTHUB_HOSTNAME  "Your Azure Hub"
#define DEVICE_ID  "Your device name"
#define SAS_TOKEN  "SAS token is filled in here and generated through azure cli"
#define MQTT_PORT  8883

#define USERNAME    IOTHUB_HOSTNAME "/" DEVICE_ID "/?api-version=2021-04-12"
#define DEVICE_TO_CLOUD_TOPIC "devices/" DEVICE_ID "/messages/events/"
#define CLOUD_TO_DEVICE_TOPIC "devices/" DEVICE_ID "/messages/devicebound/#"


// MQTT details
const char *broker = IOTHUB_HOSTNAME;
const uint16_t broker_port = MQTT_PORT;
const char *client_id = DEVICE_ID;
const char *username = USERNAME;
const char *password = SAS_TOKEN;
const char *subscribe_topic = CLOUD_TO_DEVICE_TOPIC;
const char *publish_topic = DEVICE_TO_CLOUD_TOPIC;

// Current connection index, range 0~1
const uint8_t mqtt_client_id = 0;
uint32_t check_connect_millis = 0;

void mqtt_callback(const char *topic, const uint8_t *payload, uint32_t len)
{
    Serial.println();
    Serial.println("======mqtt_callback======");
    Serial.print("Topic:"); Serial.println(topic);
    Serial.println("Payload:");
    for (int i = 0; i < len; ++i) {
        Serial.print(payload[i], HEX); Serial.print(",");
    }
    Serial.println();
    Serial.println("=========================");
}

bool mqtt_connect()
{
    Serial.print("Connecting to ");
    Serial.print(broker);

    bool ret = modem.mqtt_connect(mqtt_client_id, broker, broker_port, client_id, username, password);
    if (!ret) {
        Serial.println("Failed!"); return false;
    }
    Serial.println("successfully.");

    if (modem.mqtt_connected()) {
        Serial.println("MQTT has connected!");
    } else {
        return false;
    }
    // Set MQTT processing callback
    modem.mqtt_set_callback(mqtt_callback);
    // Subscribe to topic
    modem.mqtt_subscribe(mqtt_client_id, subscribe_topic);

    return true;
}


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

    // Initialize MQTT, use SSL, skip authentication server
    modem.mqtt_begin(true);


    if (!mqtt_connect()) {
        Serial.println("Please make sure you are using the latest released version of the firmware. Find the latest version here: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/docs/update_fw.md");
        Serial.println("If you still have problems with the latest firmware, please open an issue. Otherwise, please do not create meaningless issues.");
        Serial.println("If the firmware is up to date, please check that your token and device name are correct.");
        return ;
    }

    while (1) {
        // Check the connection every ten seconds
        if (millis() > check_connect_millis) {
            check_connect_millis = millis() + 10000UL;
            if (!modem.mqtt_connected()) {
                mqtt_connect();
            } else {
                // If connected, send information once every ten seconds
                String payload = "RunTime:" + String(millis() / 1000);
                modem.mqtt_publish(0, publish_topic, payload.c_str());
            }
        }
        // MQTT handling
        modem.mqtt_handle();
        delay(5);
    }
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

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif

/*


-----------------------------

A7670G  # 20250822:OK!

Manufacturer: SIMCOM INCORPORATED
Model: A7670G-LLSE
Revision: A124B01A7670M7
A7670M7_B01V01_240722

-------------------------------


*/
