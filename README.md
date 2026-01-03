# Smart Distance Assistant (ESP32)

**Author:** Divyarth Jain  
**Version:** 1.0.0  
**License:** Public Domain

## 📖 Project Overview
A high-precision proximity sensing system built on the **ESP32**. This device measures distance using a Time-of-Flight (ToF) laser sensor and provides real-time feedback through a visual dashboard (OLED) and a dynamic "Traffic Light" RGB LED system.

It is designed for applications like smart parking assistants, robot obstacle avoidance, or precise distance measurement tools.

## 🌟 Key Features
* **Laser Precision:** Uses the VL53L0X sensor for millimeter-level accuracy up to ~1.2 meters.
* **Visual Dashboard:** 1.3" OLED display (SH1106) showing:
    * Real-time distance in cm (1 decimal precision).
    * Dynamic progress/loading bar based on proximity.
    * Status zones ("APPROACHING", "WARNING", "STOP").
* **Smart RGB Alerts:**
    * **> 100cm:** 🔴 **Red** (Standby / Out of Range)
    * **55cm - 100cm:** 🟢 ➔ 🟡 **Green to Yellow Gradient** (Safe Zone)
    * **10cm - 55cm:** 🟡 ➔ 🔴 **Yellow to Red Gradient** (Warning Zone)
    * **< 10cm:** 🔴 **Solid Red** (Danger / Stop)

## 🛠️ Hardware Requirements

| Component | Specifications | Quantity |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 DevKit V1 (DOIT / WROOM-32) | 1 |
| **Distance Sensor** | VL53L0X (Time-of-Flight) | 1 |
| **Display** | 1.3" OLED I2C (SH1106 Driver) | 1 |
| **Indicator** | RGB LED (Common Cathode / 4-Pin) | 1 |
| **Resistors** | 220Ω or 330Ω (For LED legs) | 3 |
| **Wiring** | Jumper Wires & Breadboard | 1 Set |

## 🔌 Pinout & Wiring Configuration

### 1. I2C Bus (Sensor & Display)
*Note: Both the VL53L0X and OLED share the same I2C pins.*

| ESP32 Pin | VL53L0X Pin | OLED Pin | Description |
| :--- | :--- | :--- | :--- |
| **3.3V** | VCC / VIN | VCC | Power (3.3V Logic) |
| **GND** | GND | GND | Ground |
| **GPIO 21** | SDA | SDA | I2C Data |
| **GPIO 22** | SCL | SCL | I2C Clock |

### 2. RGB LED (Common Cathode)

| ESP32 Pin | RGB LED Pin | Description |
| :--- | :--- | :--- |
| **GND** | Longest Pin | Common Ground (Cathode) |
| **GPIO 4** | Red Leg | PWM Control Channel |
| **GPIO 16** | Green Leg | PWM Control Channel |
| **GPIO 17** | Blue Leg | PWM Control Channel |

## 💻 Software Setup

### Prerequisites
* **Arduino IDE** (v2.0+)
* **ESP32 Board Manager** installed.

### Required Libraries
Install the following libraries via the Arduino Library Manager (`Ctrl+Shift+I`):

1.  `Adafruit VL53L0X` (by Adafruit)
2.  `Adafruit SH110X` (by Adafruit) - *Crucial for 1.3" displays*
3.  `Adafruit GFX Library` (by Adafruit)
4.  `Adafruit BusIO` (by Adafruit)

## 🚀 Installation & Upload Procedure

1.  **Assembly:** Wire all components according to the Pinout table.
2.  **Open Project:** Open the `.ino` file in Arduino IDE.
3.  **Select Board:** Go to `Tools` -> `Board` -> `DOIT ESP32 DEVKIT V1`.
4.  **Important Upload Tip:**
    * *Issue:* The VL53L0X sensor can sometimes cause upload failures ("Timed out waiting for packet header").
    * *Fix:* Unplug the **3.3V power wire** connected to the sensor **before** clicking Upload.
5.  **Flash:** Click the **Upload** button.
6.  **Reconnect:** Once the IDE says "Done Uploading," plug the sensor power wire back in.
7.  **Reset:** Press the **EN** button on the ESP32 to restart the code.

## ⚙️ Logic Customization

You can modify the safety zones directly in the code:

```cpp
// Logic Thresholds
if (dist_cm < 10) { ... }  // Danger Zone (STOP)
if (dist_cm > 55) { ... }  // Gradient Transition Point
if (dist_cm > 100) { ... } // Max Range
