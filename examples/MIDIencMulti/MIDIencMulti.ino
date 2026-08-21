#include "MIDIcontroller.h"

// Default global MIDI channel
byte MIDIchannel = 1;

// Pins
const int enc1PinA = 2;
const int enc1PinB = 3;
const int enc2PinA = 4;
const int enc2PinB = 5;
const int shiftButtonPin = 6;

// Create MIDIencMulti objects (can be initialized during setup based on mode switches)
MIDIencMulti enc1; // Standard absolute CC encoder
MIDIencMulti enc2; // Relative jogwheel encoder

// Shift button: when held down, encoders will send on their alternate CC numbers
MIDIbutton shiftBtn(shiftButtonPin, 20, MOMENTARY);

void setup() {
  MIDI_setup(); // Optional helper: initializes serial ports

  // Configure enc1 as a standard absolute CC encoder (CC 21 unshifted, CC 22 shifted)
  enc1.initAsEncoder(enc1PinA, enc1PinB, 21, 22, 0, 127, PER_DETENT);

  // Configure enc2 as a dual-CC jogwheel (CC 30 for Turn Left, CC 31 for Turn Right)
  // Shifted: CC 32 for Turn Left, CC 33 for Turn Right
  enc2.initAsJog2Cc(enc2PinA, enc2PinB, 30, 31, 32, 33, PER_DETENT);
}

void loop() {
  MIDI_loop(); // Optional helper: drains incoming MIDI queues to avoid buffer overflow

  // Read shift button state
  shiftBtn.read();
  int isShifted = shiftBtn.inputState; // 1 if held down, 0 if released

  // Send encoder messages with current shift state
  enc1.send(isShifted);
  enc2.send(isShifted);
}
