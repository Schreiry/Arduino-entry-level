// Define pin numbers
const int mq7Pin = A0;      // Analog pin for MQ-7 (CO) sensor
const int mq2Pin = A1;      // Analog pin for MQ-2 (Gas/Smoke) sensor
const int redLEDpin = 4;    // Digital pin for red LED
const int buzzerPin = 3;    // Digital pin for PASSIVE buzzer

// Define thresholds for analog readings
// These values are approximations and depend on individual sensor calibration
// and environmental conditions. Adjust them based on testing.
const int MQ7_CO_THRESHOLD = 305; // Example threshold for MQ-7 (CO), adjust as needed
const int MQ2_GAS_THRESHOLD = 290; // Example threshold for MQ-2 (Gas/Smoke), adjust as needed

void generateThreatSound() {
  // Generate an approaching threat sound pattern
  // Rising pitch with increasing tempo
  for (int i = 0; i < 3; i++) {
    for (int freq = 200; freq < 800; freq += 100) {
      tone(buzzerPin, freq); // Use buzzerPin here
      delay(50);
    }
    noTone(buzzerPin); // Use buzzerPin here
    delay(100);
  }

  // Final urgent beeps
  for (int j = 0; j < 5; j++) {
    tone(buzzerPin, 1000); // Use buzzerPin here
    delay(100);
    noTone(buzzerPin); // Use buzzerPin here
    delay(100);
  }
}

void setup() {
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Set LED and buzzer pins as output
  pinMode(redLEDpin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); // Pin for passive buzzer

  // --- Sensor Warm-up ---
  // MQ sensors require a warm-up period to stabilize readings.
  Serial.println("Warming up sensors...");
  delay(20000); // Wait for 20 seconds for initial warm-up. Adjust if needed.
  Serial.println("Sensors ready. Reading data:");
}

void loop() {
  // --- Read Sensor Values ---
  // analogRead() returns values from 0 to 1023 for a 5V system.
  int mq7AnalogValue = analogRead(mq7Pin);
  int mq2AnalogValue = analogRead(mq2Pin);

  // --- Print Readings to Serial Monitor (in English) ---
  Serial.print("MQ-7 (CO) Analog Reading: ");
  Serial.println(mq7AnalogValue); // Raw analog value from MQ-7

  Serial.print("MQ-2 (Gas/Smoke) Analog Reading: ");
  Serial.println(mq2AnalogValue); // Raw analog value from MQ-2

  // --- Check for High Gas/Smoke/CO Level and Trigger Alarm ---
  // Alarm is triggered if EITHER MQ-7 reading exceeds its CO threshold
  // OR MQ-2 reading exceeds its general gas/smoke threshold.
  if (mq7AnalogValue > MQ7_CO_THRESHOLD || mq2AnalogValue > MQ2_GAS_THRESHOLD) {
    // --- Activate Alarm ---
    digitalWrite(redLEDpin, HIGH); // Turn on the red LED
    generateThreatSound();         // Generate the threat sound pattern

    Serial.println("!!! DANGEROUS GAS/SMOKE/CO LEVEL DETECTED - THREAT !!!");
    // Optional: You could add more specific messages here if needed,
    // checking which specific sensor reading triggered the alarm.
    if (mq7AnalogValue > MQ7_CO_THRESHOLD && mq2AnalogValue > MQ2_GAS_THRESHOLD) {
      Serial.println("High readings from both MQ-7 (CO) and MQ-2 (Gas/Smoke).");
    } else if (mq7AnalogValue > MQ7_CO_THRESHOLD) {
      Serial.println("High reading from MQ-7 (CO).");
    } else { // mq2AnalogValue > MQ2_GAS_THRESHOLD
      Serial.println("High reading from MQ-2 (Gas/Smoke).");
    }

    Serial.println("--- Alarm Activated ---");
  } else {
    // --- Deactivate Alarm ---
    digitalWrite(redLEDpin, LOW); // Turn off the red LED
    noTone(buzzerPin);            // Stop the buzzer sound (important for passive buzzer)

    Serial.println("Gas/Smoke/CO Levels Normal.");
  }

  Serial.println("--------------------"); // Separator for clarity

  // --- Delay before next reading ---
  // Note: The delay here affects how often the sensor is read and how often
  // the threat sound can potentially repeat if the condition persists.
  // The threat sound itself has delays within it.
  delay(2000); // Wait for 2 seconds before taking the next reading
}