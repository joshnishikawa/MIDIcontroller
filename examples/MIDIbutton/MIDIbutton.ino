#include "MIDIcontroller.h"
byte MIDIchannel = 5;
const int latchPin = 10; //any digital pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.

// MOMENTARY buttons are the default. LATCH or TRIGGER may also be set
MIDIbutton myInput(latchPin, 21, LATCH); // Control Change #21

void setup(){
  pinMode(ledPin, OUTPUT);
}

void loop(){
  myInput.send();
  digitalWrite(ledPin, myInput.state);


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
