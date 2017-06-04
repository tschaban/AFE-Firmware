#include "AFE-Web-Server.h"


AFEWebServer::AFEWebServer() {
}

void AFEWebServer::begin() {
        Serial << endl << "INFO: Starting web server";
        server.begin();
        Serial << endl << "INFO: Web server is working";
        Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
}

void  AFEWebServer::listener() {
  server.handleClient();
}

void AFEWebServer::generatePage(const String &page) {
  server.send(200, "text/html", page);
}

void AFEWebServer::handle(const char* uri,ESP8266WebServer::THandlerFunction handler) {
  Serial << endl << "INFO: Added url : " << uri << " for listening";
  server.on(uri, handler);
}
