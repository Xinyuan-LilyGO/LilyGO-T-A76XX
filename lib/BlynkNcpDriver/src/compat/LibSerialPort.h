
/*
 * libserialport binding for Arduino:
 *   http://sigrok.org/wiki/Libserialport
 *
 * Author: Volodymyr Shymanskyy
 * Date:   Jun 10, 2023
 */

#ifndef LIBSERIALPORT_H
#define LIBSERIALPORT_H

#include "Stream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libserialport.h>

class SerialPort
  : public Stream
{
  public:
    SerialPort(const char* dev) {
        _dev = dev;
    }

    virtual ~SerialPort() {
        end();
    }

    bool begin(unsigned long baud) {
        if (_port) {
            sp_set_baudrate(_port, baud);
            return true;
        }

        enum sp_return result;

        result = sp_get_port_by_name(_dev, &_port);
        if (result != SP_OK) {
            _port = NULL;
            return false;
        }

        result = sp_open(_port, SP_MODE_READ_WRITE);
        if (result != SP_OK) {
            sp_free_port(_port);
            _port = NULL;
            return false;
        }

        sp_set_baudrate(_port, baud);
        sp_set_bits(_port, 8);
        sp_set_parity(_port, SP_PARITY_NONE);
        sp_set_stopbits(_port, 1);
        sp_set_flowcontrol(_port, SP_FLOWCONTROL_NONE);

        return true;
    }

    void end() {
        if (_port) {
            sp_close(_port);
            sp_free_port(_port);
            _port = NULL;
        }
    }

    size_t write(uint8_t c) override {
        return sp_blocking_write(_port, &c, 1, 0);
    }

    // Use all other overloaded versions of the write() function from the
    // Print parent class.
    using Print::write;

    operator bool() { return _port != NULL; }

    int available() override {
        return sp_input_waiting(_port);
    }

    int read() override {
        char c;
        if (sp_nonblocking_read(_port, &c, 1) == 1) {
            return c;
        } else {
            return -1;
        }
    }

    size_t readBytes(char *buffer, size_t length) override {
        int bytes = sp_blocking_read(_port, buffer, length, _timeout);
        if (bytes >= 0) {
            return bytes;
        } else {
            return 0;
        }
    }

    size_t write(const uint8_t *buffer, size_t length) override {
        int bytes = sp_blocking_write(_port, buffer, length, _timeout);
        if (bytes >= 0) {
            return bytes;
        } else {
            return 0;
        }
    }

    int peek() override { return -1; } // TODO

  private:
    struct sp_port* _port;
    const char*     _dev;
};

#endif // LIBSERIALPORT_H
