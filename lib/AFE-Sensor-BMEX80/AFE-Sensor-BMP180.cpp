/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMP180.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBMP180::AFESensorBMP180(){};

boolean AFESensorBMP180::begin(BMEX80 *_configuration, TwoWire *_wirePort) {

  configuration = _configuration;

#ifdef DEBUG
  Serial << endl << F("INFO: Sensor type: BMP180");
#endif
  if (configuration->i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: Address: 0x") << _HEX(configuration->i2cAddress);
#endif

    return bme.begin(configuration->i2cAddress, BMP085_ULTRAHIGHRES, _wirePort);
  } else {
#ifdef DEBUG
    Serial << endl << F("INFO: Error: Address not set");
#endif
    return false;
  }
}

boolean AFESensorBMP180::read() {
#ifdef DEBUG
  Serial << endl << F("INFO: Sensor: BMP180");
#endif
  data.temperature.value = bme.readTemperature();
  data.pressure.value = bme.readPressure() / 100.0;

  return true;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80