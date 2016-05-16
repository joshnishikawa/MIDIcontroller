#ifndef MIDInote_h
#define MIDInote_h

#include "Arduino.h"

extern int* MC;

class MIDInote{
    int pin;
    int number;
    int value;
    bool velocity;
    int inLo, inHi, outLo, outHi;
    bool invert;
    int threshold;
    int floor;
    bool listening;
    int newValue;
    int loVal, hiVal;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    int divider;
    
  public:
    MIDInote();
    MIDInote(int p, int num);
    MIDInote(int p, int num, bool vel);
   	~MIDInote();

    int read();
    int send();
    bool state;
    void setNoteNumber(int num);
    void inputRange(int min, int max);
    void outputRange(int min, int max);
    void setThreshold(int thresh);
};

#endif