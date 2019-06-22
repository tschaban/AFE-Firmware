/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */
#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin() {
  Pro = Data.getProVersionConfiguration();
  miliseconds = millis();
}

boolean AFEFirmware::callService(uint8_t method) {
  HTTPClient http;
  WiFiClient client;

  String url = (method == AFE_WEBSERVICE_VALIDATE_KEY ? AFE_URL_VALIDATE_KEY
                                                      : AFE_URL_ADD_KEY);
  url += Data.getDeviceUID();
  url += "/";
  url += Pro.serial;

#ifdef DEBUG
  Serial << endl
         << endl
         << (method == AFE_WEBSERVICE_VALIDATE_KEY ? "Validating" : "Adding")
         << " serial number" << endl
         << "URL: " << url << endl
         << "response: ";
#endif

  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    /* ArduinoJson v5 */
    StaticJsonBuffer<135> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(http.getString());

    /* ArduinoJson v6
    StaticJsonDocument<135> root;
    DeserializationError error = deserializeJson(root, http.getString());
*/
    /* ArduinoJson v5*/
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      /* ArduinoJson v6
      if (!error) {
  #ifdef DEBUG
        serializeJson(root, Serial);
  #endif
  */
      Pro.valid = root["status"];

#ifdef DEBUG
      /* ArduinoJson v5 */
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << jsonBuffer.size();

      /* ArduinoJson v6
      Serial << endl
             << "success" << endl
             << "JSON Buffer size: " << root.size();*/

#endif

      Data.saveConfiguration(Pro);
    }

#ifdef DEBUG
    else {
      Serial << "failure parshing JSON";
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << "failure: opening HTTP [" << httpCode << "]";
  }
#endif
  http.end();

  return Pro.valid;
}

void AFEFirmware::reValidateKey() {

#ifdef DEBUG
  Serial << endl
         << endl
         << "-------------------- CHECKING KEY -----------------------" << endl
         << "After: " << minutes << " minutes";
#endif

  if (strlen(Pro.serial) == 0 && Pro.valid) {
    Pro.valid = false;
#ifdef DEBUG
    Serial << endl << "WARN: Valid with no Key";
#endif
    Data.saveConfiguration(Pro);
  } else if (strlen(Pro.serial) > 0) {
    boolean _valid = callService(AFE_WEBSERVICE_VALIDATE_KEY);
    if (_valid != Pro.valid) {
#ifdef DEBUG
      Serial << endl
             << "Key status is not up2date. Saving it state: Valid: " << endl
             << (_valid ? "YES" : "NO");
#endif
      Pro.valid = _valid;
      Data.saveConfiguration(Pro);
    }
  }

#ifdef DEBUG
  Serial << endl << "---------------------------------------------------------";
#endif
}

void AFEFirmware::listener() {
  if (millis() - miliseconds > 59999) {
    minutes++;
    miliseconds = millis();
    if (minutes == AFE_KEY_FREQUENCY_VALIDATION) {
      reValidateKey();
      minutes = 0;
    }
  }
}
