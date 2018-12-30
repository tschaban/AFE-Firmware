/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME280.h"

AFESensorBME280::AFESensorBME280(){};

boolean AFESensorBME280::begin() {
  AFEDataAccess Data;
  configuration = Data.getBMx80SensorConfiguration();

#if defined(DEBUG)
  Serial << endl << "Sensor type: BME280";
#endif

  if (configuration.i2cAddress != 0) {
#if defined(DEBUG)
    Serial << endl << "Address: 0x" << _HEX(configuration.i2cAddress);
#endif
    if (!bme.begin(configuration.i2cAddress)) {
      return false;
    } else {
      bme.setSampling(
          Adafruit_BME280::BME280_MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::FILTER_OFF);

      return true;
    }
  } else {
#if defined(DEBUG)
    Serial << endl << "Error: Address not set";
#endif
    return false;
  }
}

boolean AFESensorBME280::read() {
  Serial << endl << "Sensor: BME280";

  bme.takeForcedMeasurement();

  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100.0;
  data.humidity = bme.readHumidity();

  return true;
}
