#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int FSRpin = 23;  // Change this to the ANALOG pin you want to use.

MIDIdrum myPad(FSRpin, 40);
MIDIpot aftertouch(FSRpin, 77);

void setup(){
}

void loop(){
  myPad.send();
  aftertouch.send();


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
