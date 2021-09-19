/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Impulse-Catcher.h"

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)

AFEImpulseCatcher::AFEImpulseCatcher(){};

void AFEImpulseCatcher::begin(uint16_t _bouncing) {
  active = true;
  bouncing = _bouncing;
#ifdef DEBUG
  Serial << endl << F("INFO: IMPULSE CATCHER: initialized and working");
#endif
}

void AFEImpulseCatcher::newImpulse(void) {
  if (active) {
    impulseCounter++;
#ifdef DEBUG
    Serial << endl
           << F("INFO: IMPULSE CATCHER: New impulse. Total: ") << impulseCounter
           << F(", during: ") << ((millis() - counterStarted) / 1000) << F("sec.");
#endif
  }
}

void AFEImpulseCatcher::get(uint32_t &noOfImpulses, uint32_t &duration) {
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
         << F("INFO: IMPULSE CATCHER: Reading data from Binary sensor: Impulses: ") << noOfImpulses
         << F(", during: ") << (duration / 1000) << F("sec.");
#endif
}

#endif // #if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)