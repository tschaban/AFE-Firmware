#include "AFE-Sensor-DS18B20.h"

AFESensorDS18B20::AFESensorDS18B20(){};

AFESensorDS18B20::AFESensorDS18B20(uint8_t sensor_gpio) { begin(sensor_gpio); }

void AFESensorDS18B20::begin(uint8_t sensor_gpio) { gpio = sensor_gpio; }

float AFESensorDS18B20::get(uint8_t unit) {

  OneWire wireProtocol(gpio);
  DallasTemperature sensor(&wireProtocol);
  sensor.begin();

  float temperature;
  do {
    sensor.requestTemperatures();
    temperature = unit == UNIT_CELCIUS ? sensor.getTempCByIndex(0)
                                       : sensor.getTempFByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));
  return temperature + correction;
}

void AFESensorDS18B20::setCorrection(float sensor_correction) {
  correction = sensor_correction;
}
