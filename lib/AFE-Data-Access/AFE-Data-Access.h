/*

   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <AFE-EEPROM.h>
#include <AFE-Data-Structures.h>


class AFEDataAccess
{
private:
  AFEEEPROM Eeprom;

public:
AFEDataAccess();
NETWORK 	getNetworkConfiguration();
MQTT			getMQTTConfiguration();
RELAY			getRelayConfiguration(uint8_t id);
SWITCH		getSwitchConfiguration(uint8_t id);
DS18B20   getDS18B20Configuration();

};
#endif
