| Supported Targets | ESP32 | 
| ----------------- | ----- | 

# CAN Example
(See the README.md file in the upper level 'examples' directory for more information about examples.)

## Overview

This example demonstrates basic usage of `CAN driver`. The work flow of the example could be as follows:

1. Install CAN driver
2. Receive CAN messages
3. Send CAN message
4. Enable filter and receive only filtered messages (not implemented)

Code was heavily based on https://docs.espressif.com/projects/esp-idf/en/release-v3.3/api-reference/peripherals/can.html

If you have a new CAN application to create (for example, J1939 communication to a tank), try this as a basic template, then add your own code.

## How to use example
See Build, Flash, and Run below.

### Hardware Required

This example assumes you have an Olimex ESP32-EVB and was tested with a REV I version. 

#### Pin Assignment

This example uses the embedded ESP32 CAN/TWAI controller. On the ESP32-EVB the pins are assigned as

* CAN-TX is on GPIO5
* CAN-RX is on GPI35

In the example code, these assignments are hard-coded.

### Configure the project

```
idf.py menuconfig
```
No configuration options have been set up.

The CAN transceiver is a MCP2562-E/SN. By default on the ESP32-EVB it operates on the +5V sourced from the PWR2 connector or USB connector.


### Build, Flash, and Run

Build the project and flash it to the board, then run monitor tool to view serial output. Use a CAN monitoring tool to see the transmitted message.

```
idf.py -p PORT build flash monitor
```

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

```bash
Driver installed
Driver started
Message received
Message is in Extended Format
ID:1CEBFFFA, data:2 90 E7 FF FF FF FF FF
Message queued for transmission
ESP_ERR_TIMEOUT
```

## Troubleshooting

See common troubleshooting for examples from [upper level](../README.md#common-troubleshooting).

(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)
