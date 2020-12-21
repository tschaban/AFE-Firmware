/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-MCP23017-Broker.h"

#ifdef AFE_CONFIG_HARDWARE_MCP23017

AFEMCP23017Broker::AFEMCP23017Broker() {
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23017; i++) {
    _cache[i].id = i;
    _cache[i].address = AFE_HARDWARE_ITEM_NOT_EXIST;
  }
#ifdef DEBUG
  Serial << endl << "INFO: MCP23017 Broker: initialized";
#endif
}

void AFEMCP23017Broker::begin(AFEDataAccess *Data, AFEDevice *Device) {
  Data->getConfiguration(&_I2C);
  _I2CScanner.begin();

#ifdef AFE_CONFIG_HARDWARE_LED
#ifdef DEBUG
  Serial << endl
         << F("INFO: MCP23017 Broker: Caching MCP23017s ID's used by LEDs");
#endif
  for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
    LED LEDConfiguration;
    Data->getConfiguration(i, &LEDConfiguration);
    if (LEDConfiguration.mcp23017.address != AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (getId(LEDConfiguration.mcp23017.address) ==
          AFE_HARDWARE_ITEM_NOT_EXIST) {
        add(LEDConfiguration.mcp23017.address);
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("INFO: MCP23017 Broker: MCP23017[0x")
               << _HEX(LEDConfiguration.mcp23017.address)
               << F("] already in cache");
      }
#endif
    }
  }
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
  Serial << endl
         << F("INFO: MCP23017 Broker: Caching MCP23017s ID's used by Relays");
#endif
  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    RELAY RelayConfiguration;
    Data->getConfiguration(i, &RelayConfiguration);
    if (RelayConfiguration.mcp23017.address != AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (getId(RelayConfiguration.mcp23017.address) ==
          AFE_HARDWARE_ITEM_NOT_EXIST) {
        add(RelayConfiguration.mcp23017.address);
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("INFO: MCP23017 Broker: MCP23017[0x")
               << _HEX(RelayConfiguration.mcp23017.address)
               << F("] already in cache");
      }
#endif
    }
  }
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
  Serial << endl
         << F("INFO: MCP23017 Broker: Caching MCP23017s ID's used by Relays");
#endif
  for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
    SWITCH SwitchConfiguration;
    Data->getConfiguration(i, &SwitchConfiguration);
    if (SwitchConfiguration.mcp23017.address != AFE_HARDWARE_ITEM_NOT_EXIST) {
      if (getId(SwitchConfiguration.mcp23017.address) ==
          AFE_HARDWARE_ITEM_NOT_EXIST) {
        add(SwitchConfiguration.mcp23017.address);
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("INFO: MCP23017 Broker: MCP23017[0x")
               << _HEX(SwitchConfiguration.mcp23017.address)
               << F("] already in cache");
      }
#endif
    }
  }
#endif // AFE_CONFIG_HARDWARE_SWITCH
}

uint8_t AFEMCP23017Broker::getId(uint8_t address) {
  uint8_t _ret = AFE_HARDWARE_ITEM_NOT_EXIST;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23017; i++) {
    if (_cache[i].address == address) {
      _ret = _cache[i].id;
      break;
    }
  }
  return _ret;
}

void AFEMCP23017Broker::add(uint8_t address) {
#ifdef DEBUG
  Serial << endl
         << F("INFO: MCP23017 Broker: Initializing MCP23017[0x")
         << _HEX(address) << F("]");

#endif

  if (_I2CScanner.scan(address)) {
    MCP[_currentCacheIndex].begin(address, _I2C.SDA, _I2C.SCL);
    _cache[_currentCacheIndex].id = _currentCacheIndex;
    _cache[_currentCacheIndex].address = address;
    _currentCacheIndex++;
#ifdef DEBUG
    Serial << endl
           << F("INFO: MCP23017 Broker: MCP23017[0x") << _HEX(address)
           << F("] added to the cache");

#endif
  }
#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: MCP23017 Broker: MCP23017[0x") << _HEX(address)
           << F("] has not been found on I2C");
  }
#endif
}

#endif // AFE_CONFIG_HARDWARE_MCP23017