/*************************************************************
clock.ino
SparkFun SX1509 I/O Expander Example: clock output
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example demonstrates the SX1509's clock output
functionality. The OSC pins (OSCIO in the datasheet), can be
configured as either a clock input, clock output, or an
extra output!

Hardware Hookup:
	SX1509 Breakout ------ Arduino
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  OSC ------------- Check with a multimeter or o-scope

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

  // Configure clock:
  // - INTERNAL_CLOCK_2MHZ: Set clock to internal 2MHz
  // - 2: Set LED clock to divide by 2^(2-1) (2)
  // - OUTPUT: Configure OSCIO pin as a clock OUTPUT
  // - outputFreq: Sets the frequncy of output
  //   - 0: 0Hz LOW
  //   - 0x1-0xE: fOSCout = Fosc / 2 ^ (outputFreq - 1) Hz
  //   - 0xF: 0Hz HIGH
  byte outputFreq = 6; // Set output freq. to 62.5 kHz
  io.clock(INTERNAL_CLOCK_2MHZ, 2, OUTPUT, outputFreq);
}

void loop()
{
}
