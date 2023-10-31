/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BME280_h
#define _AFE_Sensor_BME280_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <Arduino.h>
#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorBME280
{

private:
  Adafruit_BME280 *bme = new Adafruit_BME280();
  BMEX80_DATA *data = new BMEX80_DATA;

#ifdef DEBUG
  AFEDebugger *Debugger;
  boolean begin(BMEX80 *_configuration, TwoWire *_wirePort, BMEX80_DATA *_data);
#endif

public:
  BMEX80 *configuration = new BMEX80;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME280();

#ifdef DEBUG
  boolean begin(BMEX80 *_configuration, TwoWire *_wirePort, AFEDebugger *_Debugger, BMEX80_DATA *_data);
#else
  boolean begin(BMEX80 *_configuration, TwoWire *_wirePort, BMEX80_DATA *_data);
  #endif
  boolean read();
};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // _AFE_Sensor_BME280_h
