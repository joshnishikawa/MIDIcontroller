#include "MIDIcontroller.h"
byte MIDIchannel = 5;
//const int latchPin = 10;
const int touchPin = 17;
const int ledPin = 13;   // Set an LED to show the state of the latch button.
//MIDIbutton latchButton(latchPin, 21, 1);
MIDIbutton touchButton(touchPin, 15, LATCH, 5000);

void setup(){
  pinMode(ledPin, OUTPUT);
}
void loop(){
//  latchButton.send();
  touchButton.send();
  digitalWrite(ledPin, touchButton.state);
}
