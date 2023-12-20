/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BH1750_h
#define _AFE_Sensor_BH1750_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BH1750

#include <AFE-Data-Access.h>
#include <AFE-Wire-Container.h>
#include <Arduino.h>
#include <BH1750.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorBH1750 {

private:
  BH1750 *bh1750 = new BH1750();
  AFEDataAccess *Data;
  AFEWireContainer *WirePort;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

public:
  BH1750_CONFIG *configuration = new BH1750_CONFIG;
  float data = -1; // stories lux value

/* Constructor */
#ifdef DEBUG
  AFESensorBH1750(AFEDataAccess *, AFEWireContainer *, AFEDebugger *);
#else
  AFESensorBH1750(AFEDataAccess *, AFEWireContainer *);
#endif

  /* Turns On sensor */
  void begin(uint8_t _id);

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_BH1750
#endif // _AFE_Sensor_BH1750_h
