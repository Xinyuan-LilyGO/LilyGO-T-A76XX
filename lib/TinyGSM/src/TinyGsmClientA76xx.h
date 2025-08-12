/**
 * @file      TinyGsmClientA76xx.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-12-05
 *
 */

#ifndef SRC_TINYGSMCLIENTA76XX_H_
#define SRC_TINYGSMCLIENTA76XX_H_

// #define TINY_GSM_DEBUG Serial
// #define TINY_GSM_USE_HEX

#include "TinyGsmBattery.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmModem.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmTemperature.tpp"
#include "TinyGsmTextToSpeech.tpp"
#include "TinyGsmGPS_EX.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmEmail.tpp"

#define GSM_NL "\r\n"
static const char GSM_OK[] TINY_GSM_PROGMEM    = "OK" GSM_NL;
static const char GSM_ERROR[] TINY_GSM_PROGMEM = "ERROR" GSM_NL;
#if defined TINY_GSM_DEBUG
static const char GSM_CME_ERROR[] TINY_GSM_PROGMEM = GSM_NL "+CME ERROR:";
static const char GSM_CMS_ERROR[] TINY_GSM_PROGMEM = GSM_NL "+CMS ERROR:";
#endif

enum RegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
  REG_SMS_ONLY     = 6,
  REG_EMERGENCY_ONLY = 11
};

enum NetworkMode {
  MODEM_NETWORK_AUTO  = 2,
  MODEM_NETWORK_GSM   = 13,
  MODEM_NETWORK_WCDMA = 14,
  MODEM_NETWORK_LTE   = 38,
  MODEM_NETWORK_UNKNOWN  = 255,
};

enum NMEA_Sentences {
    NMEA_GPGGA   = _BV(0),    // Bit 0: GPGGA (global positioning system fix data)
    NMEA_GPGLL   = _BV(1),    // Bit 1: GPGLL (The latitude and longitude of the ship's current position, positioning time and status)
    NMEA_GPGSA   = _BV(2),    // Bit 2: GPGSA (GPS DOP and active satellites)
    NMEA_GPGSV   = _BV(3),    // Bit 3: GPGSV (GPS satellites in view)
    NMEA_GPRMC   = _BV(4),    // Bit 4: GPRMC (recommended minimum specific GPS/TRANSIT data)
    NMEA_GPVTG   = _BV(5),    // Bit 5: GPVTG (track made good and ground speed)
    NMEA_ZDA   = _BV(6),      // Bit 6: https://receiverhelp.trimble.com/alloy-gnss/en-us/NMEA-0183messages_ZDA.html
    NMEA_GST   = _BV(7),      // Bit 7: https://receiverhelp.trimble.com/alloy-gnss/en-us/nmea0183-messages-gst.html?Highlight=GST
};

constexpr char EFS_PATH[] = "C";

