/*************************************************************
demo.ino
SparkFun SX1509 I/O Expander Example: fun time demo!
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This is a massive demo sketch to show off what the SX1509 can
do! It combines the keypad engine, LED driving, digital
inputs, interrupt triggering, and we even emulate a simple SPI
interface with it!

All 16 of the SX1509's pins are used. Connected to a 12-button
keypad, Serial 7-Segment Display, and three buttons-with-LEDs:

- 12-Button Keypad: https://sfe.io/p8653
- Serial 7-Segment (S7S): https://sfe.io/p11441
- 3x LED Buttons: https://sfe.io/p10443

Hardware Hookup:
	SX1509 Breakout ------ Arduino ------------ Component
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  0 --------------------------------- Keypad 2 (row 1)
		  1 --------------------------------- Keypad 7 (row 2)
		  2 --------------------------------- Keypad 6 (row 3)
		  3 --------------------------------- Keypad 4 (row 4)
		  4 --------------------------------- S7S SS
		  5 --------------------------------- Button 1
		  6 --------------------------------- Button 2
		  7 --------------------------------- Button 3
		  8 --------------------------------- Keypad 3 (col 1)
		  9 --------------------------------- Keypad 1 (col 2)
		  10 -------------------------------- Keypad 5 (col 3)
		  11 -------------------------------- S7S SDI 
		  12 -------------------------------- S7S SCK
		  13 -------------------------------- LED 1
		  14 -------------------------------- LED 2
		  15 -------------------------------- LED 3

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

#define KEY_ROWS 4 // Number of rows in the keypad matrix
#define KEY_COLS 3 // Number of columns in the keypad matrix

// keyMap maps row/column combinations to characters:
char keyMap[KEY_ROWS][KEY_COLS] = {
    {'0', '1', '2'},
    {'3', '4', '5'},
    {'6', '7', '8'},
    {'*', '0', '#'}};

// SX1509 Pins:
#define SX1509_RED_LED 13 // Red LED on breath-able pin
#define SX1509_GRN_LED 14 // Green LED on breath-able pin
#define SX1509_BLU_LED 15 // Blue LED on breath-able pin

#define SX1509_RED_BTN 5 // Red button, active-low
#define SX1509_GRN_BTN 6 // Green button, active-low
#define SX1509_BLU_BTN 7 // Blue button, active-low

#define SX1509_SDO_PIN 11 // S7S SDI pin (serial-data in)
#define SX1509_SCK_PIN 12 // S7S SCK pin (serial clock)
#define SX1509_SS_PIN 4   // S7S SS pin (slave-select)

// Arduino Pins:
#define ARDUINO_INT_PIN 2 // External interrupt pin

void setup()
{
  // Serial is used to display the keypad presses
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

  // Initialize the keypad.
  // Sleep time off (0). 16ms scan time, 8ms debounce:
  io.keypad(KEY_ROWS, KEY_COLS, 0, 16, 8);

  // Set up the LED pins as ANALOG_OUTPUTs:
  io.pinMode(SX1509_RED_LED, ANALOG_OUTPUT);
  io.pinMode(SX1509_GRN_LED, ANALOG_OUTPUT);
  io.pinMode(SX1509_BLU_LED, ANALOG_OUTPUT);

  // Then configure each of the LED pins to breathe at
  // different rates. Red LED is HIGH 2000ms, LOW 2000ms, and
  // takes 1000ms to rise/fall
  io.breathe(SX1509_RED_LED, 2000, 2000, 1000, 1000);
  // Green LED is on/off for 8s, and takes 4s to rise/fall:
  // We can also set the on/off intensity. On intensity is
  // 127 off is 16 out of 255.
  io.breathe(SX1509_GRN_LED, 8000, 8000, 4000, 4000,
             127, 16);
  // You can set the breathe pulse to either LOGARITHMIC or
  // LINEAR. They default to LINEAR.
  // Blue LED is on/off for 500 ms, rises/falls in 500ms:
  io.breathe(SX1509_BLU_LED, 500, 500, 500, 500,
             255, 0, LOGARITHMIC);

  // Instead of breathing the entire duration, we can write a
  // breathing pin HIGH to initiate SINGLE-SHOT mode. After a
  // HIGH write, each pin will do its breathe pulse once,
  // then stop.
  io.digitalWrite(SX1509_GRN_LED, HIGH);
  io.digitalWrite(SX1509_RED_LED, HIGH);
  io.digitalWrite(SX1509_BLU_LED, HIGH);
  // Call io.sync() to synchronize all LED outputs.
  io.sync();

  // Set up the button inputs. They'll all be active-low, so
  // configure them with internal pull-ups
  io.pinMode(SX1509_RED_BTN, INPUT_PULLUP);
  io.pinMode(SX1509_GRN_BTN, INPUT_PULLUP);
  io.pinMode(SX1509_BLU_BTN, INPUT_PULLUP);
  // Enable interrupts on each button pin. Generate an
  // interrupt on either a rise or fall using CHANGE.
  io.enableInterrupt(SX1509_RED_BTN, CHANGE);
  io.enableInterrupt(SX1509_GRN_BTN, CHANGE);
  io.enableInterrupt(SX1509_BLU_BTN, CHANGE);

  // Set up the outputs for the serial 7-segment display's
  // SPI interface.
  io.pinMode(SX1509_SDO_PIN, OUTPUT); // SDO (S7S's SDI)
  io.digitalWrite(SX1509_SDO_PIN, HIGH);
  io.pinMode(SX1509_SCK_PIN, OUTPUT); // SCK (serial clock)
  io.digitalWrite(SX1509_SCK_PIN, HIGH);
  io.pinMode(SX1509_SS_PIN, OUTPUT); // SS (Slave-select)
  io.digitalWrite(SX1509_SS_PIN, HIGH);

  // Finally, set up our Arduino pin(s). Set the interrupt as
  // an INPUT_PULLUP. SX1509's interrupt output is active-low.
  pinMode(ARDUINO_INT_PIN, INPUT_PULLUP);
}

void loop()
{
  // If the interrupt triggers (goes LOW):
  if (!digitalRead(ARDUINO_INT_PIN))
  {
    // Don't know if keypad or a button triggered the int
    doKeypad(); // Check the keypad first.

    // Then check the buttons.
    // The io.checkInterrupt(<pin>) function can check if a
    // single pin triggered the interrupt. Note that this
    // function will not clear the interrupt.
    if (io.checkInterrupt(SX1509_RED_BTN)) // Red button
    {
      // If the button was pressed turn the LED of.
      // If the button was released. Single-shot
      // the LED's breathe mode.
      io.digitalWrite(SX1509_RED_LED,
                      io.digitalRead(SX1509_RED_BTN));
    }
    if (io.checkInterrupt(SX1509_GRN_BTN)) // Green button
    {
      io.digitalWrite(SX1509_GRN_LED,
                      io.digitalRead(SX1509_GRN_BTN));
    }
    if (io.checkInterrupt(SX1509_BLU_BTN)) // Blue button
    {
      io.digitalWrite(SX1509_BLU_LED,
                      io.digitalRead(SX1509_BLU_BTN));
    }
  }
}

void doKeypad()
{
  static int lastKeyPress = 255;
  static unsigned long lastKeyPressTime = 0;

  static int digitPos = 0;

  // Use io.readKeypad() to check which (if any) row and
  // columns are active.
  unsigned int keyData = io.readKeypad();
  // readKeypad may be 0, which means the keypad didn't
  // generate the interrupt. If not, though...
  if (keyData != 0)
  {
    // Use getRow() and getCol() to parse the row and column:
    byte row = io.getRow(keyData);
    byte col = io.getCol(keyData);
    // Use those values to get the key pressed:
    int key = keyMap[row][col];

    // If it's a new key press, or enough time has passed
    if ((keyData != lastKeyPress) ||
        (lastKeyPressTime < millis() - 100)) //100ms
    {
      // Print the key press to serial for debugging:
      Serial.print(String(row) + " | " + String(col) + " | ");
      Serial.println(key);
      lastKeyPress = keyData;
      lastKeyPressTime = millis();

      // Use the SX1509's I/O pins to write an "SPI" byte
      // to the serial 7-segment display.
      sendSPIByte(key);
    }
  }
}

void sendSPIByte(byte data)
{
  // Mock up a simple SPI output transfer using a data
  // output (SDO), clock (SCK), and slave-select (SS).
  // Begin with all pins LOW.
  io.digitalWrite(SX1509_SDO_PIN, LOW);
  io.digitalWrite(SX1509_SCK_PIN, LOW);
  // Setting SS LOW initiates the SPI transfer:
  io.digitalWrite(SX1509_SS_PIN, LOW);

  // Cycle through all 8-bits of the data value, from
  // high to low:
  for (int b = 7; b >= 0; b--)
  {
    if (data & 1 << b)                       // If the bit is 1
      io.digitalWrite(SX1509_SDO_PIN, HIGH); // Set SDO HIGH
    else                                     // If the bit is 0
      io.digitalWrite(SX1509_SDO_PIN, LOW);  // Set SDO LOW

    // Now cycle the clock HIGH, then LOW:
    delay(1);                              // Small delay, S7S's max clock speed is 250kHz
    io.digitalWrite(SX1509_SCK_PIN, HIGH); // Write SCK HIGH
    delay(1);                              // Equivalent-ish small delay
    io.digitalWrite(SX1509_SCK_PIN, LOW);  // Write SCK LOW
  }
  // Write SS HIGH to end the transfer
  io.digitalWrite(SX1509_SS_PIN, HIGH);
}
