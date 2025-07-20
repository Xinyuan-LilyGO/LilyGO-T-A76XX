/**
 * @file      TinyGsmHttpsComm.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   LGPL-3.0
 * @date      2025-05-06
 *
 */
#pragma once

#include "TinyGsmCommon.h"

enum ServerSSLVersion {
  TINYGSM_SSL_TLS3_0,
  TINYGSM_SSL_TLS1_0,
  TINYGSM_SSL_TLS1_1,
  TINYGSM_SSL_TLS1_2,
  TINYGSM_SSL_AUTO,
};

enum HttpMethod {
  TINYGSM_HTTP_GET = 0,
  TINYGSM_HTTP_POST,
  TINYGSM_HTTP_HEAD,
  TINYGSM_HTTP_DELETE,
  TINYGSM_HTTP_PUT,
  TINYGSM_HTTP_PATCH,
};


template <class modemType, ModemPlatform platform>
class TinyGsmHttpsComm {
 public:
  /**
   * @brief Initialize the HTTPS connection.
   *
   * This function is used to start the HTTPS connection process.
   * It prepares the necessary resources and configurations for the subsequent HTTPS
   * operations.
   *
   * @return true if the initialization is successful, false otherwise.
   */
  bool https_begin() {
    https_end();

    delay(100);

    thisModem().sendAT("+HTTPINIT");
    if (thisModem().waitResponse(10000UL) != 1) { return false; }
    // set sni
    thisModem().sendAT("+CSSLCFG=\"enableSNI\",0,1");
    thisModem().waitResponse();

    return true;
  }

  /**
   * @brief Terminate the HTTPS connection.
   *
   * This function is used to end the HTTPS connection.
   * It releases the resources allocated during the connection and cleans up the related
   * configurations.
   */
  void https_end() {
    thisModem().sendAT("+HTTPTERM");
    thisModem().waitResponse(3000);
  }

  /**
   * @brief Set the URL and SSL version for the HTTPS request.
   *
   * This function sets the target URL for the HTTPS request and the SSL version to be
   * used. If no SSL version is specified, it defaults to TINYGSM_SSL_AUTO.
   *
   * @param url The target URL for the HTTPS request.
   * @param ssl_version The SSL version to be used. Defaults to TINYGSM_SSL_AUTO.
   * @return true if the URL and SSL version are set successfully, false otherwise.
   */
  bool https_set_url(const String& url, ServerSSLVersion ssl_version = TINYGSM_SSL_AUTO) {
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/243
    // Set SSL Version
    thisModem().sendAT("+CSSLCFG=\"sslversion\",0,", ssl_version);
    thisModem().waitResponse();

    thisModem().sendAT("+HTTPPARA=\"URL\",", "\"", url, "\"");
    return thisModem().waitResponse(3000) == 1;
  }

