#include "Bounce.h"
int analogPin = 20; // Change to the ANALOG pin you want to use.
int touchPin = 22;  // Change to the TOUCH pin you want to use.
Bounce reset = Bounce(12, 50); // You can assign a button to reset the values.
int newHiA;
int newLoA;
int newHiT;
int newLoT;

void setReset(){
  newHiA = analogRead(analogPin);
  newLoA = newHiA;
  newHiT = touchRead(touchPin);
  newLoT = newHiT;
  Serial.print("High Analog: "); Serial.println(newHiA);
  Serial.print("Low Analog : "); Serial.println(newLoA);
  Serial.print("High Touch: "); Serial.println(newHiT);
  Serial.print("Low Touch : "); Serial.println(newLoT);
}

void setup(){
  pinMode(12, INPUT_PULLUP);
  setReset();
}

void loop(){
  reset.update();
  if(reset.risingEdge()){
    setReset();
  }
  int newVal = analogRead(analogPin);
  if (newVal > newHiA){
    newHiA = newVal;
    Serial.print("High Analog: "); Serial.println(newHiA);
    Serial.print("Low Analog : "); Serial.println(newLoA);
  }
  else if (newVal < newLoA){
    newLoA = newVal;
    Serial.print("High Analog: "); Serial.println(newHiA);
    Serial.print("Low Analog : "); Serial.println(newLoA);
  }

// UNCOMMENT TO FIND THE RANGE OF A CAPACITIVE TOUCH INPUT
/*  newVal = touchRead(touchPin);
  if (newVal > newHiT){
    newHiT = newVal;
    Serial.print("High Touch: "); Serial.println(newHiT);
    Serial.print("Low Touch : "); Serial.println(newLoT);
  }
  else if (newVal < newLoT){
    newLoT = newVal;
    Serial.print("High Touch: "); Serial.println(newHiT);
    Serial.print("Low Touch : "); Serial.println(newLoT);
  }
*/
}
