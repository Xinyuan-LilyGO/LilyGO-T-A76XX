// REQUIRES the following Arduino libraries:
// - Adafruit INA219 Lib: https://github.com/adafruit/Adafruit_INA219
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// * @note      Requires external expansion board support https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v
// * @note      Requires external expansion board support https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v
// * @note      Requires external expansion board support https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v
// * @note      Requires external expansion board support https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v
// * @note      Requires external expansion board support https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v

#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_INA219.h>
#include <Wire.h>

const uint8_t ina219_salve_address1 = 0x40;
const uint8_t ina219_salve_address2 = 0x41;

Adafruit_INA219 ina219_1(ina219_salve_address1);
Adafruit_INA219 ina219_2(ina219_salve_address2);

// T-A7670X SIM-Hat Pin
#define SENSOR_SDA     21
#define SENSOR_SCL     22

///T-A7608-ESP32S3 SIM-Hat Pin
// #define SENSOR_SDA     2
// #define SENSOR_SCL     1


void setup(void)
{
    Serial.begin(115200);

    while (!Serial) {
        // will pause Zero, Leonardo, etc until serial console opens
        delay(1);
    }

    Wire.begin(SENSOR_SDA, SENSOR_SCL);

    Serial.println("Hello!");

    // Initialize the INA219.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    if (! ina219_1.begin()) {
        while (1) {
            Serial.println("Failed to find INA219 1 chip");
            delay(1000);
        }
    }

    if (! ina219_2.begin()) {
        while (1) {
            Serial.println("Failed to find INA219 2 chip");
            delay(1000);
        }
    }
    // To use a slightly lower 32V, 1A range (higher precision on amps):
    //ina219.setCalibration_32V_1A();
    // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    //ina219.setCalibration_16V_400mA();

    Serial.println("Measuring voltage and current with INA219 ...");
}

void loop(void)
{
    float shuntvoltage_1 = 0;
    float busvoltage_1 = 0;
    float current_mA_1 = 0;
    float loadvoltage_1 = 0;
    float power_mW_1 = 0;

    shuntvoltage_1 = ina219_1.getShuntVoltage_mV();
    busvoltage_1 = ina219_1.getBusVoltage_V();
    current_mA_1 = ina219_1.getCurrent_mA();
    power_mW_1 = ina219_1.getPower_mW();
    loadvoltage_1 = busvoltage_1 + (shuntvoltage_1 / 1000);

    Serial.print("Bus Voltage 1:   "); Serial.print(busvoltage_1); Serial.println(" V");
    Serial.print("Shunt Voltage 1: "); Serial.print(shuntvoltage_1); Serial.println(" mV");
    Serial.print("Load Voltage 1:  "); Serial.print(loadvoltage_1); Serial.println(" V");
    Serial.print("Current 1:       "); Serial.print(current_mA_1); Serial.println(" mA");
    Serial.print("Power 1:         "); Serial.print(power_mW_1); Serial.println(" mW");
    Serial.println("");


    float shuntvoltage_2 = 0;
    float busvoltage_2 = 0;
    float current_mA_2 = 0;
    float loadvoltage_2 = 0;
    float power_mW_2 = 0;

    shuntvoltage_2 = ina219_2.getShuntVoltage_mV();
    busvoltage_2 = ina219_2.getBusVoltage_V();
    current_mA_2 = ina219_2.getCurrent_mA();
    power_mW_2 = ina219_2.getPower_mW();
    loadvoltage_2 = busvoltage_2 + (shuntvoltage_2 / 1000);

    Serial.print("Bus Voltage 2:   "); Serial.print(busvoltage_2); Serial.println(" V");
    Serial.print("Shunt Voltage 2: "); Serial.print(shuntvoltage_2); Serial.println(" mV");
    Serial.print("Load Voltage 2:  "); Serial.print(loadvoltage_2); Serial.println(" V");
    Serial.print("Current 2:       "); Serial.print(current_mA_2); Serial.println(" mA");
    Serial.print("Power 2:         "); Serial.print(power_mW_2); Serial.println(" mW");
    Serial.println("");


    delay(2000);
}
