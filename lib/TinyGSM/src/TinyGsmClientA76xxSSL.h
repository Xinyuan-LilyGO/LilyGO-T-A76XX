/**
 * @file      TinyGsmClientA76xxSSL.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-12-05
 *
 */


#ifndef SRC_TINYGSMCLIENTA76XXSSL_H_
#define SRC_TINYGSMCLIENTA76XXSSL_H_

// #define TINY_GSM_DEBUG Serial
// #define TINY_GSM_USE_HEX

#define TINY_GSM_MUX_COUNT 2
#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE

#include "TinyGsmClientA76xx.h"
#include "TinyGsmTCP.tpp"
#include "TinyGsmSSL.tpp"
#include "TinyGsmMqttA76xx.h"
#include "TinyGsmHttpsA76xx.h"

// Websocket callback function
typedef void (*websocket_cb_t)(const uint8_t* buffer, size_t length);


class TinyGsmA76xxSSL : public TinyGsmA76xx<TinyGsmA76xxSSL>,
                        public TinyGsmTCP<TinyGsmA76xxSSL, TINY_GSM_MUX_COUNT>,
                        public TinyGsmSSL<TinyGsmA76xxSSL>,
                        public TinyGsmMqttA76xx<TinyGsmA76xxSSL, TINY_GSM_MQTT_CLI_COUNT>,
                        public TinyGsmHttpsA76xx<TinyGsmA76xxSSL> {
  friend class TinyGsmA76xx<TinyGsmA76xxSSL>;
  friend class TinyGsmTCP<TinyGsmA76xxSSL, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSSL<TinyGsmA76xxSSL>;
  friend class TinyGsmMqttA76xx<TinyGsmA76xxSSL, TINY_GSM_MQTT_CLI_COUNT>;
  friend class TinyGsmHttpsA76xx<TinyGsmA76xxSSL>;

  /*
   * Inner Client
   */
 public:
  class GsmClientA76xxSSL : public GsmClient {
    friend class TinyGsmA76xxSSL;

   public:
    GsmClientA76xxSSL() {}

    explicit GsmClientA76xxSSL(TinyGsmA76xxSSL& modem, uint8_t mux = 0) {
      init(&modem, mux);
    }

    bool init(TinyGsmA76xxSSL* modem, uint8_t mux = 0) {
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

      stop();

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

      at->modemDisconnect(mux);

      sock_connected = false;
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

  class GsmClientSecureA76xxSSL : public GsmClientA76xxSSL {
   public:
    GsmClientSecureA76xxSSL() {}

    explicit GsmClientSecureA76xxSSL(TinyGsmA76xxSSL& modem, uint8_t mux = 0)
        : GsmClientA76xxSSL(modem, mux) {}

   public:
    bool setCertificate(const String& certificateName) {
      return at->setCertificate(certificateName, mux);
    }

    bool setClientCertificate(const String& clientCertFile) {
      return at->setClientCertificate(clientCertFile, mux);
    }

    bool setClientPrivateKey(const String& clientKeyFile) {
      return at->setClientPrivateKey(clientKeyFile, mux);
    }

    virtual int connect(const char* host, uint16_t port, int timeout_s) override {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, true, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES
  };

  /*
   * Constructor
   */
 public:
  explicit TinyGsmA76xxSSL(Stream& stream)
      : TinyGsmA76xx<TinyGsmA76xxSSL>(stream),
        certificates(),
        client_certificate(),
        client_private_key(),
        client_private_key_password() {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientA76xxSSL"));

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

  void maintainImpl() {
    // Keep listening for modem URC's and proactively iterate through
    // sockets asking if any data is available
    bool check_socks = false;
    for (int mux = 0; mux < TINY_GSM_MUX_COUNT; mux++) {
      GsmClientA76xxSSL* sock = sockets[mux];
      if (sock && sock->got_data) {
        sock->got_data = false;
        check_socks    = true;
      }
    }
    // modemGetAvailable checks all socks, so we only want to do it once
    // modemGetAvailable calls modemGetConnected(), which also checks allf
    if (check_socks) { modemGetAvailable(0); }
    while (stream.available()) { waitResponse(15, NULL, NULL); }
  }

  /*
   * Power functions
   */
 protected:
  // Follows the A76xx template

  /*
   * Generic network functions
   */
 protected:
  /*
   * Secure socket layer functions
   */
 protected:
  bool setCertificate(const String& certificateName, const uint8_t mux = 0) {
    if (mux >= TINY_GSM_MUX_COUNT) return false;
    certificates[mux] = certificateName;
    return true;
  }

  bool setClientCertificate(const String& clientCertFile, const uint8_t mux = 0) {
    if (mux >= TINY_GSM_MUX_COUNT) return false;
    client_certificate[mux] = clientCertFile;
    return true;
  }

  bool setClientPrivateKey(const String& clientKeyFile, const uint8_t mux = 0) {
    if (mux >= TINY_GSM_MUX_COUNT) return false;
    client_private_key[mux] = clientKeyFile;
    return true;
  }

  bool setClientPrivateKeyPassword(const String& passwordFile, const uint8_t mux = 0) {
    if (mux >= TINY_GSM_MUX_COUNT) return false;
    client_private_key_password[mux] = passwordFile;
    return true;
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
    if (waitResponse() != 1) {
      // printf("[%u]Configure socket parameters\n ",__LINE__);
      return false;
    }

    // Configure timeouts for opening and closing sockets
    // AT+CIPTIMEOUT=<netopen_timeout> <cipopen_timeout>, <cipsend_timeout>
    sendAT(GF("+CIPTIMEOUT="), 75000, ',', 15000, ',', 15000);
    waitResponse();

    // PDP context activate or deactivate
    sendAT("+CGACT=1,1");
    if (waitResponse(30000UL) != 1) {
      // printf("[%u]PDP context activate or deactivate\n ",__LINE__);
      return false;
    }

    // This activates and attaches to the external PDP context that is tied
    // to the embedded context for TCP/IP (ie AT+CGACT=1,1 and AT+CGATT=1)
    // Response may be an immediate "OK" followed later by "+NETOPEN: 0".
    // We to ignore any immediate response and wait for the
    // URC to show it's really connected.
    sendAT(GF("+NETOPEN"));
    if (waitResponse(75000L, GF(GSM_NL "+NETOPEN: 0")) != 1) {
      // printf("[%u]NETOPEN\n ",__LINE__);
      return false;
    }

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
    return true;
  }

  /*
   * SIM card functions
   */
 protected:
  // Follows the A76xx template

  /*
   * Messaging functions
   */
 protected:
  // Follows all messaging functions per template

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // Follows the A76xx template

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
   * SSL functions
   */
 protected:
  bool downloadCertificateImpl(String filename, const char* buffer) {
    if (!filename.endsWith(".pem") && !filename.endsWith(".der")) {
      DBG("The file name must have type like \".pem\" or \".der\". ");
      return false;
    }
    int filename_length = filename.length() - 4;
    if (filename_length < 5 || filename_length > 108) {
      DBG("The length of filename is from 5 to 108 bytes.");
      return false;
    }
    sendAT("+CCERTDOWN=", "\"", filename, "\"", ",", strlen(buffer));
    if (waitResponse(10000UL, ">") == 1) {
      stream.write(buffer);
    } else {
      return false;
    }
    if (waitResponse(3000UL) != 1) {
      DBG("Download certificate failed !");
      return false;
    }
    return true;
  }

  bool deleteCertificateImpl(const char* filename) {
    sendAT("+CCERTDELE=\"", filename, "\"");
    if (waitResponse(3000UL) != 1) {
      DBG("Delete certificate failed !");
      return false;
    }
    return true;
  }

  /*
   * Client related functions
   */
 protected:
  bool sslConnect(const char* host, uint16_t port, uint8_t mux, bool ssl, int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;

    /*
    0 - The authentication mode, the default value is 0. no authentication.
    1 - server authentication. It needs the root CA of the server.
    2 - server and client authentication. It needs the root CA of the server, the cert
      and key of the client.(If the server does not need to
    3 - authenticate theclient ,it is equivalent to value 1.) client authentication and
    no server authentication. It needs the cert and key of the client.( (If the server
    does not need to authenticate the client ,it is equivalent to value 0.)
    */
    uint8_t authmode = 0;

    // AT+CSSLCFG=<ssl_ctx_index>
    // AT+CSSLCFG="ignorecertCN",<ssl_ctx_index>,<ignorecertCN_flag>
    // AT+CSSLCFG="negotiatetime",<ssl_ctx_index>,<negotiatetime>
    // AT+CSSLCFG="ignorelocaltime",<ssl_ctx_index>,<ignoreltime>

    if (ssl) {
      // set the ssl version
      // AT+CSSLCFG="SSLVERSION",<ctxindex>,<sslversion>
      // <sslversion> 0: SSL3.0
      //              1: TLS1.0
      //              2: TLS1.1
      //              3: TLS1.2
      //              4: ALL
      sendAT(GF("+CSSLCFG=\"sslversion\","), mux, GF(",4"));
      if (waitResponse(5000L) != 1) return false;

      // SNI is enabled by default
      // AT+CSSLCFG="enableSNI",<ssl_ctx_index>,<enableSNI_flag>
      sendAT(GF("+CSSLCFG=\"enableSNI\","), mux, ',', true);
      waitResponse(5000L);

      if (certificates[mux] != "") {
        // apply the correct certificate to the connection
        // AT+CSSLCFG="cacert",<ssl_ctx_index>,<ca_file>
        sendAT(GF("+CSSLCFG=\"cacert\","), mux, ',', GF("\""), certificates[mux].c_str(),
               GF("\""));
        if (waitResponse(5000L) != 1) return false;
        authmode = 1;
      }

      if (client_certificate[mux] != "") {
        // AT+CSSLCFG="clientcert",<ssl_ctx_index>,<clientcert_file>
        sendAT(GF("+CSSLCFG=\"clientcert\","), mux, ',', GF("\""),
               client_certificate[mux].c_str(), GF("\""));
        if (waitResponse(5000L) != 1) return false;
      }

      if (client_private_key[mux] != "") {
        // AT+CSSLCFG="clientkey",<ssl_ctx_index>,<clientkey_file>
        sendAT(GF("+CSSLCFG=\"clientkey\","), mux, ',', GF("\""),
               client_private_key[mux].c_str(), GF("\""));
        if (waitResponse(5000L) != 1) return false;
      }

      if (client_private_key_password[mux] != "") {
        // AT+CSSLCFG="password",<ssl_ctx_index>,<password_file>
        sendAT(GF("+CSSLCFG=\"password\","), mux, ',', GF("\""),
               client_private_key_password[mux].c_str(), GF("\""));
        if (waitResponse(5000L) != 1) return false;
      }

      if (client_certificate[mux] != "" && client_private_key[mux] != "") {
        authmode = 2;
      }
    }

    // Configure the authentication mode of the specified SSL context
    // AT+CSSLCFG="authmode",<ssl_ctx_index>,<authmode>
    sendAT(GF("+CSSLCFG=\"authmode\","), mux, ',', authmode);
    waitResponse();

    // Set report mode
    sendAT(GF("+CCHSET=1,1"));
    waitResponse();

    // Start SSL service, activate PDP context
    sendAT(GF("+CCHSTART"));
    waitResponse();

    // +CCHSSLCFG: <session_id>,[<ssl_ctx_index>]
    sendAT(GF("+CCHSSLCFG="), mux, ',', mux);
    waitResponse();

    // actually open the connection
    // AT+CCHOPEN=<session_id>,<host>,<port>[,<client_type>,[<bind_port>]]
    uint8_t client_type = ssl ? 2 : 1;
    if (port == 80) {
      client_type = 1;  // TCP
    } else if (port == 443) {
      client_type = 2;  // SSL
    }
    sendAT(GF("+CCHOPEN="), mux, GF(",\""), host, GF("\","), port, ',', client_type);
    if (waitResponse(timeout_ms, GF(GSM_NL "+CCHOPEN:")) != 1) { return 0; }
    // returns OK/r/n/r/n+CCHOPEN: <session_id>,<err>
    // <result> 0: Success
    //          1: Alerting state(reserved)
    //          2: Unknown error
    //          3: Busy
    //          4: Peer closed
    //          5: Operation timeout
    //          6: Transfer failed
    //          7: Memory error
    //          8: Invalid parameter
    //          9: Network error
    //          10: Open session error
    //          11: State error
    //          12: Create socket error
    //          13: Get DNS error
    //          14: Connect socket error
    //          15: Handshake error
    //          16: Close socket error
    //          17: Nonet
    //          18: Send data timeout
    //          19: Not set certificates
    streamSkipUntil(',');  // Skip mux
    // make sure the connection really opened
    int8_t res = streamGetIntBefore('\n');
    waitResponse();
    return 0 == res;
  }

  int16_t sslSend(const void* buff, size_t len, uint8_t mux) {
    // send data on prompt
    sendAT(GF("+CCHSEND="), mux, ',', (uint16_t)len);
    if (waitResponse(GF(">")) != 1) { return 0; }

    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();

    // after posting data, module responds with:
    //+CCHSEND: 0,0
    if (waitResponse(GF(GSM_NL "+CCHSEND:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    // if (streamGetIntBefore(',') != 0) { return 0; }  // If result != success
    int write_bytes = streamGetIntBefore('\n') == 0 ? len : write_bytes;
    return write_bytes;
  }

  size_t sslRead(size_t size, uint8_t mux) {
    if (!sockets[mux]) { return 0; }

    sendAT(GF("+CCHRECV="), mux, ',', (uint16_t)size);

    // +CCHRECV: DATA,0,430
    int8_t res = waitResponse(30000UL, GF("+CCHRECV: DATA,"), GF("ERROR"));
    if (res == 2) {
      delay(1000);
      return 0;
    }

    uint8_t       ret_mux       = streamGetIntBefore(',');
    const int16_t len_confirmed = streamGetIntBefore('\n');

    // DBG("### READING:", len_confirmed, "from", ret_mux);
    if (ret_mux != mux) {
      // DBG("### Data from wrong mux! Got", ret_mux, "expected", mux);
      waitResponse();
      sockets[mux]->sock_available = modemGetAvailable(mux);
      return 0;
    }

    for (int i = 0; i < len_confirmed; i++) {
      uint32_t startMillis = millis();
      while (!stream.available() && (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char c = stream.read();
      sockets[mux]->rx.put(c);
    }

    if (waitResponse("+CCHRECV:") == 1) {
      ret_mux = streamGetIntBefore(',');
      /*uint16_t remaining = */ streamGetIntBefore('\n');
    }

    // DBG("### READ:", len_confirmed, "from", mux);
    // make sure the sock available number is accurate again
    // the module is **EXTREMELY** testy about being asked to read more from
    // the buffer than exits; it will freeze until a hard reset or power cycle!
    sockets[mux]->sock_available = modemGetAvailable(mux);
    return len_confirmed;
  }

  size_t sslAvailable(uint8_t mux) {
    // If the socket doesn't exist, just return
    if (!sockets[mux]) { return 0; }
    // We need to check if there are any connections open *before* checking for
    // available characters.  The A76XX *will crash* if you ask about data
    // when there are no open connections.
    if (!modemGetConnected(mux)) { return 0; }

    // NOTE: This gets how many characters are available on all connections that
    // have data.  It does not return all the connections, just those with data.
    sendAT(GF("+CCHRECV?"));
    // +CCHRECV: LEN,2048,0
    int res = waitResponse(3000, GF("+CCHRECV: LEN,"), GFP(GSM_OK), GFP(GSM_ERROR));
    // if we get the +CCHRECV: response, read the mux number and the number of
    // characters available
    if (res == 1) {
      // +CCHRECV: LEN,2048,0
      size_t result  = streamGetIntBefore(',');
      int    ret_mux = streamGetIntBefore('\n');
      if (ret_mux == -9999) {
        // DBG("ERROR: mux = -9999");
        return 0;
      }
      GsmClientA76xxSSL* sock = sockets[ret_mux];
      waitResponse();
      // DBG("---- available:", result, "ret_mux", ret_mux);
      if (sock) { sock->sock_available = result; }
    }
    if (!sockets[mux]) { return 0; }
    // DBG("sockets[mux]->sock_available=", sockets[mux]->sock_available);
    return sockets[mux]->sock_available;
  }

  /*
   * The following versions are known to have bugs:
   * Model: A7670E-FASE
   * - A7670M7_B02V03_210922
   * - A7670M7_B07V01_240927
   * - A7670M7_B05V04_240122
   * Model: A7608SA-H
   * - A7600M7_B11V05_231108
   *
   * Other versions may also have the same problem. BUG analysis has been submitted to
   * SIMCOM 20241209
   */
  bool sslConnected(uint8_t mux) {
    sendAT(GF("+CCHOPEN?"));
    // +CCHOPEN: 0,<host>,<port>,<client_type>,<bind_port>
    // +CCHOPEN: 0,"httpbin.org",443,2,54021
    int res = waitResponse(3000, GF("+CCHOPEN:"), GFP(GSM_OK), GFP(GSM_ERROR));
    if (res == 1) {
      bool connected = false;
      // Response
      // AT+CCHOPEN?
      // +CCHOPEN: 0,"",,,
      // +CCHOPEN: 1,"",,,
      //
      // OK
      char buf[4] = {0};
      int  ret_mux;
      if (mux == 0) {
        ret_mux = streamGetIntBefore(',');
        streamGetLength(buf, 2);
        if (buf[0] == '"' && buf[1] != '"') { connected = true; }
        // Serial.printf("\n\n mux=%d [0]=%c 0x%x -- [1]=%c 0x%x Connected=%d\n", ret_mux,
        //               buf[0], buf[0], buf[1], buf[1], connected);
        waitResponse("\n");
        if (mux == ret_mux) { sockets[mux]->sock_connected = connected; }
      } else {
        // Skip mux 0
        waitResponse("\n");
      }

      if (mux == 1) {
        int8_t res = waitResponse(3000, GF("+CCHOPEN:"));
        if (res == 1) {
          buf[0]  = '0';
          buf[1]  = '0';
          ret_mux = streamGetIntBefore(',');
          streamGetLength(buf, 2);
          if (buf[0] == '"' && buf[1] != '"') { connected = true; }
          // Serial.printf("\n\n mux=%d [0]=%c 0x%x -- [1]=%c 0x%x Connected=%d\n",
          // ret_mux,buf[0], buf[0], buf[1], buf[1], connected);
          if (mux == ret_mux) { sockets[mux]->sock_connected = connected; }
        }
      }

      // Wait OK
      waitResponse();
    } else {
      DBG("## Unhandle");
    }
    // DBG("Connect = ", sockets[mux]->sock_connected);
    return sockets[mux]->sock_connected;
  }

  bool sslDisconnect(uint8_t mux) {
    sendAT(GF("+CCHCLOSE="), mux);
    waitResponse(3000);
    waitResponse(3000UL,GF("+CCHCLOSE:"),GF("ERROR"));
    streamSkipUntil('\n');
    sendAT(GF("+CCHSTOP"));
    waitResponse(3000UL,GF("+CCHSTOP:"),GF("ERROR"));
    streamSkipUntil('\n');
    return true;
  }

  /*
   * Websocket functions
   */

 public:
  void setWebSocketRecvCallback(websocket_cb_t cb) {
    _websocket_cb = cb;
  }

 protected:
  // host: The string that described the server address and port. The range of
  //       the string length is 9 to 256 bytes. The string should be like this
  //       "ws://116.247.119.165:5141/test", must begin with "ws://". If the
  //       <server_addr> not include the port, the default port is 80.If the
  //       <server_addr> not include the path, the default path is /.
  // port: Invalid parameter, no need to pass in
  // ssl:  Invalid parameter, no need to pass in
  // mux:  A numeric parameter which specifies a particular PDP context .The
  //      range is 1-n,The maximum value n is related to the pdp command of the modem
  // Examples:
  // client.connect("ws://xxxxxxx.s2.eu.hivemq.cloud:8884/mqtt",8884);
  //
  bool websocketConnect(const char* host, uint16_t port, uint8_t mux, bool ssl,
                        int timeout_s) {
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    // DBG("host:", host, " port:", port, " timeout:", timeout_s);
    if (mux) {
      sendAT(GF("+WSSTART="), mux);
    } else {
      sendAT(GF("+WSSTART"));
    }
    waitResponse();
    // AT+WSCONNECT=<server_addr>,[time_out]
    sendAT(GF("+WSCONNECT=\""), host, "\",", timeout_s);
    int8_t res = waitResponse(timeout_ms, GF("+WSCONNECT:"), GF("ERROR"));
    if (res == 1) {
      res = streamGetIntBefore('\n');
      waitResponse();  // wait ok
      return res == 0;
    }
    return false;
  }

  int16_t websocketSend(const void* buff, size_t len, uint8_t mux) {
    // send data on prompt
    sendAT(GF("+WSSEND="), len, ',', mux);
    if (waitResponse(GF(">")) != 1) { return 0; }
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();
    if (waitResponse(GF("+WSSEND:")) != 1) { return 0; }
    int     ret_mux = streamGetIntBefore(',');
    int16_t ret_len = streamGetIntBefore('\n');
    waitResponse();  // Wait ok
    return ret_len;
  }

  size_t websocketRead(size_t size, uint8_t mux) {
    // TODO:
    if (size > websocket_available_bytes) {
      size_t tmp                = websocket_available_bytes;
      websocket_available_bytes = 0;
      return tmp;
    } else {
      websocket_available_bytes -= size;
      return size;
    }
    return 0;
  }

  size_t websocketAvailable(uint8_t mux) {
    // TODO:
    return websocket_available_bytes;
  }

  bool websocketConnected(uint8_t mux) {
    sendAT(GF("+WSCONNECT?"));
    int res = waitResponse(3000, GF("+WSCONNECT:"), GFP(GSM_OK), GFP(GSM_ERROR));
    if (res == 1) {
      int connect_status = streamGetIntBefore(',');  // connect_status
      streamSkipUntil(',');                          // server_addr
      streamSkipUntil('\n');                         // server_path
      waitResponse();                                // Wait ok
      sockets[mux]->sock_connected = connect_status;
    }
    return sockets[mux]->sock_connected;
  }

  bool websocketDisconnect(uint8_t mux) {
    sendAT(GF("+WSDISC=120"));
    waitResponse(10000UL);
    if (mux) {
      sendAT(GF("+WSSTOP="), mux);
    } else {
      sendAT(GF("+WSSTOP"));
    }
    return waitResponse(3000) == 1;
  }

  bool modemConnect(const char* host, uint16_t port, uint8_t mux, bool ssl = false,
                    int timeout_s = 75) {
    // DBG("modemConnect=", host);
    if (strncmp(host, "ws://", 5) == 0) {
      connType[mux] = TINYGSM_WEBSOCKET;
      return websocketConnect(host, port, mux, ssl, timeout_s);
    } else {
      connType[mux] = TINYGSM_SSL;
      return sslConnect(host, port, mux, ssl, timeout_s);
    }
    return false;
  }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    switch (connType[mux]) {
      case TINYGSM_WEBSOCKET: return websocketSend(buff, len, mux);
      case TINYGSM_SSL: return sslSend(buff, len, mux);
      default: break;
    }
    return false;
  }

  size_t modemRead(size_t size, uint8_t mux) {
    switch (connType[mux]) {
      case TINYGSM_WEBSOCKET: return websocketRead(size, mux);
      case TINYGSM_SSL: return sslRead(size, mux);
      default: break;
    }
    return false;
  }

  size_t modemGetAvailable(uint8_t mux) {
    switch (connType[mux]) {
      case TINYGSM_WEBSOCKET: return websocketAvailable(mux);
      case TINYGSM_SSL: return sslAvailable(mux);
      default: break;
    }
    return false;
  }

  bool modemGetConnected(uint8_t mux) {
    switch (connType[mux]) {
      case TINYGSM_WEBSOCKET: return websocketConnected(mux);
      case TINYGSM_SSL: return sslConnected(mux);
      default: break;
    }
    return false;
  }

  bool modemDisconnect(uint8_t mux) {
    switch (connType[mux]) {
      case TINYGSM_WEBSOCKET: return websocketDisconnect(mux);
      case TINYGSM_SSL: return sslDisconnect(mux);
      default: break;
    }
    return false;
  }

  /*
   * Utilities
   */
 public:
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
        } else if (data.endsWith(GF("SMS DONE"))) {
          data = "";
          // TODO:
        } else if (data.endsWith(GF("*ATREADY:"))) {
          streamSkipUntil('\n');
          data = "";
          // TODO:
        } else if (data.endsWith(GF("PB DONE"))) {
          data = "";
          // TODO:
        } else if (data.endsWith(GF("SIM REMOVED"))) {
          data = "";
          // TODO:
        } else if (data.endsWith(GF("+CCHEVENT: 0,RECV EVENT"))) {
          data = "";
          // TODO:
        } else if (data.endsWith(GF("+CCH_PEER_CLOSED:"))) {
          int8_t mux = streamGetIntBefore('\n');
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->sock_connected = false;
            DBG("### Closed: ", mux);
          }
          data = "";
        } else if (data.endsWith(GF("+WSDISC:"))) {
          data = "";
          DBG("## Websocket Disconnected!");
          // TODO:
          int res = streamGetIntBefore('\n');
          DBG("Error code:", res);

        } else if (data.endsWith(GF("+WSRECEIVE:"))) {
          data = "";
          // TODO:
          DBG("## Websocket get message receive!");
          int len_confirmed = streamGetIntBefore('\n');
          DBG("Recv length:", len_confirmed);
          while (!stream.available()) { TINY_GSM_YIELD(); }
          uint8_t mux = 0;
          sockets[mux]->rx.clear();  // Clear buffer
          for (int i = 0; i < len_confirmed; i++) {
            uint32_t startMillis = millis();
            while (!stream.available() &&
                   (millis() - startMillis < sockets[mux]->_timeout)) {
              TINY_GSM_YIELD();
            }
            char c = stream.read();
            sockets[mux]->rx.put(c);
          }
          websocket_available_bytes = len_confirmed;

          if (_websocket_cb) {
            // TODO: WEBSOCKET
          }
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

 protected:
  GsmClientA76xxSSL* sockets[TINY_GSM_MUX_COUNT];
  String             certificates[TINY_GSM_MUX_COUNT];
  String             client_private_key[TINY_GSM_MUX_COUNT];
  String             client_certificate[TINY_GSM_MUX_COUNT];
  String             client_private_key_password[TINY_GSM_MUX_COUNT];
  GsmClientConnType  connType[TINY_GSM_MUX_COUNT];
  size_t             websocket_available_bytes;
  websocket_cb_t     _websocket_cb;
};

#endif  // SRC_TINYGSMCLIENTA76XXSSL_H_
