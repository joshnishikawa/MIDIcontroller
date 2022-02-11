#include "MIDIcontroller.h"

byte MIDIchannel = 5;

const int encPinA = 20;   // Change these numbers to
const int encPinB = 21;   // the two pins your encoder is on.
const int buttonPin = 19; // Many encoders feature a push switch.
const int ledPin = 13;    // Set an LED to show the state of the switch.

// Encoder parameters are: pin A, pin B, CC number
// You can also add min AND max output 
// and/or PER_DETENT as a final argument.
// PER_DETENT increases or decreases the CC value once per detent. (on detented quadratic encoders)
MIDIenc myEnc(encPinA, encPinB, 24, PER_DETENT);

MIDIbutton myButton(buttonPin, 25, LATCH); // CC #25 in latch mode

void setup(){
  pinMode(ledPin, OUTPUT);
}

void loop(){
  myEnc.send();
  myButton.send();
  digitalWrite(ledPin, myButton.state);


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
