
  ////////////////////////////////////////////////////
  //                                                //
  //          MIDIcontroller - Version 2.2.0        //
  //                                                //
  // A library for creating Teensy MIDI controllers //
  ////////////////////////////////////////////////////

/*  FEATURES INCLUDE:
  - Velocity sensitive FSR or Piezo inputs
  - momentary, latch or instantaneous buttons
  - Stable analog to MIDI conversion for potentiometers and other sensors
  - Support for encoders and capacitive sensors
  - Min/Max output can be set (or inverted) for all MIDI. Min/Max input
      can also be set for analog input (maintaining stability in conversion)

    IN THE WORKS:
  - Muxing of large arrays of notes
  - More user-friendly 'rangeFinder' example

  2.2.0 - Added support for Piezos (Must be wired properly. See example)
        - "MIDInote" class changed to "MIDIdrum" and optimized for FSR and Piezo
        - Removed redundant 'velocity' variable. Just call outputRange(127, 127)
        - Added support for using a Capacitive Touch input as a MIDIbutton
	2.1.5 - got rid of useless '*MC' pointer. renamed 'kill' to 'mode'
  2.1.3 - included an example of how to implement aftertouch
  2.1.2 - many variables changed to 'byte' or 'uint16_t' for easy storage
          public and private variables are better sorted
  2.1.0 - Split read() and send() functions. MIDI channel is now user selectable
  2.0.6 - Fixed a bug preventing poly CC to return to zero after note off.
  2.0.5 - Added a condition to prevent many double note triggers.
  2.0.4 - Added support for capacitive sensors (and started this version log)
  

  
***THIS LIBRARY REQUIRES THE MIDI, BOUNCE AND ENCODER LIBRARIES***

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
  
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
  
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

contact: joshnishikawa@gmail.com
*/