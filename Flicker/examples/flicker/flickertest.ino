#include <Flicker.h>
#define BUTTON 23
#define LED 13
Flicker flick = Flicker( BUTTON, 1300, 1100); 

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  flick.update();
  if (flick.risingEdge()){
    digitalWrite(LED, HIGH);
    Serial.println("ON");
  }
  if (flick.fallingEdge()){
    digitalWrite(LED, LOW);
    Serial.println("O F F");
  }
}

