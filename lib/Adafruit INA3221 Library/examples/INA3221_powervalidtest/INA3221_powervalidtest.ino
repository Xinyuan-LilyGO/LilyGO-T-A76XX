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

  // connect V1, V2, and V3 to 3.3V, 5V and 12V

  // PV output goes high when all are above the high limit!

  // ina3221.setPowerValidLimits(15, 20); // this will make the pin go low (all
  // are below the lower limit)

  // ina3221.setPowerValidLimits(2.7, 6); // this will make the pin go low (all
  // are above lower limit, at least one is below upper limit)

  ina3221.setPowerValidLimits(2.7, 3.1); // this will make the pin go high (all
                                         // are above the upper limit of 3.1V)
}

void loop() {}
