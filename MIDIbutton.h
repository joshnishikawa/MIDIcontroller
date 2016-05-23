#ifndef MIDIbutton_h
#define MIDIbutton_h

#include "Arduino.h"
#include "Bounce.h"

extern byte MIDIchannel;

class MIDIbutton{
    byte* MC = &MIDIchannel;

  public:
    // default constructor
    MIDIbutton();

    // constructor when only pin & control number are given
    MIDIbutton(int p, byte num);

    // " when pin, control number & mode are given
    MIDIbutton(int p, byte num, byte mod);

    // " pin, control number, on & off values are given
    MIDIbutton(int p, byte num, byte min, byte max);

    // " pin, control number, on/off values & mode are given
    MIDIbutton(int p, byte num, byte min, byte max, byte mod);
 
    // destructor
    ~MIDIbutton();

    Bounce *myButt;
    int read(); // returns 1 for fallingEdge, 0 for risingEdge or -1 if neither
    int send(); // calls read(), sends a MIDI value & returns the control number
    byte number;
    byte outLo, outHi;
    byte mode;    
    bool state;
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif