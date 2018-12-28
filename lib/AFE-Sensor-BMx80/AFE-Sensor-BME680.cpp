/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680(){};

boolean AFESensorBME680::begin() {
  AFEDataAccess Data;
  configuration = Data.getBMx80SensorConfiguration();

#if defined(DEBUG)
  Serial << endl << "Sensor type: BME680";
#endif

  if (!bme.begin()) {
    return false;
  } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    return true;
  }
}

boolean AFESensorBME680::read() {
  Serial << endl << "Sensor: BME680";

  if (bme.performReading()) {
    data.temperature = bme.temperature;
    data.pressure = bme.pressure / 100.0;
    data.humidity = bme.humidity;
    data.gasResistance = bme.gas_resistance / 1000;
    return true;
  } else {
    return false;
  }
}
