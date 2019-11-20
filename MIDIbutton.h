#ifndef MIDIbutton_h
#define MIDIbutton_h

#include "Arduino.h"
#include "Bounce.h"
#include "Flicker.h"

#define MOMENTARY 0
#define LATCH 1
#define TRIGGER 2

#define TOUCH 1

extern byte MIDIchannel;

class MIDIbutton: public Bounce, public TouchSwitch{
    byte inputType;
  public:
    // default constructor
    MIDIbutton();

    // constructor for a button
    MIDIbutton(int p, byte num, byte mode);

    // constructor for a capacitive sensor
    MIDIbutton(int p, byte num, byte mode, int type);

    // destructor
    ~MIDIbutton();

    // setThreshold() is only used for Capacitive Touch inputs. It assumes the
    // input is NOT being touched and automatically calculates a threshold
    // using a call to touchRead(). Use in setup().
    void setThreshold();

    int read(); // returns outHi for fallingEdge, outLo for risingEdge, else -1
    int send(); // calls read(), sends a MIDI value & returns the control number
    byte number;
    byte outLo = 0;
    byte outHi = 127;
    byte mode;
    byte inputState; // refers to the actual physical state of the input
    byte state; // refers to the most recently sent MIDI message
                // e.g. a button may be latched on without being held down
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif

