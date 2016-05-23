#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int FSRpin0 = A0;  // Change these to the correct ANALOG pins
const int FSRpin1 = A1;
const int FSRpin2 = A2;
const int FSRpin3 = A3;
unsigned long previousMillis;
int lastAft = 0;
int maxValue = -1;
MIDInote* notes[4];       // An array of pointers to your inputs

void setup(){
  notes[0] = new MIDInote(FSRpin0, 40, true);
  notes[1] = new MIDInote(FSRpin1, 45, true);
  notes[2] = new MIDInote(FSRpin2, 50, true);
  notes[3] = new MIDInote(FSRpin3, 55, true);

  // Use the rangeFinder example to find the usable range of
  // your sensors and enter it here.
  notes[0]->inputRange(0, 200);
  notes[1]->inputRange(0, 200);
  notes[2]->inputRange(0, 200);
  notes[3]->inputRange(0, 200);
}

void loop(){
  for (uint8_t i=0;i<(sizeof(notes)/sizeof(notes[0]));i++){
    notes[i]->send();
    if (notes[i]->afterTouchValue > maxValue){
      maxValue = notes[i]->afterTouchValue;
    }
  }
  if (millis() - previousMillis > 10 && maxValue >= 0 && maxValue != lastAft){
    usbMIDI.sendAfterTouch(maxValue, MIDIchannel);
    previousMillis = millis();
    lastAft = maxValue;
  }
  maxValue = -1;
}
