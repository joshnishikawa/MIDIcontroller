#include "MIDIpot.h"

// constructors
MIDIpot::MIDIpot(){};

MIDIpot::MIDIpot(int p, int num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  kill = false;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, int num, bool kll){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  kill = kll;
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
  number = num;
  value = 0;
  kill = false;
  inLo = 0;
  inHi = 1023;
  outLo = min;
  outHi = max;
  invert = outHi < outLo;

  // Sets the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDIpot::MIDIpot(int p, int num, int min, int max, bool kll){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  kill = kll;
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
int MIDIpot::read(){
  int newValue = analogRead(pin);
  if (newValue >= inHi && value != outHi){ // Assign hi analog to hi MIDI
    newValue = outHi;
  }
  else if (newValue <= inLo && value != outLo){ // Assign low analog to low MIDI
    newValue = outLo;
  }
  else if (newValue % divider == 0){ // Filter intermittent values
    newValue = map(newValue, inLo, inHi, outLo, outHi);
    newValue = invert ? constrain(newValue, outHi, outLo)
                      : constrain(newValue, outLo, outHi);
    if (newValue == value){
      newValue = -1;
    }
  }
  else{newValue = -1;}
  return newValue;
};

int MIDIpot::send(){
  int newValue = read();
  if (kill == true && newValue > outLo && value == outLo){ // ON before main msg
    usbMIDI.sendControlChange(3, 127, *MC);
  }
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, *MC);    // MAIN MESSAGE
    if (kill == true && newValue == outLo && value > outLo){ //KILL aft main msg
      usbMIDI.sendControlChange(3, 0, *MC);
    }
    value = newValue;
  }
  return newValue;
};


void MIDIpot::setControlNumber(int num){ // Set the CC number.
  number = num;
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

