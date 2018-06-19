#include <Flicker.h>

/*
This example uses a capacitive touch input to detect a finger.
If you hold your finger on the input for long enough,
something will happen. The flicker library uses the "duration"
method to determine how long a capacitive touch input has
remained above a given threshold.
*/

const uint8_t TOUCH_PIN = 0;
const uint8_t LED = 13;
bool held = false; // Has the button been held (or released) for long enough?

// Instantiate a Flicker object with a threshold of 1300
// YOU LIKELY NEED TO ADJUST THAT THRESHOLD.
Flicker heldInput = Flicker(TOUCH_PIN, 1300);

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  heldInput.update();

  // has our input changed for more 1/2 a second?
  Serial.println(heldInput.duration());
  Serial.println(heldInput.state ? "on" : "O F F");
  if (heldInput.read() != held && heldInput.duration() > 500 ) {
    held = !held;
  }
  
  if (held){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}
