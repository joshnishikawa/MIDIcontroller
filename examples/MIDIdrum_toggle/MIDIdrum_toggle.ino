/*
  This example will show how to use a MIDIdrum to toggle between two different
  NOTE numbers.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int drumPin = 2; 
const int ledPin = 13; //Set an LED to show the state of the input.
bool state = false;

MIDIdrum myInput(drumPin, 66);

void setup(){
  pinMode(ledPin, OUTPUT);
}

void loop(){
  if ( myInput.send() > 0) { // no input == -1, ignore OFF messages
    state = !state;
    if (state) myInput.setNoteNumber(33);
    else myInput.setNoteNumber(66);
  }

  digitalWrite(ledPin, state); // LED indicates state


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
