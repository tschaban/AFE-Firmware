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

public:
  AS3935 configuration;
  uint8_t distance ;


  /* Constructor */
  AFESensorAS3935();

  /* Turns On sensor */
  void begin();

  void interruptionReported();

  boolean strikeDetected();

  void getJSON(char *json);
};

#endif
