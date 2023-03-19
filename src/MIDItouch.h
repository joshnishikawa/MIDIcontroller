#ifndef MIDItouch_h
#define MIDItouch_h

#include "Arduino.h"
#include "Flicker.h"

#define KILL 9 // previously undefined CC# safe for general purpose assignment
#define OFF 0

extern byte MIDIchannel;

class MIDItouch: public TouchVariable{
    int pin;
    byte invert;
    int divider; // for converting from the touchRead range to MIDI (0~127)
    
  public:
    // default constructor
    MIDItouch();
    
    // constructor when only pin & control number are given
    MIDItouch(int p, byte num);
    
    // " when pin, control number are given and kill switch is enabled
    MIDItouch(int p, byte num, byte k);
    
    // deconstructor
   	~MIDItouch();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends and returns a MIDI value (or -1 if none)
    int send(bool force); // forces MIDI output regardless of input
    byte number;
    byte mode; // In case you need to kill an effect entirely
    byte killSwitch = 0; // Which CC is getting its hands dirty?
    byte value;
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
    
    // Use inputRange() with no arguments during setup()
    // To calculate the range automatically using a call to touchRead()
    void inputRange();
    
    // Or a specific range can be given.
    void inputRange(uint16_t min, uint16_t max);
    void setKillSwitch(byte k);
};

#endif
