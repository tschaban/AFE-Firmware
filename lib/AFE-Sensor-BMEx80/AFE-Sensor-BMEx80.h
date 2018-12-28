/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_BMEx80_h
#define _AFE_Sensor_BMEx80_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Sensor-BME280.h>
#include <AFE-Sensor-BME680.h>

#define TYPE_BME180_SENSOR 1
#define TYPE_BME280_SENSOR 2
#define TYPE_BME680_SENSOR 6

class AFESensorBMEx80 {

private:
  BMEx80_DATA sensorData;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

  uint8_t sensorType;

  AFESensorBME680 s2;
  AFESensorBME680 s6;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMEx80();

  void begin(uint8_t type);

  BMEx80_DATA get();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Return relay IDX in Domoticz */
  void getDomoticzIDX(BMEx80_DOMOTICZ *idx);
};

#endif
