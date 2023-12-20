/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-MCP23017-Broker.h"

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#ifdef DEBUG
AFEMCP23017Broker::AFEMCP23017Broker(AFEDataAccess *_Data, AFEDevice *_Device,
                                     AFEWireContainer *_Wire,
                                     AFEDebugger *_Debugger) {
  Data = _Data;
  Device = _Device;
  WirePort = _Wire;
  Debugger = _Debugger;
};
#else
AFEMCP23017Broker::AFEMCP23017Broker(AFEDataAccess *_Data, AFEDevice *_Device,
                                     AFEWireContainer *_Wire) {
  Data = _Data;
  Device = _Device;
  WirePort = _Wire;
};
#endif

void AFEMCP23017Broker::begin() {

#ifdef DEBUG
  Debugger->printInformation(F("initializing"), F("MCP23xxx"));
#endif

  for (uint8_t i = 0; i < Device->configuration.noOfMCP23xxx; i++) {

#ifdef AFE_ESP32
    if (configuration[i].wirePortId == AFE_CONFIG_HARDWARE_I2C_0) {
      WirePort->Scanner->setWire(WirePort->Port0);
    } else {
      WirePort->Scanner->setWire(WirePort->Port1);
    }
#else // ESP8266
    WirePort->Scanner->setWire(WirePort->Port0);
#endif

    Data->getConfiguration(i, &configuration[i]);

    if (WirePort->Scanner->scan(configuration[i].address)) {

#ifdef DEBUG
      Debugger->printInformation(F("expander address: 0x"), F("MCP23xxx"));
      Serial << _HEX(configuration[i].address);
#endif

#ifdef AFE_ESP32
      if (MCP[i].begin_I2C(configuration[i].address,
                           configuration[i].wirePortId ==
                                   AFE_CONFIG_HARDWARE_I2C_0
                               ? WirePort->Port0
                               : WirePort->Port1))
#else // ESP8266
      if (MCP[i].begin_I2C(configuration[i].address, WirePort->Port0))
#endif
      {
#ifdef DEBUG
        Debugger->printInformation(F("Initialized"), F("MCP23xxx"));
      } else {
        Debugger->printError(F("Initialization failure"), F("MCP23xxx"));
#endif
      }

#ifdef DEBUG
    } else {
      Debugger->printError(F("Device not found under I2C Address: 0x"),
                           F("MCP23xxx"));
      Serial << _HEX(configuration[i].address);
#endif
    }
  }

  delete Data;
  Data = NULL;
  delete Device;
  Device = NULL;

#ifdef DEBUG
  delete Debugger;
  Debugger = NULL;
#endif
}

#endif // AFE_CONFIG_HARDWARE_MCP23XXX