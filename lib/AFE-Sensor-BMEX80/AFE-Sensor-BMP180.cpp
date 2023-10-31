/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#include "AFE-Sensor-BMP180.h"

#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBMP180::AFESensorBMP180(){};

#ifdef DEBUG
boolean AFESensorBMP180::begin(BMEX80 *_configuration, TwoWire *_wirePort, AFEDebugger *_Debugger, BMEX80_DATA *_data)
{
  Debugger = _Debugger;
  return begin(_configuration, _wirePort, _data);
}
#endif

boolean AFESensorBMP180::begin(BMEX80 *_configuration, TwoWire *_wirePort, BMEX80_DATA *_data)
{

  configuration = _configuration;
  data = _data;

#ifdef DEBUG
  Debugger->printBulletPoint(F("Type: BMP180"));
#endif
  if (configuration->i2cAddress != 0)
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Address: 0x"));
    Serial << _HEX(configuration->i2cAddress);
#endif
    boolean ret =
        bme->begin(configuration->i2cAddress, BMP085_ULTRAHIGHRES, _wirePort);
    return ret;
  }
  else
  {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Error: Address not set"));
#endif
    return false;
  }
}

boolean AFESensorBMP180::read()
{
#ifdef DEBUG
    Debugger->printBulletPoint(F("Sensor: BMP180"));
#endif
  data->temperature.value = bme->readTemperature();
  data->pressure.value = bme->readPressure() / 100.0;

  return true;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80
