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

    // constructor when only pin & Channel number are given
    DMXbutton(int p, byte num);

    // " when pin, Channel number & mode are given
    DMXbutton(int p, byte num, byte mod);

    // " pin, Channel number, on & off values are given
    DMXbutton(int p, byte num, byte min, byte max);

    // " pin, Channel number, on/off values & mode are given
    DMXbutton(int p, byte num, byte min, byte max, byte mod);
 
    // destructor
    ~DMXbutton();

    Bounce* userButton;
    int read(); // returns 1 for fallingEdge, 0 for risingEdge or -1 if neither
    int send(); // calls read(), sends a DMX value & returns the Channel number
    byte number;
    byte outLo, outHi;
    byte mode;    
    bool state;
    void setChannelNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif
