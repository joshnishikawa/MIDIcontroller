#include "MIDItouch.h"

// constructors
MIDItouch::MIDItouch() : TouchVariable(){};

MIDItouch::MIDItouch(int p, byte num) : TouchVariable(p, 0, 127){
  pin = p;
  inputRange(); // only works if creating objects during setup
  number = num;
  value = 0;
  mode = false; // no killSwitch
  invert = outLo > outHi;
};

MIDItouch::MIDItouch(int p, byte num, byte k) : TouchVariable(p, 0, 127){
  pin = p;
  inputRange(); // only works if creating objects during setup
  number = num;
  value = 0;
  mode = true;
  killSwitch = k;
  invert = outLo > outHi;
};


// destructor
MIDItouch::~MIDItouch(){
};


int MIDItouch::read(){
  int newValue = TouchVariable::read();
  if (newValue == outHi && value != outHi){
    value = outHi;
    return value;
  }
  else if (newValue == outLo && value != outLo){
    value = outLo;
    return value;
  }
  else if (newValue == value){
    return -1;
  }
  else{
    value = newValue;
    return value;
  }
};


int MIDItouch::send(){
  int newValue = read();
  if (mode && newValue > outLo && value == outLo){  //ON before main msg
    usbMIDI.sendControlChange(killSwitch, 127, MIDIchannel);
  }
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, MIDIchannel);//MAIN MESSAGE
    if (mode && newValue == outLo && value >= outLo){//OFF after main
      usbMIDI.sendControlChange(killSwitch, 0, MIDIchannel);
    }
    value = newValue;
  }
  return newValue;
};


int MIDItouch::send(bool force){
  int newValue = TouchVariable::read();
  if (force){
    usbMIDI.sendControlChange(number, newValue, MIDIchannel);
  }
  return newValue;
}


void MIDItouch::setControlNumber(byte num){ // Set the NOTE number.
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDItouch::outputRange(byte min, byte max){
  outLo = min; // inherited from TouchVariable
  outHi = max; // inherited from TouchVariable
};


void MIDItouch::inputRange(){
  TouchVariable::setInputRange();
};


void MIDItouch::inputRange(uint16_t min, uint16_t max){
  TouchVariable::setInputRange(min, max);
};

void MIDItouch::setKillSwitch(byte k){
  if(k == 0){
    mode = false;
  }
  else{
    mode = true;
    killSwitch = constrain(k, 1, 127);
  }
};