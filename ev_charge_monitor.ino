/*
  IoT-Based EV Charge Monitoring System
  Board   : ESP32
  Sensors : ACS712 current sensor, voltage sensor (divider) module
  Output  : 16x2 I2C LCD, relay module (controls charging supply)
  Cloud   : Blynk IoT

  Blynk virtual pins:
    V0 = Voltage (V)   V1 = Current (A)   V2 = Power (W)
    V3 = Energy (Wh)   V4 = Charging switch (1 = ON, 0 = OFF)

  NOTE: Constants below are typical values. Calibrate against a multimeter.
*/

#include "secrets.h"            // must come before the Blynk include
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pins ----------
const int VOLTAGE_PIN = 34;     // ADC1
const int CURRENT_PIN = 35;     // ADC1
const int RELAY_PIN   = 26;
const bool RELAY_ACTIVE_LOW = true;   // most relay modules turn ON with LOW

// ---------- ADC ----------
const float ADC_REF = 3.3;
const int   ADC_MAX = 4095;

// ---------- Voltage sensor ----------
// Ratio = actual voltage / voltage at ADC pin (25 V module = 5.0)
const float VOLTAGE_RATIO = 5.0;

// ---------- Current sensor (ACS712) ----------
// Sensitivity in V/A: 5A = 0.185, 20A = 0.100, 30A = 0.066
const float ACS_SENSITIVITY = 0.185;
// ACS712 runs on 5 V, so divide its output down for the ESP32.
// Ratio = sensor output voltage / voltage at ADC pin (1.5 for 10k + 20k style divider; set yours)
const float ACS_DIVIDER_RATIO = 1.5;

// ---------- Protection limits (set for your battery) ----------
const float CUTOFF_VOLTAGE = 4.2;   // V, full charge for a single Li-ion cell
const float CUTOFF_CURRENT = 2.0;   // A, overcurrent limit

// ---------- Timing ----------
const unsigned long SAMPLE_INTERVAL_MS = 1000;

LiquidCrystal_I2C lcd(0x27, 16, 2);   // change to 0x3F if your LCD does not respond
BlynkTimer timer;

float acsZeroVolts = 2.5;
float voltage = 0, current = 0, power = 0, energyWh = 0;
bool chargingOn = false;
unsigned long lastSample = 0;

// Average many ADC readings to reduce noise, returns volts at the pin
float readPinVolts(int pin, int samples = 100) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(200);
  }
  return (sum / (float)samples) * ADC_REF / ADC_MAX;
}

void setRelay(bool on) {
  chargingOn = on;
  digitalWrite(RELAY_PIN, (on == RELAY_ACTIVE_LOW) ? LOW : HIGH);
}

// Blynk switch on V4
BLYNK_WRITE(V4) {
  setRelay(param.asInt() == 1);
}

void sampleAndDisplay() {
  unsigned long now = millis();
  float dtSeconds = (now - lastSample) / 1000.0;
  lastSample = now;

  voltage = readPinVolts(VOLTAGE_PIN) * VOLTAGE_RATIO;
  float sensorVolts = readPinVolts(CURRENT_PIN) * ACS_DIVIDER_RATIO;
  current = fabs((sensorVolts - acsZeroVolts) / ACS_SENSITIVITY);
  if (current < 0.05) current = 0;   // ignore noise near zero

  power = voltage * current;                    // W
  if (chargingOn) energyWh += power * dtSeconds / 3600.0;   // Wh

  // Protection: cut the supply on over-voltage or over-current
  if (chargingOn && (voltage >= CUTOFF_VOLTAGE || current >= CUTOFF_CURRENT)) {
    setRelay(false);
    Blynk.virtualWrite(V4, 0);
    Serial.println("Protection cutoff: charging stopped");
  }

  // LCD
  char line1[17], line2[17];
  snprintf(line1, sizeof(line1), "V:%5.2f I:%5.2fA", voltage, current);
  snprintf(line2, sizeof(line2), "P:%5.1fW %s", power, chargingOn ? "ON " : "OFF");
  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);

  // Blynk
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, voltage);
    Blynk.virtualWrite(V1, current);
    Blynk.virtualWrite(V2, power);
    Blynk.virtualWrite(V3, energyWh);
  }

  Serial.printf("V=%.2f V  I=%.3f A  P=%.2f W  E=%.3f Wh  Relay=%s\n",
                voltage, current, power, energyWh, chargingOn ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  pinMode(RELAY_PIN, OUTPUT);
  setRelay(false);                    // charging OFF at power-up

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("EV Charge Monitor");
  lcd.setCursor(0, 1); lcd.print("Starting...");

  // Zero-current offset, measured with the relay OFF (no charging current)
  delay(500);
  acsZeroVolts = readPinVolts(CURRENT_PIN, 500) * ACS_DIVIDER_RATIO;
  Serial.print("ACS712 zero offset (V): ");
  Serial.println(acsZeroVolts, 3);

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  lastSample = millis();
  timer.setInterval(SAMPLE_INTERVAL_MS, sampleAndDisplay);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
}
