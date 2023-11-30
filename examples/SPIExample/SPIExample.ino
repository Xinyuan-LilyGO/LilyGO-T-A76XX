/**
 * @file      SPIExample.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-30
 * @note      The sketch only demonstrates how to multiplex the SPI bus
 * * The sketch only demonstrates how to configure SPI and I2C Pin. The specific pins can be mapped to your convenient GPIO at will.
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include "src/LoRa.h"
#include "utilities.h"

// !Please confirm the correct Pin, this is just a demonstration
// !Please confirm the correct Pin, this is just a demonstration
// !Please confirm the correct Pin, this is just a demonstration
// The sketch only demonstrates how to configure SPI and I2C Pin. The specific pins can be mapped to your convenient GPIO at will.
// If the core used is ESP32, IO35,39,34,36 can only be used for input and cannot be set as output
#define RADIO_MISO_PIN              2
#define RADIO_MOSI_PIN              15
#define RADIO_SCLK_PIN              14
#define RADIO_CS_PIN                4
#define RADIO_RST_PIN               12
#define RADIO_DIO_PIN               39
#define I2C_SDA                     33
#define I2C_SCL                     32

int counter = 0;
SPIClass RadioSPI(HSPI);

void setup()
{
    Serial.begin(115200);

    // If there is an SD card onboard, then assign the SPI to the SD card
#ifdef BOARD_SCK_PIN
    SPI.begin(BOARD_SCK_PIN, BOARD_MISO_PIN, BOARD_MOSI_PIN);
    if (!SD.begin(BOARD_SD_CS_PIN)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
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
#endif

    // Assign the second SPI handler to Radio
    RadioSPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
 
    LoRa.setSPI(RadioSPI);

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO_PIN);
    
    //Examples accessing other spi devices
    if (!LoRa.begin(915E6)) {
        while (1) {
            Serial.println("Starting LoRa failed!");
            delay(1000);
        }
    }
    // Maximize the use of available IO
    Wire.begin(I2C_SDA, I2C_SCL);
    //....I2C Devices do someing ..

}

void loop()
{
    Serial.print("Sending packet: ");
    Serial.println(counter);

    // send packet
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;

    delay(1000);
}