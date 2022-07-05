/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23107_Broker_h
#define _AFE_MCP23107_Broker_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_MCP23017

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-I2C-Scanner.h>
#include <Adafruit_MCP23017.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEMCP23017Broker {

private:
  MCP23017_CACHE _cache[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23017];
  uint8_t _currentCacheIndex = 0;
  I2CPORT _I2C;
  AFEI2CScanner _I2CScanner;

  /* Adds to item to the cache */
  void add(uint8_t address);

public:
  Adafruit_MCP23017 MCP[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23017];

  /* Constructors */
  AFEMCP23017Broker();

  /* Initialize MCP + adds to the cache all items Relays, LEDs, etc. IDs
   * connected
   * to MCP */
  void begin(AFEDataAccess *, AFEDevice *);

  /* Gets ID of the MCP based on it's address. Used for caching */
  uint8_t getId(uint8_t address);
};
#endif // AFE_CONFIG_HARDWARE_MCP23017
#endif // _AFE_MCP23107_Broker_h
