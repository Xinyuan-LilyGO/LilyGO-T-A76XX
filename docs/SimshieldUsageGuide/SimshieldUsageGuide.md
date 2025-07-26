<div align="center" markdown="1">
  <img src="../../.github/LilyGo_logo.png" alt="LilyGo logo" width="100"/>
</div>

<h1 align = "center">🌟Sim Shield Usage Guide🌟</h1>


> \[!IMPORTANT]
>
> Warning: Jumper caps and DIP switches must be checked when power is on.
> Risk of damage if wrong selection is made
>

### ⚡ Electrical parameters

| Features                                | Details                     |
| --------------------------------------- | --------------------------- |
| ⚡DC interface allows input voltage      | 7~36V                       |
| ⚡Battery interface allows input voltage | 4.2V                        |
| ⚡Charge Current                         | Provided by the motherboard |

## SIM7000G/A7670X/A7608X ESP32

![SIM7000_A7608_A7670_ESP32](./images/SIM7000_A7608_A7670_ESP32.png)

1. **J25** short **IO** -> **LP2**
2. **J21** short **RP1** -> **5V**
3. **SW3** is turned to **ON**, and the remaining **SW1, SW2, and SW4** must not be in the ON position.

## SIM7600X ESP32

![SIM7600_ESP32](./images/SIM7600_ESP32.png)

1. **J25** short **IO** -> **BAT**
2. **J21** short **RP2** -> **5V**
3. **SW3** is turned to **ON**, and the remaining **SW1, SW2, and SW4** must not be in the ON position.

> \[!IMPORTANT]
> The SIM7600 pin header is 15 pins, and the insertion position must be where the arrow points.
>

## SIM7670G ESP32S3

![SIM7670G_ESP32S3](./images/SIM7670G_ESP32S3.png)

1. **J25** short **IO** -> **LP2**
2. **J21** short **RP1** -> **5V**
3. **SW2** is turned to **ON**, and the remaining **SW1, SW3, and SW4** must not be in the ON position.

## A7608X ESP32S3

![A7608_ESP32S3](./images/A7608_ESP32S3.png)

1. **J25** short **IO** -> **LP2**
2. **J21** not connected to any
3. **SW2** is turned to **ON**, and the remaining **SW1, SW3, and SW4** must not be in the ON position.

## SIM7080G ESP32S3

![SIM7080G_ESP32S3](./images/SIM7080G_ESP32S3.png)

1. **J25** not connected to any
2. **J21** not connected to any
3. **SW4** is turned to **ON**, and the remaining **SW1, SW2, and SW3** must not be in the ON position.

> \[!IMPORTANT]
>
> 1. The SIM7080G-ESP32S3 version does not expose the battery external pin. The battery external function of SimShield cannot be used. The positive pole of the 18650 battery holder needs to be welded to the left pin.In the picture, +BAT points to the welding direction of the positive pole of the battery.
> 2. The SIM7080G-ESP32S3 version has an onboard 18650 battery holder that conflicts with SimShield. The battery holder must be removed before installation
>

## Monitor battery charging and discharging jumper settings

* The vertical jumper cap in the figure below routes the battery current to INA3221 channel 2, and the battery charging and discharging current can be monitored by reading the INA3221 sensor. **At this time, do not connect anything to the crimp terminal interface.**
* If the jumper cap is not connected, the crimp terminal interface current is monitored

![Monitor battery charging and discharging jumper](./images/battery_current_detection_mode.png)

## 5V 2.00mm 2Pin power supply interface

* This interface is used to supply external power and has no other functions.

## SD interface

* SimShield remaps the SPI interface. After using SimShield, do not insert the SD card into the motherboard. Instead, insert the SD card into SimShield.

## RS485 interface

* RS485 uses hardware automatic sending and receiving control, and it is recommended to use a communication baud rate below 115200
