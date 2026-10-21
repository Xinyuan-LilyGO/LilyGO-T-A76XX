/**
 * Issue 548 standard UART sleep/wake test for LilyGO T-SIM7600X.
 *
 * Commands in the serial monitor:
 *   r - create a deterministic 4096-byte EFS file
 *   b - read it with CSCLK=0 (baseline)
 *   l - enter ESP32 light sleep with CSCLK=1, then read it after wake
 */

#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_sleep.h>

#include "../ATdebug/utilities.h"

static constexpr char TEST_PATH[] = "E:/issue548.bin";
static constexpr size_t TEST_FILE_SIZE = 4096;
static constexpr size_t WINDOW_SIZE = 1024;
static constexpr uint32_t MODEM_SLEEP_SETTLE_MS = 25000;
static constexpr uint32_t WAKE_UART_SETTLE_MS = 300;
static constexpr uint32_t IO_TIMEOUT_MS = 5000;

static uint8_t testByte(size_t index)
{
    return static_cast<uint8_t>((index * 37U + 11U) & 0xffU);
}

static void flushModem()
{
    while (SerialAT.available()) {
        SerialAT.read();
    }
}

static bool readExact(uint8_t *buffer, size_t length, uint32_t timeout)
{
    const uint32_t deadline = millis() + timeout;
    size_t received = 0;
    while (received < length && static_cast<int32_t>(millis() - deadline) < 0) {
        if (SerialAT.available()) {
            received += SerialAT.readBytes(buffer + received, length - received);
        } else {
            delay(1);
        }
    }
    return received == length;
}

static String readLine(uint32_t timeout)
{
    String line;
    const uint32_t deadline = millis() + timeout;
    while (static_cast<int32_t>(millis() - deadline) < 0) {
        if (!SerialAT.available()) {
            delay(1);
            continue;
        }
        const char c = static_cast<char>(SerialAT.read());
        line += c;
        if (c == '\n') {
            break;
        }
    }
    return line;
}

static bool command(const String &at, uint32_t timeout = IO_TIMEOUT_MS)
{
    flushModem();
    SerialAT.print(at);
    SerialAT.print("\r\n");
    String response;
    const uint32_t deadline = millis() + timeout;
    while (static_cast<int32_t>(millis() - deadline) < 0) {
        if (SerialAT.available()) {
            response += static_cast<char>(SerialAT.read());
            if (response.endsWith("\r\nOK\r\n") || response.endsWith("\r\nERROR\r\n")) {
                break;
            }
        } else {
            delay(1);
        }
    }
    Serial.printf("AT %s => %s", at.c_str(), response.length() ? response.c_str() : "<timeout>\r\n");
    return response.indexOf("\r\nOK") >= 0 || response == "OK\r\n";
}

static bool createTestFile()
{
    flushModem();
    SerialAT.printf("AT+CFTRANRX=\"%s\",%u\r\n", TEST_PATH, static_cast<unsigned>(TEST_FILE_SIZE));

    const uint32_t deadline = millis() + IO_TIMEOUT_MS;
    bool prompt = false;
    while (static_cast<int32_t>(millis() - deadline) < 0) {
        if (SerialAT.available()) {
            const char c = static_cast<char>(SerialAT.read());
            if (c == '>') {
                prompt = true;
                break;
            }
        } else {
            delay(1);
        }
    }
    if (!prompt) {
        Serial.println("CREATE FAIL: no CFTRANRX prompt");
        return false;
    }

    uint8_t block[128];
    for (size_t offset = 0; offset < TEST_FILE_SIZE; offset += sizeof(block)) {
        const size_t count = min(sizeof(block), TEST_FILE_SIZE - offset);
        for (size_t i = 0; i < count; ++i) {
            block[i] = testByte(offset + i);
        }
        SerialAT.write(block, count);
    }
    SerialAT.flush();

    String response;
    const uint32_t responseDeadline = millis() + IO_TIMEOUT_MS;
    while (static_cast<int32_t>(millis() - responseDeadline) < 0) {
        if (SerialAT.available()) {
            response += static_cast<char>(SerialAT.read());
            if (response.endsWith("\r\nOK\r\n") || response.endsWith("\r\nERROR\r\n")) {
                break;
            }
        } else {
            delay(1);
        }
    }
    Serial.printf("CREATE %s\n", response.indexOf("\r\nOK") >= 0 ? "PASS" : "FAIL");
    return response.indexOf("\r\nOK") >= 0;
}

