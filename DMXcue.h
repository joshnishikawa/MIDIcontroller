#ifndef DMXcue_h
#define DMXcue_h

#include "Arduino.h"

extern byte DMXchannel;

class DMXcue{
    int pin;
    int value;
    bool invert;
    bool listening;
    int newValue;
    int loVal, hiVal;
    bool waiting;
    unsigned int waitTime;
    unsigned long int timer;
    int divider;
    
  public:
    // default constructor
    DMXcue();
    
    // constructor when only pin and note number are given
    DMXcue(int p, byte num);

    // constructor when pin, note number and velocity sensitivity are specified
    DMXcue(int p, byte num, bool vel);

    // destructor
   	~DMXcue();

    int read();
    int send();
    bool state;
    byte number;
    bool velocity;
    int inLo, inHi;
    byte outLo, outHi;
    int afterTouchValue;
    int threshold;
    int floor;
    void setNoteNumber(byte num);
    void inputRange(int min, int max);
    void outputRange(byte min, byte max);
    void setThreshold(int thresh);
};

#endif
