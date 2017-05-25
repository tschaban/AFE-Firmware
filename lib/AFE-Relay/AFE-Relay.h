#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define RELAY_ON 1
#define RELAY_OFF 0

class AFERelay
{

public:
	AFERelay(uint8_t gpio);
	uint8_t get();
	void on();
	void off();
	void toggle();

private:
	uint8_t _gpio;

};

#endif
