#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"

extern const int MIDIchannel;

class MIDIpot{
    int pin;
    int channel;    
    int number;
    int inLo, inHi, outLo, outHi;
    int divider;
    int newValue;
    bool invert;

  public:
    MIDIpot();
    MIDIpot(int p, int num);
    MIDIpot(int p, int num, int min, int max);
   	~MIDIpot();

    void read();
    int value;
    void setControlNumber(int num);
    void setChannel(int ch);	
    void inputRange(int min, int max);
    void outputRange(int min, int max);
};

#endif