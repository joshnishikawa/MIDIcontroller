#ifndef MIDIdrum_h
#define MIDIdrum_h

#include "Arduino.h"
#include "elapsedMillis.h"
#include "MIDItransport.h"

// Platform-aware ADC defaults
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
  #define MIDI_DEFAULT_DRUM_ADC_MAX 4095
  #define MIDI_DEFAULT_DRUM_THRESH  48
  #define MIDI_DEFAULT_DRUM_SENS    40
#else
  #define MIDI_DEFAULT_DRUM_ADC_MAX 1023
  #define MIDI_DEFAULT_DRUM_THRESH  12
  #define MIDI_DEFAULT_DRUM_SENS    10
#endif

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
    void setChannel(byte chan, byte cable = 0, byte iface = MIDI_INTERFACE_USB);

    uint8_t number;
    uint8_t outLo = 1;
    uint8_t outHi = 127;
    unsigned int inHi = MIDI_DEFAULT_DRUM_ADC_MAX;
    unsigned int threshold, upperThreshold;

    byte channel = 0; // 0 = global MIDIchannel
    byte cable = 0;
    byte interface = MIDI_INTERFACE_USB;

    void setNoteNumber(uint8_t num);
    void setThreshold(unsigned int thresh);
    void inputRange(uint16_t thresh, uint16_t max);
    void setSensitivity(uint8_t sens);
    void setWaitTime(unsigned int time);
    void outputRange(uint8_t min, uint8_t max);
};

#endif

