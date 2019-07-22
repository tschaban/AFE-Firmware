/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Contactron.h"

AFEContactron::AFEContactron(){};

void AFEContactron::begin(uint8_t id, AFEDevice *_Device,
                          AFEDataAccess *_Data) {
  Data = _Data;
  Device = _Device;
  configuration = Data->getContactronConfiguration(id);
  pinMode(configuration.gpio, INPUT_PULLUP);
  state = digitalRead(configuration.gpio);
  state = state;
  if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    ContactronLed.begin(configuration.ledID);
  }

  _initialized = true;
  convert();
}

void AFEContactron::convert() {
  if (configuration.type == CONTACTRON_NO) {
    if (state) {
      ContactronLed.on();
      _state = CONTACTRON_OPEN;
    } else {
      ContactronLed.off();
      _state = CONTACTRON_CLOSED;
    }
  } else {
    if (state) {
      ContactronLed.off();
      _state = CONTACTRON_CLOSED;
    } else {
      ContactronLed.on();
      _state = CONTACTRON_OPEN;
    }
  }
}

byte AFEContactron::get() { return _state; }

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
        convert();
        _changed = true;
      }

    } else if (currentState == state && startTime > 0) {
      startTime = 0;
    }
  }
}

const char *AFEContactron::getMQTTCommandTopic() {
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
  return mqttCommandTopic;
}

const char *AFEContactron::getMQTTStateTopic() {
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = '\0';
  }
  return mqttStateTopic;
}

unsigned long AFEContactron::getDomoticzIDX() {
  return configuration.domoticz.idx;
}
