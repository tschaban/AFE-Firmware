/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Thermal-Protector.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

AFEThermalProtector::AFEThermalProtector(){};

void AFEThermalProtector::begin(AFEDataAccess *Data, uint8_t id) {
  _Data = Data;
  _id = id;
  _Data->getConfiguration(id, &configuration);

#ifdef DEBUG
  Serial << endl
         << "INFO: Thermal protection initialized. Max temp: "
         << configuration.temperature;
#endif

  if (configuration.enabled &&
      (configuration.relayId == AFE_HARDWARE_ITEM_NOT_EXIST ||
       configuration.sensorId == AFE_HARDWARE_ITEM_NOT_EXIST)) {
    configuration.enabled = false;
#ifdef DEBUG
    Serial << endl
           << "INFO: Thermal protection: has been disabled becuase Relay ID "
              "and/or Sensor ID is missing ";
#endif
  }
};

bool AFEThermalProtector::listener(float currentTemperature) {
  if (configuration.enabled) {
    if (currentTemperature > configuration.temperature && !turnOff) {
      turnOff = true;

#ifdef DEBUG
      Serial << endl
             << "INFO: Thermal protection. Temperature has exceeded the max "
                "value: "
             << configuration.temperature;
#endif

    } else if (currentTemperature <= configuration.temperature && turnOff) {
      turnOff = false;
    }
    return true;
  } else {
#ifdef DEBUG
    Serial << endl << "INFO: Thermal protection: disabled";
#endif
    return false;
  }
};

void AFEThermalProtector::on(void) {
  configuration.enabled = true;
  enable();
}
void AFEThermalProtector::off(void) {
  configuration.enabled = false;
  enable();
}
void AFEThermalProtector::toggle(void) {
  configuration.enabled ? configuration.enabled = false
                        : configuration.enabled = true;
  enable();
}

void AFEThermalProtector::enable(void) {
  _Data->saveConfiguration(_id, &configuration);
}

/* Returns Thermal Protector data in JSON format */
void AFEThermalProtector::getJSON(char *json) {
  sprintf(json, "{\"enabled\":%s}", configuration.enabled ? "true" : "false");
}

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR