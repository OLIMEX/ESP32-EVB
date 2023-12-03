/******************************************************************************
SparkFunSX1509.hpp

Ported from SparkFunSX1509.h by Brian Alano, Solar Stik, Inc.

SparkFun SX1509 I/O Expander Library Header File
Jim Lindblom @ SparkFun Electronics
Original Creation Date: September 21, 2015
https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

Here you'll find the Arduino code used to interface with the SX1509 I2C
16 I/O expander. There are functions to take advantage of everything the
SX1509 provides - input/output setting, writing pins high/low, reading 
the input value of pins, LED driver utilities (blink, breath, pwm), and
keypad engine utilites.

Development environment specifics:
	IDE: Arduino 1.6.5
	Hardware Platform: Arduino Uno
	SX1509 Breakout Version: v2.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/
//TODO I2C_NUM_0 is hardcoded. Should allow selection of any of the I2C buses.
//TODO only begin(), init() and reset() are implemented
//TEST readByte(), writeByte(). Work in reset(), but that's a poor test.

// #include "Arduino.h"
#include "stdint.h"
#include <driver/i2c.h>

#ifndef SparkFunSX1509_H
#define SparkFunSX1509_H

#ifndef I2C_ERROR_OK
#define I2C_ERROR_OK 0
#endif

#define RECEIVE_TIMEOUT_VALUE 1000 // Timeout for I2C receive

// These are used for setting LED driver to linear or log mode:
#define LINEAR 0
#define LOGARITHMIC 1

// These are used for clock config:
#define INTERNAL_CLOCK_2MHZ 2
#define EXTERNAL_CLOCK 1

#define SOFTWARE_RESET 0
#define HARDWARE_RESET 1

// Arduino definitions TODO move to separate header
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define ANALOG_OUTPUT 0x3 // To set a pin mode for PWM output


class SX1509
{
private: // These private functions are not available to Arduino sketches.
		 // If you need to read or write directly to registers, consider
		 // putting the writeByte, readByte functions in the public section
	i2c_config_t conf;
	uint8_t deviceAddress; // I2C Address of SX1509
						   // Pin definitions:
	gpio_num_t pinInterrupt;
	gpio_num_t pinOscillator;
	gpio_num_t pinReset;
	uint8_t  _slave_addr;
	// Misc variables:
	unsigned long _clkX;
	// Read Functions:
//	uint16_t readWord(uint8_t registerAddress);

	// Read Functions returning success or failure:
	esp_err_t readBytes(uint8_t firstRegisterAddress, uint8_t *destination, uint8_t length);

	esp_err_t readByte(uint8_t registerAddress, uint8_t *value);
	esp_err_t readWord(uint8_t registerAddress, uint16_t *value);
	// Write functions, returning success or failure:
	esp_err_t writeByte(uint8_t registerAddress, uint8_t writeValue);
	esp_err_t writeWord(uint8_t registerAddress, uint16_t writeValue);
	esp_err_t writeBytes(uint8_t firstRegisterAddress, uint8_t *writeArray, uint8_t length);
	// Helper functions:
	// calculateLEDTRegister - Try to estimate an LED on/off duration register,
	// given the number of milliseconds and LED clock frequency.
	uint8_t calculateLEDTRegister(unsigned long ms);
	// calculateSlopeRegister - Try to estimate an LED rise/fall duration
	// register, given the number of milliseconds and LED clock frequency.
	uint8_t calculateSlopeRegister(uint8_t ms, uint8_t onIntensity, uint8_t offIntensity);

public:
	// -----------------------------------------------------------------------------
	// Constructor - SX1509: This function sets up the pins connected to the
	//		SX1509, and sets up the private deviceAddress variable.
	// -----------------------------------------------------------------------------
	SX1509();
	
	// -----------------------------------------------------------------------------
	// begin(uint8_t address, uint8_t resetPin): This function initializes the SX1509.
	//  	It requires wire to already be begun (previous versions did not do this), resets the IC, and tries to read some
	//  	registers to prove it's connected.
	// Inputs:
	//		- slave_addr should be the 7-bit address of the SX1509. This should be
	//		 one of four values - 0x3E, 0x3F, 0x70, 0x71 - all depending on what the
	//		 ADDR0 and ADDR1 pins ar se to. This variable is required.
	//		- resetPin: This is the Arduino pin tied to the SX1509 RST pin. This
	//		 pin is optional. If not declared, the library will attempt to
	//		 software reset the SX1509.
	//		- interruptPin: This is the Arduino pin tied to the SX1509 interrupt pin. This
	//		 pin is optional. 
	//		- oscillatorPin: This is the Arduino pin tied to the SX1509 OSC pin. This
	//		 pin is optional.
	// Output: Returns a ESP_OK if communication is successful, ESP_ERR on error.
	// -----------------------------------------------------------------------------
	esp_err_t begin(uint16_t slave_addr, int sda_io_num, int scl_io_num, gpio_num_t resetPin = GPIO_NUM_MAX, gpio_num_t interruptPin = GPIO_NUM_MAX, gpio_num_t oscillatorPin = GPIO_NUM_MAX, uint32_t clk_speed = 200000);

	// -----------------------------------------------------------------------------
	// reset(bool hardware): This function resets the SX1509 - either a hardware
	//		reset or software. A hardware reset (hardware parameter = 1) pulls the
	//		reset line low, pausing, then pulling the reset line high. A software
	//		reset writes a 0x12 then 0x34 to the REG_RESET as outlined in the
	//		datasheet.
	//
	//  Input:
	//	 	- hardware: 0 executes a software reset, 1 executes a hardware reset
	// -----------------------------------------------------------------------------
	esp_err_t reset(bool hardware);

	// -----------------------------------------------------------------------------
	// init()
	// reset and test communications
	// -----------------------------------------------------------------------------
	esp_err_t init();

	// -----------------------------------------------------------------------------
	// pinMode(uint8_t pin, uint8_t inOut): This function sets one of the SX1509's 16
	//		outputs to either an INPUT or OUTPUT.
	//
	//	Inputs:
	//	 	- pin: should be a value between 0 and 15
	//	 	- inOut: The Arduino INPUT and OUTPUT constants should be used for the
	//		 inOut parameter. They do what they say!
	// -----------------------------------------------------------------------------
	esp_err_t pinMode(uint8_t pin, uint8_t inOut, uint8_t initialLevel = HIGH);
	esp_err_t pinDir(uint8_t pin, uint8_t inOut, uint8_t initialLevel = HIGH); // Legacy - use pinMode

	// -----------------------------------------------------------------------------
	// digitalWrite(uint8_t pin, uint8_t highLow): This function writes a pin to either high
	//		or low if it's configured as an OUTPUT. If the pin is configured as an
	//		INPUT, this method will activate either the PULL-UP	or PULL-DOWN
	//		resistor (HIGH or LOW respectively).
	//
	//	Inputs:
	//		- pin: The SX1509 pin number. Should be a value between 0 and 15.
	//		- highLow: should be Arduino's defined HIGH or LOW constants.
	// -----------------------------------------------------------------------------
	esp_err_t digitalWrite(uint8_t pin, uint8_t highLow);
	esp_err_t writePin(uint8_t pin, uint8_t highLow); // Legacy - use digitalWrite

	// -----------------------------------------------------------------------------
	// digitalRead(uint8_t pin): This function reads the HIGH/LOW status of a pin.
	//		The pin should be configured as an INPUT, using the pinDir function.
	//
	//	Inputs:
	//	 	- pin: The SX1509 pin to be read. should be a value between 0 and 15.
	//  Outputs:
	//		This function returns a 1 if HIGH, 0 if LOW
	// -----------------------------------------------------------------------------
	uint8_t digitalRead(uint8_t pin);
	bool digitalRead(uint8_t pin, bool *value);
	uint8_t readPin(uint8_t pin); // Legacy - use digitalRead
	bool readPin(const uint8_t pin, bool *value);

	// -----------------------------------------------------------------------------
	// ledDriverInit(uint8_t pin, uint8_t freq, bool log): This function initializes LED
	//		driving on a pin. It must be called if you want to use the pwm or blink
	//		functions on that pin.
	//
	//	Inputs:
	//		- pin: The SX1509 pin connected to an LED. Should be 0-15.
	//   	- freq: Sets LED clock frequency divider.
	//		- log: selects either linear or logarithmic mode on the LED drivers
	//			- log defaults to 0, linear mode
	//			- currently log sets both bank A and B to the same mode
	//	Note: this function automatically decides to use the internal 2MHz osc.
	// -----------------------------------------------------------------------------
	esp_err_t ledDriverInit(uint8_t pin, uint8_t freq = 1, bool log = false);

	// -----------------------------------------------------------------------------
	// analogWrite(uint8_t pin, uint8_t iOn):	This function can be used to control the intensity
	//		of an output pin connected to an LED.
	//
	//	Inputs:
	//		- pin: The SX1509 pin connecte to an LED.Should be 0-15.
	//		- iOn: should be a 0-255 value setting the intensity of the LED
	//			- 0 is completely off, 255 is 100% on.
	//
	//	Note: ledDriverInit should be called on the pin before calling this.
	// -----------------------------------------------------------------------------
	void analogWrite(uint8_t pin, uint8_t iOn);
	void pwm(uint8_t pin, uint8_t iOn); // Legacy - use analogWrite

	// -----------------------------------------------------------------------------
	// setupBlink(uint8_t pin, uint8_t tOn, uint8_t tOff, uint8_t offIntensity, uint8_t tRise, uint8_t
	//		tFall):  blink performs both the blink and breath LED driver functions.
	//
	// 	Inputs:
	//  	- pin: the SX1509 pin (0-15) you want to set blinking/breathing.
	//		- tOn: the amount of time the pin is HIGH
	//			- This value should be between 1 and 31. 0 is off.
	//		- tOff: the amount of time the pin is at offIntensity
	//			- This value should be between 1 and 31. 0 is off.
	//		- offIntensity: How dim the LED is during the off period.
	//			- This value should be between 0 and 7. 0 is completely off.
	//		- onIntensity: How bright the LED will be when completely on.
	//			- This value can be between 0 (0%) and 255 (100%).
	//		- tRise: This sets the time the LED takes to fade in.
	//			- This value should be between 1 and 31. 0 is off.
	//			- This value is used with tFall to make the LED breath.
	//		- tFall: This sets the time the LED takes to fade out.
	//			- This value should be between 1 and 31. 0 is off.
	// 	 Notes:
	//		- The breathable pins are 4, 5, 6, 7, 12, 13, 14, 15 only. If tRise and
	//			tFall are set on 0-3 or 8-11 those pins will still only blink.
	// 		- ledDriverInit should be called on the pin to be blinked before this.
	// -----------------------------------------------------------------------------
	esp_err_t setupBlink(uint8_t pin, uint8_t onReg, uint8_t offReg, uint8_t onIntensity = 255, uint8_t offIntensity = 0, uint8_t tRise = 0, uint8_t tFall = 0, bool log = false);

	// -----------------------------------------------------------------------------
	// blink(uint8_t pin, unsigned long tOn, unsigned long tOff, uint8_t onIntensity, uint8_t offIntensity);
	//  	Set a pin to blink output for estimated on/off millisecond durations.
	//
	// 	Inputs:
	//  	- pin: the SX1509 pin (0-15) you want to set blinking
	//   	- tOn: estimated number of milliseconds the pin is LOW (LED sinking current will be on)
	//   	- tOff: estimated number of milliseconds the pin is HIGH (LED sinking current will be off)
	//   	- onIntensity: 0-255 value determining LED on brightness
	//   	- offIntensity: 0-255 value determining LED off brightness
	// 	 Notes:
	//		- The breathable pins are 4, 5, 6, 7, 12, 13, 14, 15 only. If tRise and
	//			tFall are set on 0-3 or 8-11 those pins will still only blink.
	// 		- ledDriverInit should be called on the pin to be blinked before this.
	// -----------------------------------------------------------------------------
	esp_err_t blink(uint8_t pin, unsigned long tOn, unsigned long tOff, uint8_t onIntensity = 255, uint8_t offIntensity = 0);

	// -----------------------------------------------------------------------------
	// breathe(uint8_t pin, unsigned long tOn, unsigned long tOff, unsigned long rise, unsigned long fall, uint8_t onInt, uint8_t offInt, bool log);
	//  	Set a pin to breathe output for estimated on/off millisecond durations, with
	//  	estimated rise and fall durations.
	//
	// 	Inputs:
	//  	- pin: the SX1509 pin (0-15) you want to set blinking
	//   	- tOn: estimated number of milliseconds the pin is LOW (LED sinking current will be on)
	//   	- tOff: estimated number of milliseconds the pin is HIGH (LED sinking current will be off)
	//   	- rise: estimated number of milliseconds the pin rises from LOW to HIGH
	//   	- falll: estimated number of milliseconds the pin falls from HIGH to LOW
	//   	- onIntensity: 0-255 value determining LED on brightness
	//   	- offIntensity: 0-255 value determining LED off brightness
	// 	 Notes:
	//		- The breathable pins are 4, 5, 6, 7, 12, 13, 14, 15 only. If tRise and
	//			tFall are set on 0-3 or 8-11 those pins will still only blink.
	// 		- ledDriverInit should be called on the pin to be blinked before this,
	//  	  Or call pinMode(<pin>, ANALOG_OUTPUT);
	// -----------------------------------------------------------------------------
	void breathe(uint8_t pin, unsigned long tOn, unsigned long tOff, unsigned long rise, unsigned long fall, uint8_t onInt = 255, uint8_t offInt = 0, bool log = LINEAR);

	// -----------------------------------------------------------------------------
	// keypad(uint8_t rows, uint8_t columns, uint8_t sleepTime, uint8_t scanTime, uint8_t debounceTime)
	//		Initializes the keypad function on the SX1509. Millisecond durations for sleep,
	//		scan, and debounce can be set.
	//
	//	Inputs:
	//		- rows: The number of rows in the button matrix.
	//			- This value must be between 1 and 7. 0 will turn it off.
	//			- eg: 1 = 2 rows, 2 = 3 rows, 7 = 8 rows, etc.
	//		- columns: The number of columns in the button matrix
	//			- This value should be between 0 and 7.
	//			- 0 = 1 column, 7 = 8 columns, etc.
	//		- sleepTime: Sets the auto-sleep time of the keypad engine.
	//  	  Should be a millisecond duration between 0 (OFF) and 8000 (8 seconds).
	//   	  Possible values are 0, 128, 256, 512, 1000, 2000, 4000, 8000
	//		- scanTime: Sets the scan time per row. Must be set above debounce.
	//  	  Should be a millisecond duration between 1 and 128.
	//   	  Possible values are 1, 2, 4, 8, 16, 32, 64, 128.
	//		- debounceTime: Sets the debounc time per button. Must be set below scan.
	//  	  Should be a millisecond duration between 0 and 64.
	//   	  Possible values are 0 (0.5), 1, 2, 4, 8, 16, 32, 64.
	// -----------------------------------------------------------------------------
	void keypad(uint8_t rows, uint8_t columns, uint16_t sleepTime = 0, uint8_t scanTime = 1, uint8_t debounceTime = 0);

	// -----------------------------------------------------------------------------
	// readKeypad(): This function returns a 16-bit value containing the status of
	//		keypad engine.
	//
	//	Output:
	//		A 16-bit value is returned. The lower 8 bits represent the up-to 8 rows,
	//		while the MSB represents the up-to 8 columns. Bit-values of 1 indicate a
	//		button in that row or column is being pressed. As such, at least two
	//		bits should be set.
	// -----------------------------------------------------------------------------
	uint16_t readKeypad();
	uint16_t readKeyData(); // Legacy: use readKeypad();

	// -----------------------------------------------------------------------------
	// getRow(): This function returns the first active row from the return value of
	//  	readKeypad().
	//
	//	Input:
	//      - keyData: Should be the uint16_t value returned from readKeypad().
	//	Output:
	//		A 16-bit value is returned. The lower 8 bits represent the up-to 8 rows,
	//		while the MSB represents the up-to 8 columns. Bit-values of 1 indicate a
	//		button in that row or column is being pressed. As such, at least two
	//		bits should be set.
	// -----------------------------------------------------------------------------
	uint8_t getRow(uint16_t keyData);

	// -----------------------------------------------------------------------------
	// getCol(): This function returns the first active column from the return value of
	//  	readKeypad().
	//
	//	Input:
	//      - keyData: Should be the uint16_t value returned from readKeypad().
	//	Output:
	//		A 16-bit value is returned. The lower 8 bits represent the up-to 8 rows,
	//		while the MSB represents the up-to 8 columns. Bit-values of 1 indicate a
	//		button in that row or column is being pressed. As such, at least two
	//		bits should be set.
	// -----------------------------------------------------------------------------
	uint8_t getCol(uint16_t keyData);

	// -----------------------------------------------------------------------------
	// sync(void): this function resets the PWM/Blink/Fade counters, syncing any
	//		blinking LEDs. Bit 2 of REG_MISC is set, which alters the functionality
	//		of the nReset pin. The nReset pin is toggled low->high, which should
	//		reset all LED counters. Bit 2 of REG_MISC is again cleared, returning
	//		nReset pin to POR functionality
	// -----------------------------------------------------------------------------
	void sync(void);

	// -----------------------------------------------------------------------------
	// debounceConfig(uint8_t configValue): This method configures the debounce time of
	//		every input.
	//
	//	Input:
	//		- configValue: A 3-bit value configuring the debounce time.
	//			000: 0.5ms * 2MHz/fOSC
	//			001: 1ms * 2MHz/fOSC
	//			010: 2ms * 2MHz/fOSC
	//			011: 4ms * 2MHz/fOSC
	//			100: 8ms * 2MHz/fOSC
	//			101: 16ms * 2MHz/fOSC
	//			110: 32ms * 2MHz/fOSC
	//			111: 64ms * 2MHz/fOSC
	//
	//	Note: fOSC is set with the configClock function. It defaults to 2MHz.
	// -----------------------------------------------------------------------------
	void debounceConfig(uint8_t configVaule);

	// -----------------------------------------------------------------------------
	// debounceTime(uint8_t configValue): This method configures the debounce time of
	//		every input to an estimated millisecond time duration.
	//
	//	Input:
	//		- time: A millisecond duration estimating the debounce time. Actual
	//		  debounce time will depend on fOSC. Assuming it's 2MHz, debounce will
	//		  be set to the 0.5, 1, 2, 4, 8, 16, 32, or 64 ms (whatever's closest)
	//
	//	Note: fOSC is set with the configClock function. It defaults to 2MHz.
	// -----------------------------------------------------------------------------
	void debounceTime(uint8_t time);

	// -----------------------------------------------------------------------------
	// debouncePin(uint8_t pin): This method enables debounce on SX1509 input pin.
	//
	//	Input:
	//		- pin: The SX1509 pin to be debounced. Should be between 0 and 15.
	// -----------------------------------------------------------------------------
	void debouncePin(uint8_t pin);
	void debounceEnable(uint8_t pin); // Legacy, use debouncePin

	// -----------------------------------------------------------------------------
	// debounceKeypad(uint8_t pin): This method enables debounce on all pins connected
	//  to a row/column keypad matrix.
	//
	//	Input:
	//		- time: Millisecond time estimate for debounce (see debounceTime()).
	//		- numRows: The number of rows in the keypad matrix.
	//		- numCols: The number of columns in the keypad matrix.
	// -----------------------------------------------------------------------------
	void debounceKeypad(uint8_t time, uint8_t numRows, uint8_t numCols);

	// -----------------------------------------------------------------------------
	// enableInterrupt(uint8_t pin, uint8_t riseFall): This function sets up an interrupt
	//		on a pin. Interrupts can occur on all SX1509 pins, and can be generated
	//		on rising, falling, or both.
	//
	//	Inputs:
	//		-pin: SX1509 input pin that will generate an input. Should be 0-15.
	//		-riseFall: Configures if you want an interrupt generated on rise fall or
	//			both. For this param, send the pin-change values previously defined
	//			by Arduino:
	//			#define CHANGE 1	<-Both
	//			#define FALLING 2	<- Falling
	//			#define RISING 3	<- Rising
	//
	//	Note: This function does not set up a pin as an input, or configure	its
	//		pull-up/down resistors! Do that before (or after).
	// -----------------------------------------------------------------------------
	void enableInterrupt(uint8_t pin, uint8_t riseFall);

	// -----------------------------------------------------------------------------
	// interruptSource(void): Returns an uint16_t representing which pin caused
	//		an interrupt.
	//
	//	Output: 16-bit value, with a single bit set representing the pin(s) that
	//		generated an interrupt. E.g. a return value of	0x0104 would mean pins 8
	//		and 3 (bits 8 and 3) have generated an interrupt.
	//  Input:
	//  	- clear: boolean commanding whether the interrupt should be cleared
	//  	  after reading or not.
	// -----------------------------------------------------------------------------
	uint16_t interruptSource(bool clear = true);

	// -----------------------------------------------------------------------------
	// checkInterrupt(void): Checks if a single pin generated an interrupt.
	//
	//	Output: Boolean value. True if the requested pin has triggered an interrupt/
	//  Input:
	//  	- pin: Pin to be checked for generating an input.
	// -----------------------------------------------------------------------------
	bool checkInterrupt(uint8_t pin);

	// -----------------------------------------------------------------------------
	// configClock(uint8_t oscSource, uint8_t oscPinFunction, uint8_t oscFreqOut, uint8_t oscDivider)
	//		This function configures the oscillator source/speed
	//		and the clock, which is used to drive LEDs and time debounces.
	//
	//	Inputs:
	//	- oscSource: Choose either internal 2MHz oscillator or an external signal
	//		applied to the OSCIO pin.
	//		- INTERNAL_CLOCK and EXTERNAL_CLOCK are defined in the header file.
	//			Use those.
	//		- This value defaults to internal.
	//	- oscDivider: Sets the clock divider in REG_MISC.
	//		- ClkX = fOSC / (2^(RegMisc[6:4] -1))
	//		- This value defaults to 1.
	//	- oscPinFunction: Allows you to set OSCIO as an input or output.
	//		- You can use Arduino's INPUT, OUTPUT defines for this value
	//		- This value defaults to input
	//	- oscFreqOut: If oscio is configured as an output, this will set the output
	//		frequency
	//		- This should be a 4-bit value. 0=0%, 0xF=100%, else
	//			fOSCOut = FOSC / (2^(RegClock[3:0]-1))
	//		- This value defaults to 0.
	// -----------------------------------------------------------------------------
	esp_err_t configClock(uint8_t oscSource = 2, uint8_t oscPinFunction = 0, uint8_t oscFreqOut = 0, uint8_t oscDivider = 1); // Legacy, use clock();

	// -----------------------------------------------------------------------------
	// clock(uint8_t oscSource, uint8_t oscDivider, uint8_t oscPinFunction, uint8_t oscFreqOut)
	//		This function configures the oscillator source/speed
	//		and the clock, which is used to drive LEDs and time debounces.
	//  	This is just configClock in a bit more sane order.
	//
	// -----------------------------------------------------------------------------
	esp_err_t clock(uint8_t oscSource = 2, uint8_t oscDivider = 1, uint8_t oscPinFunction = 0, uint8_t oscFreqOut = 0);
};

// Add backwards compatibility for the old class name: sx1509Class
typedef SX1509 sx1509Class;

#endif // SX1509_library_H
