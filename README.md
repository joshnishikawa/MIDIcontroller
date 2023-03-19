# MIDIcontroller 3.8.8
### A library for creating Teensy MIDI controllers.
###### by Josh Nishikawa <github.com/joshnishikawa/MIDIcontroller>

 FEATURES INCLUDE:
  - velocity sensitive FSR, Piezo or Capacitive Touch inputs
  - momentary, latch or 'trigger' type MIDI buttons (also work with Cap Touch) 
  - stable analog to MIDI conversion for potentiometers and other sensors
  - stable Capacitive Touch to MIDI conversion for expression control
  - support for encoders
  - Min/Max output can be set (or inverted) for all MIDI. Min/Max input
      can also be set for analog input (maintaining stability in conversion)
___
***THIS LIBRARY REQUIRES - MIDI Library, Bounce, Encoder, Flicker***
<github.com/joshnishikawa/Flicker>
___

### VERSION LOG:
3.8.8 MAJOR RELEASE WITH BREAKING CHANGES:
- MIDIbutton is now completely deprecated. Use MIDIswitch instead.
- MIDIcapSense is now completely deprecated. Use MIDItouch instead.
- MIDIdrum using TOUCH is deprecated. It's still doable but just too unstable
    and too niche for this library. Use Flicker instead.

  OTHER CHANGES:
- Added sensitivity(int) function to MIDIdrum. Takes a number between 1 and 100.
    Lower numbers require more higher velocity to trigger MIDI. 100 is default 
    and driggers even if pressed very slowly. 99 requires at least a light tap.
- MIDIenc can now take PROGRAM_CHANGE as an argument. This allows the encoder
    to send program change messages instead of CC messages. 
- MIDIswitch can now take START, STOP, CONTINUE, CLOCK or SYSTEM_RESET as an
    argument. This allows the switch to send those messages instead of CC.

    (thanks @digitalelements for suggestions and testing all of the above)

- MIDIenc.read() now updates the value of the encoder. This allows the value
    to be changed without actually sending MIDI.
- Split analogRange and touchRange into separate utilities.
- Bugfix: MIDIenc.value initialized to outLo when user-specified.

2.5.5
- Dependency for deprecated Bounce library changed to Bounce2. This allows the 
    library to be installed via the Arduino IDE2 library manager.
- Bugfix: "unsigned int = -1" == oops!
- Other bugfixes

2.5.3
- 'MIDIswitch' now preferred over 'MIDIbutton' ( 'MIDIbutton' still works ).
- Added a send(FORCE) function to allow event-based sending of current CC value.
    This works for MIDIswitch, MIDIpot, MIDIenc and MIDItouch. This allows
    'bulk send' to be implemented. WARNING! In the main loop, just use send()

- MIDIswitch default of MOMENTARY works now. use MIDIswitch(pin, CC)

2.4.3
- Fixed the PER_DETENT bug (again).
- Made PER_DETENT the default since it works cleaner and is probably more useful
- Added a write(int) function to the MIDIenc class that will set it to a
    specific value and immediately send a MIDI message for that CC at that value

2.4.2 
- Fixed a bug preventing PER_DETENT from working on encoders.

2.4.1 
- Bounce (not Bounce2) is listed as a dependency.

2.4.0
- The .4. is for the added inputRange() and setWaitTime() functions
- This update also (actually) fixes that MIDIdrum bug preventing high velocity

2.3.4 
- Added inputRange() to MIDIdrum.
- Added an example for muxed input.
- Fixed a bug preventing MIDIdrum from sending velocity 127.

2.3.3 
- Made waitTime for MIDIdrum user-selectable.

2.3.2 
- Fixed the broken smooth() function for analog inputs.

2.3.1 
- All examples updated to prevent MIDI and usbMIDI stack crashes.

2.3.0 
- A major update to the Flicker library:
    - thresholds for Capacitive Touch buttons are automatically detected 
    - more stable expression control for MIDItouch(previously MIDIcapSens)
    - MIDIdrum includes option to use velocity-sensitive Capacitive Touch
- Even better stabilization of analog inputs
- Added option for encoders to change 1 CC value per detent
- Made "killSwitch" user-selectable (just put any CC# instead of KILL)
- Added setKillSwitch(byte) to set kill CC (use OFF or 0 to disable it).
- Added a few visual aides

2.2.5 
- Bugfixed jitter that occurred when using inputRange() with input maxed
- Arguments for specific velocities can now be passed to velocity inputs
- Added literals to highlight MOMENTARY, LATCH, TRIGGER and KILL modes
- Long overdue completion of the "Flicker" library (for Cap Touch input)
- Made separate examples for 'pot' and 'sensor' (to avoid confusion)

2.2.0 
- Added support for Piezos (Must be wired properly. See example)
- "MIDInote" class changed to "MIDIdrum" and optimized for FSR and Piezo
- Removed redundant 'velocity' variable. Just call outputRange(127, 127)
- Added support for using a Capacitive Touch input as a MIDIbutton
- Included 'Flicker' library (required for Capacitive Touch buttons)

2.1.5 
- got rid of useless '*MC' pointer. renamed 'kill' to 'mode'

2.1.3 
- included an example of how to implement aftertouch

2.1.2 
- many variables changed to 'byte' or 'uint16_t' for easy storage
- public and private variables are better sorted

2.1.0 
- Split read() and send() functions. MIDI channel is now user selectable

2.0.6 
- Fixed a bug preventing poly CC to return to zero after note off.

2.0.5 
- Added a condition to prevent many double note triggers.

2.0.4 
- Added support for capacitive sensors (and started this version log)
___

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
