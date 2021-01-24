/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_Binary_h
#define _AFE_Sensor_Binary_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

#include <AFE-Data-Access.h>

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Broker.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBinary {

private:
  boolean _initialized = false;
  boolean _detected = false;
  unsigned long startTime = 0;
  byte state; // stores actual sensor state


#ifdef AFE_CONFIG_HARDWARE_MCP23017
  AFEMCP23017Broker *_MCP23017Broker;
  boolean _MCP23017ReferenceAdded = false;
  boolean _expanderUsed = false;
  uint8_t _MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

public:
  BINARY_SENSOR configuration;

  /* Constructors */
  AFESensorBinary();

  /* Init switch */
  void begin(uint8_t id, AFEDataAccess *);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  void addMCP23017Reference(AFEMCP23017Broker *);
#endif

  /* Get state of the sensor */
  byte get(void);

  /* Listens for state changes, taking into account bouncing */
  boolean listener(void);

    /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#endif // _AFE_Sensor_Binary_h
