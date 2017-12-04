/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Sensor_DS18B20_h
#define _AFE_Sensor_DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <DallasTemperature.h>
#include <OneWire.h>

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

class AFESensorDS18B20 {

private:
  uint8_t gpio;
  float correction = 0;
  float previousTemperature = -127;
  boolean ready = false;
  uint8_t unit= 0;

  unsigned long startTime = 0;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDS18B20();
  AFESensorDS18B20(uint8_t sensor_gpio);

  void begin(uint8_t sensor_gpio);

  /* Get current temp in Celsius (default) possible options:
     - UNIT_CELCIUS
    - UNIT_FAHRENHEIT
  */


  // @TODO I think reading temp should be made in the listener, issue to solve how to get temperature but publish only changes
  float get();
  float getPrevious();

  /* If needed set temperature correction value */
  void setCorrection(float sensor_correction);

  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value changes */
  void listener();

};

#endif
