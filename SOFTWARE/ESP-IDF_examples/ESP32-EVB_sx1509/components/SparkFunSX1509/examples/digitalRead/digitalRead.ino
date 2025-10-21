/*************************************************************
digitalRead.ino
SparkFun SX1509 I/O Expander Example: digital in (digitalRead)
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example demonstrates the SX1509's digitalRead
functionality. A pin can either be set as an INPUT or
INPUT_PULLUP. We'll attach an active-low button to an 
INPUT_PULLUP input, then whenever the button read's LOW, we'll
read the state of another INPUT pin.

After uploading the sketch, open your serial monitor and set 
it to 115200 baud.

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Breadboard
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  0 ---------------------------------]BTN[----GND
		  8 -----------------------------Jumper (GND or 3.3V)

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

// SX1509 Pins:
const byte SX1509_BUTTON_PIN = 0; // Active-low button
const byte SX1509_INPUT_PIN = 8;  // Floating or jumpered input

void setup()
{
  // Serial is used in this example to display the input value
  // of the SX1509_INPUT_PIN input:
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

  // use io.pinMode(<pin>, <mode>) to set input pins as either
  // INPUT or INPUT_PULLUP. Set up a floating (or jumpered to
  // either GND or 3.3V) pin to an INPUT:
  io.pinMode(SX1509_INPUT_PIN, INPUT);
  // Use a pull-up resistor on the button's input pin. When
  // the button is pressed, the pin will be read as LOW:
  io.pinMode(SX1509_BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  // use io.digitalRead(<pin>) to check if an SX1509 input
  // pin is either HIGH or LOW.
  if (io.digitalRead(SX1509_BUTTON_PIN) == LOW)
  {
    // If the button is pressed (the pin reads LOW)
    // Print the status of the other pin:
    Serial.print("SX1509_INPUT_PIN status: ");
    // Read the pin to print either 0 or 1
    Serial.println(io.digitalRead(SX1509_INPUT_PIN));
  }
}
