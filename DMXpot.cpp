#include "DMXpot.h"

// constructors
DMXpot::DMXpot(){};

DMXpot::DMXpot(int p, byte num){
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

DMXpot::DMXpot(int p, byte num, byte m){
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

DMXpot::DMXpot(int p, byte num, byte min, byte max){
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

DMXpot::DMXpot(int p, byte num, byte min, byte max, byte m){
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
DMXpot::~DMXpot(){
};


// returns new CC if there's enough change in the analog input; -1 otherwise
int DMXpot::read(){
  int newValue = analogRead(pin);
  if (newValue >= inHi && value != outHi){ // Assign hi analog to hi DMX
    newValue = outHi;
  }
  else if (newValue <= inLo && value != outLo){ // Assign low analog to low DMX
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

int DMXpot::send(){
  int newValue = read();
  if (mode == true && newValue > outLo && value == outLo){ // ON before main msg
    usbMIDI.sendControlChange(12, 127, DMXchannel);
  }
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, DMXchannel);//MAIN MESSAGE
    if (mode == true && newValue == outLo && value > outLo){ //mode aft main msg
      usbMIDI.sendControlChange(12, 0, DMXchannel);
    }
    value = newValue;
  }
  return newValue;
};


void DMXpot::setControlNumber(byte num){ // Set the CC number.
  number = num;
};

// Limit the analog input to the usable range of a sensor.
// NOTE: Stability decreases as the difference between inHi and inLo decreases.
void DMXpot::inputRange(uint16_t min, uint16_t max){
  inLo = min;
  inHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};


// Set upper and lower limits for outgoing DMX messages.
void DMXpot::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
  invert = outHi < outLo; // Check again for reverse polarity.
};

