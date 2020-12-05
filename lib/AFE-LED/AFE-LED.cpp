/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-LED.h"

#ifdef AFE_CONFIG_HARDWARE_LED

AFELED::AFELED() {}

boolean AFELED::begin(AFEDataAccess *_Data, uint8_t id) {
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    _Data->getConfiguration(id, &LEDConfiguration);
    pinMode(LEDConfiguration.gpio, OUTPUT);
    LEDConfiguration.changeToOppositeValue
        ? digitalWrite(LEDConfiguration.gpio, LOW)
        : digitalWrite(LEDConfiguration.gpio, HIGH);
    _initialized = true;
  }
  return _initialized;
}

void AFELED::on() {
  if (LEDConfiguration.changeToOppositeValue) {
    set(HIGH);
  } else {
    set(LOW);
  }
}

void AFELED::off() {
  if (LEDConfiguration.changeToOppositeValue) {
    set(LOW);
  } else {
    set(HIGH);
  }
}

void AFELED::blink(unsigned int duration) {
  on();
  delay(duration);
  off();
}

void AFELED::blinkingOn(unsigned long blinking_interval) {
  interval = blinking_interval;
  blinking = true;
}

void AFELED::blinkingOff() { blinking = false; }

boolean AFELED::isBlinking() { return blinking; }

void AFELED::loop() {
  if (_initialized && blinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggle();
    }
  }
}

void AFELED::set(uint8_t state) {
  if (_initialized) {
    if (digitalRead(LEDConfiguration.gpio) != state) {
      digitalWrite(LEDConfiguration.gpio, state);
    }
  }
}

void AFELED::toggle() {
  if (_initialized) {
    digitalRead(LEDConfiguration.gpio) == HIGH
        ? digitalWrite(LEDConfiguration.gpio, LOW)
        : digitalWrite(LEDConfiguration.gpio, HIGH);
  }
}

#endif // AFE_CONFIG_HARDWARE_LED