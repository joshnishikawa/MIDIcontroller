#include "MIDIcontroller.h"

byte MIDIchannel = 5;

const int encPinA = 24;   // Change these numbers to
const int encPinB = 25;   // the two pins your encoder is on.

/* Encoder parameters are: 
      1)  pin A (required)
      2)  pin B (required)
      3)  a CC number ( 0 ~ 119 ) 
          OR a channel mode number ( 120 ~ 127 ) 
          OR PROGRAM_CHANGE to send program changes
          (required)
      4)  PER_VALUE or PER_DETENT (optional, PER_DETENT is default) 
 */

MIDIenc myEnc(encPinA, encPinB, 24);
// MIDIenc myEnc(encPinA, encPinB, 24, PER_VALUE); // Faster but not for use with detented encoders.
// MIDIenc myEnc(encPinA, encPinB, PROGRAM_CHANGE); // Send program change instead of CC# 

void setup(){
  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  // myEnc.write(64); // Initialize the encoder to 64
  // myEnc.outputRange(127, 0); // Reverse the direction of the encoder
  // myEnc.outputRange(56, 79); // Restrict CC value or program change # to 56 ~ 79
}

void loop(){
  myEnc.send();


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
