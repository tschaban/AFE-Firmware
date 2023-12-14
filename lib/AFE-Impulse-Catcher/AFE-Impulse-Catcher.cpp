/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Impulse-Catcher.h"

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)

#ifdef DEBUG

AFEImpulseCatcher::AFEImpulseCatcher(AFEDebugger *_Debugger){Debugger = _Debugger;};
#else
AFEImpulseCatcher::AFEImpulseCatcher(){};
#endif

void AFEImpulseCatcher::begin(uint16_t _bouncing)
{
  active = true;
  bouncing = _bouncing;
#ifdef DEBUG
  Debugger->printInformation(F("Initialized"), F("IMPULSE CATCHER"));
#endif
}

void AFEImpulseCatcher::newImpulse(void)
{
  if (active)
  {
    impulseCounter++;
#ifdef DEBUG
  Debugger->printInformation(F("New impulse"), F("IMPULSE CATCHER"));
  Debugger->printBulletPoint(F("Total: "));
  Serial << impulseCounter;
  Debugger->printBulletPoint(F("During: "));
  Serial << ((millis() - counterStarted) / 1000) << F("sec.");
#endif
  }
}

void AFEImpulseCatcher::get(uint32_t &noOfImpulses, uint32_t &duration)
{
  duration = millis() - counterStarted;
  if (duration < 0)
  { // used previous duration if timer rollouts
    duration = _previousDuration;
  }
  noOfImpulses = impulseCounter;
  _previousDuration = duration;
  impulseCounter = 0;
  counterStarted = millis();
#ifdef DEBUG

  Debugger->printInformation(F("Reading data from Binary sensor: Impulses: "), F("IMPULSE CATCHER"));
  Debugger->printBulletPoint(F("Impulses: "));
  Serial << impulseCounter;
  Debugger->printBulletPoint(F("During: "));
  Serial << ((millis() - counterStarted) / 1000) << F("sec.");
#endif
}

#endif // #if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)