/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Contactron.h"

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

AFEContactron::AFEContactron(){};

void AFEContactron::begin(uint8_t id, AFEDevice *_Device,
                          AFEDataAccess *_Data) {
  Data = _Data;
  Device = _Device;
  Data->getConfiguration(id, &configuration);
  pinMode(configuration.gpio, INPUT_PULLUP);

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON_GPIO_DIGIT_INPUT
  pinMode(configuration.gpio, INPUT);
#else
  pinMode(configuration.gpio, INPUT_PULLUP);
#endif


#ifdef AFE_CONFIG_HARDWARE_LED
  if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    ContactronLed.begin(Data, configuration.ledID);
  }
#endif
  _initialized = true;
}

boolean AFEContactron::get() {
  boolean currentState = digitalRead(configuration.gpio);
  boolean _return;
  if (configuration.type == AFE_CONTACTRON_NO) {
    if (currentState) {
#ifdef AFE_CONFIG_HARDWARE_LED
      ContactronLed.on();
#endif
      _return = AFE_CONTACTRON_OPEN;
    } else {
#ifdef AFE_CONFIG_HARDWARE_LED
      ContactronLed.off();
#endif
      _return = AFE_CONTACTRON_CLOSED;
    }
  } else {
    if (currentState) {
#ifdef AFE_CONFIG_HARDWARE_LED
      ContactronLed.off();
#endif
      _return = AFE_CONTACTRON_CLOSED;
    } else {
#ifdef AFE_CONFIG_HARDWARE_LED
      ContactronLed.on();
#endif
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

#endif // AFE_CONFIG_HARDWARE_CONTACTRON