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
    StaticJsonDocument<135> doc;
    DeserializationError error = deserializeJson(doc, http.getString());

    if (!error) {
#ifdef DEBUG
      serializeJson(doc, Serial);
#endif
      Pro.valid = doc["status"].as<boolean>();

#ifdef DEBUG
      Serial << endl << "success" << endl << "JSON Buffer size: " << doc.size();
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
