#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int FSRpin = 23;  // Change this to the correct ANALOG pin
MIDIdrum myPad(FSRpin, 40);
MIDIpot aftertouch(FSRpin, 77);

void setup(){
}

void loop(){
  myPad.send();
  aftertouch.send();
}
