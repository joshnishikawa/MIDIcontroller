#ifndef MIDIpot_h
#define MIDIpot_h

#include "Arduino.h"
#include "MIDItransport.h"

// Platform-aware ADC defaults
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
  #define MIDI_DEFAULT_ADC_MAX 4095
  #define MIDI_DEFAULT_POT_SMOOTHING 200
#else
  #define MIDI_DEFAULT_ADC_MAX 1023
  #define MIDI_DEFAULT_POT_SMOOTHING 50
#endif

#ifndef SMOOTHING
  #define SMOOTHING MIDI_DEFAULT_POT_SMOOTHING // Can be increased at the cost of some responsiveness
#endif

#define KILL 9 // previously undefined CC# safe for general purpose assignment
#define OFF 0

extern byte MIDIchannel;

class MIDIpot{
    int pin = 0;
    bool invert = false;
    int divider = 8; // for converting from analog to MIDI resolution
    int buffer = 0; // a container used by the smooth() function 
    int difference = 0; // smooth() adds this to buffer 'til there's enough bias
    uint16_t balancedValue = 0; // remains stable until signal bias changes it
    
  public:
    // default constructor
    MIDIpot();
    
    // constructor when only pin & control number are given
    MIDIpot(int p, uint8_t num);
    
    // " when pin, control number are given and kill switch is enabled
    MIDIpot(int p, uint8_t num, uint8_t k);
    
    // destructor
   	~MIDIpot();

    int read(); // read input and return a MIDI value (or -1 if none)
    int read(int raw); // process a raw analog reading and return a MIDI value (or -1 if none)
    int send(); // calls read(), sends/returns MIDI val (or -1 if none)
    int send(bool force); // forces MIDI output regardless of input
    void setChannel(byte chan, byte cable = 0, byte iface = MIDI_INTERFACE_USB);

    uint16_t inLo = 0;
    uint16_t inHi = MIDI_DEFAULT_ADC_MAX;
    uint8_t outLo = 0;
    uint8_t outHi = 127;
    uint8_t number = 0;
    uint8_t value = 0;
    uint8_t mode = 0; // In case you need to kill an effect entirely
    uint8_t killSwitch = 0; // Which CC is getting its hands dirty?

    byte channel = 0; // 0 = global MIDIchannel
    byte cable = 0;
    byte interface = MIDI_INTERFACE_USB;

    void setControlNumber(uint8_t num);
    void outputRange(uint8_t min, uint8_t max);
    void inputRange(uint16_t min, uint16_t max);
    void setKillSwitch(uint8_t k);
    int smooth(int value, int NR); // smoothes the values of analogRead()
};

#endif