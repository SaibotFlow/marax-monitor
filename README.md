# MaraX Monitior - Visualisation for the Lelit Mara X (V2) with an automatic Shot-Timer
Hello everyone! This project is for anyone who owns a Lelit Mara X (V2) espresso machine. When looking for a new espresso machine, one feature I always wanted was a shot timer. Usually the machines are beautifully designed so I didn't want to ruin the look by adding a $5 kitchen timer to the machine.
So far so good. Now you are going to say : Dude the MaraX doesnt have a Shot-Timer?! Yes, you are right but the MaraX has a serial interface!
As a developer and tinkerer a ready to use serial interface is more interessting then a ready to use Shot-Timer ;)

![Display](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/DisplayInAction.png?raw=true)
 
# Parts

- Arduino Nano (You can use any other equivalent board)
- Breadboard
- Jumper-Wires
- 0,96" SSD1306 OLED Display

# Structure & Connections

![Fritzing](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Schema.png?raw=true)

# The Interface

The MaraX has a 6-PIN Connector for the serial interface on the bottom. We only use **PIN-3** (Mara RX - Black Wire - Green Rectangle) and **PIN-4** (Mara TX - Orange Wire - White Rectangle). For testing you can use the jumper-wires later you should replace them with temperature protected cables. 
 - **PIN3 Mara** to  **D6**
 - **PIN4 Mara** to **D5**

RX Mara to TX Arudino
TX Mara to RX Arduino
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
|Pin Display|Pin Arduino Nano|
|--|--|
| VIN | 3,3V or 5V |
| GND| GND |
| SCL| A5 |
| SDA| A4 |

The reset pin is 0x3D or 0x3C. You can recognize it by the solder joint on the back side

![BackSideDisplay](https://github.com/SaibotFlow/marax-monitor/blob/main/assets/Backside.png?raw=true)

## Libaries
 - Wire.h
 - Adafruit_GFX.h
 - Adafruit_SSD1306.h
 - SoftwareSerial.h
