<div align="center" markdown="1">
  <img src=".github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align = "center">🌟LilyGo-Modem-Series🌟</h1>

[![PlatformIO CI](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/actions/workflows/platformio.yml/badge.svg)](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/actions/workflows/platformio.yml)

# News

- 2025/07/10 : The examples have completed most of the support for SIM7000G. The non-encrypted MQTT is currently waiting for a fix, and the encrypted MQTTS access has been verified.
- 2025/07/09 : Completed support for SIM7600 series
- The TinyGSM used in the example is a [fork](https://github.com/lewisxhe/TinyGSM) to support A7670,A7608,SIM7672G,SIM7670G, If you use the master branch, the compilation will not go smoothly
- A7670G/A7670E/A7670SA/A7672G are collectively referred to as A7670X
- A7608SA-H/A7608E-H,A7608E,A7608SA are collectively referred to as A7608X
- **SIM7670G** uses the **Qualcomm** platform, **A7670x** uses the **Asrmicro** platform.
- The usage methods of A7670/A7670 R2 are exactly the same, but the internal chip manufacturing process of the module is different.
- **A7670E-LNXY-UBL** this version does not support voice and SMS functions.

# 1️⃣Product

| Product(PinMap)    | SOC              | Flash    | PSRAM     |
| ------------------ | ---------------- | -------- | --------- |
| [T-A7670X][1]      | ESP32-WROVER-E   | 4MB      | 8MB(QSPI) |
| [T-Call-A7670X][2] | ESP32-WROVER-E   | 4MB      | 8MB(QSPI) |
| [T-A7608][4]       | ESP32-WROVER-E   | 4MB      | 8MB(QSPI) |
| [T-PCIE-A7670][8]  | ESP32-WROVER-E   | 4MB/16MB | 8MB(QSPI) |
| [T-SIM7600][10]    | ESP32-WROVER-E   | 4MB/16MB | 8MB(QSPI) |
| [T-SIM7000G][11]   | ESP32-WROVER-E   | 4MB/16MB | 8MB(QSPI) |
| [T-A7608-S3][6]    | ESP32-S3-WROOM-1 | 16MB     | 8MB(OPI)  |
| [T-SIM7670G-S3][7] | ESP32-S3-WROOM-1 | 16MB     | 8MB(OPI)  |
| [T-ETH-ELite][9]   | ESP32-S3-WROOM-1 | 16MB     | 8MB(OPI)  |

- For applications that do not require voice and SMS, it is recommended to use [T-SIM7670G-S3](7)

<!-- | [T-A7608-V2][8]         | ESP32-WROVER-E   | 4MB      | 8MB(QSPI) | [schematic](./schematic/A7608-ESP32-V2.pdf) | -->
<!-- | [T-Call-A7670X-V1.1][3] | ESP32-WROVER-E   | 4MB      | 8MB(QSPI) | [schematic](./schematic/T-Call-A7670-V1.1.pdf)   | -->
<!-- | [T-A7608-DC-S3][5]      | ESP32-S3-WROOM-1 | 16MB     | 8MB(OPI)  | [schematic](./schematic/T-A7608X-DC-S3-V1.0.pdf) | -->

[1]: https://www.lilygo.cc/products/t-sim-a7670e
[2]: https://www.lilygo.cc/products/t-call-v1-4
[3]: https://lilygo.cc/products/t-a7608e-h?variant=42860532433077
[4]: https://www.lilygo.cc/products/t-a7608e-h
[5]: https://lilygo.cc/products/t-a7608e-h
[6]: https://lilygo.cc/products/t-a7608e-h?variant=43932699033781
[7]: https://www.lilygo.cc/products/t-sim-7670g-s3
[8]: https://lilygo.cc/products/a-t-pcie?variant=42335922094261
[9]: https://lilygo.cc/products/t-eth-elite-1?variant=44498205049013
[10]: https://lilygo.cc/products/t-sim7600
[11]: https://lilygo.cc/products/t-sim7000g

## 2️⃣Examples

| Example                         | [T-A7670X][1]     | [T-Call-A7670X][2] | [T-SIM767XG-S3][6] | [T-A7608][3]    | [T-PCIE-A767X][8] | [T-A7608-S3][5] |
| ------------------------------- | ----------------- | ------------------ | ------------------ | --------------- | ----------------- | --------------- |
| ATdebug                         | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| Blynk_Console                   | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| GPSShield                       | ✅ (Only T-A7670G) | ❌   (Can't run)    | ❌   (Can't run)    | ❌   (Can't run) | ❌   (Can't run)   | ❌   (Can't run) |
| GPS_BuiltIn                     | ✅ (Except A7670G) | ✅(Except A7670G)   | ✅                  | ✅               | ✅                 | ✅               |
| GPS_BuiltInEx                   | ✅ (Except A7670G) | ✅(Except A7670G)   | ✅                  | ✅               | ✅                 | ✅               |
| GPS_NMEA_Parse                  | ✅ (Except A7670G) | ✅(Except A7670G)   | ✅                  | ✅               | ✅                 | ✅               |
| GPS_NMEA_Output                 | ✅ (Except A7670G) | ✅(Except A7670G)   | ✅                  | ✅               | ✅                 | ✅               |
| GPS_Acceleration                | ✅ (Except A7670G) | ✅(Except A7670G)   | ✅                  | ✅               | ✅                 | ✅               |
| HttpClient                      | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| TCPClientMultiple               | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| HttpsClient                     | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttClient                      | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| TextToSpeech                    | ✅                 | ✅                  | ❌   (Can't run)    | ✅               | ✅                 | ✅               |
| ModemPlayMusic                  | ✅                 | ✅                  | ❌   (Can't run)    | ✅               | ✅                 | ✅               |
| SecureClient                    | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| ReadBattery                     | ✅                 | ❌   (Can't run)    | ✅                  | ✅               | ❌   (Can't run)   | ✅               |
| DeepSleep                       | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| ModemSleep                      | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| ModemPowerOff                   | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| VoiceCalls                      | ✅                 | ✅                  | ❌[5]               | ✅               | ✅                 | ✅               |
| SDCard                          | ✅                 | ❌   (Can't run)    | ✅                  | ✅               | ✅                 | ✅               |
| SerialRS485                     | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| SendSMS                         | ✅                 | ✅                  | ❌[4]               | ✅               | ✅                 | ✅               |
| ReadSMS                         | ✅                 | ✅                  | ❌[4]               | ✅               | ✅                 | ✅               |
| SendLocationFromSMS             | ✅                 | ✅                  | ❌[4]               | ✅               | ✅                 | ✅               |
| SendLocationFromSMS_Use_TinyGPS | ✅                 | ✅                  | ❌[4]               | ✅               | ✅                 | ✅               |
| LBSExample                      | ✅                 | ✅                  | ❌   (No support)   | ✅               | ✅                 | ✅               |
| Network                         | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnAuth                | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnSSL                 | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnNoSSL               | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnAWS                 | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnHivemq              | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnEMQX                | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| MqttsBuiltlnWill                | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| HttpsBuiltlnGet                 | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| HttpsBuiltlnPost                | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| HttpsOTAUpgrade                 | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| HP303BSensor*                   | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| SPIExample*                     | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |
| PowerMonitoring                 | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatAccelerometer[3]          | ✅                 | ❌                  | ❌(No support)[2]   | ✅[1]            | ❌                 | ✅               |
| SimHatCurrentSensor[3]          | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatOneWireSensor[3]          | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatRelay[3]                  | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatRS485[3]                  | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatCanBusRecv[3]             | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| SimHatCanBusSend[3]             | ✅                 | ❌                  | ❌(No support)[2]   | ✅               | ❌                 | ✅               |
| ULP_Monitor                     | ✅                 | ✅                  | ❌                  | ✅               | ✅                 | ❌               |
| Traccar(GPS Tracking)           | ✅                 | ✅                  | ✅                  | ✅               | ✅                 | ✅               |


- HP303BSensor,SPIExample it only demonstrates how to define and use SPI and I2C. Please confirm the pin connection and IO definition before use.
- [1] T-A7608-ESP32 Conflict with Solar ADC
- [2] The relay driver conflicts with the board RST and cannot work
- [3] Requires external expansion board support [T-SimHat](https://www.lilygo.cc/products/lilygo%C2%AE-t-simhat-can-rs485-relay-5v)
- [4] SIM7670G - `SIM7670G-MNGV 2374B04` version supports SMS function, but it requires the operator base station to support SMS Over SGS service to send, otherwise it will be invalid
- [5] All versions of SIM7670G do not support voice

# Quick start ⚡

## 3️⃣ PlatformIO Quick Start (Recommended)

1. For users using the ESP32 version (non ESP32S3 version), please install the **CH9102 USB bridge** driver for the first time.
   - [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html)
   - [Mac OS](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
2. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
3. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
4. After the installation is complete, you need to restart `VisualStudioCode`
5. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGO-T-A76XX` directory
6. Wait for the installation of third-party dependent libraries to complete
7. Click on the `platformio.ini` file, and in the `platformio` column
8. Select the board name you want to use in `default_envs` and uncomment it.
9. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
10. Click the (✔) symbol in the lower left corner to compile
11. Connect the board to the computer USB-C , Micro-USB is used for module firmware upgrade
12. Click (→) to upload firmware
13. Click (plug symbol) to monitor serial output

## 4️⃣ Arduino IDE Quick Start

- It is recommended to use platformio without cumbersome steps
  
1. For users using the ESP32 version (non ESP32S3 version), please install the **CH9102 USB bridge** driver for the first time.
   - [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html)
   - [Mac OS](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
2. Install [Arduino IDE](https://www.arduino.cc/en/software)
3. Install [Arduino ESP32 2.0.5 or above](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
4. Copy all folders in [lib folder](./lib/)  to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
5. Open ArduinoIDE  ,`Tools` , Look at the picture to choose , **If you use the ESP32-S3 controller and need to use serial, then USB CDC ON Boot needs to be set to Enabled**

    | [T-A7670X][1]/[T-Call-A7670X][2]/[T-A7608][4]/[T-PCIE-A7670][8]/[T-SIM7600][10]/[T-SIM7000G][11] | [T-A7608-S3][6]/[T-SIM767XG-S3][7]   |
    | ------------------------------------------------------------------------------------------------ | ------------------------------------ |
    | ![esp32dev](images/esp32dev.jpg)                                                                 | ![esp32s3dev](images/esp32s3dev.jpg) |

6. `LilyGO-T-A76XX` folder -> `examples` -> `Choose the appropriate example from the list above`
7. Open the corresponding board macro definition above [utilities.h](./examples/ATdebug/utilities.h) , For example, if you use LILYGO-A7670X, you need to change ` // #define LILYGO_T_A7670` to ` #define LILYGO_T_A7670`

   ![board_select](./images/board_select.jpg)

8. Connect the board to the computer USB-C , Micro-USB is used for module firmware upgrade
9. Select `Port`
10. Click `upload` , Wait for compilation and writing to complete

> \[!IMPORTANT]
>
> 1. Unable to upload any code? Please see the FAQ below
> 2. When opening the Arduino IDE, you will be prompted that there are new libraries that can be updated. Please do not click update. There is no guarantee that it can be compiled, run, etc. after the update. Make sure that the board is running normally before deciding whether to update. You have to Know that problems may occur after updating due to library differences, or the default configuration (such as TinyGSM) will be overwritten.
> 3. If it runs abnormally after the update, please follow Article 4 of the Arduino Quick Start to delete the directory in libraries, and drag the folder in lib into the libraries directory again.

# 5️⃣ FAQ

- **ESP32 users, not ESP32S3 users, ESP32 models GPIO33 and above can only be set as input, not output**

- **ESP32 (V1.x) version [T-A7670X][1]/[T-A7608X][4]  known issue**
  - When using battery power mode, BOARD_POWERON_PIN (IO12) must be set to a high level after ESP32 starts, otherwise a reset will occur.
  - Modem cannot be put into sleep mode when powered by USB-C. For a solution, see [here](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/blob/main/examples/ModemSleep/README.MD)

- **Can't turn on the phone after connecting the battery for the first time?**
  - This is due to the characteristics of the onboard battery over-discharge and over-charge chip. It cannot be turned on when the battery is connected for the first time. This can be solved by inserting a USB charger or reconnecting the battery. For details, please see the remarks on [page 4](https://www.lcsc.com/datasheet/lcsc_datasheet_2010160933_Shenzhen-Fuman-Elec-DW06D_C82123.pdf) of the datasheet.

- **GPS not working?**
  - See [issue/137](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/137)

- **How to identify whether an external GPS module is installed**
  - See [issue/56](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/56#issuecomment-1672628977)

- **VOLTE FAQ**
  - See [issue/115](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/115)

- **Network registration denied?**
  - When the network registration is refused, please check whether the APN is set correctly. For details, please refer to [issue/104](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/104)

- **Can't use 2G(GSM)?**
  - LilyGo has launched a separate 4G(LTE) version that can only use 4G(LTE) network. Please check whether it is a separate 4G(LTE) version during ordering.

- **How to use voice call example?**
  - Voice calls require external welding of the condenser microphone and speaker. Generally, the board silk screen is marked SPK. The speaker needs to be welded, and the MIC silk screen position needs to weld the condenser microphone. , **T-A7670** the microphone and speaker connections are located [issue/130](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/130)

- **Can't upload any sketch，Please enter the upload mode manually.**
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button (If there is no BOOT button, you need to use wires to connect GND and IO0 together.)
   3. Press RST button
   4. Release the RST button
   5. Release the BOOT button (If there is no BOOT button, disconnect IO0 from GND.)
   6. Upload sketch

> \[!IMPORTANT]
> ESP32 version Since the SD card uses IO2 as CS, you cannot upload software in the Arduino IDE when you insert the SD card, so you must remove the SD card when uploading new code
>

- **If the above method still fails to upload the sketch, please follow the method below to test whether the USB2TTL communication is normal.**
  1. Connect USB-C to PC
  2. Open the serial monitor and adjust the baud rate to 115200
  3. Press the RST button on the board to restart
  4. If you can see the startup information, it means that the USB2TTL communication is normal. If nothing is displayed in the serial monitor, it may be that the USB2TTL bridge is damaged.

- **Upgrade modem firmware**
   1. [T-A76XX Upgrade docs](./docs/update_fw.md) / [Video](https://youtu.be/AZkm-Z7mKn8)
   2. [SIM7670G Upgrade issue](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX/issues/122) / [Video](https://www.youtube.com/watch?v=fAtrz_4DfVs)

- **How do I connect the antenna correctly?**
   1. Check the silk screen on the board. **GPS** stands for GPS antenna. Only active GPS antenna can be connected here.
   2. **SIM** or **MAIN**, this is the main antenna interface of LTE
   3. **AUX** This is the diversity antenna for LTE, used to enhance the signal

- **Solar input voltage range?**
   1. **4.4 ~ 6V** , As long as the voltage matches, the solar panel power is not limited

- **Where can I connect a solar panel to charge the battery?**
  1. Some development boards (T-A7670, T-A7608, T-A7670-S3, T-A7608-S3) have a built-in solar cell input interface. Just connect the solar panel according to the correct polarity.
  2. If the board has a battery and charging management, you can also connect an external power supply to the VBUS pin, which is the input pin of the USB power supply. Connecting to VBUS will share the 5V voltage of USB-C. Please note that when connecting an external charger, please disconnect USB-C.

- **SIM767XG sendSMS and VoiceCall?**
   1. Although the manual of SIM767XG states that it has the functions of making voice calls and sending text messages, the current firmware does not support it.

- **Unable to detect SIMCard?**
   1. All SIM series need to insert the SIMCard into the board first and then power on to detect the SIM card. If the order is reversed, it will report that the SIMCard cannot be detected.

- **For ESP32S3 users, if you use external power instead of USB-C, please turn off the CDC option. This is because the board will wait for USB access when it boots up (early versions, Arduino esp-core < 3.0)**
   1. For Arduino IDE users, it can be turned off in the options , Please note that turning off USB CDC will turn off Serial redirection to USBC. At this time, you will not see any Serial message output when opening the port from USBC, but output from GPIO43 and GPIO44.

    ```c
    Tools -> USB CDC On Boot -> Disable
    ```

   2. For Platformio users, you can add the following compilation flags in the ini file

    ```bash
    build_flags =
        ; Enable UARDUINO_USB_CDC_ON_BOOT will start printing and wait for terminal access during startup
        ; -DARDUINO_USB_CDC_ON_BOOT=1

        ; Enable UARDUINO_USB_CDC_ON_BOOT will turn off printing and will not block when using the battery
        -UARDUINO_USB_CDC_ON_BOOT
    ```

- **How to release the limitations of ESP32-WROVER-E GPIO12?**
  1. Since the ESP32-WROVER-E module is used, the internal flash voltage of the module is 3.3V by default. IO12 controls the startup flash startup voltage. If the external device connected to IO12 defaults to the HIGH level, then the startup will fall into an infinite restart. ,
  Two solutions,
  2. Replace the IO port and connect the default low-level device to IO12
  3. Use espefuse to forcefully set the flash voltage to 3.3V. For details, please refer [here](https://docs.espressif.com/projects/esptool/en/latest/esp32/espefuse/set-flash-voltage-cmd.html#set-flash-voltage), this can only be set once, and cannot be set incorrectly. If the setting is incorrect, the module will never start.

- **What the onboard switch does**
  - When using the internal battery pack, the switch will work normally to power/power down the board. However, if an external battery pack is used and connected to the VBAT pin, the switch will be bypassed, meaning the only way to shut down is to disconnect the battery.
  - The switch is only for battery power and has no effect when plugged into USB

- **About VBUS Pin**
  - VBUS Pin and USB-C are on the same line. Only when USB-C is connected, VBUS has voltage
  - When only the battery is connected, VBUS has no voltage output
  - If you want to connect an external power supply without connecting USB-C, VBUS Pin is the only voltage input pin. Please note that the maximum input of VBUS Pin is 5V, do not exceed 5V

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
3. SIM7670G-Datasheet
   - [SIM7670G Hardware Design manual](./datasheet/SIM767X/SIM7672X_Series_Hardware_Design_V1.02.pdf)
   - [SIM7670G AT Command](./datasheet/SIM767X/SIM767XX%20Series_AT_Command_Manual_V1.06.pdf)
   - [SIM7670G CE Certificate](./datasheet/SIM767X/SIM7670G_CE%20Certificate_2023.pdf)
   - [SIM7670G Series CMUX USER GUIDE](./datasheet/SIM767X/SIM767XX%20Series_CMUX_USER_GUIDE_V1.00.pdf)
4. Schematic
   - [T-A7608-S3 Schematic](./schematic/T-A7608-S3-V1.0.pdf)
   - [T-A7608X-DC-S3 Schematic](./schematic/T-A7608X-DC-S3-V1.0.pdf)
   - [T-A7608X Schematic](./schematic/T-A7608X-V1.0.pdf)
   - [T-A7608X-V2 Schematic](./schematic/A7608-ESP32-V2.pdf)
   - [T-A7670X Schematic](./schematic/T-A7670X-V1.1.pdf)
   - [T-Call-A7670 Schematic](./schematic/T-Call-A7670-V1.0.pdf)
   - [T-SIM7670G-S3 Schematic](./schematic/T-SIM7670G-S3-V1.0.pdf)
5. Dimensions
   - [T-A7608X-ESP32](./dimensions/T-A7608X-ESP32.dxf)
   - [T-A7670X-ESP32](./dimensions/T-A7670X-ESP32.dxf)
   - [T-Call-A7670-ESP32](./dimensions/T-Call-A7670-ESP32.dxf)
   - [T-A7608-ESP32S3](./dimensions/T-A7608-S3.dxf)
   - [T-SIM7670-ESP32S3](./dimensions/T-SIM7670G-S3.dxf)
   <!-- - [T-A7608X-DC-S3 DWG](./dimensions/T-A7608X-DC-S3-V1.0.dwg) -->

6. Schematic
   - [T-A7670X schematic](./schematic/T-A7670X-V1.1.pdf)
   - [T-Call-A7670X schematic](./schematic/T-Call-A7670-V1.0.pdf)
   - [T-A7608 schematic](./schematic/T-A7608X-V1.0.pdf)
   - [T-A7608-S3 schematic](./schematic/T-A7608-S3-V1.0.pdf)
   - [T-SIM7670G-S3 schematic](./schematic/T-SIM767XG-S3-V1.0.pdf)
   - [T-PCIE-A7670 schematic](https://github.com/Xinyuan-LilyGO/LilyGo-T-PCIE/blob/master/schematic/A7670.pdf)
   - [T-ETH-ELite schematic](https://github.com/Xinyuan-LilyGO/LilyGO-T-ETH-Series/blob/master/schematic/T-ETH-ELite-LTE-Shield.pdf)



