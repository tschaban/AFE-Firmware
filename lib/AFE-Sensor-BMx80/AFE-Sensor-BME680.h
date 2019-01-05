/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Adafruit_BME680.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBME680 {

private:
  Adafruit_BME680 bme;

public:
  BMx80 configuration;
  BMx80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME680();

  boolean begin();

  boolean read();

  BMx80_DATA get();
};

#endif
