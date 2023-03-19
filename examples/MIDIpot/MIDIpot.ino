/*
  This example is for a potentiometer. It should be wired like so:
 
  3.3v---(POT)---\
                  )---Analog Pin
  GND----/\/\/---/
          10k
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int potPin = A0;  // Change this to the ANALOG pin you want to use

/* MIDIpot parameters are:
      1) pin (required)
      2) CC number (required)
      3) a secondary on/off CC# (optional)
*/

MIDIpot myPot(potPin, 22);
// MIDIpot myPot(potPin, 22, 9); // CC# 9 OFF is sent when you turn the pot all the way down.
                              // CC# 9 ON is sent when you start turning it up again.

void setup(){
  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // myPot.inputRange(20, 900); // Specify the usable analog range for the pot
  // myPot.outputRange(127, 0); // Reverse the direction of the pot
  // myPot.outputRange(20, 120); // Restrict value to 20 ~ 120
}

void loop(){
  myPot.send();


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
