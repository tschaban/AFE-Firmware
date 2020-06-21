/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_AS3935_h
#define _AFE_Sensor_AS3935_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-I2C-Scanner.h>

#include <AS3935.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorAS3935 {

private:
  boolean ready = false;
  

public:
  AS3935 configuration;
  AS3935Sensor AS3935LightingSensor;
  AS3935_EVENT event;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 5];
#endif

  /* Constructor */
  AFESensorAS3935();

  /* Turns On sensor, true if OK, False if failure */
  boolean begin(uint8_t id);

  /* Returnes true if strike has been detected */
  boolean listener();

  /* Returns sensor data in a form of the JSON */
  void getJSON(char *json);
};

#endif
