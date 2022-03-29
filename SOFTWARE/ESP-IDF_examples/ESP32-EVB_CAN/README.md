| Supported Targets | ESP32 | 
| ----------------- | ----- | 

# CAN Example
(See the README.md file in the upper level 'examples' directory for more information about examples.)

## Overview

This example demonstrates basic usage of `CAN driver`. The work flow of the example could be as follows:

1. Install CAN driver
2. Receive CAN messages
3. Send CAN messase and receive a response
4. Enable filter and receive only filtered messages

If you have a new CAN application to create (for example, J1939 communication to a tank), try this as a basic template, then add your own code.

## How to use example

### Hardware Required

This example assumes you have an Olimex ESP32-EVB and was tested with a REV I version. The Ethernet PHY is a LAN8710A-EZC. See the [ESP32 Hardware Reference](https://docs.espressif.com/projects/esp-idf/en/latest/hw-reference/) for information on using different hardware.

#### Pin Assignment

* CAN-TX is on GPIO5
* CAN-RX is on GPI35

### Configure the project

```
idf.py menuconfig
```
The CAN transceiver is a MCP2562-E/SN. By default it operates on the +5V sourced from the PWR2 connector or USB connector.


### Build, Flash, and Run

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT build flash monitor
```

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

```bash
[TODO]
```

## Troubleshooting

See common troubleshooting for examples from [upper level](../README.md#common-troubleshooting).

(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)
