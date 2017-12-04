/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Sensor-DS18B20.h"

AFESensorDS18B20::AFESensorDS18B20(){};

AFESensorDS18B20::AFESensorDS18B20(uint8_t sensor_gpio) { begin(sensor_gpio); }

void AFESensorDS18B20::begin(uint8_t sensor_gpio) { gpio = sensor_gpio; }

float AFESensorDS18B20::getPrevious() {
  return previousTemperature;
}

float AFESensorDS18B20::get() {

  OneWire wireProtocol(gpio);
  DallasTemperature sensor(&wireProtocol);
  sensor.begin();

  float temperature;
  do {
    sensor.requestTemperatures();
    temperature = unit == UNIT_CELCIUS ? sensor.getTempCByIndex(0)
                                       : sensor.getTempFByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));

  startTime=0; // Reset timer

  return temperature + correction;
}

void AFESensorDS18B20::setCorrection(float sensor_correction) {
  correction = sensor_correction;
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
  unsigned long time = millis();

  if (startTime == 0) { // starting timer. used for switch sensitiveness
    startTime = time;
  }

  if (time - startTime >= 5000) {
    ready = true;
  }
}
