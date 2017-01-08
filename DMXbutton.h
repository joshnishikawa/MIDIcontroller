#ifndef DMXbutton_h
#define DMXbutton_h

#include "Arduino.h"
#include "Bounce.h"

extern byte DMXchannel;

class DMXbutton{
    byte* MC = &DMXchannel;

  public:
    // default constructor
    DMXbutton();

    // constructor when only pin & control number are given
    DMXbutton(int p, byte num);

    // " when pin, control number & mode are given
    DMXbutton(int p, byte num, byte mod);

    // " pin, control number, on & off values are given
    DMXbutton(int p, byte num, byte min, byte max);

    // " pin, control number, on/off values & mode are given
    DMXbutton(int p, byte num, byte min, byte max, byte mod);
 
    // destructor
    ~DMXbutton();

    Bounce* myButt;
    int read(); // returns 1 for fallingEdge, 0 for risingEdge or -1 if neither
    int send(); // calls read(), sends a DMX value & returns the control number
    byte number;
    byte outLo, outHi;
    byte mode;    
    bool state;
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif
