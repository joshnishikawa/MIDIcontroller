#include "MIDIswitch.h"

// constructors
MIDIswitch::MIDIswitch() : Bounce(){};

MIDIswitch::MIDIswitch(int p, uint8_t num) : Bounce(){
  pinMode(p, INPUT_PULLUP);
  Bounce::attach(p);
  Bounce::interval(10);
  number = num;
  numberS = num;

  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      outHi = num;
      mode = TRIGGER;
      break;
    default:
      realTime = false;
      outHi = 127;
      mode = MOMENTARY;
  }
}

#if defined(HAS_CAPACITIVE_TOUCH)
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t x) : Bounce(), TouchSwitch(p, 0){
#else
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t x) : Bounce(){
#endif

  number = num;
  numberS = num;

  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      outHi = num;
      mode = TRIGGER; // 'x' takes precedence if it indicates a different mode
      break;
    default:
      realTime = false;
  }

  switch (x){
    case BINARY: // Bounce object by default
      break;
    case TOUCH:
      inputType = TOUCH;
      break;
    case MOMENTARY: case LATCH: case TRIGGER: case NOTE: case DRUM:
      mode = x;
      break;
  }

  if (inputType == BINARY) {
    pinMode(p, INPUT_PULLUP);
    Bounce::attach(p);
    Bounce::interval(10);
  }
}

#if defined(HAS_CAPACITIVE_TOUCH)
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t m, uint8_t t) : Bounce(), TouchSwitch(p, 0){
#else
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t m, uint8_t t) : Bounce(){
#endif

  number = num;
  numberS = num;

  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      outHi = num;
      break;
    default:
      realTime = false;
  }

  if (m == BINARY || m == TOUCH || t == BINARY || t == TOUCH) {
    switch (m){
      case MOMENTARY: case LATCH: case TRIGGER: case NOTE: case DRUM:
        mode = m;
        break;
      case BINARY: case TOUCH:
        inputType = m;
        break;
    }

    switch (t){
      case MOMENTARY: case LATCH: case TRIGGER: case NOTE: case DRUM:
        mode = t;
        break;
      case BINARY: case TOUCH:
        inputType = t;
        break;
    }
  } else {
    // Neither is BINARY or TOUCH -> treated as (pin, num, numS, mode)
    numberS = m;
    mode = t;
  }

  if (inputType == BINARY) {
    pinMode(p, INPUT_PULLUP);
    Bounce::attach(p);
    Bounce::interval(10);
  }
}

#if defined(HAS_CAPACITIVE_TOUCH)
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t numS, uint8_t m, uint8_t t) : Bounce(), TouchSwitch(p, 0){
#else
MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t numS, uint8_t m, uint8_t t) : Bounce(){
#endif
  number = num;
  numberS = numS;
  mode = m;
  inputType = t;

  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      outHi = num;
      break;
    default:
      realTime = false;
  }

  if (inputType == BINARY) {
    pinMode(p, INPUT_PULLUP);
    Bounce::attach(p);
    Bounce::interval(10);
  }
}

// destructor
MIDIswitch::~MIDIswitch(){};

#if defined(HAS_CAPACITIVE_TOUCH)
void MIDIswitch::setThreshold(){
  TouchSwitch::setThreshold();
}

void MIDIswitch::setThreshold(int threshold){
  TouchSwitch::setThreshold(threshold);
}
#endif

int MIDIswitch::read(){
  if (inputType == BINARY){
    Bounce::update();          // Force a status report of the Bounce object.
    inputState = Bounce::read();
    if (Bounce::fell()){       // If the button's been pressed,
      return outHi;            // return the high CC value.
    }
    else if (Bounce::rose()){  // If the button has been released,
      return outLo;            // return the low CC value.
    }
    else{ return -1; }
  }
#if defined(HAS_CAPACITIVE_TOUCH)
  else if (inputType == TOUCH){
    TouchSwitch::update();
    inputState = TouchSwitch::read();
    if (TouchSwitch::rose()){
      return outHi;
    }
    else if (TouchSwitch::fell()){
      return outLo;
    }
    else{ return -1; }
  }
#endif
  else{ return -1; }
}

void MIDIswitch::setChannel(byte chan, byte cable, byte iface){
  this->channel = chan;
  this->cable = cable;
  this->interface = iface;
}

void MIDIswitch::setOnMessage(byte type, byte data1, byte data2){
  this->customMessage = true;
  this->onType = type;
  this->onData1 = data1;
  this->onData2 = data2;
}

void MIDIswitch::setOffMessage(byte type, byte data1, byte data2){
  this->customMessage = true;
  this->offType = type;
  this->offData1 = data1;
  this->offData2 = data2;
}

