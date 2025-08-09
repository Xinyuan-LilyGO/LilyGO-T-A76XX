<div align="center" markdown="1">
  <img src="../../../../.github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align = "center">🌟LilyGo T-Call A7670X ESP32 Version🌟</h1>

## Overview

* This page introduces the hardware parameters related to `LilyGo T-Call A7670X ESP32 Version`

### Product

| Product                   | SOC            | Flash         | PSRAM         |
| ------------------------- | -------------- | ------------- | ------------- |
| [T-Call A7670E ESP32][1]  | ESP32-WROVER-E | 4MB(Quad-SPI) | 8MB(Quad-SPI) |
| [T-Call A7670SA ESP32][1] | ESP32-WROVER-E | 4MB(Quad-SPI) | 8MB(Quad-SPI) |
| [T-Call A7670G ESP32][1]  | ESP32-WROVER-E | 4MB(Quad-SPI) | 8MB(Quad-SPI) |

[1]: https://www.lilygo.cc/products/t-call-v1-4

### Modem differences

| Model           | GPS | Phone Call | SMS | Frequency band                                                                                                     |
| --------------- | --- | ---------- | --- | ------------------------------------------------------------------------------------------------------------------ |
| A7670E-FASE     | ✅   | ✅          | ✅   | LTE-FDD:B1/B3/B5/B7/B8/B20  GSM:900/1800                                                                           |
| A7670SA-FASE    | ✅   | ✅          | ✅   | LTE-FDD:B1/B2/B3/B4/B5/B7/B8/B28/B66  GSM:850/900/1800/1900                                                        |
| A7670E-LNXY-UBL | ❌   | ❌          | ❌   | LTE-FDD:B1/B3/B5/B7/B8/B20                                                                                         |
| A7670SA-LASE    | ❌   | ✅          | ✅   | LTE-FDD:B1/B2/B3/B4/B5/B7/B8/B28/B66  GSM:850/900/1800/1900                                                        |
| A7670SA-LASC    | ❌   | ✅          | ✅   | LTE-FDD:B1/B2/B3/B4/B5/B7/B8/B28/B66  GSM:850/900/1800/1900                                                        |
| A7670G-LLSE     | ❌   | ✅          | ✅   | LTE-FDD:B1/B2/B3/B4/B5/B7/B8/B20/B28/B66  LTE-TDD:B34/B38/B39/B40/B41 GSM:850/900/1800/1900                        |
| A7670G-LABE     | ❌   | ✅          | ✅   | LTE-FDD:B1/B2/B3/B4/B5/B7/B8/B12/B13/B18/B19/B20/B25/B26/B28/B66 LTE-TDD:B34/B38/B39/B40/B41 GSM:850/900/1800/1900 |
| A7670E-LASE     | ❌   | ✅          | ✅   | LTE-FDD:B1/B3/B5/B7/B8/B20  GSM:900/1800                                                                           |

> \[!IMPORTANT]
> How to check the specific model version of the modem? Sending AT+SIMCOMATI can return the firmware version and hardware version used.
>
> Please note that whether the GNSS IPEX antenna connector is on the PCB or not depends on the actual modem version. The A7670G modem does not have internal GPS functionality, regardless of whether GPS is included in your order. If you purchased the A7670G with GPS, you will need to use an external GPS module.

### PlatformIO Quick Start

