/**
 * @file      TinyGsmMqttSIM7xxx.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   LGPL-3.0
 * @date      2025-04-23
 *
 */
#pragma once

#include "TinyGsmCommon.h"

#define TINY_GSM_MQTT_CLI_COUNT 1

template <class modemType, uint8_t muxCount = TINY_GSM_MQTT_CLI_COUNT>
class TinyGsmMqttSIM7xxx
{
public:
    typedef void (*callback_t)(const char*, const uint8_t*, uint32_t);

protected:
    bool        __sni      = false;
    uint8_t    *buffer     = NULL;
    uint32_t    bufferSize = 256;
    callback_t  callback;
    const char *cert_pem; /*!< SSL server certification, PEM format as string, if the client
                         requires to verify server */
    const char *client_cert_pem; /*!< SSL client certification, PEM format as string, if the
                                server requires to verify client */
    const char *client_key_pem;  /*!< SSL client key, PEM format as string, if the server
                                requires to verify client */
    const char *will_topic;
    const char *will_msg;
    uint8_t     will_qos = 0;
    bool        _isConnected = false;
    uint32_t    _lastCheckConnect = 0;
public:
    /*
     * Basic functions
     */
    bool mqtt_begin(bool ssl, bool sni = false)
    {
        __sni = sni;
        if (!this->buffer) {
            this->buffer = (uint8_t*)TINY_GSM_MALLOC(bufferSize);
            if (!this->buffer) return false;
        }
        this->cert_pem        = NULL;
        this->client_cert_pem = NULL;
        this->client_key_pem  = NULL;
        _isConnected = false;
        _lastCheckConnect = 0;
        memset(this->buffer, 0, bufferSize);
        thisModem().sendAT("+SMDISC");
        thisModem().waitResponse();
        return true;
    }

    bool mqtt_end()
    {
        if (this->buffer) {
            free(this->buffer);
            this->buffer = NULL;
        }
        mqtt_disconnect();
        _isConnected = false;
        _lastCheckConnect = 0;
        return true;
    }

    // caFile:         /*!< SSL server certification, PEM format as string, if the client
    // requires to verify server */ clientCertFile: /*!< SSL client certification, PEM
    // format as string, if the server requires to verify client */ clientCertKey:  /*!< SSL
    // client key, PEM format as string, if the server requires to verify client */
    void mqtt_set_certificate(const char* caFile, const char* clientCertFile = NULL,
                              const char *clientCertKey = NULL)
    {
        this->cert_pem        = caFile;
        this->client_cert_pem = clientCertFile;
        this->client_key_pem  = clientCertKey;
    }


    bool mqtt_connect(uint8_t clientIndex, const char* server, uint16_t port,
                      const char *clientID, const char *username = NULL,
                      const char *password = NULL, uint32_t keepalive_time = 60,
                      uint8_t qos = 0)
    {
        uint32_t inputTime = 10000;

        if (this->cert_pem || this->client_cert_pem || this->client_key_pem) {
            if (this->cert_pem) {
                if (!thisModem().downloadCertificateImpl("rootCA.pem", this->cert_pem)) {
                    log_e("Download rootCA failed!");
                    return false;
                }
                if (!thisModem().convertCertificateImpl(2, "rootCA.pem")) {
                    log_e("Convert rootCa failed!");
                    return false;
                }
            }

            if (this->client_cert_pem) {
                if (!thisModem().downloadCertificateImpl("deviceCert.crt",
                        this->client_cert_pem)) {
                    log_e("Download deviceCert failed!");
                    return false;
                }
            }

            if (this->client_key_pem) {
                if (!thisModem().downloadCertificateImpl("devicePrivateKey.crt",
                        this->client_key_pem)) {
                    log_e("Download devicePSK failed!");
                    return false;
                }
            }

            if (this->client_key_pem && this->client_cert_pem) {
                if (!thisModem().convertCertificateImpl(1, "deviceCert.crt",
                                                        "devicePrivateKey.crt")) {
                    log_e("Convert deviceCert or client_key failed!");
                    return false;
                }
            }

            // Configure the SSL/TLS version for a secure socket connection
            if (!thisModem().sslConfigVersionImpl(0, 3)) {
                log_e("Configure SSL Version failed!");
                return false;
            }

            if (!thisModem().sslConfigSniImpl(0, server)) {
                log_e("Configure SSL SNI failed!");
                return false;
            }

            if (this->cert_pem && !this->client_cert_pem) {
                thisModem().sendAT("+SMSSL=1,", '"', "rootCA.pem", '"', ',', "\"\"");
                if (thisModem().waitResponse() != 1) {
                    log_e("Setting SMSSL failed!");
                    return false;
                }
            }

            //! AT+SMSSL=<index>,<ca list>,<cert name> , depes AT+CSSLCFG
            //! <index>SSL status, range: 0-6
            //!     0 Not support SSL
            //!     1-6 Corresponding to AT+CSSLCFG command parameter <ctindex>
            //! <ca list>CA_LIST file name, length 20 byte
            //! <cert name>CERT_NAME file name, length 20 byte
            if (this->cert_pem && this->client_cert_pem) {
                thisModem().sendAT("+SMSSL=1,", '"', "rootCA.pem", '"', ',', '"',
                                   "deviceCert.crt", '"');
                if (thisModem().waitResponse() != 1) {
                    log_e("Setting SMSSL failed!");
                    return false;
                }
            }
        } else {
            thisModem().sendAT("+SMSSL=0");
            thisModem().waitResponse();
        }

        // ! KEEPTIME
        thisModem().sendAT("+SMCONF=\"KEEPTIME\",", keepalive_time);
        if (thisModem().waitResponse(3000) != 1) return false;

        // ! CLEANSS
        thisModem().sendAT("+SMCONF=\"CLEANSS\",1");
        if (thisModem().waitResponse(3000) != 1) return false;

        // ! QOS
        thisModem().sendAT("+SMCONF=\"QOS\",", qos);
        if (thisModem().waitResponse(3000) != 1) return false;

        // ! RETAIN
        thisModem().sendAT("+SMCONF=\"RETAIN\",1");
        if (thisModem().waitResponse(3000) != 1) return false;

        // ! Client ID
        if (clientID) {
            thisModem().sendAT("+SMCONF=\"CLIENTID\",", clientID);
            if (thisModem().waitResponse(3000) != 1) return false;
        }


        if (username) {
            thisModem().sendAT("+SMCONF=\"USERNAME\",\"", username, "\"");
            if (thisModem().waitResponse(3000) != 1) return false;
        }


        if (password) {
            thisModem().sendAT("+SMCONF=\"PASSWORD\",\"", password, "\"");
            if (thisModem().waitResponse(3000) != 1) return false;
        }

        thisModem().sendAT("+SMCONF=URL,", "\"", server, "\",", port);
        if (thisModem().waitResponse(3000) != 1) return false;

        // ! Will message
        if (will_msg && will_topic) {
            thisModem().sendAT("+SMCONF=\"TOPIC\",", will_topic);
            if (thisModem().waitResponse(3000) != 1) return false;

            thisModem().sendAT("+SMCONF=\"MESSAGE\",", will_msg);
            if (thisModem().waitResponse(3000) != 1) return false;
        }

        thisModem().sendAT("+SMCONN");
        return (thisModem().waitResponse(60000UL) == 1);
    }

