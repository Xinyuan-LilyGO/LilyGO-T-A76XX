
<h1 align = "center">🌟SIM7600 Modem Firmware Update Guide🌟</h1>

#### 1. Download Driver & Tools

* [Windows USB Drivers](https://drive.google.com/file/d/1WhHny4E-43DxUfifMkrX752oFBx7O70D/view?usp=sharing)
* [Upgrade Firmware tools](https://drive.google.com/file/d/1f02TTNoyirFPGWbob1khy9dnoBonoVe7/view?usp=sharing)
* [Upgrade recording video](https://youtu.be/7cJzjfcrFWY)

**The firmware version flashed must be consistent with the PN number of the demodulator, otherwise it will become a brick.**

* [SIMCOM-7600G-H PN:S2-1097D-Z307J](https://drive.google.com/file/d/11RRi8oIJKkH_XrOYSVGDl4XojPs5zYOM/view?usp=sharing)
* [SIMCOM-7600G-H PN:S2-1097D-Z31E3](https://drive.google.com/drive/folders/11YthuETFAIiLzhQ2mxGtca27JOckc2oP?usp=sharing)
* [SIMCOM-7600G-H PN:S2-1097C-Z31FP](https://drive.google.com/drive/folders/11pfGCjkZFXo1AnTbNawqvSGudWjKn_26?usp=sharing)
* [SIMCOM-7600G-H PN:S2-1097D-Z32DY](https://drive.google.com/file/d/12BMSSGPISMhFEDvG7C9s34YvWg5PDNNb/view?usp=sharing)
* [SIMCOM-7600E-H PN:S2-107EQ-Z30LC](https://drive.google.com/file/d/12qAtFyZo5jR1xwohpejrqwjh5R_a6uY0/view?usp=sharing)

#### 2. Switch USB input to Modem

1. Turn the DIP switch on the back of the USB to the position shown in the figure below

   <img  height="320" width="240" src=../images/USB.png>

2. Connect the USBC to the modem interface , The `USB interface` in the image below

   <img  height="290" width="640" src=../images/step9.png>

### 3. Install driver

1. Open the computer device manager, the current upgrade only supports Windows

   <img  height="500" width="650" src=../images/update_simxxxx_3.png>
   <img  height="500" width="650" src=../images/update_simxxxx_4.png>
   <img  height="500" width="650" src=../images/update_simxxxx_6.png>

2. Repeat the above process to complete the installation of all the drivers in the other device lists. At this time, do not close the device manager. When there are unknown devices during the update process, continue to update the drivers according to the method

### 4. Get current firmware version

1. Upload [ATDebug Sketch](../../../../../examples/ATdebug/ATdebug.ino)，This step is to ensure that the modem starts normally

2. Before upgrading, please send `AT+SIMCOMATI` to check the hardware version, Modem will brick if wrong version firmware is written

   <img  height="400" width="700" src=../images/version.png>

3. Please provide the information in the QR code on the modem to LilyGo to confirm the firmware version or submit in issue

### 5. Upgrade firmware steps

1. Open `sim7080_sim7500_sim7600_sim7900_sim8200 qdl v1.67 only for update.exe`
2. Follow the instructions below to update the firmware

   <img  height="400" width="800" src=../images/step1.png>

   <img  height="400" width="800" src=../images/step2.png>

   <img  height="500" width="800" src=../images/step3.png>

   <img  height="400" width="800" src=../images/step4.png>

   <img  height="1000" width="800" src=../images/step5.png>

   <img  height="800" width="800" src=../images/step6.png>

   <img  height="400" width="800" src=../images/step7.png>

### 6. After the firmware is updated, you can send `AT+SIMCOMATI` to check the version

   <img  height="1000" width="800" src=../images/step8.png>
