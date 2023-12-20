
#ifndef _AFE_Wire_Container_h
#define _AFE_Wire_Container_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_I2C

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-I2C-Scanner.h>
#include <Wire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEWireContainer {

private:
  AFEDevice *Device;
  AFEDataAccess *Data;
#ifdef DEBUG
  AFEDebugger *Debugger;
  void printConfiguration(I2CPORT *);
#endif

public:
#ifdef AFE_ESP32
  TwoWire *Port0 = new TwoWire(0);
  TwoWire *Port1 = new TwoWire(1);
#else
  TwoWire *Port0 = new TwoWire();
#endif

#ifdef DEBUG
  AFEWireContainer(AFEDevice *, AFEDataAccess *, AFEDebugger *);
#else
  AFEWireContainer(AFEDevice *, AFEDataAccess *);
#endif

  void initialize(void);
#ifdef DEBUG
  AFEI2CScanner *Scanner = new AFEI2CScanner(Debugger);
#else
  AFEI2CScanner *Scanner = new AFEI2CScanner();
#endif
};

#endif
#endif