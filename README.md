# MIDIcontroller 3.3.0
### A cross-platform library for building USB, Serial DIN, and Multi-Interface MIDI controllers.
###### by Josh Nishikawa <https://github.com/joshnishikawa/MIDIcontroller>

---

## Features
- **Cross-Platform Compatibility**: Native performance on **Teensy** (all models) with automatic compatibility across **ESP32, ESP32-S3, RP2040, Arduino AVR (Uno/Mega/Leonardo/Micro), SAMD, and STM32**.
- **Multi-Interface Routing & Virtual Cables**:
  - `MIDI_INTERFACE_USB` (0): Native USB-MIDI with virtual cable support (Cables 0–15 for `usbMIDIx4` / `usbMIDIx16`).
  - `MIDI_INTERFACE_SERIAL` (1): Hardware 5-pin DIN / 3.5mm Serial MIDI.
  - `MIDI_INTERFACE_HOST` (2): USB Host MIDI.
  - `MIDI_INTERFACE_DEBUG` (3): Formatted Serial Monitor debug output.
- **Flexible Controls**:
  - **`MIDIswitch` / `MIDIbutton`**: Momentary, latch, trigger, or Note On/Off buttons (works with physical buttons and capacitive touch).
  - **`MIDIpot`**: Stable analog-to-MIDI potentiometer/fader conversion with noise filtering and optional kill-switch.
  - **`MIDIenc` & `MIDIencMulti`**: Continuous CC encoders and multi-mode relative jogwheels (1-CC and 2-CC modes) with sub-detent filtering and shift states.
  - **`MIDIdrum`**: Velocity-sensitive FSR or Piezo drum trigger inputs with peak detection.
  - **`MIDItouch`**: Capacitive touch ribbon/slider expression control.
- **Per-Control Configuration**:
  - Assign individual channels, virtual cables, and output interfaces per control via `.setChannel(chan, cable, iface)`.
  - Customize button message types via `.setOnMessage(type, data1, data2)` and `.setOffMessage(...)`.

---

## Requirements & Dependencies
- **[MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)** (47effects)
- **[Bounce2](https://github.com/thomasfredericks/Bounce2)**
- **[Encoder](https://github.com/PaulStoffregen/Encoder)**
- **[Flicker](https://github.com/joshnishikawa/Flicker)** (Required only if using capacitive touch pins on supported hardware)

---

## Reference Charts

### Standard MIDI Continuous Controller (CC) Map
| CC Number | Description / Typical Assignment |
| :--- | :--- |
| **0** | Bank Select (MSB) |
| **1** | Modulation Wheel |
| **2** | Breath Controller |
| **4** | Foot Controller |
| **5** | Portamento Time |
| **7** | Channel Volume (Main Volume) |
| **8** | Balance |
| **10** | Pan Position |
| **11** | Expression Controller |
| **16–19** | General Purpose Sliders 1–4 |
| **64** | Sustain / Damper Pedal (On/Off) |
| **65** | Portamento (On/Off) |
| **66** | Sostenuto (On/Off) |
| **67** | Soft Pedal (On/Off) |
| **68** | Legato Footswitch |
| **70** | Sound Variation |
| **71** | Resonance / Timbre |
| **72** | Release Time |
| **73** | Attack Time |
| **74** | Brightness / Filter Cutoff |
| **80–83** | General Purpose Buttons 1–4 (On/Off) |
| **91** | Reverb Send Level |
| **92** | Tremolo Level |
| **93** | Chorus Send Level |
| **120** | All Sound Off |
| **121** | Reset All Controllers |
| **123** | All Notes Off |

---

### General MIDI Percussion / Drum Map (Channel 10)
| Note # | Key | Instrument Name | Note # | Key | Instrument Name |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **35** | B0 | Acoustic Bass Drum | **51** | D#2 | Ride Cymbal 1 |
| **36** | C1 | Bass Drum 1 | **52** | E2 | Chinese Cymbal |
| **37** | C#1 | Side Stick | **53** | F2 | Ride Bell |
| **38** | D1 | Acoustic Snare | **54** | F#2 | Tambourine |
| **39** | D#1 | Hand Clap | **55** | G2 | Splash Cymbal |
| **40** | E1 | Electric Snare | **56** | G#2 | Cowbell |
| **41** | F1 | Low Floor Tom | **57** | A2 | Crash Cymbal 2 |
| **42** | F#1 | Closed Hi-Hat | **58** | A#2 | Vibraslap |
| **43** | G1 | High Floor Tom | **59** | B2 | Ride Cymbal 2 |
| **44** | G#1 | Pedal Hi-Hat | **60** | C3 | Hi Bongo |
| **45** | A1 | Low Tom | **61** | C#3 | Low Bongo |
| **46** | A#1 | Open Hi-Hat | **62** | D3 | Mute Hi Conga |
| **47** | B1 | Low-Mid Tom | **63** | D#3 | Open Hi Conga |
| **48** | C2 | Hi-Mid Tom | **64** | E3 | Low Conga |
| **49** | C#2 | Crash Cymbal 1 | **65** | F3 | High Timbale |
| **50** | D2 | High Tom | **66** | F#3 | Low Timbale |

---

## Acknowledgements & Community Credits
Special thanks to the contributors whose forks and ideas helped shape this release:
- **[JukkaPVK](https://github.com/JukkaPVK)**: Created the `MIDIencMulti` multipurpose encoder class, detent accumulator bugfix, and shifted CC concepts.
- **[PatternAgents](https://github.com/PatternAgents) (Tom Moxon)**: Designed the multi-interface routing architecture (USB/Serial/Host/Debug), virtual USB cable support, and per-instance channel/message configuration.
- **[Drc3p0](https://github.com/Drc3p0)**: Early multi-interface explorations and the General MIDI drum note and Standard CC reference tables.

---

## Version Log
### 3.3.0
- **Cross-Platform Support**: Added universal architecture support for Teensy, ESP32, ESP32-S3, RP2040, Arduino AVR, SAMD, and STM32.
- **Multi-Interface Routing**: Added `MIDItransport` routing engine supporting USB, Serial DIN (`Serial1`), USB Host, and Serial Monitor Debug (`MIDI_INTERFACE_DEBUG`).
- **`MIDIencMulti` Component**: Added multi-mode encoder supporting absolute CC, 1-CC relative jogwheel, 2-CC relative jogwheel, and shift states.
- **Encoder Detent Bugfix**: Resolved accumulator clearing bug so `PER_DETENT = 4` stepping functions smoothly.
- **Per-Control Configuration**: Added `.setChannel(chan, cable, iface)` across all controls, plus `.setOnMessage()` and `.setOffMessage()` for buttons.
- **Reference Documentation**: Added Standard MIDI CC and General MIDI Drum Map reference tables.

---

## License
MIT License. Copyright (c) Josh Nishikawa.
