/**
 * @file      TinyGsmGPS_EX.tpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-12-10
 *
 */
#ifndef SRC_TINYGSMGPS_EX_H_
#define SRC_TINYGSMGPS_EX_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_GPS_EX

class GPSInfo {
 public:
  // The rang is 0-255, unit is second. after set <time> will report the
  // GNSS information every the seconds.
  // The function will take effect immediately.

  uint8_t  isFix;                  // Fix mode,2=2D fix,3=3D fix
  uint16_t gps_satellite_num;      // GPS satellite visible numbers
  uint16_t beidou_satellite_num;   // BEIDOU satellite visible numbers
  uint16_t glonass_satellite_num;  // GLONASS satellite visible numbers
  uint16_t galileo_satellite_num;  // GALILEO satellite visible numbers
  double   latitude;      // Latitude of current position.Output format is dd.ddddd
  char     NS_indicator;  // N/S Indicator, N=north or S=south.
  double   longitude;     // Longitude of current position. Output format is ddd.ddddd
  char     EW_indicator;  // E/W Indicator, E=east or W=west.
  // Date. Output format is ddmmyy.
  uint16_t year;
  uint8_t  month;
  uint8_t  day;
  // UTC Time. Output format is hhmmss.ss.
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  float   altitude;  // MSL Altitude. Unit is meters.
  float   speed;     // Speed Over Ground. Unit is knots.
  float   course;    // Course. Degrees.F
  float   PDOP;      // Position Dilution Of Precision.
  float   HDOP;      // Horizontal Dilution Of Precision.
  float   VDOP;      // Vertical Dilution Of Precision.

  float   GSV;       // GNSS Satellites in View
  float   GSU;       // GNSS Satellites Used
  
};

template <class modemType>
class TinyGsmGPSEx {
 public:
  /*
   * GPS/GNSS/GLONASS enhancement functions
   */
  bool getGPS_Ex(GPSInfo& info) {
    return thisModem().getGPS_ExImpl(info);
  }

  bool gpsColdStart() {
    return thisModem().gpsColdStartImpl();
  }

  bool gpsWarmStart() {
    return thisModem().gpsWarmStartImpl();
  }

  bool gpsHotStart() {
    return thisModem().gpsHotStartImpl();
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
   * GPS/GNSS/GLONASS enhancement functions
   */
  bool getGPS_ExImpl(GPSInfo& info) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool gpsColdStartImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool gpsWarmStartImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool gpsHotStartImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
};


#endif  // SRC_TINYGSMGPS_EX_H_
