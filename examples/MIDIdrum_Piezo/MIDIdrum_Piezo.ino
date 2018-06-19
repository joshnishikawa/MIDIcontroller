#include "MIDIcontroller.h"

/*This example sets up a velocity sensitive input using a piezo transducer.

  THE PIEZO MUST BE WIRED CORRECTLY FOR THS TO WORK AND TO PREVENT DAMAGE!
  - The black wire of the Piezo, the anode of a diode and
    one side of a 10k resistor should all be connected to ground.
  - The cathode of A SECOND diode should be connected to the voltage.
  - The red wire of the Piezo should be connected to a 470 ohm resistor.
  - The remaining ends of BOTH resistors and BOTH diodes should be connected
    to the same analog pin on the microcontroller.
*/

byte MIDIchannel = 5;
const int drumPin = 23; // Change this to the correct ANALOG pin

// Drum Parameters are: pin, note number
MIDIdrum myDrum(drumPin, 38);

void setup(){
}

void loop(){
  myDrum.send();
  //myDrum.send(64); could be used for a fixed velocity e.g. 64
}

/* MIDI# - NOTE - SOUND
     35    B  0   Acoustic Bass Drum
     36    C  1   Bass Drum 1
     37    C# 1   Side Stick
     38    D  1   Acoustic Snare
     39    D# 1   Hand Clap
     40    E  1   Electric Snare
     41    F  1   Low Floor Tom
     42    F# 1   Closed Hi Hat
     43    G  1   High Floor Tom
     44    G# 1   Pedal Hi-Hat
     45    A  1   Low Tom
     46    A# 1   Open Hi-Hat
     47    B  1   Low-Mid Tom
     48    C  2   Hi-Mid Tom
     49    C# 2   Crash Cymbal 1
     50    D  2   High Tom
     51    D# 2   Ride Cymbal 1
     52    E  2   Chinese Cymbal
     53    F  2   Ride Bell
     54    F# 2   Tambourine
     55    G  2   Splash Cymbal
     56    G# 2   Cowbell
     57    A  2   Crash Cymbal 2
     58    A# 2   Vibraslap
     59    B  2   Ride Cymbal 2
     60    C  3   Hi Bongo
     61    C# 3   Low Bongo
     62    D  3   Mute Hi Conga
     63    D# 3   Open Hi Conga
     64    E  3   Low Conga
     65    F  3   High Timbale
     66    F# 3   Low Timbale
     67    G  3   High Agogo
     68    G# 3   Low Agogo
     69    A  3   Cabasa
     70    A# 3   Maracas
     71    B  3   Short Whistle
     72    C  4   Long Whistle
     73    C# 4   Short Guiro
     74    D  4   Long Guiro
     75    D# 4   Claves
     76    E  4   Hi Wood Block
     77    F  4   Low Wood Block
     78    F# 4   Mute Cuica
     79    G  4   Open Cuica
     80    G# 4   Mute Triangle
     81    A  4   Open Triangle
*/
