#include "AFE-Wire-Container.h"

#ifdef AFE_CONFIG_HARDWARE_I2C

#ifdef DEBUG
AFEWireContainer::AFEWireContainer(AFEDevice *_Device, AFEDataAccess *_Data,
                                   AFEDebugger *_Debugger) {
  Device = _Device;
  Data = _Data;
  Debugger = _Debugger;
}
#else
AFEWireContainer::AFEWireContainer(AFEDevice *_Device, AFEDataAccess *_Data) {
  Device = _Device;
  Data = _Data;
}
#endif

#ifdef DEBUG
void AFEWireContainer::printConfiguration(I2CPORT *Configuration) {
  Debugger->printInformation(F("Initialization"), F("I2C[0]"));
  Debugger->printBulletPoint(F("SDA: "));
  Debugger->printValue(Configuration->SDA);
  Debugger->printBulletPoint(F("SCL: "));
  Debugger->printValue(Configuration->SCL);
#ifdef AFE_ESP32
  Debugger->printBulletPoint(F("Frequency: "));
  Serial >> (Configuration->frequency / 1000) << F("Hz");
#endif
}
#endif

void AFEWireContainer::initialize(void) {
  I2CPORT I2CBUSConfiguration;
  boolean success = false;

#ifdef AFE_ESP32
  if (Device->configuration.noOfI2Cs > 0) {
    Data->getConfiguration(0, &I2CBUSConfiguration);

#ifdef DEBUG
    printConfiguration(&I2CBUSConfiguration);
#endif
    
    success = Port0->begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                           I2CBUSConfiguration.frequency);
#else /* ESP8266 */
  Data->getConfiguration(&I2CBUSConfiguration);

#ifdef DEBUG
  printConfiguration(&I2CBUSConfiguration);
#endif

  Port0->begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL);
  success = true;
#endif // AFE_ESP32

#ifdef DEBUG
    // @TODO there was delay(100) here, testing without it
    if (!success) {
      Debugger->printError(F("Doesn't work"), F("I2C[0]"));
    } else {
      Scanner->scanAll(Port0);
    }
#endif // DEBUG

#ifdef AFE_ESP32
  }

  if (Device->configuration.noOfI2Cs > 1) {
    Data->getConfiguration(1, &I2CBUSConfiguration);

#ifdef DEBUG
    printConfiguration(&I2CBUSConfiguration);
#endif

    success = Port1->begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                           I2CBUSConfiguration.frequency);

#ifdef DEBUG
    if (!success) {
      Debugger->printError(F("Bus doesn't work"), F("I2C[1]"));
    } else {
      Scanner->scanAll(Port1);
    }
#endif // DEBUG
  }
#endif // AFE_ESP32
}

#endif // AFE_CONFIG_HARDWARE_I2C