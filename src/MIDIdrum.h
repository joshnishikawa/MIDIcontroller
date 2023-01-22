#ifndef MIDIdrum_h
#define MIDIdrum_h

#include "Arduino.h"
#include "Flicker.h"

#define FSR 0
#define PIEZO 0 // Currently FSRs and Piezos are handled exactly the same.
#define TOUCH 1

extern byte MIDIchannel;

class MIDIdrum: public TouchVelocity{
    int pin;
    byte inputType;
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

    // constructor with pin and note number for a capacitive touch input
    MIDIdrum(int p, byte num, byte type);

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
    void setThreshold();
    void setThreshold(unsigned int thresh);
    void setWaitTime(unsigned int time);
    void inputRange(uint16_t thresh, uint16_t max);
    void sensitivity(uint8_t sens);
};

#endif

