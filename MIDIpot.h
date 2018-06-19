#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"
#define KILL 1

extern byte MIDIchannel;

class MIDIpot{
    int divider; // for converting from analog to MIDI resolution
    
  public:
    // default constructor
    MIDIpot();
    
    // constructor when only pin & control number are given
    MIDIpot(int p, byte num);
    
    // " when pin, control number are given and kill switch is enabled
    MIDIpot(int p, byte num, bool m);
    
    // " pin, control number, minimum & maximum outgoing MIDI values are set
    MIDIpot(int p, byte num, byte min, byte max);
    
    // " pin, control number, min/max values are given and kill switch is enabled
    MIDIpot(int p, byte num, bool m, byte min, byte max);
    
    // destructor
   	~MIDIpot();

    int pin;
    bool invert;
    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    byte number;
    uint16_t inLo, inHi;
    byte outLo, outHi;
    bool mode;  // in case you need to kill an effect entirely
    byte value;
    void setControlNumber(byte num);
    void inputRange(uint16_t min, uint16_t max);
    void outputRange(byte min, byte max);
};

#endif