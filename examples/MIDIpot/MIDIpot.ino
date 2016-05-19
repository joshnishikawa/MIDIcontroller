#include "MIDIcontroller.h"

/*This is an example of how to use
  potentiometers and analog sensors.
  The use of a photocell will be
  demonstrated. I'ts wired like so:
 
  3.3v--(PHOTO CELL)---\
                        )---Analog Pin
  GND-------/\/\/------/
             10k
*/

byte MIDIchannel = 5;
const int potPin = A0;  // Change these numbers to the ANALOG
const int sensPin = A9; // pins your pot and sensor are on.

// Pot parameters are: pin, CC number, min value, max value
MIDIpot myPot(potPin, 22, 0, 63);
MIDIpot mySensor(sensPin, 23);
// MIDIpot works for various analog inputs.
// BOTH min and max values should be omitted if using the default (0, 127)

void setup(){
  // Use the rangeFinder example to find the usable range of
  // your sensor and enter it here. (not necessary for potentiometers)
  mySensor.inputRange(350, 950);
}

void loop(){
  myPot.send();
//  mySensor.send();
}
