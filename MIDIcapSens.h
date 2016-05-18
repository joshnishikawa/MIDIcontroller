#ifndef MIDIcapSens_h
#define MIDIcapSens_h

#include "Arduino.h"

extern int MIDIchannel;

class MIDIcapSens{
    int* MC = &MIDIchannel;
    int pin;
    int number;
    bool afterRelease;
    int outLo, outHi;
    int hiThreshold, loThreshold, offThreshold;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    
  public:
    MIDIcapSens();
    MIDIcapSens(int p, int num);
    MIDIcapSens(int p, int num, int min, int max);
   	~MIDIcapSens();

    int read(); // returns 2:risingEdge, 1:fallingEdge, 0:outOfRange, -1:none
    int send(); // calls read(), sends a MIDI value & returns the value
    int chaos();
    int value;
    bool hovered;
    bool touched;
    void setNoteNumber(int num);
    void outputRange(int min, int max);
    void setThresholds(int loT, int hiT);
    void setThresholds(int offT, int loT, int hiT);
};

#endif