1. Install the **CH9102 USB bridge** driver for the first time.
   * [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html)
   * [Mac OS](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
2. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
3. Search for the `PlatformIO` plugin in the `Visual Studio Code` extension and install it.
4. After the installation is complete, you need to restart `Visual Studio Code`
5. After restarting `Visual Studio Code`, select `File` in the upper left corner of `Visual Studio Code` -> `Open Folder` -> select the `LilyGo-Modem-Series` directory
6. Wait for the installation of third-party dependent libraries to complete
7. Click on the `platformio.ini` file, and in the `platformio` column
8. Cancel the `;` symbol in front of the line `default_envs = default_envs = T-Call-A7670X-V1-0` and use `default_envs = T-Call-A7670X-V1-0` as the default environment , If you are using V1.1, then select `default_envs = T-Call-A7670X-V1-1`
9. Uncomment one of the lines `src_dir = xxxx` to ensure that only one line is valid. For example, to enable `src_dir = examples/ATdebug`, remove the leading `;` symbol and save the file. The ATDebug example will now be compiled.
10. Click the (✔) symbol in the lower left corner to compile
11. Connect the board to the computer USB-C , Micro-USB is used for module firmware upgrade
12. Click (→) to upload firmware
13. Click (plug symbol) to monitor serial output
14. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

### Arduino IDE quick start

1. Install the **CH9102 USB bridge** driver for the first time.
   * [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html)
   * [Mac OS](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
2. Install [Arduino IDE](https://www.arduino.cc/en/software)
3. Install [Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
4. Copy all folders in the `lib` directory to the `Sketchbook location` directory. How to find the location of your own libraries, [please see here](https://support.arduino.cc/hc/en-us/articles/4415103213714-Find-sketches-libraries-board-cores-and-other-files-on-your-computer)
    * Windows: `C:\Users\{username}\Documents\Arduino`
    * macOS: `/Users/{username}/Documents/Arduino`
    * Linux: `/home/{username}/Arduino`
5. Open the corresponding example
    * Open the downloaded `LilyGo-Modem-Series`
    * Open `examples`
    * Select the sample file and open the file ending with `ino`
6. On Arduino ISelect the corresponding board in the DE tool project and click on the corresponding option in the list below to select

    | Name                                 | Value                                |
    | ------------------------------------ | ------------------------------------ |
    | Board                                | **ESP32 Dev Module**                 |
    | Port                                 | Your port                            |
    | CPU Frequency                        | 240MHZ(WiFi/BT)                      |
    | Core Debug Level                     | None                                 |
    | Erase All Flash Before Sketch Upload | Disable                              |
    | Events Run On                        | Core1                                |
    | Flash Frequency                      | 80MHZ                                |
    | Flash Mode                           | QIO                                  |
    | Flash Size                           | **4MB(32Mb)**                        |
    | JTAG Adapter                         | Disabled                             |
    | Arduino Runs On                      | Core1                                |
    | Partition Scheme                     | **Huge APP (3MB No OTA/1MB SPIFFS)** |
    | PSRAM                                | **Enable**                           |
    | Upload Speed                         | 921600                               |
    | Programmer                           | **Esptool**                          |

7. In the open sketch, select the `utilities.h` file and uncomment the line `// #define LILYGO_T_CALL_A7670_V1_0`. Delete the `//` and save the file to use this board as the default configuration, If you are using V1.1, then select `#define LILYGO_T_CALL_A7670_V1_1`

   ![board_select](../../../static/board_select.jpg)

8. Upload sketch

> \[!IMPORTANT]
>
> 1. Unable to upload any code? Please see the FAQ below
> 2. When opening the Arduino IDE, you will be prompted that there are new libraries that can be updated. Please do not click update. There is no guarantee that it can be compiled, run, etc. after the update. Make sure that the board is running normally before deciding whether to update. You have to Know that problems may occur after updating due to library differences, or the default configuration (such as TinyGSM) will be overwritten.
> 3. If it runs abnormally after the update, please follow Article 4 of the Arduino Quick Start to delete the directory in libraries, and drag the folder in lib into the libraries directory again.

### 📍 Pins Map

| Name         | V1.0 | V1.1 |
| ------------ | ---- | ---- |
| Modem TX     | 26   | 27   |
| Modem RX     | 25   | 26   |
| Modem PWRKEY | 4    | 4    |
| Modem RESET  | 27   | 5    |
| Modem RING   | 13   | 33   |
| Modem DTR    | 14   | 32   |
| Board LED    | 12   | 13   |

> \[!IMPORTANT]
> V1.0 and 1.1 use completely different pins, please pay attention to distinguish the versions
> ESP32 models GPIO33 and above can only be set as input, not output
> How to distinguish V1.0 from 1.1, check the silk screen on the PCB, which has the version information sprayed on it

### ⚡ Electrical parameters

| Features             | Details |
| -------------------- | ------- |
| 🔗USB-C Input Voltage | 5V      |

> \[!TIP]
>
> T-Call does not have a battery access function and can only be powered via USB-C
>

### Button Description

| Button    | Function                            |
| --------- | ----------------------------------- |
| BOOT      | Customizable function/Download mode |
| RST       | Reset devices button                |
| 7670_BOOT | Modem Upgrade mode                  |

* Press and hold the `BOOT` button before powering on. The board is now in waiting mode for downloading.
* Before powering on, keep the `7670_BOOT` button pressed to put the modem into firmware update mode.
* The above naming method is subject to the board silk screen printing

### Antenna Description

|      | Function                     |
| ---- | ---------------------------- |
| MAIN | LTE Main antenna interface   |
| GPS  | GPS active antenna interface |

* The above naming method is subject to the board silk screen printing

### LED Description

| FUNCTION            | COLOR                  |
| ------------------- | ---------------------- |
| MODEM STATUS        | RED   (Near the modem) |
| MODEM NETWORK STATE | RED   (Near the modem) |

* Modem Status light and network status led cannot be turned off

### Resource

* [GPS Antenna](../../../../datasheet/GPS%20Antenna%20Specifications.pdf)
* [LTE Antenna](../../../../datasheet/LTE%20Antenna%20Specifications.pdf)
* [T-Call-A7670-V1.0 Schematic](../../../../schematic/esp32/T-Call-A7670-V1.0.pdf)
* [T-Call-A7670-V1.1 Schematic](../../../../schematic/esp32/T-Call-A7670-V1.1.pdf)
* [SIMCOM official A7670X All Datasheet](https://cn.simcom.com/product/A7670X.html)
* [For A7670X series documentation, please visit the datasheet directory.](../../../../datasheet/A7670X/)
* [Board dimensions](../../../../dimensions/esp32/T-Call-A7670-ESP32.dxf)

### FAQ

* **If the above method still fails to upload the sketch, please follow the method below to test whether the USB2TTL communication is normal.**
  1. Connect USB-C to PC
  2. Open the serial monitor and adjust the baud rate to 115200
  3. Press the RST button on the board to restart
  4. If you can see the startup information, it means that the USB2TTL communication is normal. If nothing is displayed in the serial monitor, it may be that the USB2TTL bridge is damaged

* **Upgrade modem firmware**
  1. [T-A76XX Upgrade docs](../../../../docs/update_fw.md) / [Video](https://youtu.be/AZkm-Z7mKn8)
  2. [Upgrade firmware issue](https://github.com/Xinyuan-LilyGO/LilyGo-Modem-Series/issues/180)

* **Unable to detect SIMCard?**
   1. Need to insert the SIMCard into the board first and then power on to detect the SIM card. If the order is reversed, it will report that the SIMCard cannot be detected.

* **Can't upload any sketch，Please enter the upload mode manually.**
   1. Connect the board via the USB cable
   2. Press and hold the **BOOT** button , While still pressing the **BOOT** button
   3. Press **RST** button
   4. Release the **RST** button
   5. Release the **BOOT** button
   6. Upload sketch

* **About VBUS Pin**
  1. VBUS Pin and USB-C are on the same line. Only when USB-C is connected, VBUS has voltage
  2. When only the battery is connected, VBUS has no voltage output
  3. If you want to connect an external power supply without connecting USB-C, VBUS Pin is the only voltage input pin. Please note that the maximum input of VBUS Pin is 5V, do not exceed 5V
