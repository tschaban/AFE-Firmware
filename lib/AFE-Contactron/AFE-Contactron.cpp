/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Contactron.h"

AFEContactron::AFEContactron(){};

AFEContactron::AFEContactron(uint8_t id) { begin(id); }

void AFEContactron::begin(uint8_t id) {
  AFEDataAccess Data;
  ContactronConfiguration = Data.getContactronConfiguration(id);
  pinMode(ContactronConfiguration.gpio, INPUT_PULLUP);
  state = digitalRead(ContactronConfiguration.gpio);
  previousState = state;

  if (ContactronConfiguration.ledID > 0) {
    ContactronLed.begin(ContactronConfiguration.ledID - 1);
  }
  _initialized = true;
}

boolean AFEContactron::getState() {
  uint8_t _return;
  if (ContactronConfiguration.outputDefaultState == CONTACTRON_NO) {
    if (state) {
      ContactronLed.on();
      _return = CONTACTRON_OPEN;
    } else {
      ContactronLed.off();
      _return = CONTACTRON_CLOSED;
    }
  } else {
    if (state) {
      ContactronLed.on();
      _return = CONTACTRON_CLOSED;
    } else {
      ContactronLed.off();
      _return = CONTACTRON_OPEN;
    }
  }
  return _return;
}

boolean AFEContactron::changed() {
  if (connected) {
    connected = false;
    return true;
  } else {
    return false;
  }
}

void AFEContactron::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(ContactronConfiguration.gpio);
    unsigned long time = millis();

    if (currentState != previousState) { // contactron stage changed

      if (startTime == 0) { // starting timer. used for contactron bouncing
        startTime = time;
      }

      if (time - startTime >= ContactronConfiguration.bouncing) {

        state = !state;
        previousState = currentState;
        connected = true;
      }

    } else if (currentState == previousState && startTime > 0) {
      startTime = 0;
      _connected = false;
    }
  }
}
