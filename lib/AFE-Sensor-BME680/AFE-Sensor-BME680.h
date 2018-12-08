/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Adafruit_BME680.h>

class AFESensorBME680 {

private:
  BME680 configuration;
  BME680_DATA sensorData;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

  Adafruit_BME680 bme;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME680();

  void begin();

  BME680_DATA get();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Return relay IDX in Domoticz */
  void getDomoticzIDX(BME680_DOMOTICZ *idx);
};

#endif
