<h1 align = "center">T-PCIE-SIM7000 Blynk_Console</h1>

## **[English](./README.MD) | [中文](./README_CN.MD)**


# Steps:
1. Need to install the following dependencies
     - [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit-BMP085-Library)
     - [blynk-library](https://github.com/blynkkk/blynk-library)
     - [TinyGSM](https://github.com/vshymanskyy/TinyGSM)

2. Register and login[blynk.cloud](https://blynk.cloud/dashboard/login) 

3. Create a new template or use an existing template (The name can be customized)

![](../../../image/Blynk/1-Create_Template-A7670.png)

4.   When the Template is ready, go to Search -> Devices - Create New Device

![](../../../image/Blynk/2-Create_device-A7670.png)

5. Configuration module

![](../../../image/Blynk/3-Configuration_module.png)
![](../../../image/Blynk/3-1-Configuration_module.png)
![](../../../image/Blynk/3-2-Configuration_module.png)
![](../../../image/Blynk/3-3-Configuration_module.png)
![](../../../image/Blynk/3-4-Configuration_module.png)
![](../../../image/Blynk/3-5-Configuration_module.png)

6. Set up the Web dashboard

![](../../../image/Blynk/4-Dash_board.png)
![](../../../image/Blynk/4-1-Dash_board.png)
![](../../../image/Blynk/4-2-Dash_board.png)
![](../../../image/Blynk/4-3-Dash_board.png)
![](../../../image/Blynk/4-4-Dash_board.png)
![](../../../image/Blynk/4-5-Dash_board.png)
![](../../../image/Blynk/4-6-Dash_board.png)

7. Replace the macro definition in the code

    #define BLYNK_TEMPLATE_ID ""
    #define BLYNK_DEVICE_NAME ""
    #define BLYNK_AUTH_TOKEN "";

 ![](../../../image/Blynk/5-Ready_code.png)

8. Upload the code and now you control the LDE and get the data in the cloud
   
   ![](../../../image/Blynk/6.png)

9. Reference[Documentation](https://docs.blynk.io/en/)

# Hardware
Connections for T-SIM7000 and BMP180
| T-PCIE pin  | BMP180 pin |
| ----------- | ---------- |
| 3.3         | VIN        |
| GND         | GND        |
| GPIO21(SDA) | SDA        |
| GPIO22(SCL) | SCL        |





