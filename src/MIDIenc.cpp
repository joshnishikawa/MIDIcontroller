#include "MIDIenc.h"

// constructors
MIDIenc::MIDIenc(){};

MIDIenc::MIDIenc(int a, int b, byte num){
  myKnob = new Encoder(a, b);
	number = num;
  detentOrValue = 4; // CC changes once per detent
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte detentOrValue){
  myKnob = new Encoder(a, b);
	number = num;
  this->detentOrValue = detentOrValue; // CC changes per encoder value or detent
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte min, byte max){
  myKnob = new Encoder(a, b);
	number = num;
  detentOrValue = 4; // CC changes once per detent
  value = 0;
  outLo = min;
  outHi = max;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte min, byte max, byte detentOrValue){
  myKnob = new Encoder(a, b);
	number = num;
  this->detentOrValue = detentOrValue; // CC changes per encoder value or detent
  value = 0;
  outLo = min;
  outHi = max;
};

// destructor
MIDIenc::~MIDIenc(){
  delete myKnob;
};


int MIDIenc::read(){
  int incdec = myKnob->read();

  if (incdec >= detentOrValue){
    myKnob->write(0);
    if ( value < outHi ){
      value++;
      return value;
    }
    else{ return -1; }
  }
  else if (incdec <= -detentOrValue){
    myKnob->write(0);
    if ( value > outLo ){
      value--;
      return value;
    }
    else{ return -1; }
  }
  else{ return -1; }
};


int MIDIenc::send(){
  int newValue = read();
  if (newValue >= 0){
    usbMIDI.sendControlChange(number, newValue, MIDIchannel);
    value = newValue;
  }
  return newValue;
}


int MIDIenc::send(bool force){
  if (force){
    usbMIDI.sendControlChange(number, value, MIDIchannel);
    return value;
  }
  else{ return -1; }
}


// Manually set the value.
void MIDIenc::write(byte val){
  value = val;
};

// Set the CC number.
void MIDIenc::setControlNumber(byte num){
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDIenc::outputRange(byte min, byte max){
  outLo = min;
  outHi = max;
};