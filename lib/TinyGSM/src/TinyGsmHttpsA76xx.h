/**
 * @file      TinyGsmHttpsA76xx.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   LGPL-3.0
 * @date      2023-11-29
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

template <class modemType>
class TinyGsmHttpsA76xx
{
public:
    /*
     * Basic functions
     */
    bool https_begin()
    {
        https_end();

        thisModem().sendAT("+HTTPINIT");
        if (thisModem().waitResponse(10000UL) != 1) {
            return false;
        }
        // set sni
        thisModem().sendAT("+CSSLCFG=\"enableSNI\",0,1");
        thisModem().waitResponse();

        return true;
    }

    void https_end()
    {
        thisModem().sendAT("+HTTPTERM");
        thisModem().waitResponse(3000);
    }

    bool https_set_url(const String &url, ServerSSLVersion ssl_version = TINYGSM_SSL_AUTO)
    {
        // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/243
        // Set SSL Version
        thisModem().sendAT("+CSSLCFG=\"sslversion\",0,", ssl_version);
        thisModem().waitResponse();

        thisModem().sendAT("+HTTPPARA=\"URL\",", "\"", url, "\"");
        return thisModem().waitResponse(3000) == 1;
    }

    bool https_set_timeout(uint8_t conn_timeout_second = 120, uint8_t recv_timeout_second = 10, uint8_t resp_timeout_second = 20)
    {
        thisModem().sendAT("+HTTPPARA=\"CONNECTTO\",", conn_timeout_second);
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        thisModem().sendAT("+HTTPPARA=\"RECVTO\",", recv_timeout_second);
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        thisModem().sendAT("+HTTPPARA=\"RESPTO\",", recv_timeout_second);
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_set_user_agent(const String &userAgent)
    {
        return https_add_header("User-Agent", userAgent);
    }

    bool https_set_content_type(const char *contentType)
    {
        thisModem().sendAT("+HTTPPARA=\"CONTENT\",\"", contentType, "\"");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_set_accept_type(const char *acceptType)
    {
        thisModem().sendAT("+HTTPPARA=\"ACCEPT\",\"", acceptType, "\"");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_set_ssl_index(uint8_t sslId)
    {
        thisModem().sendAT("+HTTPPARA=\"SSLCFG\",\"", sslId, "\"");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_add_header(const String &name, const String &value)
    {
        return https_add_header(name.c_str(), value.c_str());
    }

    bool https_add_header(const char *name, const char *value)
    {
        thisModem().sendAT("+HTTPPARA=\"USERDATA\",\"", name, ": ", value, "\"");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_set_break(int _break, int _breakEnd)
    {
        thisModem().sendAT("+HTTPPARA=\"BREAK\",", _break);
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        thisModem().sendAT("+HTTPPARA=\"BREAKEND\",", _breakEnd);
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    int https_get(size_t *bodyLength = NULL)
    {
        thisModem().sendAT("+HTTPACTION=0");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        if (thisModem().waitResponse(60000UL, "+HTTPACTION: ") == 1) {
            int action = thisModem().streamGetIntBefore(',');
            int status = thisModem().streamGetIntBefore(',');
            size_t length = thisModem().streamGetLongLongBefore('\r');
            DBG("action:"); DBG(action);
            DBG("status:"); DBG(status);
            DBG("length:"); DBG(length);
            if (bodyLength) {
                *bodyLength = length;
            }
            return status;
        }
        return -1;
    }

    String https_header()
    {
        thisModem().sendAT("+HTTPHEAD");
        if (thisModem().waitResponse(30000UL, "+HTTPHEAD: ") != 1) {
            return "";
        }
        int length = thisModem().streamGetIntBefore('\n');
        if (length == -9999 || length == 0) {
            return "";
        }
        uint8_t *buffer = NULL;
        buffer = (uint8_t *)TINY_GSM_MALLOC(length + 1);
        if (!buffer) {
            return "";
        }
        if (thisModem().stream.readBytes(buffer, length ) != length) {
            free(buffer);
            return "";
        }
        buffer[length] = '\0';
        // wait ok
        thisModem().waitResponse();
        String header = String((const char *)buffer);
        free(buffer);
        return header;
    }


    int https_body(uint8_t *buffer, int buffer_size)
    {
        if (!buffer || !buffer_size) {
            return 0;
        }

        size_t length = https_get_size();

        if (length == 0)return 0;

        if (length > buffer_size) {
            length = buffer_size;
        }

        thisModem().sendAT("+HTTPREAD=0,", length);
        if (thisModem().waitResponse(3000) != 1) {
            return 0;
        }
        if (thisModem().waitResponse(30000UL, "+HTTPREAD: ") != 1) {
            return 0;
        }
        thisModem().streamGetIntBefore('\r');
        thisModem().streamGetIntBefore('\n');
        return thisModem().stream.readBytes(buffer, length);
    }


    String https_body()
    {
        int offset = 0;
        size_t total =  https_get_size();
        if (total == 0) {
            return "";
        }

        uint8_t *buffer =  (uint8_t *)TINY_GSM_MALLOC(total + 1);
        if (!buffer) {
            thisModem().stream.flush();
            return "";
        }

        thisModem().sendAT("+HTTPREAD=0,", total);
        if (thisModem().waitResponse(3000) != 1) {
            free(buffer);
            return "";
        }
        do {
            if (thisModem().waitResponse(30000UL, "+HTTPREAD: ") != 1) {
                free(buffer);
                return "";
            }
            int length = thisModem().streamGetIntBefore('\n');
            if (thisModem().stream.readBytes(buffer + offset, length) != length) {
                free(buffer);
                return "";
            }
            offset += length;
        } while (total != offset);
        thisModem().waitResponse(5000UL, "+HTTPREAD: 0");
        buffer[total] = '\0';
        String body = String((const char *)buffer);
        free(buffer);
        return body;
    }

    size_t https_get_size(void)
    {
        thisModem().sendAT("+HTTPREAD?");
        if (thisModem().waitResponse(120000UL, "+HTTPREAD: LEN,") != 1) {
            return 0;
        }
        size_t length = thisModem().streamGetLongLongBefore('\r');
        thisModem().stream.flush();
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
    int https_post(const char *payload, size_t size)
    {
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
    int https_post(const String &payload)
    {
        return https_method(TINYGSM_HTTP_POST, payload.c_str(), payload.length());
    }

    /**
     * @brief Sends an HTTPS POST request with a JSON-formatted String payload.
     *
     * This method is the same as the https_post method, just to unify the API for compatibility with the examples
     *
     * @param json The String object containing the JSON data to be sent in the request.
     * @return The HTTP status code of the response if the request is successful, -1
     * otherwise.
     */
    int https_post_json_format(const String &json)
    {
        return https_method(TINYGSM_HTTP_POST, json.c_str(), json.length());
    }

    /**
    * @brief Perform an HTTP PUT request with a C-style string payload.
    *
    * This function calls the `https_method` function to perform an HTTP PUT request,
    * passing the provided C-style string and its length as the request payload.
    *
    * @param payload A pointer to a character array containing the request payload data.
    * @param size The size of the request payload data in bytes.
    * @return int The return value of the `https_method` function, indicating the result of the request.
    */
    int https_put(const char *payload, size_t size)
    {
        return https_method(TINYGSM_HTTP_PUT, payload, size);
    }

    /**
     * @brief Perform an HTTP PUT request with a `String` object payload.
     *
     * This function calls the `https_method` function to perform an HTTP PUT request,
     * converting the provided `String` object to a C-style string and passing its length as the request payload.
     *
     * @param payload A reference to a `String` object containing the request payload data.
     * @return int The return value of the `https_method` function, indicating the result of the request.
     */
    int https_put(const String &payload)
    {
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
     * @return int The return value of the `https_method` function, indicating the result of the request.
     */
    int https_delete(const char *payload, size_t size)
    {
        return https_method(TINYGSM_HTTP_DELETE, payload, size);
    }

    /**
     * @brief Perform an HTTP DELETE request with a `String` object payload.
     *
     * This function calls the `https_method` function to perform an HTTP DELETE request,
     * converting the provided `String` object to a C-style string and passing its length as the request payload.
     *
     * @param payload A reference to a `String` object containing the request payload data.
     * @return int The return value of the `https_method` function, indicating the result of the request.
     */
    int https_delete(const String &payload)
    {
        return https_method(TINYGSM_HTTP_DELETE, payload.c_str(), payload.length());
    }

    /**
     * @brief  POSTFile
     * @note   Send file to server
     * @param  *filepath:File path and file name, full path required. C:/file for local storage and D:/file for SD card
     * @param  method:  0 = GET 1 = POST 2 = HEAD 3 = DELETE
     * @param  sendFileAsBody: 0 = Send file as HTTP header and body , 1 = Send file as Body
     * @retval httpCode, -1 = failed
     */
    int https_post_file(const char *filepath, uint8_t method = 1, bool sendFileAsBody = 1)
    {
        // A76XX Series_AT Command Manual_V1.09, Page 363
        // AT+HTTPPOSTFILE=<filename>[,<path>[,<method>[,<send_header>]]]
        // filename: full path required. C:/file for local storage and D:/file for SD card
        if (!filepath || strlen(filepath) < 4) return -1; // no file
        uint8_t path = filepath[0] == 'd' || filepath[0] == 'D' ? 2 : 1; // storage (2 for SD or 1 for local)
        thisModem().sendAT("+HTTPPOSTFILE=\"", filepath[3], "\",", path, ',', method, ',', sendFileAsBody);
        if (thisModem().waitResponse(120000UL) != 1) {
            return -1;
        }
        if (thisModem().waitResponse(150000UL, "+HTTPPOSTFILE:") == 1) {
            int action = thisModem().streamGetIntBefore(',');
            int status = thisModem().streamGetIntBefore(',');
            int length = thisModem().streamGetIntBefore('\r');
            DBG("action:"); DBG(action);
            DBG("status:"); DBG(status);
            DBG("length:"); DBG(length);
            return status;
        }
        return -1;
    }
private:
    int https_method(HttpMethod method, const char *payload, size_t size, uint32_t inputTimeout = 10000)
    {
        if (payload) {
            thisModem().sendAT("+HTTPDATA=", size, ",", inputTimeout);
            if (thisModem().waitResponse(30000UL, "DOWNLOAD") != 1) {
                return -1;
            }
            thisModem().stream.write(payload, size);
            if (thisModem().waitResponse(30000UL) != 1) {
                return -1;
            }
        }
        thisModem().sendAT("+HTTPACTION=", method);
        if (thisModem().waitResponse(3000) != 1) {
            return -1;
        }
        if (thisModem().waitResponse(60000UL, "+HTTPACTION:") == 1) {
            int action = thisModem().streamGetIntBefore(',');
            int status = thisModem().streamGetIntBefore(',');
            int length = thisModem().streamGetIntBefore('\r');
            DBG("action:"); DBG(action);
            DBG("status:"); DBG(status);
            DBG("length:"); DBG(length);
            return status;
        }
        return -1;
    }
    /*
     * CRTP Helper
     */
protected:
    inline const modemType &thisModem() const
    {
        return static_cast<const modemType &>(*this);
    }
    inline modemType &thisModem()
    {
        return static_cast<modemType &>(*this);
    }
};



