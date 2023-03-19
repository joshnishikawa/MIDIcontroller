/*This is an example of how to use analog sensors.
  The use of a photocell will be demonstrated. It's wired like so:
 
  3.3v--(PHOTO CELL)---\
                        )---Analog Pin
  GND-------/\/\/------/
             10k
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int sensPin = 21; // Change this to the ANALOG pin you want to use.

/* MIDIpot parameters are:
      1) pin (required)
      2) CC number (required)
      3) a secondary on/off CC# (optional)
*/

MIDIpot mySensor(sensPin, 23);
// MIDIpot mySensor(sensPin, 23, 9); // CC# 9 OFF is sent when you turn the pot all the way down.
                              // CC# 9 ON is sent when you start turning it up again.


void setup(){
  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // mySensor.inputRange(350, 950); // Use the 'findAnalogRange' example to find these values
  // mySensor.outputRange(20, 120); // Restrict output to 20 ~ 120
}

void loop(){ 
  mySensor.send();


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
