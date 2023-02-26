/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Global-Object.h"

AFEGlobalObject::AFEGlobalObject() {
#ifdef DEBUG
  Serial << endl << F("INFO: Global Object: Initialized");
#endif
}

void AFEGlobalObject::begin() {}