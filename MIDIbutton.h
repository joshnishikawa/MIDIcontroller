#ifndef MIDIbutton_h
#define MIDIbutton_h

#include "Arduino.h"
#include "Bounce.h"

extern const int MIDIchannel;

class MIDIbutton{
    int channel;
    int number;
    int outLo, outHi;
    int mode;    

  public:
    // default constructor
    MIDIbutton();

    // constructor when only pin & message number are given
    MIDIbutton(int p, int num);

    // " when pin, message number & mode are given
    MIDIbutton(int p, int num, int mod);

    // " pin, message number, velocity & off value are given
    MIDIbutton(int p, int num, int min, int max);

    // " pin, message number, velocity, off value & mode are given
    MIDIbutton(int p, int num, int min, int max, int mod);
 
    // destructor
    ~MIDIbutton();

    Bounce *myButt;
    void read();
    bool state;
    void setControlNumber(int num);
    void setChannel(int ch);
    void setMode(int mod);
    void outputRange(int min, int max);
};

#endif