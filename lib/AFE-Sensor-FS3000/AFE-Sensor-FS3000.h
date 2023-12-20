/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_FS3000_h
#define _AFE_Sensor_FS3000_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_FS3000

#include <AFE-Data-Access.h>
#include <AFE-Wire-Container.h>
#include <SparkFun_FS3000_Arduino_Library.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorFS3000 {

public:
  struct FS3000_DATA {
    uint16_t raw;
    float metersPerSecond;
    float milesPerHour;
    float meters3PerHour;
  };

  FS3000_CONFIG *configuration = new FS3000_CONFIG;
  FS3000_DATA *data = new FS3000_DATA;

/* Constructors */
#ifdef DEBUG
  AFESensorFS3000(AFEDataAccess *, AFEWireContainer *, AFEDebugger *);
#else
  AFESensorFS3000(AFEDataAccess *, AFEWireContainer *);
#endif

  boolean begin(uint8_t id);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

  boolean listener(void);

private:
  FS3000 *_fs3000 = new FS3000();
  boolean _initialized = false;
  unsigned long _startTime = 0;

  AFEDataAccess *Data;
  AFEWireContainer *WirePort;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif
};

#endif // AFE_CONFIG_HARDWARE_FS3000
#endif
