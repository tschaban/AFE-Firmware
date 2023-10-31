/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Rainmeter_h
#define _AFE_Sensor_Rainmeter_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_RAINMETER

#include <AFE-Data-Access.h>
#include <AFE-Impulse-Catcher.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFERainmeter
{

public:
  RAINMETER *configuration = new RAINMETER;

  RAINMETER_DATA *current = new RAINMETER_DATA;
  float rainLevelLast1Minute = 0;
  float rainLevelLastHour = 0;
#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
  float rainLevelLast12Hours = 0;
  float rainLevelLast24Hours = 0;
#endif

  /* Constructors */
  AFERainmeter();

/* Init switch */
#ifdef DEBUG
  boolean begin(AFEDataAccess *, AFEImpulseCatcher *, AFEDebugger *);
#else
  boolean begin(AFEDataAccess *, AFEImpulseCatcher *);
#endif
  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

  boolean listener(void);

private:
  AFEImpulseCatcher *_Sensor;
  AFEDataAccess *_Data;
  boolean _initialized = false;
  uint32_t startTime = 0;
  uint32_t start60Sec = 0;
#ifdef DEBUG
  AFEDebugger *Debugger;
  boolean begin(AFEDataAccess *, AFEImpulseCatcher *);
#endif
};

#endif // AFE_CONFIG_HARDWARE_RAINMETER
#endif
