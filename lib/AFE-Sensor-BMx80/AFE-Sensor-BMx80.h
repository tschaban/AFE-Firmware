/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMx80_h
#define _AFE_Sensor_BMx80_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

#include <AFE-Sensor-BME280.h>
#include <AFE-Sensor-BME680.h>
#include <AFE-Sensor-BMP180.h>

#define TYPE_BMP180_SENSOR 1
#define TYPE_BME280_SENSOR 2
#define TYPE_BME680_SENSOR 6

class AFESensorBMx80 {

private:
  BMx80_DATA sensorData;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

  uint8_t sensorType;

  AFESensorBMP180 s1;
  AFESensorBME280 s2;
  AFESensorBME680 s6;

public:
  BMx80 configuration;
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 5];

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMx80();

  void begin(uint8_t id);

  BMx80_DATA get();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();
};

#endif
