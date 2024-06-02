#ifndef MIDIcontroller_h
#define MIDIcontroller_h

#include <Arduino.h>
#include "MIDIswitch.h"
#include "MIDIpot.h"
#include "MIDIenc.h"
#include "MIDIdrum.h"
#if ! defined(__IMXRT1062__)
  #include "MIDItouch.h"
#endif

#define FORCE true

// Bounce, Encoder and Flicker libraries are also required
// github.com/joshnishikawa/Flicker

#endif

