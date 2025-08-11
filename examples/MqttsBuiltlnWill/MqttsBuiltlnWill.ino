/**
 * @file      MqttsBuiltlnWill.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-10-25
 * @note
 * * Example is suitable for A7670X/A7608X/SIM7670G/SIM7600 series
  * * TODO: SIM7000G To be fixed
 * * MQTT will message example, use a private server for testing, please prepare your own MQTT server,
 * * and only support MQTT3.1.1 version, and set up the will message function
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

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

// MQTT details
const char *broker = "Your mqtt server address";
const uint16_t broker_port = 1883;
const char *broker_username = "broker_username";
const char *broker_password = "broker_password";
const char *client_id = "client_id";

// Will topic , Change to the channel you want to post to
const char *will_topic = "esp32-00000-04/status";
// Will message , Change to the content you need to publish
const char *will_msg = "offline";
// Will message qos
uint8_t qos = 1;

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

    // Set will topic and message
    modem.setWillMessage(will_topic, will_msg, qos);

    bool ret = modem.mqtt_connect(mqtt_client_id, broker, broker_port, client_id, broker_username, broker_password);
    if (!ret) {
        Serial.println("Failed!"); return false;
    }
    Serial.println("successes.");

    if (modem.mqtt_connected()) {
        Serial.println("MQTT has connected!");
    } else {
        return false;
    }
    // Set MQTT processing callback
    modem.mqtt_set_callback(mqtt_callback);

    return true;
}


void setup()
{
    Serial.begin(115200); // Set console baud rate

    while (!Serial);

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

    //SIM7672G Can't set network mode
#ifndef TINY_GSM_MODEM_SIM7670G
    if (!modem.setNetworkMode(MODEM_NETWORK_AUTO)) {
        Serial.println("Set network mode failed!");
    }
    String mode = modem.getNetworkModes();
    Serial.print("Current network mode : ");
    Serial.println(mode);
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

    // Initialize MQTT, use SSL, skip authentication server
    modem.mqtt_begin(true);

    if (!mqtt_connect()) {
        Serial.println("Please make sure you are using the latest released version of the firmware. Find the latest version here: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/docs/update_fw.md");
        Serial.println("If you still have problems with the latest firmware, please open an issue. Otherwise, please do not create meaningless issues.");
        return ;
    }

    Serial.println("Please manually disconnect the USB-C to simulate an unexpected power outage."
                   "The MQTT server will send a will message to the relevant subscribers."
                   "If you have not subscribed to the will message, you will not see any effect.");
    while (1) {
        // Check the connection every ten seconds
        if (millis() > check_connect_millis) {
            check_connect_millis = millis() + 10000UL;
            if (!modem.mqtt_connected()) {
                mqtt_connect();
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
*/