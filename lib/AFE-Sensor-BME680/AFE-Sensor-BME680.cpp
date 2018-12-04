/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BME680.h"

AFESensorBME680::AFESensorBME680(){};

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

float AFESensorBME680::get() {
  if (_initialized) {
  }
  return 0;
}

float AFESensorBME680::getLatestData() {
  ready = false;
  return currentTemperature;
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
      float data = get();
      if (!configuration.sendOnlyChanges) {
        ready = true;
        startTime = 0;
      }
    }
  }
}

unsigned long AFESensorBME680::getDomoticzIDX() { return 0; }
