/*FIXME after the value bottoms out alternating DMX messages will occasionally
  continue to be sent (0 1 0 1 0...). I think this is due to missing pulses
  because no interrupts are used. I'm still trying to figure this out.
*/

#include "DMXenc.h"

// constructors
DMXenc::DMXenc(){};

DMXenc::DMXenc(int a, int b, byte num){
  myKnob = new Encoder(a, b);
	number = num;
  value = 0;
  outLo = 0;
  outHi = 127;
};

DMXenc::DMXenc(int a, int b, byte num, byte min, byte max){
  myKnob = new Encoder(a, b);
	number = num;
  value = 0;
  outLo = min;
  outHi = max;
};

// destructor
DMXenc::~DMXenc(){
  delete myKnob;
};


int DMXenc::read(){
  int newValue = -1;
  int incdec = myKnob->read(); // Using only +1 or -1
  if((incdec >= 1 && value < outHi) || (incdec <= -1 && value > outLo)){
    // If turned up but not already maxed OR down but not already bottomed out
    myKnob->write(0);           // then reset to 0
    newValue = value + incdec;  // and return new value
  }
  else{newValue = -1;}
  return newValue;
};

int DMXenc::send(){
  int newValue = read();
  if (newValue >= 0){
    usbDMX.sendChannelChange(number, newValue, DMXchannel);
    value = newValue;
  }
  return newValue;
}

// Set the CC number.
void DMXenc::setChannelNumber(byte num){
  number = num;
};

// Set upper and lower limits for outgoing DMX messages.
void DMXenc::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
};
