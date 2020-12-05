/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Firmware-Pro.h"

AFEFirmwarePro::AFEFirmwarePro(){};

void AFEFirmwarePro::begin(AFEWiFi *_Network) {
  Data.getConfiguration(&Pro);
  Network = _Network;
  miliseconds = millis();
}

void AFEFirmwarePro::callService(uint8_t method) {

  Data.getConfiguration(&Pro);

  String _response;
  String url = (method == AFE_WEBSERVICE_VALIDATE_KEY ? AFE_URL_VALIDATE_KEY
                                                      : AFE_URL_ADD_KEY);
  url.concat(Data.getDeviceUID());
  url.concat("/");
  url.concat(Pro.serial);

#ifdef DEBUG
  Serial << endl
         << F("INFO: ")
         << (method == AFE_WEBSERVICE_VALIDATE_KEY ? F("Validating")
                                                   : F("Adding"))
         << F(" serial number") << endl
         << F("INFO: URL: ") << url;
#endif

  uint16_t httpCode = Network->getJSON(url, _response);

#ifdef DEBUG
  Serial << endl << F("INFO: HTTP Response: ") << _response;
#endif
  if (_response.substring(10, 11) == "t" ||
      _response.substring(10, 11) == "f") {
    Pro.valid = _response.substring(10, 11) == "t" ? true : false;
  }
  if (httpCode == 200) {
    Data.saveConfiguration(&Pro);
  }
}

void AFEFirmwarePro::reValidateKey() {

#ifdef DEBUG
  Serial << endl
         << F("INFO: Checking AFE PRO Key after: ") << minutes << F(" minutes");
#endif

  if (strlen(Pro.serial) == 0 && Pro.valid) {
    Pro.valid = false;
#ifdef DEBUG
    Serial << endl << F("INFO: Valid with no Key");
#endif
    Data.saveConfiguration(&Pro);
  } else if (strlen(Pro.serial) > 0) {
    callService(AFE_WEBSERVICE_VALIDATE_KEY);
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
