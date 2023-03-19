#ifndef MIDIenc_h
#define MIDIenc_h

#include "Arduino.h"
#include "Bounce2.h"
#include "Encoder.h"

#define PER_VALUE 1
#define PER_DETENT 4
#define PROGRAM_CHANGE 0xC0

extern byte MIDIchannel;

class MIDIenc{
  public:
    // default constructor
    MIDIenc();
    
    // constructor when only pins and control number are given
    MIDIenc(int a, int b, byte num);
    
    // " pins, control number and whether value changes PER_DETENT or PER_VALUE
    MIDIenc(int a, int b, byte num, byte detentOrValue);

    // destructor
    ~MIDIenc();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    int send(bool force); // forces MIDI output regardless of input
    byte number;
    byte value;
    byte outLo, outHi;
    byte detentOrValue;
    Encoder *myKnob;
    void write(byte val);
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
};

#endif