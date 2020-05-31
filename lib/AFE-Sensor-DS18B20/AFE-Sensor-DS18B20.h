/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_DS18B20_h
#define _AFE_Sensor_DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif


class AFESensorDS18B20 {

private:
  DS18B20 configuration;
  OneWire WireBUS;
  DallasTemperature Sensor;

  float currentTemperature = -127;
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;



public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDS18B20();

  void begin(uint8_t id);

  uint8_t numberOfDevicesOnBus = 0;
  DeviceAddress addressesOfScannedItems[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20];
  void scan(uint8_t gpio);
  

  /* Get current temp in Celsius (default) possible options:
     - UNIT_CELCIUS
    - UNIT_FAHRENHEIT
  */

  // @TODO I think reading temp should be made in the listener, issue to solve
  // how to get temperature but publish only changes
  float getTemperature();

  float getLatestTemperature();

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Return relay IDX in Domoticz */
  unsigned long getDomoticzIDX();
#endif
};

#endif
