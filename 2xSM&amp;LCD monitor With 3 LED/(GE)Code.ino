#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// კავშირის პინები
#define SOIL_MOISTURE_PIN A0
#define WATER_LEVEL_PIN A1
#define GREEN_LED 7
#define YELLOW_LED 6
#define RED_LED 5

// ტენიანობის ზღვრული მნიშვნელობები (შეასწორეთ თქვენი საჭიროების მიხედვით)
#define DRY_THRESHOLD 600    // მშრალი ნიადაგი
#define NORMAL_THRESHOLD 400 // ნორმალური ტენიანობა
#define WET_THRESHOLD 200    // ძალიან ტენიანი

// სცადეთ სხვა I2C მისამართი - 0x27 ან 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);  // შეიცვალა 0x26-დან 0x27-ზე

// ანიმაციის ცვლადები
unsigned long previousMillis = 0;
int ledState = LOW;
int currentSpeed = 1000;
int activeLed = GREEN_LED;

void setup() {
  Serial.begin(115200);
  
  // დამატებითი დაყოვნება I2C მოწყობილობების სტაბილიზაციისთვის
  delay(100);
  
  // I2C-ის ინიციალიზაცია
  Wire.begin();
  
  // I2C მოწყობილობების სკანირების მცდელობა გაშვებისას
  scanI2CDevices();
  
  // LCD-ის ინიციალიზაცია განმეორებითი მცდელობებით
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
  
  // პინების დაყენება
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // LED-ების შემოწმება გაშვებისას
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
  // სენსორებიდან მნიშვნელობების წაკითხვა
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int waterLevel = analogRead(WATER_LEVEL_PIN);
  
  // გამოტანა Serial-ში გამართვისთვის
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(" | Water Level: ");
  Serial.println(waterLevel);
  
  // ტენიანობის მდგომარეობის განსაზღვრა
  int newLed = GREEN_LED;
  int newSpeed = 1000; // ნელი ციმციმი
  
  if(soilMoisture > DRY_THRESHOLD) {
    newLed = RED_LED;
    newSpeed = 250;    // სწრაფი ციმციმი
    Serial.println("Status: TOO DRY!");
  } else if(soilMoisture < WET_THRESHOLD) {
    newLed = RED_LED;
    newSpeed = 250;    // სწრაფი ციმციმი
    Serial.println("Status: TOO WET!");
  } else if(soilMoisture > NORMAL_THRESHOLD && soilMoisture <= DRY_THRESHOLD) {
    newLed = YELLOW_LED;
    newSpeed = 500;    // საშუალო ციმციმი
    Serial.println("Status: SLIGHTLY DRY");
  } else if(soilMoisture >= WET_THRESHOLD && soilMoisture < NORMAL_THRESHOLD) {
    newLed = YELLOW_LED;
    newSpeed = 500;    // საშუალო ციმციმი
    Serial.println("Status: SLIGHTLY WET");
  } else {
    Serial.println("Status: NORMAL");
  }
  
  // LED-ების მდგომარეობის განახლება
  updateLedState(newLed, newSpeed);
  
  // ინფორმაციის გამოტანა LCD-ზე
  updateDisplay(soilMoisture, waterLevel);
  
  delay(10); // მცირე დაყოვნება სტაბილურობისთვის
}

void updateLedState(int newLed, int newSpeed) {
  if(newLed != activeLed || newSpeed != currentSpeed) {
    // ყველა LED-ის გამორთვა მდგომარეობის შეცვლისას
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    
    activeLed = newLed;
    currentSpeed = newSpeed;
  }
  
  // ციმციმის ანიმაცია
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= currentSpeed) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(activeLed, ledState);
  }
}

void updateDisplay(int soil, int water) {
  static unsigned long lastUpdate = 0;
  
  if(millis() - lastUpdate >= 1000) { // განახლება ყოველ წამში
    lastUpdate = millis();
    
    // დისპლეის გასუფთავება ტექსტის გადაფარვის თავიდან ასაცილებლად
    lcd.clear();
    
    // პირველი სტრიქონი - ნიადაგის ტენიანობა
    lcd.setCursor(0, 0);
    lcd.print("Soil: ");
    lcd.print(soil);
    
    // მეორე სტრიქონი - წყლის დონე
    lcd.setCursor(0, 1);
    lcd.print("Water: ");
    lcd.print(water);
  }
}

// I2C მოწყობილობების სკანირების ფუნქცია
void scanI2CDevices() {
  byte error, address;
  int nDevices = 0;
  
  Serial.println("Scanning I2C bus...");
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("I2C scan completed");
  }
}
