/* 
  This is an example of how to set up a capacitive touch
  sensor as a variable MIDI input. Any capacitive object connected
  to a 'touch' pin will work. If using metal, use a layer of 
  something non-conductive between the metal and your skin.
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;

#if defined(D0)
const int touchPin = D0; // XIAO ESP32-S3 (D0 is GPIO 1 / TOUCH1)
#elif defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)
const int touchPin = 1;  // ESP32-S3 / S2 touch pin (GPIO 1)
#elif defined(ARDUINO_ARCH_ESP32)
const int touchPin = 4;  // Classic ESP32 touch pin T0 (GPIO 4)
#elif defined(__AVR__)
const int touchPin = A0; // AVR analog pin
#else
const int touchPin = 0;  // Teensy TSI touch pin 0
#endif

/* MIDItouch parameters are:
      1) pin (required)
      2) CC number (required)
      3) a secondary on/off CC# (optional)
*/

MIDItouch myInput(touchPin, 60);
// MIDItouch myInput(touchPin, 60, 9); // CC# 9 OFF is sent when reading drops below threshold.
                                  // CC# 9 ON is sent when threshold is breached.

void setup(){
  MIDI_setup();
  myInput.inputRange(); // WARNING! If you touch the input during setup(), it won't work.
  // myInput.inputRange(70, 2100); // OR use the 'findTouchRange' example to find values to specify here

  // myInput.outputRange(20, 120); // Restrict output to 20 ~ 120
}

void loop(){
  MIDI_loop(); // Flushes incoming MIDI buffers

  myInput.send();
}