/* This function will send the appropriate Control Change or Real Time messages
   for the press and/or release of any MIDI button whether it's set to 
   'MOMENTARY' 'LATCH' or 'TRIGGER' mode.*/
int MIDIswitch::send(int shiftState){
  int newValue = read();
  uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
  uint8_t activeNum = (shiftState == 1 && numberS != 0) ? numberS : number;

  if (newValue == outHi){       // If the button's been pressed,
    if (state == false){        // and if it was latched OFF,
      if (customMessage && onType != 0){
        MIDI_send(onType, onData1, onData2, targetChan, cable, interface);
      }
      else if (realTime) {
        MIDI_send(outHi, 0, 0, targetChan, cable, interface);
      }
      else if (mode == NOTE || mode == DRUM) {
        MIDI_send(MIDI_NOTE_ON, activeNum, outHi, targetChan, cable, interface);
      }
      else {
        MIDI_send(MIDI_CONTROL_CHANGE, activeNum, outHi, targetChan, cable, interface); // send CC outHi
      }
      timer = 0;
      state = true;             // Remember the button is now on.
      return realTime ? 1 : activeNum;
    }
    else{                       // If the button was latched ON,
      if (mode == TRIGGER || mode == DRUM){     // and the button's in TRIGGER mode
        if (customMessage && onType != 0){
          MIDI_send(onType, onData1, onData2, targetChan, cable, interface);
        }
        else if (realTime) {
          MIDI_send(outHi, 0, 0, targetChan, cable, interface);
        }
        else if (mode == DRUM) {
          MIDI_send(MIDI_NOTE_ON, activeNum, outHi, targetChan, cable, interface);
        }
        else {
          MIDI_send(MIDI_CONTROL_CHANGE, activeNum, outHi, targetChan, cable, interface); // send CC outHi again
        }
        return realTime ? 1 : activeNum;
      }
      else {
        if (customMessage && offType != 0){
          MIDI_send(offType, offData1, offData2, targetChan, cable, interface);
        }
        else if (!realTime) {
          if (mode == NOTE) {
            MIDI_send(MIDI_NOTE_OFF, activeNum, outLo, targetChan, cable, interface);
          }
          else {
            MIDI_send(MIDI_CONTROL_CHANGE, activeNum, outLo, targetChan, cable, interface); // send CC outLo
          }
        }
        state = false;            // Remember the button is now off.
        return outLo;
      }
    }
  }
  else if (newValue == outLo && (mode == MOMENTARY || mode == NOTE)){ // MOMENTARY released?
    if (customMessage && offType != 0){
      MIDI_send(offType, offData1, offData2, targetChan, cable, interface);
    }
    else if (!realTime) {
      if (mode == NOTE) {
        MIDI_send(MIDI_NOTE_OFF, activeNum, outLo, targetChan, cable, interface);
      }
      else {
        MIDI_send(MIDI_CONTROL_CHANGE, activeNum, outLo, targetChan, cable, interface); // send CC outLo
      }
    }
    state = false;                         // Remember the button is now off
    return outLo;
  }
  else if (state == true && mode == DRUM && timer > 50){
    state = false;
    return -1;
  }
  else return -1;
}

int MIDIswitch::send(bool force){
  if (force){
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    if (state){
      if (realTime) MIDI_send(outHi, 0, 0, targetChan, cable, interface);
      else MIDI_send(MIDI_CONTROL_CHANGE, number, outHi, targetChan, cable, interface);
      return outHi;
    } else {
      if (realTime) MIDI_send(outHi, 0, 0, targetChan, cable, interface); // ignore state for Real Time
      else MIDI_send(MIDI_CONTROL_CHANGE, number, outLo, targetChan, cable, interface);
      return outLo;
    }
  } 
  else { return -1; }
}

// sets the state of a LATCH input
void MIDIswitch::write(bool s){
  state = s;
}

// Set the CC number.
void MIDIswitch::setControlNumber(byte num){
  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      mode = TRIGGER;
      number = numberS = outHi = num;
      outLo = 0;
      break;
    default:
      realTime = false;
      number = num;
      numberS = num;
      switch (outHi){
        case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
          outHi = 127; // reset to default if Real Time message previously used
          break;
      }
  }
}

void MIDIswitch::setControlNumber(byte num, byte numS){
  setControlNumber(num);
  numberS = numS;
}

// Set specific min and max values.
void MIDIswitch::outputRange(byte min, byte max){
  outLo = constrain(min, 0, 127);
  outHi = constrain(max, 0, 127);
}

// Set the button mode.
void MIDIswitch::setMode(byte mod){
  mode = constrain(mod, 0, 2);
}
