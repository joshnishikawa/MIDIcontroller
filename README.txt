
  ////////////////////////////////////////////////////
  //                                                //
  //          DMXcontroller - Version 1.0        //
  //                                                //
  // A library for creating Teensy DMX controllers //
  ////////////////////////////////////////////////////

/*  FEATURES INCLUDE:
  - Velocity sensitive inputs (with Aftertouch or Poly Pressure)
  - momentary, latch or instantaneous buttons
  - Stable analog to DMX conversion for potentiometers and other sensors
  - Support for encoders and capacitive sensors
  - Min/Max output can be set (or inverted) for all MIDI. Min/Max input
      can also be set for analog input (maintaining stability in conversion)

   IN THE WORKS:
  - changing all MIDI to DMX 
  - renaming some variables like myButt to less irreverant language ;)
  - assessing if I need a certain item due to the differences between MIDI and DMX

    IN THE WORKS (in the repository this is ported from):
  - Support for piezos
  - Muxing of large arrays of notes
  - More user-friendly 'rangeFinder' example


1.0.0 - changed all instances of 'MIDI' to 'DMX'
  
  

  
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

contact: harryprayiv@gmail.com
*/
