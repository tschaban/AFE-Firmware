/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME280.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#ifdef DEBUG
AFESensorBME280::AFESensorBME280(BMEX80 *_configuration, BMEX80_DATA *_data,
                                 AFEDebugger *_Debugger) {
  Debugger = _Debugger;
  configuration = _configuration;
  data = _data;
};
#else
AFESensorBME280::AFESensorBME280(BMEX80 *_configuration, BMEX80_DATA *_data) {
  configuration = _configuration;
  data = _data;
};
#endif

boolean AFESensorBME280::begin(TwoWire *_wirePort) {
  Wire = _wirePort;
  if (configuration->i2cAddress != 0) {

#ifdef DEBUG
    Debugger->printBulletPoint(F(" : Address: 0x"));
    Serial << _HEX(configuration->i2cAddress);
#endif
    if (!bme->begin(configuration->i2cAddress, Wire)) {
      return false;
    } else {
      bme->setSampling(
          Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::FILTER_OFF);
#ifdef DEBUG
      if ((bme->sensorID() == BME280_SENSOR &&
           configuration->type != AFE_BME280_SENSOR) ||
          (bme->sensorID() == BMP280_SENSOR &&
           configuration->type != AFE_BMP280_SENSOR)) {
        Debugger->printBulletPoint(
            F("ERROR: Wrong sensor type in configuration"));
      }
      Debugger->printBulletPoint(F("Type: "));
      Debugger->printValue(bme->sensorID() == BME280_SENSOR ? F("BME280")
                                                            : F("BMP280"));
#endif

      return true;
    }
  } else {
#ifdef DEBUG
    Debugger->printBulletPoint(F("ERROR: Address not set"));
#endif
    return false;
  }
}

boolean AFESensorBME280::read() {

#ifdef DEBUG
  Debugger->printBulletPoint(F("Sensor: "));
  Debugger->printValue(bme->sensorID() == BME280_SENSOR ? F("BME280")
                                                        : F("BMP280"));
#endif

  bme->takeForcedMeasurement();

  data->temperature.value = bme->readTemperature();
  data->pressure.value = bme->readPressure() / 100.0;
  if (configuration->type == AFE_BME280_SENSOR) {
    data->humidity.value = bme->readHumidity();
  }

  return true;
}

#endif // AFE_CONFIG_HARDWARE_BMEX80