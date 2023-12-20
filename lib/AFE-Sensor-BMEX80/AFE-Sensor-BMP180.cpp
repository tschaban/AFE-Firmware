/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#include "AFE-Sensor-BMP180.h"

#ifdef AFE_CONFIG_HARDWARE_BMEX80

#ifdef DEBUG
AFESensorBMP180::AFESensorBMP180(BMEX80 *_configuration, BMEX80_DATA *_data,
                                 AFEDebugger *_Debugger) {
  configuration = _configuration;
  data = _data;
  Debugger = _Debugger;
};
#else
AFESensorBMP180::AFESensorBMP180(BMEX80 *_configuration, BMEX80_DATA *_data) {
  configuration = _configuration;
  data = _data;
};
#endif

boolean AFESensorBMP180::begin(TwoWire *_Wire) {

#ifdef DEBUG
  Debugger->printBulletPoint(F("Type: BMP180"));
#endif

  Wire = _Wire;

  if (configuration->i2cAddress != 0) {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Address: 0x"));

    Serial << _HEX(configuration->i2cAddress);
#endif
    boolean ret =
        bme->begin(configuration->i2cAddress, BMP085_ULTRAHIGHRES, Wire);
    return ret;
  } else {
#ifdef DEBUG
    Debugger->printBulletPoint(F("Error: Address not set"));
#endif
    return false;
  }
}

boolean AFESensorBMP180::read() {
#ifdef DEBUG
  Debugger->printBulletPoint(F("Sensor: BMP180"));
#endif
  data->temperature.value = bme->readTemperature();
  data->pressure.value = bme->readPressure() / 100.0;

  return true;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80
