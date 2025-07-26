/**
 * @file      SimShieldFactory.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-06-18
 * @note      This example is only available for SimShield
 */

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "utilities.h"
#include <Adafruit_INA3221.h>
#include <RadioLib.h>
#include <SSD1306Wire.h>


// If defined, it is in sending mode, and if undefined, it is in receiving mode
// #define INITIATING_NODE_TX


SSD1306Wire display(0x3c, SIMSHIELD_SDA, SIMSHIELD_SCL);
Adafruit_INA3221 ina3221;

// SX1262 has the following connections:
SX1262 radio = new Module(SIMSHIELD_RADIO_CS, SIMSHIELD_RADIO_IRQ, SIMSHIELD_RADIO_RST, SIMSHIELD_RADIO_BUSY);

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;
// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;
// flag to indicate transmission or reception state
bool transmitFlag = false;

bool online_radio = false;
bool online_in3221 = false;
bool online_sdcard = false;
bool online_disp = false;

uint32_t AutoBaud()
{
    static uint32_t rates[] = {115200, 9600, 57600,  38400, 19200,  74400, 74880,
                               230400, 460800, 2400,  4800,  14400, 28800
                              };
    for (uint8_t i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
        uint32_t rate = rates[i];
        Serial.printf("Trying baud rate %u\n", rate);
        SerialAT.updateBaudRate(rate);
        delay(10);
        for (int j = 0; j < 10; j++) {
            SerialAT.print("AT\r\n");
            String input = SerialAT.readString();
            if (input.indexOf("OK") >= 0) {
                Serial.printf("Modem responded at rate:%u\n", rate);
                return rate;
            }
        }
    }
    SerialAT.updateBaudRate(115200);
    return 0;
}


ICACHE_RAM_ATTR void setFlag(void)
{
    // we sent or received a packet, set the flag
    operationDone = true;
}

bool setupRadio()
{
    // initialize SX1262 with default settings
    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin(868.0);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        return false;
    }

    // set the function that will be called
    // when new packet is received
    radio.setDio1Action(setFlag);

#if defined(INITIATING_NODE_TX)
    // send the first packet on this node
    Serial.print(F("[SX1262] Sending first packet ... "));
    transmissionState = radio.startTransmit("Hello World!");
    transmitFlag = true;
#else
    // start listening for LoRa packets on this node
    Serial.print(F("[SX1262] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        return false;
    }
#endif
    return true;
}


bool setupIna3221()
{
    // Initialize the INA3221
    if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
        Serial.println("Failed to find INA3221 chip");
        return false;
    }
    Serial.println("INA3221 Found!");

    ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);

    // Set shunt resistances for all channels to 0.1 ohms
    for (uint8_t i = 0; i < 3; i++) {
        ina3221.setShuntResistance(i, 0.1);
    }

    // Set a power valid alert to tell us if ALL channels are between the two
    // limits:
    ina3221.setPowerValidLimits(3.0 /* lower limit */, 15.0 /* upper limit */);

    return true;
}

bool setupDisplay()
{
    if (!display.init()) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }

    display.clear();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10); // Draw 2X-scale text
    display.drawString(0, 0, "SIMSHIELD");
    display.display();      // Show initial text
    delay(2000);

    return true;
}


void setup()
{
    Serial.begin(115200); // Set console baud rate

    Serial.println("Start Sketch");

    Wire.begin(SIMSHIELD_SDA, SIMSHIELD_SCL);

    SPI.begin(SIMSHIELD_SCK, SIMSHIELD_MISO, SIMSHIELD_MOSI);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    SerialRS485.begin(115200, SERIAL_8N1, SIMSHIELD_RS_RX, SIMSHIELD_RS_TX);

    Serial.println("init sdcard");
    Serial.println("init sdcard");
    Serial.println("init sdcard");
    Serial.println("init sdcard");
    Serial.println("init sdcard");

    //Pull spi devices cs to high
    pinMode(SIMSHIELD_SD_CS, OUTPUT);
    pinMode(SIMSHIELD_RADIO_CS, OUTPUT);

    digitalWrite(SIMSHIELD_SD_CS, HIGH);
    digitalWrite(SIMSHIELD_RADIO_CS, HIGH);

    if (SD.begin(SIMSHIELD_SD_CS)) {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE) {
            Serial.println("No SD card attached");
        } else {
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
            online_sdcard =  true;
        }
    }



    online_disp =  setupDisplay();

    online_in3221 =  setupIna3221();

    online_radio =  setupRadio();


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

}

