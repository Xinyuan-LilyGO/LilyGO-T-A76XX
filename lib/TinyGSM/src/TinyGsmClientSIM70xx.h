/**
 * @file       TinyGsmClientSIM70xx.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTSIM70XX_H_
#define SRC_TINYGSMCLIENTSIM70XX_H_

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
#include "TinyGsmGPS_EX.tpp"
#include "TinyGsmHttpsSIM7xxx.h"
#include "TinyGsmMqttSIM7xxx.h"
#include "TinyGsmSSL.tpp"

#define TINY_GSM_MODEM_HAS_NETWORK_MODE
#define TINY_GSM_MODEM_HAS_PREFERRED_MODE

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
};

enum NetworkMode {
  MODEM_NETWORK_AUTO    = 2,
  MODEM_NETWORK_GSM     = 13,
  MODEM_NETWORK_LTE     = 38,
  MODEM_NETWORK_GSM_LTE = 51,
  MODEM_NETWORK_UNKNOWN  = 255,
};

enum NetworkPreferred {
  MODEM_PREFERRED_CATM       = 1,
  MODEM_PREFERRED_NB_IOT      = 2,
  MODEM_PREFERRED_CATM_NBIOT = 3,
  MODEM_PREFERRED_UNKNOWN = 255,
};

enum GPSWorkMode {
  GNSS_MODE_GLONASS  = _BV(0),
  GNSS_MODE_BDS   = _BV(1),
  GNSS_MODE_GALILEAN = _BV(2),
  GNSS_MODE_ALL = 0xF
};

enum GNSS_OutputPort{
  NMEA_OUTPUT_DISABLE = 0,
  NMEA_TO_USB_NMEA_PORT = 1,
  NMEA_TO_UART3_PORT = 2,
};

template <class modemType,ModemPlatform model>
class TinyGsmSim70xx : public TinyGsmModem<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmGPRS<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmSMS<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmGPS<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmTime<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmNTP<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmBattery<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmGSMLocation<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmGPSEx<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmHttpsSIM7xxx<TinyGsmSim70xx<modemType,model>,model>,
                       public TinyGsmMqttSIM7xxx<TinyGsmSim70xx<modemType,model>>,
                       public TinyGsmSSL<TinyGsmSim70xx<modemType,model>> {
  friend class TinyGsmModem<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmGPRS<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmSMS<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmGPS<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmTime<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmNTP<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmBattery<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmGSMLocation<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmGPSEx<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmHttpsSIM7xxx<TinyGsmSim70xx<modemType,model>,model>;
  friend class TinyGsmMqttSIM7xxx<TinyGsmSim70xx<modemType,model>>;
  friend class TinyGsmSSL<TinyGsmSim70xx<modemType,model>>;
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
  explicit TinyGsmSim70xx(Stream& stream) : stream(stream) {}

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

    thisModem().sendAT("+SIMCOMATI");
    if (thisModem().waitResponse(10000L, res) != 1) {
      DBG("MODEM STRING NO FOUND!");
      return name;
    }

  /*
  * String simple
  * 
  * AT+SIMCOMATI
  * 
  * Revision:1951B16SIM7080
  * CSUB:B16V01
  * APRev:1951B16SIM7080,B16V01
  * QCN:SIM7080G_P1.03_20210823
  * IMEI:xxxxxxxxxxxxxx
  * 
  * Revision:1529B10SIM7000G 
  * CSUB:V02
  * APRev:1529B10SIM7000,V02
  * QCN:MDM9206_TX3.0.SIM7000G_P1.03C_20240911
  * IMEI:xxxxxxxxxxxxxx
  * 
  * * */

    String prefix = "SIM70";
    int index = 0;
    while (index < res.length()) {
      int startPos = res.indexOf(prefix, index);
      if (startPos == -1) {
        break; 
      }
      int endPos = res.indexOf('_', startPos);
      if (endPos == -1) {
        endPos = res.indexOf('\n', startPos);
        if (endPos == -1) {
          endPos = res.length();
        }
      }
      String modelString = res.substring(startPos, endPos);
      if (modelString.length() > prefix.length()) {
        index = modelString.indexOf('\n');
        if(index != -1){
          name = modelString.substring(0, index - 1);
        }else{
          name = modelString;
        }
        break;
      }
      index = endPos + 1;
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
    thisModem().sendAT(GF("E0"));  // Echo Off
    thisModem().waitResponse();
    if (!thisModem().setPhoneFunctionality(0)) { return false; }
    if (!thisModem().setPhoneFunctionality(1, true)) { return false; }
    thisModem().waitResponse(30000L, GF("SMS Ready"));
    return thisModem().initImpl(pin);
  }

  bool powerOffImpl() {
    thisModem().sendAT(GF("+CPOWD=1"));
    return thisModem().waitResponse(GF("NORMAL POWER DOWN")) == 1;
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
  NetworkMode getNetworkMode() {
    thisModem().sendAT(GF("+CNMP?"));
    if (thisModem().waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return MODEM_NETWORK_UNKNOWN; }
    int16_t mode = thisModem().streamGetIntBefore('\n');
    thisModem().waitResponse();
    return static_cast<NetworkMode>(mode);
  }

  bool setNetworkMode(NetworkMode mode) {
    thisModem().sendAT(GF("+CNMP="), mode);
    return thisModem().waitResponse() == 1;
  }

  bool setPreferredMode(NetworkPreferred mode) {
    thisModem().sendAT(GF("+CMNB="), mode);
    return thisModem().waitResponse() == 1;
  }

  NetworkPreferred getPreferredMode() {
    thisModem().sendAT(GF("+CMNB?"));
    if (thisModem().waitResponse(GF(GSM_NL "+CMNB:")) != 1) { return MODEM_PREFERRED_UNKNOWN; }
    int16_t mode = thisModem().streamGetIntBefore('\n');
    thisModem().waitResponse();
    return static_cast<NetworkPreferred>(mode);
  }

  String getNetworkModeString() {
    NetworkMode mode = getNetworkMode();
    switch (mode)
    {
    case MODEM_NETWORK_AUTO: return "Automatic";
    case MODEM_NETWORK_GSM: return "GSM";
    case MODEM_NETWORK_LTE: return "LTE";
    case MODEM_NETWORK_GSM_LTE: return "GSM and LTE";
    default:
      break;
    }
    return "UNKNOWN";
  }

  String getPreferredModeString() {
    NetworkPreferred mode = getPreferredMode();
    switch (mode)
    {
    case MODEM_PREFERRED_CATM: return "CAT-M";
    case MODEM_PREFERRED_NB_IOT: return "NB-IOT";
    case MODEM_PREFERRED_CATM_NBIOT: return "CAT-M and NB-IOT";
    default:
      break;
    }
    return "UNKNOWN";
  }

 public:
  bool getNetworkSystemMode(bool& n, int16_t& stat) {
    // n: whether to automatically report the system mode info
    // stat: the current service. 0 if it not connected
    thisModem().sendAT(GF("+CNSMOD?"));
    if (thisModem().waitResponse(GF(GSM_NL "+CNSMOD:")) != 1) { return false; }
    n    = thisModem().streamGetIntBefore(',') != 0;
    stat = thisModem().streamGetIntBefore('\n');
    thisModem().waitResponse();
    return true;
  }

  bool setNetworkSystemMode(bool n) {
    // n: whether to automatically report the system mode info
    thisModem().sendAT(GF("+CNSMOD="), int8_t(n));
    return thisModem().waitResponse() == 1;
  }

  String getLocalIPImpl() {
    return thisModem().getLocalIPImpl();
  }

  bool setNetworkDeactivate() {
    return thisModem().setNetworkDeactivateImpl();
  }

  bool setNetworkActive() {
    return thisModem().setNetworkActiveImpl();
  }

  bool getNetworkActive() {
    return thisModem().getNetworkActiveImpl();
  }

  bool setNetworkAPN(String apn) {
    thisModem().sendAT("+CGDCONT=1,\"IP\",", apn, "\"");
    return waitResponse() == 1;
  }

  String getNetworkAPN() {
    thisModem().sendAT("+CGNAPN");
    if (waitResponse(GF(GSM_NL "+CGNAPN:")) != 1) { return ""; }
    thisModem().streamSkipUntil('\"');
    String res = thisModem().stream.readStringUntil('\"');
    waitResponse();
    return res;
  }

  /*
  * <URL> String type :Address of the remote host
  * <count> The number of Ping Echo Requset to send, range: 1~500
  * <size> Number of data bytes to send, range: 1~1400
  * <timeout> Ping request timeout value (in ms),range:0-60000
  * <replyId> Echo Reply number
  * <IP Address> IP Address of the remote host
  * <replyTime> Time, in ms, required to receive the response
  */
  int ping(const char* url, String& resolved_ip_addr, uint32_t& rep_data_packet_size,
           uint32_t& tripTime, uint8_t& TTL) {
            
    uint8_t  dest_addr_type   = 1;
    uint8_t  num_pings        = 1;
    uint8_t  data_packet_size = 64;
    uint32_t interval_time    = 1000;
    uint32_t wait_time        = 10000;
    uint8_t  ttl              = 0xFF;

    thisModem().sendAT("+SNPING4=\"", url, "\"", ",", num_pings, ",", data_packet_size, ",",interval_time);

    if (thisModem().waitResponse(10000UL, "+SNPING4: ") == 1) {
      thisModem().streamGetIntBefore(',');  //replyId
      resolved_ip_addr     = stream.readStringUntil(','); //IP address
      tripTime             = thisModem().streamGetIntBefore('\r'); //replyTime
      if (thisModem().waitResponse() != 1) { return -1; }
      rep_data_packet_size = data_packet_size;
      return 1;
    }
    return -1;
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
  // Doesn't return the "+CCID" before the number
  String getSimCCIDImpl() {
    thisModem().sendAT(GF("+CCID"));
    if (thisModem().waitResponse(GF(GSM_NL)) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }
  /*
   * FS functions
   */
 protected:
  bool modemFsInit() {
    thisModem().sendAT("+CFSINIT");
    return thisModem().waitResponse(3000UL) == 1;
  }

  bool moodemFsEnd() {
    thisModem().sendAT("+CFSTERM");
    return thisModem().waitResponse(3000UL) == 1;
  }

  /*
   * SSL functions
   */
  bool downloadCertificateImpl(String filename, const char* buffer) {
    if (!filename.endsWith(".pem") && !filename.endsWith(".crt")) {
      DBG("The file name must have type like \".pem\" or \".crt\". ");
      return false;
    }
    int filename_length = filename.length() - 4;
    if (filename_length < 5 || filename_length > 50) {
      DBG("File name length should less or equal 50 characters.");
      return false;
    }

    modemFsInit();

    thisModem().sendAT("+CFSWFILE=3,", "\"", filename, "\",", "0,", strlen(buffer), ",10000");
    if (thisModem().waitResponse(10000UL, "DOWNLOAD") == 1) {
      stream.write(buffer);
    } else {
      moodemFsEnd();
      return false;
    }

    if (thisModem().waitResponse(3000UL) != 1) {
      DBG("Download certificate failed !");
      moodemFsEnd();
      return false;
    }
    return moodemFsEnd();
  }

  bool deleteCertificateImpl(const char* filename) {
    modemFsInit();

    thisModem().sendAT("+CFSDFILE=3,\"", filename, "\"");
    if (thisModem().waitResponse(3000UL) != 1) {
      DBG("Delete certificate failed !");
      moodemFsEnd();
      return false;
    }

    return moodemFsEnd();
  }

  bool sslConfigVersionImpl(uint8_t ctxindex, uint8_t ssl_version) {
    thisModem().sendAT("+CSSLCFG=\"sslversion\",", ctxindex, ',', ssl_version);
    return (thisModem().waitResponse(5000UL) == 1);
  }

  bool sslConfigSniImpl(uint8_t ctxindex, const char* server_name) {
    thisModem().sendAT("+CSSLCFG=\"sni\",", ctxindex, ",\"", server_name, '"');
    return (thisModem().waitResponse(5000UL) == 1);
  }

  // <ssltype>
  // 1 QAPI_NET_SSL_CERTIFICATE_E
  // 2 QAPI_NET_SSL_CA_LIST_E
  // 3 QAPI_NET_SSL_PSK_TABLE_E
  bool convertCertificateImpl(uint8_t ssl_type, const char* cert_filename,
                              const char* private_key_filename = NULL) {
    if (cert_filename && private_key_filename) {
      thisModem().sendAT("+CSSLCFG=convert,", ssl_type, ",", cert_filename, ",",
             private_key_filename);
    } else if (cert_filename) {
      thisModem().sendAT("+CSSLCFG=convert,", ssl_type, ",", cert_filename);
    }
    return (thisModem().waitResponse(5000UL) == 1);
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
  bool enableGPSImpl(int8_t power_en_pin ,uint8_t enable_level) {
    if(power_en_pin != -1){
      if(model == QUALCOMM_SIM7080G){
        thisModem().sendAT("+SGPIO=0,",power_en_pin,",1,",enable_level);
      }else{
        thisModem().sendAT("+CGPIO=0,",power_en_pin,",1,",enable_level);
      }
      thisModem().waitResponse();
    } 
    thisModem().sendAT(GF("+CGNSPWR=1"));
    if (thisModem().waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl(int8_t power_en_pin ,uint8_t disable_level) {
    if(power_en_pin != -1){
      if(model == QUALCOMM_SIM7080G){
        thisModem().sendAT("+SGPIO=0,",power_en_pin,",1,",disable_level);
      }else{
        thisModem().sendAT("+CGPIO=0,",power_en_pin,",1,",disable_level);
      }
      thisModem().waitResponse();
    } 
    thisModem().sendAT(GF("+CGNSPWR=0"));
    if (thisModem().waitResponse() != 1) { return false; }
    return true;
  }

  bool isEnableGPSImpl() {
    thisModem().sendAT(GF("+CGNSPWR?"));
    if (thisModem().waitResponse(GF(GSM_NL "+CGNSPWR:")) != 1) { return false; }
    bool running = 1 == thisModem().streamGetIntBefore('\r');
    thisModem().waitResponse();
    return running;
  }

  bool setGPSBaudImpl(uint32_t baud) {
    thisModem().sendAT("+CGNSUTIPR=",baud);
    return thisModem().waitResponse(1000UL) == 1;
  }

  bool setGPSModeImpl(uint8_t mode) {
    thisModem().sendAT("+CGNSMOD=1,", 
                        (mode & GNSS_MODE_GLONASS)  ? "1" : "0", ',',
                        (mode & GNSS_MODE_BDS)   ? "1" : "0", ',',
                        (mode & GNSS_MODE_GALILEAN) ? "1" : "0");
    return waitResponse(1000L) == 1;
  }

  bool setGPSOutputRateImpl(uint8_t rate_hz) {
    if (rate_hz <= 0) {
        return false; 
    }
    double periodMs = 1000.0 / rate_hz;
    int roundedMs = (int)round(periodMs);
    if (roundedMs < 50) {
        periodMs = 50;
    } else if (roundedMs > 1000) {
        periodMs = 1000;
    } 
    thisModem().sendAT("+CGNSRTMS=",periodMs);
    return thisModem().waitResponse(1000UL) == 1;
  }

  bool enableNMEAImpl(bool outputAtPort) {
    // 7070G 2019.11.07 Delete commands,see datasheet  Version History
    thisModem().sendAT("+CGNSPORT=3");
    thisModem().waitResponse(1000UL);

    thisModem().sendAT("+CGNSMOD=1,1,1,1");
    if (thisModem().waitResponse(1000UL) != 1) { return false; }

    // 7070G 2019.11.07 Delete commands,see datasheet  Version History
    thisModem().sendAT("+CGNSCFG=2");
    thisModem().waitResponse(1000UL);

    thisModem().sendAT("+CGNSTST=1");
    if (thisModem().waitResponse(1000UL) != 1) { return false; }

    thisModem().sendAT("+CGNSPWR=1");
    if (thisModem().waitResponse(1000UL) != 1) { return false; }
    return true;
  }

  bool disableNMEAImpl() {
    thisModem().sendAT("+CGNSTST=0");
    return thisModem().waitResponse(1000L) == 1;
  }

  bool configNMEASentenceImpl(uint32_t nmea_mask){
    if(model == QUALCOMM_SIM7080G){
      thisModem().sendAT("+SGNSCFG=\"NMEATYPE\",",nmea_mask);
    }else{
      thisModem().sendAT("+CGNSNMEA=",nmea_mask);
    }
    return thisModem().waitResponse(1000L) == 1;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    thisModem().sendAT(GF("+CGNSINF"));
    if (thisModem().waitResponse(10000L, GF(GSM_NL "+CGNSINF:")) != 1) {
      return "";
    }    
    String res = stream.readStringUntil('\n');
    thisModem().waitResponse();
    res.trim();
    return res;
  }

  bool gpsColdStartImpl() {
    thisModem().sendAT(GF("+CGNSCOLD"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsWarmStartImpl() {
    thisModem().sendAT(GF("+CGNSWARM"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsHotStartImpl() {
    thisModem().sendAT(GF("+CGNSHOT"));
    if (thisModem().waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool getGPS_ExImpl(GPSInfo& info) {
    float lat = 0;
    float lon = 0;
    memset(&info, 0, sizeof(info));
    // +CGNSINF:
    // <GNSS run status>,<Fix status>,<UTC date & Time>,<Latitude>,<Longitude>,<MSL
    // Altitude>,<Speed Over Ground>, <Course Over Ground>,<Fix
    // Mode>,<Reserved1>,<HDOP>,<PDOP>,<VDOP>,<Reserved2>,<GNSS Satellites in View>,
    // <GNSS Satellites Used>,<GLONASS Satellites Used>,<Reserved3>,<C/N0 max>,<HPA>,<VPA>
    thisModem().sendAT(GF("+CGNSINF"));
    if (thisModem().waitResponse(GF(GSM_NL "+CGNSINF: ")) != 1) { return false; }

    // <GNSS run status>
    bool running = thisModem().streamGetIntBefore(',');
    if (!running) {
      log_e("GPS NOT START");
      return false;
    }
    // <Fix status>
    info.isFix = thisModem().streamGetIntBefore(',');
    if (info.isFix == 1) {
      // UTC date & Time
      info.year   = thisModem().streamGetIntLength(4);  // Four digit year
      info.month  = thisModem().streamGetIntLength(2);  // Two digit month
      info.day    = thisModem().streamGetIntLength(2);  // Two digit day
      info.hour   = thisModem().streamGetIntLength(2);  // Two digit hour
      info.minute = thisModem().streamGetIntLength(2);  // Two digit minute
      float secondWithSS =
          thisModem().streamGetFloatBefore(',');  // 6 digit second with subseconds
      info.second = static_cast<int>(secondWithSS);

      info.latitude  = thisModem().streamGetFloatBefore(',');  // Latitude
      info.longitude = thisModem().streamGetFloatBefore(',');  // Longitude
      info.altitude =
          thisModem().streamGetFloatBefore(',');  // MSL Altitude. Unit is meters
      info.speed =
          thisModem().streamGetFloatBefore(',');  // Speed Over Ground. Unit is knots.
      info.course = thisModem().streamSkipUntil(',');  // Course Over Ground. Degrees.
      thisModem().streamSkipUntil(',');                // Fix Mode
      thisModem().streamSkipUntil(',');                // Reserved1
      info.HDOP =
          thisModem().streamGetFloatBefore(',');       // Horizontal Dilution Of Precision
      info.PDOP = thisModem().streamSkipUntil(',');    // Position Dilution Of Precision
      info.VDOP = thisModem().streamSkipUntil(',');    // Vertical Dilution Of Precision
      thisModem().streamSkipUntil(',');                // Reserved2
      info.GSV = thisModem().streamGetIntBefore(',');  // GNSS Satellites in View
      info.GSU = thisModem().streamGetIntBefore(',');  // GNSS Satellites Used
      info.glonass_satellite_num =
          thisModem().streamSkipUntil(',');  // GLONASS Satellites Used
      thisModem().streamSkipUntil(',');      // Reserved3
      thisModem().streamSkipUntil(',');      // C/N0 max
      thisModem().streamSkipUntil(',');      // HPA
      thisModem().streamSkipUntil('\n');     // VPA
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
    thisModem().sendAT(GF("+CGNSINF"));
    if (thisModem().waitResponse(10000L, GF(GSM_NL "+CGNSINF:")) != 1) { return false; }
    *status = thisModem().streamGetIntLength(1);
    thisModem().streamSkipUntil(',');                // GNSS run status
    if (thisModem().streamGetIntBefore(',') == 1) {  // fix status
      // init variables
      float ilat         = 0;
      float ilon         = 0;
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

      // UTC date & Time
      iyear  = thisModem().streamGetIntLength(4);  // Four digit year
      imonth = thisModem().streamGetIntLength(2);  // Two digit month
      iday   = thisModem().streamGetIntLength(2);  // Two digit day
      ihour  = thisModem().streamGetIntLength(2);  // Two digit hour
      imin   = thisModem().streamGetIntLength(2);  // Two digit minute
      secondWithSS =
          thisModem().streamGetFloatBefore(',');  // 6 digit second with subseconds

      ilat = thisModem().streamGetFloatBefore(',');  // Latitude
      ilon = thisModem().streamGetFloatBefore(',');  // Longitude
      ialt = thisModem().streamGetFloatBefore(',');  // MSL Altitude. Unit is meters
      ispeed =
          thisModem().streamGetFloatBefore(',');  // Speed Over Ground. Unit is knots.
      thisModem().streamSkipUntil(',');           // Course Over Ground. Degrees.
      thisModem().streamSkipUntil(',');           // Fix Mode
      thisModem().streamSkipUntil(',');           // Reserved1
      iaccuracy =
          thisModem().streamGetFloatBefore(',');    // Horizontal Dilution Of Precision
      thisModem().streamSkipUntil(',');             // Position Dilution Of Precision
      thisModem().streamSkipUntil(',');             // Vertical Dilution Of Precision
      thisModem().streamSkipUntil(',');             // Reserved2
      ivsat = thisModem().streamGetIntBefore(',');  // GNSS Satellites in View
      iusat = thisModem().streamGetIntBefore(',');  // GNSS Satellites Used
      thisModem().streamSkipUntil(',');             // GLONASS Satellites Used
      thisModem().streamSkipUntil(',');             // Reserved3
      thisModem().streamSkipUntil(',');             // C/N0 max
      thisModem().streamSkipUntil(',');             // HPA
      thisModem().streamSkipUntil('\n');            // VPA

      // Set pointers
      if (lat != NULL) *lat = ilat;
      if (lon != NULL) *lon = ilon;
      if (speed != NULL) *speed = ispeed;
      if (alt != NULL) *alt = ialt;
      if (vsat != NULL) *vsat = ivsat;
      if (usat != NULL) *usat = iusat;
      if (accuracy != NULL) *accuracy = iaccuracy;
      if (iyear < 2000) iyear += 2000;
      if (year != NULL) *year = iyear;
      if (month != NULL) *month = imonth;
      if (day != NULL) *day = iday;
      if (hour != NULL) *hour = ihour;
      if (minute != NULL) *minute = imin;
      if (second != NULL) *second = static_cast<int>(secondWithSS);

      thisModem().waitResponse();
      return true;
    }

    thisModem().streamSkipUntil('\n');  // toss the row of commas
    thisModem().waitResponse();
    return false;
  }
public:

  bool configGNSS_OutputPort(GNSS_OutputPort port) {
    thisModem().sendAT("+CGNSPWR=1");
    if(thisModem().waitResponse() != 1){
      return false;
    }
    if(model == QUALCOMM_SIM7080G){
      thisModem().sendAT("+SGNSCFG=\"NMEAOUTPORT\",",port);
    }else{
      thisModem().sendAT("+CGNSCFG=",port);
    }
    if (thisModem().waitResponse() != 1) {
       return false; 
    }
    return true;
  }
  /*
   * Time functions
   */
  // Can follow CCLK as per template

  /*
   * NTP server functions
   */
  // Can sync with server using CNTP as per template

  /*
   * Battery functions
   */
 protected:
  // Follows all battery functions per template

  /*
   * Client related functions
   */
  // should implement in sub-classes

  /*
   * Utilities
   */
 public:
  // should implement in sub-classes
  int8_t waitResponse(uint32_t timeout_ms, String& data, 
                      GsmConstStr r1 = GFP(GSM_OK),
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

  int8_t waitResponse(GsmConstStr r1 = GFP(GSM_OK), 
                      GsmConstStr r2 = GFP(GSM_ERROR),
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
