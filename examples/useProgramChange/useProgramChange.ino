#include "MIDIcontroller.h"

byte MIDIchannel = 0;
const byte switchPin = 2;
const byte encPinA = 25;
const byte encPinB = 24; 
const byte padPin = 14;

MIDIenc myEnc(encPinA, encPinB, PROGRAM_CHANGE);
MIDIdrum myPad(padPin, 3);                      // CC #3 not actually used
MIDIswitch mySwitch(switchPin, 3, TRIGGER);     // CC #3 not actually used


void setup() {
  myPad.inputRange(12, 720);
  myPad.setSensitivity(99);
}


void loop() {
  int encValue = myEnc.send();

  int switchValue = mySwitch.read();
  if ( switchValue > 0 ){
    myEnc.write( myEnc.value + 1 ); // keep encoder up-to-date
    myEnc.send(FORCE);              // send the program change
  }

  int padValue = myPad.read();
  // whether myPad increments or decrements
  // depents on the state of the switch
  if ( padValue > 0 ){ // only inc/dec on rising edge
    if ( digitalRead(switchPin) == LOW ){
      myEnc.write( myEnc.value - 1 );
    } else {
      myEnc.write( myEnc.value + 1 );
    }
    myEnc.send(FORCE);
  }
}
