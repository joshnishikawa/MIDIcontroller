#include "MIDIcontroller.h"
byte MIDIchannel = 5;

const int startPin = 15;
const int stopPin = 26;

// MIDIswitch can take START, STOP, CONTINUE, CLOCK, or SYSTEM_RESET
// to send real time messages instead of CCs
// Add TOUCH as the third argument if using a capacitive touch sensor.
MIDIswitch myStart(startPin, START);
MIDIswitch myStop(stopPin, STOP);

void setup(){}

void loop(){
  myStart.send();
  myStop.send();

// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}
// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
