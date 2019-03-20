#include "AFE-OTA.h"

static const char serverIndex[] PROGMEM =
    R"(<html><body><form method='POST' action='' enctype='multipart/form-data'>
                  <input type='file' name='update'>
                  <input type='submit' value='Update'>
               </form>
         </body></html>)";
static const char successResponse[] PROGMEM = "<META http-equiv=\"refresh\" "
                                              "content=\"15;URL=/\">Update "
                                              "Success! Rebooting...\n";

ESP8266HTTPUpdateServer::ESP8266HTTPUpdateServer(bool serial_debug) {
  _serial_output = serial_debug;
  _server = NULL;
  _username = NULL;
  _password = NULL;
  _authenticated = false;
  Device.begin();
  ConfigurationPanel.begin(&Device);
}

void ESP8266HTTPUpdateServer::setup(ESP8266WebServer *server, const char *path,
                                    const char *username,
                                    const char *password) {
  _server = server;
  _username = (char *)username;
  _password = (char *)password;

  // handler for the /update form page
  _server->on(path, HTTP_GET, [&]() {
    if (_username != NULL && _password != NULL &&
        !_server->authenticate(_username, _password))
      return _server->requestAuthentication();
#ifdef DEBUG
    Serial << endl
           << "Firmware upgrade. Device is in mode:  " << Device.getMode();
#endif
    if (Device.getMode() == MODE_ACCESS_POINT ||
        Device.getMode() == MODE_CONFIGURATION) {
      _server->send(200, "text/html", ConfigurationPanel.firmwareUpgradeSite());
    } else {
      _server->send(200, "text/html", ConfigurationPanel.getIndexSite(true));
    }
  });

  // handler for the /update form POST (once file upload finishes)
  _server->on(
      path, HTTP_POST,
      [&]() {
        if (!_authenticated)
          return _server->requestAuthentication();
        _server->send(
            200, "text/html",
            ConfigurationPanel.postFirmwareUpgradeSite(Update.hasError()));
        delay(1000);
        _server->client().stop();
        ESP.restart();
      },
      [&]() {
        // handler for the file upload, get's the sketch bytes, and writes
        // them through the Update object
        HTTPUpload &upload = _server->upload();

        if (upload.status == UPLOAD_FILE_START) {
          _updaterError = String();

          _authenticated = (_username == NULL || _password == NULL ||
                            _server->authenticate(_username, _password));
          if (!_authenticated) {
#ifdef DEBUG
            Serial << endl << "Unauthenticated Update";
#endif
            return;
          }

          WiFiUDP::stopAll();
          if (_serial_output)
            Serial.printf("Update: %s\n", upload.filename.c_str());
          uint32_t maxSketchSpace =
              (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace)) { // start with max available size
            _setUpdaterError();
          }
        } else if (_authenticated && upload.status == UPLOAD_FILE_WRITE &&
                   !_updaterError.length()) {
          if (_serial_output)
            Serial.printf(".");
          if (Update.write(upload.buf, upload.currentSize) !=
              upload.currentSize) {
            _setUpdaterError();
          }
        } else if (_authenticated && upload.status == UPLOAD_FILE_END &&
                   !_updaterError.length()) {
          if (Update.end(true)) { // true to set the size to the current
                                  // progress
#ifdef DEBUG
            Serial << endl
                   << "Update Success: %u"
                   << "Rebooting... [" << upload.totalSize << "]";
#endif
          } else {
            _setUpdaterError();
          }
        } else if (_authenticated && upload.status == UPLOAD_FILE_ABORTED) {
          Update.end();
#ifdef DEBUG
          Serial << endl << "Update was aborted";
#endif
        }
        delay(0);
      });
}

void ESP8266HTTPUpdateServer::_setUpdaterError() {
  if (_serial_output)
    Update.printError(Serial);
  StreamString str;
  Update.printError(str);
  _updaterError = str.c_str();
}
