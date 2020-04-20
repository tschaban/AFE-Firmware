/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME280.h"

AFESensorBME280::AFESensorBME280(){};

boolean AFESensorBME280::begin(BMEX80 *_configuration, I2CPORT *I2C) {
  configuration = _configuration;

#if defined(DEBUG)
  Serial << endl << "INFO: Sensor type: BME280";
#endif

  if (configuration->i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "INFO: Setting I2C: SDA:" << I2C->SDA << ", SCL:" << I2C->SCL;
#endif
    Wire.begin(I2C->SDA,I2C->SCL);

#ifdef DEBUG
    Serial << endl << "INFO: Address: 0x" << _HEX(configuration->i2cAddress);
#endif
    if (!bme.begin(configuration->i2cAddress,&Wire)) {
      return false;
    } else {
      bme.setSampling(
          Adafruit_BME280::BME280_MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::FILTER_OFF);

      return true;
    }
  } else {
#ifdef DEBUG
    Serial << endl << "ERROR: Address not set";
#endif
    return false;
  }
}

boolean AFESensorBME280::read() {

#ifdef DEBUG
  Serial << endl << "INFO :Sensor: BME280";
#endif

  bme.takeForcedMeasurement();

  data.temperature.value = bme.readTemperature();
  data.pressure.value = bme.readPressure() / 100.0;
  data.humidity.value = bme.readHumidity();

  return true;
}
