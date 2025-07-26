/*
  RadioLib SX126x Receive with Interrupts Example

  This example listens for LoRa transmissions and tries to
  receive them. Once a packet is received, an interrupt is
  triggered. To successfully receive data, the following
  settings have to be the same on both transmitter
  and receiver:
  - carrier frequency
  - bandwidth
  - spreading factor
  - coding rate
  - sync word

  Other modules from SX126x family can also be used.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

//  * @note      This example is only available for SimShield

// include the library
#include <RadioLib.h>
#include "utilities.h"

SX1262 radio = new Module(SIMSHIELD_RADIO_CS, SIMSHIELD_RADIO_IRQ, SIMSHIELD_RADIO_RST, SIMSHIELD_RADIO_BUSY);

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

ICACHE_RAM_ATTR void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

void setup()
{
    Serial.begin(115200);

    //Pull spi devices cs to high
    pinMode(SIMSHIELD_SD_CS, OUTPUT);
    pinMode(SIMSHIELD_RADIO_CS, OUTPUT);

    digitalWrite(SIMSHIELD_SD_CS, HIGH);
    digitalWrite(SIMSHIELD_RADIO_CS, HIGH);

    SPI.begin(SIMSHIELD_SCK, SIMSHIELD_MISO, SIMSHIELD_MOSI);

    // initialize SX1262 with default settings
    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin(868.0);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true) {
            delay(10);
        }
    }

    // set the function that will be called
    // when new packet is received
    radio.setPacketReceivedAction(setFlag);

    // start listening for LoRa packets
    Serial.print(F("[SX1262] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true) {
            delay(10);
        }
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.scanChannel();
}

void loop()
{
    // check if the flag is set
    if (receivedFlag) {
        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = radio.readData(str);

        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int numBytes = radio.getPacketLength();
          int state = radio.readData(byteArr, numBytes);
        */

        if (state == RADIOLIB_ERR_NONE) {
            // packet was successfully received
            Serial.println(F("[SX1262] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1262] Data:\t\t"));
            Serial.println(str);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("[SX1262] RSSI:\t\t"));
            Serial.print(radio.getRSSI());
            Serial.println(F(" dBm"));

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("[SX1262] SNR:\t\t"));
            Serial.print(radio.getSNR());
            Serial.println(F(" dB"));

            // print frequency error
            Serial.print(F("[SX1262] Frequency error:\t"));
            Serial.print(radio.getFrequencyError());
            Serial.println(F(" Hz"));

        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));

        } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);

        }
    }
}
