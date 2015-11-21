#include <MIDI.h>
#include <Bounce.h>
#include <Encoder.h>
#include "MIDIcontroller.h"

/*This is an example of how to set up
  a velocity sensitive input using a
  force sensitive resistor.
  It's wired like so:
 
  GND---(FORCE RESISTOR)---\
                            )---Analog Pin
  3.3v-------/\/\/---------/
              10k
*/

const int MIDIchannel = 5;
const int pressPin = A0; // Change this to the correct ANALOG pin

// Note Parameters are: pin, note number, velocity on, aftertouch on
MIDInote myPad(pressPin, 60, true, true);

void setup(){
  // Use the rangeFinder example to find the usable range of
  // your sensor and enter it here.
  myPad.inputRange(50, 850);
  
  MIDI.begin();
}

void loop(){
  myPad.read();
}