uint32_t interval = 0;
uint32_t rs_interval = 0;
float voltage[3];
float current[3];
String rs_data;
String rd_data;
String rs_confirm_data;

void loop()
{
    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }


    if (online_in3221) {
        if (millis() > interval) {
            // Display voltage and current (in mA) for all three channels
            for (uint8_t i = 0; i < 3; i++) {
                voltage[i] = ina3221.getBusVoltage(i);
                current[i] = ina3221.getCurrentAmps(i) * 1000; // Convert to mA
                Serial.print("Channel ");
                Serial.print(i);
                Serial.print(": Voltage = ");
                Serial.print(voltage[i], 2);
                Serial.print(" V, Current = ");
                Serial.print(current[i], 2);
                Serial.println(" mA");
            }
            Serial.println();
            interval = millis() + 3000;
        }
    }

    if (millis() > rs_interval) {

#if defined(INITIATING_NODE_TX)
        rs_data = "Hello #" + String(millis() / 1000);
        SerialRS485.println(rs_data);

        uint32_t timeout = millis() + 3000;
        rs_confirm_data = "N.A";
        while (millis() < timeout) {
            if (SerialRS485.available()) {
                rs_confirm_data = SerialRS485.readStringUntil('\n');
                if (rs_confirm_data.indexOf("OK") != -1) {
                    rs_confirm_data = "Device confirm";
                }
            }
        }

        // check if the previous operation finished
        if (operationDone) {
            // reset flag
            operationDone = false;

            if (transmissionState == RADIOLIB_ERR_NONE) {
                // packet was successfully sent
                Serial.println(F("transmission finished!"));
            } else {
                Serial.print(F("failed, code "));
                Serial.println(transmissionState);
            }

            // send another one
            Serial.print(F("[SX1262] Sending another packet ... "));
            transmissionState = radio.startTransmit(rs_data);

            rd_data = rs_data;
        }


#else
        if (SerialRS485.available()) {
            rs_confirm_data = SerialRS485.readStringUntil('\n');
        }
        SerialRS485.println("OK");


        if (rs_data.length()) {
            Serial.print("RS485 Recv:"); Serial.println(rs_data);
        }

        // check if the previous operation finished
        if (operationDone) {
            // reset flag
            operationDone = false;
            // the previous operation was reception
            // print data and send another packet
            int state = radio.readData(rd_data);
            if (state == RADIOLIB_ERR_NONE) {
                // packet was successfully received
                Serial.println(F("[SX1262] Received packet!"));
                // print data of the packet
                Serial.print(F("[SX1262] Data:\t\t"));
                Serial.println(rd_data);
                // print RSSI (Received Signal Strength Indicator)
                Serial.print(F("[SX1262] RSSI:\t\t"));
                Serial.print(radio.getRSSI());
                Serial.println(F(" dBm"));
                // print SNR (Signal-to-Noise Ratio)
                Serial.print(F("[SX1262] SNR:\t\t"));
                Serial.print(radio.getSNR());
                Serial.println(F(" dB"));

                rd_data.concat(" ");
                rd_data.concat(radio.getRSSI());
                rd_data.concat("dBm");
            }
        }
#endif


        if (online_disp) {
            display.clear();
            display.drawString(0, 0, ("RS485:" + rs_confirm_data));

            if (online_radio) {
                display.drawString(0, 10, ("Radio:" + rd_data));
            } else {
                display.drawString(0, 10, ("Radio offline"));
            }

            if (online_sdcard) {
                display.drawString(0, 20, "SD Card :" + String(SD.cardSize() / 1024 / 1024 / 1024) + "GB");
            } else {
                display.drawString(0, 20, "SD Card offine");
            }

            if (online_in3221) {
                display.drawString(0, 30, "CH1:" + String(voltage[0], 2) + "V / " + String(current[0], 2) + "mA");
                display.drawString(0, 40, "CH2:" + String(voltage[1], 2) + "V / " + String(current[1], 2) + "mA");
                display.drawString(0, 50, "CH3:" + String(voltage[2], 2) + "V / " + String(current[2], 2) + "mA");
            }

            display.display();
        }

        rs_data.clear();

        rs_interval = millis() + 1000;
    }


    delay(1);
}
