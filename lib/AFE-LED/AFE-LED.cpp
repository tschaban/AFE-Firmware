#include "AFE-LED.h"

AFELED::AFELED(byte gpio) {
	_gpio = gpio;
  pinMode(_gpio, OUTPUT);
  digitalWrite(_gpio, HIGH);
}

void AFELED::on() {
  if (digitalRead(_gpio) == HIGH) {
    digitalWrite(_gpio, LOW);
  }
}

void AFELED::off() {
  if (digitalRead(_gpio) == LOW) {
    digitalWrite(_gpio, HIGH);
  }
}

void AFELED::blink(unsigned int duration) {
  on();
  delay(duration);
  off();
}

void AFELED::blinkingOn(unsigned long interval) {
   _interval = interval;
   _blinking = true;
}

void AFELED::blinkingOff() {
   _blinking = false;
}

void AFELED::loop() {
  if (_blinking==true) {
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= _interval) {
       _previousMillis = currentMillis;
       if (digitalRead(_gpio) == LOW) {
         off();
       } else {
         on();
       }
     }
  }
}
