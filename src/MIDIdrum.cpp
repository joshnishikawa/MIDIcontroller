#include "MIDIdrum.h"

// constructors
MIDIdrum::MIDIdrum(){};

MIDIdrum::MIDIdrum(int p, byte num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;
  threshold = 12;
  inHi = 1023;
  sens = 1.0;
  isOn = false;
  peak = 0;
  state = 0; //0= idle, 1= test velocity, 2= look for peak, 3= ignore aftershock
  waitTime = 3; // millis
  timer = 0;
};

// destructor
MIDIdrum::~MIDIdrum(){
};


int MIDIdrum::read(){
  int newValue = analogRead(pin);
  int range = inHi - threshold;
  int upperThreshold = inHi - ( range * sens );

  switch (state){
    case 1:
      // test for velocity
      if (timer < 1 && newValue >= upperThreshold){
        peak = newValue > peak ? newValue : peak;
        state = 2;
      }
      else if (timer >= 1){
        state = 3;
      }
      return -1;
      break;

    case 2:
      // look for peak
      if (newValue > peak) {
        peak = newValue;
        return -1;
      }
      else if (timer >= 10){
        newValue = constrain(peak, upperThreshold, inHi);
        newValue = newValue >= inHi ? outHi : map(peak,upperThreshold,inHi,outLo,outHi);
        isOn = true;
        peak = 0;
        state = 3;
        timer = 0;
        return newValue;
      } 
      else { 
        return -1; 
      }
      break;

    case 3:
      if (newValue > threshold) {
        timer = 0; // keep resetting timer if above threshold
        return -1;
      }
      else if (timer > waitTime) {
        state = 0; // go back to idle after a certain interval below threshold
        newValue = isOn ? 0 : -1;
        isOn = false;
        return newValue;
      }
      else{
        return -1;
      }
      break;

    default:
      // idle: search for threshold crossing
      if (newValue >= threshold) {
        state = 1;
        timer = 0;
      }
      return -1; //still just listening
      break;
  }
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

void MIDIdrum::outputRange(byte min, byte max){ // Set min & max output values
  outLo = min;
  outHi = max;
};

// Limit the analog input to the usable range of a sensor.
void MIDIdrum::inputRange(uint16_t thresh, uint16_t max){
  threshold = thresh;
  inHi = max;
};


void MIDIdrum::setThreshold(unsigned int thresh){
  threshold = thresh;
};

void MIDIdrum::setWaitTime(unsigned int time){
  waitTime = time;
};

void MIDIdrum::sensitivity(uint8_t s){
  s = constrain(s, 1, 100);
  sens = float(s) / 100.0;
};