#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"

#define KILL 9 // previously undefined CC# safe for general purpose assignment
#define OFF 0

extern byte MIDIchannel;

class MIDIpot{
    int pin;
    byte invert;
    int divider; // for converting from analog to MIDI resolution
    
  public:
    // default constructor
    MIDIpot();
    
    // constructor when only pin & control number are given
    MIDIpot(int p, byte num);
    
    // " when pin, control number are given and kill switch is enabled
    MIDIpot(int p, byte num, byte k);
    
    // " pin, control number, minimum & maximum outgoing MIDI values are set
    MIDIpot(int p, byte num, byte min, byte max);
    
    // " pin, control number, kill switch enabled and min/max values are given
    MIDIpot(int p, byte num, byte k, byte min, byte max);
    
    // destructor
   	~MIDIpot();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    byte number;
    uint16_t inLo = 0;
    uint16_t inHi = 1023;
    byte outLo = 0;
    byte outHi = 127;
    byte mode; // In case you need to kill an effect entirely
    byte killSwitch = 0; // Which CC is getting its hands dirty?
    byte value;
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
    void inputRange(uint16_t min, uint16_t max);
    void setKillSwitch(byte k);
    int smooth(int value, int NR); // smoothes the values of analogRead()
};

#endif