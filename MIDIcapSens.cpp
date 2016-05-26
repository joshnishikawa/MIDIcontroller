#include "MIDIcapSens.h"

// constructors
MIDIcapSens::MIDIcapSens(){};

MIDIcapSens::MIDIcapSens(int p, byte num){
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

MIDIcapSens::MIDIcapSens(int p, byte num, byte min, byte max){
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

int MIDIcapSens::send(){
  int newValue = read();
  if (newValue >= 0){
    usbMIDI.sendNoteOn(number, outHi, MIDIchannel);
    value = newValue;
  }
  return newValue;
};

int MIDIcapSens::chaos(){
  int newValue = touchRead(pin);
  if (waiting){ // Wait briefly to avoid phase shifting.
    if (millis() - timer > waitTime){
      waiting = false;
    }
  }
  else {
    if (newValue > loThreshold && touched == false){        // rising edge
      touched = true;
      newValue = -1;
    }
    else if (newValue <= offThreshold && touched == true){  // falling edge
      touched = false;
      usbMIDI.sendNoteOn(value, 0, MIDIchannel);
      timer = millis();
      waiting = true;
      newValue = 0;
    }
    else if (newValue > offThreshold && touched == true){   // send MIDI
      newValue = map(newValue, offThreshold, hiThreshold, outLo, outHi);
      newValue = constrain(newValue, outLo, outHi);
      if (newValue != value){
        usbMIDI.sendNoteOn(value, 0, MIDIchannel); // cuz we don't want TOTAL chaos
        usbMIDI.sendNoteOn(newValue, outHi, MIDIchannel);
        waitTime = 127 - newValue; // not necessary. just adds flavor.
        value = newValue;
        timer = millis();
        waiting = true;
      }
    }
    else {newValue = -1;}
  }
  return newValue;
};

void MIDIcapSens::setNoteNumber(byte num){ // Set the NOTE number.
  number = num;
};

void MIDIcapSens::setThresholds(int loT, int hiT){
  offThreshold = loT;
  loThreshold = loT;
  hiThreshold = hiT;
  afterRelease = false;
};

void MIDIcapSens::setThresholds(int offT, int loT, int hiT){
  offThreshold = offT;
  loThreshold = loT;
  hiThreshold = hiT;
  afterRelease = true;
};

void MIDIcapSens::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
};