static bool readWindow(size_t location, size_t requested)
{
    flushModem();
    SerialAT.printf("AT+CFTRANTX=\"%s\",%u,%u\r\n", TEST_PATH,
                    static_cast<unsigned>(location), static_cast<unsigned>(requested));

    size_t checked = 0;
    bool sawData = false;
    for (;;) {
        const String line = readLine(IO_TIMEOUT_MS);
        if (line.length() == 0) {
            Serial.printf("WINDOW %u FAIL: timeout waiting for DATA header\n", static_cast<unsigned>(location));
            return false;
        }

        const int header = line.indexOf("+CFTRANTX: DATA,");
        if (header >= 0) {
            const size_t comma = line.indexOf(',', header);
            const size_t chunkLength = line.substring(comma + 1).toInt();
            if (chunkLength == 0 || chunkLength > 1024) {
                Serial.println("WINDOW FAIL: invalid DATA length");
                return false;
            }
            uint8_t block[1024];
            if (!readExact(block, chunkLength, IO_TIMEOUT_MS)) {
                Serial.println("WINDOW FAIL: short DATA block");
                return false;
            }
            sawData = true;
            for (size_t i = 0; i < chunkLength; ++i) {
                if (checked + i >= requested || block[i] != testByte(location + checked + i)) {
                    Serial.printf("WINDOW %u FAIL: byte mismatch at offset %u\n",
                                  static_cast<unsigned>(location), static_cast<unsigned>(checked + i));
                    return false;
                }
            }
            checked += chunkLength;
            continue;
        }

        if (line.indexOf("+CFTRANTX:0") >= 0 || line.indexOf("+CFTRANTX: 0") >= 0) {
            String terminal;
            // Firmware variants may insert an extra CRLF between the final
            // result code and OK. Skip blank lines but stop on the terminal.
            for (uint8_t i = 0; i < 3 && terminal.indexOf("OK") < 0 && terminal.indexOf("ERROR") < 0; ++i) {
                terminal = readLine(IO_TIMEOUT_MS);
            }
            const bool ok = sawData && checked == requested && terminal.indexOf("OK") >= 0;
            Serial.printf("WINDOW %u %s (%u bytes)\n", static_cast<unsigned>(location),
                          ok ? "PASS" : "FAIL", static_cast<unsigned>(checked));
            return ok;
        }
        // Echo, blank lines, and modem URCs are ignored before the first DATA header.
    }
}

static bool readWholeFile(const char *label)
{
    Serial.printf("--- %s ---\n", label);
    bool passed = true;
    for (size_t location = 0; location < TEST_FILE_SIZE; location += WINDOW_SIZE) {
        passed = readWindow(location, WINDOW_SIZE) && passed;
    }
    Serial.printf("%s %s\n", label, passed ? "PASS" : "FAIL");
    return passed;
}

static void restoreUartAfterLightSleep()
{
    SerialAT.end();
    uart_driver_delete(UART_NUM_1);
    delay(20);
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
    delay(WAKE_UART_SETTLE_MS);
    digitalWrite(MODEM_DTR_PIN, LOW);
    delay(WAKE_UART_SETTLE_MS);
    flushModem();
    command("AT");
    command("AT+CATR=1");
    command("AT+CSCLK=0");
}

static void lightSleepCycle()
{
    command("AT+CSCLK=1");
    flushModem();
    digitalWrite(MODEM_DTR_PIN, HIGH);
    Serial.printf("DTR HIGH; waiting %u ms before ESP32 light sleep\n",
                  static_cast<unsigned>(MODEM_SLEEP_SETTLE_MS));
    delay(MODEM_SLEEP_SETTLE_MS);

    gpio_wakeup_enable(static_cast<gpio_num_t>(MODEM_RING_PIN), GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_sleep_enable_timer_wakeup(5000000ULL);
    Serial.flush();
    esp_light_sleep_start();
    gpio_wakeup_disable(static_cast<gpio_num_t>(MODEM_RING_PIN));

    Serial.println("ESP32 woke; restoring UART and modem UART sleep state");
    restoreUartAfterLightSleep();
}

void setup()
{
    Serial.begin(115200);
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);
    pinMode(MODEM_FLIGHT_PIN, OUTPUT);
    digitalWrite(MODEM_FLIGHT_PIN, HIGH);
    pinMode(MODEM_RING_PIN, INPUT_PULLUP);
    delay(1000);

    Serial.printf("READY: %s; pins RX=%d TX=%d DTR=%d RI=%d\n", PRODUCT_MODEL_NAME,
                  MODEM_RX_PIN, MODEM_TX_PIN, MODEM_DTR_PIN, MODEM_RING_PIN);
    command("AT");
    command("AT+CSCLK=0");
    Serial.println("Commands: r=create file, b=baseline, l=light sleep test");
}

void loop()
{
    if (!Serial.available()) {
        delay(10);
        return;
    }
    const char commandChar = static_cast<char>(Serial.read());
    if (commandChar == 'r') {
        createTestFile();
    } else if (commandChar == 'b') {
        command("AT+CSCLK=0");
        digitalWrite(MODEM_DTR_PIN, LOW);
        readWholeFile("BASELINE");
    } else if (commandChar == 'l') {
        lightSleepCycle();
        readWholeFile("LIGHT_SLEEP");
    }
}
