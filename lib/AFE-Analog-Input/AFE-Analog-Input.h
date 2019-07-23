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
  ADCINPUT configuration;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;
  uint16_t analogData = 0;
  uint16_t temporaryAnalogData = 0;
  uint8_t counterOfSamplings = 0;

  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];

public:
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

  /* Returns IDX values for the integration with Domoticz */
  void getDomoticzIDX(ADCINPUT_DOMOTICZ *idx);

  /* Method returns MQTT topic for analog input */
  const char *getMQTTCommandTopic();
  const char *getMQTTStateTopic();
};

#endif
