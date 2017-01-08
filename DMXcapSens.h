#ifndef DMXcapSens_h
#define DMXcapSens_h

#include "Arduino.h"

extern byte DMXchannel;

class DMXcapSens{
    int pin;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    bool hovered;
    bool touched;
    
  public:
    DMXcapSens();
    DMXcapSens(int p, byte num);
    DMXcapSens(int p, byte num, byte min, byte max);
   	~DMXcapSens();

    int read(); // returns 2:risingEdge, 1:fallingEdge, 0:outOfRange, -1:none
    int send(); // calls read(), sends a DMX value & returns the value
    int value;
    int number;
    bool afterRelease;
    byte outLo, outHi;
    int hiThreshold, loThreshold, offThreshold;
    void setNoteNumber(byte num);
    void outputRange(byte min, byte max);
    void setThresholds(int loT, int hiT);
    void setThresholds(int offT, int loT, int hiT);
};

#endif
