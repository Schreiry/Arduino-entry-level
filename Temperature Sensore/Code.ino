#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Define pins and constants
#define DHTPIN 2
#define DHTTYPE DHT22
#define BUZZER_PIN 3
#define LED_PIN 4
#define TEMP_THRESHOLD 30.0  // Set your temperature threshold here

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust I2C address if needed

void setup() {
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  delay(2000);  // Initial stabilization for sensor
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();
  
  if (isnan(t) || isnan(h)) {
    // Display error message if reading fails
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check connection");
    
    // Ensure alarm is off
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  } else {
    // Display temperature and humidity
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1);  // 1 decimal place
    lcd.write('C');
    
    lcd.setCursor(0, 1);
    lcd.print("Hum:  ");
    lcd.print(h, 1);  // 1 decimal place
    lcd.write('%');

    // Check temperature threshold
    if (t >= TEMP_THRESHOLD) {
      // Activate alarm
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000);  // For passive buzzer (1kHz tone)
      // digitalWrite(BUZZER_PIN, HIGH);  // Use this for active buzzer
    } else {
      // Turn off alarm
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);  // For passive buzzer
      // digitalWrite(BUZZER_PIN, LOW);  // Use this for active buzzer
    }
  }
  
  delay(2000);  // Wait 2 seconds between readings (DHT22 requirement)
}
