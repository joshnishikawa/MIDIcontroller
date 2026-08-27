/*
  Flicker - rangeFinder
  
  This utility reads touch input and streams live readings to both
  the Serial Monitor and Serial Plotter. Use it to find your baseline (idle),
  hover, and contact thresholds for use in other sketches.
*/

#include <Flicker.h>

// TOUCH PIN: Automatically selected for your board architecture, or customize below:
#if defined(ARDUINO_ARCH_ESP32)
  #if defined(D0)
    const uint8_t touchPin = D0; // XIAO ESP32-S3 (D0 is GPIO 1 / TOUCH1)
  #elif defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)
    const uint8_t touchPin = 1;  // ESP32-S2 / ESP32-S3 touch pin (GPIO 1-14)
  #else
    const uint8_t touchPin = 4;  // Classic ESP32 touch pin T0 (GPIO 4)
  #endif
#elif defined(D0)
  const uint8_t touchPin = D0;   // XIAO SAMD21 / RP2040
#elif defined(__AVR__)
  const uint8_t touchPin = A0;   // AVR (Uno, Nano, Mega, Leonardo) analog pin
#else
  const uint8_t touchPin = 0;    // Teensy 3.x/LC (Pin 0 is TSI touch)
#endif

// Optional hardware button pin to reset baseline (set to -1 if not using a physical button)
const int resetPin = -1; // e.g. pin 2 with INPUT_PULLUP button to GND

int baseline = 0;
int highTouch = 0;
int lowTouch = 0;
unsigned long lastPrintTime = 0;

void setReset() {
  flickerTouchInit();
  flickerTouchRead(touchPin); // Discard initial hardware warmup read
  delay(50);

  long total = 0;
  for (uint8_t i = 0; i < 16; i++) {
    total += flickerTouchRead(touchPin);
    delay(2);
  }
  baseline = (int)(total / 16);
  highTouch = baseline;
  lowTouch = baseline;

  Serial.println();
  Serial.println(F("========================================"));
  Serial.print(F("Baseline (Idle) Calibrated: "));
  Serial.println(baseline);
  Serial.println(F("Streaming live values... Touch the pin/wire."));
  Serial.println(F("Send any character via Serial to re-calibrate."));
  Serial.println(F("========================================"));
  delay(200);
}

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 2500); // Wait for Serial Monitor on USB boards

  if (resetPin >= 0) {
    pinMode(resetPin, INPUT_PULLUP);
  }

  Serial.println(F("--- Flicker Range Finder ---"));
  Serial.print(F("Touch Pin: "));
  Serial.println(touchPin);

  delay(500);
  setReset();
}

void loop() {
  // Reset baseline if user sends any input in Serial Monitor
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      Serial.read();
    }
    setReset();
  }

  // Reset baseline if optional hardware button is pressed
  if (resetPin >= 0 && digitalRead(resetPin) == LOW) {
    delay(200); // Debounce
    setReset();
  }

  int currentVal = flickerTouchRead(touchPin);

  if (currentVal > highTouch) {
    highTouch = currentVal;
  }
  if (currentVal < lowTouch) {
    lowTouch = currentVal;
  }

  // Stream live readings every 50ms (formatted for Serial Monitor and Serial Plotter)
  if (millis() - lastPrintTime >= 50) {
    lastPrintTime = millis();
    
    int delta = currentVal - baseline;
    Serial.print(F("Touch:"));
    Serial.print(currentVal);
    Serial.print(F("\tBaseline:"));
    Serial.print(baseline);
    Serial.print(F("\tMax:"));
    Serial.print(highTouch);
    Serial.print(F("\tDelta:"));
    Serial.println(delta);
  }

  delay(5);
}
