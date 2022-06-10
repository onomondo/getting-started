# Onomondo getting started kit. 

Example forked from [esp-idf repo](https://github.com/espressif/esp-idf/tree/master/examples/protocols/pppos_client). 

## What is this?
The kit is designed to showcase some of the features provided by Onomondo. Already a customer? Head over to [app.onomondo.com/getting-started](https://app.onomondo.com/getting-started) to get a guided tour of the platform and to see the data packets in real-time. 

![test2](https://user-images.githubusercontent.com/46489969/173022911-4be67fd5-9a36-4a84-95a0-610666c26827.gif)


### Onomondo Connectors 
Specifically, this example is powered by the Onomondo Connectors. It removes complexity (i.e. Cloud SDK) from the device and let the network handle it. Check out the code snippet below - would you have guessed that this is fully encrypted and integrated into the *Azure IoT Hub*? Get more info on Connectors [here](https://help.onomondo.com/what-are-onomondo-connectors). 

```c
//init cellular connectivity
esp_err_t status = initCellular(SIM7xxx, 0);
...
...
// setup of sensor
...
...
// open a socket to a non-existing server
status = openSocket("1.2.3.4", 4321);

// payload formatting
sprintf(payload, "{\"battery\": %f,\"signal\": %f,\"temperature\": %f}", battery, signal, temp);

// transmit payload
status = sendData(payload, strlen(payload), 0);
```

This it what happens on the platform: 
<!-- ![Onomondo Live Monitor](docs/typicalData.png?raw=true "Onomondo Live Monitor") -->

<img src="docs/typicalData.png?raw=true" width="800" />

At this point the data has not yet reached any public networks. Since the SIM has a Connector configured, the payload gets neatly wrapped up and securely handed over to *Azure IoT Hub* (but not limited to *Azure*: Want a webhook event: ✔, any major cloud provider ✔, custom TLS endpoint ✔).    

This is from the *Azure Logs*:

<img src="docs/dataFromAzure.png?raw=true" width="800" />

Notice how the network has added a few fields: A timestamp and a sim_id so the device can be identified. In addition to the extra fields everything has been fully encrypted. 

## How to use. 

The kit is fully programmed when you get it. However, you are encouraged to modify the example we have provided. 

First time use: Flick the power switch and the device should come online. The device will search for networks, register on a network and establish a data connection. Touch the touch-pad on the front to transmit a message with the current temperature, battery voltage and last known signal level. After a while the device will enter a low power state. Give it a shake to wake it up again. 

### Get the demo tour
On [app.onomondo.com/getting-started](https://app.onomondo.com/getting-started) you'll get the full tour. 


### Flash a precompiled firmware
In case the stock firmware gets overwritten, you can easily flash the device with a precompiled binary. Download the *ESP flash download tool* [here](https://www.espressif.com/en/support/download/other-tools?keys=download+tool), download the precompiled firmware (**onomondo_getting_started.bin**) from the repo and flash the device. 

These procompiled binaries will in the future include firmware to assist in debugging connectivity with Onomondo. 

### Build and Flash Modified firmware. 

Run `idf.py --ccache -p PORT build flash monitor` to build and flash the project..

(To exit the serial monitor, type ``Ctrl-]``.)

See the [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects. 


## Troubleshooting
If the device fails to connect (red flashing LED).  
1. Is the sim activated
2. Attaching for the first time can be slow in some cases. 
3. Is the antenna plugged in?
4. Is there sufficient charge on the battery? If the battery is discharged the device won't turn on until it has been plugged into a charger. 

Check the Network logs on the Onomondo platform. Does the platform report any activity?

Plug the device into you computer (use the port marked as MCU Debug on the back). Have a look at the out below and see if anything stands out. Does the modem reponds to any commands? When does it stop responding? 



## Example Output


### SIM7070G Output
```bash
I (451) main: Cpu freq: 160 MhZ
I (451) ACC: Config i2c param
I (461) ACC: Install i2c driver
I (461) ACC: Reading register....
I (481) main: Temperature: 26.500000
I (481) gpio: GPIO[4]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (481) gpio: GPIO[5]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (491) gpio: GPIO[25]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (511) touch_driver: test init: touch pad [6] val is 686
I (511) uart: queue free spaces: 30
I (511) sim800: PWR pin 27
I (511) gpio: GPIO[27]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (521) AT CMD: AT
E (621) esp-modem: esp_modem_dte_send_cmd(352): process command timeout
E (621) dce_service: esp_modem_dce_power_test(251): send command failed
I (721) AT CMD: AT
I (721) AT RESP: AT

E (721) esp-modem: esp_dte_handle_line(136): handle line failed
E (721) esp-modem: AT

I (721) AT RESP: OK

I (721) sim800: Modem is on!
I (731) AT CMD: AT
I (731) AT RESP: AT

E (731) esp-modem: esp_dte_handle_line(136): handle line failed
E (741) esp-modem: AT

I (741) AT RESP: OK

I (751) AT CMD: ATE0
I (751) AT RESP: ATE0

E (751) esp-modem: esp_dte_handle_line(136): handle line failed
E (761) esp-modem: ATE0

I (761) AT RESP: OK

I (761) AT CMD: AT+CGMM
I (771) AT RESP:

I (771) AT RESP: SIMCOM_SIM7070

I (781) AT RESP:

I (781) AT RESP: OK

I (781) AT CMD: AT+CGSN
I (791) AT RESP:

I (791) AT RESP: 861340049970997

I (791) AT RESP:

I (791) AT RESP: OK

I (801) AT CMD: AT+COPS?
I (801) AT RESP:

I (801) AT RESP: +COPS: 1,0,"TDC",7

I (811) AT RESP:

I (811) AT RESP: OK

I (811) AT CMD: AT+CREG?
I (821) AT RESP:

I (821) AT RESP: +CREG: 0,5

I (821) AT RESP:

I (831) AT RESP: OK

I (831) AT CMD: AT+CBANDCFG="CAT-M",3,8,20
I (841) AT RESP:

I (841) AT RESP: OK

I (841) AT CMD: AT+CNMP=38
I (851) AT RESP:

I (851) AT RESP: OK

I (851) esp-modem: No handler for line: OK

E (851) esp-modem: OK

E (1341) esp-modem: esp_modem_dte_send_cmd(352): process command timeout
I (1341) sim800: Modem INIT OK, 0x3ffbf604
I (1341) AT CMD: AT+IFC=0,0
I (1341) AT RESP:

I (1341) AT RESP: OK

I (1351) AT CMD: AT+CREG?
I (1351) AT RESP:

I (1351) AT RESP: +CREG: 0,5

I (1361) AT RESP:

I (1361) AT RESP: OK

I (1361) cellular: Module: SIMCOM_SIM7070
I (1371) cellular: Operator: "TDC"
I (1371) cellular: IMEI: 861340049970997
I (1381) cellular: IMSI:
I (1381) AT CMD: AT+CSQ
I (1381) AT RESP:

I (1391) AT RESP: +CSQ: 31,99

I (1391) AT RESP:

I (1391) AT RESP: OK

I (1401) cellular: rssi: 31, ber: 99
I (1401) AT CMD: AT+CEDRXS=1,4,"0010"
I (1411) AT RESP:

I (1411) AT RESP: OK

I (1411) AT CMD: AT+CEDRXRDP
I (1421) AT RESP:

I (1421) AT RESP: +CEDRXRDP: 4,"0010","0010","0000"

I (1421) eDRX CHECK: Supported
I (1431) AT RESP:

I (1431) AT RESP: OK

I (1431) AT CMD: AT+CPSMRDP
I (1441) AT RESP:

I (1441) AT RESP: +CPSMRDP: 0,20,14400,0,0,3600

I (1441) PSM CHECK: +CPSMRDP: 0,20,14400,0,0,3600

I (1451) PSM CHECK: 0,20,14400,0,0,3600
I (1451) PSM CHECK: Active: 0
I (1461) AT RESP:

I (1461) AT RESP: OK

I (1461) AT CMD: AT+CPSI?
I (1471) AT RESP:

I (1481) AT RESP: +CPSI: LTE CAT-M1,Online,238-01,0x07D0,12024586,465,EUTRAN-BAND20,6350,5,5,-16,-83,-53,10

E (1481) esp-modem: esp_dte_handle_line(136): handle line failed
E (1491) esp-modem: +CPSI: LTE CAT-M1,Online,238-01,0x07D0,12024586,465,EUTRAN-BAND20,6350,5,5,-16,-83,-53,10

I (1501) AT RESP:

I (1501) AT RESP: OK

I (1501) AT CMD: AT+CPSMRDP
I (1511) AT RESP:

I (1511) AT RESP: +CPSMRDP: 0,20,14400,0,0,3600

I (1521) PSM CHECK: +CPSMRDP: 0,20,14400,0,0,3600

I (1521) PSM CHECK: 0,20,14400,0,0,3600
I (1531) PSM CHECK: Active: 0
I (1531) AT RESP:

I (1531) AT RESP: OK

I (1541) AT CMD: AT+CGDCONT=1,"IP","onomondo"
I (1551) AT RESP:

I (1551) AT RESP: OK

I (1551) dce_service: define pdp context ok
I (1551) sim800: Working mode is changed! :D
I (1561) AT CMD: ATD*99#
I (1561) AT RESP:

I (1561) AT RESP: CONNECT 150000000

I (1671) cellular: Modem PPP Started
I (1671) cellular: PPP state changed event 259
I (1671) cellular: PPP state changed event 262
I (1691) cellular: PPP state changed event 263
I (1691) cellular: PPP state changed event 265
I (1711) esp-netif_lwip-ppp: Connected
I (1711) esp-netif_lwip-ppp: Name Server1: 10.85.59.251
I (1711) esp-netif_lwip-ppp: Name Server2: 10.85.59.242
I (1721) cellular: Modem Connect to PPP Server
I (1721) cellular: ~~~~~~~~~~~~~~
I (1731) cellular: IP          : 100.68.57.55
I (1731) cellular: Netmask     : 255.255.255.255
I (1741) cellular: Gateway     : 10.64.64.64
I (1741) cellular: Name Server1: 10.85.59.251
I (1751) cellular: Name Server2: 10.85.59.242
I (1751) cellular: ~~~~~~~~~~~~~~
I (1761) cellular: GOT ip event!!!
I (1761) cellular: PPP state changed event 266
I (1771) cellular: Socket created, connecting to 1.2.3.4:4321
I (3161) Transmit: {"battery": 0.284000,"signal": 31.000000,"temperature": 26.625000}
W (20361) ACC: Wakeup interrupt NOT active...,00000000
```

