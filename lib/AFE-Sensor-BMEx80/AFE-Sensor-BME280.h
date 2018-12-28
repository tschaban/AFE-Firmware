/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_BME280_h
#define _AFE_Sensor_BME280_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Adafruit_BME280.h>

class AFESensorBME280 {

private:
  Adafruit_BME280 bme;

public:
  BMEx80 configuration;
  BMEx80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME280();

  boolean begin();

  boolean read();

  BMEx80_DATA get();
};

#endif
