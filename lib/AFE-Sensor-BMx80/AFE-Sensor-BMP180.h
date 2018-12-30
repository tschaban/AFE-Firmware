/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_BMP180_h
#define _AFE_Sensor_BMP180_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <Adafruit_BMP085.h>

class AFESensorBMP180 {

private:
  Adafruit_BMP085 bme;

public:
  BMx80 configuration;
  BMx80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMP180();

  boolean begin();

  boolean read();

  BMx80_DATA get();
};

#endif
