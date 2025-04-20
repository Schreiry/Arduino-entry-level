
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define SOIL_MOISTURE_PIN A0
#define WATER_LEVEL_PIN   A1
#define GREEN_LED         7
#define YELLOW_LED        6
#define RED_LED           5

// Moisture thresholds (adjust as needed)
#define DRY_THRESHOLD     600    // Soil is too dry
#define NORMAL_THRESHOLD  400    // Soil is within normal range
#define WET_THRESHOLD     200    // Soil is very wet

// Try I2C address 0x27 or 0x3F if yours differs
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Changed from 0x26 to 0x27

// Animation variables
unsigned long previousMillis = 0;
int ledState       = LOW;
int currentSpeed   = 1000;
int activeLed      = GREEN_LED;

void setup() {
  Serial.begin(115200);
  
  // Extra delay for I2C devices to stabilize
  delay(100);
  
  // Initialize I2C
  Wire.begin();
  
  // Scan for any I2C devices at startup
  scanI2CDevices();
  
  // Attempt LCD initialization with retries
  bool lcdInitialized = false;
  for (int i = 0; i < 3; i++) {
    lcd.init();
    delay(100);
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(1000);
    lcdInitialized = true;
    break;
  }
  
  if (!lcdInitialized) {
    Serial.println("LCD initialization failed!");
  }
  
  // Configure LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // LED test sequence at startup
  digitalWrite(GREEN_LED, HIGH);
  delay(300);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  delay(300);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(300);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // Read sensor values
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int waterLevel   = analogRead(WATER_LEVEL_PIN);
  
  // Output readings to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(" | Water Level: ");
  Serial.println(waterLevel);
  
  // Determine moisture status
  int newLed   = GREEN_LED;
  int newSpeed = 1000; // Slow blink by default
  
  if (soilMoisture > DRY_THRESHOLD) {
    newLed   = RED_LED;
    newSpeed = 250;    // Fast blink for too dry
    Serial.println("Status: TOO DRY!");
  } else if (soilMoisture < WET_THRESHOLD) {
    newLed   = RED_LED;
    newSpeed = 250;    // Fast blink for too wet
    Serial.println("Status: TOO WET!");
  } else if (soilMoisture > NORMAL_THRESHOLD && soilMoisture <= DRY_THRESHOLD) {
    newLed   = YELLOW_LED;
    newSpeed = 500;    // Medium blink for slightly dry
    Serial.println("Status: SLIGHTLY DRY");
  } else if (soilMoisture >= WET_THRESHOLD && soilMoisture < NORMAL_THRESHOLD) {
    newLed   = YELLOW_LED;
    newSpeed = 500;    // Medium blink for slightly wet
    Serial.println("Status: SLIGHTLY WET");
  } else {
    Serial.println("Status: NORMAL");
  }
  
  // Update LED animation state
  updateLedState(newLed, newSpeed);
  
  // Refresh LCD display
  updateDisplay(soilMoisture, waterLevel);
  
  delay(10); // Small delay for stability
}

void updateLedState(int newLed, int newSpeed) {
  // If LED or speed changed, reset all LEDs
  if (newLed != activeLed || newSpeed != currentSpeed) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    
    activeLed    = newLed;
    currentSpeed = newSpeed;
  }
  
  // Blink animation based on currentSpeed
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= currentSpeed) {
    previousMillis = currentMillis;
    ledState       = !ledState;
    digitalWrite(activeLed, ledState);
  }
}

void updateDisplay(int soil, int water) {
  static unsigned long lastUpdate = 0;
  
  // Update display once per second
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    
    lcd.clear();  // Prevent overlapping text
    
    // First line: soil moisture
    lcd.setCursor(0, 0);
    lcd.print("Soil: ");
    lcd.print(soil);
    
    // Second line: water level
    lcd.setCursor(0, 1);
    lcd.print("Water: ");
    lcd.print(water);
  }
}

// Scans I2C bus and reports found devices
void scanI2CDevices() {
  byte error, address;
  int nDevices = 0;
  
  Serial.println("Scanning I2C bus...");
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("!");
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("I2C scan completed");
  }
}
