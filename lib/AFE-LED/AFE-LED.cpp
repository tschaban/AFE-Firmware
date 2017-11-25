#include "AFE-LED.h"

AFELED::AFELED() {}

AFELED::AFELED(uint8_t id) { begin(id); }

void AFELED::begin(uint8_t id) {
  AFEDevice Device;
  if (Device.configuration.isLED[id]) {
    AFEDataAccess Data;
    LEDConfiguration = Data.getLEDConfiguration(id);
    Data = {};
    pinMode(LEDConfiguration.gpio, OUTPUT);
    digitalWrite(LEDConfiguration.gpio, HIGH);
    _initialized = true;
  }
}

void AFELED::on() {
  if (_initialized && digitalRead(LEDConfiguration.gpio) == HIGH) {
    digitalWrite(LEDConfiguration.gpio, LOW);
  }
}

void AFELED::off() {
  if (_initialized && digitalRead(LEDConfiguration.gpio) == LOW) {
    digitalWrite(LEDConfiguration.gpio, HIGH);
  }
}

void AFELED::blink(unsigned int duration) {
  if (_initialized) {
    on();
    delay(duration);
    off();
  }
}

void AFELED::blinkingOn(unsigned long blinking_interval) {
  interval = blinking_interval;
  blinking = true;
}

void AFELED::blinkingOff() { blinking = false; }

void AFELED::loop() {
  if (_initialized && blinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (digitalRead(LEDConfiguration.gpio) == LOW) {
        off();
      } else {
        on();
      }
    }
  }
}
