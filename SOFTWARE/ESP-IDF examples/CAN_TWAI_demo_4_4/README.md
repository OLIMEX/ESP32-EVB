CAN demo for ESP-IDF for ESP32-EVB boards

These are the 2 basic TWAI (Two-Wire Automotive Interface, more popular as CAN) examples included in Espressif ESP-IDF 4.4v framework adapted to work with Olimex board ESP32-EVB rev.I

In order to work you need to have installed ESP-IDF v4.4 (could work with newer version as well but it's tested with 4.4 specifically). To install it follow the instructions described in their official website:

For Windows users: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html
For Linux users: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html

0) Optional - For better structure it is advisable to make a new folder inside the framework directory for your own examples like: "..\esp-idf-v4.4\My_Examples".
1) Copy the 2 examples to the folder you created in the ESP-IDF directory (or the default examples of the framework).
2) From terminal navigate to the folder of the example you want to upload (master/slave)
3) If you want to change anything related to the configurations type: "idf.py menuconfig". This will open a menu where you can configurate most of the features. In the "Example Configuration" you can change the pins of the CAN TX and CAN RX pins.
4) In order to upload type: "idf.py flash". First time it will take a while to compile!
5) You have to upload master example on one of the board and slave on the other one. When it's done open terminals (like PuTTY) on the respective ports. The master will initiate transmisions and the slave will send data. On the terminal you will see messages for the sends and receives.

Stanimir Petev, Olimex
2022/05/19