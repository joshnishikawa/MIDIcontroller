#include "MIDIdrum.h"

// constructors
MIDIdrum::MIDIdrum(){};

MIDIdrum::MIDIdrum(int p, byte num): TouchVelocity(){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  inputType = 0; // FSR or Piezo
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

MIDIdrum::MIDIdrum(int p, byte num, byte type): TouchVelocity(p, 1, 127){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  outLo = 1;
  outHi = 127;
  inputType = type; // 1 = Capacitive Touch, 0 = FSR or Piezo
  if (inputType == 0){
    threshold = 12;
    inHi = 1023;
    sens = 1.0;
    peak = 0;
    isOn = false;
    state = 0; //0= idle, 1= test velocity, 2= look for peak, 3= ignore aftershock
    waitTime = 3; // millis
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

  if (inputType == 0){ // Handle FSR or Piezo
    newValue = analogRead(pin);
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
        newValue = -1;
        break;

      case 2:
        // look for peak
        if (newValue > peak) {
          peak = newValue;
          newValue = -1;
        }
        else if (timer >= 10){
          newValue = constrain(peak, upperThreshold, inHi);
          newValue = newValue >= inHi ? outHi : map(peak,upperThreshold,inHi,outLo,outHi);
          isOn = true;
          peak = 0;
          state = 3;
          timer = 0;
        }
        else{newValue = -1;}
        break;

      case 3:
        if (newValue > threshold) {
          timer = 0; // keep resetting timer if above threshold
          newValue = -1;
        }
        else if (timer > waitTime) {
          state = 0; // go back to idle after a certain interval below threshold
          newValue = isOn ? 0 : -1;
          isOn = false;
        }
        else{newValue = -1;}
        break;

      default:
        // idle: search for threshold crossing
        if (newValue >= threshold) {
          state = 1;
          timer = 0;
        }
        newValue = -1; //still just listening
        break;
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

void MIDIdrum::sensitivity(uint8_t s){
  if (inputType == 1){
    // FIXME: Figure out how to set sensitivity for touch inputs.
  }
  else{
    s = constrain(s, 1, 100);
    sens = float(s) / 100.0;
  }
};