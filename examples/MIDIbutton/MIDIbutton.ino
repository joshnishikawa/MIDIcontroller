#include "MIDIcontroller.h"
int MIDIchannel = 5;
const int latchPin = 14; // the pins you have buttons on.
const int ledPin = 13;   // Set an LED to show the state of the latch button.
MIDIbutton latchButton(latchPin, 21, 92, 15, 1);
void setup(){
  pinMode(ledPin, OUTPUT);
}
void loop(){
  latchButton.send();
  digitalWrite(ledPin, latchButton.state);
}
