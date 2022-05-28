/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME280.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBME280::AFESensorBME280(){};

boolean AFESensorBME280::begin(BMEX80 *_configuration, TwoWire *_wirePort) {
  configuration = _configuration;


  if (configuration->i2cAddress != 0) {

#ifdef DEBUG
    Serial << endl << F(" : Address: 0x") << _HEX(configuration->i2cAddress);
#endif
    if (!bme.begin(configuration->i2cAddress, _wirePort)) {
      return false;
    } else {
      bme.setSampling(
          Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::FILTER_OFF);
#ifdef DEBUG
      if ((bme.sensorID() == BME280_SENSOR &&
           _configuration->type != AFE_BME280_SENSOR) ||
          (bme.sensorID() == BMP280_SENSOR &&
           _configuration->type != AFE_BMP280_SENSOR)) {
        Serial << endl << F(" : ERROR: Wrong sensor type in configuration");
      }
      Serial << endl
             << F(" : Type: ")
             << (bme.sensorID() == BME280_SENSOR ? F("BME280") : F("BMP280"));
#endif

      return true;
    }
  } else {
#ifdef DEBUG
    Serial << endl << F(" : ERROR: Address not set");
#endif
    return false;
  }
}

boolean AFESensorBME280::read() {

#ifdef DEBUG
  Serial << endl
         << F("INFO :Sensor: ")
         << (bme.sensorID() == AFE_BME280_SENSOR ? F("BME280") : F("BMP280"));
#endif

  bme.takeForcedMeasurement();

  data.temperature.value = bme.readTemperature();
  data.pressure.value = bme.readPressure() / 100.0;
  if (configuration->type == AFE_BME280_SENSOR) {
    data.humidity.value = bme.readHumidity();
  }

  return true;
}

#endif // AFE_CONFIG_HARDWARE_BMEX80