| Supported Targets | ESP32 | ESP32-S2 | ESP32-C3 |
| ----------------- | ----- | -------- | -------- |

# Ethernet Example
(See the README.md file in the upper level 'examples' directory for more information about examples.)

## Overview

This example demonstrates basic usage of `Ethernet driver` together with `tcpip_adapter`. The work flow of the example could be as follows:

1. Install Ethernet driver
2. Send DHCP requests and wait for a DHCP lease
3. If get IP address successfully, then you will be able to ping the device

If you have a new Ethernet application to go (for example, connect to IoT cloud via Ethernet), try this as a basic template, then add your own code.

## How to use example

### Hardware Required

This example assumes you have an Olimex ESP32-EVB and was tested with a REV I version. The Ethernet PHY is a LAN8710A-EZC. See the Ethernet example in the Expressif ESP32 GitHub repository for information on using different hardware.

#### Configure the project (including Pin Assignment)
Set the following options using the Expressif-IDF SDK Configuration Editor (idf.py), or equivalent:
* Internal EMAC: enabled
* Ethernet PHY Device: LAN87xx
* SMI MDC GPIO number: 23
* SMI MDIO GPIO number: 18
* PHY Reset GPIO number: -1
* PHY Address: 0
* SPI Ethernet: disabled

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
I (394) eth_example: Ethernet Started
I (3934) eth_example: Ethernet Link Up
I (3934) eth_example: Ethernet HW Addr 30:ae:a4:c6:87:5b
I (5864) tcpip_adapter: eth ip: 192.168.2.151, mask: 255.255.255.0, gw: 192.168.2.2
I (5864) eth_example: Ethernet Got IP Address
I (5864) eth_example: ~~~~~~~~~~~
I (5864) eth_example: ETHIP:192.168.2.151
I (5874) eth_example: ETHMASK:255.255.255.0
I (5874) eth_example: ETHGW:192.168.2.2
I (5884) eth_example: ~~~~~~~~~~~
```

Now you can ping your ESP32 in the terminal by entering `ping 192.168.2.151` (it depends on the actual IP address you get).

## Troubleshooting

See common troubleshooting for Ethernet examples from [upper level](../README.md#common-troubleshooting).

(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)
