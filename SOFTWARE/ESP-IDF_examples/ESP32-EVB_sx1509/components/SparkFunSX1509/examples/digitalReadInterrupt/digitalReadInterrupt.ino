/*************************************************************
digitalReadInterrupt.ino
SparkFun SX1509 I/O Expander Example: digital in w/ interrupt
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example combines the SX1509's digitalRead and interrupt
output functionalities. When a button connected to pin 0 is
pressed, the SX1509 will generate an active-low interrupt,
signalling to the Arduino that a button has been pressed.

After uploading the sketch, open your serial monitor and 
set it to 115200 baud.

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Breadboard
	      INT --------------- 2
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  0 ---------------------------------BTN----GND

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
const byte SX1509_BUTTON_PIN = 0; // IO 0 connected to button

// Arduino Pins (not SX1509!)
const byte ARDUINO_INT_PIN = 2; // SX1509 int output to D2

// Global variables:
bool buttonPressed = false; // Track button press in ISR

void setup()
{
  // Serial is used in this example to display the input
  // value of the SX1509_INPUT_PIN input:
  Serial.begin(115200);
  Serial.println("SX1509 Example");

  Wire.begin();

  // Call io.begin(<address>) to initialize the SX1509. If
  // it successfully communicates, it'll return 1.
  if (io.begin(SX1509_ADDRESS) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509.");
    while (1)
      ; // If we fail to communicate, loop forever.
  }

  // Use io.pinMode(<pin>, <mode>) to set our button to an
  // input with internal pullup resistor activated:
  io.pinMode(SX1509_BUTTON_PIN, INPUT_PULLUP);

  // Use io.enableInterrupt(<pin>, <signal>) to enable an
  // interrupt on a pin. The <signal> variable can be either
  // FALLING, RISING, or CHANGE. Set it to falling, which will
  // mean the button was pressed:
  io.enableInterrupt(SX1509_BUTTON_PIN, FALLING);

  // The SX1509 has built-in debounce features, so a single
  // button-press doesn't accidentally create multiple ints.
  // Use io.debounceTime(<time_ms>) to set the GLOBAL SX1509
  // debounce time.
  // <time_ms> can be either 0, 1, 2, 4, 8, 16, 32, or 64 ms.
  io.debounceTime(32); // Set debounce time to 32 ms.

  // After configuring the debounce time, use
  // debouncePin(<pin>) to enable debounce on an input pin.
  io.debouncePin(SX1509_BUTTON_PIN); // Enable debounce

  // Don't forget to configure your Arduino pins! Set the
  // Arduino's interrupt input to INPUT_PULLUP. The SX1509's
  // interrupt output is active-low.
  pinMode(ARDUINO_INT_PIN, INPUT_PULLUP);

  // Attach an Arduino interrupt to the interrupt pin. Call
  // the button function, whenever the pin goes from HIGH to
  // LOW.
  attachInterrupt(digitalPinToInterrupt(ARDUINO_INT_PIN),
                  button, FALLING);
}

void loop()
{
  if (buttonPressed) // If the button() ISR was executed
  {
    // read io.interruptSource() find out which pin generated
    // an interrupt and clear the SX1509's interrupt output.
    unsigned int intStatus = io.interruptSource();
    // For debugging handiness, print the intStatus variable.
    // Each bit in intStatus represents a single SX1509 IO.
    Serial.println("intStatus = " + String(intStatus, BIN));

    // If the bit corresponding to our button IO generated
    // the input:
    if (intStatus & (1 << SX1509_BUTTON_PIN))
    {
      Serial.println("Button pressed!"); // Print a message.
    }

    buttonPressed = false; // Clear the buttonPressed flag
  }
}

// button() is an Arduino interrupt routine, called whenever
// the interrupt pin goes from HIGH to LOW.
void button()
{
  buttonPressed = true; // Set the buttonPressed flag to true
  // We can't do I2C communication in an Arduino ISR. The best
  // we can do is set a flag, to tell the loop() to check next
  // time through.
}
