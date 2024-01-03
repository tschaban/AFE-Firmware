/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Thermal-Protector.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef DEBUG
AFEThermalProtector::AFEThermalProtector(AFEDataAccess *Data,
                                         AFEDebugger *Debugger) {
  _Data = Data;
  _Debugger = Debugger;
};
#else
AFEThermalProtector::AFEThermalProtector(AFEDataAccess *Data) { _Data = Data; };
#endif

void AFEThermalProtector::begin(uint8_t id) {
  _id = id;
  _Data->getConfiguration(id, configuration);

#ifdef DEBUG
  _Debugger->printInformation(F("Initialized. Max temp set: "),
                              F("Thermal protection"));
  _Debugger->printValue(configuration->temperature);
#endif

  if (configuration->enabled &&
      (configuration->relayId == AFE_HARDWARE_ITEM_NOT_EXIST ||
       configuration->sensorId == AFE_HARDWARE_ITEM_NOT_EXIST)) {
    configuration->enabled = false;
#ifdef DEBUG
    _Debugger->printInformation(
        F("Disabled becuase Relay ID and/or Sensor ID is missing"),
        F("Thermal protection"));
#endif
  }
};

bool AFEThermalProtector::listener(float currentTemperature) {
  if (configuration->enabled) {
    if (currentTemperature > configuration->temperature && !turnOff) {
      turnOff = true;

#ifdef DEBUG
      _Debugger->printInformation(F("Temperature has exceeded the max value: "),
                                  F("Thermal protection"));
      _Debugger->printValue(configuration->temperature);
#endif

    } else if (currentTemperature <= configuration->temperature && turnOff) {
      turnOff = false;
    }
    return true;
  } else {
#ifdef DEBUG
    _Debugger->printInformation(F("Disabled"), F("Thermal protection"));
#endif
    return false;
  }
};

void AFEThermalProtector::on(void) {
  configuration->enabled = true;
  enable();
}
void AFEThermalProtector::off(void) {
  configuration->enabled = false;
  enable();
}
void AFEThermalProtector::toggle(void) {
  configuration->enabled ? configuration->enabled = false
                         : configuration->enabled = true;
  enable();
}

void AFEThermalProtector::enable(void) {
  _Data->saveConfiguration(_id, configuration);
}

/* Returns Thermal Protector data in JSON format */
void AFEThermalProtector::getJSON(char *json) {
  sprintf(json, (const char *)F("{\"enabled\":%s}"),
          configuration->enabled ? (const char *)F("true")
                                 : (const char *)F("false"));
}

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR