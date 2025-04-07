/**
 * @file      TinyGsmMqttA76xx.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   LGPL-3.0
 * @date      2023-11-29
 *
 */
#pragma once

#include "TinyGsmCommon.h"

#define TINY_GSM_MQTT_CLI_COUNT 2

template <class modemType, uint8_t muxCount>
class TinyGsmMqttA76xx
{
public:
    typedef void (*callback_t)(const char *, const uint8_t *, uint32_t);
protected:
    bool __ssl = false;
    bool __sni = false;
    uint8_t *buffer = NULL;
    uint32_t bufferSize = 256;
    callback_t callback;
    const char  *cert_pem;           /*!< SSL server certification, PEM format as string, if the client requires to verify server */
    const char  *client_cert_pem;    /*!< SSL client certification, PEM format as string, if the server requires to verify client */
    const char  *client_key_pem;     /*!< SSL client key, PEM format as string, if the server requires to verify client */
    const char  *will_topic;
    const char  *will_msg;
    uint8_t will_qos = 0;


public:
    /*
     * Basic functions
     */
    bool mqtt_begin(bool ssl, bool sni = false)
    {
        __sni = sni;
        __ssl = ssl;
        if (!this->buffer) {
            this->buffer = (uint8_t *)TINY_GSM_MALLOC(bufferSize);
            if (!this->buffer)return false;
        }
        this->cert_pem = NULL;
        this->client_cert_pem = NULL;
        this->client_key_pem = NULL;
        memset(this->buffer, 0, bufferSize);
        thisModem().sendAT("+CMQTTSTART");
        if (thisModem().waitResponse(30000UL, "+CMQTTSTART: 0") != 1)return false;
        thisModem().waitResponse();
        return true;
    }

    bool mqtt_end()
    {
        if (this->buffer) {
            free(this->buffer);
            this->buffer = NULL;
        }
        thisModem().sendAT("+CMQTTSTOP");
        thisModem().waitResponse("+CMQTTSTOP: 0");
        if (thisModem().waitResponse(3000) != 1)return false;
        return true;
    }

    // caFile:           /*!< SSL server certification, PEM format as string, if the client requires to verify server */
    // clientCertFile:    /*!< SSL client certification, PEM format as string, if the server requires to verify client */
    // clientCertKey:     /*!< SSL client key, PEM format as string, if the server requires to verify client */
    void mqtt_set_certificate(const char *caFile,
                              const char *clientCertFile = NULL,
                              const char *clientCertKey = NULL)
    {
        this->cert_pem = caFile;
        this->client_cert_pem = clientCertFile;
        this->client_key_pem = clientCertKey;
    }

    void setWillMessage(const char *topic, const char *msg, uint8_t qos)
    {
        will_msg = msg;
        will_topic = topic;
        will_qos = qos;
    }

