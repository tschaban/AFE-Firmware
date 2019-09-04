/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-BMx80-Sensor-Structure.h>
#include <Adafruit_BME680.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBME680 {

private:
  Adafruit_BME680 bme;

public:
  BMx80 *configuration;
  BMx80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME680();

  boolean begin(BMx80 *);

  boolean read();

  BMx80_DATA get();
};

#endif
