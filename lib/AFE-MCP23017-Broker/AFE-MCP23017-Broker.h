/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23107_Broker_h
#define _AFE_MCP23107_Broker_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Wire-Container.h>
#include <Adafruit_MCP23X17.h>


#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEMCP23017Broker {

private:
  AFEDataAccess *Data;
  AFEDebugger *Debugger;
  AFEDevice *Device;
  AFEWireContainer *WirePort;

  MCP23XXX configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23XXX];

public:
  Adafruit_MCP23X17 MCP[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23XXX];

/**
 * @brief Construct a new AFEMCP23017Broker object
 *
 */
#ifdef DEBUG
  AFEMCP23017Broker(AFEDataAccess *_Data, AFEDevice *_Device,
                    AFEWireContainer *_Wire, AFEDebugger *_Debugger);
#else
  AFEMCP23017Broker(AFEDataAccess *_Data, AFEDevice *_Device,
                    AFEWireContainer *_Wire);
#endif

  void begin();
};
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#endif // _AFE_MCP23107_Broker_h
