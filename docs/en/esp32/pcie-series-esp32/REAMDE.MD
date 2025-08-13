<div align="center" markdown="1">
  <img src="../../../../.github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align = "center">🌟LilyGo PCI-E Series ESP32 Version🌟</h1>

## Overview

* This page introduces the hardware parameters related to `LilyGo PCI-E Series ESP32 Version`

### Product

| Product                             | SOC            | Flash          | PSRAM         |
| ----------------------------------- | -------------- | -------------- | ------------- |
| [T-PCIE Main board 4MB Version][1]  | ESP32-WROVER-E | 4MB(Quad-SPI)  | 8MB(Quad-SPI) |
| [T-PCIE Main board 16MB Version][2] | ESP32-WROVER-E | 16MB(Quad-SPI) | 8MB(Quad-SPI) |

[1]: https://lilygo.cc/products/a-t-pcie?variant=43043718725813
[2]: https://lilygo.cc/products/a-t-pcie?variant=42335921832117

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
8. Cancel the `;` symbol in front of the line `default_envs = T-PCIE-XXXX` and use `T-PCIE-XXXX` as the default environment, Name **XXXX** Please select according to the actual version
9. Uncomment one of the lines `src_dir = xxxx` to ensure that only one line is valid. For example, to enable `src_dir = examples/ATdebug`, remove the leading `;` symbol and save the file. The ATDebug example will now be compiled.
10. Click the (✔) symbol in the lower left corner to compile
11. Connect the board to the computer USB-C
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

     * `Partition Scheme` Please select according to the actual application. For example, select **Huge APP (3MB No OTA/1MB SPIFFS)** , For more partitioning schemes, please see [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html)

7. In the open sketch, select the `utilities.h` file and uncomment the line `// #define LILYGO_T_PCIE_XXXX`. Delete the `//` and save the file to use this board as the default configuration. Name **XXXX** Please select according to the actual version

   ![board_select](../../../static/board_select.jpg)

8. Upload sketch

> \[!IMPORTANT]
>
> 1. Unable to upload any code? Please see the FAQ below
> 2. When opening the Arduino IDE, you will be prompted that there are new libraries that can be updated. Please do not click update. There is no guarantee that it can be compiled, run, etc. after the update. Make sure that the board is running normally before deciding whether to update. You have to Know that problems may occur after updating due to library differences, or the default configuration (such as TinyGSM) will be overwritten.
> 3. If it runs abnormally after the update, please follow Article 4 of the Arduino Quick Start to delete the directory in libraries, and drag the folder in lib into the libraries directory again.

### 📍 Pins Map

| Name                    | GPIO NUM | Free |
| ----------------------- | -------- | ---- |
| Modem DC booster enable | 25       | ❌    |
| Modem TX                | 27       | ❌    |
| Modem RX                | 26       | ❌    |
| Modem PWRKEY            | 4        | ❌    |
| Modem DTR               | 32       | ❌    |
| Modem RING              | 33       | ❌    |
| Board LED               | 12       | ❌    |
| PMIC IRQ                | 35       | ❌    |
| PMIC SDA                | 21       | ✅️    |
| PMIC SCL                | 22       | ✅️    |

* ESP32 models GPIO33 and above can only be set as input, not output
* I2C pins are fixed and cannot be changed. They are already connected to the PMIC.

### ⚡ Electrical parameters

| Features                  | Details |
| ------------------------- | ------- |
| 🔗USB-C Input Voltage      | 5V      |
| ⚡USB-C Charge Max Current | 1000mA  |
| 🔋Battery Voltage          | 3.7V    |

* The charging current can be adjusted by PMIC, up to 1A

### Button Description

| Button              | Function                                                                                               |
| ------------------- | ------------------------------------------------------------------------------------------------------ |
| EN (Close to USB-C) | Reset devices button                                                                                   |
| PWR                 | Press and hold for 6 seconds to turn off the device, press and hold for 1 second to turn on the device |

* The above naming method is subject to the board silk screen printing

### Antenna Description

|          | Function                     |
| -------- | ---------------------------- |
| SIM/MAIN | LTE Main antenna interface   |
| GPS/GNSS | GPS active antenna interface |

