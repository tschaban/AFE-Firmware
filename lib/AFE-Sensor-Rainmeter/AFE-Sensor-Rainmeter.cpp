/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Rainmeter.h"

#ifdef AFE_CONFIG_HARDWARE_RAINMETER

AFERainmeter::AFERainmeter(){};

#ifdef DEBUG
boolean AFERainmeter::begin(AFEDataAccess *Data, AFEImpulseCatcher *Sensor, AFEDebugger *_Debugger)
{
  Debugger = _Debugger;
  return begin(Data, Sensor);
}
#endif

boolean AFERainmeter::begin(AFEDataAccess *Data, AFEImpulseCatcher *Sensor)
{
  _Data = Data;
  _Sensor = Sensor;
  Data->getConfiguration(configuration);

  _initialized = true;

  _Sensor->begin(configuration->sensitiveness);
#ifdef DEBUG
  Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printBulletPoint(F("Initializing Rain Sensor"));
  Debugger->printBulletPoint(F("GPIO: "));
  Debugger->printValue(configuration->gpio);
  Debugger->printBulletPoint(F("Interval: "));
  Serial << configuration->interval;
  Debugger->printBulletPoint(F("Boucing: "));
  Debugger->printValue(configuration->sensitiveness);
  Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

  RAINMETER_DATA *_previous = new RAINMETER_DATA;
  _Data->get(_previous);

  memcpy(current->last1h, _previous->last1h, sizeof(_previous->last1h[0]) * 60);
  current->index1h = _previous->index1h;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  current->counter = _previous->counter;
#else
  for (uint8_t i = 0; i < 12; i++)
  {
    current->last12h[i] = _previous->last12h[i];
  }
  current->last24h[0] = _previous->last24h[0];
  current->last24h[1] = _previous->last24h[1];

  current->index12h = _previous->index12h;
  current->index24h = _previous->index24h;
#endif
  startTime = millis();
  start60Sec = millis();

  return _initialized;
}

boolean AFERainmeter::listener(void)
{
  boolean _ret = false;
  if (_initialized)
  {

    // Calculate every 60sec
    if ((millis() - start60Sec >= 60000))
    {

      uint32_t noOfImpulses;
      uint32_t duration;
      _Sensor->get(noOfImpulses, duration);

      /* Level during last 1 minute */
      rainLevelLast1Minute = noOfImpulses * configuration->resolution / 1000;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      current->counter += rainLevelLast1Minute;
#endif
      /* Storing water level for passed 1 minute */
      current->last1h[current->index1h] = rainLevelLast1Minute;

      /* Recalculating the water level during the last hour */
      rainLevelLastHour = 0;
      for (uint8_t i = 0; i <= current->index1h; i++)
      {
        rainLevelLastHour = rainLevelLastHour + current->last1h[i];
      }

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ // Not required for Domoticz
      /* Storing water level for passed hour */

      current->last12h[current->index12h] = rainLevelLastHour;
      /* Recalculating the water level during the last 12 hours */
      rainLevelLast12Hours = 0;

      for (uint8_t i = 0; i <= current->index12h; i++)
      {
        rainLevelLast12Hours = rainLevelLast12Hours + current->last12h[i];
      }

      current->last24h[current->index24h] = rainLevelLast12Hours;

      rainLevelLast24Hours = current->last24h[0] + current->last24h[1];

      if (current->index1h >= 59)
      {

        if (current->index12h >= 11)
        {
          current->index24h = current->index24h == 0 ? 1 : 0;
        }

        /* Changing index for 12 hours array */
        current->index12h = current->index12h >= 11 ? 0 : current->index12h + 1;
      }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

      /* Changing index for 60 minutes array */
      current->index1h = current->index1h >= 59 ? 0 : current->index1h + 1;

#ifdef DEBUG

      Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("Reading rain sensor data"));
      Debugger->printBulletPoint(F("Last 1 min: "));
      Debugger->printValue(rainLevelLast1Minute, F("mm/min"));
      Debugger->printBulletPoint(F("Last hour: "));
      Debugger->printValue(rainLevelLastHour, F("mm/hour"));

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
      Debugger->printBulletPoint(F("Last 12 hrs: "));
      Debugger->printValue(rainLevelLast12Hours, F("mm/12hrs"));
      Debugger->printBulletPoint(F("Last 24 hrs: "));
      Debugger->printValue(rainLevelLast24Hours, F("mm/24hrs"));
#endif
      Debugger->printBulletPoint(F("Last 12 hrs: "));
      Debugger->printValue(rainLevelLast12Hours, F("mm/12hrs"));
      Debugger->printBulletPoint(F("Last 24 hrs: "));
      Debugger->printValue(rainLevelLast24Hours, F("mm/24hrs"));
      Debugger->printBulletPoint(F("Index: Hour: "));
      Debugger->printValue(current->index1h);
#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
      Debugger->printBulletPoint(F("Index: 12 hrs: "));
      Debugger->printValue(current->index12h);
      Debugger->printBulletPoint(F("Index: 24 hrs: "));
      Debugger->printValue(current->index24h);
#endif
      Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

      start60Sec = millis();
    }
  }

  /* Counter for calculating the data */
  if ((millis() - startTime >= configuration->interval * 1000))
  {
    startTime = millis();
    _Data->save(current);
    _ret = true;
  }

  return _ret;
}

void AFERainmeter::getJSON(char *json)
{
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  sprintf(json, (const char *)F("{\"rainmeter\":[{\"value\":%.3f,\"unit\":\"mm/"
                "min\"},{\"value\":%.3f,\"unit\":\"mm/"
                "h\"}]}"),
          rainLevelLast1Minute, rainLevelLastHour);
#else
  sprintf(json, (const char *)F("{\"rainmeter\":[{\"value\":%.3f,\"unit\":\"%s\"},{\"value\":%."
                "3f,\"unit\":\"%s\"},{\"value\":%.3f,\"unit\":\"%s\"},{"
                "\"value\":%.3f,\"unit\":\"%s\"}]}"),
          rainLevelLast1Minute, AFE_UNIT_MMM, rainLevelLastHour, AFE_UNIT_MMH,
          rainLevelLast12Hours, AFE_UNIT_MM12H, rainLevelLast24Hours,
          AFE_UNIT_MM24H);
#endif
}

#endif // AFE_CONFIG_HARDWARE_RAINMETER