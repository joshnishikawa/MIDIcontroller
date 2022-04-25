#include "MIDIcontroller.h"
byte MIDIchannel = 5;
const int touchPin = 17; //any Capacitive Touch capable pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.

// MOMENTARY buttons are the default. LATCH or TRIGGER may also be set
MIDIswitch myInput(touchPin, 15, TRIGGER, TOUCH);// CC #15, capacitive touch

void setup(){
  pinMode(ledPin, OUTPUT);

  // WARNING! if you setThreshold with no argument,
  // the threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold();
}
void loop(){
  myInput.send();
  digitalWrite(ledPin, myInput.state);


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
