#include <AFE-WiFi.h>

AFEWiFi Network;

void setup() {
   Serial.begin(115200);
   delay(10);

   Network.setReconnectionParams(10,1,5);
   Network.begin("SSID","password","AFE-DEVICE");

}

void loop() {
  if (Network.connected()) {
    // Connected 
  } else {
     Network.connect();
  }
}
