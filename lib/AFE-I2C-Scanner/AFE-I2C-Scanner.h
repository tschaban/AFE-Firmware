/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_I2C_Scanner_h
#define _AFE_I2C_Scanner_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_I2C

#include <Arduino.h>

#include <AFE-Data-Access.h>
#include <Wire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEI2CScanner
{

private:
  TwoWire *WirePort;

#ifdef DEBUG
  AFEDebugger *Debugger;
  void begin(TwoWire *_WirePort);
#endif

public:
  AFEI2CScanner();

  /**
   * @brief  Initialization of I2C Scanner
   *
   * @param  _WirePort        reference to TwoWire
   */

#ifdef DEBUG
  void begin(TwoWire *_WirePort, AFEDebugger *_Debugger);
#else
  void begin(TwoWire *_WirePort);
#endif

#ifdef DEBUG
  /**
   * @brief Method only call in the debug mode. It scans all adesses
   *
   */

  void scanAll();
#endif

  /**
   * @brief Method scans for device presence for inpute address.
   *
   * @param  address          address to scan
   * @return Return true if a device is foun
   */
  boolean scan(byte address);

  /**
   * @brief Method returns potential name of the device based. Based on default known
   * devices address
   *
   * @param  deviceAddress    device address
   * @return name of a device
   */
  const __FlashStringHelper *getName(byte deviceAddress);
};

#endif // AFE_CONFIG_HARDWARE_I2C
#endif // _AFE_I2C_Scanner_h
