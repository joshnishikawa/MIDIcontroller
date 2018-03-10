#ifndef MIDIcapSens_h
#define MIDIcapSens_h

#include "Arduino.h"

extern byte MIDIchannel;

class MIDIcapSens{
    int pin;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    bool hovered;
    bool touched;
    
  public:
    MIDIcapSens();
    MIDIcapSens(int p, byte num);
    MIDIcapSens(int p, byte num, byte min, byte max);
   	~MIDIcapSens();

    int read(); // returns 2:risingEdge, 1:fallingEdge, 0:outOfRange, -1:none
    int send(); // calls read(), sends a MIDI value & returns the value
    int value;
    int number;
    byte outLo, outHi;
    int upperLimit, onThreshold, offThreshold;
    bool afterRelease;
    bool state;
    void setNoteNumber(byte num);
    void outputRange(byte min, byte max);
    void setThresholds(int offT, int onT);
    void setThresholds(int offT, int onT, int upL);
};

#endif

