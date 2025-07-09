#include "Adafruit_INA3221.h"
#include <Wire.h>

// Create an INA3221 object
Adafruit_INA3221 ina3221;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for serial port to connect on some boards

  Serial.println("Adafruit INA3221 simple test");

  // Initialize the INA3221
  if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
    Serial.println("Failed to find INA3221 chip");
    while (1)
      delay(10);
  }
  Serial.println("INA3221 Found!");

  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);

  // Set shunt resistances for all channels to 0.05 ohms
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 0.05);
  }

  // Set a power valid alert to tell us if ALL channels are between the two
  // limits:
  ina3221.setPowerValidLimits(3.0 /* lower limit */, 15.0 /* upper limit */);
}

void loop() {
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
