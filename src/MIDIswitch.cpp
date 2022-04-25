#include "MIDIswitch.h"

// constructors
MIDIswitch::MIDIswitch() : Bounce(0, 0), TouchSwitch(0, 0){};

MIDIswitch::MIDIswitch(int p, byte num) : Bounce(p, 10), TouchSwitch(0, 0){
  pinMode(p, INPUT_PULLUP);
  inputType = 0; //button
  number = num;
  mode = 0; // momentary
  state = false;
};

MIDIswitch::MIDIswitch(int p, byte num, byte mode) : Bounce(p, 10), TouchSwitch(0, 0){
  pinMode(p, INPUT_PULLUP);
  inputType = 0; //button
  number = num;
  this->mode = mode; 
  state = false;
};

MIDIswitch::MIDIswitch(int p, byte num, byte mode, int type) : Bounce(0, 0), TouchSwitch(p, 0, type){
  if (type == 0){
    inputType = 0;
  }
  else if (type == 1){
    inputType = 1;
    // Creating objects during setup()? Well, you needn't setThreshold() again.
    setThreshold();
  }
  else{
    inputType = 1;
    // For backward compatability. Specifying a threshold in the constructor is 
    // deprecated. Please use TOUCH as the argument and setThreshold(). 
    TouchSwitch::setThreshold(type);
  }

  number = num;
  this->mode = mode;
  state = false;
};

// destructor
MIDIswitch::~MIDIswitch(){};


void MIDIswitch::setThreshold(){
  TouchSwitch::setThreshold();
}

int MIDIswitch::read(){
  int newValue = -1;
  if (inputType == 0){ // Button
    Bounce::update();              // Force a status report of the Bounce object.
    inputState = Bounce::state;
    if (Bounce::fallingEdge()){    // If the button's been pressed,
      newValue = outHi;            // return the high CC value.
    }
    else if (Bounce::risingEdge()){// If the button has been released,
      newValue = outLo;            // return the low CC value.
    }
    else{newValue = -1;}
  }
#if ! defined(__IMXRT1062__)
  else if (inputType == 1){ // Capacitive Touch
    TouchSwitch::update();
    inputState = TouchSwitch::read();
    if (TouchSwitch::risingEdge()){
      newValue = outHi;
    }
    else if (TouchSwitch::fallingEdge()){
      newValue = outLo;
    }
    else{newValue = -1;}
  }
#endif
  return newValue;
};


/* This function will send the appropriate Control Change messages for the press
and/or release of any MIDI button whether it's set to 'MOMENTARY' 'LATCH' or
'TRIGGER' mode.*/
int MIDIswitch::send(){
  int newValue = read();
  if (newValue == outHi){       // If the button's been pressed,
    if (state == false){        // and if it was latched OFF,
      usbMIDI.sendControlChange(number,outHi,MIDIchannel); // send CC outHi,
      newValue = number;
      state = true;             // Remember the button is now on.
    }
    else{                       // If the button was latched ON,
      if (mode == 2){           // and the button's in TRIGGER mode(2),
        usbMIDI.sendControlChange(number,outHi,MIDIchannel); // send CC outHi again 
        newValue = number;
      }
      else {usbMIDI.sendControlChange(number,outLo,MIDIchannel);}// else send outLo,
      state = false;            // Remember the button is now off.
      newValue = outLo;
    }
  }
  else if (newValue == outLo && mode == 0){// Button in MOMENTARY mode released?
    usbMIDI.sendControlChange(number,outLo,MIDIchannel); // send CC outLo,
    state = false;                         // Remember the button is now off
  }
  else {newValue = -1;}
  return newValue;
};


int MIDIswitch::send(bool force){
  if (force){
    if (state){
      usbMIDI.sendControlChange(number,outHi,MIDIchannel);
      return outHi;
    } else {
      usbMIDI.sendControlChange(number,outLo,MIDIchannel);
      return outLo;
    }
  } 
  else { return -1; }
}


// Set the CC number.
void MIDIswitch::setControlNumber(byte num){
  number = num;
};


// Set specific min and max values.
void MIDIswitch::outputRange(byte min, byte max){
	outLo = min;
	outHi = max;
};


// Set the button mode.
void MIDIswitch::setMode(byte mod){
  mode = mod;
};

