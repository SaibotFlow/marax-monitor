//Includes
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include "secrets.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

//Defines
#define SCREEN_WIDTH    128     //Width in px 
#define SCREEN_HEIGHT   64      // Height in px
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C    // or 0x3D Check datasheet or Oled Display
#define BUFFER_SIZE     32

//Secrets from secrets.h
//Will not be pushed into the repo you have to create the file by yourself
String ssid = WLAN_SSID;
String pass = WLAN_PASS;
String apiKey = API_KEY;
String pushServiceHost = "pushsafer.com";
int pushServicePort = 80;

//Instances
WiFiClient wifi;
HttpClient client = HttpClient(wifi, pushServiceHost, pushServicePort);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//orange PIN 4 Mara TX to Arduino RX D5
//black  PIN 3 Mara RX to Arduino TX D6
SoftwareSerial mySerial(D5, D6);


//Internals
long lastMillis;
int seconds = 0;
int lastSeconds = 0;
long serialTimeout = 0;
char buffer[BUFFER_SIZE];
int length = 0;

bool initialPushSent = false;
String pushTitle = "Lelit%20Mara%20X";
String pushMessage = "I%20am%20hot%20for%20you!";
String pushIcon= "62";

//Mara Data
struct MaraData {
  bool updated = false;
  String mode;
  String firmware;
  int hxTemp;
  int steamTemp;
  int targetSteamTemp;
  int boostCountdown;
  int pumpState;
  int heatState;
};


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  
  delay(10);
  //Setup Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //Loop
  }
  display.display();
  delay(500);
  display.clearDisplay();
  scrollText();


  //Setup WiFi
   Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Setup serial interface
  
  Serial.println("Setup done");
  
}



MaraData getMaraData()
{
  /*
    Example Data: C1.06,116,124,093,0840,1,0\n every ~400-500ms
    Length: 26
    [Pos] [Data] [Describtion]
    0)      C     Coffee Mode (C) or SteamMode (V)
    -        1.06  Software Version
    1)      116   current steam temperature (Celsisus)
    2)      124   target steam temperature (Celsisus)
    3)      093   current hx temperature (Celsisus)
    4)      0840  countdown for 'boost-mode'
    5)      1     heating element on or off
    6)      0     pump on or off
  */
  MaraData tmp;
  Serial.print("Serial data avail:");
  Serial.println(mySerial.available());
  while (mySerial.available() >= 26)
  {
    serialTimeout = millis();
    char rcv = mySerial.read();
    if (rcv != '\n')
    {
      buffer[length++] = rcv;
    }
    else
    {
      //Data caught
      length = 0;
      Serial.println(buffer);
      char* ptr = strtok(buffer, ",");
      int idx = 0;
      String data[7];
      if (ptr != NULL)
      { 
          while (ptr != NULL)
          {
            if( idx <= 7)
            {
              data[idx++] = String(ptr);
              ptr = strtok(NULL, ",");
            }
            else
            {
              break;
            }
          }
          tmp.updated = true;
          tmp.mode = data[0].substring(0,1);
          tmp.firmware = data[0].substring(1);
          tmp.steamTemp = data[1].toInt();
          tmp.targetSteamTemp = data[2].toInt();
          tmp.hxTemp = data[3].toInt();
          tmp.boostCountdown = data[4].toInt();
          tmp.heatState = data[5].toInt();
          tmp.pumpState = data[6].toInt();
          return tmp;
      }
    }
  }
    
  //Check for timeout and reset
  if (millis() - serialTimeout > 6000)
  {
    serialTimeout = millis();
    mySerial.write(0x11);
  }
  return tmp;
}


void updateView(int hxTemp, int steamTemp, int pumpState, int heatState, String mode)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  //HX
  display.setCursor(2, 2);
  display.setTextSize(2);
  display.print(hxTemp);
  display.setTextSize(1);
  display.print((char)247);
  display.setTextSize(1);
  display.print("C");
  //Heat
  display.setCursor(2, 30);
  display.print("H");
  if (heatState == 0)
    display.drawCircle(17, 33, 6, WHITE);
  else
    display.fillCircle(17, 33, 5, WHITE);
  display.setCursor(30, 30);
  //Pump
  display.print("P");
  if (pumpState == 0)
    display.drawRect(40, 28, 10, 10, WHITE);
  else
    display.fillRect(40, 28, 10, 10, WHITE);
  //Steam
  display.setCursor(2, 50);
  display.setTextSize(2);
  display.print(steamTemp);
  display.setTextSize(1);
  display.print((char)247);
  display.setTextSize(1);
  display.print("C");
  display.drawLine(55, 0, 55, 68, WHITE);
  display.setCursor(65, 15);
  display.setTextSize(5);
  //Timer
  if (seconds > 3)
  {
    String actual = String(seconds);
    if (actual.length()< 2)
      actual = "0" + actual;
    display.print(actual);
  }
  else if (lastSeconds > 3)
  {
    String last = String(lastSeconds);
    if (last.length()< 2)
      last = "0" + last;
    display.print(last);
  }
  else
  {
    display.print("00");
  }
  //Mode
  display.setTextSize(1);
  display.setCursor(120, 2);
  display.print(mode);
  display.display();
}

void scrollText() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  display.println(F("Loading..."));
  display.display();
}

void sendPushSaferMessage()
{
    String params = "k=" + String(API_KEY) + "&t=" + pushTitle + "&m=" + pushMessage + "&i=" + pushIcon + "&d=a";
    Serial.print("POST Data: ");
    Serial.println(params);
    client.beginRequest();
    client.post("/api");
    client.sendHeader("Content-Type","application/x-www-form-urlencoded");
    client.sendHeader("Content-Length", params.length());
    client.beginBody();
    client.print(params);
    client.endRequest();

    // Reead the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
}


void loop()
{
  //Collect data
  MaraData data = getMaraData();
  if (data.updated == true)
  {
    //Check if machine is ready
    if (data.steamTemp == data.targetSteamTemp && data.hxTemp > 90 && initialPushSent == false)
    {
        sendPushSaferMessage();
        initialPushSent = true; 
    }
    
    //Run Timer
    if (data.pumpState == 1)
    {
      if ( millis() - lastMillis >= 1000)
      {
        lastMillis = millis();
        ++seconds;
        if (seconds > 99)
          seconds = 0;
      }
    }
    else
    {
      if (seconds != 0)
        lastSeconds = seconds;
      seconds = 0;
    }
    updateView(data.hxTemp, data.steamTemp, data.pumpState, data.heatState, data.mode);
  }
  
}
