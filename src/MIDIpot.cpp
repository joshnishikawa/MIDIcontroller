#include "MIDIpot.h"

// constructors
MIDIpot::MIDIpot(){};

MIDIpot::MIDIpot(int p, uint8_t num){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = false;
  killSwitch = 0;
  inLo = 0;
  inHi = MIDI_DEFAULT_ADC_MAX;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider;
};

MIDIpot::MIDIpot(int p, uint8_t num, uint8_t k){
  pinMode(p, INPUT);
  pin = p;
  number = num;
  value = 0;
  mode = true;
  killSwitch = k;
  inLo = 0;
  inHi = MIDI_DEFAULT_ADC_MAX;
  outLo = 0;
  outHi = 127;
  invert = outLo > outHi;

  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider;
};

// destructor
MIDIpot::~MIDIpot(){
};

// returns new CC if there's enough change in the analog input; -1 otherwise
int MIDIpot::read(){
  return read(analogRead(pin));
};

int MIDIpot::read(int rawVal){
  int newValue = this->smooth(rawVal, SMOOTHING);
  int clamped = constrain(newValue, (int)inLo, (int)inHi);

  int mappedVal = map(clamped, inLo, inHi, outLo, outHi);
  mappedVal = invert ? constrain(mappedVal, outHi, outLo)
                     : constrain(mappedVal, outLo, outHi);

  if (mappedVal != value){
    return mappedVal;
  }
  return -1;
};

int MIDIpot::send(){
  return send(analogRead(pin));
};

int MIDIpot::send(int rawVal){
  int newValue = read(rawVal);
  uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;

  if (killSwitch != 0 && value == outLo && newValue > outLo){ // ON before main CC
    MIDI_send(MIDI_CONTROL_CHANGE, killSwitch, 127, targetChan, cable, interface);
  }

  if (newValue >= 0){
    MIDI_send(MIDI_CONTROL_CHANGE, number, newValue, targetChan, cable, interface); // MAIN CC MESSAGE
    if (killSwitch != 0 && value >= outLo && newValue == outLo){ // OFF after main
      MIDI_send(MIDI_CONTROL_CHANGE, killSwitch, 0, targetChan, cable, interface);
    }
    value = newValue;
  }
  return newValue;
};

int MIDIpot::send(bool force){
  if (force){
    balancedValue = analogRead(pin);
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    int clamped = constrain((int)balancedValue, (int)inLo, (int)inHi);
    uint8_t newValue = map(clamped, inLo, inHi, outLo, outHi);
    newValue = invert ? constrain(newValue, outHi, outLo)
                      : constrain(newValue, outLo, outHi);

    MIDI_send(MIDI_CONTROL_CHANGE, number, newValue, targetChan, cable, interface);
    value = newValue;
    return newValue;
  }
  else{ return -1; }
}

void MIDIpot::setChannel(byte chan, byte cable, byte iface){
  this->channel = chan;
  this->cable = cable;
  this->interface = iface;
}

void MIDIpot::setControlNumber(uint8_t num){ // Set the CC number.
  number = num;
};

// Set upper and lower limits for outgoing MIDI messages.
void MIDIpot::outputRange(uint8_t min, uint8_t max){
  outLo = constrain(min, 0, 127);
  outHi = constrain(max, 0, 127);
  invert = outHi < outLo;              // Check again for reverse polarity.
  divider = !invert ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider;
};

// Limit the analog input to the usable range of a sensor.
void MIDIpot::inputRange(uint16_t min, uint16_t max){
  inLo = min;
  inHi = max;
  divider = outHi > outLo ? (inHi-inLo)/(outHi-outLo):(inHi-inLo)/(outLo-outHi);
  divider = divider < 1 ? 1 : divider;
};

void MIDIpot::setKillSwitch(uint8_t k){
  if(k == 0){
    mode = false;
  }
  else{
    mode = true;
    killSwitch = constrain(k, 1, 127);
  }
};

int MIDIpot::smooth(int val, int NR){
  difference = val - balancedValue;
  buffer = val == 0 ? -NR : val == balancedValue ? buffer/2 : buffer+difference;

  if ((long)buffer*buffer >= (long)NR*NR){
    balancedValue = val;
    buffer = 0;
  }
  return balancedValue;
};
