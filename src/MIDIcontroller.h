#ifndef MIDIcontroller_h
#define MIDIcontroller_h

#include <Arduino.h>
#include "MIDIswitch.h"
#include "MIDIpot.h"
#include "MIDIenc.h"
#include "MIDIdrum.h"

// Include the MIDItouch class only if the board has capacitive touch pins
//            Teensy 3.6                Teensy 3.2                Teensy 3.0                Teensy LC
#if defined(__MK66FX1M0__) || defined(__MK20DX256__) || defined(__MK20DX128__) || defined(__MKL26Z64__)
  #include "MIDItouch.h"
#endif

#define FORCE true

// Bounce2, Encoder and Flicker libraries are also required
// github.com/joshnishikawa/Flicker

#endif
