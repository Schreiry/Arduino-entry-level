/***************************************************
    Definition of pins and variables
 ***************************************************/
int ledPin = 3;       // Pin for LED (PWM)
int analogPin = A0;    // Pin for LED (PWM)
int val = 0;          // Current value from the potentiometer
int minVal = 1023;    // Minimum value (updated during calibration)
int maxVal = 0;       // Maximum value (updated after calibration)

/***************************************************
   Setup function: configuration and calibration
 ***************************************************/
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  /***********************************************
    Calibration step: potentiometer
    Over 5 seconds, turn from minimum to maximum
   ***********************************************/
  Serial.println("Calibration... Turn the potentiometer from minimum to maximum");
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // 5 seconds for calibration
    val = analogRead(analogPin);
    if (val < minVal) minVal = val;     // Update minimum value
    if (val > maxVal) maxVal = val;     // Update maximum value
    delay(10);                          // Small delay for stability
  }

  /****************************
         Display calibration results
   ****************************/
  Serial.print("Minimum value :  ");
  Serial.println(minVal);
  Serial.print("Maximum value :  ");
  Serial.println(maxVal);
}

/********************************************************
        Loop function: main logic for brightness control
 ********************************************************/
void loop() {
  val = analogRead(analogPin);          // Read current value
  Serial.println(val);                  // Output data for debugging
  int mappedVal = map(val, minVal, maxVal, 0, 255); // Map to 0–255 range
  mappedVal = constrain(mappedVal, 0, 255); // Constrain range
  analogWrite(ledPin, mappedVal);       // Set brightness
}
