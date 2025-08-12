/**
 * @file       TinyGsmGPS.tpp
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMGPS_H_
#define SRC_TINYGSMGPS_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_GPS

#define GSM_MODEM_AUX_POWER     (127)
template <class modemType>
class TinyGsmGPS {
 public:
  /*
   * GPS/GNSS/GLONASS location functions
   */ 
  //power_en_pin: modem gpio number
  bool enableGPS(int8_t power_en_pin = -1,uint8_t enable_level = 1) {
    return thisModem().enableGPSImpl(power_en_pin,enable_level);
  }
  bool disableGPS(int8_t power_en_pin =-1,uint8_t disable_level = 0) {
    return thisModem().disableGPSImpl(power_en_pin , disable_level);
  }
  bool isEnableGPS() {
    return thisModem().isEnableGPSImpl();
  }
  String getGPSraw() {
    return thisModem().getGPSrawImpl();
  }
  bool enableAGPS(){
    return thisModem().enableAGPSImpl();
  }
  bool getGPS(uint8_t *status,float* lat, float* lon, float* speed = 0, float* alt = 0,
              int* vsat = 0, int* usat = 0, float* accuracy = 0, int* year = 0,
              int* month = 0, int* day = 0, int* hour = 0, int* minute = 0,
              int* second = 0) {
    return thisModem().getGPSImpl(status,lat, lon, speed, alt, vsat, usat, accuracy,
                                  year, month, day, hour, minute, second);
  }
  bool getGPSTime(int* year, int* month, int* day, int* hour, int* minute,
                  int* second) {
    float lat = 0;
    float lon = 0;
    uint8_t status;
    return thisModem().getGPSImpl(&status,&lat, &lon, 0, 0, 0, 0, 0, year, month, day,
                                  hour, minute, second);
  }

  bool setGPSBaud(uint32_t baud){
    return thisModem().setGPSBaudImpl(baud);
  }

  bool setGPSMode(uint8_t mode){
    return thisModem().setGPSModeImpl(mode);
  }

  bool setGPSOutputRate(uint8_t rate_hz){
    return thisModem().setGPSOutputRateImpl(rate_hz);
  }

  bool enableNMEA(bool outputAtPort = true){
    return thisModem().enableNMEAImpl(outputAtPort);
  }

  bool disableNMEA(){
    return thisModem().disableNMEAImpl();
  }

  bool configNMEASentence(uint32_t nmea_mask){
    return thisModem().configNMEASentenceImpl(nmea_mask);
  }
  /*
   * CRTP Helper
   */
 protected:
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  /*
   * GPS/GNSS/GLONASS location functions
   */

  bool    enableGPSImpl(int8_t power_en_pin ,uint8_t enable_level) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    disableGPSImpl(int8_t power_en_pin ,uint8_t disable_level) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    isEnableGPSImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    enableAGPSImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  String  getGPSrawImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    getGPSImpl(uint8_t *status,float* lat, float* lon, float* speed = 0, float* alt = 0,
                     int* vsat = 0, int* usat = 0, float* accuracy = 0,
                     int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                     int* minute = 0,
                     int* second = 0) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    setGPSBaudImpl(uint32_t baud)TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    setGPSModeImpl(uint8_t mode)TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    setGPSOutputRateImpl(uint8_t rate_hz)TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    enableNMEAImpl(bool outputAtPort = true)TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    disableNMEAImpl()TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool    configNMEASentenceImpl(uint32_t nmea_mask)TINY_GSM_ATTR_NOT_IMPLEMENTED;
};


#endif  // SRC_TINYGSMGPS_H_
