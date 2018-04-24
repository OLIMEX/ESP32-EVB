# ESP32-EVB
ESP32 WiFi / BLE development board with Ethernet, relays, microSD card, CAN, infrared, Li-Po charger, GPIOs made with KiCAD

Hardware revisions:

- revision A - internal, not released
- revision B - initial public release
- revision C - changes:

1. MicroUSB connector moved back, 40 mils to the middle of the board.
2. All mounting holes are now connected to GND.
3. Improved the ground layer around the infra red's pads and wires.

- revision C1 - changes:

1. Changed R38 from 10k/0603 to 100k/0603 to lower the time without clock
2. Changed C28 from 100n/0603 to 10n/0603 to increase the time in reset

- revision D - changes:

1. Added a resistor (1k) between OSC_DIS and reset of the PHY.
2. Added option to use MCP2562-E/SN and SN65HVD230D upotreba -> R58(NA/R0603)!
4. U2 be promeneno ot SY8009AAAC(SOT23-5) na SY8089AAAC(SOT23-5).
5. FET1 and FET2 were changed from IRLML6402 to WPM2015-3/TR.

- revision D1 - changes:

1. Removed C18(10uF)
2. U8 changed from BD5230G-TR(SSOP5/SOT-23-5) to NCP303LSN27T1G(SOT-23-5), due to unavailability;
3. C28 changed from 100nF/50V/20%/Y5V/C0603 to 470nF/16V/10%/X7R/C0603, because of the U8 change;

- revision D2 - changes:

1. U6 changed from MCP2551-I/SN(SOIC-8_150mil) to MCP2562-E/SN(SOIC-8_150mil)
2. R58 changed from NA/R0603 to 0R/R0603.