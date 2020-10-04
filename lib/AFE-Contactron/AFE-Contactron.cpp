/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Contactron.h"

AFEContactron::AFEContactron(){};

void AFEContactron::begin(uint8_t id, AFEDevice *_Device,
                          AFEDataAccess *_Data) {
  Data = _Data;
  Device = _Device;
  Data->getConfiguration(id,&configuration);
  pinMode(configuration.gpio, INPUT_PULLUP);
  if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    ContactronLed.begin(configuration.ledID);
  }

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
    mqttStateTopic[0] = AFE_EMPTY_STRING;
  }
#endif

  _initialized = true;
}

boolean AFEContactron::get() {
  boolean currentState = digitalRead(configuration.gpio);
  boolean _return;
  if (configuration.type == AFE_CONTACTRON_NO) {
    if (currentState) {
      ContactronLed.on();
      _return = AFE_CONTACTRON_OPEN;
    } else {
      ContactronLed.off();
      _return = AFE_CONTACTRON_CLOSED;
    }
  } else {
    if (currentState) {
      ContactronLed.off();
      _return = AFE_CONTACTRON_CLOSED;
    } else {
      ContactronLed.on();
      _return = AFE_CONTACTRON_OPEN;
    }
  }

  return _return;
}

boolean AFEContactron::changed() {
  if (_changed) {
    _changed = false;
    return true;
  } else {
    return false;
  }
}

void AFEContactron::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(configuration.gpio);
    unsigned long time = millis();

    if (currentState != state) { // contactron stage changed

      if (startTime == 0) { // starting timer. used for contactron bouncing
        startTime = time;
      }

      if (time - startTime >= configuration.bouncing) {
        state = currentState;
        _changed = true;
      }

    } else if (currentState == state && startTime > 0) {
      startTime = 0;
    }
  }
}

void AFEContactron::getJSON(char *json) {
  sprintf(json, "{\"state\":\":%s\"}",
          (get() == AFE_CONTACTRON_OPEN ? "open" : "closed"));
}