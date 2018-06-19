#include "MIDIcontroller.h"
byte MIDIchannel = 5;
const int latchPin = 10; //any digital pin
const int touchPin = 17; //any Capacitive Touch capable pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.

// MOMENTARY buttons are the default. LATCH or TRIGGER may also be set
MIDIbutton latchButton(latchPin, 21, LATCH);           // Control Change #21
MIDIbutton triggerButton(touchPin, 15, TRIGGER, 5000); // CC #15, capacitive touch threshold 5000

void setup(){
  pinMode(ledPin, OUTPUT);
}
void loop(){
  latchButton.send();
//  triggerButton.send();
  digitalWrite(ledPin, latchButton.state);
}
