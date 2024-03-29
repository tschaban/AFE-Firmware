/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMEX80_h
#define _AFE_Sensor_BMEX80_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <AFE-Data-Access.h>
#include <AFE-Sensor-BME280.h>
#include <AFE-Sensor-BME680.h>
#ifndef AFE_ESP32
#include <AFE-Sensor-BMP180.h>
#endif // AFE_ESP32
#include <AFE-Sensors-Common.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Arduino.h>

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
#ifndef AFE_ESP32 // Sensor doesn't work with current libraries
  AFESensorBMP180 *s1 = new AFESensorBMP180();
#endif // ESP32
  AFESensorBME280 *s2 = new AFESensorBME280();
  AFESensorBME680 *s6 = new AFESensorBME680();

  void applyCorrections();

  TwoWire *_WirePort;
  
#ifdef AFE_ESP32
  void begin(uint8_t id, TwoWire *WirePort);
#endif

public:
  BMEX80 *configuration = new BMEX80;
  BMEX80_DATA *data = new BMEX80_DATA;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMEX80();

/* Initialization of the sensor */
#ifdef AFE_ESP32
  void begin(uint8_t id, TwoWire *WirePort0, TwoWire *WirePort1);
#else
  void begin(uint8_t id, TwoWire *WirePort);
#endif

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
