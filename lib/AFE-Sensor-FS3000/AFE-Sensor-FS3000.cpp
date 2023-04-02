/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-FS3000.h"

#ifdef AFE_CONFIG_HARDWARE_FS3000

AFESensorFS3000::AFESensorFS3000(){};

boolean AFESensorFS3000::begin(uint8_t id, AFEDataAccess *Data) {
  Data->getConfiguration(id, configuration);

  configuration->r = 50; // @TODO 3.6.0

  if (_fs3000->begin()) {
    _fs3000->setRange(configuration->range);
    _initialized = true;
    _startTime = millis();
#ifdef DEBUG
    Serial << endl << F("INFO: FS3000: Intialized");
  } else {
    Serial << endl << F("WARN: FS3000: Not intialized");
#endif
  }

  return _initialized;
}

// wzor PI * R(m)2 * 3600 * Predkosc meter per second

boolean AFESensorFS3000::listener(void) {
  boolean _ret = false;
  if (_initialized) {
    if (millis() - _startTime >= configuration->interval * 1000) {

#ifdef DEBUG
      Serial << endl << endl << F("----- FS3000: Reading -----");
      Serial << endl
             << F(" : Time: ") << (millis() - _startTime) / 1000 << F("s");
#endif

      data->raw = _fs3000->readRaw();
      data->metersPerSecond = _fs3000->readMetersPerSecond();
      data->milesPerHour = _fs3000->readMilesPerHour();

      data->meters3PerSecond = configuration->r > 0
                                   ? ((float)AFE_CONFIG_PI_NUMBER *
                                      sq((float)configuration->r / (float)100) *
                                      (float)3600 * data->metersPerSecond)
                                   : 0;
#ifdef DEBUG
      Serial << endl << F(" : ") << data->raw << F(" raw");
      Serial << endl << F(" : ") << data->metersPerSecond << F(" m/s");
      Serial << endl << F(" : ") << data->meters3PerSecond << F(" m3/h");
      Serial << endl << F(" : ") << data->milesPerHour << F(" mil/h");
#endif

      _ret = true;
      _startTime = millis();
    }
  }

  return _ret;
}

void AFESensorFS3000::getJSON(char *json) {
  sprintf(json, "{\"fs3000\":[{\"value\":%d,\"unit\":\"raw\"},{\"value\":%."
                "3f,\"unit\":\"%s\"},{\"value\":%.3f,\"unit\":\"%s\"},{"
                "\"value\":%.3f,\"unit\":\"%s\"}]}",
          data->raw, data->metersPerSecond, F(AFE_UNIT_MS), data->milesPerHour,
          F(AFE_UNIT_MILH), data->meters3PerSecond, F(AFE_UNIT_M3H));
}

#endif // AFE_CONFIG_HARDWARE_FS3000