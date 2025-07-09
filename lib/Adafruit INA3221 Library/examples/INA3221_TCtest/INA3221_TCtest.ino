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

  // tie CH1 + CH2 to gnd - pin is high (TC not started)
  // tie CH1 to 3V+ and keep CH2 low voltage - pin will drop low after a few ms
  // to indicate TC failure
}

void loop() {
  ina3221.reset(); // TC re-inits on reset so this lets us test repeatedly

  delay(1000);
}
