/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Sensor_DS18B20_h
#define _AFE_Sensor_DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <DallasTemperature.h>
#include <OneWire.h>

class AFESensorDS18B20 {

private:
  DS18B20 configuration;
  float currentTemperature = -127;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDS18B20();

  void begin();

  /* Get current temp in Celsius (default) possible options:
     - UNIT_CELCIUS
    - UNIT_FAHRENHEIT
  */

  // @TODO I think reading temp should be made in the listener, issue to solve
  // how to get temperature but publish only changes
  float get();

  float getLatest();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();
};

#endif
