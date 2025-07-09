#include "Adafruit_INA3221.h"

/**
 * @brief Construct a new Adafruit_INA3221 object.
 */
Adafruit_INA3221::Adafruit_INA3221() {
  // Initialize the I2C device pointer to NULL
  i2c_dev = nullptr;

  // Set default shunt resistances to 0.05 ohms for all channels
  _shunt_resistance[0] = 0.05;
  _shunt_resistance[1] = 0.05;
  _shunt_resistance[2] = 0.05;
}

/**
 * @brief Initialize the INA3221 device with default settings.
 *
 * After initialization, the function performs the following configurations:
 * - Enables all three channels.
 * - Sets conversion time for both bus and shunt to 1ms.
 * - Sets averaging mode to 1x.
 * - Sets mode to continuous bus and shunt reading.
 *
 * @param i2c_addr The I2C address of the INA3221 device. Default is
 * INA3221_DEFAULT_ADDRESS.
 * @param theWire The I2C bus to use. Default is &Wire.
 * @return bool True if initialization and configuration were successful, false
 * otherwise.
 */
bool Adafruit_INA3221::begin(uint8_t i2c_addr, TwoWire *theWire) {
  // Initialize the I2C device
  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(i2c_addr, theWire);
  if (!i2c_dev->begin()) {
    return false;
  }

  // Check Manufacturer ID and Die ID
  if (getManufacturerID() != INA3221_MANUFACTURER_ID ||
      getDieID() != INA3221_DIE_ID) {
    return false;
  }

  // Perform a soft reset
  if (!reset()) {
    return false;
  }

  // Enable all three channels
  for (uint8_t i = 0; i < 3; i++) {
    if (!enableChannel(i)) {
      return false;
    }
  }

  // Set conversion time for both bus and shunt to 1ms
  if (!setBusVoltageConvTime(INA3221_CONVTIME_1MS) ||
      !setShuntVoltageConvTime(INA3221_CONVTIME_1MS)) {
    return false;
  }

  // Set averaging mode to 1x
  if (!setAveragingMode(INA3221_AVG_1_SAMPLE)) {
    return false;
  }

  // Set mode to continuous bus and shunt reading
  if (!setMode(INA3221_MODE_SHUNT_BUS_CONT)) {
    return false;
  }

  return true;
}

/**
 * @brief Get the shunt voltage for the specified channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return float The shunt voltage in volts. Returns NaN if the channel is
 * invalid.
 */
float Adafruit_INA3221::getShuntVoltage(uint8_t channel) {
  if (channel > 2) {
    return NAN; // Invalid channel
  }

  uint16_t regAddress;
  switch (channel) {
  case 0:
    regAddress = INA3221_REG_SHUNTVOLTAGE_CH1;
    break;
  case 1:
    regAddress = INA3221_REG_SHUNTVOLTAGE_CH2;
    break;
  case 2:
    regAddress = INA3221_REG_SHUNTVOLTAGE_CH3;
    break;
  }

  Adafruit_BusIO_Register shuntVoltage_reg(i2c_dev, regAddress, 2, MSBFIRST);
  int16_t rawValue = shuntVoltage_reg.read();

  // Convert raw value to voltage
  float voltage =
      (rawValue >> 3) * 40e-6; // Drop bottom 3 bits and multiply by 40uV

  return voltage;
}

/**
 * @brief Get the bus voltage for the specified channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return float The bus voltage in volts. Returns NaN if the channel is
 * invalid.
 */
float Adafruit_INA3221::getBusVoltage(uint8_t channel) {
  if (channel > 2) {
    return NAN; // Invalid channel
  }

  uint16_t regAddress;
  switch (channel) {
  case 0:
    regAddress = INA3221_REG_BUSVOLTAGE_CH1;
    break;
  case 1:
    regAddress = INA3221_REG_BUSVOLTAGE_CH2;
    break;
  case 2:
    regAddress = INA3221_REG_BUSVOLTAGE_CH3;
    break;
  }

  Adafruit_BusIO_Register busVoltage_reg(i2c_dev, regAddress, 2, MSBFIRST);
  int16_t rawValue = busVoltage_reg.read();

  // Convert raw value to voltage
  float voltage =
      (rawValue >> 3) * 8e-3; // Drop bottom 3 bits and multiply by 8mV

  return voltage;
}

