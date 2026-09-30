# IoT-Based EV Charge Monitoring System

An IoT-enabled system that monitors the charging voltage, current, power and energy consumption of a Li-ion battery (EV battery model) in real time, and allows remote monitoring and control of charging through the Blynk mobile/web dashboard.

## Abstract
With the growing adoption of electric vehicles, monitoring the charging process is essential for safety, efficiency and energy management. This project presents a low-cost IoT-based solution that measures charging voltage and current using sensors, computes power and energy on an ESP32 microcontroller, displays the readings locally on an LCD, and transmits the data to the Blynk cloud over Wi-Fi. The user can view live charging parameters and switch the charging supply ON or OFF remotely. Built-in protection automatically stops charging when the voltage or current exceeds the set limits.

## Objectives
- Measure charging voltage and current accurately using sensors.
- Calculate power and energy consumed during charging.
- Display real-time readings on a local LCD.
- Enable remote monitoring and control through a mobile/web dashboard.
- Protect the battery through automatic overvoltage and overcurrent cutoff.

## System Architecture
```
Power Supply -> Relay -> ACS712 Current Sensor -> Li-ion Battery
                                   |                  |
                          Voltage Sensor -------------+
                                   |
                                 ESP32 --> LCD Display
                                   |
                                 Wi-Fi
                                   |
                              Blynk Cloud --> Mobile / Web Dashboard
```

## Working Principle
1. When the battery is connected to the charger, the voltage sensor measures the charging voltage and the ACS712 sensor measures the charging current.
2. The ESP32 reads these values and calculates power (P = V x I) and energy (E = sum of P x dt).
3. Voltage, current and power are shown on the LCD.
4. The data is sent to the Blynk cloud over Wi-Fi and displayed on the dashboard.
5. The user can turn charging ON or OFF from the dashboard, which operates the relay module.
6. If the voltage or current crosses the set limit, the relay switches off automatically.

## Hardware Components
| Component | Purpose |
|---|---|
| ESP32 / Arduino | Main microcontroller |
| Voltage sensor | Measures charging voltage |
| ACS712 current sensor | Measures charging current |
| ESP8266 Wi-Fi module | Internet connectivity (only when using Arduino) |
| 16x2 I2C LCD | Local display of voltage, current and power |
| Relay module | Controls the charging supply |
| Li-ion battery | Battery under charge (EV battery model) |
| Power supply | Charging source and control circuit supply |

## Software and Tools
- Arduino IDE (C/C++ firmware)
- Blynk IoT platform (dashboard and cloud)
- Python, pandas and matplotlib (optional data analysis)

## Blynk Datastreams
| Virtual Pin | Parameter | Type |
|---|---|---|
| V0 | Voltage (V) | Double |
| V1 | Current (A) | Double |
| V2 | Power (W) | Double |
| V3 | Energy (Wh) | Double |
| V4 | Charging switch (ON/OFF) | Integer |

## Features
- Real-time voltage, current, power and energy monitoring
- Local LCD display and remote Blynk dashboard
- Remote charging ON/OFF control using a relay
- Automatic overvoltage and overcurrent protection
- Relay stays OFF at power-up for safety
- Low-cost and easily expandable design

## Project Structure
```
ev-charge-monitoring/
├── firmware/ev_charge_monitor/   # ESP32 source code
├── dashboard/                    # Blynk configuration
├── scripts/                      # Python data analysis
├── hardware/                     # Components list, wiring, circuit diagram
├── docs/                         # Photos and screenshots
├── README.md
└── .gitignore
```

## Getting Started
1. Install the ESP32 board package in Arduino IDE, then install the **Blynk** and **LiquidCrystal I2C** libraries.
2. Create a Blynk template with the datastreams listed above and note the Template ID and Auth Token.
3. In `firmware/ev_charge_monitor/`, copy `secrets_example.h` to `secrets.h` and enter your Wi-Fi and Blynk credentials.
4. Connect the hardware as shown in `hardware/wiring.md`.
5. Upload the code to the ESP32 and open the Serial Monitor at 115200 baud.
6. Open the Blynk dashboard to monitor and control charging.

## Applications
- EV and e-bike charging stations
- Home and public charging monitoring
- Battery management and energy tracking
- Educational and research projects

## Future Enhancements
- Charging cost estimation
- Charging history and data analytics
- Battery State of Charge (SOC) estimation
- Alerts and notifications on abnormal conditions

## Author
**Mariyammal K**
Department of Electrical and Electronics Engineering (EEE)
