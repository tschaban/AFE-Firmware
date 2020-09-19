/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Regulator.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

AFERegulator::AFERegulator(){};

void AFERegulator::begin(AFEDataAccess *Data, uint8_t id) {
  _Data = Data;
  _id = id;
  _Data->getConfiguration(id, &configuration);
}

boolean AFERegulator::listener(float value) {
  if (configuration.enabled) {
#ifdef DEBUG
    Serial << endl << "INFO: Executing regulator check ...";
#endif

    if (configuration.turnOnAbove && value > configuration.turnOn) {
      deviceState = true;
    } else if (!configuration.turnOnAbove && value < configuration.turnOn) {
      deviceState = true;
    } else if (configuration.turnOffAbove && value > configuration.turnOff) {
      deviceState = false;
    } else if (!configuration.turnOffAbove && value < configuration.turnOff) {
      deviceState = false;
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
  sprintf(json, "{}");
}

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR