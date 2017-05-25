#include <ESP8266WiFi.h>
#include <AFE-Relay.h>

AFERelay             Relay(14);

void setup() {

}

void loop() {
   Relay.on();
   delay(1000);
   Relay.off();
}
