#ifndef MIDIswitch_h
#define MIDIswitch_h

#include "Arduino.h"
#include "Bounce2.h"
#include "Flicker.h"

#define MOMENTARY 0
#define LATCH 1
#define TRIGGER 2

#define TOUCH 1

extern byte MIDIchannel;

class MIDIswitch: public Bounce, public TouchSwitch{
    byte inputType = 0; // Bounce object by default
  public:
    // default constructor
    MIDIswitch();

    // constructor for a switch with the default mode of MONENTARY
    MIDIswitch(int p, byte num);

    // constructor for a switch with MONENTARY, LATCH or TRIGGER specified
    MIDIswitch(int p, byte num, byte mode);

    // constructor for a capacitive sensor
    MIDIswitch(int p, byte num, byte mode, int type);

    // destructor
    ~MIDIswitch();

    // setThreshold() is only used for Capacitive Touch inputs. It assumes the
    // input is NOT being touched and automatically calculates a threshold
    // using a call to touchRead(). Use in setup().
    void setThreshold();
    void setThreshold(int threshold);

    int read(); // return outHi for falling edge, outLo for rising edge, else -1
    int send(); // calls read(), sends a MIDI value & returns the control number
    int send(bool force); // forces MIDI output regardless of input
    byte number = 0;     // redefined on instatiation
    byte outLo = 0;
    byte outHi = 127;
    byte mode = 0;       // momentary by default
    byte inputState = 0; // refers to the actual physical state of the input
    byte state = 0; // refers to the most recently sent MIDI message
                    // e.g. a switch may be latched on without being held down
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};


// MIDIbutton() is deprecated. Please use MIDIswitch() because 'switch' is a 
// more inclusive term for the various types of binary inputs.
// MIDIbutton() only continues to be included here for backward compatibility.
class MIDIbutton: public MIDIswitch{
  public:
  MIDIbutton() : MIDIswitch(){}
  MIDIbutton(int p, byte num) : MIDIswitch(p, num){};
  MIDIbutton(int p, byte num, byte mode) : MIDIswitch(p, num, mode){};
  MIDIbutton(int p, byte num, byte mode, int type) : MIDIswitch(p, num, mode, type){};
};


#endif

