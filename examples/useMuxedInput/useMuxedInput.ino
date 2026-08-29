/*
  useMuxedInput.ino

  Demonstrates reading multiple potentiometers or analog sensors through an
  analog multiplexer (e.g. 74HC4051 8-channel or 74HC4067 16-channel MUX).
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;

// Multiplexer address select pins (using digital pins 2, 3, 4 to avoid Serial RX/TX on pins 0/1)
#if defined(D0)
const int selector_pin_a = D0;
const int selector_pin_b = D1;
const int selector_pin_c = D2;
// const int selector_pin_d = D3; // Uncomment if using 16-channel MUX (74HC4067)
#else
const int selector_pin_a = 2;
const int selector_pin_b = 3;
const int selector_pin_c = 4;
// const int selector_pin_d = 5; // Uncomment if using 16-channel MUX (74HC4067)
#endif

const int mux_input_pin = A0; // Analog pin connected to MUX COM / Signal output
const int NUM_POTS = 8;       // Change to 16 if using 16-channel MUX

MIDIpot* Pots[NUM_POTS];

void setup(){
  MIDI_setup(); // Optional helper: initializes serial ports

  pinMode(selector_pin_a, OUTPUT);
  pinMode(selector_pin_b, OUTPUT);
  pinMode(selector_pin_c, OUTPUT);
  // pinMode(selector_pin_d, OUTPUT); // Uncomment if using 16-channel MUX

  for (int i = 0; i < NUM_POTS; i++) {
    Pots[i] = new MIDIpot(mux_input_pin, 12 + i); // Assign CC 12, 13, 14, ...
    Pots[i]->inputRange(15, 1000);                // Adjust for full physical reach
  }
}

void loop(){
  MIDI_loop(); // Flushes incoming MIDI buffers

  for (int i = 0; i < NUM_POTS; i++) {
    // Set MUX channel address
    // digitalWrite(selector_pin_d, (i & 8) ? HIGH : LOW); // Uncomment for 16-ch MUX
    digitalWrite(selector_pin_c, (i & 4) ? HIGH : LOW);
    digitalWrite(selector_pin_b, (i & 2) ? HIGH : LOW);
    digitalWrite(selector_pin_a, (i & 1) ? HIGH : LOW);

    // Give analog voltage a brief moment to settle on the ADC pin to avoid crosstalk
    delayMicroseconds(5);

    // Read and send CC for this channel
    Pots[i]->send();
  }
}
