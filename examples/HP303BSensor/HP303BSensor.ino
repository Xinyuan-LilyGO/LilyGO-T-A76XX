/**
 * @file      HP303BSensor.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-11-30
 * !!         The sensor does not exist on any board. 
 * !!         You need to connect the external sensor to the specified pin through wiring before it can be used.
 * !!         If the external module does not have a pull-up resistor, 
 * !!         maybe the sketch will not work correctly, please confirm whether the module used has a pull-up resistor
 */
#include <Arduino.h>
#include <Wire.h>
#include "src/LOLIN_HP303B.h"       //https://github.com/wemos/LOLIN_HP303B_Library


LOLIN_HP303B HP303BPressureSensor;

// Default HP303B Address 0x77 , may be is 0x76
const uint8_t slaveAddress = 0x77;

// !Configure I2C Pins , Change the Pin here to the Pin you need to use, esp can automatically route I2C to any Pin
// !Please note that if the core used is ESP32, please remember that IO33 and above can only be configured as input and cannot be used as output.
// !Please confirm the correct Pin, this is just a demonstration
const uint8_t i2c_sda = 21;
const uint8_t i2c_scl = 22;


void setup(void)
{
    Serial.begin(115200);

    Wire.begin(i2c_sda,i2c_scl);

    HP303BPressureSensor.begin(slaveAddress);

}

void loop()
{
    int32_t temperature;
    int32_t pressure;
    int16_t oversampling = 7;
    int16_t ret, ret1;

    Serial.println();
    //lets the HP303B perform a Single temperature measurement with the last (or standard) configuration
    //The result will be written to the paramerter temperature
    //ret = HP303BPressureSensor.measureTempOnce(temperature);
    //the commented line below does exactly the same as the one above, but you can also config the precision
    //oversampling can be a value from 0 to 7
    //the HP303B will perform 2^oversampling internal temperature measurements and combine them to one result with higher precision
    //measurements with higher precision take more time, consult datasheet for more information
    ret = HP303BPressureSensor.measureTempOnce(temperature, oversampling);

    if (ret != 0) {
        //Something went wrong.
        //Look at the library code for more information about return codes
        Serial.print("FAIL! ret = ");
        Serial.println(ret);
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" degrees of Celsius");
    }

    //Pressure measurement behaves like temperature measurement
    //ret = HP303BPressureSensor.measurePressureOnce(pressure);
    ret1 = HP303BPressureSensor.measurePressureOnce(pressure, oversampling);
    if (ret1 != 0) {
        //Something went wrong.
        //Look at the library code for more information about return codes
        Serial.print("FAIL! ret = ");
        Serial.println(ret1);
    } else {
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" Pascal");
    }

    delay(1000);

}

