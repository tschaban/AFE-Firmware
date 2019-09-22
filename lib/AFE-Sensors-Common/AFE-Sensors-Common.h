/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensors_Common_h
#define _AFE_Sensors_Common_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

#include <AFE-Configuration.h>

class AFESensorsCommon {

private:
public:
  /* Constructor */
  AFESensorsCommon();

  float celsiusToFerenheit(float input);
  float dewPoint(float temperature, float humidity);
  float relativePressure(float pressure, float alt, float temperature);
  
  #ifdef AFE_CONFIG_HARDWARE_BMEX80
  afe_iaq_ratings_t iaqRating(uint16_t iaq);
  afe_co2_ratings_t co2Rating(uint16_t co2);
  #endif

  #ifdef AFE_CONFIG_HUMIDITY
  afe_humidity_ratings_t humidityRating(float humidity);
  #endif

};

#endif
