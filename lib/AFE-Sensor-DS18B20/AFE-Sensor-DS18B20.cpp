/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-DS18B20.h"

AFESensorDS18B20::AFESensorDS18B20(){};

void AFESensorDS18B20::begin() {
  AFEDataAccess Data;
  configuration = Data.getSensorConfiguration();
  _initialized = true;
}

float AFESensorDS18B20::get() {
  float temperature = -127;
  if (_initialized) {
    OneWire wireProtocol(configuration.gpio);
    DallasTemperature sensor(&wireProtocol);
    sensor.begin();

    do {
      sensor.requestTemperatures();
      temperature = configuration.unit == UNIT_CELCIUS
                        ? sensor.getTempCByIndex(0)
                        : sensor.getTempFByIndex(0);
    } while (temperature == 85.0 || temperature == (-127.0));
  }
  return temperature + configuration.correction;
}

float AFESensorDS18B20::getLatestTemperature() {
  ready = false;
  return currentTemperature;
}

boolean AFESensorDS18B20::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorDS18B20::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {
      float newTemperature = get();
      if (!configuration.sendOnlyChanges ||
          newTemperature != currentTemperature) {
        currentTemperature = newTemperature;
        ready = true;
        startTime = 0;
      }
    }
  }
}

unsigned long AFESensorDS18B20::getDomoticzIDX() { return configuration.idx; }
