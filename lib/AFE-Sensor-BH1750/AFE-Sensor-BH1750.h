/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_Sensor_BH1750_h
#define _AFE_Sensor_BH1750_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <BH1750.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBH1750 {

private:
  BH1750 configuration;

  BH1750LightSensor bh1750;

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  float currentLightLevel = -1;

public:
  /* Constructor */
  AFESensorBH1750();

  /* Turns On sensor */
  void begin();

  /* returns PM2.5 and PM10 */
  float get();

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Return relay IDX in Domoticz */
  unsigned long getDomoticzIDX();
};

#endif
