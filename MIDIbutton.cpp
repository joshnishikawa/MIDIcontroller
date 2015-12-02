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
  channel = MIDIchannel;
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int mod){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = 0;
  outHi = 127;
  mode = mod; 
  channel = MIDIchannel;
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int min, int max){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = 0; 
  channel = MIDIchannel;
  state = false;
};

MIDIbutton::MIDIbutton(int p, int num, int min, int max, int mod){
  pinMode(p, INPUT_PULLUP);
  myButt = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = mod; 
  channel = MIDIchannel;
  state = false;
};

// destructor
MIDIbutton::~MIDIbutton(){
  delete myButt;
};


/* This function will send the appropriate Control Change messages for the press
and/or release of any MIDI button whether it's set to 'latch' 'momentary' or
'instantaneous' mode.*/
int MIDIbutton::read(){
  int returnme = -1;
  myButt->update();             // Force a status report of the Bounce object.
  if (myButt->fallingEdge()){   // If the button's been pressed,
    if (state == false){        // and if the button was latched OFF,
      usbMIDI.sendControlChange(number,outHi,channel);// send CC value,
      returnme = number;        // return the CC number and
      state = true;             // remember the button is now on.
      if (mode == 2){           // If the button's in instant mode(2),
        usbMIDI.sendControlChange(number,outLo,channel);// send CC outLo,
        state = true;           // and remember the STATE ONLY is on.
      }
    }
    else{                       // If the button was latched ON,
      if (mode == 2){           // If the button's in instant mode(2),
        usbMIDI.sendControlChange(number,outHi,channel);// send CC max first 
      }
      usbMIDI.sendControlChange(number,outLo,channel);// send CC outLo,
      returnme = number;        // return the CC number and
      state = false;            // remember the button is now off.
    }
  }
  else if (myButt->risingEdge()){// If the button has been released,
    if (mode == 0){             // and if the button's in momentary mode,
      usbMIDI.sendControlChange(number,outLo,channel);// send CC outLo,
      returnme = number;        // return the CC number and
      state = false;            // remember the button is now off.
    }
  }
  return returnme;
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


// Set a specific MIDI channel for a single button.
void MIDIbutton::setChannel(int ch){
  channel = ch;
};