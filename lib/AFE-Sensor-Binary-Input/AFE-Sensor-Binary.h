/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Binary_h
#define _AFE_Sensor_Binary_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBinary {

public:
  uint16_t bouncing = AFE_HARDWARE_WIND_SENSOR_DEFAULT_BOUNCING;

  /* Constructors */
  AFESensorBinary();

  /* Init switch */
  void begin(uint16_t _bouncing = AFE_HARDWARE_WIND_SENSOR_DEFAULT_BOUNCING);

  void newImpulse(void);
  void get(uint32_t &noOfImpulses, uint32_t &duration);

private:
  boolean  active = false;
  uint32_t impulseCounter = 0;
  uint32_t counterStarted = 0;

  uint32_t _previousDuration = 0; // Used in case of time rollout
  

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif
};

#endif
