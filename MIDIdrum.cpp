#include "MIDIdrum.h"

// constructors
MIDIdrum::MIDIdrum(){};

MIDIdrum::MIDIdrum(int p, byte num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;
  peak = 0;
  threshold = 12;
  state = 0;     // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
  waitTime = 36; // millis
  timer = 0;
};

// destructor
MIDIdrum::~MIDIdrum(){
};

int MIDIdrum::read(){
  int newValue = analogRead(pin);
  if (state == 0) {
    if (newValue > threshold) {
      state = 1;
      peak = newValue;
      timer = 0;
    }
    newValue = -1; //still just listening
  }
  else if (state == 1) {
    if (newValue > peak) {
      peak = newValue;
      newValue = -1;
    }
    else if (timer >= 10) {
      newValue = map(peak, threshold, 1023, outLo, outHi);
      state = 2;
      timer = 0;
    }
    else{newValue = -1;}
  }
  else {
    if (newValue > threshold) {
      timer = 0; // keep resetting timer if above threshold
    }
    else if (timer > waitTime) {
      state = 0; // go back to idle after a certain interval below threshold
      usbMIDI.sendNoteOn(number, 0, MIDIchannel);
    }
    newValue = -1;
  }
  return newValue;
};

int MIDIdrum::send(){
  int newValue = read();
  if (newValue >= 0){
    usbMIDI.sendNoteOn(number, newValue, MIDIchannel);
  }
  return newValue;
};

int MIDIdrum::send(int vel){
  int newValue = read();
  if (newValue >= 0){
    constrain(vel, 1, 127);
    usbMIDI.sendNoteOn(number, vel, MIDIchannel);
  }
  return newValue;
};

void MIDIdrum::setNoteNumber(byte num){ // Set the NOTE number.
  number = num;
};

void MIDIdrum::outputRange(byte min, byte max){ // Set min & max poly pressure
  outLo = min;
  outHi = max;
};

void MIDIdrum::setThreshold(int thresh){
  threshold = thresh;
};

