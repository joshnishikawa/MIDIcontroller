#ifndef MIDIencMulti_h
#define MIDIencMulti_h

#include <Arduino.h>
#include "Encoder.h"
#include "MIDItransport.h"

#ifndef PER_VALUE
#define PER_VALUE 1
#define PER_DETENT 4
#endif

#define ENCODER_MODE_STANDARD 0
#define ENCODER_MODE_JOG_1_CC 1
#define ENCODER_MODE_JOG_2_CC 2

// Backward-compatible alias definitions
#define ENCODER 0
#define JOG_1_CC 1
#define JOG_2_CC 2

extern byte MIDIchannel;

class MIDIencMulti {
public:
  MIDIencMulti();
  ~MIDIencMulti();

  // Mode 0: Standard Absolute CC Encoder
  void initAsEncoder(int a, int b, byte num, byte min = 0, byte max = 127, byte detentOrValue = PER_VALUE);
  void initAsEncoder(int a, int b, byte num, byte numS, byte min, byte max, byte detentOrValue = PER_VALUE);

  // Mode 1: Single CC Jogwheel (Relative CC)
  void initAsJog1Cc(int a, int b, byte num, byte outValLeft, byte outValRight, byte detentOrValue = PER_VALUE);
  void initAsJog1Cc(int a, int b, byte num, byte numS, byte outValLeft, byte outValRight, byte detentOrValue = PER_VALUE);

  // Mode 2: Dual CC Jogwheel (One CC for CW, another CC for CCW)
  void initAsJog2Cc(int a, int b, byte numLeft, byte numRight, byte detentOrValue = PER_VALUE);
  void initAsJog2Cc(int a, int b, byte numLeft, byte numRight, byte numLeftS, byte numRightS, byte detentOrValue = PER_VALUE);

  int read();
  int send(int shiftState = 0);
  int send(bool force);

  void setChannel(byte chan, byte cable = 0, byte iface = MIDI_INTERFACE_USB);
  void setControlNumber(byte num);
  void outputRange(byte min, byte max);
  void write(byte val);

  byte number = 0;
  byte numberS = 0;
  byte numberLeft = 0;
  byte numberRight = 0;
  byte numberLeftS = 0;
  byte numberRightS = 0;
  byte outValueLeft = 1;
  byte outValueRight = 127;

  byte value = 0;
  byte outLo = 0;
  byte outHi = 127;
  byte detentOrValue = PER_VALUE;
  byte encoderMode = ENCODER_MODE_STANDARD;

  byte channel = 0; // 0 = use global MIDIchannel
  byte cable = 0;
  byte interface = MIDI_INTERFACE_USB;

  Encoder *myKnob = NULL;

private:
  void allocateEncoder(int a, int b);
};

#endif
