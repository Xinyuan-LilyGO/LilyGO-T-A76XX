/**
 * @file      TinyGsmEmail.tpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-07-13
 *
 */
#pragma once

#include "TinyGsmCommon.h"

template <class modemType>
class TinyGsmEmail
{
protected:
    String _attachment_filename;
    String PATH = "C";

    uint32_t writeFileToEFS(const char *filename, const uint8_t *data, size_t size)
    {
        size_t writeLength = 0;
        thisModem().sendAT("+CFTRANRX=\"", PATH, ":/", filename, "\",", String(size));
        if (thisModem().waitResponse(30000, ">") == 1) {
            writeLength = thisModem().stream.write(data, size);
            thisModem().stream.println();
            thisModem().waitResponse(60000);
        }
        return writeLength;
    }
public:
    /*
     * Basic functions
     */
    bool email_set_host( String smtp_server, uint16_t smtp_port)
    {
        return email_set_host(smtp_server.c_str(), smtp_port);
    }

    bool email_set_host(const char *smtp_server, uint16_t smtp_port)
    {
        if (!smtp_server)return false;
        thisModem().sendAT("+CSMTPSSRV=", "\"", smtp_server, "\",", smtp_port);
        return thisModem().waitResponse(9000) == 1;
    }

    bool email_set_auth(bool requestAuth, String username, String password)
    {
        return email_set_auth(requestAuth, username.c_str(), password.c_str());
    }

    bool email_set_auth(bool requestAuth, const char *username, const char *password)
    {
        if (!username || !password)return false;
        thisModem().sendAT("+CSMTPSAUTH=", requestAuth, ",\"", username, "\"", ",\"", password, "\"");
        return thisModem().waitResponse(9000) == 1;
    }

    bool email_set_sender(String mail_from, String sender_name)
    {
        return email_set_sender(mail_from.c_str(), sender_name.c_str());
    }

    bool email_set_sender(const char *mail_from, const char *sender_name)
    {
        thisModem().sendAT("+CSMTPSFROM=", "\"", mail_from, "\",\"", sender_name, "\"");
        return thisModem().waitResponse(9000) == 1;
    }

    bool email_set_recipient(String recipient_address, String recipient_name, uint8_t recipient_kind = 0)
    {
        return email_set_recipient(recipient_address.c_str(), recipient_name.c_str(), recipient_kind);
    }

    bool email_set_recipient(const char *recipient_address, const char *recipient_name, uint8_t recipient_kind = 0)
    {
        thisModem().sendAT("+CSMTPSRCPT=", recipient_kind, ",0,", "\"", recipient_address, "\",\"", recipient_name, "\"");
        return thisModem().waitResponse(9000) == 1;
    }

    bool email_set_subject(String subject)
    {
        return email_set_subject(subject.c_str());
    }

    bool email_set_subject(const char *subject)
    {
        if (!subject)return false;
        thisModem().sendAT("+CSMTPSSUB=", strlen(subject), ",", "\"utf-8\"");
        if (thisModem().waitResponse(10000UL, ">") == 1) {
            thisModem().stream.write(subject);
        }
        return thisModem().waitResponse() == 1;
    }

    bool email_set_body(String body)
    {
        return email_set_body(body.c_str());
    }

    bool email_set_body(const char *body, const char  *character = NULL)
    {
        if (!body)return false;
        if (character) {
            // AT+CSMTPSBCH="utf-8"
            thisModem().sendAT("+CSMTPSBCH=", "\"", character, "\"");
            if (thisModem().waitResponse(3000) != 1)return false;
        }
        thisModem().sendAT("+CSMTPSBODY=", strlen(body));
        if (thisModem().waitResponse(10000UL, ">") == 1) {
            thisModem().stream.write(body);
        }
        return thisModem().waitResponse() == 1;
    }

    void email_set_path(const char *path)
    {
        PATH = path;
    }

    bool email_set_attachment(String filename, const uint8_t *data, size_t size)
    {
        return email_set_attachment(filename.c_str(), data, size);
    }

    // Select attachment
    bool email_set_attachment(const char *filename, const uint8_t *data, size_t size)
    {
        _attachment_filename.clear();
        _attachment_filename.concat(PATH);
        _attachment_filename.concat(":/");
        // Check file exists or not
        _attachment_filename.concat(filename);
        thisModem().sendAT("+FSATTRI=", _attachment_filename);
        if (thisModem().waitResponse() == 2 /*File not exist*/) {
            if (writeFileToEFS(filename, data, size) != size) {
                return false;
            }
        }
        thisModem().sendAT("+CSMTPSFILE=1,", "\"", PATH, ":/", filename, "\"");
        return thisModem().waitResponse(9000) == 1;
    }

    int32_t email_send()
    {
        int res = -1;
        thisModem().sendAT("+CSMTPSSEND");
        if (thisModem().waitResponse(3000) != 1)return -1;
        if (thisModem().waitResponse(9000, "+CSMTPSSEND: ") == 1) {
            res =  thisModem().streamGetLongLongBefore('\n');
            thisModem().stream.flush();
        }
        if (_attachment_filename.length()) {
            thisModem().sendAT("+FSDEL=", _attachment_filename);
            thisModem().waitResponse();
            _attachment_filename.clear();
        }
        return res;
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
