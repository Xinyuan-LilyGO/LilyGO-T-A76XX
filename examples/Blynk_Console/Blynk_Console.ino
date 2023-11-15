/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  Attention! Please check out TinyGSM guide:
    https://tiny.cc/tinygsm-readme

  Change GPRS apm, user, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!

 *************************************************************/

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"


// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#include "utilities.h"
#include <Arduino.h>
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

BlynkTimer timer;
TinyGsm modem(SerialAT);

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";


BLYNK_WRITE(V0)
{
    if (param.asInt() == 1) {
        Serial.println("OFF");
        Blynk.logEvent("LED STATE", "OFF");
    } else {
        Serial.println("ON");
        Blynk.logEvent("LED STATE", "ON");
    }
}

void sendRandomData()
{
    //Send randomly generated fake data
    float h = random(60, 100);
    float t = random(60, 100);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, t);
}

void setup()
{
    Serial.begin(115200);
    // Turn on DC boost to power on the modem
#ifdef BOARD_POWERON_PIN
    pinMode(BOARD_POWERON_PIN, OUTPUT);
    digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

    // Set modem reset pin ,reset modem
    pinMode(MODEM_RESET_PIN, OUTPUT);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
    digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
    digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
    // Turn on modem
    pinMode(BOARD_PWRKEY_PIN, OUTPUT);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);
    delay(100);
    digitalWrite(BOARD_PWRKEY_PIN, HIGH);
    delay(1000);
    digitalWrite(BOARD_PWRKEY_PIN, LOW);

    // Set modem baud
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");
    delay(3000);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.init()) {
        Serial.println("Failed to restart modem, delaying 10s and retrying");
        return;
    }

    String name = modem.getModemName();
    Serial.println("Modem Name: " + name);


    Blynk.begin(BLYNK_AUTH_TOKEN, modem, apn, user, pass);
    // Setup a function to be called every two second
    timer.setInterval(2000L, sendRandomData);

    Serial.println("loop...");
}

void loop()
{
    Blynk.run();
    timer.run();
}

