/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */
#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin() { Pro = Data.getProVersionConfiguration(); }

boolean AFEFirmware::isUnlocked() { return Pro.valid; }

boolean AFEFirmware::callService(uint8_t method) {
  HTTPClient http;

  String url = (method == AFE_WEBSERVICE_VALIDATE_KEY ? AFE_URL_VALIDATE_KEY
                                                      : AFE_URL_ADD_KEY);
  url += Data.getDeviceUID();
  url += "/";
  url += Pro.serial;

#ifdef DEBUG
  Serial << endl
         << endl
         << "------------ "
         << (method == AFE_WEBSERVICE_VALIDATE_KEY ? "Validating" : "Adding")
         << " serial number ------------" << endl
         << "URL: " << url << endl
         << "response: ";
#endif

  http.begin(url);
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
