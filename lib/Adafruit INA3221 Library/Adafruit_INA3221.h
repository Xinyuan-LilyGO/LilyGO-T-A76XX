#ifndef ADAFRUIT_INA3221_H
#define ADAFRUIT_INA3221_H

#include "Adafruit_BusIO_Register.h"
#include <Arduino.h>
#include <Wire.h>

#define INA3221_DEFAULT_ADDRESS 0x40

#define INA3221_MANUFACTURER_ID                                                \
  0x5449                      ///< Manufacturer ID for Texas Instruments
#define INA3221_DIE_ID 0x3220 ///< Die ID for INA3221

// Register Definitions
#define INA3221_REG_CONFIGURATION 0x00    ///< Configuration
#define INA3221_REG_SHUNTVOLTAGE_CH1 0x01 ///< Shunt Voltage Channel 1
#define INA3221_REG_BUSVOLTAGE_CH1 0x02   ///< Bus Voltage Channel 1
#define INA3221_REG_SHUNTVOLTAGE_CH2 0x03 ///< Shunt Voltage Channel 2
#define INA3221_REG_BUSVOLTAGE_CH2 0x04   ///< Bus Voltage Channel 2
#define INA3221_REG_SHUNTVOLTAGE_CH3 0x05 ///< Shunt Voltage Channel 3
#define INA3221_REG_BUSVOLTAGE_CH3 0x06   ///< Bus Voltage Channel 3
#define INA3221_REG_CRITICAL_ALERT_LIMIT_CH1                                   \
  0x07 ///< Critical Alert Limit Channel 1
#define INA3221_REG_WARNING_ALERT_LIMIT_CH1                                    \
  0x08 ///< Warning Alert Limit Channel 1
#define INA3221_REG_CRITICAL_ALERT_LIMIT_CH2                                   \
  0x09 ///< Critical Alert Limit Channel 2
#define INA3221_REG_WARNING_ALERT_LIMIT_CH2                                    \
  0x0A ///< Warning Alert Limit Channel 2
#define INA3221_REG_CRITICAL_ALERT_LIMIT_CH3                                   \
  0x0B ///< Critical Alert Limit Channel 3
#define INA3221_REG_WARNING_ALERT_LIMIT_CH3                                    \
  0x0C                                    ///< Warning Alert Limit Channel 3
#define INA3221_REG_SHUNTVOLTAGE_SUM 0x0D ///< Shunt Voltage Sum
#define INA3221_REG_SHUNTVOLTAGE_SUM_LIMIT 0x0E ///< Shunt Voltage Sum Limit
#define INA3221_REG_MASK_ENABLE 0x0F            ///< Mask/Enable
#define INA3221_REG_POWERVALID_UPPERLIMIT 0x10  ///< Power-Valid Upper Limit
#define INA3221_REG_POWERVALID_LOWERLIMIT 0x11  ///< Power-Valid Lower Limit
#define INA3221_REG_MANUFACTURER_ID 0xFE        ///< Manufacturer ID
#define INA3221_REG_DIE_ID 0xFF                 ///< Die ID

// Mask/Enable Register bit flags for INA3221
#define INA3221_CONV_READY (1UL << 0)     ///< Conversion Ready
#define INA3221_TIMECONT_ALERT (1UL << 1) ///< Timing Control Alert
#define INA3221_POWER_VALID (1UL << 2)    ///< Power Valid Alert
#define INA3221_WARN_CH3 (1UL << 3)       ///< Warning Alert for Channel 3
#define INA3221_WARN_CH2 (1UL << 4)       ///< Warning Alert for Channel 2
#define INA3221_WARN_CH1 (1UL << 5)       ///< Warning Alert for Channel 1
#define INA3221_SUMMATION (1UL << 6)      ///< Summation Alert
#define INA3221_CRITICAL_CH3 (1UL << 7)   ///< Critical Alert for Channel 3
#define INA3221_CRITICAL_CH2 (1UL << 8)   ///< Critical Alert for Channel 2
#define INA3221_CRITICAL_CH1 (1UL << 9)   ///< Critical Alert for Channel 1

/**
 * @brief Enumeration for the 'AVG2-0' values in the Configuration register.
 *
 * Represents the number of samples that will be averaged for each measurement.
 */
typedef enum {
  INA3221_AVG_1_SAMPLE = 0b000,    ///< Average over 1 sample
  INA3221_AVG_4_SAMPLES = 0b001,   ///< Average over 4 samples
  INA3221_AVG_16_SAMPLES = 0b010,  ///< Average over 16 samples
  INA3221_AVG_64_SAMPLES = 0b011,  ///< Average over 64 samples
  INA3221_AVG_128_SAMPLES = 0b100, ///< Average over 128 samples
  INA3221_AVG_256_SAMPLES = 0b101, ///< Average over 256 samples
  INA3221_AVG_512_SAMPLES = 0b110, ///< Average over 512 samples
  INA3221_AVG_1024_SAMPLES = 0b111 ///< Average over 1024 samples
} ina3221_avgmode;

/**
 * @brief Enumeration for the conversion time values in the Configuration
 * register.
 *
 * Represents the conversion time for both bus voltage and shunt voltage.
 */
typedef enum {
  INA3221_CONVTIME_140US = 0b000, ///< Conversion time: 140µs exact
  INA3221_CONVTIME_204US = 0b001, ///< Conversion time: 204µs exact
  INA3221_CONVTIME_332US = 0b010, ///< Conversion time: 332µs exact
  INA3221_CONVTIME_588US = 0b011, ///< Conversion time: 588µs exact
  INA3221_CONVTIME_1MS = 0b100,   ///< Conversion time: 1.1ms exact
  INA3221_CONVTIME_2MS = 0b101,   ///< Conversion time: 2.116ms exact
  INA3221_CONVTIME_4MS = 0b110,   ///< Conversion time: 4.156ms exact
  INA3221_CONVTIME_8MS = 0b111    ///< Conversion time: 8.244ms exact
} ina3221_convtime;

/**
 * @brief Enumeration for the 'MODE [3-1]' values in the Configuration register.
 *
 * Represents the operating mode of the INA3221.
 */
typedef enum {
  INA3221_MODE_POWER_DOWN = 0b000,     ///< Power-down
  INA3221_MODE_SHUNT_TRIG = 0b001,     ///< Shunt voltage, triggered
  INA3221_MODE_BUS_TRIG = 0b010,       ///< Bus voltage, triggered
  INA3221_MODE_SHUNT_BUS_TRIG = 0b011, ///< Shunt and bus, triggered
  INA3221_MODE_POWER_DOWN2 = 0b100,    ///< Power-down (reserved mode)
  INA3221_MODE_SHUNT_CONT = 0b101,     ///< Shunt voltage, continuous
  INA3221_MODE_BUS_CONT = 0b110,       ///< Bus voltage, continuous
  INA3221_MODE_SHUNT_BUS_CONT = 0b111  ///< Shunt and bus, continuous
} ina3221_mode;

/**! Class to hold interface for INA3221 chip */

class Adafruit_INA3221 {
public:
  Adafruit_INA3221();

  bool begin(uint8_t i2c_addr = INA3221_DEFAULT_ADDRESS,
             TwoWire *theWire = &Wire);
  uint16_t getManufacturerID();
  uint16_t getDieID();
  float getShuntVoltage(uint8_t channel);
  float getBusVoltage(uint8_t channel);
  void setShuntResistance(uint8_t channel, float ohms);
  float getShuntResistance(uint8_t channel);
  float getCurrentAmps(uint8_t channel);
  bool reset();
  bool enableChannel(uint8_t channel);
  bool isChannelEnabled(uint8_t channel);
  bool setAveragingMode(ina3221_avgmode mode);
  ina3221_avgmode getAveragingMode();
  bool setBusVoltageConvTime(ina3221_convtime time);
  ina3221_convtime getBusVoltageConvTime();
  bool setShuntVoltageConvTime(ina3221_convtime time);
  ina3221_convtime getShuntVoltageConvTime();
  bool setMode(ina3221_mode mode);
  ina3221_mode getMode();
  bool setCriticalAlertThreshold(uint8_t channel, float current);
  float getCriticalAlertThreshold(uint8_t channel);
  bool setWarningAlertThreshold(uint8_t channel, float current);
  float getWarningAlertThreshold(uint8_t channel);
  bool setPowerValidLimits(float lowVlimit, float highVlimit);

  uint16_t getFlags();
  bool setSummationChannels(bool ch1, bool ch2, bool ch3);
  // float getCurrentSum();

private:
  Adafruit_I2CDevice *i2c_dev;
  float _shunt_resistance[3];
};

#endif // ADAFRUIT_INA3221_H