    bool mqtt_connect(
        uint8_t clientIndex,
        const char *server, uint16_t port,
        const char *clientID,
        const char *username = NULL,
        const char *password = NULL,
        uint32_t keepalive_time = 60)
    {
        uint8_t authMethod = 0;

        if (clientIndex > muxCount) {
            return false;
        }

        if (this->cert_pem || this->client_cert_pem || this->client_key_pem) {
            if (this->cert_pem) {
                thisModem().sendAT("+CCERTDOWN=\"ca_cert.pem\",", strlen(this->cert_pem));
                if (thisModem().waitResponse(10000UL, ">") == 1) {
                    thisModem().stream.write(this->cert_pem);
                }
                if (thisModem().waitResponse() != 1) {
                    ESP_LOGE("A76XX", "Write ca_cert pem failed!");
                    return false;
                }
                thisModem().sendAT("+CSSLCFG=\"cacert\",0,\"ca_cert.pem\"");
                thisModem().waitResponse();

            }
            if (this->client_cert_pem) {
                thisModem().sendAT("+CCERTDOWN=\"cert.pem\",", strlen(this->client_cert_pem));
                if (thisModem().waitResponse(10000UL, ">") == 1) {
                    thisModem().stream.write(this->client_cert_pem);
                }
                if (thisModem().waitResponse() != 1) {
                    ESP_LOGE("A76XX", "Write cert pem failed!");
                    return false;
                }
                thisModem().sendAT("+CSSLCFG=\"clientcert\",0,\"cert.pem\"");
                thisModem().waitResponse();

            }
            if (this->client_key_pem) {
                thisModem().sendAT("+CCERTDOWN=\"key_cert.pem\",", strlen(this->client_key_pem));
                if (thisModem().waitResponse(10000UL, ">") == 1) {
                    thisModem().stream.write(this->client_key_pem);
                }
                if (thisModem().waitResponse() != 1) {
                    ESP_LOGE("A76XX", "Write key_cert failed!");
                    return false;
                }
                thisModem().sendAT("+CSSLCFG=\"clientkey\",0,\"key_cert.pem\"");
                thisModem().waitResponse();
            }

            // authMethod:
            // 0 – no authentication.
            // 1 – server authentication. It needs the root CA of the server.
            // 2 – server and client authentication. It needs the root CA of the server, the cert and key of the client.
            // 3 – client authentication and no server authentication. It needs the cert and key of the client.
            if (this->client_cert_pem && this->client_key_pem && !this->cert_pem) {
                authMethod = 3;
            } else if (this->client_cert_pem && this->client_key_pem && this->cert_pem) {
                authMethod = 2;
            }  else if (this->cert_pem) {
                authMethod = 1;
            } else {
                authMethod = 0;
            }

            thisModem().sendAT("+CSSLCFG=\"sslversion\",0,4");
            thisModem().waitResponse();

            thisModem().sendAT("+CMQTTSSLCFG=", clientIndex, ",", 0);
            thisModem().waitResponse(3000);

            __ssl = true;
        }

        // Some MQTT brokers need to enable sni
        if (__sni) {
            thisModem().sendAT("+CSSLCFG=\"enableSNI\",0,1");
            thisModem().waitResponse();
        }

        thisModem().sendAT("+CSSLCFG=\"authmode\",0,", authMethod);
        thisModem().waitResponse();

        thisModem().sendAT("+CMQTTREL=", clientIndex);
        thisModem().waitResponse(3000);

        thisModem().sendAT("+CMQTTACCQ=", clientIndex, ",\"", clientID, "\",", __ssl);
        if (thisModem().waitResponse(3000) != 1)return false;

        // Set MQTT3.1.1 , Default use MQTT 3.1
        thisModem().sendAT("+CMQTTCFG=\"version\",", clientIndex, ",4");
        thisModem().waitResponse(30000UL);

        if (will_msg && will_topic) {
            if (!mqttWillTopic(clientIndex, will_topic)) {
                return false;
            }
            if (!mqttWillMessage(clientIndex, will_msg, will_qos)) {
                return false;
            }
        }

        if (username && password) {
            thisModem().sendAT("+CMQTTCONNECT=", clientIndex, ',', "\"tcp://", server, ':', port, "\",", keepalive_time, ',', 1, ",\"", username, "\",\"", password, "\"");
        } else {
            thisModem().sendAT("+CMQTTCONNECT=", clientIndex, ',', "\"tcp://", server, ':', port, "\",", keepalive_time, ',', 1);
        }
        if (thisModem().waitResponse(30000UL) != 1)return false;

        if (thisModem().waitResponse(30000UL, "+CMQTTCONNECT: ") != 1) {
            return false;
        }
        thisModem().streamSkipUntil(',');
        int res = thisModem().stream.read();
        if (res != '0')return false;
        return true;

    }

    int mqtt_disconnect(uint8_t clientIndex = 0, uint32_t timeout = 120)
    {
        if (clientIndex > muxCount) {
            return false;
        }
        thisModem().sendAT("+CMQTTDISC=", clientIndex, ',', timeout);
        thisModem().waitResponse(3000);
        thisModem().waitResponse(10000UL, "+CMQTTDISC: ");
        // int id = thisModem().streamGetIntBefore(',');
        thisModem().streamSkipUntil(',');
        int status = thisModem().streamGetIntBefore('\n');
        thisModem().stream.flush();

        thisModem().sendAT("+CMQTTREL=", clientIndex);
        thisModem().waitResponse(3000);
        thisModem().sendAT("+CMQTTSTOP");
        thisModem().waitResponse("+CMQTTSTOP: ");
        status = thisModem().streamGetIntBefore('\n');
        if (thisModem().waitResponse(3000) != 1)return false;
        return status;
    }

