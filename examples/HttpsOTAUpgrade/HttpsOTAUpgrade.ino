/**
 * @file      HttpsOTAUpgrade.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-30
 * @note
 * * Example is suitable for A7670X/A7608X/SIM767G/SIM7000G/SIM7600 series
 * * Use modem to connect to custom server to upgrade the esp32 firmware
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include "utilities.h"
#include <TinyGsmClient.h>
#include <Update.h>

// The following download link is for testing only and can be replaced with  firmware download link for another server.
#if  defined(LILYGO_T_A7670)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7670.bin";
#elif defined(LILYGO_T_CALL_A7670_V1_0)
const char *server_url =  "https://lewishe.pro/ota/firmware-tcall-a7670.bin";
#elif defined(LILYGO_T_CALL_A7670_V1_1)
const char *server_url =  "https://lewishe.pro/ota/firmware-tcall-a7670-v1-1.bin";
#elif defined(LILYGO_T_SIM7670G_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-sim7672g.bin";
#elif defined(LILYGO_T_A7608X)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608.bin";
#elif defined(LILYGO_T_A7608X_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608-s3.bin";
#elif defined(LILYGO_T_A7608X_DC_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608-s3-dc.bin";
#elif defined(LILYGO_SIM7600X)
const char *server_url =  "https://lewishe.pro/ota/firmware-sim7600x.bin";
#elif defined(LILYGO_SIM7000G)
const char *server_url =  "https://lewishe.pro/ota/firmware-sim7000g.bin";
#else
#error "Use ArduinoIDE, please open the macro definition corresponding to the board above <utilities.h>"
#endif



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

    // Initialize HTTPS
    Serial.println("Initialize HTTPS");
    modem.https_begin();

    // Set OTA Server URL
    if (!modem.https_set_url(server_url)) {
        Serial.println("Failed to set the URL. Please check the validity of the URL!");
        return;
    }

    /*
    * If you use SIM7000G, the speed after sending the request depends on the wireless communication rate.
    * By default, it will wait until the modem responds to the data before exiting.
    * You can use https_set_timeout() to set the request response timeout.
    * The above instructions are only for SIM7000G
    * */
    uint32_t start = millis();
    size_t firmware_size = 0;
    int httpCode = 0;
    Serial.println("Get firmware form HTTPS");
    // Send http get request, if it is correct, you can get the firmware size
    httpCode = modem.https_get(&firmware_size);
    uint32_t end = millis();
    Serial.printf("Request url use %lu ms\n", end - start);

    if (httpCode != 200) {
        Serial.print("HTTP get failed ! error code = ");
        Serial.println(httpCode);

        if (httpCode == 706) {
            // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/171
            Serial.println("SIM7670G has 288KB available user storage space. Use AT+FSMEM to query, so files larger than 288KB cannot be updated.");
        }

        return;
    }

    // Get firmware size
    Serial.print("Firmware size : "); Serial.print(firmware_size); Serial.println("Kb");

    // Begin Update firmware
    Serial.println("Start upgrade firmware ...");
    if (!Update.begin(firmware_size)) {
        Serial.println("Not enough space to begin OTA");
        return;
    }

    uint8_t buffer[1024];
    int written = 0;
    int progress = 0;
    int total = 0;

    while (1) {
        // Read firmware form modem buffer
        int len = modem.https_body(buffer, 1024);
        if (len == 0)break;

        written = Update.write(buffer, len);
        if (written != len) {
            Serial.println("Written only : " + String(written) + "/" + String(len) + ". Retry?");
        }
        total += written;
        int newProgress = (total * 100) / firmware_size;
        if (newProgress - progress >= 5 || newProgress == 100) {
            progress = newProgress;
            Serial.print(String("\r ") + progress + "%\n");
        }
    }
    if (!Update.end()) {
        Serial.println("Update not finished? Something went wrong!");
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        return;
    }

    Serial.println();

    if (!Update.isFinished()) {
        Serial.println("Update successfully completed.");
    }

    Serial.println("=== Update successfully completed. Rebooting.");

    delay(1500);

    modem.https_end();

    esp_restart();
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

-----------------------------

SIM7000G    # 2025/07/10:OK!
AT+SIMCOMATI
Revision:1529B11SIM7000G
CSUB:V01
APRev:1529B11SIM7000,V01
QCN:MDM9206_TX3.0.SIM7000G_P1.03C_20240911

*/
