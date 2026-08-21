# Consolidated Implementation Plan: Multi-Board & Multi-Interface MIDIcontroller

Comprehensive plan unifying **cross-microcontroller support**, **multi-interface routing**, **the `MIDIencMulti` multi-mode encoder component**, **encoder detent bugfixes**, **MIDI reference documentation**, and **community acknowledgements**.

---

## 1. Objectives & Architectural Highlights

1. **Universal Hardware Compatibility**:
   - Seamless support across **Teensy (all models), ESP32, ESP32-S3, RP2040, Arduino AVR (Uno/Mega/Leonardo/Micro), SAMD, and STM32**.
   - Preserves 100% backward compatibility with existing Teensy code. Outgoing MIDI automatically bridges to Teensy `usbMIDI`, native USB-MIDI / `Adafruit_TinyUSB`, or standard 47effects `MIDI.h`.
2. **Multi-Interface Routing & Virtual Cables**:
   - **`MIDI_INTERFACE_USB` (0)**: Native USB MIDI with virtual cable support (Cables 0–15 for `usbMIDIx4` / `usbMIDIx16`).
   - **`MIDI_INTERFACE_SERIAL` (1)**: Hardware 5-pin DIN / 3.5mm Serial MIDI (via `Serial1` or user-defined `HardwareSerial`).
   - **`MIDI_INTERFACE_HOST` (2)**: USB Host MIDI (e.g. Teensy 3.6 / 4.1 USB Host).
   - **`MIDI_INTERFACE_DEBUG` (3)**: Human-readable Serial Monitor output with timestamps and HEX payload data.
3. **Per-Control Configuration & Custom Messages**:
   - Controls support `.setChannel(channel, cable, interface)` with global `MIDIchannel` fallback by default.
   - `MIDIbutton` supports `.setOnMessage(type, data1, data2)` & `.setOffMessage(...)` to send Note On/Off, Program Change, etc., in addition to default CC.
4. **`MIDIencMulti` Multi-Purpose Encoder**:
   - Runtime configuration: `initAsEncoder(...)` (absolute CC), `initAsJog1Cc(...)` (single-CC relative jog), and `initAsJog2Cc(...)` (dual-CC relative jog).
   - Built-in `shiftState` handling for secondary CCs on shifted layers.
5. **`MIDIenc` Detent Bugfix**:
   - Accumulates intermediate sub-detent pulses properly on detented encoders (`PER_DETENT = 4`) without premature accumulator resets.
6. **Built-in MIDI Reference Tables**:
   - General MIDI percussion note mapping and Standard MIDI CC assignment charts in `README.md`.
7. **Community Acknowledgements & Credits**:
   - Highlight the contributions and ideas from **JukkaPVK**, **PatternAgents (Tom Moxon)**, and **Drc3p0** in `README.md`.

---

## 2. Component-by-Component Changes

### Transport Layer & Core Architecture

#### [NEW] [MIDItransport.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDItransport.h)
- Interface constants: `MIDI_INTERFACE_USB`, `MIDI_INTERFACE_SERIAL`, `MIDI_INTERFACE_HOST`, `MIDI_INTERFACE_DEBUG`.
- Central dispatcher: `MIDI_send(byte type, byte data1, byte data2, byte channel, byte cable = 0, byte interface = MIDI_INTERFACE_USB, const uint8_t *sysex = NULL, uint16_t sysexLen = 0)`.
- Global helpers: `MIDI_setup()` and `MIDI_loop()`.

#### [MODIFY] [MIDIcontroller.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIcontroller.h)
- Include `MIDItransport.h`, all control headers, and `MIDIencMulti.h`.
- Provide weak default `byte MIDIchannel = 1;` for out-of-the-box sketch compilation.

#### [MODIFY] [library.properties](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/library.properties)
- Set `architectures=*` and update description.

#### [MODIFY] [README.md](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/README.md)
- Complete API documentation for multi-interface routing, `MIDIencMulti`, and per-instance configuration.
- Quick-reference tables for Standard MIDI CCs and GM Drum Note Numbers.
- **Acknowledgements & Credits Section**:
  - **[JukkaPVK](https://github.com/JukkaPVK)**: For designing the `MIDIencMulti` multipurpose encoder class, detent bugfixes, and shifted CC concepts.
  - **[PatternAgents](https://github.com/PatternAgents) (Tom Moxon)**: For the multi-interface routing architecture (USB/Serial/Host/Debug), virtual USB cable support, and per-instance channel/message design.
  - **[Drc3p0](https://github.com/Drc3p0)**: For early multi-interface contributions and standard MIDI / General MIDI drum reference charts.

---

### Control Components

#### [MODIFY] [MIDIbutton.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIbutton.h) & [MIDIbutton.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIbutton.cpp)
- Add `.setChannel(chan, cable, iface)`, `.setOnMessage(type, d1, d2)`, `.setOffMessage(type, d1, d2)`, and `send(int shiftState = 0)`.
- Route through `MIDI_send(...)`.

#### [MODIFY] [MIDIpot.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIpot.h) & [MIDIpot.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIpot.cpp)
- Add `.setChannel(chan, cable, iface)` and route through `MIDI_send(...)`.

#### [MODIFY] [MIDIenc.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIenc.h) & [MIDIenc.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIenc.cpp)
- **Detent Bugfix**: Only clear `myKnob->write(0)` when full detent threshold is crossed.
- Add `.setChannel(...)` and `send(int shiftState = 0)`.

#### [NEW] [MIDIencMulti.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIencMulti.h) & [MIDIencMulti.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIencMulti.cpp)
- Multi-mode encoder class supporting `initAsEncoder`, `initAsJog1Cc`, `initAsJog2Cc`, and `shiftState`.

#### [MODIFY] [MIDIdrum.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIdrum.h) & [MIDIdrum.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDIdrum.cpp)
- Add `.setChannel(...)` and route through `MIDI_send(...)`.

#### [MODIFY] [MIDItouch.h](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDItouch.h) & [MIDItouch.cpp](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/src/MIDItouch.cpp)
- Enable capacitive touch sensing on Teensy and ESP32 (`touchRead`), add `.setChannel(...)`.

#### [MODIFY] [keywords.txt](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/keywords.txt)
- Add keywords for all new classes, methods, and constants.

---

### Examples

#### [NEW] [MIDIencMulti.ino](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/examples/MIDIencMulti/MIDIencMulti.ino)
- Demonstrates encoder, 1-CC jogwheel, and 2-CC jogwheel modes with button shift state.

#### [NEW] [MultiInterface_Routing.ino](file:///c:/Users/joshn/OneDrive/Code/Sketchbook/libraries/MIDIcontroller/examples/MultiInterface_Routing/MultiInterface_Routing.ino)
- Demonstrates simultaneous USB MIDI, Serial DIN MIDI, Note-On buttons, and Serial Monitor debugging.

---

## 3. Verification Plan

1. **Architecture & Header Verification**:
   - Validate preprocessor logic across Teensy, ESP32, AVR, RP2040, SAMD, and STM32 environments.
2. **Behavioral Testing**:
   - `MIDIbutton`: CC mode by default; Note On/Off when configured with `setOnMessage`/`setOffMessage`.
   - `MIDIenc`: Verify smooth multi-pulse detent stepping with `PER_DETENT = 4`.
   - `MIDIencMulti`: Verify Encoder, Jog1CC, and Jog2CC modes with unshifted and shifted calls.
   - Multi-Interface: Verify routing to USB, Serial DIN, and Debug monitor output.
