/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Rainmeter.h"

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

AFESensorRainmeter::AFESensorRainmeter(){};

boolean AFESensorRainmeter::begin(AFEDataAccess *Data, AFESensorBinary *Sensor) {
  _Data = Data;
  _Sensor = Sensor;
 Data->getConfiguration(&configuration);

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
#endif

  _initialized = true;

  _Sensor->begin(configuration.sensitiveness);
#ifdef DEBUG
  Serial << endl
         << F("INFO: Rain sensor initialized and working") << endl
         << F(" - GPIO: ") << configuration.gpio << endl
         << F(" - Interval: ") << configuration.interval << endl
         << F(" - Boucing: ") << configuration.sensitiveness;
#endif

  /* Clean arrays */
  for (uint8_t i = 0; i < 60; i++) {
    rainLevelDuring1Hour[i] = 0;
  }
  for (uint8_t i = 0; i < 12; i++) {
    rainLevelDuring12Hours[i] = 0;
  }
  rainLevelDuring24Hours[0] = 0;
  rainLevelDuring24Hours[1] = 0;

  startTime = millis();
  start60Sec = millis();

  return _initialized;
}

boolean AFESensorRainmeter::listener(void) {
  boolean _ret = false;
  if (_initialized) {

    // Calculate every 60sec
    if ((millis() - start60Sec >= 60000)) {

      uint32_t noOfImpulses;
      uint32_t duration;
      _Sensor->get(noOfImpulses, duration);

      /* Level during last 1 minute */
      rainLevelLast1Minute = noOfImpulses * configuration.resolution/ 1000;

      /* Storing water level for passed 1 minute */
      rainLevelDuring1Hour[currentIndex1Hour] = rainLevelLast1Minute;

      /* Recalculating the water level during the last hour */
      rainLevelLastHour = 0;
      for (uint8_t i = 0; i <= currentIndex1Hour; i++) {
        rainLevelLastHour = rainLevelLastHour + rainLevelDuring1Hour[i];
      }

      /* Storing water level for passed hour */
      rainLevelDuring12Hours[currentIndex12Hours] = rainLevelLastHour;
      /* Recalculating the water level during the last 12 hours */
      rainLevelLast12Hours = 0;
      for (uint8_t i = 0; i <= currentIndex12Hours; i++) {
        rainLevelLast12Hours = rainLevelLast12Hours + rainLevelDuring12Hours[i];
      }

      rainLevelDuring24Hours[currentIndex24Hours] = rainLevelLast12Hours;

      rainLevelLast24Hours =
          rainLevelDuring24Hours[0] + rainLevelDuring24Hours[1];

      if (currentIndex1Hour == 59) {

        if (currentIndex12Hours == 11) {
          currentIndex24Hours = currentIndex24Hours == 0 ? 1 : 0;
        }

        /* Changing index for 12 hours array */
        currentIndex12Hours =
            currentIndex12Hours == 11 ? 0 : currentIndex12Hours + 1;
      }

      /* Changing index for 60 minutes array */
      currentIndex1Hour = currentIndex1Hour == 59 ? 0 : currentIndex1Hour + 1;

#ifdef DEBUG
      Serial << endl
             << F("INFO: Reading rain sensor data:") << endl
             << F(" - last 1min: ") << rainLevelLast1Minute << F(" mm/min") << endl
             << F(" - last hour: ") << rainLevelLastHour << F(" mm/hour") << endl
             << F(" - last 12 hours: ") << rainLevelLast12Hours << F(" mm/12-hours")
             << endl
             << F(" - last 24 hours: ") << rainLevelLast24Hours << F(" mm/24-hours")
             << endl
             << F(" - INDEX: ") << endl
             << F("  : Hour Index= ") << currentIndex1Hour << endl
             << F("  : 12 Hours Index = ") << currentIndex12Hours << endl
             << F("  : 24 Hours index = ") << currentIndex24Hours << endl;

#endif

      start60Sec = millis();
    }
  }

  /* Counter for calculating the data */
  if ((millis() - startTime >= configuration.interval * 1000)) {
    startTime = millis();
    _ret = true;
  }

  return _ret;
}

void AFESensorRainmeter::getJSON(char *json) {
  sprintf(json, "{\"rainmeter\":[{\"value\":%.3f,\"unit\":\"mm/"
                "min\"},{\"value\":%.3f,\"unit\":\"mm/h\"},{\"value\":%.3f,\"unit\":\"mm/12h\"},{\"value\":%.3f,\"unit\":\"mm/24h\"}]}",
          rainLevelLast1Minute, rainLevelLastHour, rainLevelLast12Hours, rainLevelLast24Hours);
}

#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR