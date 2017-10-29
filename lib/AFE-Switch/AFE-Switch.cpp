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

boolean AFESwitch::isON() { return state ? false : true; }

boolean AFESwitch::isOFF() { return state ? true : false; }

void AFESwitch::listener() {
  boolean currentState = digitalRead(gpio);
  if (currentState != previousState) {
    /*
        Serial << endl
               << "Current = " << currentState << " : Previous = " <<
       previousState
               << " : State = " << state << " millis() = " << millis()
               << " : startTime = " << startTime;
    */
    if (startTime == 0) {
      startTime = millis();
    }

    if (millis() - startTime >= sensitiveness) {

      if (type == SWITCH_TYPE_MONO) {
        if (!pressed) {
          state = !state;
          pressed = true;
        }
      } else {
        state = !state;
        previousState = currentState;
      }

      /*
            if (type == SWITCH_TYPE_BI) {
              state = currentState;
            } else {
              if (!currentState) {
                if (!pressed) {
                  pressed = true;
                  state = !state;
                }
              } else {
                pressed = false;
              }
            }
      */
    }
  } else if (currentState == previousState && startTime > 0) {
    startTime = 0;
    pressed = false;
  }
}
