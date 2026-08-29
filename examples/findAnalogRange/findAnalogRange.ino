/*
  This sketch is here for convenience. Use it to determine the usable analog
  range of a sensor or pot. It will print the highest and lowest values it sees.
  Use that info to set the input range or threshold for objects in your sketch.
*/

// Change to the ANALOG pin you want to use (e.g. A0)
const int analogPin = A0;

// Optional hardware button pin to reset the range (set to -1 if not using a physical button)
const int resetPin = -1;

int hi = 0;
int lo = 0;

void setReset(){
  hi = analogRead(analogPin);
  lo = hi;
  Serial.println();
  Serial.println(F("--- Range Reset ---"));
  Serial.print(F("Current: ")); Serial.print(hi);
  Serial.print(F("\tLow: ")); Serial.print(lo);
  Serial.print(F("\tHigh: ")); Serial.println(hi);
}

void setup(){
  Serial.begin(115200);
  while (!Serial && millis() < 2500); // Wait for USB Serial connection on boards like ESP32-S3 / SAMD / Leonardo

  if (resetPin >= 0) {
    pinMode(resetPin, INPUT_PULLUP);
  }

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("--- MIDIcontroller Analog Range Finder ---"));
  Serial.print(F("Analog Pin: ")); Serial.println(analogPin);
  Serial.println(F("Move the pot or sensor through its full physical range."));
  Serial.println(F("Send any character via Serial Monitor to reset."));
  Serial.println(F("========================================"));

  delay(200);
  setReset();
}

void loop(){
  // Reset range if user sends any input in Serial Monitor
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      Serial.read();
    }
    setReset();
  }

  // Reset range if optional hardware button is pressed
  if (resetPin >= 0 && digitalRead(resetPin) == LOW) {
    delay(200); // Debounce
    setReset();
  }

  int newVal = analogRead(analogPin);
  if (newVal > hi){
    hi = newVal;
    Serial.print(F("Current: ")); Serial.print(newVal);
    Serial.print(F("\tLow: ")); Serial.print(lo);
    Serial.print(F("\tHigh: ")); Serial.print(hi);
    Serial.print(F("\tSpan: ")); Serial.println(hi - lo);
  }
  else if (newVal < lo){
    lo = newVal;
    Serial.print(F("Current: ")); Serial.print(newVal);
    Serial.print(F("\tLow: ")); Serial.print(lo);
    Serial.print(F("\tHigh: ")); Serial.print(hi);
    Serial.print(F("\tSpan: ")); Serial.println(hi - lo);
  }

  delay(5);
}

