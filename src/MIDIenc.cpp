#include "MIDIenc.h"

// constructors
MIDIenc::MIDIenc(){
  myKnob = NULL;
};

MIDIenc::MIDIenc(int a, int b, byte num){
  myKnob = new Encoder(a, b);
  number = num;
  numberS = num;
  detentOrValue = PER_DETENT; // CC changes once per detent
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte detentOrValue){
  myKnob = new Encoder(a, b);
  number = num;
  numberS = num;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_DETENT; // CC changes per encoder value or detent
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte numS, byte detentOrValue){
  myKnob = new Encoder(a, b);
  number = num;
  numberS = numS;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_DETENT; // CC changes per encoder value or detent
  value = 0;
  outLo = 0;
  outHi = 127;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte min, byte max, byte detentOrValue){
  myKnob = new Encoder(a, b);
  number = num;
  numberS = num;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_DETENT; // CC changes per encoder value or detent
  value = min;
  outLo = min;
  outHi = max;
};

MIDIenc::MIDIenc(int a, int b, byte num, byte numS, byte min, byte max, byte detentOrValue){
  myKnob = new Encoder(a, b);
  number = num;
  numberS = numS;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_DETENT; // CC changes per encoder value or detent
  value = min;
  outLo = min;
  outHi = max;
};

// destructor
MIDIenc::~MIDIenc(){
  if (myKnob != NULL) {
    delete myKnob;
    myKnob = NULL;
  }
};

int MIDIenc::read(){
  if (myKnob == NULL) return -1;
  int incdec = myKnob->read();

  if (incdec >= detentOrValue){
    myKnob->write(0);
    if (value < outHi){
      value++;
      return value;
    }
    else{ return -1; }
  }
  else if (incdec <= -detentOrValue){
    myKnob->write(0);
    if (value > outLo){
      value--;
      return value;
    }
    else{ return -1; }
  }
  else{ return -1; }
};

int MIDIenc::send(int shiftState){
  int newValue = read();
  if (newValue >= 0){
    value = newValue;
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    uint8_t ccNum = (shiftState == 1 && numberS != 0) ? numberS : number;

    if (ccNum == PROGRAM_CHANGE){
      MIDI_send(MIDI_PROGRAM_CHANGE, value, 0, targetChan, cable, interface);
    }
    else{
      MIDI_send(MIDI_CONTROL_CHANGE, ccNum, newValue, targetChan, cable, interface);
    }
  }
  return newValue;
}

int MIDIenc::send(bool force){
  if (force){
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    if (number == PROGRAM_CHANGE){
      MIDI_send(MIDI_PROGRAM_CHANGE, value, 0, targetChan, cable, interface);
    }
    else{
      MIDI_send(MIDI_CONTROL_CHANGE, number, value, targetChan, cable, interface);
    }
    return value;
  }
  else{ return -1; }
}

void MIDIenc::setChannel(byte chan, byte cable, byte iface){
  this->channel = chan;
  this->cable = cable;
  this->interface = iface;
}

// Manually set the value.
void MIDIenc::write(byte val){
  value = constrain(val, outLo, outHi);
};

// Set the CC number.
void MIDIenc::setControlNumber(byte num){
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDIenc::outputRange(byte min, byte max){
  outLo = constrain(min, 0, 127);
  outHi = constrain(max, 0, 127);
};