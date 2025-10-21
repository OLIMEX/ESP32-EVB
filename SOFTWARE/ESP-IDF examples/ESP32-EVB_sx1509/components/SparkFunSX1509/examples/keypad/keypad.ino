/*************************************************************
keypad.ino
SparkFun SX1509 I/O Expander Example: keypad matrix
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

This example demonstrates how to use the SX1509's keypad engine to monitor a
matrix of button inputs.

For this example, we'll wire the SX1509 up to a 12-pad keypad
  (https://www.sparkfun.com/products/8653).

After uploading the sketch, open your serial monitor and set it to 115200 baud.

Hardware Hookup:
	SX1509 Breakout ------ Arduino -------- Keypad Pin
	      GND -------------- GND
	      3V3 -------------- 3.3V
		  SDA ------------ SDA (A4)
		  SCL ------------ SCL (A5)
		  0 ---------------------------------- 2 (row 1)
		  1 ---------------------------------- 7 (row 2)
		  2 ---------------------------------- 6 (row 3)
		  3 ---------------------------------- 4 (row 4)
		  8 ---------------------------------- 3 (col 1)
		  9 ---------------------------------- 1 (col 2)
		  10 --------------------------------- 5 (col 3)

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
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

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

  // To initialize the keypad, call io.keypad(<rows>, <cols>)
  // You can also define the duration of inactivity before the
  // keypad engine sleeps, time spent scanning each row, and
  // the debounce time per button.

  // After a set number of milliseconds, the keypad engine
  // will go into a low-current sleep mode.
  // Sleep time range: 128 ms - 8192 ms (powers of 2) 0=OFF
  unsigned int sleepTime = 256;
  // Scan time defines the number of milliseconds devoted to
  // each row in the matrix.
  // Scan time range: 1-128 ms, powers of 2
  byte scanTime = 2; // Scan time per row, in ms
  // Debounce sets the minimum amount of time that must pass
  // before a button can be pressed again.
  // Debounce time range: 0.5 - 64 ms (powers of 2)
  byte debounceTime = 1; // Debounce time
  // Note: Scan time must be greater than debounce time!
  // Take all of those values to set up the keypad engine:
  io.keypad(KEY_ROWS, KEY_COLS, sleepTime, scanTime, debounceTime);

  // Note: we don't get to pick which pins the SX1509 connects
  // to each row/column. They go up sequetially on pins 0-7
  // (rows), and 8-15 (cols).
}

void loop()
{
  // Use io.readKeypad() to check if any keys have been pressed:
  unsigned int keyData = io.readKeypad();
  // If keyData is 0, then nothing has been pressed. Otherwise
  // at least two bits in the 16-bit value will be set, each
  // corresponding to either a row or a column.
  if (keyData != 0) // If a key was pressed:
  {
    // Use io.getRow(<readKeypad>) and io.getCol(<readKeypad>)
    // to find the active row and columns:
    byte row = io.getRow(keyData);
    byte col = io.getCol(keyData);

    // Once you've found out the active row/col, put them in
    // keyMap to get the character pressed.
    char key = keyMap[row][col];
    Serial.println("Row: " + String(row));
    Serial.println("Col: " + String(col));
    Serial.print("Key: ");
    Serial.println(key);
  }
}
