/*FIXME after the value bottoms out alternating MIDI messages will occasionally
  continue to be sent (0 1 0 1 0...). I think this is due to missing pulses
  because no interrupts are used. I'm still trying to figure this out.
*/

#include "MIDIenc.h"

// constructors
MIDIenc::MIDIenc(){};

MIDIenc::MIDIenc(int a, int b, int num){
  myKnob = new Encoder(a, b);
	channel = MIDIchannel;
	number = num;
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, int num, int min, int max){
  myKnob = new Encoder(a, b);
	channel = MIDIchannel;
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
  int returnme = -1;
  int incdec = myKnob->read(); // Using only +1 or -1
  myKnob->write(0);            // then resetting to 0
  // allows one MIDIencoder to set values for any number of objects.

  if((incdec == 1 && value < outHi) || (incdec == -1 && value > outLo)){
    // If turned up but not already maxed OR down but not already bottomed out
    value += incdec;
    usbMIDI.sendControlChange(number, value, channel);
    returnme = value;
  }
  return returnme;
};


// Set the CC number.
void MIDIenc::setControlNumber(int num){
  number = num;
};

// Set a specific MIDI channel for a single enc.
void MIDIenc::setChannel(int ch){
  channel = ch;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDIenc::outputRange(int min, int max){
  outLo = min;
  outHi = max;
};