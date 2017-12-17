/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */


#include "AFE-Sensor-DHT.h"

AFESensorDHT::AFESensorDHT(){};

void AFESensorDHT::begin() {
  AFEDataAccess Data;
  configuration = Data.getDHTConfiguration();
  _initialized = true;
}

float AFESensorDHT::get() {
  DHT dht(configuration.gpio, configuration.type == 1
                   ? DHT11
                   : configuration.type == 2 ? DHT21 : DHT22);
  dht.begin();
  float temperature = dht.readTemperature();

  return temperature + configuration.temperature.correction;
}

float  AFESensorDHT::getLatest() {
  ready = false;
  return currentTemperature;
}

boolean AFESensorDHT::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorDHT::listener() {
  if (_initialized) {
  unsigned long time = millis();

  if (startTime == 0) { // starting timer. used for switch sensitiveness
    startTime = time;
  }

  if (time - startTime >= configuration.temperature.interval * 1000) {
    float newTemperature = get();
    if (newTemperature!=currentTemperature) {
      currentTemperature = newTemperature;
      ready = true;
      startTime = 0;
    }
  }
}
}
