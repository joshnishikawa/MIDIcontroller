#include "DMXbutton.h"

// constructors
DMXbutton::DMXbutton(){};

DMXbutton::DMXbutton(int p, byte num){
  pinMode(p, INPUT_PULLUP);
  userButton = new Bounce(p, 50);
  number = num;
  outLo = 0;
  outHi = 127;
  mode = 0; 
  state = false;
};

DMXbutton::DMXbutton(int p, byte num, byte mod){
  pinMode(p, INPUT_PULLUP);
  userButton = new Bounce(p, 50);
  number = num;
  outLo = 0;
  outHi = 127;
  mode = mod; 
  state = false;
};

DMXbutton::DMXbutton(int p, byte num, byte min, byte max){
  pinMode(p, INPUT_PULLUP);
  userButton = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = 0; 
  state = false;
};

DMXbutton::DMXbutton(int p, byte num, byte min, byte max, byte mod){
  pinMode(p, INPUT_PULLUP);
  userButton = new Bounce(p, 50);
  number = num;
  outLo = min;
  outHi = max;
  mode = mod; 
  state = false;
};

// destructor
DMXbutton::~DMXbutton(){
  delete userButton;
};


int DMXbutton::read(){
  int newValue = -1;
  userButton->update();              // Force a status report of the Bounce object.
  if (userButton->fallingEdge()){    // If the button's been pressed,
    newValue = outHi;            // return the CC value and
  }
  else if (userButton->risingEdge()){// If the button has been released,
    newValue = outLo;            // return the CC value and
  }
  else{newValue = -1;}
  return newValue;
};


/* This function will send the appropriate Channel Change messages for the press
and/or release of any DMX button whether it's set to 'latch' 'momentary' or
'instantaneous' mode.*/
int DMXbutton::send(){
  int newValue = read();
  if (newValue == outHi){       // If the button's been pressed,
    if (state == false){        // and if it was latched OFF,
      usbDMX.sendChannelChange(number,outHi,DMXchannel); // send CC outHi,
      newValue = number;
      state = true;             // and remember the button is now on.
    }
    else{                       // If the button was latched ON,
      if (mode == 2){           // and the button's in instant mode(2),
        usbDMX.sendChannelChange(number,outHi,DMXchannel); // send CC outHi again 
        newValue = number;
      }
      else {usbDMX.sendChannelChange(number,outLo,DMXchannel);}// else send outLo,
      state = false;            // and remember the button is now off.
      newValue = outLo;
    }
  }
  else if (newValue == outLo && mode == 0){// Button in momentary mode released?
    usbDMX.sendChannelChange(number,outLo,DMXchannel); // send CC outLo,
    state = false;                         // and remember the button is now off
  }
  else {newValue = -1;}
  return newValue;
};


// Set the CC number.
void DMXbutton::setChannelNumber(byte num){
  number = num;
};


// Set specific min and max values.
void DMXbutton::outputRange(byte min, byte max){
	outLo=min;
	outHi=max;
};


// Set the button mode.
void DMXbutton::setMode(byte mod){
  mode = mod;
};

