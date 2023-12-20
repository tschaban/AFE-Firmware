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
#include <AFE-Wire-Container.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorBMEX80 : public AFESensorsCommon {

private:
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;

#ifndef AFE_ESP32 // Sensor doesn't work with current libraries
#ifdef DEBUG
  AFESensorBMP180 *s1 = new AFESensorBMP180(configuration, data, Debugger);
#else
  AFESensorBMP180 *s1 = new AFESensorBMP180(configuration, data);
#endif
#endif // ESP32

#ifdef DEBUG
  AFESensorBME280 *s2 = new AFESensorBME280(configuration, data, Debugger);
#else
  AFESensorBME280 *s2 = new AFESensorBME280(configuration, data);
#endif

#ifdef DEBUG
  AFESensorBME680 *s6 = new AFESensorBME680(configuration, data, Debugger);
#else
  AFESensorBME680 *s6 = new AFESensorBME680(configuration, data);
#endif

  void applyCorrections();

  AFEWireContainer *WirePort;
  AFEDataAccess *Data;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

public:
  BMEX80 *configuration = new BMEX80;
  BMEX80_DATA *data = new BMEX80_DATA;

/* Constructor: entry parameter is GPIO number where Sensor is connected to */
#ifdef DEBUG
  AFESensorBMEX80(AFEDataAccess *, AFEWireContainer *, AFEDebugger *);
#else
  AFESensorBMEX80(AFEDataAccess *, AFEWireContainer *);
#endif

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
