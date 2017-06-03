#include "AFE-Switch.h"


AFESwitch::AFESwitch(uint8_t switch_gpio,uint8_t switch_type) {
  gpio = switch_gpio;
  pinMode(gpio, INPUT_PULLUP);
  state = digitalRead(gpio);
  type = switch_type;
}

boolean AFESwitch::isON() {
  return state?false:true;
}

boolean AFESwitch::isOFF() {
  return state?true:false;
}

void AFESwitch::listener() {
   boolean currentState = digitalRead(gpio);
   if (type==SWITCH_TYPE_MONO) {
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
}