    bool mqtt_publish(uint8_t clientIndex, const char *topic, const char *playload,
                      uint8_t qos = 0, uint32_t timeout = 60)
    {
        if (clientIndex > muxCount) {
            return false;
        }
        // +CMQTTTOPIC: (0-1),(1-1024)
        // <client_index>,<req_length>
        //  publish message topic
        thisModem().sendAT("+CMQTTTOPIC=", clientIndex, ',', strlen(topic));
        if (thisModem().waitResponse(10000UL, ">") != 1) {
            return false;
        }
        thisModem().stream.write(topic);
        thisModem().stream.println();

        if (thisModem().waitResponse() != 1) {
            return false;
        }

        // AT+CMQTTPAYLOAD Input the publish message body
        // +CMQTTPAYLOAD: (0-1),(1-10240)
        // <client_index>,<req_length>
        thisModem().sendAT("+CMQTTPAYLOAD=", clientIndex, ',', strlen(playload));
        if (thisModem().waitResponse(10000UL, ">") != 1) {
            return false;
        }
        thisModem().stream.write(playload);
        thisModem().stream.println();
        // Wait return OK
        if (thisModem().waitResponse() != 1) {
            return false;
        }
        // +CMQTTPUB: (0-1),(0-2),(60-180),(0-1),(0-1)
        // <client_index>,<qos>,<pub_timeout>,<ratained>,<dup>
        thisModem().sendAT("+CMQTTPUB=", clientIndex, ',', qos, ',', timeout);
        if (thisModem().waitResponse() != 1) {
            return false;
        }
        return true;
    }

    bool mqtt_subscribe(uint8_t clientIndex, const char *topic, uint8_t qos = 0, uint8_t dup = 0)
    {
        if (clientIndex > muxCount) {
            return false;
        }
        // Subscribe a message to server
        // +CMQTTSUB: (0-1),(1-1024),(0-2),(0-1)
        thisModem().sendAT("+CMQTTSUB=", clientIndex, ',', strlen(topic), ',', qos, ',', dup);
        if (thisModem().waitResponse(10000UL, ">") != 1) {
            return false;
        }
        thisModem().waitResponse('>');
        thisModem().stream.write(topic);
        thisModem().stream.println();
        // Wait return OK
        if (thisModem().waitResponse(10000UL) != 1) {
            return false;
        }
        thisModem().waitResponse("+CMQTTSUB: ");
        int id = thisModem().streamGetIntBefore(',');
        int status = thisModem().streamGetIntBefore('\n');
        thisModem().stream.flush();
        return id == clientIndex && status == 0;
    }

    bool mqtt_unsubscribe(uint8_t clientIndex, const char *topic)
    {
        if (clientIndex > muxCount) {
            return false;
        }
        thisModem().sendAT("+CMQTTUNSUBTOPIC=", clientIndex, ',', strlen(topic));
        if (thisModem().waitResponse(10000UL, ">") != 1) {
            return false;
        }
        thisModem().stream.write(topic);
        thisModem().stream.println();
        // Wait return OK
        if (thisModem().waitResponse(10000UL) != 1) {
            return false;
        }
        // Subscribe a message to server
        // +CMQTTSUB: (0-1),(1-1024),(0-2),(0-1)
        thisModem().sendAT("+CMQTTUNSUB=", clientIndex, ',', '0');
        if (thisModem().waitResponse(10000UL) != 1) {
            return false;
        }
        // thisModem().waitResponse("+CMQTTUNSUB: 0,0");
        // return true;
        thisModem().waitResponse("+CMQTTUNSUB: ");
        int id = thisModem().streamGetIntBefore(',');
        int status = thisModem().streamGetIntBefore('\n');
        thisModem().stream.flush();
        return id == clientIndex && status == 0;
    }

