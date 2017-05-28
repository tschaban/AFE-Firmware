/*
  WiFi component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <Streaming.h>

class AFEWiFi {

private:
   boolean _accessPoint = false;
	 uint8_t _no_connection_attempts = 20;
   uint8_t _duration_between_connection_attempts = 1;
   uint8_t _duration_between_next_connection_attempts_series = 60;
	 boolean _sleepMode = false;

public:
	AFEWiFi();

	void connect(const char* ssid, const char* password);

};

#endif
