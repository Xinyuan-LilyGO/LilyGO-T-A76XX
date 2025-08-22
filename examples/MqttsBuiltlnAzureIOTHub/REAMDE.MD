# 20250822 Test with A7670G

```bash
AT
.
AT
AT
AT
AT
.
AT

*ATREADY: 1
AT
AT

OK

AT+CPIN?
AT+CPIN?
+CME ERROR: 13

+CPIN: READY
SIM card online
Wait for the modem to register with the network.AT+CEREG?

+CGEV: EPS PDN ACT 1

+CGEV: ME PDN ACT 8,1
AT+CEREG?

+CEREG: 0,1

OK
Online registration successful

Registration Status:1
AT+CPSI?
AT+CPSI?

+CPSI: LTE,Online,460-11,0x7764,117148181,127,EUTRAN-BAND1,100,5,5,8,53,60,-4

OK
Inquiring UE system information:LTE,Online,460-11,0x7764,117148181,127,EUTRAN-BAND1,100,5,5,8,53,60,-4
AT+NETOPEN
AT+NETOPEN

OK

+NETOPEN: 0AT+IPADDR

AT+IPADDR

+IPADDR: 10.4.112.219

OK
Network IP:10.4.112.219
AT+SIMCOMATI
AT+SIMCOMATI

Manufacturer: SIMCOM INCORPORATED
Model: A7670G-LLSE
Revision: A124B01A7670M7
A7670M7_B01V01_240722
IMEI: 

OK
AT+SIMCOMATI

Manufacturer: SIMCOM INCORPORATED
Model: A7670G-LLSE
Revision: A124B01A7670M7
A7670M7_B01V01_240722
IMEI: 

OK

AT+CMQTTSTART
AT+CMQTTSTART

OK

+CMQTTSTART: 0
Connecting to xxxxxxx.azure-devices.net
AT+CSSLCFG="authmode",0,0
AT+CSSLCFG="authmode",0,0

OK
AT+CMQTTREL=0
AT+CMQTTREL=0

+CMQTTREL: 0,20

ERROR
AT+CMQTTACCQ=0,"MyDevice",1
AT+CMQTTACCQ=0,"MyDevice",1

OK
AT+CMQTTCFG="version",0,4
AT+CMQTTCFG="version",0,4

OK
AT+CMQTTCONNECT=0,"tcp://xxxxxxx.azure-devices.net:8883",60,1,"xxxxxxx.azure-devices.net/MyDevice/?api-version=2021-04-12","SharedAccessSignature sr=xxxxxxx.azure-devices.net%2Fdevices%2FMyDevice&sig=pA3ZJVc6BD7eCt59ySawBn%2BbZJZDzgSa2AEF6Tj2d%2FY%3D&se=1755875053"
AT+CMQTTCONNECT=0,"tcp://xxxxxxx.azure-devices.net:8883",60,1,"xxxxxxx.azure-devices.net/MyDevice/?api-version=2021-04-12","SharedAccessSignature sr=xxxxxxx.azure-devices.net%2Fdevices%2FMyDevice&sig=pA3ZJVc6BD7eCt59ySawBn%2BbZJZDzgSa2AEF6Tj2d%2FY%3D&se=1755875053"

PB DONE

SMS DONE

OK

+CMQTTCONNECT: 0,0successfully.
AT+CMQTTDISC?

AT+CMQTTDISC?

+CMQTTDISC: 0,0
+CMQTTDISC: 1,1

OK
MQTT has connected!
AT+CMQTTSUB=0,39,0,0
AT+CMQTTSUB=0,39,0,0

>devices/MyDevice/messages/devicebound/#

OK

+CMQTTSUB: 0,0
AT+CMQTTTOPIC=0,33
AT+CMQTTTOPIC=0,33

```