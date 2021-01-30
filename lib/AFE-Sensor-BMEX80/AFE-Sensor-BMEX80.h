/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMEX80_h
#define _AFE_Sensor_BMEX80_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-Sensor-BME280.h>
#include <AFE-Sensor-BME680.h>
#include <AFE-Sensor-BMP180.h>
#include <AFE-Sensors-Common.h>
#include <ArduinoJson.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif


class AFESensorBMEX80 : public AFESensorsCommon {

private:
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

  AFESensorBMP180 s1;
  AFESensorBME280 s2;
  AFESensorBME680 s6;

  void applyCorrections();

public:
  BMEX80 configuration;
  BMEX80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMEX80();

  /* Initialization of the sensor */
  void begin(uint8_t id);

  /* Returns sensor data in JSON format */
  void getJSON(char *);

  /* True if data in a buffer - ready to read */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // _AFE_Sensor_BMEX80_h