/**
 * @brief Set the shunt resistance for a specific channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @param ohms The resistance value in ohms.
 */
void Adafruit_INA3221::setShuntResistance(uint8_t channel, float ohms) {
  if (channel < 3) {
    _shunt_resistance[channel] = ohms;
  }
}

/**
 * @brief Get the shunt resistance for a specific channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return float The resistance value in ohms for the specified channel.
 */
float Adafruit_INA3221::getShuntResistance(uint8_t channel) {
  if (channel < 3) {
    return _shunt_resistance[channel];
  }
  return NAN; // Return NaN for invalid channel
}

/**
 * @brief Get the current in amperes for a specific channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return float The current value in amperes for the specified channel.
 */
float Adafruit_INA3221::getCurrentAmps(uint8_t channel) {
  if (channel >= 3) {
    return NAN; // Return NaN for invalid channel
  }

  float shunt_voltage = getShuntVoltage(channel);

  // Check if shunt voltage is NaN before dividing
  if (isnan(shunt_voltage)) {
    return NAN;
  }

  return shunt_voltage / _shunt_resistance[channel];
}

/**
 * @brief Get the Die ID from the INA3221 - should be 0x3220
 *
 * @return uint16_t The 2-byte Die ID - should be 0x3220
 */
uint16_t Adafruit_INA3221::getDieID() {
  Adafruit_BusIO_Register dieID_reg(i2c_dev, INA3221_REG_DIE_ID, 2, MSBFIRST);
  return dieID_reg.read();
}

/**
 * @brief Get the Manufacturer ID from the INA3221 - should be 0x5449
 *
 * @return uint16_t The 2-byte Manufacturer ID - should be 0x5449
 */
uint16_t Adafruit_INA3221::getManufacturerID() {
  Adafruit_BusIO_Register manufacturerID_reg(
      i2c_dev, INA3221_REG_MANUFACTURER_ID, 2, MSBFIRST);
  return manufacturerID_reg.read();
}

/**
 * @brief Reset the INA3221 device.
 *
 * @return bool True if the reset was successful, false otherwise.
 */
bool Adafruit_INA3221::reset() {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits rst_bit(
      &config_reg, 1, 15); // RST is the 15th bit in the Configuration Register

  return rst_bit.write(1); // Set the RST bit to 1
}

/**
 * @brief Enable the specified channel.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::enableChannel(uint8_t channel) {
  if (channel > 2) {
    return false; // Invalid channel
  }

  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits channel_bit(
      &config_reg, 1, 14 - channel); // Channel enable bits are 12-14 in the
                                     // Configuration Register

  return channel_bit.write(1); // Set the corresponding channel bit to 1
}

/**
 * @brief Check if the specified channel is enabled.
 *
 * @param channel The channel number (0, 1, or 2).
 * @return bool True if the channel is enabled, false otherwise.
 */
bool Adafruit_INA3221::isChannelEnabled(uint8_t channel) {
  if (channel > 2) {
    return false; // Invalid channel
  }

  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits channel_bit(
      &config_reg, 1, 14 - channel); // Channel enable bits are 12-14 in the
                                     // Configuration Register

  return channel_bit.read(); // Read the corresponding channel bit
}

/**
 * @brief Set the averaging mode for the INA3221.
 *
 * @param mode The averaging mode. Possible values are:
 * - INA3221_AVG_1_SAMPLE
 * - INA3221_AVG_4_SAMPLES
 * - INA3221_AVG_16_SAMPLES
 * - INA3221_AVG_64_SAMPLES
 * - INA3221_AVG_128_SAMPLES
 * - INA3221_AVG_256_SAMPLES
 * - INA3221_AVG_512_SAMPLES
 * - INA3221_AVG_1024_SAMPLES
 *
 * @return bool True if the operation was successful, false otherwise.
 **/
bool Adafruit_INA3221::setAveragingMode(ina3221_avgmode mode) {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits avg_bits(
      &config_reg, 3, 9); // AVG2-0 bits are 9-11 in the Configuration Register

  return avg_bits.write(static_cast<uint8_t>(mode));
}

