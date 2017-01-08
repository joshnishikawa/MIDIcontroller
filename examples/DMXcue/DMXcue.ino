#include "DMXcontroller.h"

/*This is an example of how to set up
  a velocity sensitive input using a
  force sensitive resistor.
  It's wired like so:
 
  3.3v--(FORCE RESISTOR)---\
                            )---Analog Pin
  GND--------/\/\/---------/
              10k
*/

byte DMXchannel = 5;
const int pressPin = A1; // Change this to the correct ANALOG pin

// channel Parameters are: pin, channel number, intensity on
DMXchannel myPad(pressPin, 37, true);

void setup(){
  // Use the rangeFinder example to find the usable range of
  // your sensor and enter it here.
  myPad.inputRange(300, 760);

  // Set the threshold where you want channels to trigger.
  myPad.setThreshold(100);
}

void loop(){
  myPad.send();
}


