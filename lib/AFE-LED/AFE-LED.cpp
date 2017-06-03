#include "AFE-LED.h"

AFELED::AFELED(uint8_t led_gpio) {
	gpio = led_gpio;
  pinMode(gpio, OUTPUT);
  digitalWrite(gpio, HIGH);
}

void AFELED::on() {
  if (digitalRead(gpio) == HIGH) {
    digitalWrite(gpio, LOW);
  }
}

void AFELED::off() {
  if (digitalRead(gpio) == LOW) {
    digitalWrite(gpio, HIGH);
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

void AFELED::blinkingOff() {
   blinking = false;
}

void AFELED::loop() {
  if (blinking==true) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;
       if (digitalRead(gpio) == LOW) {
         off();
       } else {
         on();
       }
     }
  }
}