/**
 * @brief Get the current averaging mode of the INA3221.
 *
 * @return ina3221_avgmode The current averaging mode. Possible return values
 * are:
 * - INA3221_AVG_1_SAMPLE
 * - INA3221_AVG_4_SAMPLES
 * - INA3221_AVG_16_SAMPLES
 * - INA3221_AVG_64_SAMPLES
 * - INA3221_AVG_128_SAMPLES
 * - INA3221_AVG_256_SAMPLES
 * - INA3221_AVG_512_SAMPLES
 * - INA3221_AVG_1024_SAMPLES
 */
ina3221_avgmode Adafruit_INA3221::getAveragingMode() {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits avg_bits(
      &config_reg, 3, 9); // AVG2-0 bits are 9-11 in the Configuration Register

  return static_cast<ina3221_avgmode>(avg_bits.read());
}

/**
 * @brief Set the shunt voltage conversion time for the INA3221.
 *
 * @param convTime The shunt voltage conversion time. Possible values are:
 * - INA3221_CONVTIME_140US
 * - INA3221_CONVTIME_204US
 * - INA3221_CONVTIME_332US
 * - INA3221_CONVTIME_588US
 * - INA3221_CONVTIME_1MS
 * - INA3221_CONVTIME_2MS
 * - INA3221_CONVTIME_4MS
 * - INA3221_CONVTIME_8MS
 *
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setShuntVoltageConvTime(ina3221_convtime convTime) {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits vshct_bits(
      &config_reg, 3,
      3); // VSHCT 2-0 bits are 3-5 in the Configuration Register

  return vshct_bits.write(static_cast<uint8_t>(convTime));
}

/**
 * @brief Get the current shunt voltage conversion time of the INA3221.
 *
 * @return ina3221_convtime The current shunt voltage conversion time. Possible
 * return values are:
 * - INA3221_CONVTIME_140US
 * - INA3221_CONVTIME_204US
 * - INA3221_CONVTIME_332US
 * - INA3221_CONVTIME_588US
 * - INA3221_CONVTIME_1MS
 * - INA3221_CONVTIME_2MS
 * - INA3221_CONVTIME_4MS
 * - INA3221_CONVTIME_8MS
 */
ina3221_convtime Adafruit_INA3221::getShuntVoltageConvTime() {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits vshct_bits(
      &config_reg, 3,
      3); // VSHCT 2-0 bits are 3-5 in the Configuration Register

  return static_cast<ina3221_convtime>(vshct_bits.read());
}

/**
 * @brief Set the bus voltage conversion time for the INA3221.
 *
 * @param convTime The bus voltage conversion time. Possible values are:
 * - INA3221_CONVTIME_140US
 * - INA3221_CONVTIME_204US
 * - INA3221_CONVTIME_332US
 * - INA3221_CONVTIME_588US
 * - INA3221_CONVTIME_1MS
 * - INA3221_CONVTIME_2MS
 * - INA3221_CONVTIME_4MS
 * - INA3221_CONVTIME_8MS
 *
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setBusVoltageConvTime(ina3221_convtime convTime) {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits vbusct_bits(
      &config_reg, 3,
      6); // VBUSCT 2-0 bits are 6-8 in the Configuration Register

  return vbusct_bits.write(static_cast<uint8_t>(convTime));
}

/**
 * @brief Get the current bus voltage conversion time of the INA3221.
 *
 * @return ina3221_convtime The current bus voltage conversion time. Possible
 * return values are:
 * - INA3221_CONVTIME_140US
 * - INA3221_CONVTIME_204US
 * - INA3221_CONVTIME_332US
 * - INA3221_CONVTIME_588US
 * - INA3221_CONVTIME_1MS
 * - INA3221_CONVTIME_2MS
 * - INA3221_CONVTIME_4MS
 * - INA3221_CONVTIME_8MS
 */
ina3221_convtime Adafruit_INA3221::getBusVoltageConvTime() {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits vbusct_bits(
      &config_reg, 3,
      6); // VBUSCT 2-0 bits are 6-8 in the Configuration Register

  return static_cast<ina3221_convtime>(vbusct_bits.read());
}

