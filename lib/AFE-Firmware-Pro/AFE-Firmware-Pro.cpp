/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Firmware-Pro.h"

AFEFirmwarePro::AFEFirmwarePro(){};

void AFEFirmwarePro::begin() {
  Pro = Data.getProVersionConfiguration();
  miliseconds = millis();
}

boolean AFEFirmwarePro::callService(uint8_t method) {
  HTTPClient http;
  WiFiClient client;

  String url = (method == AFE_WEBSERVICE_VALIDATE_KEY ? AFE_URL_VALIDATE_KEY
                                                      : AFE_URL_ADD_KEY);
  url += Data.getDeviceUID();
  url += "/";
  url += Pro.serial;

#ifdef DEBUG
  Serial << endl
         << "INFO: "
         << (method == AFE_WEBSERVICE_VALIDATE_KEY ? "Validating" : "Adding")
         << " serial number" << endl
         << "INFO: URL: " << url << endl
         << "INFO: Response: ";
#endif

  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    StaticJsonBuffer<AFE_RESPONSE_KEY_VALIDATION> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(http.getString());
    if (root.success()) {

#ifdef DEBUG
    Serial << "success" << endl << "INFO: JSON: ";
#endif

#ifdef DEBUG
      root.printTo(Serial);
#endif

      Pro.valid = root["status"];
#ifdef DEBUG
      Serial << endl
             << "INFO: JSON: Buffer size: " << AFE_RESPONSE_KEY_VALIDATION
             << ", actual JSON size: " << jsonBuffer.size();
      if (AFE_RESPONSE_KEY_VALIDATION < jsonBuffer.size() + 10) {
        Serial << endl << "WARN: Too small buffer size";
      }
#endif

      Data.saveConfiguration(Pro);
    }

#ifdef DEBUG
    else {
      Serial << "ERROR: JSON not pharsed";
    }
#endif
  }
#ifdef DEBUG
  else {
     
    Serial << endl << "ERROR: HTTP [" << httpCode << "] not opended";
  }
#endif
  http.end();

  return Pro.valid;
}

void AFEFirmwarePro::reValidateKey() {

#ifdef DEBUG
  Serial << endl
         << "INFO: Checking AFE PRO Key after: " << minutes << " minutes";
#endif

  if (strlen(Pro.serial) == 0 && Pro.valid) {
    Pro.valid = false;
#ifdef DEBUG
    Serial << endl << "INFO: Valid with no Key";
#endif
    Data.saveConfiguration(Pro);
  } else if (strlen(Pro.serial) > 0) {
    boolean _valid = callService(AFE_WEBSERVICE_VALIDATE_KEY);
    if (_valid != Pro.valid) {
#ifdef DEBUG
      Serial << endl
             << "INFO: Key status is not up2date. Saving it state: Valid: " << endl
             << (_valid ? "YES" : "NO");
#endif
      Pro.valid = _valid;
      Data.saveConfiguration(Pro);
    }
  }
}

void AFEFirmwarePro::listener() {
  if (millis() - miliseconds > 59999) {
    minutes++;
    miliseconds = millis();
    if (minutes == AFE_KEY_FREQUENCY_VALIDATION) {
      reValidateKey();
      minutes = 0;
    }
  }
}
