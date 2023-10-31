/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-Anemometer.h"

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER

AFEAnemometer::AFEAnemometer(){};

#ifdef DEBUG
/* Init switch */
boolean AFEAnemometer::begin(AFEDataAccess *Data, AFEImpulseCatcher *Sensor, AFEDebugger *_Debugger)
{
  Debugger = _Debugger;
  return begin(Data, Sensor);
}
#endif

boolean AFEAnemometer::begin(AFEDataAccess *Data,
                             AFEImpulseCatcher *Sensor)
{
  _Data = Data;
  _Sensor = Sensor;
  startTime = millis();
  Data->getConfiguration(configuration);
  _Sensor->begin(configuration->sensitiveness);
  _initialized = true;

  switch (configuration->impulseDistanceUnit)
  {
  case AFE_DISTANCE_CENTIMETER:
    oneImpulseDistanceCM = configuration->impulseDistance;
    break;
  case AFE_DISTANCE_METER:
    oneImpulseDistanceCM = configuration->impulseDistance * 100;
    break;
  case AFE_DISTANCE_KILOMETER:
    oneImpulseDistanceCM = configuration->impulseDistance * 1000;
    break;
  default:
    _initialized = true;
#ifdef DEBUG
    Debugger->printError(F("Anemometer sensor NOT initialized. Wrong distance unit for impulse: "), F("ANEMOMETER"));
    Debugger->printValue(configuration->impulseDistanceUnit);
#endif
    break;
  }

  if (_initialized)
  {
#ifdef DEBUG
    Debugger->printHeader(2, 1, 50, AFE_DEBUG_HEADER_TYPE_DASH);
    Debugger->printBulletPoint(F("Anemometer sensor initialized and working"));
    Debugger->printBulletPoint(F("GPIO: "));
    Debugger->printValue(configuration->gpio);
    Debugger->printBulletPoint(F("Interval: "));
    Serial << configuration->interval;
    Debugger->printBulletPoint(F("1 impulse distance:: "));
    Debugger->printValue(configuration->impulseDistance);
    Debugger->printBulletPoint(F("1 impulse unit:: "));
    Debugger->printValue(configuration->impulseDistanceUnit);
    Debugger->printBulletPoint(F("Boucing: "));
    Debugger->printValue(configuration->sensitiveness);
    Debugger->printHeader(1, 2, 50, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
  }

  return _initialized;
}

boolean AFEAnemometer::listener(void)
{
  boolean _ret = false;
  if (_initialized)
  {
    if ((millis() - startTime >= configuration->interval * 1000))
    {

      uint32_t noOfImpulses;
      uint32_t duration;
      _Sensor->get(noOfImpulses, duration);

      lastSpeedMS =
          ((noOfImpulses * (oneImpulseDistanceCM / 100) * 1000) / duration);
      lastSpeedKMH = lastSpeedMS * 18 / 5;

#ifdef DEBUG
      Debugger->printHeader(1, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("Reading data from Anemometer sensor"));
      Debugger->printBulletPoint(F("Speed [m/s]: "));
      Serial << lastSpeedMS;
      Debugger->printBulletPoint(F("Speed [km/h]:  speed: "));
      Serial << lastSpeedKMH;
      Debugger->printBulletPoint(F("Number of impulses: "));
      Serial << noOfImpulses;
      Debugger->printBulletPoint(F("Interval [msec]: "));
      Serial << duration;
      Debugger->printHeader(1, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
      startTime = millis();
      _ret = true;
    }
  }
  return _ret;
}

void AFEAnemometer::newImpulse(void)
{
  impulseCounter++;
#ifdef DEBUG
  Debugger->printInformation(F("New impulse. Total: "), F("ANEMOMETER"));
  Serial << impulseCounter << F(", during: ") << ((millis() - counterStarted) / 1000) << F("sec.");
#endif
}

void AFEAnemometer::get(uint32_t &noOfImpulses, uint32_t &duration)
{
  duration = millis() - counterStarted;
  if (duration < 0)
  { // used previous duration if timer rollouts
    duration = _previousDuration;
  }
  noOfImpulses = impulseCounter;
  _previousDuration = duration;
  impulseCounter = 0;
  counterStarted = millis();
#ifdef DEBUG
  Debugger->printInformation(F("Reading data from Binary sensor: Impulses: "), F("ANEMOMETER"));
  Serial << noOfImpulses
         << F(", during: ") << (duration / 1000) << F("sec.");
#endif
}
void AFEAnemometer::getJSON(char *json)
{
  sprintf(json,
          (const char *)F("{\"anemometer\":[{\"value\":%.2f,\"unit\":\"%s\"},{\"value\":%.2f,\"unit\":\"%s\"}]}"),
          lastSpeedMS, AFE_UNIT_MS, lastSpeedKMH, AFE_UNIT_KMH);
}

#endif // AFE_CONFIG_HARDWARE_ANEMOMETER
