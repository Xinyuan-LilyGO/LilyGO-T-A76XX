/**
 * @file      HttpsOTAUpgrade.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-30
 * @note
 * * Example is suitable for A7670X/A7608X/SIM7672 series
 * * Use modem to connect to custom server to upgrade the esp32 firmware
 * * Example uses a forked TinyGSM <https://github.com/lewisxhe/TinyGSM>, which will not compile successfully using the mainline TinyGSM.
 */
#define TINY_GSM_RX_BUFFER          1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

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
#elif defined(LILYGO_T_SIM767XG_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-sim7672g.bin";
#elif defined(LILYGO_T_A7608X)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608.bin";
#elif defined(LILYGO_T_A7608X_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608-s3.bin";
#elif defined(LILYGO_T_A7608X_DC_S3)
const char *server_url =  "https://lewishe.pro/ota/firmware-a7608-s3-dc.bin";
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
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    // Set modem reset pin ,reset modem
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(100);
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

    if (!modem.enableNetwork()) {
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

    // Send GET request
    int httpCode = 0;
    Serial.println("Get firmware form HTTPS");
    httpCode = modem.https_get();
    if (httpCode != 200) {
        Serial.print("HTTP get failed ! error code = ");
        Serial.println(httpCode);
        return;
    }

    // Get firmware size
    size_t firmware_size = modem.https_get_size();
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
