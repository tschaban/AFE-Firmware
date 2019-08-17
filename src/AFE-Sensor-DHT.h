/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_DHXX

#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

#if defined(DEBUG)
#include <Streaming.h>
#endif

#define IDX_TYPE_TEMPERATURE 0
#define IDX_TYPE_HUMIDITY 1
#define IDX_TYPE_TEMPERATURE_AND_HUMIDITY 2

class AFESensorDHT {

private:
  DH configuration;

  float currentTemperature;
  float currentHumidity;
  float currentDewPoint;
  float currentHeatIndex;

  boolean dataInMemory = false;
  boolean _initialized = false;

  int readResult;
  unsigned long startTime = 0;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  void dht_wrapper();

  /* Method returns temperature */
  float getTemperature();

  /* Method returns humidity */
  float getHumidity();

  /* Method returns heat index */
  float getHeatIndex();

  /* Method returns Dew Point */
  float getDewPoint();

  /* It returns true if data has been read from the sensor */
  boolean isReady();

  /* Method should be added to the main loop to check temperature / humidity in
   * defined time frame */
  void listener();

  /* Method returns sensor IDX for temperature, humidity, temperature and
   * humidity */
  unsigned long getDomoticzIDX(uint8_t type);

  /* Get HeatIndex publishing configuration item */
  boolean publishHeatIndex();

  /* True if dew point should be published */
  boolean publishDewPoint();
};

#endif
#endif
