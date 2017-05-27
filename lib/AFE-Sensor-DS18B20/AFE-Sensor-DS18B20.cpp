#include "AFE-Sensor-DS18B20.h"

AFESensorDS18B20::AFESensorDS18B20(byte gpio) {
   _gpio = gpio;
}

float AFESensorDS18B20::get(byte unit) {

  OneWire wireProtocol(_gpio);
  DallasTemperature sensor(&wireProtocol);
  sensor.begin();

  float temperature;
  do {
    sensor.requestTemperatures();
    temperature = unit==UNIT_CELCIUS?sensor.getTempCByIndex(0):sensor.getTempFByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));
  return temperature + _correction;
}

void AFESensorDS18B20::setCorrection(float correction) {
  _correction = correction;
}
