/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <DHT.h>
#include <AFE-Data-Structures.h>
#include <AFE-Data-Access.h>

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

class AFESensorDHT {

private:
  DH configuration;
  float currentTemperature = -127;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  void begin();

  /* Get current temp in Celsius (default) possible options:
     - UNIT_CELCIUS
     - UNIT_FAHRENHEIT
  */

  float get();

  float getLatest();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value changes */
  void listener();

};

#endif
