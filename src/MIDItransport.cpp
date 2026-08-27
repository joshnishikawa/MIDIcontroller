#include "MIDItransport.h"

// Weak default for global MIDIchannel so sketches compile even if omitted
byte MIDIchannel __attribute__((weak)) = 1;

static MIDICustomSendCallback customSendCallback = NULL;

void MIDI_setCustomSendCallback(MIDICustomSendCallback cb) {
  customSendCallback = cb;
}

#if !defined(MIDI_PLATFORM_TEENSY) && !defined(usbMIDI)
USBMIDI_CompatibilityShim usbMIDI;

void USBMIDI_CompatibilityShim::sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_NOTE_ON, note, velocity, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_NOTE_OFF, note, velocity, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendControlChange(uint8_t control, uint8_t value, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_CONTROL_CHANGE, control, value, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendProgramChange(uint8_t program, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_PROGRAM_CHANGE, program, 0, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendPitchBend(int value, uint8_t channel, uint8_t cable) {
  uint8_t d1 = value & 0x7F;
  uint8_t d2 = (value >> 7) & 0x7F;
  MIDI_send(MIDI_PITCH_BEND, d1, d2, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendPolyPressure(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_AFTERTOUCH_POLY, note, pressure, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendAfterTouch(uint8_t pressure, uint8_t channel, uint8_t cable) {
  MIDI_send(MIDI_AFTERTOUCH_CHAN, pressure, 0, channel, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::sendSysEx(uint32_t length, const uint8_t *data, bool hasTerm, uint8_t cable) {
  MIDI_send(MIDI_SYSTEM_EXCLUSIVE, 0, 0, 0, cable, MIDI_INTERFACE_USB, data, length);
}

void USBMIDI_CompatibilityShim::sendRealTime(uint8_t type, uint8_t cable) {
  MIDI_send(type, 0, 0, 0, cable, MIDI_INTERFACE_USB);
}

void USBMIDI_CompatibilityShim::send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable) {
  MIDI_send(type, data1, data2, channel, cable, MIDI_INTERFACE_USB);
}

bool USBMIDI_CompatibilityShim::read(uint8_t channel) {
  ensureEsp32USBStarted();
  return false;
}
#endif

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_TINYUSB_ENABLED)
#include "USB.h"
#include "USBMIDI.h"
static USBMIDI esp32USBMIDI;
static bool esp32USBStarted = false;

void ensureEsp32USBStarted() {
  if (!esp32USBStarted) {
    esp32USBStarted = true;
    esp32USBMIDI.begin();
    USB.begin();
  }
}
#else
void ensureEsp32USBStarted() {}
#endif

void MIDI_send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, 
               uint8_t cable, uint8_t interface, 
               const uint8_t *sysex, uint16_t sysexLen) {
  
  if (customSendCallback != NULL) {
    customSendCallback(type, data1, data2, channel, cable);
  }

  switch (interface) {
    case MIDI_INTERFACE_DEBUG:
      Serial.print(F("[MIDI DEBUG] Ch:"));
      Serial.print(channel);
      Serial.print(F(" Type:0x"));
      if (type < 16) Serial.print('0');
      Serial.print(type, HEX);
      Serial.print(F(" D1:"));
      Serial.print(data1);
      Serial.print(F(" D2:"));
      Serial.print(data2);
      Serial.print(F(" Cable:"));
      Serial.println(cable);
      break;

    case MIDI_INTERFACE_USB:
#if defined(MIDI_PLATFORM_TEENSY)
      // Normalize type nibble if channel was included in status
      if (type >= 0x80 && type < 0xF0) {
        uint8_t msgType = type & 0xF0;
        switch (msgType) {
          case MIDI_NOTE_OFF:
            usbMIDI.sendNoteOff(data1, data2, channel, cable);
            break;
          case MIDI_NOTE_ON:
            usbMIDI.sendNoteOn(data1, data2, channel, cable);
            break;
          case MIDI_AFTERTOUCH_POLY:
            usbMIDI.sendPolyPressure(data1, data2, channel, cable);
            break;
          case MIDI_CONTROL_CHANGE:
            usbMIDI.sendControlChange(data1, data2, channel, cable);
            break;
          case MIDI_PROGRAM_CHANGE:
            usbMIDI.sendProgramChange(data1, channel, cable);
            break;
          case MIDI_AFTERTOUCH_CHAN:
            usbMIDI.sendAfterTouch(data1, channel, cable);
            break;
          case MIDI_PITCH_BEND:
            usbMIDI.sendPitchBend(data1 | (data2 << 7), channel, cable);
            break;
          default:
            usbMIDI.send(msgType, data1, data2, channel, cable);
            break;
        }
      } else if (type >= 0xF8) {
        usbMIDI.sendRealTime(type, cable);
      } else if (type == MIDI_SYSTEM_EXCLUSIVE && sysex != NULL && sysexLen > 0) {
        usbMIDI.sendSysEx(sysexLen, sysex, true, cable);
      } else {
        usbMIDI.send(type, data1, data2, channel, cable);
      }
#elif defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_TINYUSB_ENABLED)
      ensureEsp32USBStarted();
      if (type >= 0x80 && type < 0xF0) {
        uint8_t msgType = type & 0xF0;
        switch (msgType) {
          case MIDI_NOTE_OFF:
            esp32USBMIDI.noteOff(data1, data2, channel);
            break;
          case MIDI_NOTE_ON:
            esp32USBMIDI.noteOn(data1, data2, channel);
            break;
          case MIDI_CONTROL_CHANGE:
            esp32USBMIDI.controlChange(data1, data2, channel);
            break;
          case MIDI_PROGRAM_CHANGE:
            esp32USBMIDI.programChange(data1, channel);
            break;
          case MIDI_AFTERTOUCH_POLY:
            esp32USBMIDI.polyPressure(data1, data2, channel);
            break;
          case MIDI_AFTERTOUCH_CHAN:
            esp32USBMIDI.channelPressure(data1, channel);
            break;
          case MIDI_PITCH_BEND:
            esp32USBMIDI.pitchBend((int16_t)(data1 | (data2 << 7)), channel);
            break;
          default:
            break;
        }
      } else if (type >= 0xF8) {
        esp32USBMIDI.write(type);
      }
#endif
      break;

    case MIDI_INTERFACE_SERIAL:
      // On Serial MIDI, format raw 31250 baud MIDI or bridge if HardwareSerial configured
      #if defined(HAVE_HWSERIAL1) || defined(Serial1)
      if (type >= 0x80 && type < 0xF0) {
        uint8_t status = (type & 0xF0) | ((channel - 1) & 0x0F);
        Serial1.write(status);
        Serial1.write(data1 & 0x7F);
        if ((type & 0xF0) != MIDI_PROGRAM_CHANGE && (type & 0xF0) != MIDI_AFTERTOUCH_CHAN) {
          Serial1.write(data2 & 0x7F);
        }
      } else if (type >= 0xF8) {
        Serial1.write(type);
      }
      #endif
      break;

    case MIDI_INTERFACE_HOST:
      // Reserved for USB Host MIDI (e.g. Teensy USBHost_t36)
      break;

    default:
      break;
  }
}

void MIDI_setup(unsigned long baud) {
  ensureEsp32USBStarted();
  Serial.begin(baud);
#if defined(HAVE_HWSERIAL1) || defined(Serial1)
  Serial1.begin(31250);
#endif
}

void MIDI_loop(void) {
  ensureEsp32USBStarted();
#if defined(MIDI_PLATFORM_TEENSY)
  while (usbMIDI.read()) {}
#endif
#if defined(HAVE_HWSERIAL1) || defined(Serial1)
  while (Serial1.available() > 0) {
    Serial1.read();
  }
#endif
}
