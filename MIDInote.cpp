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
  floor = inLo + 3;
  threshold = 80;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 15000; // micros
  timer = 0;
  state = false;

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
  floor = inLo + 3;
  threshold = 80;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 15000; // micros
  timer = 0;
  state = false;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

MIDInote::MIDInote(int p, int num, bool vel, bool pp){
  pinMode(p, INPUT);
  pin = p;
  channel = MIDIchannel;
  number = num;
  velocity = vel;
  polyPressure = pp;
  inLo = 0;
  inHi = 1023;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;
  floor = inLo + 3;
  threshold = 80;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 15000; // micros
  timer = 0;
  state = false;

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 127 (NOT GOOD!)
};

// destructor
MIDInote::~MIDInote(){
};


int MIDInote::read(){
  int returnme = -1;
  int newValue = analogRead(pin);

  // After a note off, wait until signal hits floor to eliminate double triggers
  // on hits near the threshold & wait a few ms to eliminate phase shift.
  if (waiting){
    if (micros() - timer > waitTime && newValue <= floor){
      waiting = false;
      timer = micros();
    }
  }
  else {
    if (velocity){ // sendVelocity() also handles poly pressure if engaged
      returnme = sendVelocity(newValue);
    }
    else if (polyPressure){ // sendPolyPressure() also sends fixed-velocity note
      returnme = sendPolyPressure(newValue);
    }
    else { // No velocity, no poly pressure: ON/OFF only.
      if (state == false && newValue > threshold){
        usbMIDI.sendNoteOn(number, outHi, channel);
        returnme = newValue;
        state = true;
      }
      else if (state == true && newValue <= threshold){
        usbMIDI.sendNoteOn(number, outLo, channel);
        returnme =newValue;
        state = false;
        waiting = true;
      }
      else{
        returnme = -1;
      }
    }
  }
  return returnme;
};


int MIDInote::sendVelocity(int newValue){
  int returnme = -1;

  if(state){ // A note is on so... 
    if (newValue > threshold){
      if (polyPressure){ // Send poly pressure until analog signal drops.
        newValue = analogToMIDI(newValue);
        if (newValue >= 0){
          // CCs are more versatile and easier to assign than poly pressure.
          // Uncomment whichever you wish to use...or both.
          usbMIDI.sendPolyPressure(number, newValue, channel);
//        usbMIDI.sendControlChange(number, newValue, channel);
        }
        else {
          returnme = -1;
        }
      }
      else {
        returnme = -1;
      }
    }
    else { // Send NOTE off when signal drops.
      usbMIDI.sendNoteOn(number, outLo, channel); // note off
      returnme = number;
//    if (polyPressure){ // If using CC, send AFTER note off for DAW assigning.
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
    if (micros() - timer >= 300){ // Compare hiVal & loVal for spikes.
      if (listening){ // After spike detected and peak found...
        newValue = constrain(newValue / divider, outLo, outHi); // assign MIDI &
        usbMIDI.sendNoteOn(number, newValue, channel); // send note on.
        returnme = newValue;
        state = true;
        listening = false;
      }
      else if (hiVal - loVal > threshold){ // Spike found. Listen for peak.
        listening = true;
      }
      else { // No spike this interval. Reset hiVal and loVal & check again.
        hiVal = 0;
        loVal = newValue;
      }
      timer = micros();
    }
  }
  return returnme;
};


int MIDInote::sendPolyPressure(int newValue){
  int returnme = -1;

  if (newValue > threshold){
    if (state == false){
      usbMIDI.sendNoteOn(number, outHi, channel);
      returnme = outHi;
      state = true;
    }
    else {
      newValue = analogToMIDI(newValue);
      if (newValue >= 0){
        usbMIDI.sendPolyPressure(number, newValue, channel);
//      usbMIDI.sendControlChange(number, newValue, channel); // SEE LINE #140
      }
      else {
        returnme = -1;
      }
    }
  }
  else if (newValue <= threshold && state == true){
    usbMIDI.sendNoteOn(number, outLo, channel); // note off
//  usbMIDI.sendControlChange(number, newValue, channel); // SEE LINE #156
    returnme = number;
    state = false;
    waiting = true;
    timer = micros();
  }
  else {
    returnme = -1;
  }
  return returnme;
};


int MIDInote::analogToMIDI(int newValue){
  static int value = 0;
  int returnme = -1;
  
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
    value = newValue;
    returnme = newValue;
  }
  else{
    returnme = -1;
  }
  return returnme;
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

