/**
 * @file       TinyGsmClientSIM7600.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTSIM7600_H_
#define SRC_TINYGSMCLIENTSIM7600_H_

// #define TINY_GSM_DEBUG Serial
// #define TINY_GSM_USE_HEX

#define TINY_GSM_MUX_COUNT 10
#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE
#define TINY_GSM_MODEM_HAS_NETWORK_MODE

#include "TinyGsmBattery.tpp"
#include "TinyGsmCalling.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmModem.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmTemperature.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"
#include "TinyGsmGPS_EX.tpp"
#include "TinyGsmMqttA76xx.h"
#include "TinyGsmHttpsComm.h"
#include "TinyGsmTextToSpeech.tpp"
#include "TinyGsmFSComm.tpp"

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
  MODEM_NETWORK_AUTO                     = 2,    
  MODEM_NETWORK_GSM                      = 13,   
  MODEM_NETWORK_WCDMA                    = 14,   
  MODEM_NETWORK_LTE                      = 38,   
  MODEM_NETWORK_TDSCDMA                  = 59,   
  MODEM_NETWORK_CDMA                     = 9,    
  MODEM_NETWORK_EVDO                     = 10,   
  MODEM_NETWORK_GSM_WCDMA                = 19,   
  MODEM_NETWORK_CDMA_EVDO                = 22,   
  MODEM_NETWORK_ANY_BUT_LTE              = 48,   
  MODEM_NETWORK_GSM_TDSCDMA              = 60,   
  MODEM_NETWORK_GSM_WCDMA_TDSCDMA        = 63,   
  MODEM_NETWORK_CDMA_EVDO_GSM_WCDMA_TDSCDMA = 67,
  MODEM_NETWORK_GSM_WCDMA_LTE            = 39,   
  MODEM_NETWORK_GSM_LTE                  = 51,   
  MODEM_NETWORK_WCDMA_LTE                = 54,   
  MODEM_NETWORK_UNKNOWN                  = 255   
};

enum SIM7600X_GPSMode {
    GNSS_MODE_GPS = 0,                                // 0b0000  GPS
    GNSS_MODE_GPS_GLONASS = 1 << 0,                   // 0b0001 GPS + GLONASS
    GNSS_MODE_GPS_BDS = 1 << 1,                       // 0b0010 GPS + BDS
    GNSS_MODE_GPS_GLONASS_BDS = (1 << 0) | (1 << 1), // 0b0011 GPS + GLONASS + BDS
    GNSS_MODE_GPS_GALILEO = 1 << 2,                  // 0b0100 ，GPS + GALILEO
    GNSS_MODE_GPS_GLONASS_GALILEO = (1 << 0) | (1 << 2), // 0b0101 ，GPS + GLONASS + GALILEO
    GNSS_MODE_GPS_BDS_GALILEO = (1 << 1) | (1 << 2),  // 0b0110 ，GPS + BDS + GALILEO
    GNSS_MODE_GPS_GLONASS_BDS_GALILEO = (1 << 0) | (1 << 1) | (1 << 2), // 0b0111 ，GPS + GLONASS + BDS + GALILEO
    GNSS_MODE_GPS_QZSS = 1 << 3,               // 0b1000 ，GPS + QZSS
    GNSS_MODE_GPS_GLONASS_QZSS = (1 << 0) | (1 << 3),    // 0b1001 ，GPS + GLONASS + QZSS 
    GNSS_MODE_GPS_BDS_QZSS = (1 << 1) | (1 << 3),     // 0b1010 ，GPS + BDS + QZSS 
    GNSS_MODE_GPS_GLONASS_BDS_QZSS = (1 << 0) | (1 << 1) | (1 << 3), // 0b1011 ，GPS + GLONASS + BDS + QZSS 
    GNSS_MODE_GPS_GALILEO_QZSS = (1 << 2) | (1 << 3),    // 0b1100 ，GPS + GALILEO + QZSS 
    GNSS_MODE_GPS_GLONASS_GALILEO_QZSS = (1 << 0) | (1 << 2) | (1 << 3), // 0b1101 ，GPS + GLONASS + GALILEO + QZSS 
    GNSS_MODE_GPS_BDS_GALILEO_QZSS = (1 << 1) | (1 << 2) | (1 << 3),  // 0b1110 ，GPS + BDS + GALILEO + QZSS 
    GNSS_MODE_ALL = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) // 0b1111 ，GPS + all other supported systems
};

enum NMEA_Sentences {
    NMEA_GPGGA   = _BV(0),    // Bit 0: GPGGA (global positioning system fix data)
    NMEA_GPRMC   = _BV(1),    // Bit 1: GPRMC (recommended minimum specific GPS/TRANSIT data)
    NMEA_GPGSV   = _BV(2),    // Bit 2: GPGSV (GPS satellites in view)
    NMEA_GPGSA   = _BV(3),    // Bit 3: GPGSA (GPS DOP and active satellites)
    NMEA_GPVTG   = _BV(4),    // Bit 4: GPVTG (track made good and ground speed)
    NMEA_PQXFI   = _BV(5),    // Bit 5: PQXFI (Global Positioning System Extended Fix Data)
    NMEA_GLGSV   = _BV(6),    // Bit 6: GLGSV (GLONASS satellites in view GLONASS fixes only)
    NMEA_GNGSA   = _BV(7),    // Bit 7: GNGSA (1. GPS/2. Glonass/3. GALILE DOP and Active Satellites)
    NMEA_GNGNS   = _BV(8),    // Bit 8: GNGNS (fix data for GNSS receivers;output for GPS,GLONASS,GALILEO)
    // Bit 9: reserve
    NMEA_GAGSV   = _BV(10),   // Bit 10: GAGSV (GALILEO satellites in view)
    // Bit 11: reserve
    // Bit 12: reserve
    // Bit 13: reserve
    // Bit 14: reserve
    // Bit 15: reserve
    NMEA_BDGSA_PQGSA = _BV(16),  // Bit 16: BDGSA/PQGSA (BEIDOU/QZSS DOP and active satellites)
    NMEA_BDGSV_PQGSV = _BV(17)   // Bit 17: BDGSV/PQGSV (BEIDOUQZSS satellites in view)
};

class TinyGsmSim7600 : public TinyGsmModem<TinyGsmSim7600>,
                       public TinyGsmGPRS<TinyGsmSim7600>,
                       public TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>,
                       public TinyGsmSMS<TinyGsmSim7600>,
                       public TinyGsmGSMLocation<TinyGsmSim7600>,
                       public TinyGsmGPS<TinyGsmSim7600>,
                       public TinyGsmTime<TinyGsmSim7600>,
                       public TinyGsmNTP<TinyGsmSim7600>,
                       public TinyGsmBattery<TinyGsmSim7600>,
                       public TinyGsmTemperature<TinyGsmSim7600>,
                       public TinyGsmCalling<TinyGsmSim7600>,
                       public TinyGsmGPSEx<TinyGsmSim7600>,
                       public TinyGsmTextToSpeech<TinyGsmSim7600>,
                       public TinyGsmMqttA76xx<TinyGsmSim7600, TINY_GSM_MQTT_CLI_COUNT>,
                       public TinyGsmHttpsComm<TinyGsmSim7600, QUALCOMM_SIM7600G>,
                       public TinyGsmFSComm<TinyGsmSim7600, QUALCOMM_SIM7600G> {
  friend class TinyGsmModem<TinyGsmSim7600>;
  friend class TinyGsmGPRS<TinyGsmSim7600>;
  friend class TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSMS<TinyGsmSim7600>;
  friend class TinyGsmGPS<TinyGsmSim7600>;
  friend class TinyGsmGSMLocation<TinyGsmSim7600>;
  friend class TinyGsmTime<TinyGsmSim7600>;
  friend class TinyGsmNTP<TinyGsmSim7600>;
  friend class TinyGsmBattery<TinyGsmSim7600>;
  friend class TinyGsmTemperature<TinyGsmSim7600>;
  friend class TinyGsmCalling<TinyGsmSim7600>;
  friend class TinyGsmMqttA76xx<TinyGsmSim7600, TINY_GSM_MQTT_CLI_COUNT>;
  friend class TinyGsmHttpsComm<TinyGsmSim7600, QUALCOMM_SIM7600G>;
  friend class TinyGsmGPSEx<TinyGsmSim7600>;
  friend class TinyGsmTextToSpeech<TinyGsmSim7600>;
  friend class TinyGsmFSComm<TinyGsmSim7600, QUALCOMM_SIM7600G>;

  /*
   * Inner Client
   */
 public:
  class GsmClientSim7600 : public GsmClient {
    friend class TinyGsmSim7600;

   public:
    GsmClientSim7600() {}

    explicit GsmClientSim7600(TinyGsmSim7600& modem, uint8_t mux = 0) {
      init(&modem, mux);
    }

    bool init(TinyGsmSim7600* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;

      if (mux < TINY_GSM_MUX_COUNT) {
        this->mux = mux;
      } else {
        this->mux = (mux % TINY_GSM_MUX_COUNT);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, false, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+CIPCLOSE="), mux);
      sock_connected = false;
      at->waitResponse();
    }
    void stop() override {
      stop(15000L);
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Inner Secure Client
   */

  /*TODO(?))
  class GsmClientSecureSIM7600 : public GsmClientSim7600
  {
  public:
    GsmClientSecure() {}

    GsmClientSecure(TinyGsmSim7600& modem, uint8_t mux = 0)
     : public GsmClient(modem, mux)
    {}

  public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, true, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES
  };
  */

  /*
   * Constructor
   */
 public:
  explicit TinyGsmSim7600(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSIM7600"));

    if (!testAT()) { return false; }

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Disable time and time zone URC's
    sendAT(GF("+CTZR=0"));
    if (waitResponse(10000L) != 1) { return false; }

    // Enable automatic time zome update
    sendAT(GF("+CTZU=1"));
    if (waitResponse(10000L) != 1) { return false; }

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != NULL && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  String getModemNameImpl() {
    String name = "UNKNOWN";
    String res;

    sendAT(GF("E0"));  // Echo Off
    waitResponse();

    sendAT("I");
    if (waitResponse(10000L, res) != 1) {
      DBG("MODEM STRING NO FOUND!");
      return name;
    }
    int modelIndex    = res.indexOf("Model:") + 6;
    int nextLineIndex = res.indexOf('\n', modelIndex);
    if (nextLineIndex != -1) {
      String modelString = res.substring(modelIndex, nextLineIndex);
      int    start       = modelString.indexOf("SIM7600");
      if (start != -1) { name = modelString.substring(start); }
    } else {
      DBG("Model string not found.");
    }
    return name;
  }

  bool factoryDefaultImpl() {  // these commands aren't supported
    return false;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = NULL) {
    if (!testAT()) { return false; }
    sendAT(GF("+CRESET"));
    if (waitResponse(10000L) != 1) { return false; }
    delay(5000L);  // TODO(?):  Test this delay!
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPOF"));
    return waitResponse() == 1;
  }

  bool radioOffImpl() {
    if (!setPhoneFunctionality(4)) { return false; }
    delay(3000);
    return true;
  }

  bool sleepEnableImpl(bool enable = true) {
    sendAT(GF("+CSCLK="), enable);
    return waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  RegStatus getRegistrationStatus() {
    return (RegStatus)getRegistrationStatusXREG("CGREG");
  }

 protected:
  bool isNetworkConnectedImpl() {
    RegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

 public:

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

    sendAT("+CPING=\"", url, "\"", ",", dest_addr_type, ",", num_pings, ",",
                       data_packet_size, ",", interval_time, ",", wait_time, ",", ttl);

    if (waitResponse() != 1) { return -1; }
    if (waitResponse(10000UL, "+CPING: ") == 1) {
      result_type = streamGetIntBefore(',');
      switch (result_type) {
        case 1:
          resolved_ip_addr     = stream.readStringUntil(',');
          rep_data_packet_size = streamGetIntBefore(',');
          tripTime             = streamGetIntBefore(',');
          TTL                  = streamGetIntBefore('\n');
          break;
        case 2: break;
        case 3: break;
        default: break;
      }
    }
    waitResponse(GSM_NL);
    return result_type;
  }

  String getNetworkModeString() {
      int16_t mode = getNetworkMode();
      switch (mode) {
          case MODEM_NETWORK_AUTO: 
              return "AUTO";
          case MODEM_NETWORK_GSM: 
              return "GSM Only";
          case MODEM_NETWORK_WCDMA: 
              return "WCDMA Only";
          case MODEM_NETWORK_LTE: 
              return "LTE Only";
          case MODEM_NETWORK_TDSCDMA: 
              return "TDS-CDMA Only";
          case MODEM_NETWORK_CDMA: 
              return "CDMA Only";
          case MODEM_NETWORK_EVDO: 
              return "EVDO Only";
          case MODEM_NETWORK_GSM_WCDMA: 
              return "GSM+WCDMA Only";
          case MODEM_NETWORK_CDMA_EVDO: 
              return "CDMA+EVDO Only";
          case MODEM_NETWORK_ANY_BUT_LTE: 
              return "Any but LTE";
          case MODEM_NETWORK_GSM_TDSCDMA: 
              return "GSM+TDSCDMA Only";
          case MODEM_NETWORK_GSM_WCDMA_TDSCDMA: 
              return "GSM+WCDMA+TDSCDMA Only";
          case MODEM_NETWORK_CDMA_EVDO_GSM_WCDMA_TDSCDMA: 
              return "CDMA+EVDO+GSM+WCDMA+TDSCDMA Only";
          case MODEM_NETWORK_GSM_WCDMA_LTE: 
              return "GSM+WCDMA+LTE Only";
          case MODEM_NETWORK_GSM_LTE: 
              return "GSM+LTE Only";
          case MODEM_NETWORK_WCDMA_LTE: 
              return "WCDMA+LTE Only";
          case MODEM_NETWORK_UNKNOWN: 
              return "UNKNOWN";
          default: 
              return "UNKNOWN";
      }
  }

  NetworkMode getNetworkMode() {
    sendAT(GF("+CNMP?"));
    if (waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return MODEM_NETWORK_UNKNOWN; }
    int16_t mode = streamGetIntBefore('\n');
    waitResponse();
    return static_cast<NetworkMode>(mode);
  }

  bool setNetworkMode(NetworkMode mode) {
    sendAT(GF("+CNMP="), mode);
    return waitResponse() == 1;
  }

  String getLocalIPImpl() {
    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    String ipAddress = "NOT OPEN NETWORK";
    int    res       = waitResponse(GF("+IPADDR: "), GF("+IP ERROR: Network not opened"));
    if (res == 1) {
      ipAddress = stream.readStringUntil('\n');
    } else if (res == 2) {
      waitResponse();
    }
    return ipAddress;
  }

  bool setNetworkActive() {
    sendAT(GF("+NETOPEN"));
    int res = waitResponse(GF("+NETOPEN: 0"), GF("+IP ERROR: Network is already opened"));
    if (res == 2) {
      // DBG("> Network has open......");
      waitResponse();
    }
    if (res != 1 && res != 2) { return false; }
    return true;
  }

  bool setNetworkDeactivate() {
    sendAT(GF("+NETCLOSE"));
    if (waitResponse() != 1) { return false; }
    int res = waitResponse(GF("+NETCLOSE: 0"), GF("+NETCLOSE: 2"));
    if (res != 1 && res != 2) { return false; }
    return true;
  }

  bool getNetworkActive() {
    sendAT(GF("+NETOPEN?"));
    int res = waitResponse(GF("+NETOPEN: 1"));
    if (res == 1) { return true; }
    return false;
  }

  String getNetworkAPN() {
    sendAT("+CGDCONT?");
    if (waitResponse(GF(GSM_NL "+CGDCONT: ")) != 1) { return "ERROR"; }
    streamSkipUntil(',');
    streamSkipUntil(',');
    streamSkipUntil('\"');
    String res = stream.readStringUntil('\"');
    waitResponse();
    if (res == "") { res = "APN IS NOT SET"; }
    return res;
  }

  bool setNetworkAPN(String apn) {
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, "\"");
    return waitResponse() == 1;
  }

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user = NULL, const char* pwd = NULL) {
    gprsDisconnect();  // Make sure we're not connected first

    // Define the PDP context

    // The CGDCONT commands set up the "external" PDP context

    // Set the external authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=1,0,\""), user, GF("\",\""), pwd, '"');
      waitResponse();
    }

    // Define external PDP context 1
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"', ",\"0.0.0.0\",0,0");
    waitResponse();

    // Configure TCP parameters

    // Select TCP/IP application mode (command mode)
    sendAT(GF("+CIPMODE=0"));
    waitResponse();

    // Set Sending Mode - send without waiting for peer TCP ACK
    sendAT(GF("+CIPSENDMODE=0"));
    waitResponse();

    // Configure socket parameters
    // AT+CIPCCFG= <NmRetry>, <DelayTm>, <Ack>, <errMode>, <HeaderType>,
    //            <AsyncMode>, <TimeoutVal>
    // NmRetry = number of retransmission to be made for an IP packet
    //         = 10 (default)
    // DelayTm = number of milliseconds to delay before outputting received data
    //          = 0 (default)
    // Ack = sets whether reporting a string "Send ok" = 0 (don't report)
    // errMode = mode of reporting error result code = 0 (numberic values)
    // HeaderType = which data header of receiving data in multi-client mode
    //            = 1 (+RECEIVE,<link num>,<data length>)
    // AsyncMode = sets mode of executing commands
    //           = 0 (synchronous command executing)
    // TimeoutVal = minimum retransmission timeout in milliseconds = 75000
    sendAT(GF("+CIPCCFG=10,0,0,0,1,0,75000"));
    if (waitResponse() != 1) { return false; }

    // Configure timeouts for opening and closing sockets
    // AT+CIPTIMEOUT=<netopen_timeout> <cipopen_timeout>, <cipsend_timeout>
    sendAT(GF("+CIPTIMEOUT="), 75000, ',', 15000, ',', 15000);
    waitResponse();

    // Start the socket service

    // This activates and attaches to the external PDP context that is tied
    // to the embedded context for TCP/IP (ie AT+CGACT=1,1 and AT+CGATT=1)
    // Response may be an immediate "OK" followed later by "+NETOPEN: 0".
    // We to ignore any immediate response and wait for the
    // URC to show it's really connected.
    sendAT(GF("+NETOPEN"));
    if (waitResponse(75000L, GF(GSM_NL "+NETOPEN: 0")) != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Close all sockets and stop the socket service
    // Note: On the LTE models, this single command closes all sockets and the
    // service
    sendAT(GF("+NETCLOSE"));
    if (waitResponse(60000L, GF(GSM_NL "+NETCLOSE: 0")) != 1) { return false; }

    return true;
  }

  bool isGprsConnectedImpl() {
    sendAT(GF("+NETOPEN?"));
    // May return +NETOPEN: 1, 0.  We just confirm that the first number is 1
    if (waitResponse(GF(GSM_NL "+NETOPEN: 1")) != 1) { return false; }
    waitResponse();

    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    // sendAT(GF("+CGPADDR=1")); // Show PDP address
    if (waitResponse() != 1) { return false; }

    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  // Gets the CCID of a sim card via AT+CCID
  String getSimCCIDImpl() {
    sendAT(GF("+CICCID"));
    if (waitResponse(GF(GSM_NL "+ICCID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 protected:
  bool callHangupImpl() {
    sendAT(GF("+CHUP"));
    return waitResponse() == 1;
  }

  /*
   * Messaging functions
   */
 protected:
  // Follows all messaging functions per template

  /*
   * GSM Location functions
   */
 protected:
  // Can return a GSM-based location from CLBS as per the template

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl(int8_t power_en_pin, uint8_t enable_level) {
    if (power_en_pin == GSM_MODEM_AUX_POWER) {
      sendAT("+CVAUXV=2800");
      waitResponse();
      sendAT("+CVAUXS=1");
      waitResponse();
    } else if (power_en_pin != -1) {
      sendAT("+CGDRT=", power_en_pin, ",1");
      waitResponse();
      sendAT("+CGSETV=", power_en_pin, ",", enable_level);
      waitResponse();
    }
    if (isEnableGPSImpl()) { return true; }
    sendAT(GF("+CGPS=1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl(int8_t power_en_pin, uint8_t disable_level) {
    bool isEnabled = isEnableGPSImpl();
    if (power_en_pin == GSM_MODEM_AUX_POWER) {
      sendAT("+CVAUXS=0");
      waitResponse();
    } else if (power_en_pin != -1) {
      sendAT("+CGSETV=", power_en_pin, ",", disable_level);
      waitResponse();
      sendAT("+CGDRT=", power_en_pin, ",0");
      waitResponse();
    }
    sendAT(GF("+CGPS=0"));
    if (waitResponse() != 1) { return false; }
    if (isEnabled) {
      return waitResponse(60000UL, GF("+CGPS: 0")) == 1;
    } else {
      return true;
    }
  }

  bool isEnableGPSImpl() {
    sendAT(GF("+CGPS?"));
    if (waitResponse(60000UL, "+CGPS:") != 1) { return false; }
    bool running = 1 == streamGetIntBefore(',');
    waitResponse();
    return running;
  }

  bool enableAGPSImpl() {
    // TODO:
    return false;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(GSM_NL "+CGNSSINFO:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  bool gpsColdStartImpl() {
    sendAT(GF("+CGPSCOLD"));
    if (waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsWarmStartImpl() {
    sendAT(GF("+CGPSWARM"));
    if (waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool gpsHotStartImpl() {
    sendAT(GF("+CGPSHOT"));
    if (waitResponse(10000L) != 1) { return false; }
    return true;
  }

  bool getGPS_ExImpl(GPSInfo& info) {
    float lat = 0;
    float lon = 0;
    // +CGNSSINFO:[<mode>],
    // [<GPS-SVs>],[BEIDOU-SVs],[<GLONASS-SVs>],[<GALILEO-SVs>],
    // [<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[<speed>],[<course>],[<PDOP>],[HDOP],[VDOP]
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(GSM_NL "+CGNSSINFO: ")) != 1) { return false; }

    info.isFix = streamGetIntBefore(',');  // mode 2=2D Fix or 3=3DFix
    if (info.isFix == 2 || info.isFix == 3) {
      int16_t ret = -9999;
      // GPS-SVs      satellite valid numbers
      ret                    = streamGetIntBefore(',');
      info.gps_satellite_num = ret != -9999 ? ret : 0;
      // BEIDOU-SVs   satellite valid numbers
      ret                       = streamGetIntBefore(',');
      info.beidou_satellite_num = ret != -9999 ? ret : 0;
      // GLONASS-SVs  satellite valid numbers
      ret                        = streamGetIntBefore(',');
      info.glonass_satellite_num = ret != -9999 ? ret : 0;
      // Latitude in ddmm.mmmmmm
      lat = streamGetFloatBefore(',');
      // N/S Indicator, N=north or S=south
      info.NS_indicator = stream.read();
      streamSkipUntil(',');
      // Longitude in ddmm.mmmmmm
      lon = streamGetFloatBefore(',');
      // E/W Indicator, E=east or W=west
      info.EW_indicator = stream.read();
      streamSkipUntil(',');
      // Date. Output format is ddmmyy
      // Two digit day
      info.day = streamGetIntLength(2);
      // Two digit month
      info.month = streamGetIntLength(2);
      // Two digit year
      info.year = streamGetIntBefore(',');
      // UTC Time. Output format is hhmmss.s
      // Two digit hour
      info.hour = streamGetIntLength(2);
      // Two digit minute
      info.minute = streamGetIntLength(2);
      // 4 digit second with subseconds
      float secondWithSS = streamGetFloatBefore(',');
      info.second        = static_cast<int>(secondWithSS);
      // MSL Altitude. Unit is meters
      info.altitude = streamGetFloatBefore(',');
      // Speed Over Ground. Unit is knots.
      info.speed = streamGetFloatBefore(',');
      // Course Over Ground. Degrees.
      info.course = streamSkipUntil(',');
      // After set, will report GPS every x seconds
      streamSkipUntil(',');
      // Position Dilution Of Precision
      float pdop = streamGetFloatBefore(',');
      info.PDOP  = pdop != -9999.0F ? pdop : 0;
      // Horizontal Dilution Of Precision
      float hdop = streamGetFloatBefore(',');
      info.HDOP  = hdop != -9999.0F ? hdop : 0;
      // Vertical Dilution Of Precision
      float vdop = streamGetFloatBefore(',');
      info.VDOP  = vdop != -9999.0F ? vdop : 0;
      streamSkipUntil('\n');
      waitResponse();
      info.latitude = (floor(lat / 100) + fmod(lat, 100.) / 60) *
          (info.NS_indicator == 'N' ? 1 : -1);
      info.longitude = (floor(lon / 100) + fmod(lon, 100.) / 60) *
          (info.EW_indicator == 'E' ? 1 : -1);


      if (info.year < 2000) { info.year += 2000; }
      return true;
    }

    waitResponse();
    return false;
  }


  // get GPS informations
  bool getGPSImpl(uint8_t* status, float* lat, float* lon, float* speed = 0,
                  float* alt = 0, int* vsat = 0, int* usat = 0, float* accuracy = 0,
                  int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                  int* minute = 0, int* second = 0) {
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(GSM_NL "+CGNSSINFO:")) != 1) { return false; }

    uint8_t fixMode = streamGetIntBefore(',');  // mode 2=2D Fix or 3=3DFix
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

      streamSkipUntil(',');               // GPS satellite valid numbers
      streamSkipUntil(',');               // GLONASS satellite valid numbers
      streamSkipUntil(',');               // BEIDOU satellite valid numbers
      ilat  = streamGetFloatBefore(',');  // Latitude in ddmm.mmmmmm
      north = stream.read();              // N/S Indicator, N=north or S=south
      streamSkipUntil(',');
      ilon = streamGetFloatBefore(',');  // Longitude in ddmm.mmmmmm
      east = stream.read();              // E/W Indicator, E=east or W=west
      streamSkipUntil(',');

      // Date. Output format is ddmmyy
      iday   = streamGetIntLength(2);    // Two digit day
      imonth = streamGetIntLength(2);    // Two digit month
      iyear  = streamGetIntBefore(',');  // Two digit year

      // UTC Time. Output format is hhmmss.s
      ihour        = streamGetIntLength(2);      // Two digit hour
      imin         = streamGetIntLength(2);      // Two digit minute
      secondWithSS = streamGetFloatBefore(',');  // 4 digit second with subseconds

      ialt   = streamGetFloatBefore(',');  // MSL Altitude. Unit is meters
      ispeed = streamGetFloatBefore(',');  // Speed Over Ground. Unit is knots.
      streamSkipUntil(',');                // Course Over Ground. Degrees.
      streamSkipUntil(',');                // After set, will report GPS every x seconds
      iaccuracy = streamGetFloatBefore(',');  // Position Dilution Of Precision
      streamSkipUntil(',');                   // Horizontal Dilution Of Precision
      streamSkipUntil(',');                   // Vertical Dilution Of Precision
      streamSkipUntil('\n');                  // TODO(?) is one more field reported??

      if (status) { *status = fixMode; }
      // Set pointers
      if (lat != NULL)
        *lat = (floor(ilat / 100) + fmod(ilat, 100.) / 60) * (north == 'N' ? 1 : -1);
      if (lon != NULL)
        *lon = (floor(ilon / 100) + fmod(ilon, 100.) / 60) * (east == 'E' ? 1 : -1);
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

      waitResponse();
      return true;
    }
    waitResponse();
    return false;
  }

  bool setGPSBaudImpl(uint32_t baud) {
    DBG("Modem does not support set GPS baudrate.");
    return true;
  }

  // mode: See SIM7600X_GPSMode enum
  bool setGPSModeImpl(uint8_t mode) {
    // Set gnss mode must turn off gps model
    disableGPSImpl(-1, -1);
    sendAT("+CGNSSMODE=", mode, ",1");
    bool rlst = waitResponse(1000UL) == 1;
    enableGPSImpl(-1, -1);
    log_d("set gpio mode : %d\n", rlst);
    return rlst;
  }

  // 1 = 1HZ , other = 10HZ
  bool setGPSOutputRateImpl(uint8_t rate_hz) {
    sendAT("+CGPSNMEARATE=", rate_hz == 1 ? 1 : 10);
    return waitResponse(1000UL) == 1;
  }

  bool enableNMEAImpl(bool outputAtPort) {
    // sendAT("+CGPS=0");
    // waitResponse(1000UL);
    // waitResponse(30000UL,"+CGPS: 0");
    // sendAT("+CGNSSMODE=15,1");
    // waitResponse(1000UL);
    // sendAT("+CGPSNMEA=200191");
    // waitResponse(1000UL);
    // sendAT("+CGPSNMEARATE=1");
    // waitResponse(1000UL);
    sendAT("+CGPS=1");
    waitResponse(1000UL);
    sendAT("+CGPSINFOCFG=1,31");
    return waitResponse(1000UL) == 1;
  }

  bool disableNMEAImpl() {
    sendAT("+CGPSINFOCFG=0,31");
    return waitResponse(1000UL) == 1;
  }

  bool configNMEASentenceImpl(uint32_t nmea_mask) {
    sendAT("+CGPSNMEA=",nmea_mask);
    return waitResponse(1000L) == 1;
  }
  /*
   * Time functions
   */
 protected:
  // Can follow the standard CCLK function in the template

  /*
   * NTP server functions
   */
  // Can sync with server using CNTP as per template

  /*
   * Battery functions
   */
 protected:
  // returns volts, multiply by 1000 to get mV
  uint16_t getBattVoltageImpl() {
    sendAT(GF("+CBC"));
    if (waitResponse(GF(GSM_NL "+CBC:")) != 1) { return 0; }

    // get voltage in VOLTS
    float voltage = streamGetFloatBefore('\n');
    // Wait for final OK
    waitResponse();
    // Return millivolts
    uint16_t res = voltage * 1000;
    return res;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  uint8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(uint8_t& chargeState, int8_t& percent, uint16_t& milliVolts) {
    chargeState = 0;
    percent     = 0;
    milliVolts  = getBattVoltage();
    return true;
  }

  /*
   * Temperature functions
   */
 protected:
  // get temperature in degree celsius
  uint16_t getTemperatureImpl() {
    sendAT(GF("+CPMUTEMP"));
    if (waitResponse(GF(GSM_NL "+CPMUTEMP:")) != 1) { return 0; }
    // return temperature in C
    uint16_t res = streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  /*
   * Text to speech functions
   */
 protected:
  bool textToSpeechImpl(String& text, uint8_t mode) {
    sendAT(GF("+CTTS="), mode, ',', '"', text, '"');
    if (waitResponse() != 1) { return false; }
    if (waitResponse(10000UL, GF("+CTTS: 0")) != 1) { return false; }
    return true;
  }

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t mux, bool ssl = false,
                    int timeout_s = 15) {
    if (ssl) { DBG("SSL not yet supported on this module!"); }
    // Make sure we'll be getting data manually on this connection
    sendAT(GF("+CIPRXGET=1"));
    if (waitResponse() != 1) { return false; }

    // Establish a connection in multi-socket mode
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    sendAT(GF("+CIPOPEN="), mux, ',', GF("\"TCP"), GF("\",\""), host, GF("\","), port);
    // The reply is OK followed by +CIPOPEN: <link_num>,<err> where <link_num>
    // is the mux number and <err> should be 0 if there's no error
    if (waitResponse(timeout_ms, GF(GSM_NL "+CIPOPEN:")) != 1) { return false; }
    uint8_t opened_mux    = streamGetIntBefore(',');
    uint8_t opened_result = streamGetIntBefore('\n');
    if (opened_mux != mux || opened_result != 0) return false;
    return true;
  }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    if (waitResponse(GF(">")) != 1) { return 0; }
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();
    if (waitResponse(GF(GSM_NL "+CIPSEND:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    streamSkipUntil(',');  // Skip requested bytes to send
    // TODO(?):  make sure requested and confirmed bytes match
    return streamGetIntBefore('\n');
  }

  size_t modemRead(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
#ifdef TINY_GSM_USE_HEX
    sendAT(GF("+CIPRXGET=3,"), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
#else
    sendAT(GF("+CIPRXGET=2,"), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
#endif
    streamSkipUntil(',');  // Skip Rx mode 2/normal or 3/HEX
    streamSkipUntil(',');  // Skip mux/cid (connecion id)
    int16_t len_requested = streamGetIntBefore(',');
    //  ^^ Requested number of data bytes (1-1460 bytes)to be read
    int16_t len_confirmed = streamGetIntBefore('\n');
    // ^^ The data length which not read in the buffer
    for (int i = 0; i < len_requested; i++) {
      uint32_t startMillis = millis();
#ifdef TINY_GSM_USE_HEX
      while (stream.available() < 2 &&
             (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char buf[4] = {
          0,
      };
      buf[0] = stream.read();
      buf[1] = stream.read();
      char c = strtol(buf, NULL, 16);
#else
      while (!stream.available() && (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char c = stream.read();
#endif
      sockets[mux]->rx.put(c);
    }
    // DBG("### READ:", len_requested, "from", mux);
    // sockets[mux]->sock_available = modemGetAvailable(mux);
    sockets[mux]->sock_available = len_confirmed;
    waitResponse();
    return len_requested;
  }

  size_t modemGetAvailable(uint8_t mux) {
    if (!sockets[mux]) return 0;
    sendAT(GF("+CIPRXGET=4,"), mux);
    size_t result = 0;
    if (waitResponse(GF("+CIPRXGET:")) == 1) {
      streamSkipUntil(',');  // Skip mode 4
      streamSkipUntil(',');  // Skip mux
      result = streamGetIntBefore('\n');
      waitResponse();
    }
    // DBG("### Available:", result, "on", mux);
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnected(uint8_t mux) {
    // Read the status of all sockets at once
    sendAT(GF("+CIPCLOSE?"));
    if (waitResponse(GF("+CIPCLOSE:")) != 1) {
      // return false;  // TODO:  Why does this not read correctly?
    }
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      // +CIPCLOSE:<link0_state>,<link1_state>,...,<link9_state>
      bool muxState = stream.parseInt();
      if (sockets[muxNo]) { sockets[muxNo]->sock_connected = muxState; }
    }
    waitResponse();  // Should be an OK at the end
    if (!sockets[mux]) return false;
    return sockets[mux]->sock_connected;
  }

  /*
   * Utilities
   */
 public:
  // TODO(vshymanskyy): Optimize this!
  int8_t waitResponse(uint32_t timeout_ms, String& data, GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    /*String r1s(r1); r1s.trim();
    String r2s(r2); r2s.trim();
    String r3s(r3); r3s.trim();
    String r4s(r4); r4s.trim();
    String r5s(r5); r5s.trim();
    DBG("### ..:", r1s, ",", r2s, ",", r3s, ",", r4s, ",", r5s);*/
    data.reserve(64);
    uint8_t  index       = 0;
    uint32_t startMillis = millis();
    do {
      TINY_GSM_YIELD();
      while (stream.available() > 0) {
        TINY_GSM_YIELD();
        int8_t a = stream.read();
        // putchar(a);
        if (a <= 0) continue;  // Skip 0x00 bytes, just in case
        data += static_cast<char>(a);
        if (r1 && data.endsWith(r1)) {
          index = 1;
          goto finish;
        } else if (r2 && data.endsWith(r2)) {
          index = 2;
          goto finish;
        } else if (r3 && data.endsWith(r3)) {
#if defined TINY_GSM_DEBUG
          if (r3 == GFP(GSM_CME_ERROR)) {
            streamSkipUntil('\n');  // Read out the error
          }
#endif
          index = 3;
          goto finish;
        } else if (r4 && data.endsWith(r4)) {
          index = 4;
          goto finish;
        } else if (r5 && data.endsWith(r5)) {
          index = 5;
          goto finish;
        } else if (data.endsWith(GF(GSM_NL "+CIPRXGET:"))) {
          int8_t mode = streamGetIntBefore(',');
          if (mode == 1) {
            int8_t mux = streamGetIntBefore('\n');
            if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
              sockets[mux]->got_data = true;
            }
            data = "";
            // DBG("### Got Data:", mux);
          } else {
            data += mode;
          }
        } else if (data.endsWith(GF(GSM_NL "+RECEIVE:"))) {
          int8_t  mux = streamGetIntBefore(',');
          int16_t len = streamGetIntBefore('\n');
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->got_data = true;
            if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
          }
          data = "";
          // DBG("### Got Data:", len, "on", mux);
        } else if (data.endsWith(GF("+IPCLOSE:"))) {
          int8_t mux = streamGetIntBefore(',');
          streamSkipUntil('\n');  // Skip the reason code
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->sock_connected = false;
          }
          data = "";
          DBG("### Closed: ", mux);
        } else if (data.endsWith(GF("+CIPEVENT:"))) {
          // Need to close all open sockets and release the network library.
          // User will then need to reconnect.
          DBG("### Network error!");
          if (!isGprsConnected()) { gprsDisconnect(); }
          data = "";
        }
      }
    } while (millis() - startMillis < timeout_ms);
  finish:
    if (!index) {
      data.trim();
      if (data.length()) { DBG("### Unhandled:", data); }
      data = "";
    }
    // data.replace(GSM_NL, "/");
    // DBG('<', index, '>', data);
    return index;
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
    return waitResponse(timeout_ms, data, r1, r2, r3, r4, r5);
  }

  int8_t waitResponse(GsmConstStr r1 = GFP(GSM_OK), GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    return waitResponse(1000, r1, r2, r3, r4, r5);
  }

 public:
  Stream& stream;

 protected:
  GsmClientSim7600* sockets[TINY_GSM_MUX_COUNT];
  const char*       gsmNL = GSM_NL;
};

#endif  // SRC_TINYGSMCLIENTSIM7600_H_
