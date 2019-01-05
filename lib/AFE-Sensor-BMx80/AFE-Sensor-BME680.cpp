/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680(){};

boolean AFESensorBME680::begin() {
  AFEDataAccess Data;
  configuration = Data.getBMx80SensorConfiguration();

#ifdef DEBUG
  Serial << endl << "Sensor type: BME680";
#endif

  if (configuration.i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "Address: 0x" << _HEX(configuration.i2cAddress);
#endif
    if (!bme.begin(configuration.i2cAddress)) {
      return false;
    } else {
      bme.setTemperatureOversampling(BME680_OS_8X);
      bme.setHumidityOversampling(BME680_OS_2X);
      bme.setPressureOversampling(BME680_OS_4X);
      bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
      bme.setGasHeater(320, 150); // 320*C for 150 ms
      return true;
    }
  } else {
#ifdef DEBUG
    Serial << endl << "Error: Address not set";
#endif
    return false;
  }
}

boolean AFESensorBME680::read() {
#ifdef DEBUG
  Serial << endl << "Sensor: BME680";
#endif
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
