/*
  Relay component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/* Relay states */
#define RELAY_ON 1
#define RELAY_OFF 0

class AFERelay
{

private:
	uint8_t _gpio;

public:

	/* Constructor: entry parameter is GPIO number where Relay is connected to */
	AFERelay(uint8_t gpio);

	/* Returns 0 if relay is OFF, 1 if relay is ON */
	uint8_t get();

	/* Turns on relay */
	void on();

	/* Turns off relay */
	void off();

	/* Toggles relay state from ON to OFF or from OFF to ON */
	void toggle();
};

#endif
