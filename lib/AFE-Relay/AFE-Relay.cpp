#include "AFE-Relay.h"


AFERelay::AFERelay(uint8_t relay_gpio) {
	gpio = relay_gpio;
	pinMode(gpio, OUTPUT);
}

byte AFERelay::get() {
	return digitalRead(gpio) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on() {
	if (get() == RELAY_OFF) {
		digitalWrite(gpio, HIGH);
	}
}

/* Set relay to OFF */
void AFERelay::off() {
	if (get() == RELAY_ON) {
		digitalWrite(gpio, LOW);
	}
}

/* Toggle relay */
void AFERelay::toggle() {
	if (digitalRead(gpio) == LOW) {
		on();
	}
	else {
		off();
	}
}
