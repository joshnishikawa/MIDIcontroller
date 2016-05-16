#include "MIDIbutton.h"

// constructors
MIDIbutton::MIDIbutton(){};

MIDIbutton::MIDIbutton(int p, int num){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = 0;
  outHi = 127;
  mode = 0; 
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int mod){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = 0;
  outHi = 127;
  mode = mod; 
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int min, int max){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = 0; 
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int min, int max, int mod){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = mod; 
  state = false;
};

// destructor
MIDIbutton::~MIDIbutton(){
  delete myButt;
};


int MIDIbutton::read(){
  int newValue = -1;
  myButt->update();              // Force a status report of the Bounce object.
  if (myButt->fallingEdge()){    // If the button's been pressed,
    newValue = outHi;            // return the CC value and
  }
  else if (myButt->risingEdge()){// If the button has been released,
    newValue = outLo;            // return the CC value and
  }
  else {newValue = -1;}
  return newValue;
};


/* This function will send the appropriate Control Change messages for the press
and/or release of any MIDI button whether it's set to 'latch' 'momentary' or
'instantaneous' mode.*/
int MIDIbutton::send(){
  int newValue = read();
  if (newValue == outHi){       // If the button's been pressed,
    if (state == false){        // and if it was latched OFF,
      usbMIDI.sendControlChange(number,outHi,*MC); // send CC outHi,
      state = true;             // and remember the button is now on.
    }
    else{                       // If the button was latched ON,
      if (mode == 2){           // and the button's in instant mode(2),
        usbMIDI.sendControlChange(number,outHi,*MC); // send CC outHi again 
      }
      else {usbMIDI.sendControlChange(number,outLo,*MC);}// else send outLo,
      state = false;            // and remember the button is now off.
      newValue = outLo;
    }
  }
  else if (newValue == outLo && mode == 0){// Button in momentary mode released?
    usbMIDI.sendControlChange(number,outLo,*MC); // send CC outLo,
    state = false;                         // and remember the button is now off
  }
  else {newValue = -1;}
  return newValue;
};


// Set the CC number.
void MIDIbutton::setControlNumber(int num){
  number = num;
};


// Set specific min and max values.
void MIDIbutton::outputRange(int min, int max){
	outLo=min;
	outHi=max;
};


// Set the button mode.
void MIDIbutton::setMode(int mod){
  mode = mod;
};

