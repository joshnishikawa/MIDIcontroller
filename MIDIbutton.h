#ifndef MIDIbutton_h
#define MIDIbutton_h

#include "Arduino.h"
#include "Bounce.h"
#include "Flicker.h"

#define MOMENTARY 0
#define LATCH 1
#define TRIGGER 2

extern byte MIDIchannel;

class MIDIbutton: public Bounce, public Flicker{
    bool inputType;
  public:
    // default constructor
    MIDIbutton();

    // constructor for a button
    MIDIbutton(int p, byte num, byte mod);

    // constructor for a capacitive sensor
    MIDIbutton(int p, byte num, byte mod, int thresh);

    // destructor
    ~MIDIbutton();

    int read(); // returns outHi for fallingEdge, outLo for risingEdge, else -1
    int send(); // calls read(), sends a MIDI value & returns the control number
    byte number;
    byte outLo = 0;
    byte outHi = 127;
    byte mode;
    bool inputState; // refers to the actual physical state of the input
    bool state;      // refers to the most recently sent MIDI message
                     // e.g. a button may be latched on without being held down
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif