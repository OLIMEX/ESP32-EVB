/*
 Simple example for ESP32-EVB rev.I with MDO-IO2 rev.B connected to the UEXT
 In order to compile and upload you need to install the Espressif package.
 To do so:
 1) go to Main menu --> File --> Preferences
 2) In the field "Additional Boards Manager URLs" add this URL: "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" (you can use the button on the right side and add it on a new line)
 3) Go to Main Menu --> Tools --> Boards --> Boards manager...
 4) Look for esp32 package by Espressif Systems and install the latest version

 When all this is done select the board from Main menu --> Tools --> Board --> ESP32 Arduino* --> Olimex ESP32-EVB
 
 * on newer versions of Arduino the third party packages create submenu of the boards for better structure. If you are using older versions (older than 1.8.18) there will be no such submenu and after Board look for Olimex ESP32-EVB

 Select the USB port on which the ESP32-EVB has connected, compile and upload

 Demo description:
 When you press the ESP32-EVB button a commands for reading ID and set relay will be triggered using I2C protocol. You can see the ID on the serial monitor (press Ctrl+Shift+M to open it) 

 2022/05/11
 Stanimir Petev, Olimex
*/

#include <Wire.h>

#define MOD_IO2_ADDRESS     0x21
#define MOD_IO2_READ_ID     0x20
#define MOD_IO2_SET_RELAYS  0x40

void setup()
{
  pinMode (KEY_BUILTIN, INPUT);
  Serial.begin (115200);
  Wire.begin ();
}

void loop()
{
  static int Value = 0, Key_Released = 1;

  if (!digitalRead(KEY_BUILTIN) && Key_Released)
  {
    Key_Released = 0;
    Wire.beginTransmission (MOD_IO2_ADDRESS);
    Wire.write (MOD_IO2_READ_ID);
    Wire.endTransmission ();
    Wire.requestFrom (MOD_IO2_ADDRESS, 1, 0);
    Serial.print ("ID is 0x"); Serial.println (Wire.read (), HEX);
  
    Wire.begin ();
    Wire.beginTransmission (MOD_IO2_ADDRESS);
    Wire.write (MOD_IO2_SET_RELAYS);
    Wire.write (Value);
    Wire.endTransmission ();
    Value = (Value + 1) & 3;
  }

  if (digitalRead(KEY_BUILTIN))
    Key_Released = 1;
}
