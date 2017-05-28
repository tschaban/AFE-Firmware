#include "AFE-Switch.h"


AFESwitch::AFESwitch(uint8_t gpio,uint8_t type) {
  _gpio = gpio;
  pinMode(_gpio, INPUT_PULLUP);
  _state = digitalRead(_gpio);
  _type = type;
}

boolean AFESwitch::isON() {
  return _state?false:true;
}

boolean AFESwitch::isOFF() {
  return _state?true:false;
}

void AFESwitch::listener() {
   boolean currentState = digitalRead(_gpio);
   if (_type==SWITCH_TYPE_MONO) {
     _state = currentState;
   } else {
     if (!currentState) {
        if (!_pressed) {
          _pressed = true;
          _state = !_state;
        }
     } else {
       _pressed = false;
     }
   }
}
