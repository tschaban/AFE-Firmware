/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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
    boolean isValid;
    if (RestAPI->sent(isValid, "is-pro") ==
        AFE_CONFIG_JSONRPC_REST_OK_RESPONSE) {

      if (Pro.valid!=isValid) {
        Data->saveConfiguration(&Pro);
        Pro.valid = isValid;
#ifdef DEBUG
        Serial << endl << F("INFO: AFE PRO: Key state has been changed");
#endif
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: AFE PRO: Key checked: ") << (isValid
          ? F("valid")
          : F("invalid"));
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("ERROR: AFE PRO: while checing the key");
#endif
    }
  }
}

void AFEFirmwarePro::listener() {
  if (millis() - miliseconds > 10000) {
    
    Serial << endl << F("INFO: #### Time:" ) << millis();
    validate();

      Serial << endl
         << "INFO: MEMORY: After key check: : "
         << String(system_get_free_heap_size() / 1024) << "kB";

    // minutes++;
    miliseconds = millis();
    // if (minutes == AFE_KEY_FREQUENCY_VALIDATION) {
    //  validate();
    //  minutes = 0;
    // }
  }
}