/**
 * @brief Set the operating mode for the INA3221.
 *
 * @param mode The operating mode. Possible values are:
 * - INA3221_MODE_POWER_DOWN
 * - INA3221_MODE_SHUNT_TRIG
 * - INA3221_MODE_BUS_TRIG
 * - INA3221_MODE_SHUNT_BUS_TRIG
 * - INA3221_MODE_POWER_DOWN2
 * - INA3221_MODE_SHUNT_CONT
 * - INA3221_MODE_BUS_CONT
 * - INA3221_MODE_SHUNT_BUS_CONT
 *
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setMode(ina3221_mode mode) {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits mode_bits(
      &config_reg, 3,
      0); // MODE [3-1] bits are 0-2 in the Configuration Register

  return mode_bits.write(static_cast<uint8_t>(mode));
}

/**
 * @brief Get the current operating mode of the INA3221.
 *
 * @return ina3221_mode The current operating mode. Possible return values are:
 * - INA3221_MODE_POWER_DOWN
 * - INA3221_MODE_SHUNT_TRIG
 * - INA3221_MODE_BUS_TRIG
 * - INA3221_MODE_SHUNT_BUS_TRIG
 * - INA3221_MODE_POWER_DOWN2
 * - INA3221_MODE_SHUNT_CONT
 * - INA3221_MODE_BUS_CONT
 * - INA3221_MODE_SHUNT_BUS_CONT
 */
ina3221_mode Adafruit_INA3221::getMode() {
  Adafruit_BusIO_Register config_reg(i2c_dev, INA3221_REG_CONFIGURATION, 2,
                                     MSBFIRST);
  Adafruit_BusIO_RegisterBits mode_bits(
      &config_reg, 3,
      0); // MODE [3-1] bits are 0-2 in the Configuration Register

  return static_cast<ina3221_mode>(mode_bits.read());
}

/**
 * @brief Set the Critical-Alert threshold for a specific channel.
 *
 * @param channel The channel number (0-2).
 * @param current The current threshold in Amperes.
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setCriticalAlertThreshold(uint8_t channel,
                                                 float current) {
  if (channel > 2)
    return false; // Invalid channel
  int16_t threshold = static_cast<int16_t>(
      current * _shunt_resistance[channel] / 40e-6 * 8); // 40uV per LSB

  uint16_t reg_addr = INA3221_REG_CRITICAL_ALERT_LIMIT_CH1 + 2 * channel;

  Adafruit_BusIO_Register alert_reg(i2c_dev, reg_addr, 2, MSBFIRST);
  return alert_reg.write(threshold);
}

/**
 * @brief Get the Critical-Alert threshold for a specific channel.
 *
 * @param channel The channel number (0-2).
 * @return float The current threshold in Amperes. Returns NaN for invalid
 * channel.
 */
float Adafruit_INA3221::getCriticalAlertThreshold(uint8_t channel) {
  if (channel > 2)
    return NAN; // Invalid channel

  uint16_t reg_addr = INA3221_REG_CRITICAL_ALERT_LIMIT_CH1 + 2 * channel;
  Adafruit_BusIO_Register alert_reg(i2c_dev, reg_addr, 2, MSBFIRST);

  int16_t threshold = alert_reg.read();

  // Convert back to Amperes
  float threshA = (float)(threshold >> 3) * 40e-6 / _shunt_resistance[channel];
  return threshA;
}

/**
 * @brief Set the Warning-Alert threshold for a specific channel.
 *
 * @param channel The channel number (0-2).
 * @param current The current threshold in Amperes.
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setWarningAlertThreshold(uint8_t channel,
                                                float current) {
  if (channel > 2)
    return false; // Invalid channel

  int16_t threshold =
      static_cast<int16_t>(current * _shunt_resistance[channel] *
                           8); // Convert to int16_t, considering the 3 LSBs
  uint16_t reg_addr = INA3221_REG_WARNING_ALERT_LIMIT_CH1 + channel;

  Adafruit_BusIO_Register alert_reg(i2c_dev, reg_addr, 2, MSBFIRST);
  return alert_reg.write(threshold);
}

/**
 * @brief Get the Warning-Alert threshold for a specific channel.
 *
 * @param channel The channel number (0-2).
 * @return float The current threshold in Amperes. Returns NaN for invalid
 * channel.
 */
