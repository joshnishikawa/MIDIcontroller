#ifndef MIDItransport_h
#define MIDItransport_h

#include <Arduino.h>

// Interface types
#define MIDI_INTERFACE_USB    0
#define MIDI_INTERFACE_SERIAL 1
#define MIDI_INTERFACE_HOST   2
#define MIDI_INTERFACE_DEBUG  3

// MIDI Message Types (Status bytes / nibbles)
#ifndef MIDI_NOTE_OFF
#define MIDI_NOTE_OFF          0x80
#define MIDI_NOTE_ON           0x90
#define MIDI_AFTERTOUCH_POLY   0xA0
#define MIDI_CONTROL_CHANGE    0xB0
#define MIDI_PROGRAM_CHANGE    0xC0
#define MIDI_AFTERTOUCH_CHAN   0xD0
#define MIDI_PITCH_BEND        0xE0
#define MIDI_SYSTEM_EXCLUSIVE  0xF0
#define MIDI_TIME_CODE         0xF1
#define MIDI_SONG_POS          0xF2
#define MIDI_SONG_SELECT       0xF3
#define MIDI_TUNE_REQ          0xF6
#define MIDI_CLOCK             0xF8
#define MIDI_START             0xFA
#define MIDI_CONTINUE          0xFB
#define MIDI_STOP              0xFC
#define MIDI_ACTIVE_SENSING    0xFE
#define MIDI_SYS_RESET         0xFF
#endif

// Default global MIDI channel fallback (weak definition if not defined in sketch)
#ifndef MIDI_CHANNEL_OMNI
#define MIDI_CHANNEL_OMNI 0
#endif

#if defined(TEENSYDUINO) || defined(CORE_TEENSY)
  #define MIDI_PLATFORM_TEENSY 1
#endif

// Compatibility shim for non-Teensy platforms using usbMIDI syntax
#if !defined(MIDI_PLATFORM_TEENSY) && !defined(usbMIDI)
class USBMIDI_CompatibilityShim {
public:
  void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable = 0);
  void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable = 0);
  void sendControlChange(uint8_t control, uint8_t value, uint8_t channel, uint8_t cable = 0);
  void sendProgramChange(uint8_t program, uint8_t channel, uint8_t cable = 0);
  void sendPitchBend(int value, uint8_t channel, uint8_t cable = 0);
  void sendPolyPressure(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable = 0);
  void sendAfterTouch(uint8_t pressure, uint8_t channel, uint8_t cable = 0);
  void sendSysEx(uint32_t length, const uint8_t *data, bool hasTerm = true, uint8_t cable = 0);
  void sendRealTime(uint8_t type, uint8_t cable = 0);
  void send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable = 0);
  bool read(uint8_t channel = 0) { return false; }
};
extern USBMIDI_CompatibilityShim usbMIDI;
#endif

// Central Dispatch Function
void MIDI_send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, 
               uint8_t cable = 0, uint8_t interface = MIDI_INTERFACE_USB, 
               const uint8_t *sysex = NULL, uint16_t sysexLen = 0);

// Global Helper Functions
void MIDI_setup(unsigned long baud = 115200);
void MIDI_loop(void);

// Custom Transport Callback Hook
typedef void (*MIDICustomSendCallback)(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable);
void MIDI_setCustomSendCallback(MIDICustomSendCallback cb);

#endif
