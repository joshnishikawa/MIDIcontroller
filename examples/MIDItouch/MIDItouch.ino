#include "MIDIcontroller.h"

// This is an example of how to set up a capacitive touch
// sensor as a MIDI input. Any capacitive object connected
// to a 'touch' pin will work. If using metal, use a layer of 
// something non-conductive between the metal and skin.

byte MIDIchannel = 5;
const int touchPin = 22; // Change this to the correct TOUCH pin

// Parameters are: pin and CC number
MIDItouch myInput(touchPin, 60);

void setup(){
  // WARNING! if you use inputRange() with no arguments,
  // the input range is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.inputRange();
}

void loop(){
  myInput.send();
}
