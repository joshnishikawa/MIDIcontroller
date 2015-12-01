#include <MIDI.h>
#include <Bounce.h>
#include <Encoder.h>
#include "MIDIbutton.h"

int MIDIchannel = 5;
const int holdPin = 2;   // Change these numbers to
const int latchPin = 12; // the pins you have buttons on.

const int ledPin = 13;   // Set an LED to show the state of the latch button.

// Button parameters are: pin, CC number, min value, max value, mode
// '0' is momentary mode, '1' is latch mode, '2' sends both ON/OFF when pressed
MIDIbutton latchButton(latchPin, 21, 92, 15, 1);
MIDIbutton holdButton(holdPin, 20);
// As you can see, the '0' may be omitted for momentary buttons.
// BOTH min and max values should be omitted if using the default (0, 127)

void setup(){
  pinMode(ledPin, OUTPUT);
  MIDI.begin();
}

void loop(){
  holdButton.read();
  latchButton.read();
  digitalWrite(ledPin, latchButton.state);
}
