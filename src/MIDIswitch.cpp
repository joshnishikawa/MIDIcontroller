#include "MIDIswitch.h"

// constructors
MIDIswitch::MIDIswitch() : Bounce(0, 0), TouchSwitch(0, 0){};

MIDIswitch::MIDIswitch(int p, uint8_t num) : Bounce(p, 10), TouchSwitch(0, 0){
  pinMode(p, INPUT_PULLUP);
  number = num;

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


MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t x) : Bounce(p, 10), TouchSwitch(p, 0){
  number = num;

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
    case MOMENTARY: case LATCH: case TRIGGER:
      mode = x;
      break;
  }

  if (inputType == BINARY) pinMode(p, INPUT_PULLUP);
}


MIDIswitch::MIDIswitch(int p, uint8_t num, uint8_t m, uint8_t t) : Bounce(0, 0), TouchSwitch(p, 0){
  number = num;

  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      realTime = true;
      outHi = num;
      break;

    default:
      realTime = false;
  }

  switch (m){
    case MOMENTARY: case LATCH: case TRIGGER:
      mode = m;
      break;
    case BINARY: case TOUCH:
      inputType = m;
      break;
  }

  switch (t){
    case MOMENTARY: case LATCH: case TRIGGER:
      mode = t;
      break;
    case BINARY: case TOUCH:
      inputType = t;
      break;
  }

  if (inputType == BINARY) pinMode(p, INPUT_PULLUP);
}


// destructor
MIDIswitch::~MIDIswitch(){};


void MIDIswitch::setThreshold(){
  TouchSwitch::setThreshold();
}

void MIDIswitch::setThreshold(int threshold){
  TouchSwitch::setThreshold(threshold);
}


int MIDIswitch::read(){
  if (inputType == BINARY){
    Bounce::update();          // Force a status report of the Bounce object.
    inputState = Bounce::state;
    if (Bounce::fell()){       // If the button's been pressed,
      return outHi;            // return the high CC value.
    }
    else if (Bounce::rose()){  // If the button has been released,
      return outLo;            // return the low CC value.
    }
    else{ return -1; }
  }
#if ! defined(__IMXRT1062__)
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
};



/* This function will send the appropriate Control Change or Real Time messages
   for the press and/or release of any MIDI button whether it's set to 
   'MOMENTARY' 'LATCH' or 'TRIGGER' mode.*/
int MIDIswitch::send(){
  int newValue = read();

  if (newValue == outHi){       // If the button's been pressed,
    if (state == false){        // and if it was latched OFF,
      if (realTime) usbMIDI.sendRealTime(outHi);
      else usbMIDI.sendControlChange(number,outHi,MIDIchannel); // send CC outHi,
      state = true;             // Remember the button is now on.
      return realTime ? 1 : number;
    }
    else{                       // If the button was latched ON,
      if (mode == TRIGGER){     // and the button's in TRIGGER mode,
        if (realTime) usbMIDI.sendRealTime(outHi);
        else usbMIDI.sendControlChange(number,outHi,MIDIchannel); // send CC outHi again
        return realTime ? 1 : number;
      }
      else {
        if (!realTime) usbMIDI.sendControlChange(number,outLo,MIDIchannel); // send CC outLo,
        state = false;            // Remember the button is now off.
        return outLo;
      }
    }
  }
  else if (newValue == outLo && mode == MOMENTARY){ // MOMENTARY released?
    if (!realTime) usbMIDI.sendControlChange(number,outLo,MIDIchannel); // send CC outLo,
    state = false;                         // Remember the button is now off
    return outLo;
  }

  else return -1;
};


int MIDIswitch::send(bool force){
  if (force){
    if (state){
      if (realTime) usbMIDI.sendRealTime(outHi);
      else usbMIDI.sendControlChange(number,outHi,MIDIchannel);
      return outHi;
    } else {
      if (realTime) usbMIDI.sendRealTime(outHi); // ignore state for Real Time
      else usbMIDI.sendControlChange(number,outLo,MIDIchannel);
      return outLo;
    }
  } 
  else { return -1; }
}


// Set the CC number.
void MIDIswitch::setControlNumber(byte num){
  switch (num){
    case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
      number = outHi = num;
      outLo = 0;
      break;
    
    default:
      number = num;
      switch (outHi){
        case START: case STOP: case CONTINUE: case CLOCK: case SYSTEM_RESET:
          outHi = 127; // reset to default if Real Time message previously used
          break;
      }
  }
};


// Set specific min and max values.
void MIDIswitch::outputRange(byte min, byte max){
  outLo = constrain(min, 0, 127);
  outHi = constrain(max, 0, 127);
};


// Set the button mode.
void MIDIswitch::setMode(byte mod){
  mode = constrain(mod, 0, 2);
};
