# Arduino UNO Project: Soil Moisture & Water Level Monitoring

## 🌟 Overview

Welcome to a refined journey of horticultural stewardship powered by **Arduino UNO**. In this repository, we breathe life into sensors and LEDs, forging a graceful yet robust system that measures soil moisture and water level—ensuring your plants and reservoirs remain in perfect harmony.

🛠️ **Key Features:**

- **Analog Sensing**: Dual-channel measurement of soil moisture and water depth via A0 and A1.
- **I2C Display**: Live feedback on a 16×2 LCD screen, eliminating cryptic serial logs.
- **LED Status Indicators**: Green, yellow, and red LEDs pulse to reflect environmental conditions.
- **Adaptive Thresholds**: Customizable dry, normal, and wet thresholds with dynamic blinking animations.
- **Non-Blocking Design**: Crafted around `millis()` for responsiveness—no blocking `delay()` calls in the main loop.

---

## 📋 Table of Contents

1. [Hardware Requirements](#hardware-requirements)
2. [Circuit Diagram](#circuit-diagram)
3. [Sensor Principles](#sensor-principles)
   - [Soil Moisture Sensor](#soil-moisture-sensor)
   - [Analog Water Level Sensor](#analog-water-level-sensor)
4. [Installation](#installation)
5. [Code Walkthrough](#code-walkthrough)
   - [Setup & Initialization](#setup--initialization)
   - [Reading & Processing](#reading--processing)
   - [LED Animation](#led-animation)
   - [Display Update](#display-update)
6. [Configuration & Calibration](#configuration--calibration)
7. [Best Practices](#best-practices)
8. [Future Enhancements](#future-enhancements)
9. [License](#license)

---

## 🛠 Hardware Requirements

| Component                      | Description                                   | Quantity |
|--------------------------------|-----------------------------------------------|----------|
| **Arduino UNO**                | Microcontroller board                         | 1        |
| **16×2 I2C LCD Display**       | I2C interface, reduces wiring clutter         | 1        |
| **Soil Moisture Sensor**       | Measures soil conductivity via probe pair     | 1        |
| **Analog Water Level Sensor**  | Detects liquid presence/depth                 | 1        |
| **LEDs (Green, Yellow, Red)**  | Visual status indicators                      | 3        |
| **220 Ω Resistors**            | Current limiting for LEDs                    | 3        |
| **Breadboard & Jumper Wires**  | Prototyping convenience                       | Assorted |

---

## 🔌 Circuit Diagram

![Wiring Diagram](circuit.png)

1. **Power & Ground**: 5 V → VCC of sensors/display; GND → GND.
2. **Sensors**:
   - Soil Moisture → **A0**
   - Water Level → **A1**
3. **I2C LCD**:
   - SDA → **A4**
   - SCL → **A5**
4. **LEDs**:
   - Green → **D7**
   - Yellow → **D6**
   - Red → **D5**
   - Each LED in series with 220 Ω resistor.

---

## 🧪 Sensor Principles

### Soil Moisture Sensor

The soil sensor operates on the principle of electrical conductivity between two metal probes. Moist soil conducts electricity better (lower resistance), yielding **lower** analog readings. Dry soil resists current flow, producing **higher** values. Readings range from **0** (very wet) to **1023** (very dry).

### Analog Water Level Sensor

This probe-style sensor detects liquid contact along its length. The deeper the immersion, the greater the conductive surface area, and the **lower** the analog output. Values span **0** (fully immersed) to **1023** (dry).

---

## 🚀 Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/soil-water-monitor.git
   ```
2. Open `SoilWaterMonitor.ino` in the Arduino IDE.
3. Install dependencies:
   - **LiquidCrystal_I2C** library via Library Manager.
4. Upload to your Arduino UNO.

---

## 📜 Code Walkthrough

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SOIL_MOISTURE_PIN A0
#define WATER_LEVEL_PIN   A1
#define GREEN_LED         7
#define YELLOW_LED        6
#define RED_LED           5

#define DRY_THRESHOLD     600  // Above: too dry
#define NORMAL_THRESHOLD  400  // Between normal
#define WET_THRESHOLD     200  // Below: too wet

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
int ledState       = LOW;
int currentSpeed   = 1000;
int activeLed      = GREEN_LED;

void setup() { /* Serial, I2C init, LCD test, LED sequence */ }
void loop()  { /* Read, compute status, update LEDs & LCD */ }
```

### Setup & Initialization

- **Serial.begin(115200)**: Debugging and real-time logs.
- **Wire.begin()**: Activate I2C bus.
- **scanI2CDevices()**: Auto-detect connected I2C addresses.
- **lcd.init()/backlight()**: Retry mechanism to ensure display is ready.
- **LED startup test**: Confirms wiring and pin functionality.

### Reading & Processing

1. **Analog Reads**: `soil = analogRead(A0)`, `water = analogRead(A1)`.
2. **Threshold Check**:
   - `soil > DRY_THRESHOLD` → **TOO DRY**
   - `soil < WET_THRESHOLD` → **TOO WET**
   - Intermediate values mapped to **SLIGHTLY DRY/WET** or **NORMAL**.
3. **Status Logging**: Human-readable messages via `Serial.println()`.

### LED Animation

Function `updateLedState(newLed, newSpeed)`:

- Resets all LEDs on state or speed change.
- Uses non-blocking `millis()` to toggle the `activeLed` at `currentSpeed` intervals.

### Display Update

Function `updateDisplay(soil, water)`:

- Runs once per second (via `millis()`).
- Clears previous text to prevent ghosting.
- Prints two-line summary:
  ```
  Soil: 447
  Water: 495
  ```

---

## 🎚 Configuration & Calibration

- **Adjust Thresholds**: Tune `DRY_THRESHOLD`, `NORMAL_THRESHOLD`, and `WET_THRESHOLD` to match your soil type and water tank dimensions.
- **Sensor Longevity**: Continuous power accelerates corrosion on the soil probe—consider power-cycling the sensor or using a high-side MOSFET switch.

---

## 💡 Best Practices

- **Debounce Sensors**: Add simple moving-average if readings fluctuate.
- **Enclosure**: Protect electronics from moisture and UV exposure.
- **Modular Design**: Abstract thresholds and pin definitions into a config header for scalability.

---

## 🚀 Future Enhancements

- **Wireless Telemetry**: ESP8266/ESP32 integration for remote monitoring.
- **Mobile Dashboard**: Live graphs and push notifications.
- **Automated Irrigation**: Relay-controlled water pumps based on sensor data.

---

## 📜 License

This project is released under the **MIT License**. See [LICENSE](LICENSE) for details.

---

> _“Nurture your garden, nurture your code—both bloom with mindful care.”_

