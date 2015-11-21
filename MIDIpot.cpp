#include "MIDIpot.h"

// constructors
MIDIpot::MIDIpot(){};

MIDIpot::MIDIpot(int p, int num){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  newValue = 0;
  value = 0;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, int num, int min, int max){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  newValue = 0;
  value = 0;
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


// Sends CC only if there's a significant enough change in analog input
void MIDIpot::read(){
  newValue = analogRead(pin);
  if (newValue >= inHi){ // Explicitly assign hi analog to hi MIDI
    newValue = outHi;
  }
  else if (newValue <= inLo){ // Explicitly assign low analog to low MIDI
    newValue = outLo;
  }
  else if (newValue % divider == 0){ // Filter intermittent values
    newValue = map(newValue, inLo, inHi, outLo, outHi);
    newValue = invert ? constrain(newValue, outHi, outLo) : constrain(newValue, outLo, outHi);
  }
  else{
    newValue = -1;
  }

  if (newValue >= 0 && newValue != value){
    usbMIDI.sendControlChange(number, newValue, channel);
    value = newValue;
  }
};

void MIDIpot::setControlNumber(int num){ // Set the CC number.
  number = num;
};

void MIDIpot::setChannel(int ch){ // Set the MIDI channel
  channel = ch;
};

// Limit the analog input to the usable range of a sensor.
// NOTE: Stability decreases as the difference between inHi and inLo decreases.
void MIDIpot::inputRange(int min, int max){
  inLo = min;
  inHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};


// Set upper and lower limits for outgoing MIDI messages.
void MIDIpot::outputRange(int min, int max){
  outLo = min;
  outHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
  invert = outHi < outLo; // Check again for reverse polarity.
};

