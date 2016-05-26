#ifndef MIDIenc_h
#define MIDIenc_h

#include "Arduino.h"
#include "Bounce.h"
#include "Encoder.h"

extern byte MIDIchannel;

class MIDIenc{
  public:
    // default constructor
    MIDIenc();
    
    // constructor when only pins and control number are given
    MIDIenc(int a, int b, byte num);
    
    // " when pins, control number, minimum and maximum outgoing MIDI values set
    MIDIenc(int a, int b, byte num, byte min, byte max);
    
    // destructor
    ~MIDIenc();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    byte number;
    byte value;
    byte outLo, outHi;
    Encoder *myKnob;
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
};

#endif