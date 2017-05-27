#include <AFE-Switch.h>
#include <Streaming.h>

AFESwitch Switch(14);

boolean _switch = false;

void setup() {
   Serial.begin(115200);
   delay(10);
}

void loop() {
  Switch.listener();

  if (Switch.isON()) {
    if (_switch==false) {
       Serial << "turned ON" << endl;
       _switch=true;
    }
  }

  if (Switch.isOFF()) {
    if (_switch==true) {
      Serial << "turned OFF" << endl;
      _switch=false;
    }
  }

}
