#ifndef MIDIdrum_h
#define MIDIdrum_h

#include "Arduino.h"
#include "Flicker.h"

extern byte MIDIchannel;

class MIDIdrum{
    int pin;
    int peak;
    int sensitivity;
    int state;  // 0 = idle, 1 = looking for peak, 2 = ignoring aftershock
    bool isOn;
    unsigned int waitTime;
    elapsedMillis timer;
    
  public:
    // default constructor
    MIDIdrum();
    
    // constructor with pin and note number only.
    MIDIdrum(int p, uint8_t num);

    // constructor with pin, note number and sensitivity.
    MIDIdrum(int p, uint8_t num, uint8_t sens);

    // destructor
   	~MIDIdrum();

    int read();
    int send();
    int send(int vel);
    uint8_t number;
    uint8_t outLo = 1;
    uint8_t outHi = 127;
    unsigned int inHi = 1023;
    unsigned int threshold, upperThreshold;
    void setNoteNumber(uint8_t num);
    void setThreshold(unsigned int thresh);
    void inputRange(uint16_t thresh, uint16_t max);
    void setSensitivity(uint8_t sens);
    void setWaitTime(unsigned int time);
    void outputRange(uint8_t min, uint8_t max);
};

#endif

