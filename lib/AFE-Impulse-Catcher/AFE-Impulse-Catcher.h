 /* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Impulse_Cahtcher_h
#define _AFE_Impulse_Cahtcher_h

#include <AFE-Configuration.h>

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)

#include <Arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEImpulseCatcher {

public:
  uint16_t bouncing = AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING;

  /* Constructors */
  AFEImpulseCatcher();

  /* Init switch */
  void begin(uint16_t _bouncing = AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING);

  void newImpulse(void);
  void get(uint32_t &noOfImpulses, uint32_t &duration);

private:
  boolean  active = false;
  uint32_t impulseCounter = 0;
  uint32_t counterStarted = 0;

  uint32_t _previousDuration = 0; // Used in case of time rollout
};

#endif // #if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) || defined(AFE_CONFIG_HARDWARE_RAINMETER)
#endif // _AFE_Impulse_Cahtcher_h