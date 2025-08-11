/**
 * @file      MqttsBuiltlnAWS.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-28
 * @note
 * * Example is suitable for A7670X/A7608X/SIM7670G/SIM7000G/SIM7080G/SIM7600 series
 * * Connect MQTT Broker as https://aws.amazon.com/campaigns/IoT
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 * *!!!! When using ESP to connect to AWS, the AWS IOT HUB policy must be set to all devices, otherwise the connection cannot be made.
 * *!!!! When using ESP to connect to AWS, the AWS IOT HUB policy must be set to all devices, otherwise the connection cannot be made.
 * *!!!! When using ESP to connect to AWS, the AWS IOT HUB policy must be set to all devices, otherwise the connection cannot be made.
 * *!!!! When using ESP to connect to AWS, the AWS IOT HUB policy must be set to all devices, otherwise the connection cannot be made.
 * *Youtube : https://youtu.be/am-rTDzm4lQ
 * */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include "utilities.h"
#include <TinyGsmClient.h>
#include "certs/AWSClientCertificate.h"
#include "certs/AWSClientPrivateKey.h"
#include "certs/AmazonRootCA.h"

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
// The URL of xxxxxxx-ats.iot.ap-southeast-2.amazonaws.com can be found in the settings for endpoint in your AWS IOT Core account
const char *broker = "xxxxxxx.iot.ap-southeast-2.amazonaws.com";
const uint16_t broker_port = 8883;
const char *client_id = "A76XX";

// Replace the topic you want to subscribe to
const char *subscribe_topic = "GsmMqttTest/subscribe";
// Replace the theme you want to publish
const char *publish_topic = "GsmMqttTest/publish";

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

    bool ret = modem.mqtt_connect(mqtt_client_id, broker, broker_port, client_id);
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

    delay(5000);

    String ipAddress = modem.getLocalIP();
    Serial.print("Network IP:"); Serial.println(ipAddress);

    // Print modem software version
    String res;
    modem.sendAT("+SIMCOMATI");
    modem.waitResponse(10000UL, res);
    Serial.println(res);

    delay(2000);

    // Initialize MQTT, use SSL
    modem.mqtt_begin(true);

    // Set Amazon Certificate
    modem.mqtt_set_certificate(AmazonRootCA, AWSClientCertificate, AWSClientPrivateKey);

    // Connecting to AWS IOT Core
    if (!mqtt_connect()) {
        Serial.println("Please make sure you are using the latest released version of the firmware. Find the latest version here: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/docs/update_fw.md");
        Serial.println("If you still have problems with the latest firmware, please open an issue. Otherwise, please do not create meaningless issues.");
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


/*
TEST WITH

20250507:
ISSUE: https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/245#issuecomment-2854747063
Manufacturer: INCORPORATED
Model: A7670E-FASE
Revision: A011B07A7670M7_F
A7670M7_B07V01_240927
QCN:
IMEI: XXXXXXXXXXXXXXXXX
MEID:
+GCAP: +CGSM,+FCLASS,+DS
DeviceInfo:

20240821:
A7670E-FASE: PN:S2-10AAW-Z319S
Manufacturer: INCORPORATED
Model: A7670E-FASE
Revision: A011B05A7670M7_F
A7670M7_B05V04_240122
QCN:
IMEI: XXXXXXXXXXXXXXXXX
MEID:
+GCAP: +CGSM,+FCLASS,+DS
DeviceInfo:

-------------------------------

20240821:
SIM7670G: PN:S2-10D1Y-Z327P
Manufacturer: SIMCOM INCORPORATED
Model: SIM7670G-MNGV
Revision: 2374B02SIM767XM5A_M
SIM767XM5_B02V01_240717

!!!!!!!!!!!!!!!!FAILED 2374B03SIM767XM5A_M
https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/275
Manufacturer: SIMCOM INCORPORATED
Model: SIM7670G-MNGV
Revision: 2374B03SIM767XM5A_M
SIM767XM5_B03V02_240914
!!!!!!!!!!!!!!!!FAILED 2374B03SIM767XM5A_M

-------------------------------

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


-----------------------------

SIM7000G    # 2025/07/10:OK!
AT+SIMCOMATI
Revision:1529B11SIM7000G
CSUB:V01
APRev:1529B11SIM7000,V01
QCN:MDM9206_TX3.0.SIM7000G_P1.03C_20240911

!!!!!!!!!!!!!!!!FAILED 1529B11SIM7000 QCN:MDM9206_TX3.0.SIM7000G_P1.02_20180726
Revision:1529B11SIM7000G
CSUB:V01
APRev:1529B11SIM7000,V01
QCN:MDM9206_TX3.0.SIM7000G_P1.02_20180726
!!!!!!!!!!!!!!!!FAILED 1529B11SIM7000 QCN:MDM9206_TX3.0.SIM7000G_P1.02_20180726

-----------------------------

SIM7080G  # 20250807:OK!

Revision:1951B16SIM7080
CSUB:B16V01
APRev:1951B16SIM7080,B16V01
QCN:SIM7080G_P1.03_20210823

-------------------------------

*/
#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif
