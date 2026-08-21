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
  uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;

  if (mode && newValue > outLo && value == outLo){ // ON before main msg
    MIDI_send(MIDI_CONTROL_CHANGE, killSwitch, 127, targetChan, cable, interface);
  }
  if (newValue >= 0){
    MIDI_send(MIDI_CONTROL_CHANGE, number, newValue, targetChan, cable, interface); // MAIN MESSAGE
    if (mode && newValue == outLo && value >= outLo){ // OFF after main
      MIDI_send(MIDI_CONTROL_CHANGE, killSwitch, 0, targetChan, cable, interface);
    }
    value = newValue;
  }
  return newValue;
};

int MIDItouch::send(bool force){
  int newValue = TouchVariable::read();
  if (force){
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    MIDI_send(MIDI_CONTROL_CHANGE, number, newValue, targetChan, cable, interface);
  }
  return newValue;
}

void MIDItouch::setChannel(byte chan, byte cable, byte iface){
  this->channel = chan;
  this->cable = cable;
  this->interface = iface;
}

void MIDItouch::setControlNumber(byte num){ // Set the NOTE number.
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDItouch::outputRange(byte min, byte max){
  outLo = min; // inherited from TouchVariable
  outHi = max; // inherited from TouchVariable
};

// Use inputRange() with no arguments during setup()
// To calculate the range automatically using a call to touchRead()
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