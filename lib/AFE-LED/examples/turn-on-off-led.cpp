#include <AFE-LED.h>

AFELED LED(13);

void setup() {
}

void loop() {
        LED.on();
        delay(1000);
        LED.off();
        delay(1000);
}
