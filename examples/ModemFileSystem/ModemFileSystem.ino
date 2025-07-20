/**
 * @file      ModemFileSystem.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-07-20
 * @note      Example is suitable for A7670X/A7608X/SIM767G/SIM7600 series,SIM7000 series not support
 */

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#include "utilities.h"
#include <TinyGsmClient.h>
#include "Arduino.h"

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#include "music.h"

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

    delay(5000);

    size_t totalBytes,  usedBytes, fileSize = 0, readSize = 0;
    const char *filename = "/music.mp3";

    modem.fs_mem(totalBytes, usedBytes);

    Serial.printf("FS Total:%lu Kb Used:%luKb\n", totalBytes / 1024, usedBytes / 1024);

    int res = modem.fs_write(filename, music, music_mp3_len);

    if (res < 0) {
        Serial.println("Error writing data. Please check whether the data size exceeds the available size of the file system.");
        return;
    }

    res = modem.fs_attri(filename, fileSize);

    Serial.printf("Get the file size as %lu bytes", fileSize);

    uint8_t *buffer = (uint8_t *)ps_malloc(fileSize);
    if (!buffer) {
        Serial.println("Failed to apply for comparison buffer");
        return;
    }

    readSize = modem.fs_read(filename, buffer, fileSize);
    Serial.printf("Read file as %lu bytes\n", readSize);

    if (memcmp(buffer, music, music_mp3_len) == 0) {
        Serial.println("Buffer comparison complete, data matches!");
    } else {
        Serial.println("Buffer comparison completed, data does not match!");
    }

    res = modem.fs_del(filename);
    if (res < 0) {
        Serial.println("Failed to delete the file. The file may not exist.");
    }

    // Release buffer
    free(buffer);

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
