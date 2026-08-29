#include "MIDIcontroller.h"

// Default global MIDI channel
byte MIDIchannel = 1;

// Hardware Pins (with portable fallbacks for AVR, Teensy, and ESP32)
#if defined(D0)
const int enc1PinA = D0;
const int enc1PinB = D1;
const int enc2PinA = D2;
const int enc2PinB = D3;
const int shiftButtonPin = D4;
#else
const int enc1PinA = 2;
const int enc1PinB = 3;
const int enc2PinA = 4;
const int enc2PinB = 5;
const int shiftButtonPin = 6;
#endif

// Create MIDIencMulti objects (can be dynamically configured in setup())
MIDIencMulti enc1; // Standard absolute CC encoder
MIDIencMulti enc2; // Multi-mode relative jogwheel encoder

// Shift button: when held down, encoders send on their secondary/shifted CC numbers
MIDIbutton shiftBtn(shiftButtonPin, 20, MOMENTARY);

void setup() {
  MIDI_setup(); // Optional helper: initializes serial ports

  // Mode 0: Standard absolute CC encoder (CC 21 unshifted, CC 22 when shifted, range 0~127)
  enc1.initAsEncoder(enc1PinA, enc1PinB, 21, 22, 0, 127, PER_DETENT);

  // Mode 2: Dual-CC Jogwheel (CC 30 for Turn Left / CCW, CC 31 for Turn Right / CW)
  // Shifted: CC 32 for Turn Left, CC 33 for Turn Right
  enc2.initAsJog2Cc(enc2PinA, enc2PinB, 30, 31, 32, 33, PER_DETENT);

  // Note: For Single-CC Jogwheel (Mode 1), you could use:
  // enc2.initAsJog1Cc(enc2PinA, enc2PinB, 30, 31, 1, 127, PER_DETENT);
}

void loop() {
  MIDI_loop(); // Flushes incoming MIDI buffers

  // Read shift button state (INPUT_PULLUP is active LOW)
  shiftBtn.read();
  int isShifted = (shiftBtn.inputState == LOW) ? 1 : 0; // 1 if held down, 0 if released

  // Send encoder messages with current shift state
  enc1.send(isShifted);
  enc2.send(isShifted);
}
