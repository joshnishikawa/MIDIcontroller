#include "DMXcapSens.h"

// constructors
DMXcapSens::DMXcapSens(){};

DMXcapSens::DMXcapSens(int p, byte num){
  pin = p;
  number = num;
  value = 0;
  offThreshold = 1150;
  loThreshold = 1500;
  hiThreshold = 4500;
  afterRelease = false;
  outLo = 0;
  outHi = 127;
  waiting = false;
  waitTime = 10; // millis
  timer = 0;
  hovered = false;
  touched = false;
};

DMXcapSens::DMXcapSens(int p, byte num, byte min, byte max){
  pin = p;
  number = num;
  value = 0;
  offThreshold = 1150;
  loThreshold = 1500;
  hiThreshold = 4500;
  afterRelease = false;
  outLo = min;
  outHi = max;
  waiting = false;
  waitTime = 10; // millis
  timer = 0;
  hovered = false;
  touched = false;
};

// destructor
DMXcapSens::~DMXcapSens(){
};

int DMXcapSens::read(){
  int newValue = touchRead(pin);
  if (waiting){ // Wait briefly to avoid phase shifting.
    if (millis() - timer > waitTime){
      waiting = false;
    }
    newValue = -1;
  }
  else {
    if (newValue >= hiThreshold && touched == false){     // rising edge
      newValue = outHi;
      hovered = false;
      touched = true;
      timer = millis();
      waiting = true;
    }
    else if (newValue < loThreshold && touched == true){  // falling edge
      newValue = outLo;
      touched = false;
      if (afterRelease){
        hovered = true;
        newValue = outLo;
      }
      else {
        newValue = 0;
      }
      timer = millis();
      waiting = true;
    }
    else if (newValue < offThreshold && hovered == true){ // out of range
      newValue = 0;
      touched = false;
      hovered = false;
      timer = millis();
      waiting = true;
    }
    else {newValue = -1;}
  }
  return newValue;  
};

int DMXcapSens::send(){
  int newValue = read();
  if (newValue >= 0){
    usbDMX.sendNoteOn(number, outHi, DMXchannel);
    value = newValue;
  }
  return newValue;
};

void DMXcapSens::setNoteNumber(byte num){ // Set the NOTE number.
  number = num;
};

void DMXcapSens::setThresholds(int loT, int hiT){
  offThreshold = loT;
  loThreshold = loT;
  hiThreshold = hiT;
  afterRelease = false;
};

void DMXcapSens::setThresholds(int offT, int loT, int hiT){
  offThreshold = offT;
  loThreshold = loT;
  hiThreshold = hiT;
  afterRelease = true;
};

void DMXcapSens::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
};
