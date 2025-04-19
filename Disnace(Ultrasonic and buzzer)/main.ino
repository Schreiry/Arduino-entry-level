#include "Ultrasonic.h"

// Define pins
#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define BUZZER_PIN 2  // Connect buzzer to this pin

// Define distance thresholds (in cm)
#define DISTANCE_THRESHOLD_1 10   // Very close
#define DISTANCE_THRESHOLD_2 20   // Close
#define DISTANCE_THRESHOLD_3 40
#define DISTANCE_THRESHOLD_4 60
#define DISTANCE_THRESHOLD_5 70
#define DISTANCE_THRESHOLD_6 80
#define DISTANCE_THRESHOLD_7 90   // Medium distance
#define DISTANCE_THRESHOLD_8 100
#define DISTANCE_THRESHOLD_9 110  // Far distance
#define MAX_DISTANCE 150         // Maximum reliable distance for HC-SR04

// Define buzzer tones
#define TONE_1 3000  // High pitch for very close objects
#define TONE_2 2700  // for close objects
#define TONE_3 2400  // for medium distance
#define TONE_4 2000   // Low pitch for far objects
#define TONE_5 1700 
#define TONE_6 1500 
#define TONE_7 1200 
#define TONE_8 900 
#define TONE_9 500 

// Initialize ultrasonic sensor
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initial message
  Serial.println("Ultrasonic Distance Measurement with Buzzer Alert");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // Measure distance in centimeters
  distance = ultrasonic.read(CM);
  
  // Display distance on Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("- - - - - - - -  ");
  
  // Determine which buzzer tone to play based on distance
  if (distance <= 0 || distance > MAX_DISTANCE) {
    // Invalid measurement or out of range, stop buzzer
    noTone(BUZZER_PIN);
    Serial.println("Object out of range or invalid measurement");
  }
  else if (distance <= DISTANCE_THRESHOLD_2) {
    // Close - medium-high frequency
    tone(BUZZER_PIN, TONE_2);
    delay(150);
    noTone(BUZZER_PIN);
    delay(150);
  }
  else if (distance <= DISTANCE_THRESHOLD_3) {
    // Medium distance - medium-low frequency
    tone(BUZZER_PIN, TONE_3);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);
  }
  else if (distance <= DISTANCE_THRESHOLD_4) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_4);
    delay(300);
    noTone(BUZZER_PIN);
    delay(300);
  }
  else if (distance <= DISTANCE_THRESHOLD_5) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_5);
    delay(400);
    noTone(BUZZER_PIN);
    delay(400);
  }
    else if (distance <= DISTANCE_THRESHOLD_6) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_6);
    delay(450);
    noTone(BUZZER_PIN);
    delay(450);
  }
    else if (distance <= DISTANCE_THRESHOLD_7) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_7);
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  }
    else if (distance <= DISTANCE_THRESHOLD_8) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_8);
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  }
    else if (distance <= DISTANCE_THRESHOLD_9) {
    // Far - low frequency, slow beeps
    tone(BUZZER_PIN, TONE_9);
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  }
  else {
    // Valid measurement but beyond the last threshold
    // No sound, just display the reading
    noTone(BUZZER_PIN);
  }
  
  // Small delay between measurements
  delay(200);
}
