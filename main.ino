#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Adafruit_VL53L0X.h"

// --- PINS ---
#define PIN_RED    4
#define PIN_GREEN  16
#define PIN_BLUE   17

// --- I2C CONFIG ---
#define i2c_Address 0x3C 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// --- OBJECTS ---
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  // 1. Setup RGB LED Pins
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // 2. Initialize Display
  if(!display.begin(i2c_Address, true)) { 
    Serial.println(F("SH1106 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
  
  // 3. Initialize Sensor
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

// Helper to set color (0-255)
void setColor(int r, int g, int b) {
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE, b);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); 

  display.clearDisplay();
  
  // Check for valid reading
  if (measure.RangeStatus != 4) {
    
    // Get precise distance for Display
    float dist_float = measure.RangeMilliMeter / 10.0;
    // Get integer distance for LED Logic
    int dist_cm = (int)dist_float;
    
    // --- LED LOGIC (Traffic Light Gradient) ---
    if (dist_cm < 10) {
      // DANGER (< 10cm) -> RED
      setColor(255, 0, 0); 
    } else if (dist_cm > 100) {
      // OUT OF RANGE (> 100cm) -> RED
      setColor(255, 0, 0);
    } else {
      // GRADIENT ZONE (10cm to 100cm)
      int redVal = 0;
      int greenVal = 0;

      if (dist_cm > 55) {
        // Zone: Green -> Yellow
        greenVal = 255;
        redVal = map(dist_cm, 100, 55, 0, 255);
      } else {
        // Zone: Yellow -> Red
        redVal = 255;
        greenVal = map(dist_cm, 55, 10, 255, 0);
      }
      
      // Safety clamp
      redVal = constrain(redVal, 0, 255);
      greenVal = constrain(greenVal, 0, 255);
      
      setColor(redVal, greenVal, 0);
    }

    // --- DISPLAY LOGIC ---
    
    // 1. Title Text
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.print("Distance Sensor");
    
    // 2. The Big Number (With Decimal)
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.print(dist_float, 1); // 1 Decimal place
    
    display.setTextSize(1);
    display.print(" cm");

    // 3. The Loading Bar
    // Map distance (0-100cm) to screen width (0-128 pixels)
    // We reverse it so the bar fills up as you get closer (optional, remove '128 -' to flip)
    int barWidth = map(dist_cm, 0, 100, 128, 0); 
    barWidth = constrain(barWidth, 0, 128);
    
    display.fillRect(0, 55, barWidth, 8, SH110X_WHITE); // Filled part
    display.drawRect(0, 55, 128, 8, SH110X_WHITE);      // Outline frame
    
  } else {
    // Sensor Error / Out of Range
    setColor(0, 0, 255); // Blue for error
    display.setCursor(10, 25);
    display.print("---");
  }
  
  display.display();
  delay(50); 
}
