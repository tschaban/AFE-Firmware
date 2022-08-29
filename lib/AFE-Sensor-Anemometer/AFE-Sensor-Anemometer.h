/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Wind_h
#define _AFE_Sensor_Wind_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER

#include <arduino.h>
#include <AFE-Data-Access.h>
#include <AFE-Impulse-Catcher.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAnemometer {

public:
  ANEMOMETER *configuration = new ANEMOMETER; 
  float lastSpeedMS =
      0; // used by HTTPs API - stores and gets the lastest value by HTTP API
  float lastSpeedKMH =
      0; // used by HTTPs API - stores and gets the lastest value by HTTP API

  /* Constructors */
  AFEAnemometer();

  /* Init switch */
  boolean begin(AFEDataAccess *, AFEImpulseCatcher *);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

  boolean listener(void);

private:
  AFEImpulseCatcher *_Sensor;
  AFEDataAccess *_Data;
  boolean _initialized = false;
  uint32_t startTime = 0;
  float oneImpulseDistanceCM = 0;


  boolean  active = false;
  uint32_t impulseCounter = 0;
  uint32_t counterStarted = 0;
  uint32_t _previousDuration = 0; // Used in case of time rollout

  void newImpulse(void);
  void get(uint32_t &noOfImpulses, uint32_t &duration);


};

#endif // AFE_CONFIG_HARDWARE_ANEMOMETER
#endif // _AFE_Sensor_Wind_h
