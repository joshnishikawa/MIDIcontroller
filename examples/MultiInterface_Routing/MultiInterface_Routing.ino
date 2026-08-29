#include "MIDIcontroller.h"

/*
  MultiInterface_Routing.ino

  Demonstrates per-control channel & destination interface routing:
  - Button 1: Sends standard USB MIDI Note On/Off (Middle C) on Channel 1
  - Button 2: Sends Serial DIN MIDI Note On/Off (D4) on Channel 2
  - Potentiometer 1: Sends USB MIDI Control Change #7 (Volume) on Channel 1
  - Potentiometer 2: Sends MIDI Debug messages to the Serial Monitor
*/

#if defined(D0)
const int btn1Pin = D0;
const int btn2Pin = D1;
#else
const int btn1Pin = 2;
const int btn2Pin = 3;
#endif
const int pot1Pin = A0;
const int pot2Pin = A1;

// Declare controls
MIDIbutton btnUsb(btn1Pin, 60, MOMENTARY);
MIDIbutton btnSerial(btn2Pin, 62, MOMENTARY);
MIDIpot potVolume(pot1Pin, 7);
MIDIpot potDebug(pot2Pin, 10);

void setup() {
  MIDI_setup(115200); // Initializes USB Serial at 115200 and Serial1 at 31250 baud

  // Configure Button 1 to send Note On / Note Off instead of CC
  btnUsb.setOnMessage(MIDI_NOTE_ON, 60, 127);   // Middle C, Velocity 127
  btnUsb.setOffMessage(MIDI_NOTE_OFF, 60, 0);   // Middle C, Velocity 0
  btnUsb.setChannel(1, 0, MIDI_INTERFACE_USB);  // Channel 1, Cable 0, USB

  // Configure Button 2 to send Note On / Note Off over Serial DIN MIDI (Serial1)
  btnSerial.setOnMessage(MIDI_NOTE_ON, 62, 127);
  btnSerial.setOffMessage(MIDI_NOTE_OFF, 62, 0);
  btnSerial.setChannel(2, 0, MIDI_INTERFACE_SERIAL); // Channel 2, Serial DIN

  // Configure Pot 1 for USB MIDI Channel 1
  potVolume.setChannel(1, 0, MIDI_INTERFACE_USB);

  // Configure Pot 2 for Serial Monitor Debug output
  potDebug.setChannel(1, 0, MIDI_INTERFACE_DEBUG);
}

void loop() {
  MIDI_loop(); // Flushes incoming MIDI buffers

  btnUsb.send();
  btnSerial.send();
  potVolume.send();
  potDebug.send();
}
