#include "MIDIcontroller.h"

/*This example sets up a velocity sensitive input using a piezo transducer.

  THE PIEZO MUST BE WIRED CORRECTLY FOR THS TO WORK AND TO PREVENT DAMAGE!
  - The black wire of the Piezo, the anode of a diode and
    one side of a 10k resistor should all be connected to ground.
  - The cathode of A SECOND diode should be connected to the voltage.
  - The red wire of the Piezo should be connected to a 470 ohm resistor.
  - The remaining ends of BOTH resistors and BOTH diodes should be connected
    to the same analog pin on the microcontroller.
  
  (You could replace the 10k pot with a 10k resistor to adjust sensitivity.)
*/

byte MIDIchannel = 5;
const int drumPin = 23; // Change this to the correct ANALOG pin

// Drum Parameters are: pin, note number, velocity on
MIDIdrum myDrum(drumPin, 38);

void setup(){
}

void loop(){
  myDrum.send();
}

