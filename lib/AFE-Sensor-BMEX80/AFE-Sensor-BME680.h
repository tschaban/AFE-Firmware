/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <Bsec.h>
//#include <bsec_serialized_configurations_iaq.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBME680 {

private:
  Bsec Bme;

public:
  BMEX80 *configuration;
  BMEX80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME680();

  boolean begin(BMEX80 *, I2CPORT *);

  boolean read();

#ifdef DEBUG
  void checkBmeStatus();
#endif

};

#endif
