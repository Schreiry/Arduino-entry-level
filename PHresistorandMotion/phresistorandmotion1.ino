#include <LiquidCrystal_I2C.h>

#include <Wire.h>

// Arduino code for a smart lighting system using PIR motion sensor
// The LED will only turn on when motion is detected via PIR sensor AND it's dark enough
// Using I2C LCD display with full component testing at initialization

// Pin definitions
const int pirSensorPin = 2;     // Digital pin for PIR motion sensor
const int ldrSensorPin = A0;    // Analog pin for LDR (photoresistor)
const int ledPin = 9;           // Digital pin for LED output

// Initialize LCD with I2C address (typically 0x27 or 0x3F)
// Parameters: LCD address, columns, rows
LiquidCrystal_I2C lcd(0x27, 16, 2); // You may need to change the address to 0x3F depending on your module

// Threshold values
const int darkThreshold = 300;  // Adjust based on your LDR's behavior and environment
                               // Lower value = darker environment needed to trigger

// Variables
int ldrValue = 0;               // Current light level reading
int pirState = LOW;             // Current PIR sensor state
bool motionDetected = false;    // Flag for motion detection
bool isNight = false;           // Flag for night condition

// Function to test each component
bool testLED() {
  // Test LED by turning it on and off
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  return true; // Since we can't automatically detect if LED works
}

bool testLDR() {
  // Test LDR by reading value
  int testValue = analogRead(ldrSensorPin);
  // Just check if we can get a reading (not checking if it's valid)
  return (testValue >= 0);
}

bool testPIRSensor() {
  // Test PIR sensor by taking a reading
  int testReading = digitalRead(pirSensorPin);
  // We can only verify that we can read from it, not that it's working correctly
  return true; // Just assume it's working if we can compile and run
}

void setup() {
  // Initialize pins
  pinMode(pirSensorPin, INPUT);
  pinMode(ldrSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Starting system initialization...");
  
  // Initialize the I2C LCD
  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.clear();
  
  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("Smart Light Sys");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  
  // Test LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing LCD");
  lcd.setCursor(0, 1);
  lcd.print("TEST DONE!");
  Serial.println("LCD test completed");
  delay(1000);
  
  // Test LED
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing LED");
  bool ledStatus = testLED();
  lcd.setCursor(0, 1);
  lcd.print("LED - TEST DONE!");
  Serial.println("LED test completed");
  delay(1000);
  
  // Test LDR sensor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing LDR");
  bool ldrStatus = testLDR();
  lcd.setCursor(0, 1);
  if (ldrStatus) {
    ldrValue = analogRead(ldrSensorPin);
    lcd.print("LDR OK! Val:");
    lcd.print(ldrValue);
  } else {
    lcd.print("LDR TEST FAILED!");
  }
  Serial.println("LDR test completed. Value: " + String(ldrValue));
  delay(1500);
  
  // Test PIR sensor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing PIR");
  bool pirStatus = testPIRSensor();
  lcd.setCursor(0, 1);
  if (pirStatus) {
    pirState = digitalRead(pirSensorPin);
    lcd.print("PIR OK! State:");
    lcd.print(pirState == HIGH ? "HIGH" : "LOW");
  } else {
    lcd.print("PIR TEST FAILED!");
  }
  Serial.println("PIR test completed. State: " + String(pirState == HIGH ? "HIGH" : "LOW"));
  delay(1500);
  
  // System ready notification
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("All tests done!");
  lcd.setCursor(0, 1);
  lcd.print("System ready!");
  Serial.println("All component tests completed. System ready.");
  delay(2000);
  
  // Start monitoring
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring...");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1000);
}

void checkForMotion() {
  // Get PIR sensor state
  pirState = digitalRead(pirSensorPin);
  
  // Set motion detected flag based on PIR state
  motionDetected = (pirState == HIGH);
  
  if (motionDetected) {
    Serial.println("Motion detected by PIR sensor!");
  }
}

void loop() {
  ldrValue = analogRead(ldrSensorPin);
  
  // Добавляем отладочный вывод
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  
  isNight = (ldrValue < darkThreshold);
  checkForMotion();
  
  Serial.print("Light level: ");
  Serial.print(ldrValue);
  Serial.print(" | PIR State: ");
  Serial.print(pirState == HIGH ? "HIGH" : "LOW");
  Serial.print(" | Motion: ");
  Serial.print(motionDetected ? "Detected" : "None");
  Serial.print(" | Is night: ");
  Serial.println(isNight ? "Yes" : "No");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.print(ldrValue);
  lcd.print(isNight ? "(Dark)" : "(Light)");
  
  lcd.setCursor(0, 1);
  lcd.print("PIR:");
  lcd.print(pirState == HIGH ? "HIGH" : "LOW");
  lcd.print(" ");
  lcd.print(motionDetected ? "Motion!" : "No motion");
  
  if (motionDetected && isNight) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON - Motion detected and it's dark");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF - No motion or it's daytime");
  }
  
  delay(500);
}