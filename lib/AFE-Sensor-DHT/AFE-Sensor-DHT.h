/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DHT

#include <AFE-Data-Access.h>
#include <AFE-Sensors-Common.h>
#include <Arduino.h>
#include <DHTesp.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorDHT : public AFESensorsCommon {

private:
  AFEDataAccess *Data;
#ifdef DEBUG
  AFEDebugger *Debugger;
#endif
  boolean _initialized = false;

  unsigned long startTime = 0;

  DHTesp *dht = new DHTesp();

#ifdef DEBUG
  void begin(AFEDataAccess *, uint8_t id);
#endif

public:
  DHT_CONFIG *configuration = new DHT_CONFIG;

  float currentTemperature;
  float currentHumidity;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

#ifdef DEBUG
  /* Initializing method with Debugger on */
  void begin(AFEDataAccess *, AFEDebugger *, int8_t id);
#else
  /* Initializing method */
  void begin(AFEDataAccess *, uint8_t id);
#endif
  /* Method should be added to the main loop to check temperature / humidity in
   * defined time frame */
  boolean listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_DHT
#endif // _AFE_Sensor_DHT_h
