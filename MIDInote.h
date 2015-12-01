#ifndef MIDIkey_h
#define MIDIkey_h

#include "Arduino.h"

extern int MIDIchannel;

class MIDInote{
    int pin;
    int channel;    
    int number;
    bool velocity;
    bool polyPressure;
    int inLo, inHi, outLo, outHi;
    bool invert;
    int threshold;
    bool listening;
    int newValue;
    int loVal, hiVal;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    int divider;
    void readVelocity();
    void readPolyPressure();

  public:
    MIDInote();
    MIDInote(int p, int num);
    MIDInote(int p, int num, bool vel);
    MIDInote(int p, int num, bool vel, bool aft);
   	~MIDInote();

    void read();
    bool state;
    int value;
    void setNoteNumber(int num);
    void setChannel(int ch);	
    void inputRange(int min, int max);
    void outputRange(int min, int max);
    void setThreshold(int thresh);
};

#endif