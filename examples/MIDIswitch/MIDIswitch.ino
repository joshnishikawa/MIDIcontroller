/*
  This example allows any type of switch to be used to send MIDI messages.
  MOMENTARY, LATCH or TRIGGER may be specified (MOMENTARY is default)
  MOMENTARY: Sends ON message when connection is made, OFF when connection is broken.
  LATCH: Sends alternating ON OFF messages each time a connection is made.
  TRIGGER: Sends an ON *and* OFF message each time a connection is made.

  A capacititive touch sensor may also be used.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int latchPin = 10; //any digital pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.

/* MIDIswitch parameters are:
      1) pin (required)
      2) CC number (required)
      3) MOMENTARY, LATCH or TRIGGER (optional, MOMENTARY is default)
      4) BINARY or TOUCH (optional, BINARY is default)
*/

MIDIswitch myInput(latchPin, 21); // Momentary button for CC# 21
// MIDIswitch myInput(latchPin, 21, LATCH); // will latch on and off
// MIDIswitch myInput(latchPin, 21, LATCH, TOUCH); // use a capacitive touch sensor instead of a switch

void setup(){
  pinMode(ledPin, OUTPUT);

  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // myInput.setControlNumber(22); // change CC#
  // myInput.setMode(TRIGGER); // will send an ON and OFF message for each press
  // myInput.outputRange(30, 70); // toggle between 30 and 70
}

void loop(){
  myInput.send();
  digitalWrite(ledPin, myInput.state);


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
