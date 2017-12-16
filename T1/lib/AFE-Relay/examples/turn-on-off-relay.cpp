#include <AFE-Relay.h>

AFERelay  Relay(12);

void setup() {}

void loop() {
   Relay.on();
   delay(1000);
   Relay.off();
   delay(1000);
}
