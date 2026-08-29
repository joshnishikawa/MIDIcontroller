/*
  This example shows how to use a switch to toggle between two different
  MIDI messages (e.g. toggling Real Time START and STOP on alternate presses).
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;

#if defined(D0)
const int switchPin = D0; // XIAO / ESP32 digital pin
#else
const int switchPin = 2;  // Arduino AVR / Teensy digital pin
#endif
const int ledPin = 13;   // Set an LED to show the state of the toggle.

bool isRunning = false;

// Initialize in TRIGGER mode for Real Time START
MIDIswitch myInput(switchPin, START); // Don't use LATCH since we alternate message types manually

void setup(){
  MIDI_setup(); // Optional helper: initializes serial ports
  pinMode(ledPin, OUTPUT);
}

void loop(){
  MIDI_loop(); // Flushes incoming MIDI buffers

  if (myInput.send() > -1) { // Returns message type (> -1) when triggered
    isRunning = !isRunning;
    if (isRunning) {
      myInput.setControlNumber(STOP); // Next press will send STOP
    } else {
      myInput.setControlNumber(START); // Next press will send START
    }
  }

  digitalWrite(ledPin, isRunning ? HIGH : LOW); // LED indicates running state
}
