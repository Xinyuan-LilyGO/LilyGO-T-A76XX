/**
 * @file      SetRingMusic.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-08-05
 * @note      Only support A7670X A7608X , Not support SIM7670G
 */

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

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

#include "ring_buffer.h"

#ifdef LILYGO_T_SIM7670G_S3
#error "Only support A7670X A7608X , Not support SIM7670G"
#endif

#ifdef TINY_GSM_MODEM_SIM7600
#error "SIM7600 series has not yet implemented this playback function"
#endif

#ifdef LILYGO_SIM7000G
#error "SIM7000G not  playback function"
#endif

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
#ifdef MODEM_RESET_PIN
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
#endif

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


    // For detailed AT commands, please see datasheet/A76XX/A76XX_Series_AT_Command_Manual_V1.12.pdf
    // The total capacity of A7670 is 4MB and cannot exceed the available space.
    // You need to connect the speaker to the board SPK+ ,SPK- to hear the sound, see images/speaker.jpg
    modem.sendAT("+FSMEM");
    if (modem.waitResponse("+FSMEM: C:(") != 1) {
        Serial.println("Failed to get memory size!"); return;
    }
    String capSize = modem.stream.readStringUntil('\n');
    capSize.replace("\n", "");
    capSize.replace(")", "");
    Serial.printf("Capacity size [<total>/<used>]: "); Serial.println(capSize);

    modem.sendAT("+FSLS");
    if (modem.waitResponse("+FSLS: FILES:") == 1) {
        String list;
        modem.waitResponse(1000UL, list);
        list.replace("OK", "");
        Serial.printf("File list: "); Serial.println(list);
    } else {
        Serial.println("No find any file!");
    }
    Serial.println("Try opening the file");
    modem.sendAT("+FSOPEN=", "C:/ring.mp3");
    if (modem.waitResponse() != 1) {
        Serial.println("Open file failed!"); return ;
    }
    Serial.println("Writing audio files");
    modem.sendAT("+FSWRITE=1,", iPhone_Ring_mp3_len, 10);
    if (modem.waitResponse(3000UL, "CONNECT") != 1) {
        Serial.println("Write file failed!"); return ;
    }
    modem.stream.write(ring_buffer, iPhone_Ring_mp3_len);
    if (modem.waitResponse(10000UL) == 1) {
        Serial.println("Write file success!");
    }
    modem.sendAT("+FSCLOSE=1");
    if (modem.waitResponse() != 1) {
        Serial.println("Close file failed!"); return ;
    }

    Serial.println("Play File...");
    uint8_t repeat = 1; //Repeat times
    modem.sendAT("+CCMXPLAY=\"C:/ring.mp3\",0,", repeat);
    if (modem.waitResponse() != 1) {
        Serial.println("Play mp3 failed!"); return ;
    }
    Serial.println("Play mp3 success!");

    // Set ring file
    modem.sendAT("+CRINGSET=\"C:/ring.mp3\"");
    if (modem.waitResponse() != 1) {
        Serial.println("Set ring file failed!"); return ;
    } else {
        Serial.println("Now you can try to call the modem and it should ring normally.");
    }
}

void loop()
{
    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }
}

#ifndef TINY_GSM_FORK_LIBRARY
#error "No correct definition detected, Please copy all the [lib directories](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/tree/main/lib) to the arduino libraries directory , See README"
#endif


/*
TEST WITH:

Manufacturer: SIMCOM INCORPORATED
Model: A7670G-LLSE
Revision: A124B01A7670M7
A7670M7_B01V01_240722

*/