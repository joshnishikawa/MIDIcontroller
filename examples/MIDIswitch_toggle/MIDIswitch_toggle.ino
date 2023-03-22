/*
  This example will show how to use a switch to toggle between two different
  MIDI messages. It could allow a switch to toggle START and STOP for example.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int switchPin = 2; 
const int ledPin = 13; //Set an LED to show the state of the input.
bool state = false;

MIDIswitch myInput(switchPin, START); // Don't use LATCH

void setup(){
  pinMode(ledPin, OUTPUT);
}

void loop(){
  if ( myInput.send() > -1) { // no input == -1
    state = !state;
    if (state) myInput.setControlNumber(STOP);
    else myInput.setControlNumber(START);
  }

  digitalWrite(ledPin, state); // LED indicates state


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
