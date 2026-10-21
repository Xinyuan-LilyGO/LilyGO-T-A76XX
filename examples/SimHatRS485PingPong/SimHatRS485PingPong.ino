/**
 * @file      SimHatRS485PingPong.ino
 * @license   MIT
 * @copyright Copyright (c) 2025 ShenZhen XinYuan Electronic Technology Co., Ltd
 * @note      RS485 ping-pong example for the T-SimHat CAN/RS485/Relay board.
 *
 * Flash this sketch to two boards connected through RS485. Keep
 * RS485_PING_PONG_INITIATOR defined on one board and comment it out on the
 * other board. The initiator sends PING periodically and the other board
 * replies with PONG.
 */
#include "Arduino.h"
#include "utilities.h"

#define SerialRS485     Serial2

#if defined(LILYGO_T_A7670)  || defined(LILYGO_SIM7600X) || \
    defined(LILYGO_SIM7000G) || defined(LILYGO_T_A7608X)

#define CAN_RX_PIN          21
#define CAN_TX_PIN          22
#define CAN_ENABLE_PIN      0
#define RS485_TX_PIN        19
#define RS485_RX_PIN        18
#define RS485_ENABLE_PIN    23
#define RELAY_PIN           32  // SIM7600 modem DTR pin conflict

#elif defined(LILYGO_T_A7608X_S3)

#define CAN_RX_PIN          2
#define CAN_TX_PIN          1
#define CAN_ENABLE_PIN      0
#define RS485_TX_PIN        41
#define RS485_RX_PIN        40
#define RS485_ENABLE_PIN    42
#define RELAY_PIN           5

#elif defined(LILYGO_T_SIM7670G_S3)

#define CAN_RX_PIN          2
#define CAN_TX_PIN          1
#define CAN_ENABLE_PIN      48
#define RS485_TX_PIN        41
#define RS485_RX_PIN        40
#define RS485_ENABLE_PIN    42
#define RELAY_PIN           -1  // SIM7670G reset pin conflict

#else
#error "Not support this board"
#endif

// Define this on exactly one board. Comment it out on the responder board.
#define RS485_PING_PONG_INITIATOR


#ifdef RS485_PING_PONG_INITIATOR
#define PING_PONG_ROLE "Initiator (PING)"
#else
#define PING_PONG_ROLE "Responder (PONG)"
#endif

namespace
{
constexpr uint32_t PING_INTERVAL_MS = 1000;
constexpr size_t FRAME_BUFFER_SIZE = 32;

uint32_t nextPingAt = 0;
char frameBuffer[FRAME_BUFFER_SIZE];
size_t frameLength = 0;

void sendFrame(const char *frame)
{
    SerialRS485.println(frame);
    // Wait until the complete frame has left the UART before receiving again.
    SerialRS485.flush();
    Serial.printf("%s RS485 TX: %s\n", PING_PONG_ROLE, frame);
}

void toggleRelay()
{
#if RELAY_PIN >= 0
    digitalWrite(RELAY_PIN, digitalRead(RELAY_PIN) == HIGH ? LOW : HIGH);
#endif
}

void processFrame(const char *frame)
{
    Serial.printf("%s RS485 RX: %s\n", PING_PONG_ROLE, frame);

#ifdef RS485_PING_PONG_INITIATOR
    if (strcmp(frame, "PONG") == 0) {
        toggleRelay();
    }
#else
    if (strcmp(frame, "PING") == 0) {
        toggleRelay();
        sendFrame("PONG");
    }
#endif
}

void receiveFrames()
{
    while (SerialRS485.available() > 0) {
        const char value = static_cast<char>(SerialRS485.read());

        if (value == '\r') {
            continue;
        }

        if (value == '\n') {
            if (frameLength > 0) {
                frameBuffer[frameLength] = '\0';
                processFrame(frameBuffer);
                frameLength = 0;
            }
            continue;
        }

        if (frameLength < FRAME_BUFFER_SIZE - 1) {
            frameBuffer[frameLength++] = value;
        } else {
            // Discard an overlong frame and wait for its line terminator.
            frameLength = 0;
        }
    }
}
} // namespace

void setup()
{
    pinMode(RS485_ENABLE_PIN, OUTPUT);
    digitalWrite(RS485_ENABLE_PIN, HIGH);


#if RELAY_PIN >= 0
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
#endif

    Serial.begin(115200); // Set console baud rate

    // Modem use Serial1
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    // RS485 use Serial2
    SerialRS485.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

#ifdef RS485_PING_PONG_INITIATOR
    Serial.println("RS485 ping-pong: initiator (PING)");
#else
    Serial.println("RS485 ping-pong: responder (PONG)");
#endif
}

void loop()
{
    receiveFrames();

#ifdef RS485_PING_PONG_INITIATOR
    const uint32_t now = millis();
    if (static_cast<int32_t>(now - nextPingAt) >= 0) {
        sendFrame("PING");
        nextPingAt = now + PING_INTERVAL_MS;
    }
#endif

    delay(1);
}
