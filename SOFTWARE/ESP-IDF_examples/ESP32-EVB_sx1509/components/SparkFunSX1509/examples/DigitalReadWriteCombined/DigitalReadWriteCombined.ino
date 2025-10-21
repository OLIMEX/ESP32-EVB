/*************************************************************
  digitalReadWriteCombined.ino
  SparkFun SX1509 I/O Expander Example: digital I/O (digitalRead/digitalWrite)
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: September 21, 2015
  https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

  This example demonstrates the SX1509's digitalRead and digitalWrite
  functionality. We'll attach an active-low button to an
  INPUT_PULLUP input and attach an LED to a pin set as OUTPUT.
  Then whenever the button read's LOW, we'll toggle the LED.
  Note that the code will wait until the button is released
  before reading the SX1509 pins again.

  After uploading the sketch, open your serial monitor and set
  it to 115200 baud.

  Hardware Hookup:
  SX1509 Breakout ------ Arduino -------- Breadboard
        GND -------------- GND
        3V3 -------------- 3.3V
        SDA ------------ SDA (A4)
        SCL ------------ SCL (A5)
        0 ---------------------------------]BTN[----GND
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
// Note: these aren't Arduino pins. They're the SX1509 I/O:
const byte SX1509_LED_PIN = 15; // LED connected to 15 (source ing current)
const byte SX1509_BUTTON_PIN = 0; // Button connected to 0 (Active-low button)

bool ledState = false;

void setup()
{
  // Serial is used in this example to display the input value
  // of the SX1509_INPUT_PIN input:
  Serial.begin(115200);
  Serial.println("SX1509 Example");

  Wire.begin(); //Initialize I2C bus

  pinMode(13, OUTPUT); // Use pin 13 LED as debug output
  digitalWrite(13, LOW); // Start it as low


  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (io.begin(SX1509_ADDRESS) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509.");
    digitalWrite(13, HIGH); // If we failed to communicate, turn the pin 13 LED on
    while (1)
      ; // If we fail to communicate, loop forever.
  }

  // Call io.pinMode(<pin>, <mode>) to set any SX1509 pin as
  // either an INPUT, OUTPUT, INPUT_PULLUP, or ANALOG_OUTPUT
  // Set output for LED:
  io.pinMode(SX1509_LED_PIN, OUTPUT);
  // Use a pull-up resistor on the button's input pin. When
  // the button is pressed, the pin will be read as LOW:
  io.pinMode(SX1509_BUTTON_PIN, INPUT_PULLUP);

  // Blink the LED a few times before we start:
  for (int i = 0; i < 5; i++)
  {
    // Use io.digitalWrite(<pin>, <LOW | HIGH>) to set an
    // SX1509 pin either HIGH or LOW:
    io.digitalWrite(SX1509_LED_PIN, HIGH);
    delay(100);
    io.digitalWrite(SX1509_LED_PIN, LOW);
    delay(100);
  }
}

void loop()
{
  // Use io.digitalRead() to check if an SX1509 input I/O is
  // either LOW or HIGH.
  if (io.digitalRead(SX1509_BUTTON_PIN) == LOW)
  {
    // Print the status of the other pin:
    Serial.print("SX1509_BUTTON_PIN status: ");
    // Read the pin to print either 0 or 1
    Serial.println(io.digitalRead(SX1509_BUTTON_PIN));

    // If the button is pressed toggle the LED:
    ledState = !ledState;
    io.digitalWrite(SX1509_LED_PIN, ledState);

    // Print the status of the other pin:
    Serial.print("SX1509_LED_PIN status: ");
    // Read the pin to print either 0 or 1
    Serial.println(ledState);


    Serial.print("Waiting for button to release...");
    while (io.digitalRead(SX1509_BUTTON_PIN) == LOW)
      ; // Wait for button to release
    Serial.println("Button released!");

    //delay(200); //uncomment to add a small delay for  button debouncing
  }
}
