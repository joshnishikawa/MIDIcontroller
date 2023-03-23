#include "MIDIdrum.h"

// constructors
MIDIdrum::MIDIdrum(){};

MIDIdrum::MIDIdrum(int p, uint8_t num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;

  threshold = 12;
  sensitivity = 10; // 90% sensitive by default
  upperThreshold = threshold + sensitivity;

  inHi = 1023;
  isOn = false;
  peak = 0;
  state = 0; //0= idle, 1= test velocity, 2= look for peak, 3= ignore aftershock
  waitTime = 0; // millis
  timer = 0;
};

MIDIdrum::MIDIdrum(int p, uint8_t num, uint8_t sens){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 0;
  outHi = 127;

  threshold = 10;
  sens = constrain(sens, 0, 100);
  sensitivity = 100 - sens;
  upperThreshold = threshold + sensitivity;

  sensitivity = 10; // 90% sensitive by default
  upperThreshold = threshold + sensitivity;

  inHi = 1023;
  isOn = false;
  peak = 0;
  state = 0; //0= idle, 1= test velocity, 2= look for peak, 3= ignore aftershock
  waitTime = 0; // millis
  timer = 0;
};

// destructor
MIDIdrum::~MIDIdrum(){
};


int MIDIdrum::read(){
  int newValue = analogRead(pin);

  switch (state){
    case 1:
      // test for velocity
      if (timer < 2 && newValue >= upperThreshold){
        peak = newValue;
        state = 2;
      }
      else if (timer >= 2){
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
        if (isOn){
          isOn = false;
          return 0;
        } else return -1;
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

void MIDIdrum::setNoteNumber(uint8_t num){ // Set the NOTE number.
  number = num;
};

void MIDIdrum::outputRange(uint8_t min, uint8_t max){ // Set min & max output values
  outLo = constrain(min, 0, 127);
  outHi = constrain(max, 0, 127);
};

// Limit the analog input to the usable range of a sensor.
void MIDIdrum::inputRange(uint16_t thresh, uint16_t max){
  threshold = constrain(thresh, 0, 1023);
  inHi = constrain(max, 0, 1023);
};

void MIDIdrum::setThreshold(unsigned int thresh){
  threshold = constrain(thresh, 0, 1023);
};

void MIDIdrum::setWaitTime(unsigned int time){
  waitTime = time;
};

void MIDIdrum::setSensitivity(uint8_t sens){
  // sensitivity(100) should be thought of as 100% sensitive meaning that notes
  // will always sound regardless of velocity. 0 would mean that there needs to
  // be enough velocity to reach an analog reading 100 above the threshold
  // within 2ms of the threshold crossing.
  sens = constrain(sens, 0, 100);
  sensitivity = 100 - sens;
  upperThreshold = threshold + sensitivity;
};