/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Regulator.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

AFERegulator::AFERegulator(){};

void AFERegulator::begin(AFEDataAccess *_Data, uint8_t id) {
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
    Serial << endl << "INFO: Regulator is turned off";
#endif
    return false;
  }
}

void AFERegulator::on() {
  configuration.enabled = true;
  enable(configuration.enabled);
}
void AFERegulator::off() {
  configuration.enabled = false;
  enable(configuration.enabled);
}
void AFERegulator::toggle() {
  configuration.enabled ? configuration.enabled = false
                        : configuration.enabled = true;
  enable(configuration.enabled);
}
boolean AFERegulator::enabled() { return configuration.enabled; }

void AFERegulator::enable(boolean state) {
  AFEDataAccess Data;
  // Data.saveRegulatorState(state, THERMOSTAT_REGULATOR);
}

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR