// Please read Flicker.h for information about the liscence and authors

#include "WProgram.h"
#include "Flicker.h"

Flicker::Flicker(uint8_t pin, int threshold){
  this->pin = pin;
  this->threshold = threshold;
  this->offThreshold = threshold-(threshold/10); // to prevent multiple triggers near the threshold
  state = false;
};



void Flicker::retrigger(unsigned long interval)
{
	 this->retrigger_millis = interval;
}



int Flicker::update()
{
	if ( trigger() ) {
        retrigger(0);
        return stateChanged = 1;
    }

  // We need to retrigger, so simulate a state change
	if ( retrigger_millis && (millis() - previous_millis >= retrigger_millis) ) {
        previous_millis = millis();
		 retrigger(0);
		 return stateChanged = 1;
	}

	return stateChanged = 0;
}


unsigned long Flicker::duration()
{
  return millis() - previous_millis;
}


int Flicker::read()
{
	return (int)state;
}


// Protected: triggers the pin
int Flicker::trigger() {
  int newValue = touchRead(pin);
  if (newValue >= threshold && !state){
    state = true;
    previous_millis = millis();
    return 1;
  }
  else if (newValue <= offThreshold && state){
    state = false;
    previous_millis = millis();
    return 1;
  }
  else {return 0;}
}



// The risingEdge method is true for one scan after the triggered input goes from off-to-on.
bool  Flicker::risingEdge() { return stateChanged && state; }
// The fallingEdge method is true for one scan after the triggered input goes from on-to-off. 
bool  Flicker::fallingEdge() { return stateChanged && !state; }

void Flicker::setThreshold(int T){
  threshold = T;
};

