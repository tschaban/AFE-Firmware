/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Rainmeter_h
#define _AFE_Sensor_Rainmeter_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR


#include <AFE-Data-Access.h>
#include <AFE-Sensor-Binary.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorRainmeter {

public:
  RAINMETER configuration;
  
  RAINMETER_DATA current;
  float rainLevelLast1Minute = 0;
  float rainLevelLastHour = 0;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  float rainLevelLast12Hours = 0;
  float rainLevelLast24Hours = 0;
#endif

  /* Constructors */
  AFESensorRainmeter();

  /* Init switch */
  boolean begin(AFEDataAccess *, AFESensorBinary *);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

  boolean listener(void);

private:
  AFESensorBinary *_Sensor;
  AFEDataAccess *_Data;
  boolean _initialized = false;
  uint32_t startTime = 0;
  uint32_t start60Sec = 0;
};

#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
#endif
