SparkFun SX1509 IO Expander Breakout Arduino Library
========================================

[![SparkFun SX1509 IO Expander Breakout](https://cdn.sparkfun.com//assets/parts/1/0/9/5/6/13601-01.jpg)](https://www.sparkfun.com/products/13601)

[*SparkFun SX1509 IO Expander Breakout (SKU)*](https://www.sparkfun.com/products/13601)

Arduino library for the SX1509 16-I/O expander. Capable of driving LEDs - with blink, and breathe functions - or monitoring up to 64 buttons in an 8x8 array.

Are you low on I/O? No problem! The SX1509 Breakout is a 16-channel GPIO expander with an I2C interface – that means with just two wires, your microcontroller can interface with 16 fully configurable digital input/output pins. But the SX1509 can do so much more than just simple digital pin control. It can produce PWM signals, so you can dim LEDs. It can be set to blink or even breathe pins at varying rates. This breakout is similar to a multiplexer or "mux," in that it allows you to get more IO from less pins. And, with a built-in keypad engine, it can interface with up to 64 buttons set up in an 8x8 matrix.

Two headers at the top and bottom of the breakout board function as the input and control headers to the board. This is where you can supply power to the SX1509, and where your I2C signals – SDA and SCL – will terminate. GPIO and power buses are broken out in every-which direction, and configurable jumpers cover most of the rest of the board.

Since the I/O banks can operate between 1.2V and 3.6V (5.5V tolerant) independent of both the core and each other, this device can also work as a level-shifter. The SX1509 breakout makes it easy to prototype so you can add more I/O onto your Arduino or I/O limited controller. We've even spun up an Arduino Library to get you started!

Please be aware that v3 of the library breaks previous version compatiblity on a few things:

* Wire.begin() must be called explicitly within the Arduino sketch before io.begin() is called. This may break code that does not have Wire.begin().
* io.begin() has been expanded to include io.begin(deviceAddress, wirePort, resetPin). This may break code where resetPin was the 2nd argument.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/extras** - Additional documentation for the user. These files are ignored by the IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
--------------

* **[SparkFun SX1509 Breakout Board Hookup Guide](https://learn.sparkfun.com/tutorials/sx1509-io-expander-breakout-hookup-guide)** - SparkFun tutorial demonstrating how to hook up the SX1509 Breakout and use this library.
* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.
* **[Product Repository](https://github.com/sparkfun/SX1509_IO-Expander)** - Main repository (including hardware files) for the SX1509 IO Expander Breakout.

Products that use this Library
---------------------------------

* [BOB-13601](https://www.sparkfun.com/products/13601) - SX1509 16 I/O Breakout Board (v2.0)
* [BOB-11502](https://www.sparkfun.com/products/retired/11502)- (Retired) 16 I/O I2C port expander.

Version History
---------------

* [V_3.x] - Incorporate PRs including the ability to pass in a wire port in begin().
* [V_2.0.1](https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library/tree/V_2.0.1) - Version 2.0.1. More user-friendly function calls. Increased clock functionality. More examples. Mostly backwards compatible with older versions.
* [V_1.0.0](https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library/tree/V_1.0.0) - Version 1.0.0

License Information
-------------------

This product is _**open source**_!

The **code** is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release anything derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