template <class modemType>
class TinyGsmA76xx : public TinyGsmModem<TinyGsmA76xx<modemType>>,
                     public TinyGsmGPRS<TinyGsmA76xx<modemType>>,
                     public TinyGsmSMS<TinyGsmA76xx<modemType>>,
                     public TinyGsmGPS<TinyGsmA76xx<modemType>>,
                     public TinyGsmTime<TinyGsmA76xx<modemType>>,
                     public TinyGsmNTP<TinyGsmA76xx<modemType>>,
                     public TinyGsmBattery<TinyGsmA76xx<modemType>>,
                     public TinyGsmTemperature<TinyGsmA76xx<modemType>>,
                     public TinyGsmTextToSpeech<TinyGsmA76xx<modemType>>,
                     public TinyGsmGSMLocation<TinyGsmA76xx<modemType>>,
                     public TinyGsmCalling<TinyGsmA76xx<modemType>>,
                     public TinyGsmGPSEx<TinyGsmA76xx<modemType>>,
                     public TinyGsmEmail<TinyGsmA76xx<modemType>>{
  friend class TinyGsmModem<TinyGsmA76xx<modemType>>;
  friend class TinyGsmGPRS<TinyGsmA76xx<modemType>>;
  friend class TinyGsmSMS<TinyGsmA76xx<modemType>>;
  friend class TinyGsmGPS<TinyGsmA76xx<modemType>>;
  friend class TinyGsmTime<TinyGsmA76xx<modemType>>;
  friend class TinyGsmNTP<TinyGsmA76xx<modemType>>;
  friend class TinyGsmBattery<TinyGsmA76xx<modemType>>;
  friend class TinyGsmGSMLocation<TinyGsmA76xx<modemType>>;
  friend class TinyGsmTemperature<TinyGsmA76xx<modemType>>;
  friend class TinyGsmTextToSpeech<TinyGsmA76xx<modemType>>;
  friend class TinyGsmGPSEx<TinyGsmA76xx<modemType>>;
  friend class TinyGsmCalling<TinyGsmA76xx<modemType>>;
  friend class TinyGsmEmail<TinyGsmA76xx<modemType>>;
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
   * Constructor
   */
 public:
  explicit TinyGsmA76xx(Stream& stream) : stream(stream) {}

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    return thisModem().initImpl(pin);
  }

  String getModemNameImpl() {
    String name = "UNKNOWN";
    String res;

    thisModem().sendAT(GF("E0"));  // Echo Off
    thisModem().waitResponse();

    thisModem().sendAT("I");
    if (thisModem().waitResponse(10000L, res) != 1) {
      DBG("MODEM STRING NO FOUND!");
      return name;
    }
    int modelIndex    = res.indexOf("Model:") + 6;
    int nextLineIndex = res.indexOf('\n', modelIndex);
    if (nextLineIndex != -1) {
      String modelString = res.substring(modelIndex, nextLineIndex);
      modelString.trim();
      if (modelString.startsWith("A76")) {
        name = modelString;
        DBG("### Modem:", name);
      }
    } else {
      DBG("Model string not found.");
    }
    return name;
  }

  bool factoryDefaultImpl() {           // these commands aren't supported
    thisModem().sendAT(GF("&FZE0&W"));  // Factory + Reset + Echo Off + Write
    thisModem().waitResponse();
    thisModem().sendAT(GF("+IPR=0"));  // Auto-baud
    thisModem().waitResponse();
    thisModem().sendAT(GF("+IFC=0,0"));  // No Flow Control
    thisModem().waitResponse();
    thisModem().sendAT(GF("+ICF=3,3"));  // 8 data 0 parity 1 stop
    thisModem().waitResponse();
    thisModem().sendAT(GF("+CSCLK=0"));  // Disable Slow Clock
    thisModem().waitResponse();
    thisModem().sendAT(GF("&W"));  // Write configuration
    return thisModem().waitResponse() == 1;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = NULL) {
    thisModem().sendAT(GF("+CRESET"));
    thisModem().waitResponse();
    thisModem().waitResponse(30000L, GF("SMS Ready"));
    return thisModem().initImpl(pin);
  }

  bool powerOffImpl() {
    thisModem().sendAT(GF("+CPOF"));
    return thisModem().waitResponse() == 1;
  }

  // During sleep, the SIM70xx module has its serial communication disabled.
  // In order to reestablish communication pull the DRT-pin of the SIM70xx
  // module LOW for at least 50ms. Then use this function to disable sleep
  // mode. The DTR-pin can then be released again.
  bool sleepEnableImpl(bool enable = true) {
    thisModem().sendAT(GF("+CSCLK="), enable);
    return thisModem().waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    thisModem().sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return thisModem().waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  RegStatus getRegistrationStatus() {
    RegStatus epsStatus = (RegStatus)thisModem().getRegistrationStatusXREG("CEREG");
    // If we're connected on EPS, great!
    if (epsStatus == REG_OK_HOME || epsStatus == REG_OK_ROAMING) {
      return epsStatus;
    } else {
      // Otherwise, check GPRS network status
      // We could be using GPRS fall-back or the board could be being moody
      return (RegStatus)thisModem().getRegistrationStatusXREG("CGREG");
    }
  }

 protected:
  bool isNetworkConnectedImpl() {
    RegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

 public:
  String getNetworkModeString() {
    int16_t mode = getNetworkMode();
    switch (mode) {
      case MODEM_NETWORK_AUTO: return "AUTO";
      case MODEM_NETWORK_GSM: return "GSM";
      case MODEM_NETWORK_WCDMA: return "WCDMA";
      case MODEM_NETWORK_LTE: return "LTE";
      default: break;
    }
    return "UNKNOWN";
  }

  NetworkMode getNetworkMode() {
    thisModem().sendAT(GF("+CNMP?"));
    if (thisModem().waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return MODEM_NETWORK_UNKNOWN; }
    int16_t mode = thisModem().streamGetIntBefore('\n');
    thisModem().waitResponse();
    return static_cast<NetworkMode>(mode);
  }

  bool setNetworkMode(NetworkMode mode) {
    switch (mode) {
      case MODEM_NETWORK_AUTO:
      case MODEM_NETWORK_GSM:
      case MODEM_NETWORK_WCDMA:
      case MODEM_NETWORK_LTE: break;
      default: return false;
    }
    thisModem().sendAT(GF("+CNMP="), mode);
    return thisModem().waitResponse() == 1;
  }

  String getLocalIP() {
    String res;
    thisModem().sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    if (thisModem().waitResponse(GF("+IPADDR: ")) != 1) { return ""; }
    thisModem().waitResponse(1000, res);
    res.replace(GSM_NL "OK" GSM_NL, "");
    res.replace(GSM_NL, "");
    res.trim();
    return res;
  }

  bool setNetworkActive() {
    thisModem().sendAT(GF("+NETOPEN"));
    int res = thisModem().waitResponse(GF("+NETOPEN: 0"),
                                       GF("+IP ERROR: Network is already opened"));
    if (res == 2) {
      // DBG("> Network has open......");
      thisModem().waitResponse();
    }
    if (res != 1 && res != 2) { return false; }
    return true;
  }

  bool setNetworkDeactivate() {
    thisModem().sendAT(GF("+NETCLOSE"));
    if (thisModem().waitResponse() != 1) { return false; }
    int res = thisModem().waitResponse(GF("+NETCLOSE: 0"), GF("+NETCLOSE: 2"));
    if (res != 1 && res != 2) { return false; }
    return true;
  }

  bool getNetworkActive() {
    thisModem().sendAT(GF("+NETOPEN?"));
    int res = thisModem().waitResponse(GF("+NETOPEN: 1"));
    if (res == 1) { return true; }
    return false;
  }

  String getNetworkAPN() {
    thisModem().sendAT("+CGDCONT?");
    if (thisModem().waitResponse(GF(GSM_NL "+CGDCONT: ")) != 1) { return "ERROR"; }
    thisModem().streamSkipUntil(',');
    thisModem().streamSkipUntil(',');
    thisModem().streamSkipUntil('\"');
    String res = thisModem().stream.readStringUntil('\"');
    thisModem().waitResponse();
    if (res == "") { res = "APN IS NOT SET"; }
    return res;
  }

  bool setNetworkAPN(String apn) {
    thisModem().sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, "\"");
    return thisModem().waitResponse() == 1;
  }

  /*
   * Return code:
   *     -1 ping failed
   *     1 Ping success
   *     2 Ping time out
   *     3 Ping result
   * * */
  int ping(const char* url, String& resolved_ip_addr, uint32_t& rep_data_packet_size,
           uint32_t& tripTime, uint8_t& TTL) {
    uint8_t  dest_addr_type   = 1;
    uint8_t  num_pings        = 1;
    uint8_t  data_packet_size = 64;
    uint32_t interval_time    = 1000;
    uint32_t wait_time        = 10000;
    uint8_t  ttl              = 0xFF;
    int      result_type      = -1;

    thisModem().sendAT("+CPING=\"", url, "\"", ",", dest_addr_type, ",", num_pings, ",",
                       data_packet_size, ",", interval_time, ",", wait_time, ",", ttl);

    if (thisModem().waitResponse() != 1) { return -1; }
    if (thisModem().waitResponse(10000UL, "+CPING: ") == 1) {
      result_type = thisModem().streamGetIntBefore(',');
      switch (result_type) {
        case 1:
          resolved_ip_addr     = stream.readStringUntil(',');
          rep_data_packet_size = thisModem().streamGetIntBefore(',');
          tripTime             = thisModem().streamGetIntBefore(',');
          TTL                  = thisModem().streamGetIntBefore('\n');
          break;
        case 2: break;
        case 3: break;
        default: break;
      }
    }
    return result_type;
  }

  /*
   * Hardware functions
   */
  bool pinMode(uint8_t pin, uint8_t mode) {
      switch (mode) {
      case INPUT:
          mode = 0;   //IN
          break;
      case OUTPUT:
          mode = 1;   //OUTPUT
          break;
      default:
          return false;
      }
      thisModem().sendAT("+CGDRT=", pin, ',', mode);
      if (thisModem().waitResponse() != 1) {
          return false;
      }
      return true;
  }

  bool digitalWrite(uint8_t pin, uint8_t val) {
      thisModem().sendAT("+CGSETV=", pin, ',', val);
      if (thisModem().waitResponse() != 1) {
          return false;
      }
      return true;
  }

  /*
   * GPRS functions
   */
 protected:
  // should implement in sub-classes
  bool gprsConnectImpl(const char* apn, const char* user = NULL, const char* pwd = NULL) {
    return thisModem().gprsConnectImpl(apn, user, pwd);
  }

  bool gprsDisconnectImpl() {
    return thisModem().gprsDisconnectImpl();
  }

  /*
   * SIM card functions
   */
 protected:
  // Doesn't return the "+CICCID" before the number
  String getSimCCIDImpl() {
    thisModem().sendAT(GF("+CICCID"));
    if (thisModem().waitResponse(GF(GSM_NL)) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  /*
   * Messaging functions
   */
 protected:
  // Follows all messaging functions per template

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl(int8_t power_en_pin, uint8_t enable_level) {
    if (power_en_pin == GSM_MODEM_AUX_POWER) {
      thisModem().sendAT("+CVAUXS=1");
      thisModem().waitResponse();
    } else if (power_en_pin != -1) {
      thisModem().sendAT("+CGDRT=", power_en_pin, ",1");
      thisModem().waitResponse();
      thisModem().sendAT("+CGSETV=", power_en_pin, ",", enable_level);
      thisModem().waitResponse();
    }
    // Detection turned on?
    // if(gpsHotStartImpl()){
    //   return true;
    // }
    thisModem().sendAT(GF("+CGNSSPWR=1"));
    if (thisModem().waitResponse(30000UL, "+CGNSSPWR: READY!") != 1) { return false; }
    return true;
  }

  bool disableGPSImpl(int8_t power_en_pin, uint8_t disable_level) {
    if (power_en_pin == GSM_MODEM_AUX_POWER) {
      thisModem().sendAT("+CVAUXS=0");
      thisModem().waitResponse();
    } else if (power_en_pin != -1) {
      thisModem().sendAT("+CGDRT=", power_en_pin, ",1");
      thisModem().waitResponse();
      thisModem().sendAT("+CGSETV=", power_en_pin, ",", disable_level);
      thisModem().waitResponse();
      thisModem().sendAT("+CGDRT=", power_en_pin, ",0");
      thisModem().waitResponse();
    }
    // Detection turned off?
    if(!isEnableGPSImpl()){
      return true;
    }
    thisModem().sendAT(GF("+CGNSSPWR=0"));
    if (thisModem().waitResponse() != 1) { return false; }
    return true;
  }

  bool isEnableGPSImpl() {
    thisModem().sendAT(GF("+CGNSSPWR?"));
    if (thisModem().waitResponse("+CGNSSPWR:") != 1) { return false; }
    // +CGNSSPWR:<GNSS_Power_status>,<AP_Flash_status>,<GNSS_dynamic_load>
    bool running = 1 == thisModem().streamGetIntBefore(',');
    thisModem().waitResponse();
    return running;
  }

  bool enableAGPSImpl() {
    thisModem().sendAT(GF("+CGNSSPWR?"));
    if (thisModem().waitResponse("+CGNSSPWR:") != 1) { return false; }
    // +CGNSSPWR:<GNSS_Power_status>,<AP_Flash_status>,<GNSS_dynamic_load>
    if (1 == thisModem().streamGetIntBefore(',')) {
      thisModem().sendAT("+CAGPS");
      if (thisModem().waitResponse(30000UL, "+AGPS:") != 1) { return false; }
      String res = stream.readStringUntil('\n');
      if (res.startsWith(" success.")) { return true; }
    }
    return false;
  }

  bool setGPSBaudImpl(uint32_t baud) {
    thisModem().sendAT("+CGNSSIPR=", baud);
    return thisModem().waitResponse(1000L) == 1;
  }

  bool setGPSModeImpl(uint8_t mode) {
    thisModem().sendAT("+CGNSSMODE=", mode);
    return thisModem().waitResponse(1000L) == 1;
  }

  bool setGPSOutputRateImpl(uint8_t rate_hz) {
    thisModem().sendAT("+CGPSNMEARATE=", rate_hz);
    return thisModem().waitResponse(1000L) == 1;
  }

  bool enableNMEAImpl(bool outputAtPort) {
    if(outputAtPort){
      thisModem().sendAT("+CGNSSTST=1");
    }else{
      thisModem().sendAT("+CGNSSTST=0");
    }
    if (thisModem().waitResponse(1000L) != 1) { return false; }
    // Select the output port for NMEA sentence
    thisModem().sendAT("+CGNSSPORTSWITCH=0,1");
    if (thisModem().waitResponse(1000L) != 1) { return false; }
    return true;
  }

  bool disableNMEAImpl() {
    thisModem().sendAT("+CGNSSTST=0");
    thisModem().waitResponse(1000L);
    // Select the output port for NMEA sentence
    thisModem().sendAT("+CGNSSPORTSWITCH=1,0");
    return thisModem().waitResponse(1000L) == 1;
  }

  bool configNMEASentenceImpl(uint32_t nmea_mask) {
    thisModem().sendAT("+CGNSSNMEA=", 
      nmea_mask & NMEA_GPGGA ? "1": "0", "," ,
      nmea_mask & NMEA_GPGLL ? "1": "0", "," ,
      nmea_mask & NMEA_GPGSA ? "1": "0", "," ,
      nmea_mask & NMEA_GPGSV ? "1": "0", "," ,
      nmea_mask & NMEA_GPRMC ? "1": "0", "," ,
      nmea_mask & NMEA_GPVTG ? "1": "0", "," ,
      nmea_mask & NMEA_ZDA   ? "1": "0", "," ,
      nmea_mask & NMEA_GST   ? "1": "0");
    return thisModem().waitResponse(1000L) == 1;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    thisModem().sendAT(GF("+CGNSSINFO"));
    if (thisModem().waitResponse(10000L, GF(GSM_NL "+CGNSSINFO:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  bool gpsColdStartImpl() {
    thisModem().sendAT(GF("+CGPSCOLD"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsWarmStartImpl() {
    thisModem().sendAT(GF("+CGPSWARM"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsHotStartImpl() {
    thisModem().sendAT(GF("+CGPSHOT"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool getGPS_ExImpl(GPSInfo& info) {
    float lat = 0;
    float lon = 0;
    // +CGNSSINFO:[<mode>],
    // [<GPS-SVs>],[BEIDOU-SVs],[<GLONASS-SVs>],[<GALILEO-SVs>],
    // [<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[<speed>],[<course>],[<PDOP>],[HDOP],[VDOP]
    thisModem().sendAT(GF("+CGNSSINFO"));
    if (thisModem().waitResponse(GF(GSM_NL "+CGNSSINFO: ")) != 1) { return false; }

    info.isFix = thisModem().streamGetIntBefore(',');  // mode 2=2D Fix or 3=3DFix
    if (info.isFix == 2 || info.isFix == 3) {
      int16_t ret = -9999;
      // GPS-SVs      satellite valid numbers
      ret                    = thisModem().streamGetIntBefore(',');
      info.gps_satellite_num = ret != -9999 ? ret : 0;
      // BEIDOU-SVs   satellite valid numbers
      ret                       = thisModem().streamGetIntBefore(',');
      info.beidou_satellite_num = ret != -9999 ? ret : 0;
      // GLONASS-SVs  satellite valid numbers
      ret                        = thisModem().streamGetIntBefore(',');
      info.glonass_satellite_num = ret != -9999 ? ret : 0;
      // GALILEO-SVs  satellite valid numbers
      ret                        = thisModem().streamGetIntBefore(',');
      info.galileo_satellite_num = ret != -9999 ? ret : 0;
      // Latitude in ddmm.mmmmmm
      lat = thisModem().streamGetFloatBefore(',');
      // N/S Indicator, N=north or S=south
      info.NS_indicator = stream.read();
      thisModem().streamSkipUntil(',');
      // Longitude in ddmm.mmmmmm
      lon = thisModem().streamGetFloatBefore(',');
      // E/W Indicator, E=east or W=west
      info.EW_indicator = stream.read();
      thisModem().streamSkipUntil(',');
      // Date. Output format is ddmmyy
      // Two digit day
      info.day = thisModem().streamGetIntLength(2);
      // Two digit month
      info.month = thisModem().streamGetIntLength(2);
      // Two digit year
      info.year = thisModem().streamGetIntBefore(',');
      // UTC Time. Output format is hhmmss.s
      // Two digit hour
      info.hour = thisModem().streamGetIntLength(2);
      // Two digit minute
      info.minute = thisModem().streamGetIntLength(2);
      // 4 digit second with subseconds
      float secondWithSS = thisModem().streamGetFloatBefore(',');
      info.second        = static_cast<int>(secondWithSS);
      // MSL Altitude. Unit is meters
      info.altitude = thisModem().streamGetFloatBefore(',');
      // Speed Over Ground. Unit is knots.
      info.speed = thisModem().streamGetFloatBefore(',');
      // Course Over Ground. Degrees.
      info.course = thisModem().streamSkipUntil(',');
      // After set, will report GPS every x seconds
      thisModem().streamSkipUntil(',');
      // Position Dilution Of Precision
      float pdop = thisModem().streamGetFloatBefore(',');
      info.PDOP  = pdop != -9999.0F ? pdop : 0;
      // Horizontal Dilution Of Precision
      float hdop = thisModem().streamGetFloatBefore(',');
      info.HDOP  = hdop != -9999.0F ? hdop : 0;
      // Vertical Dilution Of Precision
      float vdop = thisModem().streamGetFloatBefore(',');
      info.VDOP  = vdop != -9999.0F ? vdop : 0;
      thisModem().streamSkipUntil('\n');
      thisModem().waitResponse();
      info.latitude  = (lat) * (info.NS_indicator == 'N' ? 1 : -1);
      info.longitude = (lon) * (info.EW_indicator == 'E' ? 1 : -1);
      if (info.year < 2000) { info.year += 2000; }
      return true;
    }

    thisModem().waitResponse();
    return false;
  }


  // get GPS informations
  bool getGPSImpl(uint8_t* status, float* lat, float* lon, float* speed = 0,
                  float* alt = 0, int* vsat = 0, int* usat = 0, float* accuracy = 0,
                  int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                  int* minute = 0, int* second = 0) {
    thisModem().sendAT(GF("+CGNSSINFO"));
    if (thisModem().waitResponse(GF(GSM_NL "+CGNSSINFO: ")) != 1) { return false; }

    uint8_t fixMode = thisModem().streamGetIntBefore(',');  // mode 2=2D Fix or 3=3DFix
                                                            // TODO(?) Can 1 be returned
    if (fixMode == 1 || fixMode == 2 || fixMode == 3) {
      // init variables
      float ilat = 0;
      char  north;
      float ilon = 0;
      char  east;
      float ispeed       = 0;
      float ialt         = 0;
      int   ivsat        = 0;
      int   iusat        = 0;
      float iaccuracy    = 0;
      int   iyear        = 0;
      int   imonth       = 0;
      int   iday         = 0;
      int   ihour        = 0;
      int   imin         = 0;
      float secondWithSS = 0;
      // 20240513 fixed
      // A7600M7_B11V05_231108
      // +CGNSSINFO:
      // 3,13,14,,,xx.xxxx,N,xx.xxxx,E,130524,035736.00,53.6,0.000,,1.7,1.2,1.1,
      ivsat =
          thisModem().streamGetIntBefore(',');  // GPS-SVs      satellite valid numbers
      thisModem().streamSkipUntil(',');         // BEIDOU-SVs   satellite valid numbers
      thisModem().streamSkipUntil(',');         // GLONASS-SVs  satellite valid numbers
      thisModem().streamSkipUntil(',');         // GALILEO-SVs  satellite valid numbers
      ilat  = thisModem().streamGetFloatBefore(',');  // Latitude in ddmm.mmmmmm
      north = stream.read();                          // N/S Indicator, N=north or S=south
      thisModem().streamSkipUntil(',');
      ilon = thisModem().streamGetFloatBefore(',');  // Longitude in ddmm.mmmmmm
      east = stream.read();                          // E/W Indicator, E=east or W=west
      thisModem().streamSkipUntil(',');

      // Date. Output format is ddmmyy
      iday   = thisModem().streamGetIntLength(2);    // Two digit day
      imonth = thisModem().streamGetIntLength(2);    // Two digit month
      iyear  = thisModem().streamGetIntBefore(',');  // Two digit year

      // UTC Time. Output format is hhmmss.s
      ihour = thisModem().streamGetIntLength(2);  // Two digit hour
      imin  = thisModem().streamGetIntLength(2);  // Two digit minute
      secondWithSS =
          thisModem().streamGetFloatBefore(',');  // 4 digit second with subseconds

      ialt = thisModem().streamGetFloatBefore(',');  // MSL Altitude. Unit is meters
      ispeed =
          thisModem().streamGetFloatBefore(',');  // Speed Over Ground. Unit is knots.
      thisModem().streamSkipUntil(',');           // Course Over Ground. Degrees.
      thisModem().streamSkipUntil(',');  // After set, will report GPS every x seconds
      iaccuracy =
          thisModem().streamGetFloatBefore(',');  // Position Dilution Of Precision
      thisModem().streamSkipUntil(',');           // Horizontal Dilution Of Precision
      thisModem().streamSkipUntil(',');           // Vertical Dilution Of Precision
      thisModem().streamSkipUntil('\n');          // TODO(?) is one more field reported??
      if (status) { *status = fixMode; }
      // Set pointers
      if (lat != NULL) { *lat = (ilat) * (north == 'N' ? 1 : -1); }
      if (lon != NULL) { *lon = (ilon) * (east == 'E' ? 1 : -1); }
      if (speed != NULL) *speed = ispeed;
      if (alt != NULL) *alt = ialt;
      if (vsat != NULL) *vsat = ivsat;
      if (usat != NULL) *usat = iusat;
      if (accuracy != NULL) *accuracy = iaccuracy;
      if (iyear < 2000) { iyear += 2000; }
      if (year != NULL) *year = iyear;
      if (month != NULL) *month = imonth;
      if (day != NULL) *day = iday;
      if (hour != NULL) *hour = ihour;
      if (minute != NULL) *minute = imin;
      if (second != NULL) *second = static_cast<int>(secondWithSS);

      thisModem().waitResponse();
      // Sometimes, although fix is displayed,
      // the value of longitude and latitude 0 will be set as invalid
      if (ilat == 0 || ilon == 0) { return false; }
      return true;
    }
    thisModem().waitResponse();
    return false;
  }

  /*
   * Time functions
   */
  // Can follow CCLK as per template
  int NTPServerSyncImpl(const String& server = "pool.ntp.org", byte TimeZone = 3) {
    // Set NTP server and timezone
    thisModem().sendAT(GF("+CNTP=\""), server, "\",", String(TimeZone));
    if (thisModem().waitResponse(10000L) != 1) { return -1; }

    // Request network synchronization
    thisModem().sendAT(GF("+CNTP"));
    if (thisModem().waitResponse(10000L, GF("+CNTP:"))) {
      return '0' == thisModem().stream.read();
    } else {
      return -1;
    }
    return -1;
  }

  bool getNetworkTimeImpl(int* year, int* month, int* day, int* hour, int* minute,
                          int* second, float* timezone) {
    thisModem().sendAT(GF("+CCLK?"));
    if (thisModem().waitResponse(2000L, GF("+CCLK: \"")) != 1) { return false; }

    int iyear     = 0;
    int imonth    = 0;
    int iday      = 0;
    int ihour     = 0;
    int imin      = 0;
    int isec      = 0;
    int itimezone = 0;

    // Date & Time
    iyear       = thisModem().streamGetIntBefore('/');
    imonth      = thisModem().streamGetIntBefore('/');
    iday        = thisModem().streamGetIntBefore(',');
    ihour       = thisModem().streamGetIntBefore(':');
    imin        = thisModem().streamGetIntBefore(':');
    isec        = thisModem().streamGetIntLength(2);
    char tzSign = thisModem().stream.read();
    itimezone   = thisModem().streamGetIntBefore('\n');
    if (tzSign == '-') { itimezone = itimezone * -1; }

    // Set pointers
    if (iyear < 2000) iyear += 2000;
    if (year != NULL) *year = iyear;
    if (month != NULL) *month = imonth;
    if (day != NULL) *day = iday;
    if (hour != NULL) *hour = ihour;
    if (minute != NULL) *minute = imin;
    if (second != NULL) *second = isec;
    if (timezone != NULL) *timezone = static_cast<float>(itimezone);

    // Final OK
    thisModem().waitResponse();
    return true;
  }

  /*
   * NTP server functions
   */
  // Can sync with server using CNTP as per template

  /*
   * Battery functions
   */
  // returns volts, multiply by 1000 to get mV
  uint16_t getBattVoltageImpl() {
    thisModem().sendAT(GF("+CBC"));
    if (thisModem().waitResponse(GF(GSM_NL "+CBC:")) != 1) { return 0; }

    // get voltage in VOLTS
    float voltage = thisModem().streamGetFloatBefore('\n');
    // Wait for final OK
    thisModem().waitResponse();
    // Return millivolts
    uint16_t res = voltage * 1000;
    return res;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  uint8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(uint8_t& chargeState, int8_t& percent, uint16_t& milliVolts) {
    chargeState = 0;
    percent     = 0;
    milliVolts  = getBattVoltageImpl();
    return true;
  }

 protected:
  // Follows all battery functions per template

  /*
   * Client related functions
   */
  // should implement in sub-classes

  /*
   * Temperature functions
   */
 protected:
  // get temperature in degree celsius
  uint16_t getTemperatureImpl() {
    thisModem().sendAT(GF("+CPMUTEMP"));
    if (thisModem().waitResponse(GF(GSM_NL "+CPMUTEMP:")) != 1) { return 0; }
    // return temperature in C
    uint16_t res = thisModem().streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  /*
   * Text to speech functions
   */
 protected:
  bool textToSpeechImpl(String& text, uint8_t mode) {
    thisModem().sendAT(GF("+CTTS="), mode, ',', '"', text, '"');
    if (thisModem().waitResponse() != 1) { return false; }
    if (thisModem().waitResponse(10000UL, GF("+CTTS: 0")) != 1) { return false; }
    return true;
  }

  /*
   * Utilities
   */
 public:
  // should implement in sub-classes
  int8_t waitResponse(uint32_t timeout_ms, String& data, GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    return thisModem().waitResponse(timeout_ms, data, r1, r2, r3, r4, r5);
  }

  int8_t waitResponse(uint32_t timeout_ms, GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    String data;
    return thisModem().waitResponse(timeout_ms, data, r1, r2, r3, r4, r5);
  }

  int8_t waitResponse(GsmConstStr r1 = GFP(GSM_OK), GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    return thisModem().waitResponse(1000, r1, r2, r3, r4, r5);
  }

 public:
  Stream& stream;

 protected:
  const char* gsmNL = GSM_NL;
};

#endif  // SRC_TINYGSMCLIENTSIM70XX_H_
