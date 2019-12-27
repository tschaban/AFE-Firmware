/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_ADC_VCC_h
#define _AFE_Sensor_ADC_VCC_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAnalogInput {

private:
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;
  uint8_t counterOfSamplings = 0;

public:
  ADCINPUT configuration;
  ADCINPUT_DATA data;

#ifdef AFE_CONFIG_API_MQTT_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 5];
#endif

  /* Constructor */
  AFEAnalogInput();

  /* Initialized analog input using configuration parameters */
  void begin();

  /* Returns values from Analog Input */
  ADCINPUT_DATA get();

  /* Returns true if there is a new value available from analog input */
  boolean isReady();

  /* Main method that takes care for analog reads based on config parameters */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif
