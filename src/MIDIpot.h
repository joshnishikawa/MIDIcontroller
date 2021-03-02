#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"

#define SMOOTHING 50 // Can be increased at the cost of some responsiveness
#define KILL 9 // previously undefined CC# safe for general purpose assignment
#define OFF 0

extern byte MIDIchannel;

class MIDIpot{
    int pin = 0;
    byte invert = false;
    int divider = 8; // for converting from analog to MIDI resolution
    int buffer = 0; // a container used by the smooth() function 
    int difference = 0; // smooth() adds this to buffer 'til there's enough bias
    int balancedValue = 0; // this remains stable until signal bias changes it
    
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
    int inLo = 0;
    int inHi = 1023;
    int outLo = 0;
    int outHi = 127;
    byte number = 0;
    byte value = 0;
    byte mode = 0; // In case you need to kill an effect entirely
    byte killSwitch = 0; // Which CC is getting its hands dirty?
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
    void inputRange(uint16_t min, uint16_t max);
    void setKillSwitch(byte k);
    int smooth(int value, int NR); // smoothes the values of analogRead()
};

#endif