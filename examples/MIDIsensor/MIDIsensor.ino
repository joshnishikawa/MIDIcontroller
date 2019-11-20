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
const int sensPin = 21; // Change this to the ANALOG pin you want to use.

// parameters are: pin, CC number
MIDIpot mySensor(sensPin, 23);

void setup(){
  Serial.begin(9600);
  // Use the rangeFinder example to find the usable range of your sensor
  // and call inputRange() with the lowest and highest values.
  mySensor.inputRange(350, 950);
  mySensor.outputRange(120, 8);
}

void loop(){ 
  Serial.println(mySensor.send());
}
