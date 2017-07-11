#include <AFE-WiFi.h>
#include <AFE-Web-Server.h>
#include <AFE-EEPROM.h>
#include <Streaming.h>

AFEWiFi Network;
AFEWebServer WebServer;
AFEEEPROM Eeprom;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial << endl << "INFO: Erasing EEPROM: ";
  Eeprom.erase();
  Serial << "done";

  for (uint8_t i=0;i<255;i++) {
    Eeprom.write(i,1,i);
    Serial << endl << "Writing [" << i << "] = " << i << " char = " << (char)i;
  }

  for (uint8_t j=0;j<255;j++) {
    Serial << endl << "Reading [" << j << "] = " << Eeprom.readByte(j);
  }

  Network.setReconnectionParams(30,1,5);
  Network.begin("Za Strugiem","julia2105","AFE-DEVICE",WIFI_MODE_SERVER);
  Network.connect();
  WebServer.handle("/",handleRoot);
  WebServer.handle("/mqtt",handleMQTTConfiguration);
  WebServer.handle("/relay",handleRelayConfiguration);
  WebServer.handle("/ds18b20",handleDS18B20Configuration);
  WebServer.handle("/switch",handleSwitchConfiguration);
  WebServer.handle("/domoticz",handleDomoticzConfiguration);
  WebServer.handle("/upgrade",handleUpdateConfiguration);
  WebServer.handle("/reset",handleResetConfiguration);
  WebServer.handle("/exit",handleExitConfiguration);
  WebServer.handle("/favicon.ico",handleFavicon);
  WebServer.begin();

}

void loop() {
  if (Network.connected()) {
     WebServer.listener();
  } else {
     Network.connect();
  }

}
