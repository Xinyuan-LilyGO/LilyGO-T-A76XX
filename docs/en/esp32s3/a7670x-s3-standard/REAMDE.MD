<div align="center" markdown="1">
  <img src="../../../../.github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align = "center">🌟LilyGo T-A7670X-S3-Standard🌟</h1>

## Overview

* This page introduces the hardware parameters related to `LilyGo T-A7670X-S3-Standard`

### Product

| Product                   | SOC              | Flash          | PSRAM         |
| ------------------------- | ---------------- | -------------- | ------------- |
| [T-A7670X-S3-Standard][1] | ESP32-S3-WROOM-1 | 16MB(Quad-SPI) | 2MB(Quad-SPI) |

[1]: https://www.lilygo.cc/products/


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

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `Visual Studio Code` extension and install it.
3. After the installation is complete, you need to restart `Visual Studio Code`
4. After restarting `Visual Studio Code`, select `File` in the upper left corner of `Visual Studio Code` -> `Open Folder` -> select the `LilyGo-Modem-Series` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Cancel the `;` symbol in front of the line `default_envs = T-A7670X-S3-Standard` and use `T-A7670X-S3-Standard` as the default environment
8. Uncomment one of the lines `src_dir = xxxx` to ensure that only one line is valid. For example, to enable `src_dir = examples/ATdebug`, remove the leading `;` symbol and save the file. The ATDebug example will now be compiled.
9. Click the (✔) symbol in the lower left corner to compile
10. Connect the board to the computer `ESP-USB` , `Modem-USB` is used for module firmware upgrade
11. Click (→) to upload firmware
12. Click (plug symbol) to monitor serial output
13. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

