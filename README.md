# Smart Onion Storage and Monitoring System (SIH 2025)

An ESP32 IoT system that monitors onion storage conditions and controls a **12 V ventilation blower** automatically to reduce post-harvest spoilage. Live readings go to a **Blynk IoT** cloud dashboard.

Built for **Smart India Hackathon (SIH) 2025**, where it was selected as an advanced candidate in the initial round.

## Features
- **Temperature and humidity** sensing with a DHT11.
- **Second temperature probe:** an NTC thermistor read through a voltage divider and converted with the Steinhart/Beta equation.
- **Automatic fan control:** the blower switches at a 30 °C NTC temperature threshold.
- **Cloud dashboard (Blynk):**

| Virtual pin | Data |
|---|---|
| V0 | DHT11 temperature (°C) |
| V1 | DHT11 humidity (%) |
| V2 | NTC temperature (°C) |
| V3 | Fan status (1 = ON) |

## Hardware
| Part | GPIO |
|---|---|
| ESP32 Dev board | — |
| DHT11 | 4 |
| Fan driver (relay/MOSFET to 12 V blower) | 25 |
| NTC thermistor | 34 (ADC1) |

## Files
| Folder | Description |
|---|---|
| `sihattemt3/` | Offline version: DHT11 + fan control, no cloud |
| `sihattemt4/` | Full version: DHT11 + NTC + Blynk dashboard |

## Build and flash
1. Install the Arduino IDE with the **ESP32 board package**, plus the **DHT sensor library** and **Blynk** libraries.
2. In `sihattemt4.ino`, set `BLYNK_TEMPLATE_ID`, `BLYNK_AUTH_TOKEN`, `ssid` and `pass`.
3. Upload, then open the Blynk app or web dashboard.

## Author
**Keerthivasan J**, B.E. EEE, Sri Sairam Engineering College ([GitHub](https://github.com/Keerthivasan-cdm) · [LinkedIn](https://linkedin.com/in/keerthi-vasan-j-))
