/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Regulator.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef DEBUG
AFERegulator::AFERegulator(AFEDataAccess *Data, AFEDebugger *Debugger) {
  _Data = Data;
  _Debugger = Debugger;
};
#else
AFERegulator::AFERegulator(AFEDataAccess *Data) { _Data = Data; };
#endif

void AFERegulator::begin(uint8_t id, AFERelay *Relay) {
  _id = id;
  _Relay = Relay;
  initialized = true;
  _Data->getConfiguration(id, configuration);
#ifdef DEBUG
  _Debugger->printInformation(F("initialized"), F("Regulator"));
#endif
}

boolean AFERegulator::listener(float value) {
  if (configuration->enabled && initialized) {
#ifdef DEBUG
    _Debugger->printInformation(F("Executing regulator check ..."),
                                F("Regulator"));
#endif
    deviceState = _Relay->get();
    if (configuration->turnOnAbove && value > configuration->turnOn) {
      deviceState = AFE_RELAY_ON;
    } else if (!configuration->turnOnAbove && value < configuration->turnOn) {
      deviceState = AFE_RELAY_ON;
    } else if (configuration->turnOffAbove && value > configuration->turnOff) {
      deviceState = AFE_RELAY_OFF;
    } else if (!configuration->turnOffAbove && value < configuration->turnOff) {
      deviceState = AFE_RELAY_OFF;
    }
    return true;
  } else {
#ifdef DEBUG
    _Debugger->printInformation(F("Disabled"), F("Regulator"));
#endif
    return false;
  }
}

void AFERegulator::on(void) {
  configuration->enabled = true;
  enable();
}
void AFERegulator::off(void) {
  configuration->enabled = false;
  enable();
}
void AFERegulator::toggle(void) {
  configuration->enabled ? configuration->enabled = false
                         : configuration->enabled = true;
  enable();
}

void AFERegulator::enable(void) {
  _Data->saveConfiguration(_id, configuration);
}

/* Returns Regulator data in JSON format */
void AFERegulator::getJSON(char *json) {
  sprintf(json, (const char *)F("{\"enabled\":%s}"),
          configuration->enabled ? (const char *)F("true")
                                 : (const char *)F("false"));
}

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR