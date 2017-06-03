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
#include <DNSServer.h>
#include <Streaming.h>

#define WIFI_MODE_CLIENT 0
#define WIFI_MODE_SERVER 1
#define WIFI_MODE_AP 2


class AFEWiFi {

private:
const char*   ssid;
const char*  password;
boolean accessPoint = false;
uint8_t noConnectionAttempts = 20;
uint8_t durationBetweenConnectionAttempts = 1;
uint8_t durationBetweenNextConnectionAttemptsSeries = 60;
unsigned long sleepStartTime = 0;
boolean sleepMode = false;
DNSServer dnsServer;

public:

/* Constructor: no actions */
AFEWiFi();

/* Sets connectionparameters and host name. Must be invoked before connect method */
void begin(const char* network_ssid, const char* network_password, const char* netowork_hostname, uint8_t mode=WIFI_MODE_CLIENT);

/* Sets parameters related to reconnection to WiFi Network if there was problems to connect to it */
void setReconnectionParams(
								uint8_t no_connection_attempts,
								uint8_t duration_between_connection_attempts,
								uint8_t duration_between_next_connection_attempts_series);

/* Connecting to WiFi Access point */
void connect();

/* Return TRUE if device is connected to WiFi Acces Point */
boolean connected();


};

#endif
