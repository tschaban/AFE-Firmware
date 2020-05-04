/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Binary.h"

AFESensorBinary::AFESensorBinary(){};

void AFESensorBinary::begin(uint16_t _bouncing) {
  active = true;
  bouncing = _bouncing;
#ifdef DEBUG
  Serial << endl << F("INFO: Binary sensor initialized and working");
#endif
}

void AFESensorBinary::newImpulse(void) {
  if (active) {
    impulseCounter++;
#ifdef DEBUG
    Serial << endl
           << F("INFO: New impulse. Total: ") << impulseCounter
           << F(", during: ") << ((millis() - counterStarted) / 1000) << F("sec.");
#endif
  }
}

void AFESensorBinary::get(uint32_t &noOfImpulses, uint32_t &duration) {
  duration = millis() - counterStarted;
  if (duration < 0) { // used previous duration if timer rollouts
    duration = _previousDuration;
  }
  noOfImpulses = impulseCounter;
  _previousDuration = duration;
  impulseCounter = 0;
  counterStarted = millis();
#ifdef DEBUG
  Serial << endl
         << F("INFO: Reading data from Binary sensor: Impulses: ") << noOfImpulses
         << F(", during: ") << (duration / 1000) << F("sec.");
#endif
}