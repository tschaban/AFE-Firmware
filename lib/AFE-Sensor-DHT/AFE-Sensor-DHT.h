/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DHT

#include <arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-Sensors-Common.h>
#include <DHTesp.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif



#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorDHT : public AFESensorsCommon {

private:
  AFEDataAccess *Data;

  boolean _initialized = false;

  unsigned long startTime = 0;

  DHTesp dht;

public:
  DHT configuration;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  float currentTemperature;
  float currentHumidity;
/*
  float currentAbsoluteHumidity;
  float currentDewPoint;
  float currentHeatIndex;
  byte currentPerception;
  float currentComfortRatio;
  ComfortState currentComfortState;
*/


  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  /* Initializing method */
  void begin(AFEDataAccess *, uint8_t id);

  /* Method should be added to the main loop to check temperature / humidity in
   * defined time frame */
  boolean listener();

    /* Returns the sensor data in JSON format */
  void getJSON(char *json);

};

#endif // AFE_CONFIG_HARDWARE_DHT
#endif // _AFE_Sensor_DHT_h
