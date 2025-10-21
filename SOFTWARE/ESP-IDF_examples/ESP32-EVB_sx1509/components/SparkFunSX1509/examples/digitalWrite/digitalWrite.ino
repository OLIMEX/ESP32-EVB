/*************************************************************
digitalWrite.ino
SparkFun SX1509 I/O Expander Example: digital out (digitalWrite)
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This simple example demonstrates the SX1509's digital output 
functionality. Attach an LED to SX1509 IO 15, or just look at
it with a multimeter. We're gonna blink it!

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Breadboard
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  15 -------------------------------- LED+
		                                 LED- -/\/\/\- GND
                                                330

Development environment specifics:
	IDE: Arduino 1.6.5
	Hardware Platform: Arduino Uno
	SX1509 Breakout Version: v2.0

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.
*************************************************************/

#include <Wire.h>           // Include the I2C library (required)
#include <SparkFunSX1509.h> //Click here for the library: http://librarymanager/All#SparkFun_SX1509

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
SX1509 io;                        // Create an SX1509 object to be used throughout

// SX1509 pin definitions:
const byte SX1509_LED_PIN = 15; // LED connected to pin 15

void setup()
{
  Serial.begin(115200);
  Serial.println("SX1509 Example");

  Wire.begin();

  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (io.begin(SX1509_ADDRESS) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509.");
    while (1)
      ; // If we fail to communicate, loop forever.
  }

  // Call io.pinMode(<pin>, <mode>) to set an SX1509 pin as
  // an output:
  io.pinMode(SX1509_LED_PIN, OUTPUT);
}

void loop()
{
  // It's blinken time!
  // Call io.digitalWrite(<pin>, <HIGH | LOW>) to set a SX1509
  // output pin as either 3.3V or 0V.
  io.digitalWrite(SX1509_LED_PIN, HIGH);
  delay(500);                           // Delay half-a-second
  io.digitalWrite(SX1509_LED_PIN, LOW); // Set the I/O low
  delay(500);                           // Delay half-a-second
}
