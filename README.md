# ESP32-EVB

ESP32 wifi / BLE development board with Ethernet, relays, microSD card, CAN, infrared, Li-Po charger, GPIOs.

Product page: https://www.olimex.com/Products/IoT/ESP32/ESP32-EVB/open-source-hardware

Open-source hardware board made with open-source CAD software. Comes with either ESP32-WROOM-32E or ESP32-WROOM-32UE module, depending on variant. Board vairants are:

| Board variant | ESP32 module  | External antenna  | Temperature range |
| ------------- | ------------- | -------------  | ------------- |
| ESP32-EVB     | ESP32-WROOM-32E  | No  | 0-70°C  |
| ESP32-EVB-EA  | ESP32-WROOM-32UE  | Yes  | 0-70°C  |
| ESP32-EVB-EA-IND  | ESP32-WROOM-32UE  | Yes  | -40+85°C |
| ESP32-EVB-IND  | ESP32-WROOM-32E  | No  | -40+85°C |

ESP32-EVB - ESP32-WROOM-32E module, no external antenna, commercial temeprature range 0-70°C

ESP32-EVB-EA - ESP32-WROOM-32UE module, fitting external antenna, commercial temeprature range 0-70°C

ESP32-EVB-EA-IND - ESP32-WROOM-32UE module, fitting external antenna, industrial temeprature range -40+85°C

ESP32-EVB-IND - ESP32-WROOM-32E module, no external antenna, industrial temeprature range -40+85°C

You can find design hardware sources, schematic exports, BOMs, and more, in the "HARDWARE" folder.

You can find ESP-IDF and Arduino IDE examples, in the "SOFTWARE" folder.

# FAQ

---

**Q:** I have some trouble downloading my code to ESP32-EVB. Is there a way to force boot mode?

**A:** By default the board comes without BOOT button functioanlity. However, we have providedan option to can convert user BUT1 into BOOT button to force button but this requires hardware change and soldering. Soldering experience is required since it is a tight spot. You need to perform two changes:

- Solder the pads of R46 together (or solder 0R resistor in 0603 size in the R46 position there);
- Unsolder R14 and solder its pads together (or again solder 0R resistor in 0603 size in the R14 position).

After the two changes, to manually force boot mode, hold button BUT1 then reset the board with RST1. Both R46 and R14 are near the button BUT1. Notice that the button will still act as user button after the initial boot sequence. You can find more about what changing these resistors does if you inspect the schematic.

---

**Q:** The Ethernet doesn't start after a restart, it throws "init phy failed" error. What should I do?

**A:** Ethernet might hang of initialized too quickly after a reset. Add some delay at the start of the code before the Ethernet initialization. For example, if you are using the default Arduino IDE demo for LAN8720 try adding delay () after Serial.begin(), as shown below:
```
void setup()
{
Serial.begin(115200);
delay (2000);
WiFi.onEvent(WiFiEvent);
ETH.begin();
}
```