### Arduino IDE quick start

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
3. Copy all folders in the `lib` directory to the `Sketchbook location` directory. How to find the location of your own libraries, [please see here](https://support.arduino.cc/hc/en-us/articles/4415103213714-Find-sketches-libraries-board-cores-and-other-files-on-your-computer)
    * Windows: `C:\Users\{username}\Documents\Arduino`
    * macOS: `/Users/{username}/Documents/Arduino`
    * Linux: `/home/{username}/Arduino`
4. Open the corresponding example
    * Open the downloaded `LilyGo-Modem-Series`
    * Open `examples`
    * Select the sample file and open the file ending with `ino`
5. On Arduino ISelect the corresponding board in the DE tool project and click on the corresponding option in the list below to select

    | Name                                 | Value                               |
    | ------------------------------------ | ----------------------------------- |
    | Board                                | **ESP32S3 Dev Module**              |
    | Port                                 | Your port                           |
    | USB CDC On Boot                      | Enable                              |
    | CPU Frequency                        | 240MHZ(WiFi)                        |
    | Core Debug Level                     | None                                |
    | USB DFU On Boot                      | Disable                             |
    | Erase All Flash Before Sketch Upload | Disable                             |
    | Flash Mode                           | QIO 80Mhz                           |
    | Flash Size                           | **16MB(128Mb)**                     |
    | Arduino Runs On                      | Core1                               |
    | USB Firmware MSC On Boot             | Disable                             |
    | Partition Scheme                     | **16M Flash (3MB APP/9.9MB FATFS)** |
    | PSRAM                                | **QSPI PSRAM**                      |
    | Upload Speed                         | 921600                              |
    | Programmer                           | **Esptool**                         |

     * `Partition Scheme` Please select according to the actual application. For example, select **16M Flash (3MB APP/9.9MB FATFS)** , For more partitioning schemes, please see [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html)

6. In the open sketch, select the `utilities.h` file and uncomment the line `// #define LILYGO_A7670X_S3_STAN`. Delete the `//` and save the file to use this board as the default configuration.

   ![board_select](../../../static/board_select.jpg)

7. Upload sketch

> \[!IMPORTANT]
>
> 1. Unable to upload any code? Please see the FAQ below
> 2. When opening the Arduino IDE, you will be prompted that there are new libraries that can be updated. Please do not click update. There is no guarantee that it can be compiled, run, etc. after the update. Make sure that the board is running normally before deciding whether to update. You have to Know that problems may occur after updating due to library differences, or the default configuration (such as TinyGSM) will be overwritten.
> 3. If it runs abnormally after the update, please follow Article 3 of the Arduino Quick Start to delete the directory in libraries, and drag the folder in lib into the libraries directory again.

### 📍 Pins Map

| Name              | GPIO NUM | Free |
| ----------------- | -------- | ---- |
| Modem TX          | 4        | ❌    |
| Modem RX          | 5        | ❌    |
| Modem RING        | 6        | ❌    |
| Modem DTR         | 7        | ❌    |
| Modem PWRKEY      | 46       | ❌    |
| GPS RX            | 48       | ❌    |
| GPS TX            | 45       | ❌    |
| Modem RTS*        | 48       | ❌    |
| Modem CTS*        | 45       | ❌    |
| GPS PPS           | 17       | ❌    |
| GPS Shield Wakeup | 0        | ❌    |
| Battery ADC Pin   | 8        | ❌    |
| Solar ADC Pin     | 18       | ❌    |
| SD SCK            | 12       | ❌    |
| SD MISO           | 13       | ❌    |
| SD MOSI           | 11       | ❌    |
| SD CS             | 10       | ❌    |
| Power Save Mode   | 42       | ❌    |
| Camera XCLK       | 21       | ❌    |
| Camera SIOD       | 1        | ❌    |
| Camera SIOC       | 41       | ❌    |
| Camera VSYNC      | 9        | ❌    |
| Camera HREF       | 14       | ❌    |
| Camera PCLK       | 37       | ❌    |
| Camera Y9         | 40       | ❌    |
| Camera Y8         | 39       | ❌    |
| Camera Y7         | 38       | ❌    |
| Camera Y6         | 36       | ❌    |
| Camera Y5         | 16       | ❌    |
| Camera Y4         | 47       | ❌    |
| Camera Y3         | 15       | ❌    |
| Camera Y2         | 35       | ❌    |
| Default SDA       | 3        | ✅️    |
| Default SCL       | 2        | ✅️    |

* The `Power Save Mode pin` is used to set the power capability of the power supply. Setting it to HIGH enables maximum power output. Setting it to LOW reduces current consumption.
* If a camera is connected externally, there will be no free GPIO. If the camera is not used, the camera's GPIO can be used for other purposes.
* `SDA`, `SCL` can only be used as I2C function, not as other functions
* `GPS Shield Wakeup Pin` is only used when an external GPS module is connected. Otherwise, it is idle. It is not recommended to use this GPIO. This IO is connected only when using `A7670G + L76K GPS`, otherwise it is not connected
* The camera must be powered on to be detected, otherwise the camera cannot be used. For how to turn it on, please refer to the Camera example.
* Modem RTS/Modem CTS are not connected by default. They are connected to the GPS RX/TX of the Modem by default. If you need to use them, you need to short-circuit the RTS/CTS short-circuit pads on the back of the PCB, and then separate the GPS TX and RX pads.

### ⚡ Electrical parameters

| Features                        | Details |
| ------------------------------- | ------- |
| 🔗USB-C Input Voltage            | 5V      |
| ⚡Solar Input Voltage            | 5~6V    |
| ⚡USB-C/Solar Charge Max Current | 500mA   |
| 🔋Battery Voltage                | 3.7V    |

> \[!TIP]
>
> 1. The onboard JST2.0 interface is only used to connect the solar panel to charge the battery, and cannot provide power to the board from this interface
> 2. It is recommended to use a solar panel with an output voltage of 6V or 5V and a power of about 5-10W. Solar energy can only charge the battery. Without the battery, the panel cannot supply power.
> Please note that this is only a suggestion. It is not necessary to use a 5-10W solar panel. 1W is also acceptable.
> The size of the solar panel determines the current and speed of solar charging. Please note that depending on the environment,
> the sunshine time may not exceed one hour per day. At this time, if the solar panel is too small and the light is not sufficient, it cannot be charged.

### ⚡Battery Protector Description

| Features                | Details  |
| ----------------------- | -------- |
| Over-Voltage Threshold  | 4.30V    |
| Under-Voltage Threshold | 2.4V     |
| Over-Charge Current     | 3.6A     |
| Over-Discharge Current  | 4.6A     |
| Short-Circuit Current   | 2 × 4.6A |

### Connector Description

| Button                     | Function                      | Location   |
| -------------------------- | ----------------------------- | ---------- |
| JST 1.0mm `SPEK`           | Connect speakers              | PCB Bottom |
| JST 1.0mm `MIC`            | Connect condenser microphone  | PCB Bottom |
| JST 1.0mm `RTC`            | Connect 3~3.3V button battery | PCB Bottom |
| JST 2.0mm Solar            | Connect to a 5~6V solar panel | PCB Top    |
| 24 Pin 0.5mm FPC Connector | Connect OV2640/OV5640 camera  | PCB Top    |

* Camera models support cameras supported by esp32 platform

### Button Description

| Button               | Function                            | Location   |
| -------------------- | ----------------------------------- | ---------- |
| BOOT  (ESP32S3 back) | Customizable function/Download mode | PCB Bottom |
| RST   (ESP32S3 back) | Reset devices button                | PCB Bottom |
| SBOOT(Near to modem) | Modem Upgrade mode                  | PCB Bottom |

* **RST** and **BOOT** Located on the bottom of the ESP32 module
* **SBOOT** is located on the back near the modem module
* Press and hold the `BOOT` button before powering on. The board is now in waiting mode for downloading.
* Before powering on, keep the `SBOOT` button pressed to put the modem into firmware update mode.
* The above naming method is subject to the board silk screen printing

### Antenna Description

| NAME | Function                     |
| ---- | ---------------------------- |
| LTE  | LTE Main antenna interface   |
| GNSS | GPS active antenna interface |

* When using, you only need to connect the LTE antenna you wear to the LTE IPEX first generation interface. The AUX antenna does not need to be connected.
* The above naming method is subject to the board silk screen printing

### LED Description

| FUNCTION        | COLOR                            | STATE                                                                  |
| --------------- | -------------------------------- | ---------------------------------------------------------------------- |
| MODEM STATE     | RED   (Near the modem)           | Startup:ON                                                             |
| MODEM NET       | RED   (Near the modem)           | Network status indicator                                               |
| GPS PPS LED     | RED  (Near the modem)            | Starting:ON,Startup completed:OFF,Position fix:Flashes once per second |
| CHARGE LED      | RED  (Near the battery switch)   | Charging:ON  Charging complete:OFF                                     |
| CHARGE DONE LED | GREEN  (Near the battery switch) | Charging:OFF  Charging complete:ON                                     |

* `CHARGE DONE LED` remains on if no battery is connected
* Modem Status light and network status led cannot be turned off

### Resource

* [GPS Antenna](../../../../datasheet/GPS%20Antenna%20Specifications.pdf)
* [LTE Antenna](../../../../datasheet/LTE%20Antenna%20Specifications.pdf)
* [T-A7670X-S3-Standard Schematic](../../../../schematic/standard/T-A7670X-S3-Standard_Rev1.0.pdf)
* [SIMCOM official A7670X All Datasheet](https://cn.simcom.com/product/A7670X.html)
* [For A7670X series documentation, please visit the datasheet directory.](../../../../datasheet/A7670X/)

### FAQ

* **Upgrade modem firmware**
  1. [T-A76XX Upgrade docs](../../../../docs/update_fw.md) / [Video](https://youtu.be/AZkm-Z7mKn8)

* **Unable to detect SIMCard?**
   1. Need to insert the SIMCard into the board first and then power on to detect the SIM card. If the order is reversed, it will report that the SIMCard cannot be detected.

* **Can't upload any sketch，Please enter the upload mode manually.**
   1. Connect the board via the USB cable
   2. Press and hold the **BOOT** button , While still pressing the **BOOT** button
   3. Press **RST** button
   4. Release the **RST** button
   5. Release the **BOOT** button
   6. Upload sketch

* **What the onboard switch does**
  1. When using the internal battery pack, the switch will work normally to power/power down the board. However, if an external battery pack is used and connected to the VBAT pin, the switch will be bypassed, meaning the only way to shut down is to disconnect the battery.
  2. The switch is only for battery power and has no effect when plugged into USB

* **About VBUS Pin**
  1. VBUS Pin and USB-C are on the same line. Only when USB-C is connected, VBUS has voltage
  2. When only the battery is connected, VBUS has no voltage output
  3. If you want to connect an external power supply without connecting USB-C, VBUS Pin is the only voltage input pin. Please note that the maximum input of VBUS Pin is 5V, do not exceed 5V
