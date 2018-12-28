/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BMP180.h"

AFESensorBMP180::AFESensorBMP180(){};

boolean AFESensorBMP180::begin() {
  AFEDataAccess Data;
  configuration = Data.getBMx80SensorConfiguration();

#if defined(DEBUG)
  Serial << endl << "Sensor type: BMP180";
#endif

  return bme.begin();
}

boolean AFESensorBMP180::read() {
  Serial << endl << "Sensor: BMP180";

  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100.0;

  return true;
}
