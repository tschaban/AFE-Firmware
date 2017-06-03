#include "AFE-Web-Server.h"
#include "AFE-Sites-Handlers.cpp"

AFEWebServer::AFEWebServer() {
}

void AFEWebServer::begin() {
        Serial << endl << "INFO: Starting web server";
        server.on("/", handleRoot);
        server.on("/favicon.ico",handleFavicon);
        server.onNotFound(handleNotFound);
        server.begin();
        Serial << endl << "INFO: Web server is working";
        Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
}

void  AFEWebServer::handleRequest() {
  server.handleClient();
}

void AFEWebServer::generatePage() {
  server.send(200, "text/html", "PAGE");
}
