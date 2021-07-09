#ifdef AFE_CONFIG_HARDWARE_I2C

#include <Wire.h>
#ifdef DEBUG
#include <AFE-I2C-Scanner.h>
#endif
#ifdef AFE_ESP32
TwoWire WirePort0 = TwoWire(0);
TwoWire WirePort1 = TwoWire(1);
#else
TwoWire WirePort0;
#endif
void initializeI2CBUS(void);

/***** Body ******/

void initializeI2CBUS(void) {
  I2CPORT I2CBUSConfiguration;
  AFEI2CScanner I2CBus;

#ifdef AFE_ESP32
  if (Device.configuration.noOfI2Cs > 0) {
    Data.getConfiguration(0, &I2CBUSConfiguration);
    WirePort0.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                    I2CBUSConfiguration.frequency);
#else
  Data.getConfiguration(&I2CBUSConfiguration);
  WirePort0.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL);
#endif // AFE_ESP32

#ifdef DEBUG
    Serial << endl
           << "INFO: I2C(0): SDA: " << I2CBUSConfiguration.SDA
           << ", SCL: " << I2CBUSConfiguration.SCL;
#ifdef AFE_ESP32
    Serial << ", Frequency: " << I2CBUSConfiguration.frequency / 1000
           << "Hz, initialized";
#endif // AFE_ESP32

    Serial << endl << "INFO: Scannings for devices on I2C(0)";
    I2CBus.begin(&WirePort0);
    I2CBus.scanAll();

#endif // DEBUG

#ifdef AFE_ESP32
  }

  if (Device.configuration.noOfI2Cs > 1) {
    Data.getConfiguration(1, &I2CBUSConfiguration);
#ifdef DEBUG
    Serial << endl
           << "INFO: I2C(1): SDA: " << I2CBUSConfiguration.SDA
           << ", SCL: " << I2CBUSConfiguration.SCL
           << ", Frequency: " << I2CBUSConfiguration.frequency / 1000
           << "Hz, initialized";

    Serial << endl << "INFO: Scannings for devices on I2C(1)";
    I2CBus.begin(&WirePort1);
    I2CBus.scanAll();

#endif // DEBUG
    WirePort1.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                    I2CBUSConfiguration.frequency);
  }
#endif // AFE_ESP32
};

#endif // AFE_CONFIG_HARDWARE_I2C