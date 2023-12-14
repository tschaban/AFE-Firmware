/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Humidistat.h"

AFEHumidistat::AFEHumidistat(AFEDataAccess *_Data){Data = _Data};

void AFEHumidistat::begin(REGULATOR *config) { configuration = config; }

boolean AFEHumidistat::isReady()
{
  if (ready)
  {
    ready = false;
    return true;
  }
  else
  {
    return false;
  }
}

byte AFEHumidistat::getRelayState() { return relayState; }

void AFEHumidistat::listener(float currentTemperature)
{
  if (configuration->enabled)
  {
    if (configuration->turnOnAbove &&
        currentTemperature > configuration->turnOn)
    {
      relayState = AFE_RELAY_ON;
      ready = true;
    }
    else if (!configuration->turnOnAbove &&
             currentTemperature < configuration->turnOn)
    {
      relayState = AFE_RELAY_ON;
      ready = true;
    }
    else if (configuration->turnOffAbove &&
             currentTemperature > configuration->turnOff)
    {
      relayState = AFE_RELAY_OFF;
      ready = true;
    }
    else if (!configuration->turnOffAbove &&
             currentTemperature < configuration->turnOff)
    {
      relayState = AFE_RELAY_OFF;
      ready = true;
    }
  }
}

void AFEHumidistat::on()
{
  configuration->enabled = true;
  enable(configuration->enabled);
}
void AFEHumidistat::off()
{
  configuration->enabled = false;
  enable(configuration->enabled);
}
void AFEHumidistat::toggle()
{
  configuration->enabled ? configuration->enabled = false
                         : configuration->enabled = true;
  enable(configuration->enabled);
}
boolean AFEHumidistat::enabled() { return configuration->enabled; }

void AFEHumidistat::enable(boolean state)
{
  Data->saveRegulatorState(state, HUMIDISTAT_REGULATOR);
}
