#ifndef DMXenc_h
#define DMXenc_h

#include "Arduino.h"
#include "Bounce.h"
#include "Encoder.h"

extern byte DMXchannel;

class DMXenc{
  public:
    // default constructor
    DMXenc();
    
    // constructor when only pins and control number are given
    DMXenc(int a, int b, byte num);
    
    // " when pins, control number, minimum and maximum outgoing DMX values set
    DMXenc(int a, int b, byte num, byte min, byte max);
    
    // destructor
    ~DMXenc();

    int read(); // read input and return a DMX value (or -1 if none)
    int send(); // calls read(), sends and returns a DMX value (or -1 if none)
    byte number;
    byte value;
    byte outLo, outHi;
    Encoder *myKnob;
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
};

#endif
