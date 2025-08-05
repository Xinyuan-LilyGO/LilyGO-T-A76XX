/**
 * @file      SDCard_Speed.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-08-05
 *
 */

#include <SD.h>
#include <FS.h>
#include "utilities.h"
#if defined(LILYGO_T_CALL_A7670_V1_0) || defined(LILYGO_T_A7608X_DC_S3) || defined(LILYGO_T_PCIE_A767X)
#error "This board not SD slot"
#endif

// #define USING_SIMSHIELD //Define this line to use the SimShield pinout

#ifdef USING_SIMSHIELD

#define EXAMPLE_SPI_MISO    SIMSHIELD_MISO
#define EXAMPLE_SPI_MOSI    SIMSHIELD_MOSI
#define EXAMPLE_SPI_SCK     SIMSHIELD_SCK
#define EXAMPLE_SD_CS       SIMSHIELD_SD_CS

#else   /*USING_SIMSHIELD*/

#define EXAMPLE_SPI_MISO    BOARD_MISO_PIN
#define EXAMPLE_SPI_MOSI    BOARD_MOSI_PIN
#define EXAMPLE_SPI_SCK     BOARD_SCK_PIN
#define EXAMPLE_SD_CS       BOARD_SD_CS_PIN

#endif  /*USING_SIMSHIELD*/

const size_t TEST_FILE_SIZE = 1024 * 1024; // 1MB
const size_t BLOCK_SIZE = 512;

bool testWriteSpeed(float &writeTime, float &writeSpeed)
{
    Serial.println("Starting write speed test...");
    File testFile = SD.open("/testfile.bin", FILE_WRITE);
    if (!testFile) {
        Serial.println("Error opening file for writing!");
        return false;
    }

    uint32_t startTime = millis();
    for (size_t i = 0; i < TEST_FILE_SIZE; i += BLOCK_SIZE) {
        uint8_t buffer[BLOCK_SIZE];
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
            buffer[j] = (uint8_t)(i + j);
        }
        testFile.write(buffer, BLOCK_SIZE);
    }
    uint32_t endTime = millis();
    testFile.close();

    writeTime = (endTime - startTime) / 1000.0;
    writeSpeed = TEST_FILE_SIZE / (writeTime * 1024.0);
    Serial.printf("Write time: %.2f s\n", writeTime);
    Serial.printf("Write speed: %.2f KB/s\n", writeSpeed);
    return true;
}

bool testReadSpeed(float &readTime, float &readSpeed)
{
    Serial.println("Starting read speed test...");
    File testFile = SD.open("/testfile.bin", FILE_READ);
    if (!testFile) {
        Serial.println("Error opening file for reading!");
        return false;
    }

    uint32_t startTime = millis();
    uint8_t buffer[BLOCK_SIZE];
    for (size_t i = 0; i < TEST_FILE_SIZE; i += BLOCK_SIZE) {
        testFile.read(buffer, BLOCK_SIZE);
    }
    uint32_t endTime = millis();
    testFile.close();

    readTime = (endTime - startTime) / 1000.0;
    readSpeed = TEST_FILE_SIZE / (readTime * 1024.0);
    Serial.printf("Read time: %.2f s\n", readTime);
    Serial.printf("Read speed: %.2f KB/s\n", readSpeed);
    return true;
}

void setup()
{
    Serial.begin(115200);

#ifdef BOARD_POWERON_PIN
    // T-A7670X V1.x SD card power supply and Modem are controlled by IO12. When using battery, IO12 must be set to HIGH
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    SPI.begin(EXAMPLE_SPI_SCK, EXAMPLE_SPI_MISO, EXAMPLE_SPI_MOSI);
    if (!SD.begin(EXAMPLE_SD_CS)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    float writeTime, writeSpeed;
    bool rlst = testWriteSpeed(writeTime, writeSpeed);
    if (rlst) {
        Serial.printf("Write block use %.2f s\nWrite Speed is %.2f KB/s\n", writeTime, writeSpeed);
    } else {
        Serial.println("Write speed test Failed");
    }
    float readTime, readSpeed;
    rlst = testReadSpeed(readTime, readSpeed);
    if (rlst) {
        Serial.printf("Read block use %.2f s\nRead Speed is %.2f KB/s\n", readTime, readSpeed);
    } else {
        Serial.println("Read speed test Failed");
    }

    SD.end();
}

void loop()
{
}
