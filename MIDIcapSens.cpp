#include "MIDIcapSens.h"

// constructors
MIDIcapSens::MIDIcapSens(){};

MIDIcapSens::MIDIcapSens(int p, byte num){
  pin = p;
  number = num;
  value = 0;
  offThreshold = 1150;
  onThreshold = 1500;
  upperLimit = 4500;
  afterRelease = false;
  outLo = 0;
  outHi = 127;
  waiting = false;
  waitTime = 10; // millis
  timer = 0;
  hovered = false;
  touched = false;
  state = false;
};

MIDIcapSens::MIDIcapSens(int p, byte num, byte min, byte max){
  pin = p;
  number = num;
  value = 0;
  offThreshold = 1150;
  onThreshold = 1500;
  upperLimit = 4500;
  afterRelease = false;
  outLo = min;
  outHi = max;
  waiting = false;
  waitTime = 10; // milliseconds
  timer = 0;
  hovered = false;
  touched = false;
  state = false;
};

// destructor
MIDIcapSens::~MIDIcapSens(){
};

int MIDIcapSens::read(){
  int newValue = touchRead(pin);
  if (waiting){ // Wait briefly to avoid phase shifting.
    if (millis() - timer > waitTime){
      waiting = false;
    }
    newValue = -1;
  }
  else {
    if (newValue >= upperLimit && touched == false){     // rising edge
      newValue = outHi;
      hovered = false;
      touched = true;
      timer = millis();
      waiting = true;
    }
    else if (newValue < onThreshold && touched == true){  // falling edge
      newValue = outLo;
      touched = false;
      if (afterRelease){
        hovered = true;
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

int MIDIcapSens::send(){
  int newValue = read();
  if (newValue >= 0){
    usbMIDI.sendNoteOn(number, outHi, MIDIchannel);
    value = newValue;
  }
  return newValue;
};

void MIDIcapSens::setNoteNumber(byte num){ // Set the NOTE number.
  number = num;
};

void MIDIcapSens::setThresholds(int offT, int onT){
  offThreshold = offT;
  onThreshold = onT;
};

void MIDIcapSens::setThresholds(int offT, int onT, int upL){
  offThreshold = offT;
  onThreshold = onT;
  upperLimit = upL;
};

void MIDIcapSens::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
};