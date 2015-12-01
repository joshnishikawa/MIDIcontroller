#include "MIDInote.h"

// constructors
MIDInote::MIDInote(){};

MIDInote::MIDInote(int p, int num){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  velocity = false;
  polyPressure = false;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outHi < outLo;
  threshold = 100;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 10000; // micros
  timer = 0;
  state = false;
  newValue = 0;
  value = 0;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDInote::MIDInote(int p, int num, bool vel){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  velocity = vel;
  polyPressure = false;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outHi < outLo;
  threshold = 100;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 10000; // micros
  timer = 0;
  state = false;
  newValue = 0;
  value = 0;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDInote::MIDInote(int p, int num, bool vel, bool aft){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  velocity = vel;
  polyPressure = aft;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;
  threshold = 100;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 10000; // micros
  timer = 0;
  state = false;
  newValue = 0;
  value = 0;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

// destructor
MIDInote::~MIDInote(){
};


void MIDInote::read(){
  if (waiting){ // Wait after note off to reduce false triggers.
    if (micros() - timer > waitTime){
      waiting = false;
      timer = micros();
    }
  }
  else if (velocity){ // readVelocity() also handles poly pressure if engaged
    newValue = analogRead(pin);
    readVelocity();
  }
  else if (polyPressure){ // PolyPressure without velocity? It's possible.
    newValue = analogRead(pin);
    if (newValue > threshold){
      usbMIDI.sendNoteOn(number, outHi, channel);
      state = true;
      readPolyPressure();
    }
    else {
      usbMIDI.sendAfterTouch(outLo, channel);
//    usbMIDI.sendControlChange(number, newValue, channel); // SEE LINES #146 & #200
      usbMIDI.sendNoteOn(number, outLo, channel);
      state = false;
      waiting = true;
      timer = micros();
    }
  }
  else { // No velocity, no poly pressure. ON/OFF only.
    if (state == false && analogRead(pin) >= threshold){
      usbMIDI.sendNoteOn(number, outHi, channel);
      state = true;
    }
    else if (state == true && analogRead(pin) < threshold){
      usbMIDI.sendNoteOn(number, outLo, channel);
      state = false;
    }
  }
};

void MIDInote::readVelocity(){
  if(state){ // A note is on so... 
    if (newValue > threshold){ // Send poly pressure until analog signal drops or
      if (polyPressure){
        readPolyPressure();
      }
    }
    else { // send a final poly pressure message and NOTE off.
      if (polyPressure){
        usbMIDI.sendAfterTouch(outLo, channel);
      }
      usbMIDI.sendNoteOn(number, outLo, channel);
//    if (polyPressure){ // if using CC, send AFTER note off for DAW assigning.
//      usbMIDI.sendControlChange(number, newValue, channel);
//    }
      state = false;
      waiting = true;
      hiVal = 0;
      loVal = threshold;
      timer = micros();
    }
  }
  else { // Check for user input.
    if (newValue > hiVal){ // Keep track of the highest value...
      hiVal = newValue;
    }
    else if (newValue < loVal && listening == false){ // and the lowest value.
      loVal = newValue;
    }

    if (micros() - timer >= 500){ // Compare hiVal & loVal for spikes.
      if (listening){ // After spike detected and peak found...
        newValue = constrain(newValue / divider, outLo, outHi); // assign MIDI &
        usbMIDI.sendNoteOn(number, newValue, channel); // send NOTE on.
        state = true;
        listening = false;
      }
      else if (hiVal - loVal > threshold){ // Spike found. Listen for peak.
        listening = true;
      }
      else { // No spike this milli. Reset hiVal and loVal & check again.
        hiVal = 0;
        loVal = threshold;
      }
      timer = micros();
    }
  }
};

void MIDInote::readPolyPressure(){
  if (newValue >= inHi){ // Explicitly assign hi analog to hi MIDI
    newValue = outHi;
  }
  else if (newValue <= inLo){ // Explicitly assign low analog to low MIDI
    newValue = outLo;
  }
  else if (newValue % divider == 0){ // Filter intermittent values
    newValue = map(newValue, inLo, inHi, outLo, outHi);
    newValue = invert ? constrain(newValue, outHi, outLo) :
                        constrain(newValue, outLo, outHi);
  }
  else{
    newValue = -1;
  }

  if (newValue >= 0 && newValue != value){
    // I find a CC to be more versatile and easier to assign than poly pressure.
    // Uncomment whichever you wish to use...or both.
    usbMIDI.sendAfterTouch(newValue, channel);
//  usbMIDI.sendControlChange(number, newValue, channel);

    value = newValue;
  }
};


void MIDInote::setNoteNumber(int num){ // Set the NOTE number.
  number = num;
};

void MIDInote::setChannel(int ch){// Set MIDI channel for a single NOTE.
  channel = ch;  
};

// Limit the analog input to the usable range of a sensor.
// Stability decreases as the difference between inHi and inLo decreases.
void MIDInote::inputRange(int min, int max){
  inLo = min;
  inHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

void MIDInote::outputRange(int min, int max){ // Set min & max poly pressure
  outLo = min;
  outHi = max;
  invert = outLo > outHi; // Check again for reverse polarity.
  // Reset the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

void MIDInote::setThreshold(int thresh){
  threshold = thresh;
};