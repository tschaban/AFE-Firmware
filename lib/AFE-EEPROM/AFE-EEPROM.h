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

#define EEPROM_SIZE 512

class AFEEEPROM
{
  private:

  public:
    AFEEEPROM();

    /* it stories String in EEPROM starting from position of input
		parameter address of length defined by input parameter size */

    void write(uint16_t address, uint16_t size, String in);

    /* it's stores int (long) in EEPROM - there is automated conversion */
		void write(uint16_t address, uint16_t size, long in);

		/* it's stores float in EEPROM - there is automated conversion */
		void write(uint16_t address, uint16_t size, float in);

    /* it's stores boolean in EEPROM - there is automated conversion.
		Boolean is stored as 0 or 1 in EEPROM.
		*/
		void write(uint16_t address, boolean in);

    /* Returns string from EEPORM starting from position of input parameter
		address of length defined by input parameter size
		*/
		String 	read(uint16_t address, uint16_t size);

		/* Returns boolean from EEPORM from position of address */
		boolean read(uint16_t address);

    /* It clears EEPROM from position starting from address and size
		long
		*/
		void clear(uint16_t address, uint16_t size);

    /* It erase entire EEPROM */
		void erase();

};
#endif
