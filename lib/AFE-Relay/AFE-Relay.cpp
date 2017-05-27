#include "AFE-Relay.h"


AFERelay::AFERelay(byte gpio) {
	_gpio = gpio;
	pinMode(_gpio, OUTPUT);
}

byte AFERelay::get() {
	return digitalRead(_gpio) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void AFERelay::on() {
	if (get() == RELAY_OFF) {
		digitalWrite(_gpio, HIGH);
	}
}

/* Set relay to OFF */
void AFERelay::off() {
	if (get() == RELAY_ON) {
		digitalWrite(_gpio, LOW);
	}
}

/* Toggle relay */
void AFERelay::toggle() {
	if (digitalRead(_gpio) == LOW) {
		on();
	}
	else {
		off();
	}
}
