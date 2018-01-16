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

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <DHT.h>
#include <Streaming.h>

class AFESensorDHT {

private:
  DH configuration;
  float currentTemperature = -127;
  float currentHumidity = -1;
  boolean temperatureInBuffer = false;
  boolean humidityInBuffer = false;
  unsigned long temperatureCounterStartTime = 0;
  unsigned long humidityCounterStartTime = 0;
  boolean _initialized = false;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  void begin();

  float getTemperature();
  float getHumidity();
  float getLatestTemperature();
  float getLatestHumidity();
  float getHeatIndex();

  boolean temperatureSensorReady();
  boolean humiditySensorReady();

  void listener();
};

#endif
