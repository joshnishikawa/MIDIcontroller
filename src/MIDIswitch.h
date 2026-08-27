#ifndef MIDIswitch_h
#define MIDIswitch_h

#include <Arduino.h>
#include <Bounce2.h>
#include "elapsedMillis.h"
#include "MIDItransport.h"

// Check for capacitive touch support:
// Teensy 3.6, 3.2, 3.1, 3.0, LC, or ESP32 / ESP32-S3
#if defined(__MK66FX1M0__) || defined(__MK20DX256__) || defined(__MK20DX128__) || defined(__MKL26Z64__) || defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  #define HAS_CAPACITIVE_TOUCH 1
  #include <Flicker.h>
#endif

#define MOMENTARY 0
#define LATCH 1
#define TRIGGER 2

#define BINARY 3
#define TOUCH 4

#define NOTE 5
#define DRUM 6

#define START 0xFA
#define STOP 0xFC
#define CONTINUE 0xFB
#define CLOCK 0xF8
#define SYSTEM_RESET 0xFF

extern byte MIDIchannel;

#if defined(HAS_CAPACITIVE_TOUCH)
class MIDIswitch: public Bounce, public TouchSwitch{
#else
class MIDIswitch: public Bounce{
#endif
    uint8_t inputType = BINARY; // Bounce object by default
    bool realTime = false;
    elapsedMillis timer;
  public:
    // default constructor
    MIDIswitch();

    // constructor for a switch with the default mode of MOMENTARY
    MIDIswitch(int p, uint8_t num);

    // 'x' could be BINARY, TOUCH, MOMENTARY, LATCH, TRIGGER, NOTE or DRUM
    MIDIswitch(int p, uint8_t num, uint8_t x);

    // constructor for specifying mode (MOMENTARY, LATCH, TRIGGER, etc.)
    // and type (BINARY, TOUCH), or shifted CC (num, numS, mode)
    MIDIswitch(int p, uint8_t num, uint8_t m, uint8_t t);

    // constructor specifying shifted CC, mode, and type
    MIDIswitch(int p, uint8_t num, uint8_t numS, uint8_t m, uint8_t t);

    // destructor
    ~MIDIswitch();

#if defined(HAS_CAPACITIVE_TOUCH)
    // setThreshold() is only used for Capacitive Touch inputs. It assumes the
    // input is NOT being touched and automatically calculates a threshold
    // using a call to touchRead(). Use in setup().
    void setThreshold();
    void setThreshold(int threshold);
#endif

    int read(); // return outHi for falling edge, outLo for rising edge, else -1
    int send(int shiftState = 0); // calls read(), sends a MIDI value & returns the control number
    int send(bool force);   // forces MIDI output regardless of input
    void write(bool s);     // sets the state of a LATCH input

    void setChannel(byte chan, byte cable = 0, byte iface = MIDI_INTERFACE_USB);
    void setOnMessage(byte type, byte data1, byte data2);
    void setOffMessage(byte type, byte data1, byte data2);

    uint8_t number = 0;     // CC or Note number, redefined on instantiation
    uint8_t numberS = 0;    // Shifted CC / Note number
    uint8_t outLo = 0;
    uint8_t outHi = 127;
    uint8_t mode = MOMENTARY; // momentary by default
    uint8_t inputState = 0; // refers to the actual physical state of the input
    bool state = false;     // refers to the most recently sent MIDI message
                            // e.g. a switch may be latched on when not held down

    byte channel = 0;       // 0 = use global MIDIchannel
    byte cable = 0;
    byte interface = MIDI_INTERFACE_USB;

    bool customMessage = false;
    uint8_t onType = 0;
    uint8_t onData1 = 0;
    uint8_t onData2 = 0;
    uint8_t offType = 0;
    uint8_t offData1 = 0;
    uint8_t offData2 = 0;

    void setControlNumber(byte num);
    void setControlNumber(byte num, byte numS);
    void setMode(byte mod);
    void outputRange(byte min, byte max);
};

// Typedef alias so sketches using MIDIbutton work transparently
typedef MIDIswitch MIDIbutton;

#endif
