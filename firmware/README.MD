# Write to new users

- This page provides a quick diagnosis of whether the hardware is normal. The firmware in the example table below can provide a quick test of the hardware. Follow the writing method below.
- When using GPS positioning test, please do it outdoors or by extending the active GPS antenna. The geographical location cannot be obtained indoors.
- For network testing, please connect the LTE antenna to the board's silk screen MAIN or SIM IPEX antenna base.
- Please do not use a SIM card with a locked PIN code. Please delete the PIN code before use.

# 1️⃣ Quickly diagnose hardware

| Product            | GPS Test Firmware                               | Network Test Firmware                    |
| ------------------ | ----------------------------------------------- | ---------------------------------------- |
| [T-A7670X][1]      | [firmware](./T-A7670X_GPS_NMEA_Parse.bin)       | [firmware](./T-A7670X_Network.bin)       |
| [T-Call-A7670X][2] | [firmware](./T-Call-A7670X_GPS_NMEA_Parse.bin)  | [firmware](./T-Call-A7670X_Network.bin)  |
| [T-A7608][3]       | [firmware](./T-A7608X_GPS_NMEA_Parse.bin)       | [firmware](./T-A7608X_Network.bin)       |
| [T-A7608-DC-S3][4] | [firmware](./T-A7608X-DC-S3_GPS_NMEA_Parse.bin) | [firmware](./T-A7608X-DC-S3_Network.bin) |
| [T-A7608-S3][5]    | [firmware](./T-A7608X-S3_GPS_NMEA_Parse.bin)    | [firmware](./T-A7608X-S3_Network.bin)    |
| [T-SIM7672G-S3][6] | [firmware](./T-SIM7672G_GPS_NMEA_Parse.bin)     | [firmware](./T-SIM7672G_Network.bin)     |

[1]: https://www.lilygo.cc/products/t-sim-a7670e
[2]: https://www.lilygo.cc
[3]: https://www.lilygo.cc/products/t-a7608e-h
[4]: https://www.lilygo.cc
[5]: https://www.lilygo.cc
[6]: https://www.lilygo.cc

| Steps  | [T-A7670X][1]/[T-Call-A7670X][2]/[T-A7608][3] | [T-A7608-DC-S3][4]/[T-A7608-DC-S3][4]/[T-A7608-S3][5]/[T-SIM7672G-S3][6] |
| ------ | --------------------------------------------- | ------------------------------------------------------------------------ |
| Step 1 | ![esp32dev](images/esp32-1.png)               | ![esp32s3dev](images/esp32s3-1.png)                                      |
| Step 2 | ![esp32dev](images/esp32-2.png)               | ![esp32s3dev](images/esp32s3-2.png)                                      |

# 2️⃣FAQ

- Can't upload any sketch，Please enter the upload mode manually.
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button (If there is no BOOT button, you need to use wires to connect GND and IO0 together.)
   3. Press RST button
   4. Release the RST button
   5. Release the BOOT button (If there is no BOOT button, disconnect IO0 from GND.)
   6. Upload sketch


