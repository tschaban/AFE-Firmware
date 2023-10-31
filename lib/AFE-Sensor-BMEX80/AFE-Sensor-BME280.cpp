/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BME280.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBME280::AFESensorBME280(){};

#ifdef DEBUG
boolean AFESensorBME280::begin(BMEX80 *_configuration, TwoWire *_wirePort, AFEDebugger *_Debugger, BMEX80_DATA *_data)
{
  Debugger = _Debugger;
  return begin(_configuration, _wirePort, _data);
}
#endif

boolean AFESensorBME280::begin(BMEX80 *_configuration, TwoWire *_wirePort, BMEX80_DATA *_data)
{
  configuration = _configuration;
  data = _data;

  if (configuration->i2cAddress != 0)
  {

#ifdef DEBUG
    Debugger->printBulletPoint(F(" : Address: 0x"));
    Serial << _HEX(configuration->i2cAddress);
#endif
    if (!bme->begin(configuration->i2cAddress, _wirePort))
    {
      return false;
    }
    else
    {
      bme->setSampling(
          Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1,
          Adafruit_BME280::FILTER_OFF);
#ifdef DEBUG
      if ((bme->sensorID() == BME280_SENSOR &&
           _configuration->type != AFE_BME280_SENSOR) ||
          (bme->sensorID() == BMP280_SENSOR &&
           _configuration->type != AFE_BMP280_SENSOR))
      {
        Debugger->printBulletPoint(F("ERROR: Wrong sensor type in configuration"));
      }
      Debugger->printBulletPoint(F("Type: "));
      Debugger->printValue(bme->sensorID() == BME280_SENSOR ? F("BME280") : F("BMP280"));
#endif

      return true;
    }
  }
  else
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("ERROR: Address not set"));
#endif
    return false;
  }
}

boolean AFESensorBME280::read()
{

#ifdef DEBUG
  Debugger->printBulletPoint(F("Sensor: "));
  Debugger->printValue(bme->sensorID() == BME280_SENSOR ? F("BME280") : F("BMP280"));
#endif

  bme->takeForcedMeasurement();

  data->temperature.value = bme->readTemperature();
  data->pressure.value = bme->readPressure() / 100.0;
  if (configuration->type == AFE_BME280_SENSOR)
  {
    data->humidity.value = bme->readHumidity();
  }

  return true;
}

#endif // AFE_CONFIG_HARDWARE_BMEX80