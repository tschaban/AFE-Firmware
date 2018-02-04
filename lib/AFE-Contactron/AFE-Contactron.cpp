/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Contactron.h"

AFEContactron::AFEContactron(){};

AFEContactron::AFEContactron(uint8_t id) { begin(id); }

void AFEContactron::begin(uint8_t id) {
  AFEDataAccess Data;
  ContactronConfiguration = Data.getContactronConfiguration(id);
  MQTT MQTTConfiguration;
  MQTTConfiguration = Data.getMQTTConfiguration();
  pinMode(ContactronConfiguration.gpio, INPUT_PULLUP);
  state = digitalRead(ContactronConfiguration.gpio);
  state = state;
  sprintf(mqttTopic, "%s%s/", MQTTConfiguration.topic,
          ContactronConfiguration.name);
  if (ContactronConfiguration.ledID > 0) {
    ContactronLed.begin(ContactronConfiguration.ledID - 1);
  }
  _initialized = true;
}

void AFEContactron::convert() {
  if (ContactronConfiguration.outputDefaultState == CONTACTRON_NO) {
    if (state) {
      ContactronLed.on();
      _state = CONTACTRON_OPEN;
    } else {
      ContactronLed.off();
      _state = CONTACTRON_CLOSED;
    }
  } else {
    if (state) {
      ContactronLed.on();
      _state = CONTACTRON_CLOSED;
    } else {
      ContactronLed.off();
      _state = CONTACTRON_OPEN;
    }
  }
}

byte AFEContactron::get() { return _state; }

const char *AFEContactron::getName() { return ContactronConfiguration.name; }

const char *AFEContactron::getMQTTTopic() {
  return ContactronConfiguration.mqttTopic;
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
    boolean currentState = digitalRead(ContactronConfiguration.gpio);
    unsigned long time = millis();

    if (currentState != state) { // contactron stage changed

      if (startTime == 0) { // starting timer. used for contactron bouncing
        startTime = time;
      }

      if (time - startTime >= ContactronConfiguration.bouncing) {
        state = currentState;
        convert();
        _changed = true;
      }

    } else if (currentState == state && startTime > 0) {
      startTime = 0;
    }
  }
}
