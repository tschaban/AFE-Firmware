/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Switch.h"

AFESwitch::AFESwitch(){};

AFESwitch::AFESwitch(uint8_t id) { begin(id); }

void AFESwitch::begin(uint8_t id) {
  AFEDataAccess Data;
  SwitchConfiguration = Data.getSwitchConfiguration(id);
  pinMode(SwitchConfiguration.gpio, INPUT_PULLUP);
  state = digitalRead(SwitchConfiguration.gpio);
  previousState = state;

  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0) {
    Led.begin(systeLedID - 1);
  }
  _initialized = true;
}

boolean AFESwitch::getState() { return state; }

boolean AFESwitch::isPressed() {
  if (pressed) {
    pressed = false;
    return true;
  } else {
    return false;
  }
}

boolean AFESwitch::is5s() {
  if (pressed4fiveSeconds) {
    pressed4fiveSeconds = false;
    return true;
  } else {
    return false;
  }
}

boolean AFESwitch::is10s() {
  if (pressed4tenSeconds) {
    pressed4tenSeconds = false;
    return true;
  } else {
    return false;
  }
}

void AFESwitch::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(SwitchConfiguration.gpio);
    unsigned long time = millis();

    if (currentState != previousState) { // buttons has been pressed

      if (startTime == 0) { // starting timer. used for switch sensitiveness
        startTime = time;
      }

      if (time - startTime >=
          SwitchConfiguration.sensitiveness) { // switch prssed, sensitiveness
                                               // taken into account, processing
                                               // event
        if (SwitchConfiguration.type == SWITCH_TYPE_MONO) {

          if (!_pressed) { // This is set only once when switch is pressed
            state = !state;
            _pressed = true;
            pressed = true;
          }

          /* Code only for Mulitifunction switch: pressed for 5 and 10 seconds
           */
          if (SwitchConfiguration.functionality == SWITCH_MULTI) {
            if (time - startTime >= 10000 && !pressed4tenSeconds) {
              pressed4tenSeconds = true;
            }

            if (time - startTime >= 5000 && !_pressed4fiveSeconds) {
              Led.blink(500);
              _pressed4fiveSeconds = true;
            }
          }
        } else { // This is BI-stable code
          state = !state;
          previousState = currentState;
          pressed = true;
        }
      }

    } else if (currentState == previousState && startTime > 0) {
      /* Code only for Mulitifunction switch: pressed for 5 and 10 seconds */
      if (SwitchConfiguration.functionality == SWITCH_MULTI) {
        if ( // SwitchConfiguration.type == SWITCH_TYPE_MONO &&
            time - startTime >= 5000 && time - startTime < 10000) {
          pressed4fiveSeconds = true;
        }
        _pressed4fiveSeconds = false;
      }
      startTime = 0;
      _pressed = false;
    }
  }
}

uint8_t AFESwitch::getFunctionality() {
  return SwitchConfiguration.functionality;
}

uint8_t AFESwitch::getControlledRelayID() {
  return SwitchConfiguration.relayID;
}
