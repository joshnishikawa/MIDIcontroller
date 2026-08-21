#include "MIDIencMulti.h"

MIDIencMulti::MIDIencMulti() {
  myKnob = NULL;
}

MIDIencMulti::~MIDIencMulti() {
  if (myKnob != NULL) {
    delete myKnob;
    myKnob = NULL;
  }
}

void MIDIencMulti::allocateEncoder(int a, int b) {
  if (myKnob != NULL) {
    delete myKnob;
  }
  myKnob = new Encoder(a, b);
}

void MIDIencMulti::initAsEncoder(int a, int b, byte num, byte min, byte max, byte detentOrValue) {
  allocateEncoder(a, b);
  this->number = num;
  this->numberS = num;
  this->outLo = min;
  this->outHi = max;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = min;
  this->encoderMode = ENCODER_MODE_STANDARD;
}

void MIDIencMulti::initAsEncoder(int a, int b, byte num, byte numS, byte min, byte max, byte detentOrValue) {
  allocateEncoder(a, b);
  this->number = num;
  this->numberS = numS;
  this->outLo = min;
  this->outHi = max;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = min;
  this->encoderMode = ENCODER_MODE_STANDARD;
}

void MIDIencMulti::initAsJog1Cc(int a, int b, byte num, byte outValLeft, byte outValRight, byte detentOrValue) {
  allocateEncoder(a, b);
  this->number = num;
  this->numberS = num;
  this->outValueLeft = outValLeft;
  this->outValueRight = outValRight;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = 0;
  this->encoderMode = ENCODER_MODE_JOG_1_CC;
}

void MIDIencMulti::initAsJog1Cc(int a, int b, byte num, byte numS, byte outValLeft, byte outValRight, byte detentOrValue) {
  allocateEncoder(a, b);
  this->number = num;
  this->numberS = numS;
  this->outValueLeft = outValLeft;
  this->outValueRight = outValRight;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = 0;
  this->encoderMode = ENCODER_MODE_JOG_1_CC;
}

void MIDIencMulti::initAsJog2Cc(int a, int b, byte numLeft, byte numRight, byte detentOrValue) {
  allocateEncoder(a, b);
  this->numberLeft = numLeft;
  this->numberRight = numRight;
  this->numberLeftS = numLeft;
  this->numberRightS = numRight;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = 0;
  this->outLo = 0;
  this->outHi = 127;
  this->encoderMode = ENCODER_MODE_JOG_2_CC;
}

void MIDIencMulti::initAsJog2Cc(int a, int b, byte numLeft, byte numRight, byte numLeftS, byte numRightS, byte detentOrValue) {
  allocateEncoder(a, b);
  this->numberLeft = numLeft;
  this->numberRight = numRight;
  this->numberLeftS = numLeftS;
  this->numberRightS = numRightS;
  this->detentOrValue = detentOrValue > 0 ? detentOrValue : PER_VALUE;
  this->value = 0;
  this->outLo = 0;
  this->outHi = 127;
  this->encoderMode = ENCODER_MODE_JOG_2_CC;
}

int MIDIencMulti::read() {
  if (myKnob == NULL) return -1;
  int incdec = myKnob->read();

  if (encoderMode == ENCODER_MODE_STANDARD) {
    if (incdec >= detentOrValue) {
      myKnob->write(0);
      if (value < outHi) {
        value++;
        return value;
      }
      return -1;
    } else if (incdec <= -detentOrValue) {
      myKnob->write(0);
      if (value > outLo) {
        value--;
        return value;
      }
      return -1;
    }
    return -1;
  } else {
    // Jog modes: check if threshold crossed
    if (incdec >= detentOrValue) {
      myKnob->write(0);
      return 1; // CW (Turn Right)
    } else if (incdec <= -detentOrValue) {
      myKnob->write(0);
      return -1; // CCW (Turn Left)
    }
    return 0; // No step
  }
}

int MIDIencMulti::send(int shiftState) {
  uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
  int step = read();

  if (encoderMode == ENCODER_MODE_STANDARD) {
    if (step >= 0) {
      uint8_t ccNum = (shiftState == 1 && numberS != 0) ? numberS : number;
      MIDI_send(MIDI_CONTROL_CHANGE, ccNum, step, targetChan, cable, interface);
      return step;
    }
    return -1;
  } else if (encoderMode == ENCODER_MODE_JOG_1_CC) {
    if (step != 0) {
      uint8_t ccNum = (shiftState == 1 && numberS != 0) ? numberS : number;
      uint8_t valToSend = (step > 0) ? outValueRight : outValueLeft;
      MIDI_send(MIDI_CONTROL_CHANGE, ccNum, valToSend, targetChan, cable, interface);
      return valToSend;
    }
    return -1;
  } else if (encoderMode == ENCODER_MODE_JOG_2_CC) {
    if (step > 0) {
      uint8_t ccNum = (shiftState == 1 && numberRightS != 0) ? numberRightS : numberRight;
      MIDI_send(MIDI_CONTROL_CHANGE, ccNum, outHi, targetChan, cable, interface);
      return outHi;
    } else if (step < 0) {
      uint8_t ccNum = (shiftState == 1 && numberLeftS != 0) ? numberLeftS : numberLeft;
      MIDI_send(MIDI_CONTROL_CHANGE, ccNum, outHi, targetChan, cable, interface);
      return outHi;
    }
    return -1;
  }
  return -1;
}

int MIDIencMulti::send(bool force) {
  if (force && encoderMode == ENCODER_MODE_STANDARD) {
    uint8_t targetChan = (channel == 0) ? MIDIchannel : channel;
    MIDI_send(MIDI_CONTROL_CHANGE, number, value, targetChan, cable, interface);
    return value;
  }
  return -1;
}

void MIDIencMulti::setChannel(byte chan, byte cable, byte iface) {
  this->channel = chan;
  this->cable = cable;
  this->interface = iface;
}

void MIDIencMulti::setControlNumber(byte num) {
  this->number = num;
}

void MIDIencMulti::outputRange(byte min, byte max) {
  this->outLo = constrain(min, 0, 127);
  this->outHi = constrain(max, 0, 127);
}

void MIDIencMulti::write(byte val) {
  this->value = constrain(val, outLo, outHi);
}
