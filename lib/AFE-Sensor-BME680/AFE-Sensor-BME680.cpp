/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680(){};

void AFESensorBME680::begin() {
  AFEDataAccess Data;
  configuration = Data.getBME680SensorConfiguration();

#if defined(DEBUG)
  Serial << endl << endl << "-------- BME680: Initializing --------";
#endif

  if (!bme.begin()) {
#ifdef DEBUG
    Serial << endl << "BME680: Not found";
#endif
    while (1)
      ;
  } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    _initialized = true;
#ifdef DEBUG
    Serial << endl << "BME680: Found";
#endif
  }

#if defined(DEBUG)
  Serial << endl << "--------------------------------------" << endl;
#endif
}

BME680_DATA AFESensorBME680::get() {
  ready = false;
  return sensorData;
}

boolean AFESensorBME680::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBME680::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {
#if defined(DEBUG)
      Serial << endl << endl << "-------- BME680: Reading --------";
#endif

      BME680_DATA data;
      data.temperature = data.pressure = data.humidity = data.gasResistance =
          -1;
      if (bme.performReading()) {
        data.temperature = bme.temperature;
        data.pressure = bme.pressure / 100.0;
        data.humidity = bme.humidity;
        data.gasResistance = bme.gas_resistance / 1000;
#if defined(DEBUG)
        Serial << endl
               << "Temperature = " << data.temperature << endl
               << "Humidity = " << data.humidity << endl
               << "Pressure = " << data.pressure << endl
               << "Gas Sesnor = " << data.gasResistance;
      } else {
        Serial << "No data found";
#endif
      }

      if (data.temperature != -1 && data.pressure != -1 &&
          data.humidity != -1 && data.gasResistance != -1) {
        if (!configuration.sendOnlyChanges ||
            data.temperature != sensorData.temperature ||
            data.pressure != sensorData.pressure ||
            data.humidity != sensorData.humidity ||
            data.gasResistance != sensorData.gasResistance) {

          sensorData.temperature = data.temperature;
          sensorData.pressure = data.pressure;
          sensorData.humidity = data.humidity;
          sensorData.gasResistance = data.gasResistance;
          ready = true;
        }
      }
      startTime = 0;
#if defined(DEBUG)
      Serial << endl << "---------------------------------" << endl;
#endif
    }
  }
}

void AFESensorBME680::getDomoticzIDX(BME680_DOMOTICZ *idx) {
  *idx = configuration.idx;
}
