#ifndef DMXpot_h
#define DMXpot_h

#include "Arduino.h"

extern byte DMXchannel;

class DMXpot{
    int divider; // for converting from analog to DMX resolution
    
  public:
    // default constructor
    DMXpot();
    
    // constructor when only pin & Channel number are given
    DMXpot(int p, byte num);
    
    // " when pin, Channel number are given and kill switch is enabled
    DMXpot(int p, byte num, byte m);
    
    // " pin, Channel number, minimum & maximum outgoing DMX values are set
    DMXpot(int p, byte num, byte min, byte max);
    
    // " pin, Channel number, min/max values are given and kill switch is enabled
    DMXpot(int p, byte num, byte min, byte max, byte m);
    
    // destructor
   	~DMXpot();

    int pin;
    bool invert;
    int read(); // read input and return a DMX value (or -1 if none)
    int send(); // calls read(), sends and returns a DMX value (or -1 if none)
    byte number;
    uint16_t inLo, inHi;
    byte outLo, outHi;
    byte mode;  // in case you need to kill an effect entirely
    byte value;
    void setChannelNumber(byte num);
    void inputRange(uint16_t min, uint16_t max);
    void outputRange(byte min, byte max);
};

#endif
