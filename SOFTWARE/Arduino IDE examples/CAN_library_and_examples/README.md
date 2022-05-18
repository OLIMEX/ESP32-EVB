Olimex ESP32-EVB CAN library and examples for Arduino IDE

- How to load this library and the examples in Arduino IDE?

You can import the whole zip archive. To do so:

- Download the esp32-can-protocol-demo-master.zip archive;
- Start Arduino IDE;
- Navigate to Sketch -> Include Library - > Add .ZIP Library and point to esp32-can-protocol-demo-master.zip

Then you can load the examples from:

File - > Examples -> ESP32CAN

The two examles ESP32-EVB_CAN and ESP32-EVB_CAN_Relay_Toggle were written for Olimex and there is description on 
the usage inside each example.

This library is patched version of the original: https://github.com/miwagner/ESP32-Arduino-CAN by Michael Wagner

The patch includes a fix suggested in this pull request: https://github.com/miwagner/ESP32-Arduino-CAN/pull/33
to resolve the differences between modules caused by the ECO3 feature of Espressif on ESP32-WROOM-32E: https://www.esp32.com/viewtopic.php?t=15581

It also has 2 extra examples specifically for Olimex board ESP32-EVB rev.I.

Without this patch new and old ESP32-WROOM can't communicate through CAN with each other due to the halved speed 
of the new modules or you should double the CAN speed of the ESP32-WROOM-32E.

