#ifndef MIDIdrum_h
#define MIDIdrum_h

#include "Arduino.h"
#include "Flicker.h"

extern byte MIDIchannel;

class MIDIdrum{
    int pin;
    int newValue;
    int peak;
    float sens;
    int state;  // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
    bool isOn;
    unsigned int waitTime;
    elapsedMillis timer;
    
  public:
    // default constructor
    MIDIdrum();
    
    // constructor with pin and note number for a piezo or FSR
    MIDIdrum(int p, byte num);

    // destructor
   	~MIDIdrum();

    int read();
    int send();
    int send(int vel);
    byte number;
    byte outLo = 1;
    byte outHi = 127;
    unsigned int inHi = 1023;
    unsigned int threshold;
    void setNoteNumber(byte num);
    void outputRange(byte min, byte max);
    void setThreshold(unsigned int thresh);
    void setWaitTime(unsigned int time);
    void inputRange(uint16_t thresh, uint16_t max);
    void sensitivity(uint8_t sens);
};

#endif

