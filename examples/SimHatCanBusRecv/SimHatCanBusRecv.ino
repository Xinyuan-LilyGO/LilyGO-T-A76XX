/**
 * @file      SimHatCanBusRecv.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-05-09
 * @note      A7670G + external L76K GPS module solution is not supported.
 *            SIM7600 MODEM DTR PIN ,GPIO conflict, cannot be used
 *            SIM7670G RESET PIN,GPIO conflict, cannot be used
 */

// * @note      Requires external expansion board support https://lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v?variant=42200124752053
// * @note      Requires external expansion board support https://lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v?variant=42200124752053
// * @note      Requires external expansion board support https://lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v?variant=42200124752053
// * @note      Requires external expansion board support https://lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v?variant=42200124752053
// * @note      Requires external expansion board support https://lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v?variant=42200124752053

#include <Arduino.h>
#include "driver/twai.h"
#include "utilities.h"

#if defined(LILYGO_T_A7670)  || defined(LILYGO_SIM7600X) || \
    defined(LILYGO_SIM7000G) || defined(LILYGO_T_A7608X)

    #define CAN_RX_PIN          21
    #define CAN_TX_PIN          22
    #define CAN_ENABLE_PIN      0
    #define RS485_TX_PIN        19
    #define RS485_RX_PIN        18
    #define RS485_ENABLE_PIN    23
    #define RELAY_PIN           32

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
    #define RELAY_PIN           -1 //SIM7670G RESET PIN,GPIO conflict, cannot be used

#else
    #error "Not support this board"
#endif

// Interval:
#define POLLING_RATE_MS 1000

static bool driver_installed = false;

void setup()
{
    // Start Serial:
    Serial.begin(115200);

    pinMode(CAN_ENABLE_PIN, OUTPUT);
    digitalWrite(CAN_ENABLE_PIN, HIGH);

    // Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  //Look in the api-reference for other speed sets.
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    Serial.println("TWAI receive");

    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        Serial.println("Driver installed");
    } else {
        Serial.println("Failed to install driver");
        return;
    }

    // Start TWAI driver
    if (twai_start() == ESP_OK) {
        Serial.println("Driver started");
    } else {
        Serial.println("Failed to start driver");
        return;
    }

    // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
    uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
    if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
        Serial.println("CAN Alerts reconfigured");
    } else {
        Serial.println("Failed to reconfigure alerts");
        return;
    }

    // TWAI driver is now successfully installed and started
    driver_installed = true;
}

static void handle_rx_message(twai_message_t &message)
{
    // Process received message
    if (message.extd) {
        Serial.println("Message is in Extended Format");
    } else {
        Serial.println("Message is in Standard Format");
    }
    Serial.printf("ID: %x\nByte:", message.identifier);
    if (!(message.rtr)) {
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf(" %d = %02x,", i, message.data[i]);
        }
        Serial.println("");
    }
}

void loop()
{
    if (!driver_installed) {
        // Driver not installed
        delay(1000);
        return;
    }
    // Check if alert happened
    uint32_t alerts_triggered;
    twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
    twai_status_info_t twaistatus;
    twai_get_status_info(&twaistatus);

    // Handle alerts
    if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
        Serial.println("Alert: TWAI controller has become error passive.");
    }
    if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
        Serial.println("Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.");
        Serial.printf("Bus error count: %d\n", twaistatus.bus_error_count);
    }
    if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
        Serial.println("Alert: The RX queue is full causing a received frame to be lost.");
        Serial.printf("RX buffered: %d\t", twaistatus.msgs_to_rx);
        Serial.printf("RX missed: %d\t", twaistatus.rx_missed_count);
        Serial.printf("RX overrun %d\n", twaistatus.rx_overrun_count);
    }

    // Check if message is received
    if (alerts_triggered & TWAI_ALERT_RX_DATA) {
        // One or more messages received. Handle all.
        twai_message_t message;
        while (twai_receive(&message, 0) == ESP_OK) {
            handle_rx_message(message);
        }
    }
}
