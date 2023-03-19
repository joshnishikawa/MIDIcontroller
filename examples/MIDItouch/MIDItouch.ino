/* 
  This is an example of how to set up a capacitive touch
  sensor as a variable MIDI input. Any capacitive object connected
  to a 'touch' pin will work. If using metal, use a layer of 
  something non-conductive between the metal and your skin.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int touchPin = 22; // Change this to the correct TOUCH pin

/* MIDIpot parameters are:
      1) pin (required)
      2) CC number (required)
      3) a secondary on/off CC# (optional)
*/

MIDItouch myInput(touchPin, 60);
// MIDItouch myInput(touchPin, 60, 9); // CC# 9 OFF is sent when reading drops below threshold.
                                  // CC# 9 ON is sent when threshold is breached.

void setup(){
  // WARNING! if you use inputRange() with no arguments,
  // the input range is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.inputRange();
  // myInput.inputRange(70, 2100); // OR use the 'findTouchRange' example to find values to specify here
  // myInput.outputRange(20, 120); // Restrict output to 20 ~ 120
}

void loop(){
  myInput.send();


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
