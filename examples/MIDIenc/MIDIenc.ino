#include <MIDI.h>
#include <Bounce.h>
#include <Encoder.h>
#include "MIDIcontroller.h"

const int MIDIchannel = 5;

const int encPinA = 21;   // Change these numbers to
const int encPinB = 22;   // the two pins your encoder is on.
const int buttonPin = 23; // Many encoders feature a push switch.
const int ledPin = 13;    // Set an LED to show the state of the switch.

// Encoder parameters are: pin A, pin B, CC number
MIDIenc myEnc(encPinA, encPinB, 24);

MIDIbutton myButton(buttonPin, 25, 1); // CC #25 in latch mode

void setup(){
  pinMode(ledPin, OUTPUT);
  MIDI.begin();
}

void loop(){
  myEnc.read();
  myButton.read();
  digitalWrite(ledPin, myButton.state);
}
