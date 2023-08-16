# MaraX Monitior - Visualisation for the Lelit Mara X (V2) with an automatic Shot-Timer



Hello everyone! This project is for anyone who owns a Lelit Mara X (V2) espresso machine. When looking for a new espresso machine, one feature I always wanted was a shot timer. Usually the machines are beautifully designed so I didn't want to ruin the look by adding a $5 kitchen timer to the machine.
So far so good. Now you are going to say : Dude the MaraX doesnt have a Shot-Timer?! Yes, you are right but the MaraX has a serial interface!
As a developer and tinkerer a ready to use serial interface is more interessting then a ready to use Shot-Timer ;)

If you like my project, please give me a star and if you want, you can also buy me a coffee :-)

<a href="https://www.buymeacoffee.com/saibotFlow386" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>

![Display](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/DisplayInAction.png?raw=true)
 
## Updates
- New file include *marax_monitor_wemos_wifi.ino* included.
- Supporting Wemos D1 Mini Pro (ESP8226) with WLAN.
- Sending Push-Notification when machine is hot via Pushsafer-Service

# How to use this repo?
This repo should help you to implement your own solution. I implemented my personal solution, probably it fits to you. If not you can change the code yourself. I commented the code a bit to show what happens where.

There are 2 Files
- marax_monitor.ino
- marax_monitor_wemos_wlan.ino

 The *marax_monitor.ino* is my first solution. I used an Arduino Nano for that.
 
 The *marax_monitor_wemo_wlan.ino* is a more smart solution. In that case smart means: It sends a push notification when the machine is ready (not  hot). For me the machine is ready when: 
 
 `data.steamTemp == data.targetSteamTemp && data.hxTemp > 90 && initialPushSent == false`
 
  I used a Wemos D1 Mini Pro - small board and WLAN out of the box. You could easily extend this solution e.g. adding a WebUi or pushing the data to a server. There are many ideas. 

 ## Secret data
 SSID, WLAN-Password and API-Key are sensitive data and should not be pushed in an open repository.
 So I created a *.gitignore* which is ignoring the file **secrets.h** in the sketch-folder. In this file you can put your secrets like that:


```
 #include WLAN_SSID "SSID"
 #inlcude WLAN_PASS "PASS"
 #inlcude API_KEY "MyKey"
```

 ## Push Service
 For sending push notifications I am using the PushSafer-Service. It is easy to use and I already had experience with it. 
 But you could change your service as well. So you have to change the *sendPushSaferMessage* Method, the host and the port.
 
 **Note**:
 
 If you dont want to send any push notifications you can set
 `bool initialPushSent` to `true` then the programm will not send any push notifications.
 You can change *title*, *message*, *icon* etc. here:
 ```
  String pushTitle = "Lelit%20Mara%20X";
  String pushMessage = "I%20am%20hot%20for%20you!";
  String pushIcon= "62";
```
 

# Parts

- Arduino Nano / Wemos D1 Mini Pro (You can use any other equivalent board)
- Breadboard
- Jumper-Wires
- 0,96" SSD1306 OLED Display

# Structure & Connections
**Important**: If you use a Wemos or any ESP8266 board you have to connect:
- **SCK/SCL** to **D1** 
- **SDA** to **D2**

Arudino is using A4 and A5 Pin
- **SCK/SCL** to **A4**
- **SDA** to **A5**
- 
![Fritzing](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Schema.png?raw=true)

# The Interface

The MaraX has a 6-PIN Connector for the serial interface on the bottom. We only use **PIN-3** (Mara RX - Black Wire - Green Rectangle) and **PIN-4** (Mara TX - Orange Wire - White Rectangle). For testing you can use the jumper-wires later you should replace them with temperature protected cables. 
 - **PIN3 Mara** to  **D6**
 - **PIN4 Mara** to **D5**

*RX Mara to TX Board*

*TX Mara to RX Board*

You will receive the following data each ~400ms.

![enter image description here](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Bottom.png?raw=true)

The serial interface itself

![enter image description here](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Connector.png?raw=true)
  
  ***Note**: I also did some tests with a reed sensor and the vibration pump. However its not necessary for the MaraX the interface provides the information about the status of the pump. Maybe I will do another project for a Shot-Timer only for espresso machines with a vibration pump only :-)*

## The Data

Example Data: **C1.06,116,124,093,0840,1,0\n**
- The delimitter is: **,**
- The end of data is: **\n**
- Length: **26**

|Data|Description |
|--|--|
| C | Machine-Mode: C = CoffeeMode; V = Vapour/SteamMode |
| 1.06 | Firmware |
| 116 | Current Steam Temperature in Celsius |
| 124| Target Steam Temperature in Celsius |
| 093| Curent Hx Temperature in Celsius |
| 0840| Countdown Boost-Mode |
| 1| Heat state (0 = off; 1= on) |
| 0| Pump state (0 = off; 1= on) |


# Display Wiring
|Pin Display|Pin Board(Arduino/ESP8266|
|--|--|
| VIN | 3,3V or 5V |
| GND| GND |
| SCL| A5/D2 |
| SDA| A4/D1 |

The reset pin is 0x3D or 0x3C. You can recognize it by the solder joint on the back side

![BackSideDisplay](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Backside.png?raw=true)

## Libaries
 - Wire.h
 - Adafruit_GFX.h
 - Adafruit_SSD1306.h
 - SoftwareSerial.h

Additionaly for Wemos / Esp8266
 - ArduinoHttpClient
 - Esp8266WiFi
 - SPI.h
