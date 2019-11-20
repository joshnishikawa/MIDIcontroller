#include "MIDIbutton.h"

// constructors
MIDIbutton::MIDIbutton() : Bounce(0, 0), TouchSwitch(0, 0){};

MIDIbutton::MIDIbutton(int p, byte num, byte mod) : Bounce(p, 10), TouchSwitch(0, 0){
  pinMode(p, INPUT_PULLUP);
  inputType = 0; //button
  number = num;
  this->mode = mode; 
  state = false;
};

MIDIbutton::MIDIbutton(int p, byte num, byte mode, int type) : Bounce(0, 0), TouchSwitch(p, 0, type){
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
    // deperecated. Please use TOUCH as the argument and setThreshold(). 
    TouchSwitch::setThreshold(type);
  }

  number = num;
  this->mode = mode;
  state = false;
};

// destructor
MIDIbutton::~MIDIbutton(){};


void MIDIbutton::setThreshold(){
  TouchSwitch::setThreshold();
}

int MIDIbutton::read(){
  int newValue = -1;
  if (inputType == 0){
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
  else if (inputType == 1){
    TouchSwitch::update();
    inputState = TouchSwitch::state;
    if (TouchSwitch::risingEdge()){
      newValue = outHi;
    }
    else if (TouchSwitch::fallingEdge()){
      newValue = outLo;
    }
    else{newValue = -1;}
  }
  return newValue;
};


/* This function will send the appropriate Control Change messages for the press
and/or release of any MIDI button whether it's set to 'MOMENTARY' 'LATCH' or
'TRIGGER' mode.*/
int MIDIbutton::send(){
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


// Set the CC number.
void MIDIbutton::setControlNumber(byte num){
  number = num;
};


// Set specific min and max values.
void MIDIbutton::outputRange(byte min, byte max){
	outLo = min;
	outHi = max;
};


// Set the button mode.
void MIDIbutton::setMode(byte mod){
  mode = mod;
};

