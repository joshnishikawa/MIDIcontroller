#include "MIDIdrum.h"

// constructors
MIDIdrum::MIDIdrum(){};

MIDIdrum::MIDIdrum(int p, byte num): TouchVelocity(){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;
  inputType = 0; // FSR or Piezo
  inHi = 1023;
  peak = 0;
  threshold = 12;
  state = 0;     // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
  waitTime = 36; // millis
  timer = 0;
};

MIDIdrum::MIDIdrum(int p, byte num, byte type): TouchVelocity(p, 1, 127){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;
  inputType = type; // 1 = Capacitive Touch, 0 = FSR or Piezo
  if (inputType == 0){
    inHi = 1023;
    peak = 0;
    threshold = 12;
    state = 0;     // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
    waitTime = 36; // millis
    timer = 0;
  }
  else{
    setThreshold(); // only works if creating objects during setup
  }
};

// destructor
MIDIdrum::~MIDIdrum(){
};


int MIDIdrum::read(){
  int newValue;
  if (inputType != 1){ // Handle FSR or Piezo
    newValue = analogRead(pin);
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
      else if (timer >= 10){
        newValue = newValue >= inHi ? outHi : map(peak,threshold,inHi,outLo,outHi);
        state = 2;
        timer = 0;
      }
      else{newValue = -1;}
    }
    else {
      if (newValue > threshold) {
        timer = 0; // keep resetting timer if above threshold
        newValue = -1;
      }
      else if (timer > waitTime) {
        state = 0; // go back to idle after a certain interval below threshold
        newValue = 0;
      }
      else{newValue = -1;}
    }
  }
#if ! defined(__IMXRT1062__)
  else { // Handle Capacitive Touch
    newValue = TouchVelocity::responsiveRead();
    if (TouchVelocity::fallingEdge()){
      newValue = 0;
    }
    else if (newValue == 0){
      newValue = -1;
    }
    else{
      // Leave the reading from TouchVelocity::responsiveRead() as is.
    }
  }
#endif
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

void MIDIdrum::outputRange(byte min, byte max){ // Set min & max output values
  if (inputType == 1){
    TouchVelocity::setOutputRange(min, max);
  }
  else{
    outLo = min;
    outHi = max;
  }
};

// Limit the analog input to the usable range of a sensor.
void MIDIdrum::inputRange(uint16_t thresh, uint16_t max){
  threshold = thresh;
  inHi = max;
};

void MIDIdrum::setThreshold(){
  if (inputType == 0){
    // Do nothing if someone accidentally omits the argument for a Piezo or FSR
  }
  else{
    TouchVelocity::setThreshold();
  }
};

void MIDIdrum::setThreshold(unsigned int thresh){
  if (inputType == 1){
    // Do nothing if someone adds a specific threshold as an argument for a
    // Touch input. (This is not going to go the way that you think.)
  }
  else{threshold = thresh;}
};

void MIDIdrum::setWaitTime(unsigned int time){
  waitTime = time;
};
