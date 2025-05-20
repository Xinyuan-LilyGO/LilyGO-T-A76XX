/**
 * @file      TinyGsmHttpsSIM7xxx.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   LGPL-3.0
 * @date      2025-04-23
 *
 */
#pragma once

#include "TinyGsmCommon.h"

enum ServerSSLVersion {
    TINYGSM_SSL_AUTO,  // Unkown
    TINYGSM_SSL_TLS1_0,
    TINYGSM_SSL_TLS1_1,
    TINYGSM_SSL_TLS1_2,
    TINYGSM_SSL_DTLS1_0,
    TINYGSM_SSL_DTLS1_2,
};

enum HttpMethod {
    TINYGSM_HTTP_GET = 1,
    TINYGSM_HTTP_PUT,
    TINYGSM_HTTP_POST,
    TINYGSM_HTTP_PATCH,
    TINYGSM_HTTP_HEAD,
};

#define TINYGSM_MODEM_HTTPS_NO_HEADER
#define TINYGSM_SIM7XXX_HTTP_BODY_MAX_LEN 1024

template <class modemType>
class TinyGsmHttpsSIM7xxx
{
private:
    String      _pathParam;
    String      _baseDomain;
    size_t      _bodyLength;
    size_t      _bodyOffset;
    uint8_t     _ctxindex;
    const char *_root_ca_filename;

public:
    /*
     * Basic functions
     */

    bool https_begin(uint8_t ctxindex = 1, const char *ca_filename = NULL)
    {
        _ctxindex = ctxindex;

        https_end();

        // set sni
        thisModem().sendAT("+CSSLCFG=\"sni\",0,1");
        thisModem().waitResponse();

        thisModem().sendAT("+SHCHEAD");
        thisModem().waitResponse();

        // max 1024 bytes
        thisModem().sendAT("+SHCONF=\"BODYLEN\",1024");
        thisModem().waitResponse();

        // max 350 bytes
        thisModem().sendAT("+SHCONF=\"HEADERLEN\",350");
        thisModem().waitResponse();

        // 0:ipv4 1:ipv6
        thisModem().sendAT("+SHCONF=\"IPVER\",0");
        thisModem().waitResponse();

        thisModem().sendAT("+SHCONF=\"TIMEOUT\",60");
        thisModem().waitResponse();

        thisModem().sendAT("+SHCPARA");
        thisModem().waitResponse();

        _pathParam        = "";
        _baseDomain       = "";
        _root_ca_filename = ca_filename;

        return true;
    }

    void https_end()
    {
        thisModem().sendAT("+SHDISC");
        thisModem().waitResponse(3000);
    }

    bool https_set_url(const String    &url,
                       ServerSSLVersion ssl_version = TINYGSM_SSL_TLS1_2)
    {
        if (url.length() > 64) {
            log_e("URL too long ,max length is 64bytes");
            return false;
        }
        if (!extractURLParts(url, _pathParam, _baseDomain)) {
            return false;
        }

        // SIM70XX does not support protocol auto-negotiation
        if (ssl_version == TINYGSM_SSL_AUTO) {
            ssl_version = TINYGSM_SSL_TLS1_2;
        }

        // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/243
        // Set SSL Version
        thisModem().sendAT("+CSSLCFG=\"sslversion\",", _ctxindex, ',', ssl_version);
        thisModem().waitResponse();

        if (_root_ca_filename) {
            thisModem().sendAT("+SHSSL=", _ctxindex, ",\"", _root_ca_filename, "\"");
            if (thisModem().waitResponse(3000) != 1) {
                return false;
            }
        } else {
            thisModem().sendAT("+SHSSL=", _ctxindex, ",\"\"");
            if (thisModem().waitResponse(3000) != 1) {
                return false;
            }
        }

        thisModem().sendAT("+SHCONF=\"URL\",", '"', _baseDomain, '"');
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }

        thisModem().sendAT("+SHCONN");
        return thisModem().waitResponse(60000UL) == 1;
    }

    bool https_set_timeout(uint8_t timeout_second = 60)
    {
        thisModem().sendAT("+SHCONF=\"TIMEOUT\",", timeout_second);
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
        return https_add_header("Content-Type", contentType);
    }

    bool https_set_accept_type(const char *acceptType)
    {
        return https_add_header("ACCEPT", acceptType);
    }

    bool https_set_ssl_index(uint8_t sslId)
    {
        return false;
    }

    bool https_add_header(const String &name, const String &value)
    {
        return https_add_header(name.c_str(), value.c_str());
    }

    bool https_add_header(const char *name, const char *value)
    {
        thisModem().sendAT("+SHAHEAD=\"", name, "\",\"", value, "\"");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        return true;
    }

    bool https_set_break(int _break, int _breakEnd)
    {
        return false;
    }

    int https_get(size_t *bodyLength = NULL)
    {
        thisModem().sendAT("+SHREQ=\"", _pathParam, "\",1");
        if (thisModem().waitResponse(3000) != 1) {
            return false;
        }
        if (thisModem().waitResponse(60000UL, "+SHREQ:") == 1) {
            thisModem().streamSkipUntil(',');
            int status  = thisModem().streamGetIntBefore(',');
            _bodyLength = thisModem().streamGetLongLongBefore('\r');
            DBG("status:");
            DBG(status);
            DBG("length:");
            DBG(_bodyLength);
            if (bodyLength) {
                *bodyLength = _bodyLength;
            }
            return status;
        }
        return -1;
    }

    String https_header()
    {
        return "SIM70XX MODEM does not support getting request header";
    }

    int https_body(uint8_t *buffer, int buffer_size)
    {
        if (!buffer || !buffer_size) {
            return 0;
        }
        size_t length = https_get_size();
        if (length == 0) return 0;
        if (length > buffer_size) {
            length = buffer_size;
        }
        thisModem().sendAT("+SHREAD=", _bodyOffset, ',', length);
        if (thisModem().waitResponse(3000) != 1) {
            return 0;
        }
        if (thisModem().waitResponse(30000UL, "+SHREAD: ") != 1) {
            return 0;
        }
        thisModem().streamGetIntBefore('\r');
        thisModem().streamGetIntBefore('\r');
        _bodyLength -= length;
        _bodyOffset += length;
        return thisModem().stream.readBytes(buffer, length);
    }

    String https_body()
    {
        int    offset = 0;
        size_t total  = https_get_size();
        if (total == 0) {
            return "";
        }

        uint8_t *buffer = (uint8_t *)TINY_GSM_MALLOC(total + 1);
        if (!buffer) {
            thisModem().stream.flush();
            return "";
        }
        thisModem().sendAT("+SHREAD=0,", total);
        if (thisModem().waitResponse(3000) != 1) {
            free(buffer);
            return "";
        }
        do {
            if (thisModem().waitResponse(30000UL, "+SHREAD: ") != 1) {
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
        thisModem().waitResponse(5000UL, "+SHREAD: 0");
        buffer[total] = '\0';
        String body   = String((const char *)buffer);
        free(buffer);
        return body;
    }

    size_t https_get_size(void)
    {
        return _bodyLength;
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
     * @param inputTimeout The timeout value (in milliseconds) for waiting for responses
     *                     during the request process. Defaults to 10000 milliseconds.
     * @return The HTTP status code of the response if the request is successful, -1
     * otherwise.
     */
    int https_post(const char *payload, size_t size, uint32_t inputTimeout = 10000)
    {
        if (size > TINYGSM_SIM7XXX_HTTP_BODY_MAX_LEN) {
            log_e("SIM7XXX max body length is 1024 bytes");
            return -1;
        }
        if (payload == NULL || size == 0) {
            return -1;
        }
        if (payload) {
            thisModem().sendAT("+SHBOD=", '"', (char *)payload, '"', ',', size);
            if (thisModem().waitResponse(30000UL) != 1) {
                return -1;
            }
            thisModem().sendAT("+SHREQ=\"", _pathParam, "\",", TINYGSM_HTTP_POST);
            if (thisModem().waitResponse(30000UL) != 1) {
                return -1;
            }
            if (thisModem().waitResponse(60000UL, "+SHREQ:") == 1) {
                thisModem().streamSkipUntil(',');
                int status  = thisModem().streamGetIntBefore(',');
                _bodyLength = thisModem().streamGetIntBefore('\r');
                DBG("status:", status, "length:", _bodyLength);
                return status;
            }
        }
        return -1;
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
        return https_post(payload.c_str(), payload.length());
    }

    /**
     * @brief Sends an HTTPS POST request with a JSON-formatted String payload.
     *
     * This function first escapes the double quotes in the JSON string, then sends
     * an HTTPS POST request using the processed JSON data. It logs both the original
     * and processed JSON strings for debugging purposes.
     *
     * @param json The String object containing the JSON data to be sent in the request.
     * @return The HTTP status code of the response if the request is successful, -1
     * otherwise.
     */
    int https_post_json_format(const String &json)
    {
        String payload = escapeQuotes(json);
        // log_d("InputJSON:%s", json.c_str());
        // log_d("ProcessStr:%s", payload.c_str());
        return https_post(payload.c_str(), json.length());
    }

    /**
     * @brief  downloadFile
     * @param  &url:  url
     * @param  &filePath: For example: "/customer/test.bin","/custapp/ test.bin
     * ","/fota/test.bin"
     * @param  timeout: Timeout of HTTP request. Unit is second. Range is 10-1000, default
     * value is 50.
     * @param  retryCount: Retry times of HTTP request Range is 5-100
     * @retval File Size
     */
    // ! No TEST
    size_t downloadFile(const String &url, const String &filePath, int timeout = 50,
                        int retryCount = 5)
    {
        int httpCode = -1;
        thisModem().sendAT("+HTTPTOFS=\"", url, "\",\"", filePath, "\",", timeout, ",",
                           retryCount);
        if (thisModem().waitResponse(3000) != 1) {
            return 0;
        }

        size_t len = 0;
        // If the file size is too large, the given waiting time should be longer
        if (thisModem().waitResponse(360000UL, "+HTTPTOFS: ") == 1) {
            httpCode = thisModem().streamGetIntBefore(',');
            len      = thisModem().streamGetLongLongBefore('\n');
            thisModem().waitResponse();
            if (httpCode != 200) {
                return 0;
            }
        }

        return len;
    }

    /**
     * @brief  https_post_file
     * @note   Send file to server
     * @param  *filepath:File path and file name, full path required
     * @param  method:  0 = GET 1 = POST 2 = HEAD 3 = DELETE
     * @param  sendFileAsBody: 0 = Send file as HTTP header and body , 1 = Send file as Body
     * @retval httpCode, -1 = failed
     */
    // ! No TEST
    int https_post_file(const char *filepath, uint8_t method = 1, bool sendFileAsBody = 1)
    {
        // AT+HTTPPOSTFILE=<filename>[,<path>[,<method>[,<send_header>]]]
        uint8_t path = 1;
        if (!filepath) return -1;
        if (&filepath[3] == NULL) return -1;
        if (filepath[0] == 'd' || filepath[0] == 'D') {
            path = 2;
        }
        thisModem().sendAT("+HTTPPOSTFILE=\"", &filepath[3], "\",", path, ',', method, ',',
                           sendFileAsBody);
        if (thisModem().waitResponse(120000UL) != 1) {
            return -1;
        }
        if (thisModem().waitResponse(150000UL, "+HTTPPOSTFILE:") == 1) {
            int action = thisModem().streamGetIntBefore(',');
            int status = thisModem().streamGetIntBefore(',');
            int length = thisModem().streamGetIntBefore('\r');
            DBG("action:");
            DBG(action);
            DBG("status:");
            DBG(status);
            DBG("length:");
            DBG(length);
            return status;
        }
        return -1;
    }

private:
    // This function is used to check and escape double quotes in a string.
    String escapeQuotes(const String &input)
    {
        String result;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] == '"') {
                result += '\\';
            }
            result += input[i];
        }
        return result;
    }

    bool extractURLParts(String url, String &pathParam, String &baseDomain)
    {
        if (url.indexOf("://") == -1) {
            log_e(
                "Error: The input string is not a valid URL (missing the protocol header ://)");
            return false;
        }
        int protocolEnd = url.indexOf("//") + 2;
        int pathStart   = url.indexOf("/", protocolEnd);
        if (pathStart == -1) {
            pathParam  = "/";
            baseDomain = url;
        } else {
            pathParam  = url.substring(pathStart);
            baseDomain = url.substring(0, pathStart);
        }
        return true;
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