  /**
   * @brief Set the timeouts for the HTTPS connection, receiving data, and getting the
   * response.
   *
   * This function allows you to set the timeouts for different stages of the HTTPS
   * request. If no values are provided, it uses the default timeouts: 120 seconds for
   * connection, 10 seconds for receiving data, and 20 seconds for getting the response.
   *
   * @param conn_timeout_second The timeout for the connection in seconds. Defaults to
   * 120.
   * @param recv_timeout_second The timeout for receiving data in seconds. Defaults to 10.
   * @param resp_timeout_second The timeout for getting the response in seconds. Defaults
   * to 20.
   * @return true if the timeouts are set successfully, false otherwise.
   */
  bool https_set_timeout(uint8_t conn_timeout_second = 120,
                         uint8_t recv_timeout_second = 10,
                         uint8_t resp_timeout_second = 20) {
    thisModem().sendAT("+HTTPPARA=\"CONNECTTO\",", conn_timeout_second);
    if (thisModem().waitResponse(3000) != 1) { return false; }
    thisModem().sendAT("+HTTPPARA=\"RECVTO\",", recv_timeout_second);
    if (thisModem().waitResponse(3000) != 1) { return false; }
    thisModem().sendAT("+HTTPPARA=\"RESPTO\",", recv_timeout_second);
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Set the user agent for the HTTPS request.
   *
   * This function sets the user agent string that will be sent with the HTTPS request.
   * The user agent identifies the client making the request.
   *
   * @param userAgent The user agent string to be set.
   * @return true if the user agent is set successfully, false otherwise.
   */
  bool https_set_user_agent(const String& userAgent) {
    return https_add_header("User-Agent", userAgent);
  }

  /**
   * @brief Set the content type for the HTTPS request.
   *
   * This function sets the content type of the data that will be sent in the HTTPS
   * request. It is used to indicate the format of the data.
   *
   * @param contentType The content type string to be set.
   * @return true if the content type is set successfully, false otherwise.
   */
  bool https_set_content_type(const char* contentType) {
    thisModem().sendAT("+HTTPPARA=\"CONTENT\",\"", contentType, "\"");
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Set the accept type for the HTTPS request.
   *
   * This function sets the types of content that the client is willing to accept in the
   * response. It helps the server to send the appropriate data format.
   *
   * @param acceptType The accept type string to be set.
   * @return true if the accept type is set successfully, false otherwise.
   */
  bool https_set_accept_type(const char* acceptType) {
    thisModem().sendAT("+HTTPPARA=\"ACCEPT\",\"", acceptType, "\"");
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Set the SSL index for the HTTPS request.
   *
   * This function sets the SSL index which is used to select the appropriate SSL
   * configuration.
   *
   * @param sslId The SSL index to be set.
   * @return true if the SSL index is set successfully, false otherwise.
   */
  bool https_set_ssl_index(uint8_t sslId) {
    thisModem().sendAT("+HTTPPARA=\"SSLCFG\",\"", sslId, "\"");
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Add a custom header to the HTTPS request using String objects.
   *
   * This function allows you to add a custom header to the HTTPS request.
   * The header consists of a name and a value, both represented as String objects.
   *
   * @param name The name of the header.
   * @param value The value of the header.
   * @return true if the header is added successfully, false otherwise.
   */
  bool https_add_header(const String& name, const String& value) {
    return https_add_header(name.c_str(), value.c_str());
  }

  /**
   * @brief Add a custom header to the HTTPS request using C-style strings.
   *
   * This function allows you to add a custom header to the HTTPS request.
   * The header consists of a name and a value, both represented as C-style strings.
   *
   * @param name The name of the header.
   * @param value The value of the header.
   * @return true if the header is added successfully, false otherwise.
   */
  bool https_add_header(const char* name, const char* value) {
    thisModem().sendAT("+HTTPPARA=\"USERDATA\",\"", name, ": ", value, "\"");
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Set the break conditions for the HTTPS request.
   *
   * This function sets the break conditions which can be used to control the flow of the
   * HTTPS request.
   *
   * @param _break The start break condition.
   * @param _breakEnd The end break condition.
   * @return true if the break conditions are set successfully, false otherwise.
   */
  bool https_set_break(int _break, int _breakEnd) {
    thisModem().sendAT("+HTTPPARA=\"BREAK\",", _break);
    if (thisModem().waitResponse(3000) != 1) { return false; }
    thisModem().sendAT("+HTTPPARA=\"BREAKEND\",", _breakEnd);
    if (thisModem().waitResponse(3000) != 1) { return false; }
    return true;
  }

  /**
   * @brief Send a GET request and optionally get the length of the response body.
   *
   * This function sends a GET request to the specified URL.
   * If a pointer to a size_t variable is provided, it will store the length of the
   * response body.
   *
   * @param bodyLength A pointer to a size_t variable to store the length of the response
   * body. Defaults to NULL.
   * @return The HTTP status code of the response.
   */
  int https_get(size_t* bodyLength = NULL) {
    thisModem().sendAT("+HTTPACTION=0");
    if (thisModem().waitResponse(3000) != 1) { return false; }
    if (thisModem().waitResponse(60000UL, "+HTTPACTION: ") == 1) {
      int    action = thisModem().streamGetIntBefore(',');
      int    status = thisModem().streamGetIntBefore(',');
      size_t length = thisModem().streamGetLongLongBefore('\r');
      log_d("Method:%d http code:%d length:%u", action, status, length);
      if (bodyLength) { *bodyLength = length; }
      return status;
    }
    return -1;
  }

  /**
   * @brief Get the headers of the HTTPS response.
   *
   * This function retrieves the headers of the HTTPS response as a String object.
   *
   * @return A String object containing the headers of the HTTPS response.
   */
  String https_header() {
    thisModem().sendAT("+HTTPHEAD");
    if (!https_wait_header_respond()) { return ""; }
    int length = thisModem().streamGetIntBefore('\n');
    if (length == -9999 || length == 0) {
      log_e("header is invalid");
      return "";
    }
    uint8_t* buffer = NULL;
    buffer          = (uint8_t*)TINY_GSM_MALLOC(length + 1);
    if (!buffer) {
      log_e("malloc is failed");
      return "";
    }
    if (thisModem().stream.readBytes(buffer, length) != length) {
      free(buffer);
      log_e("readbytes is failed");
      return "";
    }
    buffer[length] = '\0';
    // wait ok
    thisModem().waitResponse();
    String header = String((const char*)buffer);
    free(buffer);
    return header;
  }

  /**
   * @brief Get the body of the HTTPS response and store it in a buffer.
   *
   * This function reads the body of the HTTPS response and stores it in the provided
   * buffer.
   *
   * @param buffer The buffer to store the response body.
   * @param buffer_size The size of the buffer.
   * @return The number of bytes read from the response body.
   */
  int https_body(uint8_t* buffer, int buffer_size) {
    if (!buffer || !buffer_size) { return 0; }

    size_t length = https_get_size();

    if (length == 0) return 0;

    if (length > buffer_size) { length = buffer_size; }

    thisModem().sendAT("+HTTPREAD=0,", length);
    if (thisModem().waitResponse(3000) != 1) { return 0; }
    if (!https_wait_body_respond()) { return 0; }
    thisModem().streamGetIntBefore('\r');
    thisModem().streamGetIntBefore('\n');
    return thisModem().stream.readBytes(buffer, length);
  }

  /**
   * @brief Get the body of the HTTPS response as a String object.
   *
   * This function retrieves the body of the HTTPS response as a String object.
   *
   * @return A String object containing the body of the HTTPS response.
   */
  String https_body() {
    int    offset = 0;
    size_t total  = https_get_size();
    if (total == 0) { return ""; }
    uint8_t* buffer = (uint8_t*)TINY_GSM_MALLOC(total + 1);
    if (!buffer) {
      thisModem().stream.flush();
      return "";
    }
    log_v("malloc memory %u bytes", total + 1);
    thisModem().sendAT("+HTTPREAD=0,", total);
    if (thisModem().waitResponse(3000) != 1) {
      free(buffer);
      return "";
    }
    do {
      if (!https_wait_body_respond()) {
        free(buffer);
        return "";
      }
      int length = thisModem().streamGetIntBefore('\n');
      log_v("length = %d total:%d offset:%d", length, total, offset);
      if (length == -9999) { break; }
      if (thisModem().stream.readBytes(buffer + offset, length) != length) {
        free(buffer);
        return "";
      }
      offset += length;
    } while (total != offset);
    thisModem().waitResponse(5000UL, "+HTTPREAD: 0");
    buffer[total] = '\0';
    String body   = String((const char*)buffer);
    free(buffer);
    return body;
  }

  /**
   * @brief Query HTTPS response data size
   * @return For A76XX, SIM7670G, this method returns the current remaining bytes, while
   * SIM7600G returns the current readable data size, not the total data byte size
   */
  size_t https_get_size(void) {
    thisModem().sendAT("+HTTPREAD?");
    if (thisModem().waitResponse(120000UL, "+HTTPREAD: LEN,") != 1) { return 0; }
    size_t length = thisModem().streamGetLongLongBefore('\r');
    thisModem().waitResponse();
    return length;
  }

  /**
   * @brief Sends an HTTPS POST request with a raw payload.
   *
   * This function sends an HTTPS POST request using the provided payload. It checks
   * the size of the payload to ensure it does not exceed the maximum allowed length.
   * It then configures the HTTP body and sends the request.
   *
   * @param payload A pointer to the raw payload data to be sent in the request.
   * @param size The size of the payload in bytes.
   * @return The HTTP status code of the response if the request is successful, -1
   * otherwise.
   */
  int https_post(const char* payload, size_t size) {
    return https_method(TINYGSM_HTTP_POST, payload, size);
  }

  /**
   * @brief Sends an HTTPS POST request with a String payload.
   *
   * This is a wrapper function that converts a String object to a C-style string
   * and its length, then calls the https_post function with raw payload parameters.
   *
   * @param payload The String object containing the payload data to be sent in the
   * request.
   * @return The HTTP status code of the response if the request is successful, -1
   * otherwise.
   */
  int https_post(const String& payload) {
    return https_method(TINYGSM_HTTP_POST, payload.c_str(), payload.length());
  }

  /**
   * @brief Sends an HTTPS POST request with a JSON-formatted String payload.
   *
   * This method is the same as the https_post method, just to unify the API for
   * compatibility with the examples
   *
   * @param json The String object containing the JSON data to be sent in the request.
   * @return The HTTP status code of the response if the request is successful, -1
   * otherwise.
   */
  int https_post_json_format(const String& json) {
    return https_method(TINYGSM_HTTP_POST, json.c_str(), json.length());
  }

  /**
   * @brief Perform an HTTP PUT request with a C-style string payload.
   *
   * This function calls the `https_method` function to perform an HTTP PUT request,
   * passing the provided C-style string and its length as the request payload.
   *
   * SIM7600X NOT SUPPORT HTTP PUT METHOD
   *
   * @param payload A pointer to a character array containing the request payload data.
   * @param size The size of the request payload data in bytes.
   * @return int The return value of the `https_method` function, indicating the result of
   * the request.
   */
  int https_put(const char* payload, size_t size) {
    if (platform == QUALCOMM_SIM7600G) {
      log_e("SIM7600 NOT SUPPORT HTTP PUT METHOD");
      return -1;
    }
    return https_method(TINYGSM_HTTP_PUT, payload, size);
  }

  /**
   * @brief Perform an HTTP PUT request with a `String` object payload.
   *
   * This function calls the `https_method` function to perform an HTTP PUT request,
   * converting the provided `String` object to a C-style string and passing its length as
   * the request payload.
   *
   * SIM7600X NOT SUPPORT HTTP PUT METHOD
   * 
   * @param payload A reference to a `String` object containing the request payload data.
   * @return int The return value of the `https_method` function, indicating the result of
   * the request.
   */
  int https_put(const String& payload) {
    if (platform == QUALCOMM_SIM7600G) {
      log_e("SIM7600 NOT SUPPORT HTTP PUT METHOD");
      return -1;
    }
    return https_method(TINYGSM_HTTP_PUT, payload.c_str(), payload.length());
  }

  /**
   * @brief Perform an HTTP DELETE request with a C-style string payload.
   *
   * This function calls the `https_method` function to perform an HTTP DELETE request,
   * passing the provided C-style string and its length as the request payload.
   *
   * @param payload A pointer to a character array containing the request payload data.
   * @param size The size of the request payload data in bytes.
   * @return int The return value of the `https_method` function, indicating the result of
   * the request.
   */
  int https_delete(const char* payload, size_t size) {
    return https_method(TINYGSM_HTTP_DELETE, payload, size);
  }

  /**
   * @brief Perform an HTTP DELETE request with a `String` object payload.
   *
   * This function calls the `https_method` function to perform an HTTP DELETE request,
   * converting the provided `String` object to a C-style string and passing its length as
   * the request payload.
   *
   * @param payload A reference to a `String` object containing the request payload data.
   * @return int The return value of the `https_method` function, indicating the result of
   * the request.
   */
  int https_delete(const String& payload) {
    return https_method(TINYGSM_HTTP_DELETE, payload.c_str(), payload.length());
  }

  /**
   * @brief  POSTFile
   * @note   Send file to server
   * @param  *filepath:File path and file name, full path required. C:/file for local
   * storage and D:/file for SD card
   * @param  method:  0 = GET 1 = POST 2 = HEAD 3 = DELETE
   * @param  sendFileAsBody: 0 = Send file as HTTP header and body , 1 = Send file as Body
   * @retval httpCode, -1 = failed
   */
  int https_post_file(const char* filepath, uint8_t method = 1, bool sendFileAsBody = 1) {
    // A76XX Series_AT Command Manual_V1.09, Page 363
    // AT+HTTPPOSTFILE=<filename>[,<path>[,<method>[,<send_header>]]]
    // filename: full path required. C:/file for local storage and D:/file for SD card
    if (!filepath || strlen(filepath) < 4) return -1;  // no file
    uint8_t path = filepath[0] == 'd' || filepath[0] == 'D'
        ? 2
        : 1;  // storage (2 for SD or 1 for local)
    thisModem().sendAT("+HTTPPOSTFILE=\"", filepath[3], "\",", path, ',', method, ',',
                       sendFileAsBody);
    if (thisModem().waitResponse(120000UL) != 1) { return -1; }
    if (thisModem().waitResponse(150000UL, "+HTTPPOSTFILE:") == 1) {
      int action = thisModem().streamGetIntBefore(',');
      int status = thisModem().streamGetIntBefore(',');
      int length = thisModem().streamGetIntBefore('\r');
      log_d("Method:%d http code:%d length:%u", action, status, length);
      return status;
    }
    return -1;
  }

 private:
  bool https_wait_header_respond() {
    const char* header_respond = "+HTTPHEAD: ";
    switch (platform) {
      case QUALCOMM_SIM7600G: header_respond = "+HTTPHEAD: DATA,";
      default: break;
    }
    return thisModem().waitResponse(30000UL, header_respond) == 1;
  }

  bool https_wait_body_respond() {
    const char* body_respond = "+HTTPREAD: ";
    switch (platform) {
      case QUALCOMM_SIM7600G: body_respond = "+HTTPREAD: DATA,";
      default: break;
    }
    return thisModem().waitResponse(30000UL, body_respond) == 1;
  }

  int https_method(HttpMethod method, const char* payload, size_t size,
                   uint32_t inputTimeout = 10000) {
    if (payload) {
      thisModem().sendAT("+HTTPDATA=", size, ",", inputTimeout);
      if (thisModem().waitResponse(30000UL, "DOWNLOAD") != 1) { return -1; }
      thisModem().stream.write(payload, size);
      if (thisModem().waitResponse(30000UL) != 1) { return -1; }
    }
    thisModem().sendAT("+HTTPACTION=", method);
    if (thisModem().waitResponse(3000) != 1) { return -1; }
    if (thisModem().waitResponse(60000UL, "+HTTPACTION:") == 1) {
      int    action = thisModem().streamGetIntBefore(',');
      int    status = thisModem().streamGetIntBefore(',');
      size_t length = thisModem().streamGetLongLongBefore('\r');
      log_d("Method:%d http code:%d length:%u", action, status, length);
      return status;
    }
    return -1;
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
};
