#include "Bounce2.h"
int analogPin = A0; // Change to the ANALOG pin you want to use.
int resetPin = 19;   // You can assign a button to reset the values.
Bounce reset = Bounce(resetPin, 50);
int hi;
int lo;

void setReset(){
  hi = analogRead(analogPin);
  lo = hi;
  Serial.print("High: "); Serial.print(hi); Serial.print("\t");
  Serial.print("Low: "); Serial.println(lo);
}

void setup(){
  pinMode(resetPin, INPUT_PULLUP);
  setReset();
}

void loop(){
  reset.update();
  if(reset.rose()){
    setReset();
  }

  int newVal = analogRead(analogPin);
  if (newVal > hi){
    hi = newVal;
    Serial.print("High: "); Serial.print(hi); Serial.print("\t");
    Serial.print("Low: "); Serial.println(lo);
  }
  else if (newVal < lo){
    lo = newVal;
    Serial.print("High: "); Serial.print(hi); Serial.print("\t");
    Serial.print("Low: "); Serial.println(lo);
  }
}
