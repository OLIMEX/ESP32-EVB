/*
 WiFi Web Server for ESP32-EVB and WATER-VALVE-6-5MM-12VDC
 from www.olimex.com

 A simple web server that lets turn on/off the relay 1
 through a button or through delay command on a specific time.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 created on Olimex ESP32-EVB, 5 december 2012 by Stanimir Petev @ olimex
 this software is far for complete and just for demo purposes, feel free to use and change

 The project is based on the example of Tom Igoe (SimpleWiFiServer) on 25 Nov 2012
 and ported for sparkfun esp32 on 31.01.2017 by Jan Hendrik Berlin
 */

#include <WiFi.h>

#include <NTPtimeESP.h>

#define DEBUG_ON

#define RELAY  32

NTPtime NTPch("ch.pool.ntp.org");   // Choose server pool as required

strDateTime dateTime;

const char* ssid     = "your_wifi_ssid";	//enter here your WiFi SSID
const char* password = "your_wifi_password";	//enter here your WiFi password

WiFiServer server(80);

// Client variables 
char linebuf[80];
int charcount=0;

int Hour=12, Minute=0, Second=0;
int SetHourON=0, SetMinuteON=0, SetSecondON=0;
int SetHourOFF=0, SetMinuteOFF=0, SetSecondOFF=0;

byte actualHour = dateTime.hour;
byte actualMinute = dateTime.minute;
byte actualSecond = dateTime.second;
int actualyear = dateTime.year;
byte actualMonth = dateTime.month;
byte actualday = dateTime.day;
byte actualdayofWeek = dateTime.dayofWeek;

extern const char WebPage [168][80];

void Print_Webpage (WiFiClient &client)
{
  int i;
  char Buff[100], Parameter[10];
  for (i=0; WebPage[i][0]; i++)
  {
    switch (i)
    {
      // list of lines where parameters should be placed
      case 125: // <div class=\"left\">Status <b class=%s</b></div>",
        if (digitalRead (RELAY))
          sprintf (Parameter, "\"%s\">%s", "on", "ON");
        else
          sprintf (Parameter, "\"%s\">%s", "off", "OFF");
        break;
      case 126: // <div class=\"right\">Current time <b>%s</b></div>",
        sprintf (Parameter, "%02d:%02d:%02d", actualHour, actualMinute, actualSecond);
        break;
      case 130: // <div class=\"value\">%s</div>",
        sprintf (Parameter, "%02d", Hour);
        break;
      case 137: // <div class=\"value\">%s</div>",
        sprintf (Parameter, "%02d", Minute);
        break;
      case 144: // <div class=\"value\">%s</div>",
        sprintf (Parameter, "%02d", Second);
        break;
      case 153: // <div class=\"value\">%s</div>",
        sprintf (Parameter, "%02d:%02d:%02d", SetHourON, SetMinuteON, SetSecondON);
        break;
      case 157: // <div class=\"value\">%s</div>",
        sprintf (Parameter, "%02d:%02d:%02d", SetHourOFF, SetMinuteOFF, SetSecondOFF);
        break;
      default:
        Parameter[0] = 0;
        break;
    }
    sprintf (Buff, WebPage[i], Parameter);
    client.println (Buff);
  }
}

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
}

long int NTPLastUpdate, LastUpdate, Time;
bool NTPUpdateFlag=true;
void loop()
{
  Time = millis ();
  if (Time - NTPLastUpdate > 120000)
    NTPUpdateFlag = true;
  if (NTPUpdateFlag)
  {
    dateTime = NTPch.getNTPtime(1.0, 1);
  
    // check dateTime.valid before using the returned time
    // Use "setSendInterval" or "setRecvTimeout" if required
    if(dateTime.valid)
    {
      NTPUpdateFlag = false;
      NTPch.printDateTime(dateTime); 
      actualHour = dateTime.hour;
      actualMinute = dateTime.minute;
      actualSecond = dateTime.second;
      actualyear = dateTime.year;
      actualMonth = dateTime.month;
      actualday =dateTime.day;
      actualdayofWeek = dateTime.dayofWeek;
      NTPLastUpdate = millis();
      LastUpdate = millis();
    }
  }

  if (Time - LastUpdate > 1000)
  {
    LastUpdate = Time;
    actualSecond++;
    if (actualSecond >= 60)
    {
      actualSecond = 0;
      actualMinute++;
      if (actualMinute >= 60)
      {
        actualMinute = 0;
        actualHour++;
        if (actualHour >= 24)
          actualHour = 0;
      }
    }
  }
  
  if ((actualHour == SetHourON) && (actualMinute == SetMinuteON) && (actualSecond == SetSecondON))
    digitalWrite(RELAY, HIGH);
  if ((actualHour == SetHourOFF) && (actualMinute == SetMinuteOFF) && (actualSecond == SetSecondOFF))
    digitalWrite(RELAY, LOW);

  // listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          Print_Webpage (client);
          break;
        }
        
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /setON") > 0)
          {
            SetHourON = Hour;
            SetMinuteON = Minute;
            SetSecondON = Second;
          }
          else
          if (strstr(linebuf,"GET /setOFF") > 0)
          {
            SetHourOFF = Hour;
            SetMinuteOFF = Minute;
            SetSecondOFF = Second;
          }
          else
          if (strstr(linebuf,"GET /off") > 0)
          {
            digitalWrite (RELAY, LOW);
          }
          else
          if (strstr(linebuf,"GET /on") > 0)
          {
            digitalWrite (RELAY, HIGH);
          }
          else
          if (strstr(linebuf,"GET /h+") > 0)
          {
            Hour = (Hour + 1) % 24;
          }
          else
          if (strstr(linebuf,"GET /h-") > 0)
          {
            Hour = (Hour + 24 - 1) % 24;
          }
          else
          if (strstr(linebuf,"GET /m+") > 0)
          {
            Minute = (Minute + 1) % 60;
          }
          else
          if (strstr(linebuf,"GET /m-") > 0)
          {
            Minute = (Minute + 60 - 1) % 60;
          }
          else
          if (strstr(linebuf,"GET /s+") > 0)
          {
            Second = (Second + 1) % 60;
          }
          else
          if (strstr(linebuf,"GET /s-") > 0)
          {
            Second = (Second + 60 - 1) % 60;
          }

          
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        }
        else
        if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

