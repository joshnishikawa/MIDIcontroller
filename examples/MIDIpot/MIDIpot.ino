#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int potPin = A0;  // Change this to the ANALOG pin you want to use

// Pot parameters are: pin, CC number, KILL switch enabled
// When KILL is enabled, separate CC messages (with a different number) will be sent
// when you turn the pot all the way down and when you start turning it up again.
// Simply omit the "KILL" argument if you don't want that.
MIDIpot myPot(potPin, 22, KILL);

// OPTIONAL: use outputRange() to limit the min/max MIDI output values
// mysensor.outputRange(12, 90);

void setup(){
}

void loop(){
  myPot.send();
}
