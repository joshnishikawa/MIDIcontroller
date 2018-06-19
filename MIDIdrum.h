#ifndef MIDIdrum_h
#define MIDIdrum_h

#include "Arduino.h"

extern byte MIDIchannel;

class MIDIdrum{
    int pin;
    int newValue;
    int peak;
    int state;  // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
    unsigned int waitTime;
    elapsedMillis timer;
    
  public:
    // default constructor
    MIDIdrum();
    
    // constructor when pin and note number are given
    MIDIdrum(int p, byte num);

    // destructor
   	~MIDIdrum();

    int read();
    int send();
    int send(int vel);
    byte number;
    byte outLo, outHi;
    int threshold;
    void setNoteNumber(byte num);
    void outputRange(byte min, byte max);
    void setThreshold(int thresh);
};

#endif

