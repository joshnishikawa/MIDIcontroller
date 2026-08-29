/*
  useAfterTouch.ino

  Demonstrates using a single Force-Sensitive Resistor (FSR) or pressure sensor
  for BOTH velocity-sensitive drum hits (MIDIdrum) AND continuous pressure /
  aftertouch (MIDIpot) on the same analog pin!
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int FSRpin = A0;  // Change this to the ANALOG pin connected to your FSR

// Trigger Electric Snare (Note 40) on hit
MIDIdrum myPad(FSRpin, 40);

// Continuous pressure sends Filter Cutoff / Aftertouch (CC 74 or Channel Pressure)
MIDIpot aftertouch(FSRpin, 74);

void setup(){
  MIDI_setup(); // Optional helper: initializes serial ports

  myPad.setSensitivity(50); // 100% triggers on light contact; lower requires harder strike
  myPad.setWaitTime(30);    // 30ms debounce interval
}

void loop(){
  MIDI_loop(); // Flushes incoming MIDI buffers

  myPad.send();      // Process drum hit trigger
  aftertouch.send(); // Process continuous pressure modulation
}
