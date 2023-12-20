/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_I2C_Scanner_h
#define _AFE_I2C_Scanner_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_I2C

#include <Arduino.h>
#include <Wire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEI2CScanner {

private:
  boolean portSet = false;
  TwoWire *WirePort;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

public:
#ifdef DEBUG
  AFEI2CScanner(AFEDebugger *);

#else
  AFEI2CScanner();
#endif

void setWire(TwoWire *_Wire);

#ifdef DEBUG
  /**
   * @brief Method only call in the debug mode. It scans all adesses
   *
   */

  void scanAll();
  void scanAll(TwoWire *_Wire);
#endif

  /**
   * @brief Method scans for device presence for inpute address.
   *
   * @param  address          address to scan
   * @return Return true if a device is foun
   */
  
  boolean scan(byte address);
  boolean scan(TwoWire *_Wire, byte address);

  /**
   * @brief Method returns potential name of the device based. Based on default
   * known
   * devices address
   *
   * @param  deviceAddress    device address
   * @return name of a device
   */
  const __FlashStringHelper *getName(byte deviceAddress);
};

#endif // AFE_CONFIG_HARDWARE_I2C
#endif // _AFE_I2C_Scanner_h