    bool mqtt_connected(uint8_t clientIndex = 0)
    {
        if (clientIndex > muxCount) {
            return false;
        }
        static uint32_t lastCheck = 0;
        if (millis() - lastCheck < 10000) {
            return true;
        }
        lastCheck = millis();
        int result = 0;
        int i = TINY_GSM_MQTT_CLI_COUNT;
        thisModem().sendAT("+CMQTTDISC?");
        while (i--) {
            if (thisModem().waitResponse("+CMQTTDISC: ") == 1) {
                if (thisModem().streamGetIntBefore(',') == clientIndex) {
                    result = thisModem().streamGetIntBefore('\n');
                    thisModem().waitResponse();
                    return result == 0;
                }
            }
        }
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
            this->buffer = (uint8_t *)TINY_GSM_MALLOC(size);
        } else {
            uint8_t *newBuffer = (uint8_t *)TINY_GSM_REALLOC(this->buffer, size);
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
    /*

    +CMQTTRXSTART: 0,17,2039
    Recvice CMQTTRXSTART
    topic_total_len:17
    payload_total_len:2039
    +CMQTTRXTOPIC: 0,17
    /Sim7600/user/get
    +CMQTTRXPAYLOAD: 0,1500
        +CMQTTRXEND: 0
    */
    // +CMQTTRXSTART:<client_index>,<topic_total_len>,<payload_total_len>
    // +CMQTTRXTOPIC: <client_index>, <sub_topic_len><sub_topic>
    bool mqtt_handle(uint32_t timeout = 100)
    {
        //TODO:More than 1500 bytes will carry the Modem return flag
        // +CMQTTCONNLOST: 1,1
        if (thisModem().waitResponse(timeout, "+CMQTTRXSTART:") == 1) {
            thisModem().streamSkipUntil(',');
            size_t topicSize = 0;
            size_t plyloadSize = 0;
            size_t topic_total_len =  thisModem().streamGetIntBefore(',');
            size_t payload_total_len =  thisModem().streamGetIntBefore('\n');
            if (thisModem().waitResponse(timeout, "+CMQTTRXTOPIC:") == 1) {

                thisModem().streamSkipUntil('\n');
                topicSize = topic_total_len > bufferSize ? bufferSize - 1 : topic_total_len;
                thisModem().stream.readBytes(buffer, topicSize);
                buffer[topicSize] = '\0';
                topicSize += 1;

                if (topicSize == bufferSize) {
                    DBG("Buffer overflow!");
                    thisModem().waitResponse(10000UL);
                    return false;
                }
                size_t recvSize = 0;
                size_t remainingSize = bufferSize - topicSize;
                size_t bufferOffset = topicSize;

                do {
                    if (thisModem().waitResponse(timeout, "+CMQTTRXPAYLOAD:") == 1) {
                        thisModem().streamSkipUntil(',');
                        int packetSize = thisModem().streamGetIntBefore('\n');

                        plyloadSize = packetSize > remainingSize ? remainingSize : packetSize;

                        if (bufferOffset >= bufferSize) {
                            DBG("Buffer overflow!");
                            break;
                        }
                        thisModem().stream.readBytes(buffer + bufferOffset, plyloadSize);

                        remainingSize -= plyloadSize;
                        bufferOffset += plyloadSize;
                        recvSize += packetSize;

                    }
                } while (recvSize != payload_total_len);

                if (thisModem().waitResponse(timeout, "+CMQTTRXEND: 0") == 1) {
                    if (this->callback) {
                        this->callback((const char *)buffer, buffer + topicSize, recvSize);
                    }
                    memset(this->buffer, 0, bufferSize);
                    return true;
                }

            }
        }
        return false;
    }


protected:
    bool mqttWillTopic(uint8_t clientIndex, const char *topic)
    {
        if (clientIndex > muxCount) {
            DBG("Error: Client index out of bounds");
            return false;
        }

        // Set the Will topic
        // +CMQTTWILLTOPIC: <client_index>,<req_length>
        thisModem().sendAT("+CMQTTWILLTOPIC=", clientIndex, ',', strlen(topic));

        int response = thisModem().waitResponse(10000UL, ">");
        if (response != 1) {
            DBG("Error: Did not receive expected '>' prompt, response: ", response);
            return false;
        }

        // Send the actual topic
        thisModem().stream.write(topic);
        thisModem().stream.println();

        response = thisModem().waitResponse();
        if (response != 1) {
            DBG("Error: Did not receive 'OK' after sending the Will topic, response: ", response);
            return false;
        }

        return true;
    }

    bool mqttWillMessage(uint8_t clientIndex, const char *message, uint8_t qos)
    {
        if (clientIndex > muxCount) {
            DBG("Error: Client index out of bounds");
            return false;
        }

        // Set the Will message
        // +CMQTTWILLMSG: <client_index>,<req_length>,<qos>
        thisModem().sendAT("+CMQTTWILLMSG=", clientIndex, ',', strlen(message), ',', qos);

        int response = thisModem().waitResponse(10000UL, ">");
        if (response != 1) {
            DBG("Error: Did not receive expected '>' prompt, response: ", response);
            return false;
        }

        // Send the actual message
        thisModem().stream.write(message);
        thisModem().stream.println();

        response = thisModem().waitResponse();
        if (response != 1) {
            DBG("Error: Did not receive 'OK' after sending the Will message, response: ", response);
            return false;
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



