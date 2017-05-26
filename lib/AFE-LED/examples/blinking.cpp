#include <AFE-LED.h>

AFELED  LED(13);

void setup() {
  LED.blinkingOn(100);
}

void loop() {
   LED.loop();
}
