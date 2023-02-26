/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Firmware.h"

AFEFirmware::AFEFirmware(){};

void AFEFirmware::begin(AFEDataAccess *_Data, AFEJSONRPC *_RestAPI) {
  Data = _Data;
  RestAPI = _RestAPI;
  Data->getConfiguration(pro);
  Data->getConfiguration(version);
  miliseconds = millis();
}

void AFEFirmware::validate(void) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: AFE PRO: Checking key after: ") << minutes
         << F(" minutes");
#endif

  if (strlen(pro->serial) == 0 && pro->valid) {
    pro->valid = false;
#ifdef DEBUG
    Serial << endl << F("INFO: AFE PRO: Valid with no key");
#endif
    Data->saveConfiguration(pro);
  } else if (strlen(pro->serial) > 0) {
    String _HtmlResponse;
    boolean isValid;
    _HtmlResponse.reserve(6);
    if (RestAPI->sent(_HtmlResponse, AFE_CONFIG_JSONRPC_REST_METHOD_IS_PRO)) {
      isValid = _HtmlResponse.length() > 0 && _HtmlResponse.equals("true")
                    ? true
                    : false;

      if (pro->valid != isValid) {
        pro->valid = isValid;
        Data->saveConfiguration(pro);
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

void AFEFirmware::listener(void) {
  if (millis() - miliseconds > 59999) {
    minutes++;
    miliseconds = millis();
    if (minutes == AFE_KEY_FREQUENCY_VALIDATION) {
      validate();
      getLatestFirmwareVersion();
      minutes = 0;
    }
  }
}

void AFEFirmware::getLatestFirmwareVersion(void) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: Firmware: Checking latest version after: ") << minutes
         << F(" minutes");
#endif

  String _HtmlResponse;
  _HtmlResponse.reserve(10);
  if (RestAPI->sent(
          _HtmlResponse,
          AFE_CONFIG_JSONRPC_REST_METHOD_GET_LATEST_FIRMWARE_VERSION)) {
    if (_HtmlResponse.length() > 0 &&
        _HtmlResponse.length() <= sizeof(version->installed_version)) {
      char _tempVersion[sizeof(version->installed_version)];
      _HtmlResponse.toCharArray(_tempVersion, sizeof(version->installed_version));
      if (strcmp(_tempVersion, version->installed_version) != 0) {
        Data->saveLatestFirmwareVersion(_tempVersion);
#ifdef DEBUG
        Serial << endl
               << "INFO: FIRMWARE: New version of firmware is available: "
               << _HtmlResponse;
      } else {
        Serial << endl
               << "INFO: FIRMWARE: Device uses the latest version of the "
                  "firmware: "
               << _HtmlResponse;
#endif
      }
    }
  }
}