float Adafruit_INA3221::getWarningAlertThreshold(uint8_t channel) {
  if (channel > 2)
    return NAN; // Invalid channel

  uint16_t reg_addr = INA3221_REG_WARNING_ALERT_LIMIT_CH1 + channel;
  Adafruit_BusIO_Register alert_reg(i2c_dev, reg_addr, 2, MSBFIRST);

  int16_t threshold = alert_reg.read();
  return static_cast<float>(threshold) /
         (_shunt_resistance[channel] * 8); // Convert back to Amperes
}

/**
 * @brief Get the flag indicators from the Mask/Enable Register.
 *
 * @return uint16_t The flag indicators. Use the defined bit flags to check
 * specific flags.
 *
 * Possible flags to test against:
 * - INA3221_CONV_READY
 * - INA3221_TIMECONT_ALERT
 * - INA3221_POWER_VALID
 * - INA3221_WARN_CH3
 * - INA3221_WARN_CH2
 * - INA3221_WARN_CH1
 * - INA3221_SUMMATION
 * - INA3221_CRITICAL_CH3
 * - INA3221_CRITICAL_CH2
 * - INA3221_CRITICAL_CH1
 */
uint16_t Adafruit_INA3221::getFlags() {
  Adafruit_BusIO_Register mask_enable_reg(i2c_dev, INA3221_REG_MASK_ENABLE, 2,
                                          MSBFIRST);
  uint16_t flags = mask_enable_reg.read();

  // Mask to keep only the relevant flag bits
  uint16_t mask = INA3221_CONV_READY | INA3221_TIMECONT_ALERT |
                  INA3221_POWER_VALID | INA3221_WARN_CH3 | INA3221_WARN_CH2 |
                  INA3221_WARN_CH1 | INA3221_SUMMATION | INA3221_CRITICAL_CH3 |
                  INA3221_CRITICAL_CH2 | INA3221_CRITICAL_CH1;

  return flags & mask;
}

/**
 * @brief Set the summation channels in the Mask/Enable Register.
 *
 * @param ch1 Enable or disable summation for channel 1.
 * @param ch2 Enable or disable summation for channel 2.
 * @param ch3 Enable or disable summation for channel 3.
 * @return bool True if the operation was successful, false otherwise.
 */
bool Adafruit_INA3221::setSummationChannels(bool ch1, bool ch2, bool ch3) {
  Adafruit_BusIO_Register mask_enable_reg(i2c_dev, INA3221_REG_MASK_ENABLE, 2,
                                          MSBFIRST);

  // Create a RegisterBits object for the SCC bits
  Adafruit_BusIO_RegisterBits scc_bits(&mask_enable_reg, 3,
                                       12); // 3 bits starting from bit 12

  // Calculate the new SCC value based on the provided boolean values
  uint16_t scc_value = (ch3 << 0) | (ch2 << 1) | (ch1 << 2);

  // Write the new SCC value
  return scc_bits.write(scc_value);
}

/**
 * @brief Set the Power-Valid upper and lower limits
 *
 * This function writes the specified upper and lower voltage limits to the
 * INA3221's Power-Valid registers. The voltage values are converted into
 * a 16-bit register value (each LSB represents 8 mV).
 *
 * @param vlimitlow The lower voltage limit in volts.
 * @param vlimithigh The upper voltage limit in volts.
 * @return true if both writes were successful, false otherwise.
 */
bool Adafruit_INA3221::setPowerValidLimits(float vlimitlow, float vlimithigh) {
  // Create the Adafruit_BusIO_Register object for the lower limit register
  Adafruit_BusIO_Register low_limit_reg_obj = Adafruit_BusIO_Register(
      i2c_dev, INA3221_REG_POWERVALID_LOWERLIMIT, 2, MSBFIRST);

  // Write the lower limit value and check if successful
  bool successLow = low_limit_reg_obj.write((vlimitlow * 1000));

  // Create the Adafruit_BusIO_Register object for the upper limit register
  Adafruit_BusIO_Register high_limit_reg_obj = Adafruit_BusIO_Register(
      i2c_dev, INA3221_REG_POWERVALID_UPPERLIMIT, 2, MSBFIRST);

  // Write the upper limit value and check if successful
  bool successHigh = high_limit_reg_obj.write((vlimithigh * 1000));

  // Return true only if both writes were successful
  return successLow && successHigh;
}
