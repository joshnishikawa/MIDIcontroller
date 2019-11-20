#include "MIDIcontroller.h"

/*This is an example of how to set up
  a velocity sensitive TOUCH input. */

byte MIDIchannel = 5;
const int touchPin = 1; // Change this to the correct TOUCH pin

// 38 is the note number (See GM DRUM SOUNDS below)
// TOUCH indicated that you're using a capacitive touch input.
MIDIdrum myInput(touchPin, 38, TOUCH);

void setup(){
  // WARNING! For TOUCH, call setThreshold() with no argument.
  // The threshold is calculated based on a call to
  // touchRead() so DON'T touch the input during setup()
  myInput.setThreshold();
}

void loop(){
  myInput.send();
}


/* GM DRUM SOUNDS 
MIDI# - NOTE - SOUND
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


                  MIDI NOTE NUMBERS 
      C   C#  D   D#  E   F   F#  G   G#  A   A#  B
   |------------------------------------------------
-2 |   0   1   2   3   4   5   6   7   8   9  10  11
-1 |  12  13  14  15  16  17  18  19  20  21  22  23
 0 |  24  25  26  27  28  29  30  31  32  33  34  35
 1 |  36  37  38  39  40  41  42  43  44  45  46  47
 2 |  48  49  50  51  52  53  54  55  56  57  58  59
 3 |  60  61  62  63  64  65  66  67  68  69  70  71
 4 |  72  73  74  75  76  77  78  79  80  81  82  83
 5 |  84  85  86  87  88  89  90  91  92  93  94  95
 6 |  96  97  98  99 100 101 102 103 104 105 106 107
 7 | 108 109 110 111 112 113 114 115 116 117 118 119
 8 | 120 121 122 123 124 125 126 127
*/
