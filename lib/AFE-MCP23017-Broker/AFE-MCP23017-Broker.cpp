/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-MCP23017-Broker.h"

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX

AFEMCP23017Broker::AFEMCP23017Broker(){};

#ifdef AFE_ESP32
void AFEMCP23017Broker::begin(AFEDataAccess *Data, AFEDevice *Device,
                              TwoWire *WirePort0, woWire *WirePort1) {
#else
void AFEMCP23017Broker::begin(AFEDataAccess *Data, AFEDevice *Device,
                              TwoWire *WirePort0) {
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: MCP23017 Broker: initializing");
#endif

  AFEI2CScanner I2CScanner;
  _WirePort0 = WirePort0;

  for (uint8_t i = 0; i < Device->configuration.noOfMCP23xxx; i++) {

#ifdef AFE_ESP32
    if (configuration[i].wirePortId == AFE_CONFIG_HARDWARE_I2C_0) {
      I2CScanner.begin(_WirePort0);
    } else {
      I2CScanner.begin(_WirePort1);
    }
#else // ESP8266
    I2CScanner.begin(_WirePort0);
#endif

    Data->getConfiguration(i, &configuration[i]);

    if (I2CScanner.scan(configuration[i].address)) {

#ifdef DEBUG
      Serial << endl
             << F(": MCP23XXX expander address: 0x")
             << _HEX(configuration[i].address);
#endif

#ifdef AFE_ESP32
      if (begin_I2C(configuration[i].address,
                    configuration[i].wirePortId == AFE_CONFIG_HARDWARE_I2C_0
                        ? _WirePort0
                        : _WirePort1))
#else // ESP8266
      if (MCP[i].begin_I2C(configuration[i].address, _WirePort0))
#endif
      {
#ifdef DEBUG
        Serial << endl << F("INFO: MCP23XXX: Initialized");
      } else {
        Serial << endl << F("ERROR: MCP23XXX: Initialization failure");
#endif
    }

#ifdef DEBUG
  }
  else {
    Serial << endl
           << F(": Error: Device not found under I2C Address: 0x")
           << _HEX(configuration[i].address);
#endif
  }
}
}

#endif // AFE_CONFIG_HARDWARE_MCP23XXX