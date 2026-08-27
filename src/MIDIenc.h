#ifndef MIDIenc_h
#define MIDIenc_h

#include "Arduino.h"
#include "Encoder.h"
#include "MIDItransport.h"

#define PER_VALUE 1
#define PER_DETENT 4
#define PROGRAM_CHANGE 0xC0

extern byte MIDIchannel;

class MIDIenc{
  public:
    // default constructor
    MIDIenc();
    
    // constructor when only pins and control number are given
    MIDIenc(int a, int b, byte num);
    
    // " pins, control number and whether value changes PER_DETENT or PER_VALUE
    MIDIenc(int a, int b, byte num, byte detentOrValue);

    // " pins, unshifted CC and shifted CC number
    MIDIenc(int a, int b, byte num, byte numS, byte detentOrValue);

    // " pins, CC, min, max, detent
    MIDIenc(int a, int b, byte num, byte min, byte max, byte detentOrValue);

    // " pins, CC, shifted CC, min, max, detent
    MIDIenc(int a, int b, byte num, byte numS, byte min, byte max, byte detentOrValue);

    // destructor
    ~MIDIenc();

    int read(); // read input and return a MIDI value (or -1 if none)
    int send(int shiftState = 0); // calls read(), sends and returns a MIDI value (or -1 if none)
    int send(bool force); // forces MIDI output regardless of input
    void setChannel(byte chan, byte cable = 0, byte iface = MIDI_INTERFACE_USB);

    byte number = 0;
    byte numberS = 0;
    byte value = 0;
    byte outLo = 0;
    byte outHi = 127;
    byte detentOrValue = PER_DETENT;
    byte channel = 0; // 0 = global MIDIchannel
    byte cable = 0;
    byte interface = MIDI_INTERFACE_USB;

    int pinA = -1;
    int pinB = -1;
    Encoder *myKnob = NULL;
    void allocateKnob();
    void write(byte val);
    void setControlNumber(byte num);
    void outputRange(byte min, byte max);
};

#endif