    bool mqtt_disconnect(uint8_t clientIndex = 0, uint32_t timeout = 120)
    {
        thisModem().sendAT("+SMDISC");
        return thisModem().waitResponse(60000UL) == 1;
    }

    bool mqtt_publish(uint8_t clientIndex, const char* topic, const char* payload,
                      uint8_t qos = 0, uint32_t timeout = 60)
    {
        // AT+SMPUB=<topic>,<contentlength>,<qos>,<retain>
        //  publish message topic
        thisModem().sendAT("+SMPUB=\"", topic, "\",", strlen(payload), ",", qos, ",", "1");
        if (thisModem().waitResponse(10000UL, ">") != 1) {
            return false;
        }
        thisModem().stream.write(payload);
        thisModem().stream.println();
        // Wait return OK
        return thisModem().waitResponse() == 1;
    }

    bool mqtt_subscribe(uint8_t clientIndex, const char* topic, uint8_t qos = 0,
                        uint8_t dup = 0)
    {
        thisModem().sendAT("+SMSUB=\"", topic, "\",", qos);
        return thisModem().waitResponse() == 1;
    }

    bool mqtt_unsubscribe(uint8_t clientIndex, const char* topic)
    {
        thisModem().sendAT("+SMUNSUB=\"", topic, "\"");
        return thisModem().waitResponse() == 1;
    }

    bool mqtt_connected(uint8_t clientIndex = 0)
    {
        if (millis() - _lastCheckConnect < 10000) {
            return _isConnected;
        }
        _lastCheckConnect  = millis();
        int result = 0;
        thisModem().sendAT("+SMSTATE?");
        if (thisModem().waitResponse("+SMSTATE: ") == 1) {
            result = thisModem().streamGetIntBefore('\n');
            thisModem().waitResponse();
            _isConnected = (result == 1);
            return _isConnected;
        }
        _isConnected = false;
        return false;
    }

    bool mqtt_set_rx_buffer_size(uint32_t size)
    {
        if (size == 0) {
            return false;
        }
        if (size == this->bufferSize) {
            return true;
        }
        if (this->bufferSize == 0) {
            this->buffer = (uint8_t*)TINY_GSM_MALLOC(size);
        } else {
            uint8_t *newBuffer = (uint8_t*)TINY_GSM_REALLOC(this->buffer, size);
            if (newBuffer != NULL) {
                this->buffer = newBuffer;
            } else {
                return false;
            }
        }
        this->bufferSize = size;
        return (this->buffer != NULL);
    }

    void mqtt_set_callback(callback_t cb)
    {
        this->callback = cb;
    }

    bool mqtt_handle(uint32_t timeout = 100)
    {
        if (thisModem().waitResponse(timeout, "+SMSUB:") == 1) {
            size_t topicSize   = 0;
            size_t payloadSize = 0;
            topicSize          = thisModem().stream.readBytesUntil(',', buffer, bufferSize);
            payloadSize        = thisModem().stream.readBytesUntil(',', buffer + topicSize,
                                 bufferSize - topicSize - 1);
            if (this->callback) {
                buffer[topicSize] = '\0';
                this->callback((const char*)buffer, buffer + topicSize + 1, payloadSize);
            }
            memset(this->buffer, 0, bufferSize);
            return true;
        }
        return false;
    }

    void setWillMessage(const char* topic, const char* msg, uint8_t qos)
    {
        will_msg   = msg;
        will_topic = topic;
        will_qos   = qos;
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
