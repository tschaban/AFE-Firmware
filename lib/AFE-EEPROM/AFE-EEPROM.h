/*
   EEPROM component
   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_EEPROM_h
#define _AFE_EEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <EEPROM.h>
#include <ESP8266WiFi.h>

#define EEPROM_SIZE 512

class AFEEEPROM
{
private:

public:
AFEEEPROM();

/* it stories String in EEPROM starting from position of input parameter size */
void write(uint16_t address, uint16_t size, String in);
void write(uint16_t address, uint16_t size, long in);
void write(uint16_t address, uint16_t size, float in);
void write(uint16_t address, boolean in);


/* Returns string from EEPORM starting from position of input parameter address of length defined by input parameter size */
String  read(uint16_t address, uint16_t size);
boolean read(uint16_t address);

/* Returns IP Address */
IPAddress readIP(uint16_t address);

/* Those two could be used to store 0-255 int under one address */
void writeUInt8(uint16_t address, uint8_t in);
uint8_t readUInt8(uint16_t address);

/* It clears EEPROM from position starting from address and size long */
void clear(uint16_t address, uint16_t size);

/* It erase entire EEPROM */
void erase();

};
#endif
