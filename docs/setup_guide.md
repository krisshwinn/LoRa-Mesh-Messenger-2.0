# Setup Guide

## 1. Software Installation

### Arduino IDE Setup
1. Install [Arduino IDE](https://www.arduino.cc/en/software).
2. Add Board URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` in Preferences -> Additional Board Manager URLs.
3. Open Boards Manager, search for and install **"Heltec ESP32 Dev-Boards"**.
4. Install the following libraries via the Library Manager:
   - **RadioLib**
   - **ArduinoJson**
   - **TinyGPSPlus**

## 2. Hardware Connections

Connect the NEO-6M GPS module to the Heltec WiFi LoRa 32 V3 as follows:

| Heltec V3 | GPS Module |
|-----------|------------|
| 3.3V      | VCC        |
| GND       | GND        |
| GPIO17    | TX         |
| GPIO16    | RX         |

## 3. Quick Start: Flash Devices
1. Connect the Heltec board via USB.
2. Select board: **"Heltec WiFi LoRa 32(V3)"**.
3. Open `firmware/LoRa_Messenger.ino`.
4. In `config.h`, change `DEVICE_ID` to a unique name for each device (e.g., `NODE_A`, `NODE_B`, `NODE_C`).
5. Click **Upload**.
