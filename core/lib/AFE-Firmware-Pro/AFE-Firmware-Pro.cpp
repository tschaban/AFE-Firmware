/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Firmware-Pro.h"

AFEFirmwarePro::AFEFirmwarePro(){};

void AFEFirmwarePro::begin(AFEDataAccess *_Data, AFEJSONRPC *_RestAPI) {
  Data = _Data;
  RestAPI = _RestAPI;
  Data->getConfiguration(&Pro);
  miliseconds = millis();
}

void AFEFirmwarePro::validate() {

#ifdef DEBUG
  Serial << endl
         << F("INFO: AFE PRO: Checking key after: ") << minutes
         << F(" minutes");
#endif

  if (strlen(Pro.serial) == 0 && Pro.valid) {
    Pro.valid = false;
#ifdef DEBUG
    Serial << endl << F("INFO: AFE PRO: Valid with no key");
#endif
    Data->saveConfiguration(&Pro);
  } else if (strlen(Pro.serial) > 0) {
    String _HtmlResponse;
    boolean isValid;
    _HtmlResponse.reserve(6);
    if (RestAPI->sent(_HtmlResponse, AFE_CONFIG_JSONRPC_REST_METHOD_IS_PRO)) {
      isValid = _HtmlResponse.length() > 0 && _HtmlResponse.equals("true")
                    ? true
                    : false;

      if (Pro.valid != isValid) {
        Pro.valid = isValid;
        Data->saveConfiguration(&Pro);
#ifdef DEBUG
        Serial << endl << F("INFO: AFE PRO: Key state has been changed");
#endif
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: AFE PRO: Key checked: ")
             << (isValid ? F("valid") : F("invalid"));
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("ERROR: AFE PRO: while checing the key");
    }
#endif
  }
}

void AFEFirmwarePro::listener() {
  if (millis() - miliseconds > 59999) {
    minutes++;
    miliseconds = millis();
    if (minutes == AFE_KEY_FREQUENCY_VALIDATION) {
      validate();
      minutes = 0;
    }
  }
}
