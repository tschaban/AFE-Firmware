#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {
}

void AFEWebServer::begin() {
        Serial << endl << "INFO: Starting web server";
        server.begin();
        Serial << endl << "INFO: Web server is working";
        Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
}

void AFEWebServer::listener() {
        server.handleClient();
}

void AFEWebServer::publishHTML(String &page) {
        server.send(200, "text/html", page);
}

void AFEWebServer::handle(const char* uri,ESP8266WebServer::THandlerFunction handler) {
        Serial << endl << "INFO: Added url : " << uri << " for listening";
        server.on(uri, handler);
}

uint8_t AFEWebServer::getUrlCommand() {
        if (server.hasArg("command")) {
                if (server.arg(0).toInt()==SERVER_CMD_SAVE) {
                        return SERVER_CMD_SAVE;
                }
        }
        return SERVER_CMD_NONE;
}
