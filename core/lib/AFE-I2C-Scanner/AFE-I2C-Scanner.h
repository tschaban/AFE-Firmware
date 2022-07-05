/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_I2C_Scanner_h
#define _AFE_I2C_Scanner_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_I2C

#include <arduino.h>

#include <AFE-Data-Access.h>
#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEI2CScanner {

private:
TwoWire *WirePort;

public:
  AFEI2CScanner();

/**
 * @brief  Initialization of I2C Scanner
 * 
 * @param  _WirePort        reference to TwoWire
 */
  void begin(TwoWire *_WirePort);

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
  const char *getName(byte deviceAddress);
};

#endif // AFE_CONFIG_HARDWARE_I2C
#endif // _AFE_I2C_Scanner_h
