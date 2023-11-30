<h1 align = "center">🌟LilyGO T-A76XX🌟</h1>

[![PlatformIO CI](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/actions/workflows/platformio.yml/badge.svg)](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/actions/workflows/platformio.yml)

# News

- The TinyGSM used in the example is a [fork](https://github.com/lewisxhe/TinyGSM) to support A7670,A7608,SIM7672,If you use the master branch, the compilation will not go smoothly
- A7670G/A7670E/A7670SA are collectively referred to as A7670X
- A7608SA-H/A7608E-H,A7608E,A7608SA are collectively referred to as A7608X

# 1️⃣Product

| Product(PinMap)    | SOC              | Flash | PSRAM     | Schematic                                        |
| ------------------ | ---------------- | ----- | --------- | ------------------------------------------------ |
| [T-A7670X][1]      | ESP32-WROVER-E   | 4MB   | 8MB(QSPI) | [schematic](./schematic/T-A7670X-V1.1.pdf)       |
| [T-Call-A7670X][2] | ESP32-WROVER-E   | 4MB   | 8MB(QSPI) | [schematic](./schematic/T-Call-A7670-V1.0.pdf)   |
| [T-A7608][3]       | ESP32-WROVER-E   | 4MB   | 8MB(QSPI) | [schematic](./schematic/T-A7608X-V1.0.pdf)       |
| [T-A7608-DC-S3][4] | ESP32-S3-WROOM-1 | 16MB  | 8MB(OPI)  | [schematic](./schematic/T-A7608X-DC-S3-V1.0.pdf) |
| [T-A7608-S3][5]    | ESP32-S3-WROOM-1 | 16MB  | 8MB(OPI)  | [schematic](./schematic/T-A7608-S3-V1.0.pdf)     |
| [T-SIM7672G-S3][6] | ESP32-S3-WROOM-1 | 16MB  | 8MB(OPI)  | [schematic](./schematic/T-SIM7672G-S3-V1.0.pdf)  |

[1]: https://www.lilygo.cc/products/t-sim-a7670e
[2]: https://www.lilygo.cc
[3]: https://www.lilygo.cc/products/t-a7608e-h
[4]: https://www.lilygo.cc
[5]: https://www.lilygo.cc
[6]: https://www.lilygo.cc

## 2️⃣Examples

| Example            | [T-A7670X][1]     | [T-Call-A7670X][2] | [T-SIM7672G-S3][6] | [T-A7608][3]    | [T-A7608-DC-S3][4] | [T-A7608-S3][5] |
| ------------------ | ----------------- | ------------------ | ------------------ | --------------- | ------------------ | --------------- |
| ATdebug            | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| Blynk_Console      | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| GPSShield          | ✅ (Only T-A7670G) | ❌   (Can't run)    | ❌   (Can't run)    | ❌   (Can't run) | ❌   (Can't run)    | ❌   (Can't run) |
| GPS_BuiltIn        | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| GPS_NMEA_Parse     | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| HttpClient         | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| HttpsClient        | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| MqttClient         | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| ReadBattery        | ✅                 | ❌   (Can't run)    | ✅                  | ✅               | ❌   (Can't run)    | ✅               |
| DeepSleep          | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| ModemSleep         | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| VoiceCalls         | ✅                 | ✅                  | ❌   (No support)   | ✅               | ✅                  | ✅               |
| SDCard             | ✅                 | ❌   (Can't run)    | ✅                  | ✅               | ✅                  | ✅               |
| SerialRS485        | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| SendSMS            | ✅                 | ✅                  | ❌   (No support)   | ✅               | ✅                  | ✅               |
| LBSExample         | ✅                 | ✅                  | ❌   (No support)   | ✅               | ✅                  | ✅               |
| Network            | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| MqttsBuiltlnAuth   | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| MqttsBuiltlnSSL    | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| MqttsBuiltlnAWS    | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| MqttsBuiltlnHivemq | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| HttpsBuiltlnGet    | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| HttpsBuiltlnPost   | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |
| HttpsOTAUpgrade    | ✅                 | ✅                  | ✅                  | ✅               | ✅                  | ✅               |

# Quick start ⚡

## 3️⃣ PlatformIO Quick Start (Recommended)

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGO-T-A76XX` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Select the board name you want to use in `default_envs` and uncomment it.
8. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
9. Click the (✔) symbol in the lower left corner to compile
10. Connect the board to the computer USB
11. Click (→) to upload firmware
12. Click (plug symbol) to monitor serial output
13. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

## 4️⃣ Arduino IDE Quick Start

- It is recommended to use platformio without cumbersome steps

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino ESP32 2.0.5 or above](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
3. Copy all folders in [lib folder](./lib/)  to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
4. Open ArduinoIDE  ,`Tools` , Look at the picture to choose
    | [T-A7670X][1]/[T-Call-A7670X][2]/[T-A7608][3] | [T-A7608-DC-S3][4]/[T-A7608-DC-S3][4]/[T-A7608-S3][5]/[T-SIM7672G-S3][6] |
    | --------------------------------------------- | ------------------------------------------------------------------------ |
    | ![esp32dev](images/esp32dev.jpg)              | ![esp32s3dev](images/esp32s3dev.jpg)                                     |
5. `LilyGO-T-A76XX` folder -> `examples` -> `Choose the appropriate example from the list above`
6. Open the corresponding board macro definition above [utilities.h](./examples/ATdebug/utilities.h)
7. Select `Port`
8. Click `upload` , Wait for compilation and writing to complete
9. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

### Writing as a beginner

- When opening the Arduino IDE, you will be prompted that there are new libraries that can be updated. Please do not click update. There is no guarantee that it can be compiled, run, etc. after the update. Make sure that the board is running normally before deciding whether to update. You have to Know that problems may occur after updating due to library differences, or the default configuration (such as TinyGSM) will be overwritten.
- If it runs abnormally after the update, please follow Article 4 of the Arduino Quick Start to delete the directory in libraries, and drag the folder in lib into the libraries directory again.

## 5️⃣ Notes for new Users <T-A7670X/T-A7608X>

1. The A7670G does not have a built-in GPS modem, regardless of whether the motherboard contains a GPS connector.

- **The following does not apply to T-Call-A7670**

1. Pin VIN is a USB input, if a battery is being used to power the device there will be no voltage output from VIN meaning 3.3v is the only power output.
2. When using the built-in battery pack the on/off switch will function as normal, supplying/cutting off power to the board, However, if an external battery pack is used and attached to the VBAT pin the on/off switch will be bypassed meaning the only way to shut off will be to disconnect the batteries.
3. On/off switch is for battery use only, it will not function when plugged into USB.
4. Recommended solar panels are 4.4v to 6v **DO NOT EXCEED OR BOARD MAY BE DAMAGED** 
5. When an SD card in you can not upload software in Arduino IDE since the SD card uses IO2 as CS, the SD card must be removed when uploading a new sketch.
6. The `T-A7670G` modem does not contain a GPS positioning component and requires an additional external GPS positioning module to use the positioning function. [LilyGo T-A7670G with GPS](https://www.lilygo.cc/products/t-sim-a7670e?variant=43043706077365) uses [Quectel L76K](https://www.quectel.com/cn/product/gnss-l76k) as the **T-A7670G** auxiliary positioning function, which requires a part of the IO, please refer to the [GPSShield example](./examples/GPSShield/) , How to identify whether you have an external GPS module please see [#issues56](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/56#issuecomment-1672628977)

# 6️⃣ FAQ

- Can't upload any sketch，Please enter the upload mode manually.
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button (If there is no BOOT button, you need to use wires to connect GND and IO0 together.)
   3. Press RST button
   4. Release the RST button
   5. Release the BOOT button (If there is no BOOT button, disconnect IO0 from GND.)
   6. Upload sketch
- Upgrade modem firmware
   1. [T-A76XX Upgrade docs](./docs/update_fw.md)
- How do I connect the antenna correctly?
   1. Check the silk screen on the board. **GPS** stands for GPS antenna. Only active GPS antenna can be connected here.
   2. **SIM** or **MAIN**, this is the main antenna interface of LTE
   3. **AUX** This is the diversity antenna for LTE, used to enhance the signal
- Solar input voltage range?
   1. **4.4 ~ 6V** , As long as the voltage matches, the solar panel power is not limited
- Can't get location?
   1. Please write the compiled firmware for testing. Please see [here](./firmware/README.MD) for details.
- Where can I access solar energy to charge the panel?
   1. Some boards(T-A7670,T-A7608,T-A7670-S3,T-A7608-S3) have their own solar battery input interface, you just need to connect the solar panel correctly according to the polarity.
   2. If the board has an external VBAT pin, you can connect the solar rechargeable battery input to VBAT. Please be careful not to exceed the battery supply voltage, which is usually 4.2V.
   3. If there is no VBAT, you can also connect it to the VBUS Pin, which is the input pin for USB power supply. Connecting it to VBUS will share the 5V of the USBC. Please note that when connecting an external charger, disconnect the USBC or disconnect the solar energy. enter
- How to use voice call example?
   1. Voice calls require external welding of the condenser microphone and speaker. Generally, the board silk screen is marked SPK. The speaker needs to be welded, and the MIC silk screen position needs to weld the condenser microphone.
- SIM7672G sendSMS and VoiceCall?
   1. Although the manual of SIM7672G states that it has the functions of making voice calls and sending text messages, the current firmware does not support it.


# 7️⃣Resource

1. SIMCOM official website document center
   - [SIMCOM official A7670X All Datasheet](https://cn.simcom.com/product/A7670X.html)
   - [SIMCOM official A7608X All Datasheet](https://cn.simcom.com/product/A7608X-H-E_SA.html)
   - [SIMCOM official SIM7672 All Datasheet](https://en.simcom.com/product/SIM7672.html)
2. A7670/A7608-Datasheet
   - [A76xx AT Command](./datasheet/A76XX/A76XX_Series_AT_Command_Manual_V1.09.pdf)
   - [A76xx MQTT(S) Application](./datasheet/A76XX/A76XX%20Series_MQTT(S)_Application%20Note_V1.02.pdf)
   - [A76xx HTTP(S) Application](./datasheet/A76XX/A76XX%20Series_HTTP(S)_Application%20Note_V1.02.pdf)
   - [A76xx GNSS Application](./datasheet/A76XX/A76XX%20Series_GNSS_Application%20Note_V1.02.pdf)
   - [A76xx FTP Application](./datasheet/A76XX/A76XX%20Series_FTP(S)_Application%20Note_V1.02.pdf)
   - [A76xx LBS Application](./datasheet/A76XX/A76XX%20Series_LBS_Application%20Note_V1.02.pdf)
   - [A76xx SSL Application](./datasheet/A76XX/A76XX%20Series_SSL_Application%20Note_V1.02.pdf)
   - [A76xx Sleep Application](./datasheet/A76XX/A76XX%20Series_Sleep%20Mode_Application%20Note_V1.02.pdf)
   - [A76xx Hardware Design manual](./datasheet/A76XX/A7670C_R2_硬件设计手册_V1.06.pdf)
   - [A76xx TCPIP Application](./datasheet/A76XX/A76XX%20Series_TCPIP_Application%20Note_V1.02.pdf)
3. SIM7672-Datasheet
   - [SIM7672 Hardware Design manual](./datasheet/SIM7672G/SIM7672X_Series_Hardware_Design_V1.02.pdf)
   - [SIM7672 AT Command](./datasheet/SIM7672G/SIM767XX%20Series_AT_Command_Manual_V1.01.pdf)
4. Schematic
   - [T-A7608-S3 Schematic](./schematic/T-A7608-S3-V1.0.pdf)
   - [T-A7608X-DC-S3 Schematic](./schematic/T-A7608X-DC-S3-V1.0.pdf)
   - [T-A7608X Schematic](./schematic/T-A7608X-V1.0.pdf)
   - [T-A7670X Schematic](./schematic/T-A7670X-V1.1.pdf)
   - [T-Call-A7670 Schematic](./schematic/T-Call-A7670-V1.0.pdf)
   - [T-SIM7672-S3 Schematic](./schematic/T-SIM7672-S3-V1.0.pdf)
5. Dimensions
   - [T-A7608-S3 DWG](./dimensions/T-A7608-S3-V1.0.dwg)
   - [T-A7608X-DC-S3 DWG](./dimensions/T-A7608X-DC-S3-V1.0.dwg)
   - [T-A7608X DWG](./dimensions/T-A7608X-V1.0.dwg)
   - [T-A7670X DWG](./dimensions/T-A7670X-V1.1.dwg)
   - [T-Call-A7670 DWG](./dimensions/T-Call-A7670-V1.0.dwg)
   - [T-SIM7672-S3 DWG](./dimensions/T-SIM7672-S3-V1.0.dwg)







