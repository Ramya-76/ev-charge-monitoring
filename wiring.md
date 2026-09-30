# Wiring (ESP32)

| Module pin | ESP32 pin | Notes |
|---|---|---|
| Voltage sensor S (signal) | GPIO 34 | ADC1 |
| Voltage sensor VCC / GND | 3.3V / GND | |
| ACS712 OUT | GPIO 35 (through voltage divider) | Divide the 5V-scale output down to 3.3V max |
| ACS712 VCC / GND | 5V (VIN) / GND | |
| LCD (I2C) SDA | GPIO 21 | |
| LCD (I2C) SCL | GPIO 22 | |
| LCD VCC / GND | 5V (VIN) / GND | |
| Relay IN | GPIO 26 | Active-LOW modules turn ON with LOW |
| Relay VCC / GND | 5V (VIN) / GND | |

Power path: Power supply -> Relay (COM/NO) -> ACS712 (IP+ / IP-) -> Li-ion battery.
The voltage sensor is connected across the battery terminals. Keep all grounds common.

Add your circuit diagram and block diagram images in this folder and link them here.
