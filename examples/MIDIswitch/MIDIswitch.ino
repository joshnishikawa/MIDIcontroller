/*
  This example allows any type of switch to be used to send MIDI messages.
  A capacititive touch sensor may also be used.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int switchPin = 10; //any digital pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.

/* MIDIswitch parameters are:
      1) pin (required)
      2)  a Control Change / Channel Mode number: 0 ~ 127
          OR a Real Time message: START, STOP, CONTINUE, CLOCK, SYSTEM_RESET
          (required)
      3) MOMENTARY, LATCH or TRIGGER
          MOMENTARY: Sends ON messages when connection is made, OFF when broken.
          LATCH: Sends alternating ON OFF messages each time a connection is made.
          TRIGGER: Sends ON messages each time a connection is made but no OFF messages.
          (optional, CC default is MOMENTARY, Real Time default is TRIGGER)
      4) BINARY or TOUCH
          (optional, BINARY is default, TOUCH is for capacitive touch sensors)
*/

MIDIswitch myInput(switchPin, 21); // Momentary button for CC# 21
// MIDIswitch myInput(switchPin, 21, LATCH);        // will latch on and off
// MIDIswitch myInput(switchPin, 21, LATCH, TOUCH); // use a capacitive touch sensor instead of a switch
// MIDIswitch myInput(switchPin, START);            // will send start messages
// MIDIswitch myInput(switchPin, 38, NOTE);           // will send note messages

void setup(){
  pinMode(ledPin, OUTPUT);

  // YOU MUST UNCOMMENT ONE OF THE FOLLOWING 2 LINES TO USE A TOUCH SENSOR
  // myInput.inputRange(); // WARNING! If you touch the input during setup(), it won't work!
  // myInput.inputRange(70, 2100); // OR use the 'findTouchRange' example to find values to specify here

  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // myInput.setControlNumber(22); // change CC#
  // myInput.setMode(TRIGGER); // will send an ON and OFF message for each press
  // myInput.outputRange(30, 70); // toggle between 30 and 70
}

void loop(){
  myInput.send();
  digitalWrite(ledPin, myInput.state);


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
