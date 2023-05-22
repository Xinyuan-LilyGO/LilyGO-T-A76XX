<h1 align = "center">T-A7670 Blynk_Console</h1>

## **[English](./README.MD) | [中文](./README_CN.MD)**

# 步骤:

1. 需要安装下面的库
     - [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit-BMP085-Library)
     - [blynk-library](https://github.com/blynkkk/blynk-library)
     - [TinyGSM](https://github.com/vshymanskyy/TinyGSM)

2. 注册登录[blynk.cloud](https://blynk.cloud/dashboard/login) 

3. 创建新模板或使用已有模板

![](../../../../image/Blynk/1-Create_Template-A7670.png)

1. 当模板准备好，进入搜索->设备-创建新设备  

![](../../../../image/Blynk/2-Create_device-A7670.png)

5. 配置模块

![](../../../../image/Blynk/3-Configuration_module.png)
![](../../../../image/Blynk/3-1-Configuration_module.png)
![](../../../../image/Blynk/3-2-Configuration_module.png)
![](../../../../image/Blynk/3-3-Configuration_module.png)
![](../../../../image/Blynk/3-4-Configuration_module.png)
![](../../../../image/Blynk/3-5-Configuration_module.png)

6. 设置 Web 仪表板 

![](../../../../image/Blynk/4-Dash_board.png)
![](../../../../image/Blynk/4-1-Dash_board.png)
![](../../../../image/Blynk/4-2-Dash_board.png)
![](../../../../image/Blynk/4-3-Dash_board.png)
![](../../../../image/Blynk/4-4-Dash_board.png)
![](../../../../image/Blynk/4-5-Dash_board.png)
![](../../../../image/Blynk/4-6-Dash_board.png)

7. 替换代码中的宏

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN "";

![](../../../../image/Blynk/5-Ready_code.png)

8. 上传代码，可以控制LDE和获取数据

 ![](../../../../image/Blynk/6.png)

10. 参考[文档](https://docs.blynk.io/en/)


# 硬件接线
| T-A7670 pin | BMP180 pin |
| ----------- | ---------- |
| 3.3         | VIN        |
| GND         | GND        |
| GPIO21(SDA) | SDA        |
| GPIO22(SCL) | SCL        |

