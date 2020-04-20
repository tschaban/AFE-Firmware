/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMP180.h"

AFESensorBMP180::AFESensorBMP180(){};

boolean AFESensorBMP180::begin(BMEX80 *_configuration, I2CPORT *I2C) {

  configuration = _configuration;

#ifdef DEBUG
  Serial << endl << "INFO: Sensor type: BMP180";
#endif
  if (configuration->i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "INFO: Address: 0x" << _HEX(configuration->i2cAddress);
#endif
    return bme.begin(I2C->SDA,I2C->SCL, BMP085_ULTRAHIGHRES, configuration->i2cAddress);
  } else {
#ifdef DEBUG
    Serial << endl << "INFO: Error: Address not set";
#endif
    return false;
  }
}

boolean AFESensorBMP180::read() {
#ifdef DEBUG
  Serial << endl << "INFO: Sensor: BMP180";
#endif
  data.temperature.value = bme.readTemperature();
  data.pressure.value = bme.readPressure() / 100.0;

  return true;
}
