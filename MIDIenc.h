#ifndef MIDIenc_h
#define MIDIenc_h

#include "Arduino.h"
#include "Bounce.h"
#include "Encoder.h"

extern const int MIDIchannel;

class MIDIenc{
    Encoder *myKnob;
    int channel;
    int number;
    int outLo, outHi;

  public:
    MIDIenc();
    MIDIenc(int a, int b, int num);
    MIDIenc(int a, int b, int num, int min, int max);
    ~MIDIenc();

    void read();
    int value;
    void setControlNumber(int num);
    void setChannel(int ch);	
    void outputRange(int min, int max);
};

#endif