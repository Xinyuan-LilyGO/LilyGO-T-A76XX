/**
 * @file      CameraWebServer.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-08-05
 * @note      You must select partition scheme from the board menu that has at least 3MB APP space.
 *      Example from arduino-esp32 : https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer
 *
 *      This sketch is only applicable to the
 *      1. T-A7670X-S3-Standard
 *      2. T-SIM7000G-S3-Standard
 *      3. T-SIM7080G-S3-Standard
 *      4. T-SIM7670G-S3-Standard
 *      Other models are not supported
 *
 * */
#include "esp_camera.h"
#include <WiFi.h>
#include <Wire.h>
#include "utilities.h"

#if !defined(LILYGO_SIM7000G_S3_STAN) && !defined(LILYGO_SIM7080G_S3_STAN) \
    && !defined(LILYGO_SIM7670G_S3_STAN) && !defined(LILYGO_A7670X_S3_STAN)
#error "This sketch is only applicable to the T-A7670X-S3-Standard,T-SIM7000G-S3-Standard,T-SIM7080G-S3-Standard,T-SIM7670G-S3-Standard"
#endif

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "Your-WiFi-SSID";
const char *password = "Your-WiFi-Password";

extern void startCameraServer();

#define uS_TO_S_FACTOR      1000000ULL   // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP       180          // Time ESP32 will go to sleep (in seconds) 

// #define ENABLE_BATTERY_MON            // Enable voltage detection function
#define BATTERY_VOLTAGE_LOW     3000     // It will enter sleep mode if the voltage is lower than 3V

bool setCameraPower(bool enable)
{
    static bool started = false;
    if (!started) {
        Wire.begin(BOARD_SDA_PIN, BOARD_SCL_PIN);
        Wire.beginTransmission(0x28);
        if (Wire.endTransmission() != 0) {
            Serial.println("Camera power chip not found!");
            return false;
        }
    }
    started = true;

    uint8_t vdd[] = {
        0x04,   /*reg address*/
        0x7C,   /*REG03 DVDD1 1.496V*/
        0x7C,   /*REG04 DVDD2 1.496v*/
        0xCA,   /*REG05 AVDD1 3.0V*/
        0xB1    /*REG06 AVDD2 2.8V*/
    };
    Wire.beginTransmission(0x28);
    Wire.write(vdd, sizeof(vdd) / sizeof(vdd[0]));
    Wire.endTransmission();

    uint8_t control[] = {0x0E, 0x0F};
    if (!enable) {
        control[1] = 0x00;
    }
    Wire.beginTransmission(0x28);
    Wire.write(control, sizeof(control) / sizeof(control[0]));
    Wire.endTransmission();
    return true;
}


uint16_t get_battery_voltage()
{
    uint16_t vol = analogReadMilliVolts(BOARD_BAT_ADC_PIN) * 2;
    Serial.printf("Voltage:%u\n", vol);
    return vol;
}

void set_device_to_sleep()
{
    Serial.println("Enter esp32 goto deepsleep!");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
    while (1);
}


void setup()
{
    Serial.begin(115200);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef ENABLE_BATTERY_MON
    int tryCount = 4;
    while (tryCount--) {
        if (get_battery_voltage() < (BATTERY_VOLTAGE_LOW + 100)) {
            set_device_to_sleep();
        } else {
            Serial.println("Battery voltage is normal");
        }
        delay(500);
    }
#endif

    // Turn on the camera power
    if (!setCameraPower(true)) {
        while (1) {
            Serial.println("The camera PMIC failed to start.");
            delay(1000);
        };
    }

    // Disable power conservation and use maximum power
    pinMode(BOARD_POWER_SAVE_MODE_PIN, OUTPUT);
    digitalWrite(BOARD_POWER_SAVE_MODE_PIN, HIGH);

    // Pull down DTR to ensure the modem is not in sleep state
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    Serial.println("Power on the modem PWRKEY.");
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    //Ton >= 100 <= 500
    delay(300);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);


    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = CAMERA_Y2_PIN;
    config.pin_d1 = CAMERA_Y3_PIN;
    config.pin_d2 = CAMERA_Y4_PIN;
    config.pin_d3 = CAMERA_Y5_PIN;
    config.pin_d4 = CAMERA_Y6_PIN;
    config.pin_d5 = CAMERA_Y7_PIN;
    config.pin_d6 = CAMERA_Y8_PIN;
    config.pin_d7 = CAMERA_Y9_PIN;
    config.pin_xclk = CAMERA_XCLK_PIN;
    config.pin_pclk = CAMERA_PCLK_PIN;
    config.pin_vsync = CAMERA_VSYNC_PIN;
    config.pin_href = CAMERA_HREF_PIN;
    config.pin_sccb_sda = CAMERA_SIOD_PIN;
    config.pin_sccb_scl = CAMERA_SIOC_PIN;
    config.pin_pwdn = CAMERA_PWDN_PIN;
    config.pin_reset = CAMERA_RESET_PIN;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;  // for streaming
    //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if (config.pixel_format == PIXFORMAT_JPEG) {
        if (psramFound()) {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        } else {
            // Limit the frame size when PSRAM is not available
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    } else {
        // Best option for face detection/recognition
        config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
        config.fb_count = 2;
#endif
    }


    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);

        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1);        // flip it back
        s->set_brightness(s, 1);   // up the brightness just a bit
        s->set_saturation(s, -2);  // lower the saturation
    }
    // drop down frame size for higher initial frame rate
    if (config.pixel_format == PIXFORMAT_JPEG) {
        s->set_framesize(s, FRAMESIZE_QVGA);
    }

    // s->set_vflip(s, 1);

    WiFi.begin(ssid, password);
    WiFi.setSleep(false);

    Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    startCameraServer();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");


}

uint32_t interval = 0;
uint32_t sleep_count = 0;
void loop()
{

    // Enable voltage detection function
#ifdef ENABLE_BATTERY_MON
    if (millis() > interval) {
        uint16_t battery_voltage =  analogReadMilliVolts(BOARD_BAT_ADC_PIN) * 2;
        uint16_t soli_voltage =  analogReadMilliVolts(BOARD_SOLAR_ADC_PIN) * 2;
        if (battery_voltage < BATTERY_VOLTAGE_LOW) {
            // If the voltage is lower than the set value, it will automatically enter sleep mode.
            Serial.printf("battery_voltage:%u\n", battery_voltage);
            sleep_count++;
            if (sleep_count >= 10) {
                Serial.println("Power off modem");
                digitalWrite(BOARD_PWRKEY_PIN, HIGH);
                //Toff > 2500ms
                delay(2800);
                digitalWrite(BOARD_PWRKEY_PIN, LOW);
                Serial.println("Disable camera");
                esp_camera_deinit();
                Serial.println("Power off camera");

                // Power off the camera
                setCameraPower(false);

                Wire.end();

                Serial.println("Enter esp32 goto deepsleep!");
                esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
                delay(200);
                esp_deep_sleep_start();
                Serial.println("This will never be printed");
            }
        } else {
            sleep_count = 0;
        }
        Serial.printf("BATT:%umV SOLI:%umV\n", battery_voltage, soli_voltage);
        interval = millis() + 1000;
    }
#endif


    if (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    if (Serial.available()) {
        SerialAT.write(Serial.read());
    }
}
