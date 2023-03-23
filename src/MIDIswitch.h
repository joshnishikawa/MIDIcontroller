#ifndef MIDIswitch_h
#define MIDIswitch_h

#include "Arduino.h"
#include "Bounce2.h"
#include "Flicker.h"

#define MOMENTARY 0
#define LATCH 1
#define TRIGGER 2

#define BINARY 3
#define TOUCH 4

#define START 0xFA
#define STOP 0xFC
#define CONTINUE 0xFB
#define CLOCK 0xF8
#define SYSTEM_RESET 0xFF

extern byte MIDIchannel;

class MIDIswitch: public Bounce, public TouchSwitch{
    uint8_t inputType = BINARY; // Bounce object by default
    bool realTime = false;
  public:
    // default constructor
    MIDIswitch();

    // constructor for a switch with the default mode of MONENTARY
    MIDIswitch(int p, uint8_t num);

    // 'x' could be BINARY, TOUCH, MONENTARY, LATCH or TRIGGER
    MIDIswitch(int p, uint8_t num, uint8_t x);

    // constructor for specifying mode (MOMENTARY, LATCH, TRIGGER)
    // and type (BINARY, TOUCH)
    MIDIswitch(int p, uint8_t num, uint8_t m, uint8_t t);

    // destructor
    ~MIDIswitch();

    // setThreshold() is only used for Capacitive Touch inputs. It assumes the
    // input is NOT being touched and automatically calculates a threshold
    // using a call to touchRead(). Use in setup().
    void setThreshold();
    void setThreshold(int threshold);

    int read(); // return outHi for falling edge, outLo for rising edge, else -1
    int send(); // calls read(), sends a MIDI value & returns the control number
    int send(bool force);   // forces MIDI output regardless of input
    void write(bool s);     // sets the state of a LATCH input
    uint8_t number = 0;     // redefined on instatiation
    uint8_t outLo = 0;
    uint8_t outHi = 127;
    uint8_t mode = MOMENTARY; // momentary by default
    uint8_t inputState = 0; // refers to the actual physical state of the input
    bool state = false;     // refers to the most recently sent MIDI message
                            //e.g. a switch may be latched on when not held down
    void setControlNumber(byte num);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

#endif
