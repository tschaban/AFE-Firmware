/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Wind.h"

AFESensorWind::AFESensorWind(){};

boolean AFESensorWind::begin(AFEDataAccess *Data, AFESensorBinary *Sensor) {
  _Data = Data;
  _Sensor = Sensor;
  startTime = millis();
  configuration = Data->getWindSensorConfiguration();

  _initialized = true;

  switch (configuration.impulseDistanceUnit) {
  case AFE_DISTANCE_CENTIMETER:
    oneImpulseDistanceCM = configuration.impulseDistance;
    break;
  case AFE_DISTANCE_METER:
    oneImpulseDistanceCM = configuration.impulseDistance * 100;
    break;
  case AFE_DISTANCE_KILOMETER:
    oneImpulseDistanceCM = configuration.impulseDistance * 1000;
    break;
  default:
    _initialized = true;
#ifdef DEBUG
    Serial << endl
           << "ERROR: Wind sensor NOT initialized. Wrong distance unit for "
              "impulse: "
           << configuration.impulseDistanceUnit;
#endif
    break;
  }

  if (_initialized) {
    _Sensor->begin(configuration.sensitiveness);
#ifdef DEBUG
    Serial << endl
           << "INFO: Wind sensor initialized and working" << endl
           << " - GPIO: " << configuration.gpio << endl
           << " - Interval: " << configuration.interval << endl
           << " - 1 impulse distance: " << configuration.impulseDistance << endl
           << " - 1 impulse distance unit: "
           << configuration.impulseDistanceUnit << endl
           << " - Boucing: " << configuration.sensitiveness;
#endif
  }

  return _initialized;
}

boolean AFESensorWind::listener(void) {
  boolean _ret = false;
  if (_initialized) {
    if ((millis() - startTime >= configuration.interval * 1000)) {
      _ret = true;
    }
  }
  return _ret;
}

void AFESensorWind::get(float &speedMS, float &speedKMH) {
  uint32_t noOfImpulses;
  uint32_t duration;
  _Sensor->get(noOfImpulses, duration);

  speedMS = ((noOfImpulses * (oneImpulseDistanceCM/100) * 60000) / duration);

  speedKMH = speedMS * 36 / 10;

#ifdef DEBUG
  Serial << endl
         << "INFO: Wind speed: " << speedMS << "M/s, " << speedKMH << "km/h"
         << endl
         << " - impulse: " << noOfImpulses << endl
         << " - duration: " << duration << "msec. [" << duration / 1000
         << "sek]";
#endif
  startTime = millis();
}