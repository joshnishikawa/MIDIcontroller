#include "MIDIcontroller.h"
byte MIDIchannel = 5;
const int latchPin = 6; // the pins you have buttons on.
const int ledPin = 13;   // Set an LED to show the state of the latch button.

//This creates button for CC number 21 with 
//an 'off' value of 0 and 'on' value of 115 in latch mode(1)
MIDIbutton latchButton(latchPin, 21, 0, 115, 1);

void setup(){
  pinMode(ledPin, OUTPUT);
}

void loop(){
  latchButton.send();
  digitalWrite(ledPin, latchButton.state);
}
