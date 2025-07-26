/**
 * @file      SimShieldCurrentSensor.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-07-26
 * @note      This example is only available for SimShield
 */

#include "Arduino.h"
#include <Wire.h>
#include "utilities.h"
#include <Adafruit_INA3221.h>


Adafruit_INA3221 ina3221;

void setup()
{
    Serial.begin(115200); // Set console baud rate

    Serial.println("Start Sketch");

    Wire.begin(SIMSHIELD_SDA, SIMSHIELD_SCL);

    // Initialize the INA3221
    if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
        while (1) {
            Serial.println("Failed to find INA3221 chip");
            delay(1000);
        }
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

}


void loop()
{
    // Display voltage and current (in mA) for all three channels
    for (uint8_t i = 0; i < 3; i++) {
        float voltage = ina3221.getBusVoltage(i);
        float current = ina3221.getCurrentAmps(i) * 1000; // Convert to mA

        Serial.print("Channel ");
        Serial.print(i);
        Serial.print(": Voltage = ");
        Serial.print(voltage, 2);
        Serial.print(" V, Current = ");
        Serial.print(current, 2);
        Serial.println(" mA");
    }

    Serial.println();
    // Delay for 250ms before the next reading
    delay(250);
}
