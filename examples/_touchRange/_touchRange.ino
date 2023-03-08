#include "Bounce2.h"
int touchPin = 22; // Change to the TOUCH pin you want to use.
int resetPin = 19; // you can set a pin to reset hi/lo
Bounce reset = Bounce(resetPin, 50);
int hi;
int lo;

void setReset(){
  hi = touchRead(touchPin);
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

  int newVal = touchRead(touchPin);
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
