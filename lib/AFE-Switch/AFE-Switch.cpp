#include "AFE-Switch.h"

AFESwitch::AFESwitch(){};

AFESwitch::AFESwitch(uint8_t switch_gpio, uint8_t switch_type,
                     uint16_t switch_sensitiveness) {
  begin(switch_gpio, switch_type, switch_sensitiveness);
}

void AFESwitch::begin(uint8_t switch_gpio, uint8_t switch_type,
                      uint16_t switch_sensitiveness) {
  gpio = switch_gpio;
  type = switch_type;
  sensitiveness = switch_sensitiveness;
  pinMode(gpio, INPUT_PULLUP);
  state = digitalRead(gpio);
  previousState = state;
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

  boolean currentState = digitalRead(gpio);
  unsigned long time = millis();

  if (currentState != previousState) {

    if (startTime == 0) {
      Serial << endl << "INFO: Switch timer started";
      startTime = time;
    }

    if (time - startTime >= sensitiveness) {

      if (type == SWITCH_TYPE_MONO) {

        if (!_pressed) {
          state = !state;
          _pressed = true;
          pressed = true;
        }

        if (time - startTime >= 10000 && !pressed4tenSeconds) {
          pressed4tenSeconds = true;
        }

      } else { // This is BI-stable code
        state = !state;
        previousState = currentState;
        pressed = true;
      }
    }
  } else if (currentState == previousState && startTime > 0) {
    Serial << endl << "INFO: Pressed for " << time - startTime;

    if (type == SWITCH_TYPE_MONO && time - startTime >= 5000 &&
        time - startTime < 10000) {
      pressed4fiveSeconds = true;
    }

    startTime = 0;
    _pressed = false;
  }
}
