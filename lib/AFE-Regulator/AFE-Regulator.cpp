/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Regulator.h"

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

AFERegulator::AFERegulator(){};

void AFERegulator::begin(AFEDataAccess *Data, uint8_t id) {
  _Data = Data;
  _id = id;
  _Data->getConfiguration(id, &configuration);

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Defining get and state MQTT Topics */
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
  }

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

}

void AFERegulator::setInitialState(boolean state) {
  deviceState = state;
  initialized = true;
}

boolean AFERegulator::listener(float value) {
  if (configuration.enabled && initialized) {
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
  sprintf(json, "{\"enabled\":%s}", configuration.enabled ? "true" : "false");
}

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR