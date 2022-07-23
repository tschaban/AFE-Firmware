/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23107_Broker_h
#define _AFE_MCP23107_Broker_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-I2C-Scanner.h>
#include <Adafruit_MCP23X17.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEMCP23017Broker {

private:
  I2CPORT _I2C;
  AFEI2CScanner _I2CScanner;
  AFEDataAccess *_Data;
  AFEDevice *_Device;
  TwoWire *_WirePort0;
#ifdef AFE_ESP32
  TwoWire _WirePort0;
#endif

  MCP23XXX configuration[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23XXX];

public:
  Adafruit_MCP23X17 MCP[AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23XXX];

  /**
   * @brief Construct a new AFEMCP23017Broker object
   * 
   */
  AFEMCP23017Broker();

#ifdef AFE_ESP32
  /**
   * @brief Initialize MCP + adds to the cache all items Relays, LEDs, etc. IDs
   * connected to MCP
   *
   * @param  Data             reference to database
   * @param  WirePort0        reference to WirePort0
   * @param  WirePort1        reference to WirePort1
   */
  void begin(AFEDataAccess *Data, TwoWire *WirePort0,
             TwoWire *WirePort1);
#else
  /**
   * @brief Initialize MCP + adds to the cache all items Relays, LEDs, etc. IDs
   * connected to MCP
   *
   * @param  Data             reference to database
   * @param  WirePort0        reference to WirePort0
   */
  void begin(AFEDataAccess *Data,  TwoWire *WirePort0);
#endif


};
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#endif // _AFE_MCP23107_Broker_h