* The above naming method is subject to the board silk screen printing

### LED Description

| FUNCTION            | COLOR                         |
| ------------------- | ----------------------------- |
| MODEM STATUS        | RED   (On the modem)          |
| MODEM NETWORK STATE | RED   (On the modem)          |
| CHARGE LED          | BLUE  (Near the ON BOARD LED) |
| ON BOARD LED        | GREEN  (Near the esp module)  |

* Modem Status light and network status led cannot be turned off

### Resource

* [GPS Antenna](../../../../datasheet/GPS%20Antenna%20Specifications.pdf)
* [LTE Antenna](../../../../datasheet/LTE%20Antenna%20Specifications.pdf)

#### SIM7000X Series

* [T-SIM7000G Schematic](../../../../schematic/esp32/T-SIM7000G-200415.pdf)
* [SIMCOM official SIM7000G All Datasheet](https://cn.simcom.com/product/SIM7000X.html)
* [For SIM7000 series documentation, please visit the datasheet directory.](../../../../datasheet/SIM7000/)

#### A7608X Series

* [SIMCOM official A7608X All Datasheet](https://cn.simcom.com/product/A7608X-H-E_SA.html)
* [For A7608 series documentation, please visit the datasheet directory.](../../../../datasheet/A76XX/)

#### SIM7670G Series

* [SIMCOM official A7670X All Datasheet](https://cn.simcom.com/product/A7670X.html)
* [For A7670X series documentation, please visit the datasheet directory.](../../../../datasheet/A76XX/)

#### SIM7600X Series

* [SIMCOM official SIM7600X All Datasheet](https://cn.simcom.com/product/SIM7600X.html)
* [For SIM7600 series documentation, please visit the datasheet directory.](../../../../datasheet/SIM7600X/)

#### SIM7080G Series

* [SIMCOM official SIM7080G-S3 All Datasheet](https://cn.simcom.com/product/SIM7080G.html)
* [For SIM7080G-S3 series documentation, please visit the datasheet directory.](../../../../datasheet/SIM7080/)

### FAQ

* **If the above method still fails to upload the sketch, please follow the method below to test whether the USB2TTL communication is normal.**
  1. Connect USB-C to PC
  2. Open the serial monitor and adjust the baud rate to 115200
  3. Press the RST button on the board to restart
  4. If you can see the startup information, it means that the USB2TTL communication is normal. If nothing is displayed in the serial monitor, it may be that the USB2TTL bridge is damaged

* **Upgrade modem firmware** Please refer here
  1. [T-SIM7000G Upgrade docs](../sim7000-esp32/upgrade/sim7000_upgrade.md)
  2. [T-SIM7670G Upgrade docs](./upgrade/sim7000_upgrade.md)
  3. [T-A7670X   Upgrade docs](../../../update_fw.md)
  4. [T-SIM7600X Upgrade docs](../sim7600-esp32/upgrade/sim7600_upgrade.md)
  5. [T-SIM7080G Upgrade docs](../../esp32s3/sim7080-s3-standard/upgrade/README.MD)

* **Unable to detect SIMCard?**
   1. Need to insert the SIMCard into the board first and then power on to detect the SIM card. If the order is reversed, it will report that the SIMCard cannot be detected.

* **Can't upload any sketch，Please enter the upload mode manually.**
   1. Connect the board via the USB cable
   2. Use wires to connect GND and IO0 together
   3. Press RST button
   4. Release the RST button
   5. Disconnect IO0 from GND
   6. Upload sketch

* **What the onboard switch does**
  1. When using the internal battery pack, the switch will work normally to power/power down the board. However, if an external battery pack is used and connected to the VBAT pin, the switch will be bypassed, meaning the only way to shut down is to disconnect the battery.
  2. The switch is only for battery power and has no effect when plugged into USB

* **About VBUS Pin**
  1. VBUS Pin and USB-C are on the same line. Only when USB-C is connected, VBUS has voltage
  2. When only the battery is connected, VBUS has no voltage output
  3. If you want to connect an external power supply without connecting USB-C, VBUS Pin is the only voltage input pin. Please note that the maximum input of VBUS Pin is 5V, do not exceed 5V
