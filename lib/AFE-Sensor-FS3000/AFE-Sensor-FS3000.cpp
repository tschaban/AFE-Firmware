/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-FS3000.h"

#ifdef AFE_CONFIG_HARDWARE_FS3000
#ifdef DEBUG
AFESensorFS3000::AFESensorFS3000(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort,
                                 AFEDebugger *_Debugger) {
  Data = _Data;
  WirePort = _WirePort;
  Debugger = _Debugger;
};
#else
AFESensorFS3000::AFESensorFS3000(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort) {
  Data = _Data;
  WirePort = _WirePort;
};
#endif

boolean AFESensorFS3000::begin(uint8_t id) {
  Data->getConfiguration(id, configuration);
  delete Data;
  Data = NULL;

#ifdef AFE_ESP32
  bool _success = _fs3000->begin(configuration->wirePortId =
                                     0 ? *WirePort->Port0 : *WirePort->Port1);
#else
  bool _success = _fs3000->begin(*WirePort->Port0);
#endif

  if (_success) {
    _fs3000->setRange(configuration->range);
    _initialized = true;
    _startTime = millis();
#ifdef DEBUG
    Debugger->printInformation(F("Intialized"), F("FS3000"));
  } else {
    Debugger->printError(F("Not Intialized"), F("FS3000"));
#endif
  }

  return _initialized;
}

// wzor PI * R(mm/1000)2 * 3600 * Predkosc meter per second

boolean AFESensorFS3000::listener(void) {
  boolean _ret = false;
  if (_initialized) {
    if (millis() - _startTime >= configuration->interval * 1000) {

#ifdef DEBUG
      Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("FS3000: Reading"));
      Debugger->printBulletPoint(F("Interval: "));
      Serial << (millis() - _startTime) / 1000 << F("s");
#endif

      data->raw = _fs3000->readRaw();
      data->metersPerSecond = _fs3000->readMetersPerSecond();
      data->milesPerHour = _fs3000->readMilesPerHour();

      data->meters3PerHour = configuration->r > 0
                                 ? ((float)AFE_CONFIG_PI_NUMBER *
                                    sq((float)configuration->r / (float)1000) *
                                    (float)3600 * data->metersPerSecond)
                                 : 0;
#ifdef DEBUG
      Debugger->printBulletPoint(F("Raw data: "));
      Serial << data->raw;

      Debugger->printBulletPoint(F("M/S: "));
      Debugger->printValue(data->metersPerSecond);

      Debugger->printBulletPoint(F("M3/H: "));
      Debugger->printValue(data->meters3PerHour);

      Debugger->printBulletPoint(F("Mil/H: "));
      Debugger->printValue(data->milesPerHour);

      Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);

#endif

      _ret = true;
      _startTime = millis();
    }
  }

  return _ret;
}

void AFESensorFS3000::getJSON(char *json) {
  sprintf(json, (const char *)F(
                    "{\"fs3000\":[{\"value\":%d,\"unit\":\"raw\"},{\"value\":%."
                    "3f,\"unit\":\"%s\"},{\"value\":%.3f,\"unit\":\"%s\"},{"
                    "\"value\":%.3f,\"unit\":\"%s\"}]}"),
          data->raw, data->metersPerSecond, F(AFE_UNIT_MS), data->milesPerHour,
          F(AFE_UNIT_MILH), data->meters3PerHour, F(AFE_UNIT_M3H));
}

#endif // AFE_CONFIG_HARDWARE_FS3000_Debuuger