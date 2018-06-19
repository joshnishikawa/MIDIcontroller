#include <Flicker.h>

const uint8_t TOUCH_PIN = 0;
const uint8_t LED = 13;

// Instantiate a Flicker object with a threshold of 1300
// YOU LIKELY NEED TO ADJUST THAT THRESHOLD.
Flicker flick = Flicker(TOUCH_PIN, 1300); 

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  flick.update();
  if (flick.risingEdge()){
    digitalWrite(LED, HIGH);
  }
  if (flick.fallingEdge()){
    digitalWrite(LED, LOW);
  }
}

