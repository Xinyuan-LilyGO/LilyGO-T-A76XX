#include <Wire.h>
#include "Adafruit_INA3221.h"

// Create an INA3221 object
Adafruit_INA3221 ina3221;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);  // Wait for serial port to connect on some boards

  Serial.println("Adafruit INA3221 simple test");
  
  // Initialize the INA3221
  if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
    Serial.println("Failed to find INA3221 chip");
    while (1) delay(10);
  }
  Serial.println("INA3221 Found!");

  // Set shunt resistances for all channels to 0.05 ohms
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 0.05);
  }
  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);
  ina3221.setCriticalAlertThreshold(0, 0.325);
  Serial.print("Critical threshold for channel 1: "); 
  Serial.print(ina3221.getCriticalAlertThreshold(0));
  Serial.println(" A");

  ina3221.setCriticalAlertThreshold(1, 1.0);
  Serial.print("Critical threshold for channel 2: "); 
  Serial.print(ina3221.getCriticalAlertThreshold(1));
  Serial.println(" A");


  ina3221.setCriticalAlertThreshold(2, 1.5);
  Serial.print("Critical threshold for channel 3: "); 
  Serial.print(ina3221.getCriticalAlertThreshold(2));
  Serial.println(" A");
}

void loop() {
  // Display voltage and current (in mA) for all three channels
  for (uint8_t i = 0; i < 3; i++) {
    float voltage = ina3221.getBusVoltage(i);
    float current = ina3221.getCurrentAmps(i) * 1000;  // Convert to mA

    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": Voltage = ");
    Serial.print(voltage, 2);
    Serial.print(" V, Current = ");
    Serial.print(current, 2);
    Serial.println(" mA");
  }

  uint16_t flags = ina3221.getFlags();
  if (flags & INA3221_CRITICAL_CH1) {
    Serial.println("   !!! Channel 1 Critical Current !!!");
  }
  if (flags & INA3221_CRITICAL_CH2) {
    Serial.println("   !!! Channel 2 Critical Current !!!");
  }
  if (flags & INA3221_CRITICAL_CH3) {
    Serial.println("   !!! Channel 3 Critical Current !!!");
  }
  

  Serial.println();
  // Delay for 250ms before the next reading
  delay(250);
}
