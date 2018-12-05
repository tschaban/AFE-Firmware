/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680() { begin(); };

void AFESensorBME680::begin() {
  AFEDataAccess Data;
  configuration = Data.getBME680SensorConfiguration();

  if (!bme.begin()) {
#ifdef DEBUG
    Serial << endl << "BME680 not found";
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
    Serial << endl << "BME680 found";
#endif
  }

  // Set up oversampling and filter initialization
}

BME680_DATA AFESensorBME680::get() {
  BME680_DATA data;
  data.temperature = data.pressure = data.humidity = data.gasResistance = 0;
  if (bme.performReading()) {
    data.temperature = bme.temperature;
    data.pressure = bme.pressure / 100.0;
    data.humidity = bme.humidity;
    data.gasResistance = bme.gas_resistance / 1000;
  }
  return data;
}

BME680_DATA AFESensorBME680::getLatestData() {
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
#if DEBUG
      Serial << endl << "Reading BME680";
#endif
      BME680_DATA data = get();

#if DEBUG
      Serial << endl
             << "T=" << data.temperature << ", H=" << data.humidity
             << ", P=" << data.pressure << ", G=" << data.gasResistance;
#endif

      if (data.temperature != 0 && data.pressure != 0 && data.humidity != 0 &&
          data.gasResistance != 0) {
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
    }
  }
}

unsigned long AFESensorBME680::getDomoticzIDX() { return 0; }
