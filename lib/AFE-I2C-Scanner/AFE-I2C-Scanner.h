/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */


#ifndef _AFE_I2C_Scanner_h
#define _AFE_I2C_Scanner_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_I2C

#include <arduino.h>

#include <Wire.h>
#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEI2CScanner {


private:

TwoWire WirePort;

public:
  AFEI2CScanner();

  void begin();

#ifdef DEBUG
  /* Method only call in the debug mode. It scans all adesses */
  void scanAll();
#endif

  /* Method scans for device presence for inpute address. Return true if a device is found  */
  boolean scan(byte address);

  /* Method returns potential name of the device based. Based on default known devices addresses */
  const char *getName(byte deviceAddress);
};

#endif // AFE_CONFIG_HARDWARE_I2C
#endif // _AFE_I2C_Scanner_h
