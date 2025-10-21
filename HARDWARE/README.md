# ESP32-EVB

Hardware revisions changes:

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

- revision E - changes:

MCP2551 got "Not recommended for new designs" status so it is replaced with MCP2562 and this lead to:

1. R58(0R/R0603) was deleted and U6's pin<5> was connected to +3.3V.
2. R52(680R/R0603) was deleted.
3. R50(330R/R0603) and R51(330R/R0603) were deleted.
4. The MCP2562-E_SN(SOIC-8_150mil)'s symbol was updated, i,.e. U6's component was exchanged with 2562!

Changes related to the problems with ethernet PHY:

5. The following components were added to enable powering on the PHY after the 50MHz oscillator has been started: L5(NA(FB0805/600R/2A)), FET3(WPM2015-3/TR), FET4(BSS138), C29(22uF/6.3V/20%/X5R/C0603), R50(1k/R0603) and R51(10k/R0603).

Changes related to optimizations:

6. R26(330R/R0603) and R27(330R/R0603) were changed to 220R/R0603 to reserve a feeder.
7. C1 was changed from 47uF/6.3V/20%/X5R/C0805 to NA(47uF/6.3V/20%/X5R/C0805).	
8. C3 was changed from 47uF/6.3V/20%/X5R/C0805 to NA(47uF/6.3V/20%/X5R/C0805).

- revision F - changes:

1. BUT1 and RST1 new packages.
2. Minor PCB routing optimizations.
3. All footprints were updated.

- revision G - internal, not released

1. Changed the Li-Po charger to BL4054B (was MCP73833 previously).

- revision H - limited release, changes:

1. CH340 now powered only from USB (not from battery). Added D7 and D8 on TX/RX, levels are adjusted via R35 and R40. Pin #20 now disconnected. SY8089AAAC(SOT23-5) devider now R20 49.9k and R19 220k. R24 now 470ê.
2. Added battery voltage sense option. This is controlled with jumper BAT/BUT1, C30, R9, and R10. It is not enabled by default to maintain compatiblity with previous revisions.
3. Added 3D models of the components.
4. L2 now 2.2uH/1.5A/DCR=72mR/20%/3.00x3.00x1.50mm/CD32(NR3015T2R2M). Some improvements on ESP-WROOM-32_MODULE package.

- revision H1 - released, changes:

Improved start up for certain hardware setups. Revision H1 has same PCB as revision H (and the PCB of revision H1 says "H") but couple of component changes:

1. Changed R24 from 470k/0603 to 1k/0603, the 1k resistor is soldered over the 470k one to reduce chances of damage.

- revsion I - released, changes:

Applied the changes from revision H1 to the PCB and the design, and further improved start-up reliability by adding a capacitor:

1. Added C31(1uF/10V/10%/X5R/C0603) on the ESP_EN signal and R24's value changed from 1k/0603 to 10k/0603;
2. Added several text lables at the botttom of the board about CAN's connector signals and the battery polarity.

- revision I1 - released, changes:

1. Changed C18 from NA(10uF/6.3V/20%/X5R/C0603) to 1nF/50V/5%/NPO(COG)/C0603 - this improves Ethernet reliability during relay switcing;

- revision I2 - released, changes:

1. Changed U8, from NCP303LSN27T1G(SOT-23-5) to NCP303LSN18T1G(SOT-23-5).

- revision I3 - released, changes:

1. Changed U6 from MCP2562-E/SN(SOIC-8_150mil) to SIT1042T/3(SOIC-8_150mil).

- revision I4 - released

1. Changed U2 value to NCV303LSN23T1G.

- revision J - not released

1. Due to global unavailability of MCP2562-E/SN chips, it was necessary to adjust the CAN schematic for compatiblity with MCP2551/SIT1050 too. Following changes were required:
- U6 changed from MCP2562-E/SN(SOIC-8_150mil) to SIT1050T(SOIC-8_150mil);
- Added R12, NA/R0603;
- Added R13, 330R/R0603;
- Added R15, 680R/R0603;
2. CE sign added in the PCB;
3. Changed C18 from NA(10uF/6.3V/20%/X5R/C0603) to 1nF/50V/5%/NPO(COG)/C0603 - this improves Ethernet behavior during relay switching;
4. Added U11(VDA2710NTA(SOT-23)), D10(BAT54A(SOT23-3)), R58(49.9R/1%/R0402), R52(220k/R0603) and C32(1uF/10V/10%/X5R/C0603), so that we can have option to replace the reset chip NCP303LSN27T1G(SOT-23-5). Changed U8 to (NA(NCP303LSN27T1G(SOT-23-5))) and C28 to NA(470nF/16V/10%/X7R/C0603);
5. Added D9(1N5819S4(SOD-123)), to rptoect ESP_EN from 4.2V surge during programming; also to protect C32 gettomg cjarged from DTR output of CH340, else new reset schematic (VDA2710NTA) has random timing after pressing reset button;
6. Added ESD protection on the Ethernet, TVS1(ESDS314DBVR(SOT-23-5));
7. R28, R31, R32, and R33 changed from 49.9R/1%/R0603 to 49.9R/1%/R0402;
8. U7 changed from RPM7236-H8 to VS1838B.

- revision K - released

1. Labelling improvements;
2. Removed "GPIO4" from the "Internal Bootstrapping Resistors" table.


- revision K1 - released

1. Changed reset IC from VDA2710NTA(SOT-23)to HX7027 (due to unavailability of VDA2710NTA).
2. HX7027 has slightly bigger consumption and when powered trought R52 the voltage might get too low; so also changed R52 from 220k to 100k.

- revision K2 - released
------
1. Changed battery chager to TP4054-42-SOT25-R;
2. Changed R8 from 2k to 10k to reduce battery charger current to 100mA.

- revision L - released
------

1. Migated the whole project to KiCad7;
2. R57 deleted to reduce possible environment noise picking in combination with relay switching;
3. Relays changed;
4. U2-SY8089 changed to TPS62A02ADRLR(SOT-563-6);
5. CH340 changed with smaller variant ch340X.