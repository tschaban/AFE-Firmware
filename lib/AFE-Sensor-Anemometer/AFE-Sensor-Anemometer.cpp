/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Anemometer.h"

AFESensorAnemometer::AFESensorAnemometer(){};

boolean AFESensorAnemometer::begin(AFEDataAccess *Data,
                                   AFESensorBinary *Sensor) {
  _Data = Data;
  _Sensor = Sensor;
  startTime = millis();
  configuration = Data->getAnemometerSensorConfiguration();

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
    Serial
        << endl
        << "ERROR: Anemometer sensor NOT initialized. Wrong distance unit for "
           "impulse: "
        << configuration.impulseDistanceUnit;
#endif
    break;
  }

  if (_initialized) {
    _Sensor->begin(configuration.sensitiveness);
#ifdef DEBUG
    Serial << endl
           << "INFO: Anemometer sensor initialized and working" << endl
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

boolean AFESensorAnemometer::listener(void) {
  boolean _ret = false;
  if (_initialized) {
    if ((millis() - startTime >= configuration.interval * 1000)) {

      uint32_t noOfImpulses;
      uint32_t duration;
      _Sensor->get(noOfImpulses, duration);

      lastSpeedMS =
          ((noOfImpulses * (oneImpulseDistanceCM / 100) * 1000) / duration);
      lastSpeedKMH = lastSpeedMS * 36 / 10;

#ifdef DEBUG
      Serial << endl
             << "INFO: Anemometer speed: " << lastSpeedMS << "m/s, " << lastSpeedKMH
             << "km/h" << endl
             << " - no of impulses: " << noOfImpulses << endl
             << " - duration: " << duration << "msec. [" << duration / 1000
             << "sec]";
#endif
      startTime = millis();
      _ret = true;
    }
  }
  return _ret;
}

void AFESensorAnemometer::getJSON(char *json) {
  sprintf(json, "{\"windSpeed\":[{\"value\":%.2f,\"unit\":\"m/"
                "s\"},{\"value\":%.2f,\"unit\":\"km/h\"}]}",
          lastSpeedMS, lastSpeedKMH);
}
