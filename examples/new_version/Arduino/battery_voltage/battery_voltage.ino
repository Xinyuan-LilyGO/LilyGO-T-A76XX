/*
 * This sketch is to get the current battery voltage
 */

#include "Arduino.h"
#include "esp_adc_cal.h"

#define BAT_ADC    35
#define BAT_EN       12

float Voltage = 0.0;
uint32_t readADC_Cal(int ADC_Raw);

void setup()
{
    Serial.begin(115200);
    pinMode(BAT_EN, OUTPUT);
    digitalWrite(BAT_EN, HIGH);
}

void loop()
{

    Voltage = (readADC_Cal(analogRead(BAT_ADC))) * 2;
    Serial.printf("%.2fV", Voltage / 1000.0); // Print Voltage (in V)
    delay(500);
}

uint32_t readADC_Cal(int ADC_Raw)
{
    esp_adc_cal_characteristics_t adc_chars;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}