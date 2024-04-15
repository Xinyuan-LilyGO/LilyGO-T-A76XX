#include <Arduino.h>

#define uS_TO_S_FACTOR      1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP       30          /* Time ESP32 will go to sleep (in seconds) */

#define BOARD                               "ESP32 Wrover Module"
#define MON                                 Serial
#define SIM                                 Serial1

#define PWRKEY_PIN                          (4)
#define MODEM_RESET_PIN                     (5)
#define POW_PIN                             (12)
#define MODEM_DTR_PIN                       (25)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (27)
#define MODEM_RING_PIN                      (33)
#define VBAT_PIN                            (35)
#define MODEM_RESET_LEVEL                   HIGH

const int SER_BAUD =                          115200;

void setup()
{
    // Serial Setup
    MON.begin(SER_BAUD);
    while (!MON) {
        delay(100);
    }
    MON.println("Serial Started");

    // SIM Serial
    SIM.begin(SER_BAUD, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
    while (!SIM) {
        delay(100);
    }

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Serial.println("Wakeup timer");
        int i = 30;
        while (i > 0) {
            Serial.printf("Modem will start in %d seconds\n", i);
            Serial.flush();
            delay(1000);
            i--;
        }
        Serial.println("TurnON Modem!");
    }


    // Release reset GPIO hold
    gpio_hold_dis((gpio_num_t)MODEM_RESET_PIN);

    // Set modem reset pin ,reset modem
    // The module will also be started during reset.
    Serial.println("Set Reset Pin.");
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);


    Serial.println("Power on the modem");
    pinMode(PWRKEY_PIN, OUTPUT);
    digitalWrite(PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(PWRKEY_PIN, HIGH);
    //Ton >= 100 <= 500
    delay(300);
    digitalWrite(PWRKEY_PIN, LOW);


    delay(20000);

    // Power off modem
    Serial.println("Power off the modem");
    digitalWrite(PWRKEY_PIN, HIGH);
    delay(2000);
    digitalWrite(PWRKEY_PIN, LOW);

    delay(5000);

    Serial.println("Set Rset Pin hold low");
    // Keep it low during the sleep period. If the module uses GPIO5 as reset,
    // there will be a pulse when waking up from sleep that will cause the module to start directly.
    // https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/85
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    gpio_hold_en((gpio_num_t)MODEM_RESET_PIN);
    gpio_deep_sleep_hold_en();


    Serial.println("Enter esp32 goto deepsleep!");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop()
{
}