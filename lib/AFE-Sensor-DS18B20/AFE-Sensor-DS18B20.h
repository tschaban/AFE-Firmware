/*
  DS18B20 sensor component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

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

class AFESensorDS18B20
{

private:
  uint8_t gpio;
	float correction = 0;

public:

	/* Constructor: entry parameter is GPIO number where Sensor is connected to */
	AFESensorDS18B20(uint8_t sensor_gpio);

  /* Get current temp in Celsius (default) possible options:
     - UNIT_CELCIUS
		 - UNIT_FAHRENHEIT
	*/
	float get(uint8_t unit=0);

  /* If needed set temperature correction value */
	void setCorrection(float sensor_correction);

};

#endif
