/**
 * @file      ULP_Monitor.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-12-11
 * @note      esp32 ulp use arduino-esp32 v2.0.11 test , esp32s3 no support yet
 */

#include <Arduino.h>
#include "driver/rtc_io.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/adc.h"
#include "utilities.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

#include "esp32s3/ulp.h"
#if CONFIG_ESP32S3_ULP_COPROC_RESERVE_MEM != 0
#define ULP_COPROC_MEM CONFIG_ESP32S3_ULP_COPROC_RESERVE_MEM
#else
#define ULP_COPROC_MEM 100
#endif
#define JUMP_TO(label, value) M_BG(label, value)
#endif  /*CONFIG_IDF_TARGET_ESP32S3*/

#ifdef CONFIG_IDF_TARGET_ESP32
#include "esp32/ulp.h"
#define ULP_COPROC_MEM CONFIG_ULP_COPROC_RESERVE_MEM
#define JUMP_TO(label, value) M_BGE(label, value)

#if ESP_ARDUINO_VERSION == ESP_ARDUINO_VERSION_VAL(2,0,17)
#error "esp32 ulp does not work properly in version 2.0.17, tested in version 2.0.11 and it works fine, other versions can be tested by yourself"
#endif
#if ESP_ARDUINO_VERSION != ESP_ARDUINO_VERSION_VAL(2,0,11)
#warning "examples tested in version 2.0.11 and it works fine, other versions can be tested by yourself"
#endif
#endif  /*CONFIG_IDF_TARGET_ESP32*/


#if defined(LILYGO_T_A7670) || defined(LILYGO_T_A7608X)

// GPIO35 ADC1 CH7
#define ADC_CH 0
#define ADC_PAD_INDEX 7

#elif defined(LILYGO_T_SIM7670G_S3) || defined(LILYGO_T_A7608X_S3)      //No support

// GPIO4 ADC1 CH3
#define ADC_CH 0
#define ADC_PAD_INDEX 3

#if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3,0,0)
#error "Please update the esp32 core version to a version greater than v3.0.0, how to update, please see here https://docs.espressif.com/projects/arduino-esp32/en/latest/"
#endif

#error "No support yet."
#else
#error "No support !"
#endif

#define ULP_START_OFFSET 32

// ULP program
const ulp_insn_t program[] = {
    // Copy an immediate value into register: dest = imm
    I_MOVI(R0, 0),
    I_MOVI(R1, 0),
    I_MOVI(R2, 0),

    M_LABEL(1),       // do {
    I_MOVI(R0, 200),  // R0 = n * 1000 / 5, where n is the number of seconds to delay, 200 = 1 s
    M_LABEL(2),       // do {
    // since ULP runs at 8 MHz
    // 40000 cycles correspond to 5 ms (max possible delay is 65535 cycles or 8.19 ms)
    I_DELAY(40000),
    I_SUBI(R0, R0, 1),  // R0 --;

    JUMP_TO(2, 1),  // } while (R0 >= 1); ... jump to label 2 if R0 > 0

    I_ADC(R2, ADC_CH, ADC_PAD_INDEX),  // R2 ≈ adc_get_raw (ADC1_CHANNEL_X);
    I_ADDI(R1, R1, 1),                 // R1 ++
    // store sample count to RTC_SLOW_MEM [0]
    I_ST(R1, 0, 0),  // RTC_SLOW_MEM [1] = R1;
    I_ST(R2, 0, 1),  // RTC_SLOW_MEM [2] = R3;

    I_MOVR(R0, R2),  // R0 = R2

    // Compare wake-up analog value
    // What is the ADC value at the specified voltage based on actual measurement?
    JUMP_TO(3, 1900),  //R0 >= 1900 jump label 3

    I_MOVR(R0, R1),  // while (R1 < 100);
    M_BL(1, 1000),   // ... jump to label 1

    M_LABEL(3),  //LABEL 3
    I_WAKE(),    //WAKEUP ESP32

    I_HALT(),  //STOP ULP PROCESS
    // I_END(),
};

size_t size = sizeof(program) / sizeof(ulp_insn_t);

void setup()
{
    Serial.begin(115200);

    uint32_t adc_result = 0;
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        adc_result = RTC_SLOW_MEM[1] & 0xFFF;
        Serial.print("adc result = ");
        Serial.println(adc_result);
        Serial.println("===========================");
    	for (int i = 0; i <= 10; i++) {
        	Serial.printf("[%02d] %4i\n", i, RTC_SLOW_MEM[i] & 0xFFF);  // Only 12 LS bits are used for ADC conversion and (at least with some ESP32 boards) the bits are inverted
    	}
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }

    // Clear rtc memory
    memset(RTC_SLOW_MEM, 0, ULP_COPROC_MEM);

    // Set adc pin and adc attenuation to 11db
    analogSetPinAttenuation(BOARD_BAT_ADC_PIN, ADC_11db);

    // Load ULP program
    ESP_ERROR_CHECK(ulp_process_macros_and_load(ULP_START_OFFSET, program, &size));
    Serial.printf("ULP program occupied %u 32-bit words of RTC_SLOW_MEM, make sure you put your variables after this.\n", size);

    // Run ULP program
    ESP_ERROR_CHECK(ulp_run(ULP_START_OFFSET));
}

void loop()
{
// Disable this code and debug to print the simulated value of the ulp conversion
#if 1
  	// read battery voltage before deep sleep
    Serial.println("===========================");
    for (int i = 0; i <= 10; i++) {
        uint32_t battery_voltage = analogReadMilliVolts(BOARD_BAT_ADC_PIN);
        battery_voltage *= 2;
        Serial.printf("Battery:%umV\n", battery_voltage);
        ++i;
        delay(1000);
    }

    Serial.println("Sleeping...");

    delay(1000);

    // Set power domain keep open
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    if (ADC_CH == 0) {
    	// configure channel for ulp usage
    	adc1_config_channel_atten(static_cast<adc1_channel_t>(ADC_PAD_INDEX), ADC_ATTEN_DB_11);
    	adc1_config_width(ADC_WIDTH_BIT_12);
    	// adc value before ULP enable, fixes reading issues in deep sleep
    	adc1_get_raw(static_cast<adc1_channel_t>(ADC_PAD_INDEX));
    	adc1_ulp_enable();
    }

#if SOC_PM_SUPPORT_RTC_SLOW_MEM_PD
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
#endif

    // Set the ULP mode to wake up the main chip
    esp_sleep_enable_ulp_wakeup();

    // Entering sleep mode
    esp_deep_sleep_start();

#else
    Serial.println("===========================");
    for (int i = 0; i <= 10; i++) {
        Serial.printf("[%02d] %4i\n", i, RTC_SLOW_MEM[i] & 0xFFF);  // Only 12 LS bits are used for ADC conversion and (at least with some ESP32 boards) the bits are inverted
    }
    delay(1000);
#endif
}