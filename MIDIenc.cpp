/*FIXME after the value bottoms out alternating MIDI messages will occasionally
  continue to be sent (0 1 0 1 0...). I think this is due to missing pulses
  because no interrupts are used. I'm still trying to figure this out.
*/

#include "MIDIenc.h"

// constructors
MIDIenc::MIDIenc(){};

MIDIenc::MIDIenc(int a, int b, int num){
  myKnob = new Encoder(a, b);
	number = num;
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, int num, int min, int max){
  myKnob = new Encoder(a, b);
	number = num;
  value = 0;
  outLo = min;
  outHi = max;
};

// destructor
MIDIenc::~MIDIenc(){
  delete myKnob;
};


int MIDIenc::read(){
  int newValue = -1;
  int incdec = myKnob->read(); // Using only +1 or -1
  myKnob->write(0);            // then resetting to 0
  if((incdec == 1 && value < outHi) || (incdec == -1 && value > outLo)){
    // If turned up but not already maxed OR down but not already bottomed out
    newValue = value + incdec;
  }
  else {newValue = -1;}
  return newValue;
};

int MIDIenc::send(){
  int newValue = read();
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, *MC);
    value = newValue;
  }
  return newValue;
}

// Set the CC number.
void MIDIenc::setControlNumber(int num){
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDIenc::outputRange(int min, int max){
  outLo = min;
  outHi = max;
};