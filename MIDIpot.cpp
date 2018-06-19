#include "MIDIpot.h"

// constructors
MIDIpot::MIDIpot(){};

MIDIpot::MIDIpot(int p, byte num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = 0;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, byte num, bool m){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = m;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, byte num, byte min, byte max){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = 0;
  inLo = 0;
  inHi = 1023;
  outLo = min;
  outHi = max;
  invert = outHi < outLo;

  // Sets the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, byte num, bool m, byte min, byte max){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = m;
  inLo = 0;
  inHi = 1023;
  outLo = min;
  outHi = max;
  invert = outHi < outLo;

  // Sets the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

// destructor
MIDIpot::~MIDIpot(){
};


// returns new CC if there's enough change in the analog input; -1 otherwise
int MIDIpot::read(){
  int newValue = analogRead(pin);
  if (newValue >= inHi && value != outHi){ // Assign hi analog to hi MIDI
    value = outHi;
    newValue = value;
  }
  else if (newValue <= inLo && value != outLo){ // Assign low analog to low MIDI
    value = outLo;
    newValue = value;
  }
  else if (newValue % divider == 0){ // Filter intermittent values
    newValue = map(newValue, inLo, inHi, outLo, outHi);
    newValue = invert ? constrain(newValue, outHi, outLo)
                      : constrain(newValue, outLo, outHi);
    newValue = newValue == value ? -1 : newValue;
  }
  else{newValue = -1;}
  return newValue;
};

int MIDIpot::send(){
  int newValue = read();
  if (mode == true && newValue > outLo && value == outLo){  //ON before main msg
    usbMIDI.sendControlChange(number+1, 127, MIDIchannel);
  }
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, MIDIchannel);//MAIN MESSAGE
    if (mode == true && newValue == outLo && value >= outLo){//OFF after main
      usbMIDI.sendControlChange(number+1, 0, MIDIchannel);
    }
    value = newValue;
  }
  return newValue;
};


void MIDIpot::setControlNumber(byte num){ // Set the CC number.
  number = num;
};

// Limit the analog input to the usable range of a sensor.
// NOTE: Stability decreases as the difference between inHi and inLo decreases.
void MIDIpot::inputRange(uint16_t min, uint16_t max){
  inLo = min;
  inHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};


// Set upper and lower limits for outgoing MIDI messages.
void MIDIpot::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
  invert = outHi < outLo; // Check again for reverse polarity.
};

