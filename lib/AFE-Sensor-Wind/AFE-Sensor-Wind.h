/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Wind_h
#define _AFE_Sensor_Wind_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Sensor-Binary.h>


#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorWind {

public:
  WINDSENSOR configuration;

  /* Constructors */
  AFESensorWind();

  /* Init switch */
  boolean begin(AFEDataAccess *, AFESensorBinary *);

  void get(float &speedMS, float &speedKMH);

  boolean listener(void);

private:
  AFESensorBinary *_Sensor;
  AFEDataAccess *_Data;
  boolean _initialized = false;
  uint32_t startTime = 0;

  float oneImpulseDistanceCM = 0;

};

#endif
