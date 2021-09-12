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
  boolean success = false;

#ifdef AFE_ESP32
  if (Device.configuration.noOfI2Cs > 0) {
    Data.getConfiguration(0, &I2CBUSConfiguration);

#ifdef DEBUG
    Serial << endl
           << "INFO: I2C[0]: SDA: " << I2CBUSConfiguration.SDA
           << ", SCL: " << I2CBUSConfiguration.SCL
           << ", Frequency: " << (I2CBUSConfiguration.frequency / 1000) << "Hz";
#endif

    success = WirePort0.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                              I2CBUSConfiguration.frequency);
#else
  Data.getConfiguration(&I2CBUSConfiguration);

#ifdef DEBUG
  Serial << endl
         << "INFO: I2C: SDA: " << I2CBUSConfiguration.SDA
         << ", SCL: " << I2CBUSConfiguration.SCL;
#endif

  WirePort0.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL);
  success = true;
#endif // AFE_ESP32

#ifdef DEBUG
     yield();
    if (!success) {
      Serial << endl << "ERROR: I2C[0]: Bus doesn't work";
    } else {
      Serial << endl << "INFO: I2C[0]: Scannings for devices";
      I2CBus.begin(&WirePort0);
      I2CBus.scanAll();
    }
#endif // DEBUG

#ifdef AFE_ESP32
  }

  if (Device.configuration.noOfI2Cs > 1) {
    Data.getConfiguration(1, &I2CBUSConfiguration);

#ifdef DEBUG
    Serial << endl
           << "INFO: I2C[1]: SDA: " << I2CBUSConfiguration.SDA
           << ", SCL: " << I2CBUSConfiguration.SCL
           << ", Frequency: " << I2CBUSConfiguration.frequency / 1000 << "Hz";
#endif // DEBUG

    success = WirePort1.begin(I2CBUSConfiguration.SDA, I2CBUSConfiguration.SCL,
                              I2CBUSConfiguration.frequency);

#ifdef DEBUG
    if (!success) {
      Serial << endl << "ERROR: I2C[1]: Bus doesn't work";
    } else {
      Serial << endl << "INFO: I2C[1]: Scannings for devices";
      I2CBus.begin(&WirePort1);
      I2CBus.scanAll();
    }
#endif // DEBUG
  }
#endif // AFE_ESP32
};

#endif // AFE_CONFIG_HARDWARE_I2C