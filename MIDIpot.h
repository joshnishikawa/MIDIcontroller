#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"

extern int MIDIchannel;

class MIDIpot{
    int* MC = &MIDIchannel;
    int pin;
    int number;
    bool kill;  // in case you need to kill an effect entirely
    int inLo, inHi, outLo, outHi;
    int divider; // for converting from analog to MIDI resolution
    bool invert;
    
  public:
    // default constructor
    MIDIpot();
    
    // constructor when only pin & control number are given
    MIDIpot(int p, int num);
    
    // " when pin, control number are given and kill switch is enabled
    MIDIpot(int p, int num, bool kll);
    
    // " pin, control number, minimum & maximum outgoing MIDI values are set
    MIDIpot(int p, int num, int min, int max);
    
    // " pin, control number, min/max values are given and kill switch is enabled
    MIDIpot(int p, int num, int min, int max, bool kll);
    
    // destructor
   	~MIDIpot();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    int value = 0;
    void setControlNumber(int num);
    void inputRange(int min, int max);
    void outputRange(int min, int max);
};

#endif