#ifndef MIDIcapSens_h
#define MIDIcapSens_h

#include "Arduino.h"

extern int MIDIchannel;

class MIDIcapSens{
    int pin;
    int channel;    
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

    int read();
    int chaos();
    int value;
    bool hovered;
    bool touched;
    void setNoteNumber(int num);
    void setChannel(int ch);	
    void outputRange(int min, int max);
    void setThresholds(int loT, int hiT);
    void setThresholds(int offT, int loT, int hiT);
};

#endif