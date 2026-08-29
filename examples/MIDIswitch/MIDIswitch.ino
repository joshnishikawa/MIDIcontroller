/*
  This example allows any type of switch to be used to send MIDI messages.
  A capacititive touch sensor may also be used.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
#if defined(D0)
const int switchPin = D0; // XIAO / ESP32 digital pin
#else
const int switchPin = 2;  // Arduino AVR / Teensy digital pin
#endif
const int ledPin = 13;   // Set an LED to show the state of a latch button.

/* MIDIswitch parameters are:
      1) pin (required)
      2) a Control Change / Channel Mode number: 0 ~ 127
         OR a Note number (when mode is NOTE or DRUM)
         OR a Real Time message: START, STOP, CONTINUE, CLOCK, SYSTEM_RESET
         (required)
      3) MOMENTARY, LATCH, TRIGGER, NOTE, or DRUM
          MOMENTARY: Sends CC ON when pressed, CC OFF when released.
          LATCH: Alternates CC ON and OFF on each press.
          TRIGGER: Sends CC ON on each press (no OFF message).
          NOTE: Sends Note On on press, Note Off on release.
          DRUM: Sends Note On and automatically times out.
          (optional, default for CC is MOMENTARY, default for Real Time is TRIGGER)
      4) BINARY or TOUCH
          (optional, BINARY is default, TOUCH is for capacitive touch sensors)
*/

// MIDIswitch myInput(switchPin, 21); // Momentary button for CC# 21
MIDIswitch myInput(switchPin, 21, LATCH);        // will latch on and off
// MIDIswitch myInput(switchPin, 21, LATCH, TOUCH); // use a capacitive touch sensor instead of a switch
// MIDIswitch myInput(switchPin, START);            // will send start messages
// MIDIswitch myInput(switchPin, 38, NOTE);         // will send note messages (Snare Drum Note 38)

void setup(){
  MIDI_setup(); // Optional helper: initializes serial ports
  pinMode(ledPin, OUTPUT);

  // YOU MUST UNCOMMENT ONE OF THE FOLLOWING 2 LINES TO USE A TOUCH SENSOR
  // myInput.setThreshold(); // Auto-calibrates baseline. WARNING: Do not touch during setup()!
  // myInput.setThreshold(500); // OR specify a known threshold value directly

  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // myInput.setControlNumber(22); // change CC# or Note#
  // myInput.setMode(TRIGGER);     // change mode (MOMENTARY, LATCH, TRIGGER, NOTE, DRUM)
  // myInput.outputRange(30, 70);  // toggle between custom values (e.g. 30 and 70)
}

void loop(){
  MIDI_loop(); // Flushes incoming MIDI buffers

  myInput.send();
  digitalWrite(ledPin, myInput.state);
}
