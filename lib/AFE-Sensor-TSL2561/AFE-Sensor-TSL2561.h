/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_TSL2561_h
#define _AFE_Sensor_TSL2561_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_TSL2561

#include <Arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-Wire-Container.h>
#include <Adafruit_TSL2561_U.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorTSL2561 {

private:
  Adafruit_TSL2561_Unified *tls2561 = new Adafruit_TSL2561_Unified();

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  AFEWireContainer *WirePort;
  AFEDataAccess *Data;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

public:
  TSL2561 *configuration = new TSL2561;
  /* Sensor's data */
  uint32_t illuminance = 0;
  uint16_t broadband, ir;

/* Constructor */
#ifdef DEBUG
  AFESensorTSL2561(AFEDataAccess *, AFEWireContainer *, AFEDebugger *);
#else
  AFESensorTSL2561(AFEDataAccess *, AFEWireContainer *);
#endif

  /* Turns On sensor */
  void begin(uint8_t _id);

  /* Method has to be added to the loop in order to listen for sensor value
   * changes, Is true when data has been read from the sensor */
  boolean listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_TSL2561
#endif // _AFE_Sensor_TSL2561_h
