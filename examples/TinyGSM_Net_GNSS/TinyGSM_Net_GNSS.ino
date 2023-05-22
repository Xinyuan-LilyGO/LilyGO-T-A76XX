/*
  FILE: AllFunctions.ino
  AUTHOR: Koby Hale
  PURPOSE: Test functionality
*/

#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialMon Serial
#define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    false
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true
#define TINY_GSM_TEST_TCP    false




// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "YourAPN";
const char gprsUser[] = "";
const char gprsPass[] = "";

// Server details to test TCP/SSL
const char server[]   = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";


#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

#define UART_BAUD    115200
#define PIN_DTR      25
#define PIN_TX       26
#define PIN_RX       27
#define PWR_PIN      4
#define BAT_ADC      35
#define BAT_EN       12
#define PIN_RI       33
#define RESET        5

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13


int counter, lastIndex, numberOfPieces = 24;
String pieces[24], input;


void setup()
{
    // Set console baud rate
    Serial.begin(115200);
    delay(10);

    Serial.println("setup...");


    pinMode(BAT_EN, OUTPUT);
    digitalWrite(BAT_EN, HIGH);

    //A7608 Reset
    pinMode(RESET, OUTPUT);
    digitalWrite(RESET, LOW);
    delay(100);
    digitalWrite(RESET, HIGH);
    delay(3000);
    digitalWrite(RESET, LOW);

    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, LOW);
    delay(100);
    digitalWrite(PWR_PIN, HIGH);
    delay(1000);
    digitalWrite(PWR_PIN, LOW);



    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS)) {
        Serial.println("SDCard MOUNT FAIL");
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "SDCard Size: " + String(cardSize) + "MB";
        Serial.println(str);
    }

    Serial.println("\nWait...");

    delay(1000);

    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.init()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
        return;
    }

    String name = modem.getModemName();
    delay(500);
    Serial.println("Modem Name: " + name);

    String modemInfo = modem.getModemInfo();
    delay(500);





}

void loop()
{



    /*
         while (1) {
             while (Serial.available()) {
                 SerialAT.write(Serial.read());
             }
             while (SerialAT.available()) {
                 Serial.write(SerialAT.read());
             }
         }*/


    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.init()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
    }

    String name = modem.getModemName();
    delay(500);
    Serial.println("Modem Name: " + name);

    String modemInfo = modem.getModemInfo();
    delay(500);



#if TINY_GSM_TEST_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem.getSimStatus() != 3 ) {
        modem.simUnlock(GSM_PIN);
    }
#endif

#if TINY_GSM_TEST_GPRS && defined TINY_GSM_MODEM_XBEE
    // The XBee must run the gprsConnect function BEFORE waiting for network!
    modem.gprsConnect(apn, gprsUser, gprsPass);
#endif


#if TINY_GSM_TEST_GPRS
    DBG("Waiting for network...");
    if (!modem.waitForNetwork()) {
        delay(10000);
        return;
    }

    if (modem.isNetworkConnected()) {
        DBG("Network connected");
    }
#endif


#if TINY_GSM_TEST_GPRS
    DBG("Connecting to", apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        delay(10000);
        return;
    }

    bool res = modem.isGprsConnected();
    DBG("GPRS status:", res ? "connected" : "not connected");

    String ccid = modem.getSimCCID();
    DBG("CCID:", ccid);

    String imei = modem.getIMEI();
    DBG("IMEI:", imei);

    String cop = modem.getOperator();
    DBG("Operator:", cop);

    IPAddress local = modem.localIP();
    DBG("Local IP:", local);

    int csq = modem.getSignalQuality();
    DBG("Signal quality:", csq);

#endif
#if TINY_GSM_TEST_TCP
    TinyGsmClient client(modem, 0);
    const int     port = 80;
    DBG("Connecting to", server);
    if (!client.connect(server, port)) {
        DBG("... failed");
    } else {
        // Make a HTTP GET request:
        client.print(String("GET ") + resource + " HTTP/1.0\r\n");
        client.print(String("Host: ") + server + "\r\n");
        client.print("Connection: close\r\n\r\n");

        // Wait for data to arrive
        uint32_t start = millis();
        while (client.connected() && !client.available() &&
                millis() - start < 30000L) {
            delay(100);
        };

        // Read data
        start          = millis();
        char logo[1204] = {
            '\0',
        };
        int read_chars = 0;
        while (client.connected() && millis() - start < 10000L) {
            while (client.available()) {
                logo[read_chars]     = client.read();
                logo[read_chars + 1] = '\0';
                read_chars++;
                start = millis();
            }
        }


        //Serial.println(logo);
        DBG("#####  RECEIVED:", strlen(logo), "CHARACTERS");
        client.stop();
    }
#endif


#if TINY_GSM_TEST_GPRS
    modem.gprsDisconnect();
    if (!modem.isGprsConnected()) {
        DBG("GPRS disconnected");
    } else {
        DBG("GPRS disconnect: Failed.");
    }
#endif

#if TINY_GSM_TEST_GPS

    //Disable gnss
    modem.sendAT("+CGNSSPWR=0");
    modem.waitResponse(10000L);

    //Enable gnss
    modem.sendAT("+CGNSSPWR=1");
    modem.waitResponse(10000L);

    //Wait gnss start.
    SerialMon.print("\tWait GPS reday.");
    while (modem.waitResponse(1000UL, "+CGNSSPWR: READY!") != 1) {
        SerialMon.print(".");
    }
    SerialMon.println();

    //Set gnss mode use GPS.
    modem.sendAT("+CGNSSMODE=1");
    modem.waitResponse(10000L);


    float parameter1,  parameter2;
    char buf[16];
    while (1) {
        if (modem.getGPS(&parameter1, &parameter2)) {
            modem.sendAT(GF("+CGNSSINFO"));
            if (modem.waitResponse(GF(GSM_NL "+CGNSSINFO:")) == 1) {
                String res = modem.stream.readStringUntil('\n');
                String lat = "";
                String n_s = "";
                String lon = "";
                String e_w = "";
                res.trim();
                lat = res.substring(8, res.indexOf(',', 8));
                n_s = res.substring(19, res.indexOf(',', res.indexOf(',', 19)));
                lon = res.substring(21, res.indexOf(',', res.indexOf(',', 21)));
                e_w = res.substring(33, res.indexOf(',', res.indexOf(',', 33)));
                delay(100);
                Serial.println("****************GNSS********************");
                Serial.printf("lat:%s %s\n", lat, n_s);
                Serial.printf("lon:%s %s\n", lon, e_w);
                float flat = atof(lat.c_str());
                float flon = atof(lon.c_str());
                flat = (floor(flat / 100) + fmod(flat, 100.) / 60) *
                       (n_s == "N" ? 1 : -1);
                flon = (floor(flon / 100) + fmod(flon, 100.) / 60) *
                       (e_w == "E" ? 1 : -1);
                Serial.print("Latitude:"); Serial.println(flat);
                Serial.print("Longitude:"); Serial.println(flon);
            }
            break;
        } else {
            Serial.print("getGPS ");
            Serial.println(millis());
        }
        delay(2000);
    }

    //Disable gnss
    modem.sendAT("+CGNSSPWR=0");
    modem.waitResponse(10000L);

#endif


#if TINY_GSM_POWERDOWN
    // Try to power-off (modem may decide to restart automatically)
    // To turn off modem completely, please use Reset/Enable pins
    Serial.println("Poweroff.");
    DBG("Poweroff.");
    modem.poweroff();

#endif

    // Test is complete Set it to sleep mode
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();

    // Do nothing forevermore
    while (true) {
        modem.maintain();
    }
}
