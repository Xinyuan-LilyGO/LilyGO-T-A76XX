<h1 align = "center">T-PCIE-SIM7600 Cayenne</h1>

💦 ☀💨🌹 The sample code is based on the **[T-PCIE-SIM7670]()** board, which takes the data of the external BMP180 sensor and uploads it to **[Cayenne](https://cayenne.mydevices.com/)**.

![](../../../image/Cayenne/A7670.png)


# Steps:
1. Need to install the following dependencies
     - [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit-BMP085-Library)
     - [CayenneMQTT](https://github.com/myDevicesIoT/Cayenne-MQTT-Arduino)
     - [ArduinoJson](https://github.com/bblanchon/ArduinoJson) Use version 6.13.0
     - [TinyGSM](https://github.com/vshymanskyy/TinyGSM)
2. Set the Cayenne authentication info to match the authentication info from the Dashboard.If you don't have an account, then [create an account](https://accounts.mydevices.com/auth/realms/cayenne/login-actions/registration?client_id=cayenne-web-app&tab_id=01AaoLwmlng)
3. After logging in, you will see the dashboard, click **Add new** in the upper left,Choose **Generic ESP8266**,Then you can see the MQTT username, password and ID you need to use,Replace it with the definition in `main.cpp`
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
7. By default, GSM is used to upload sensor data. You can also change it to ESP32 to upload. You only need to host the **USE_GSM** macro in the sketch.

# Hardware
Connections for T-SIM7600 and BMP180
| T-PCIE pin  | BMP180 pin |
| ----------- | ---------- |
| 3.3         | VIN        |
| GND         | GND        |
| GPIO21(SDA) | SDA        |
| GPIO22(SCL) | SCL        |





