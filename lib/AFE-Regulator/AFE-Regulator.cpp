/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Regulator.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

AFERegulator::AFERegulator(){};

void AFERegulator::begin(AFEDataAccess *Data, uint8_t id) {
  _Data = Data;
  _id = id;
  _Data->getConfiguration(id, &configuration);
}

void AFERegulator::addRelayReference(AFERelay *Relay) {
  _Relay = Relay;
  initialized = true;
#ifdef DEBUG
    Serial << endl << "INFO: Regulator initialized";
#endif  
}

boolean AFERegulator::listener(float value) {
  if (configuration.enabled && initialized) {
#ifdef DEBUG
    Serial << endl << "INFO: Executing regulator check ...";
#endif
    deviceState = _Relay->get();
    if (configuration.turnOnAbove && value > configuration.turnOn) {
      deviceState = AFE_RELAY_ON;
    } else if (!configuration.turnOnAbove && value < configuration.turnOn) {
      deviceState = AFE_RELAY_ON;
    } else if (configuration.turnOffAbove && value > configuration.turnOff) {
      deviceState = AFE_RELAY_OFF;
    } else if (!configuration.turnOffAbove && value < configuration.turnOff) {
      deviceState = AFE_RELAY_OFF;
    }
    return true;
  } else {
#ifdef DEBUG
    Serial << endl << "INFO: Regulator: disabled";
#endif
    return false;
  }
}

void AFERegulator::on(void) {
  configuration.enabled = true;
  enable();
}
void AFERegulator::off(void) {
  configuration.enabled = false;
  enable();
}
void AFERegulator::toggle(void) {
  configuration.enabled ? configuration.enabled = false
                        : configuration.enabled = true;
  enable();
}

void AFERegulator::enable(void) {
  _Data->saveConfiguration(_id, &configuration);
}

/* Returns Regulator data in JSON format */
void AFERegulator::getJSON(char *json) {
  sprintf(json, "{\"enabled\":%s}", configuration.enabled ? "true" : "false");
}

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR