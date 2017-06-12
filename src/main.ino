#include <AFE-WiFi.h>
#include <AFE-Web-Server.h>

#include <Streaming.h>

AFEWiFi Network;
AFEWebServer WebServer;


void setup() {
  Serial.begin(115200);
  delay(10);

  Network.setReconnectionParams(30,1,5);
  Network.begin("POLICJA: Posterunek","1231231234","AFE-DEVICE",WIFI_MODE_SERVER);
  Network.connect();
  WebServer.handle("/",handleRoot);
  WebServer.handle("/mqtt",handleMQTTConfiguration);
  WebServer.handle("/relay",handleRelayConfiguration);
  WebServer.handle("/ds18b20",handleDS18B20Configuration);
  WebServer.handle("/switch",handleSwitchConfiguration);
  WebServer.handle("/domoticz",handleDomoticzConfiguration);
  WebServer.handle("/update",handleUpdateConfiguration);
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
