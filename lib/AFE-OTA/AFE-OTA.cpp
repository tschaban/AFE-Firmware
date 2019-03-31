#include "AFE-OTA.h"

static const char serverIndex[] PROGMEM =
    R"(<html><body><form method='POST' action='' enctype='multipart/form-data'>
                  <input type='file' name='update'>
                  <input type='submit' value='Update'>
               </form>
         </body></html>)";

ESP8266HTTPUpdateServer::ESP8266HTTPUpdateServer() { _server = NULL; }

void ESP8266HTTPUpdateServer::setup(ESP8266WebServer *server) {
  _server = server;

  // handler for the /update form page
  _server->on("/update", HTTP_GET, [&]() {
    Device.begin();
    ConfigurationPanel.begin(&Device);

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
      "/update", HTTP_POST,
      [&]() {
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

          WiFiUDP::stopAll();
#ifdef DEBUG
          Serial << endl << "Firmware upgrade: " << upload.filename << endl;
#endif
          uint32_t maxSketchSpace =
              (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace)) { // start with max available size
            _setUpdaterError();
          }
        } else if (upload.status == UPLOAD_FILE_WRITE &&
                   !_updaterError.length()) {
#ifdef DEBUG
          Serial.printf(".");
#endif
          if (Update.write(upload.buf, upload.currentSize) !=
              upload.currentSize) {
            _setUpdaterError();
          }
        } else if (upload.status == UPLOAD_FILE_END &&
                   !_updaterError.length()) {
          if (Update.end(true)) { // true to set the size to the current
                                  // progress
#ifdef DEBUG
            Serial << endl
                   << "Update Success. Firmware size: " << upload.totalSize
                   << endl
                   << "Rebooting..." << endl
                   << endl;
#endif
          } else {
            _setUpdaterError();
          }
        } else if (upload.status == UPLOAD_FILE_ABORTED) {
          Update.end();
#ifdef DEBUG
          Serial << endl << "Update was aborted";
#endif
        }
        delay(0);
      });
}

void ESP8266HTTPUpdateServer::_setUpdaterError() {
#ifdef DEBUG
  Update.printError(Serial);
#endif
  StreamString str;
  Update.printError(str);
  _updaterError = str.c_str();
}
