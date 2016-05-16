#ifndef MIDIbutton_h
#define MIDIbutton_h

#include "Arduino.h"
#include "Bounce.h"

extern int* MC;

class MIDIbutton{
    int outLo, outHi;
    int mode;    

  public:
    // default constructor
    MIDIbutton();

    // constructor when only pin & control number are given
    MIDIbutton(int p, int num);

    // " when pin, control number & mode are given
    MIDIbutton(int p, int num, int mod);

    // " pin, control number, on & off values are given
    MIDIbutton(int p, int num, int min, int max);

    // " pin, control number, on/off values & mode are given
    MIDIbutton(int p, int num, int min, int max, int mod);
 
    // destructor
    ~MIDIbutton();

    Bounce *myButt;
    int read(); // returns 1 for fallingEdge, 0 for risingEdge or -1 if neither
    int send(); // calls read(), sends a MIDI value & returns the control number
    int number;
    bool state;
    void setControlNumber(int num);
    void setMode(int mod);
    void outputRange(int min, int max);
};

#endif