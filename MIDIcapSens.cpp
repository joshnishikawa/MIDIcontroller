#include "MIDIcapSens.h"

// constructors
MIDIcapSens::MIDIcapSens(){};

MIDIcapSens::MIDIcapSens(int p, int num){
  pin = p;
  channel = MIDIchannel;
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


MIDIcapSens::MIDIcapSens(int p, int num, int min, int max){
  pin = p;
  channel = MIDIchannel;
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
  int returnme = -1;
  if (waiting){ // Wait briefly to avoid phase shifting.
    if (millis() - timer > waitTime){
      waiting = false;
    }
  }
  else {
    int newValue = touchRead(pin);
    if (newValue >= hiThreshold && touched == false){     // rising edge
      usbMIDI.sendNoteOn(number, outHi, channel);
      touched = true;
      timer = millis();
      waiting = true;
      returnme = outHi;
    }
    else if (newValue < loThreshold && touched == true){  // falling edge
      touched = false;
      if (afterRelease){
        usbMIDI.sendNoteOn(number, outLo, channel);
        hovered = true;
        returnme = outLo;
      }
      else {
        usbMIDI.sendNoteOn(number, 0, channel);
        returnme = 0;
      }
      timer = millis();
      waiting = true;
    }
    else if (newValue < offThreshold && hovered == true){
      hovered = false;
      touched = false; // just in case
      usbMIDI.sendNoteOn(number, 0, channel);
      timer = millis();
      waiting = true;
      returnme = 0;
    }
  }
  return returnme;
};


int MIDIcapSens::chaos(){
  int returnme = -1;
  if (waiting){ // Wait briefly to avoid phase shifting.
    if (millis() - timer > waitTime){
      waiting = false;
    }
  }
  else {
    int newValue = touchRead(pin);
    if (newValue > loThreshold && touched == false){        // rising edge
      touched = true;
      returnme = outHi;
    }
    else if (newValue <= offThreshold && touched == true){  // falling edge
      touched = false;
      usbMIDI.sendNoteOn(value, 0, channel);
      timer = millis();
      waiting = true;
      returnme = 0;
    }
    else if (newValue > offThreshold && touched == true){   // send MIDI
      newValue = map(newValue, offThreshold, hiThreshold, outLo, outHi);
      newValue = constrain(newValue, outLo, outHi);
      if (newValue != value){
        usbMIDI.sendNoteOn(value, 0, channel); // cuz we don't want TOTAL chaos
        usbMIDI.sendNoteOn(newValue, outHi, channel);
        waitTime = 127 - newValue; // not necessary. just adds flavor.
        value = newValue;
        timer = millis();
        waiting = true;
      }
    }
    else {
      returnme = -1;
    }
  }
  return returnme;
};


void MIDIcapSens::setNoteNumber(int num){ // Set the NOTE number.
  number = num;
};


void MIDIcapSens::setChannel(int ch){// Set MIDI channel for a single input.
  channel = ch;  
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


void MIDIcapSens::outputRange(int min, int max){
  outLo = min;
  outHi = max;
};