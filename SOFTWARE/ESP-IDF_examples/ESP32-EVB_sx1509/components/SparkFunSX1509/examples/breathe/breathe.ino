/*************************************************************
breathe.ino
SparkFun SX1509 I/O Expander Example: breathe output
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example demonstrates the SX1509's set-it-and-forget-it
breathe function. The SX1509 will pulse an LED, smoothly
ramping its brightness up-then-down. We'll set the pin up as
an ANALOG_OUTPUT, and call io.breathe() all in setup(), then
watch the LED pulse by itself in loop().

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Breadboard
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  15 --------------------------------- LED+
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

// SX1509 Pin definition:
const byte SX1509_LED_PIN = 15; // LED to SX1509's pin 15

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

  // Use the internal 2MHz oscillator.
  // Set LED clock to 500kHz (2MHz / (2^(3-1)):
  io.clock(INTERNAL_CLOCK_2MHZ, 3);

  // To breathe an LED, make sure you set it as an
  // ANALOG_OUTPUT, so we can PWM the pin:
  io.pinMode(SX1509_LED_PIN, ANALOG_OUTPUT);

  // Breathe an LED: 1000ms LOW, 500ms HIGH,
  // 500ms to rise from low to high
  // 250ms to fall from high to low
  io.breathe(SX1509_LED_PIN, 1000, 500, 500, 250);
  // The timing parameters are in milliseconds, but they
  // aren't 100% exact. The library will estimate to try to
  // get them as close as possible. Play with the clock
  // divider to maybe get more accurate timing.
}

void loop()
{
  // Enjoy your hypnotically breathing LED!
}
