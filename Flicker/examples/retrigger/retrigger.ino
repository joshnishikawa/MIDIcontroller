#include <Flicker.h>

// As long as the input is held, the LED will blink

const uint8_t TOUCH_PIN = 0;
const uint8_t LED = 13;

// A variable to store the current LED state
int ledState = LOW;

// Instantiate a Flicker object with a threshold of 1300
// YOU LIKELY NEED TO ADJUST THAT THRESHOLD.
Flicker flick = Flicker(TOUCH_PIN, 1300); 

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  if (flick.update()){
   bool value = flick.read();
   if ( value == true) {
     flick.retrigger(500);
     ledState =!ledState;
   }
   else {
     ledState = LOW;
   }
    digitalWrite(LED, ledState );
  }
}

