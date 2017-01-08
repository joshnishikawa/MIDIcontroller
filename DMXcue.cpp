#include "DMXcue.h"

// constructors
DMXcue::DMXcue(){};

DMXcue::DMXcue(int p, byte num){ // ATTN: Needs to be ported to apply to DMX
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  velocity = false;
  inLo = 0;
  inHi = 65536;
  outLo = 0;
  outHi = 255;
  invert = outHi < outLo;
  threshold = 80;
  floor = inLo + 3;
  listening = false;  // ATTN: Needs to be ported to apply to DMX
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 10000; // micros
  timer = 0;
  state = false;
  afterTouchValue = 0;  // ATTN: Needs to be ported to apply to DMX

  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 255 (NOT GOOD!)
};

DMXcue::DMXcue(int p, byte num, bool vel){ // ATTN: Needs to be ported to apply to DMX
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  velocity = vel;
  inLo = 0;
  inHi = 65536;
  outLo = 0;
  outHi = 255;
  invert = outHi < outLo;
  threshold = 80;
  floor = inLo + 3;
  listening = false;
  loVal = threshold; 
  hiVal = 0;
  waiting = false;
  waitTime = 10000; // micros
  timer = 0;
  state = false;
  afterTouchValue = 0;  // ATTN: Needs to be ported to apply to DMX
  
  // Sets the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 255 (NOT GOOD!)
};

// destructor
DMXcue::~DMXcue(){
};

int DMXcue::read(){  // ATTN: Being ported to apply to DMX
  int newValue = analogRead(pin);
  // After a note off, wait until signal hits floor to eliminate double triggers
  // on hits near the threshold & wait a few msecs to eliminate phase shift.
  if (waiting){
    if (micros() - timer > waitTime && newValue <= floor){
      waiting = false;
      timer = micros();
    }
    newValue = -1;
  }
  else {
    if (velocity){
      if(state){ // A note is on so... 
        if (newValue < threshold){ // Send NOTE off when signal drops.
          newValue = outLo;
          afterTouchValue = 0;
          state = false;
          waiting = true;
          hiVal = 0;
          loVal = threshold;
          timer = micros();
        }
        else {
          afterTouchValue = constrain(newValue / divider, outLo, outHi);
          newValue = -1;
        }
      }
      else { // Check for user input.
        afterTouchValue = -1;
        if (newValue > hiVal){ // Keep track of the highest value...
          hiVal = newValue;
        }
        else if (newValue < loVal && listening == false){//and the lowest value.
          loVal = newValue;
        }
        if (micros() - timer >= 100){ // Compare hiVal & loVal for spikes.
          if (listening){ // After spike detected and peak found...
            newValue = constrain(hiVal / divider, outLo, outHi); // assign DMX
            state = true;
            listening = false;
          }
          else if (hiVal - loVal > threshold){ // Spike found. Listen for peak.
            newValue = -1;
            listening = true;
          }
          else { // No spike this interval. Reset hiVal and loVal & check again.
            newValue = -1;
            hiVal = 0;
            loVal = newValue;
          }
          timer = micros();
        }
        else {newValue = -1;}
      }
    }
    else { // No velocity, no poly pressure: ON/OFF only.
      if (state == false && newValue > threshold){
        newValue = outHi;
        state = true;
      }
      else if (state == true && newValue <= threshold){
        newValue = outLo;
        state = false;
        waiting = true;
      }
      else{newValue = -1;}
    }
  }
  return newValue;
};

int DMXcue::send(){  // ATTN: Needs to be ported to apply to DMX  (USB is not used)
  int newValue = read();
  if (newValue >= 0){
    usbDMX.sendNoteOn(number, newValue, DMXchannel);
    value = newValue;
  }
  return newValue;
};

void DMXcue::setNoteNumber(byte num){ // Set the NOTE number.
  number = num;
};

// Limit the analog input to the usable range of a sensor.
// Stability decreases as the difference between inHi and inLo decreases.
void DMXcue::inputRange(int min, int max){
  inLo = min;
  inHi = max;
  // Reset the interval at which alalog signals will actually register.
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 255 (NOT GOOD!)
};

void DMXcue::outputRange(byte min, byte max){ // Set min & max poly pressure
  outLo = min;
  outHi = max;
  invert = outLo > outHi; // Check again for reverse polarity.
  // Reset the interval at which alalog signals will actually register.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider; // Allows analog range < 255 (NOT GOOD!)
};

void DMXcue::setThreshold(int thresh){
  threshold = thresh;
};

