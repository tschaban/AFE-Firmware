/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_AS3935_h
#define _AFE_Sensor_AS3935_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Wire.h>
#include <SparkFun_AS3935.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorAS3935 {

private:
  SparkFun_AS3935 AS3935Sensor;

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  

public:
  AS3935 configuration;
  float currentLightLevel = -1;
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 5];

  /* Constructor */
  AFESensorAS3935();

  /* Turns On sensor */
  void begin();

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  void getJSON(char *json);
};

#endif
