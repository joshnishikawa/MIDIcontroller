/*
  This sketch is here for convenience. Use it to determine the usable range of a
  capacitive touch sensor. It will print the highest and lowest values it sees.
  Use that info to set the input range or threshold for objects in your sketch.
*/

#include "Bounce2.h"

int touchPin = 23; // Change to the TOUCH pin you want to use.
int resetPin = 2; // you can set a pin to reset hi/lo
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

  int newVal = touchRead(17);
  newVal = touchRead(18);
  newVal = touchRead(19);
  newVal = touchRead(22);

  newVal = touchRead(touchPin);
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
