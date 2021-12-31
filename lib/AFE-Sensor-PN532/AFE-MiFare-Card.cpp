/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-MiFare-Card.h"

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

AFEMiFareCard::AFEMiFareCard(){};

void AFEMiFareCard::begin(uint8_t id, AFEDataAccess *_Data) {
#ifdef DEBUG
  Serial << endl << F("INFO: MiFare Card: ") << id;
#endif
  _Data->getConfiguration(id, &configuration);
}

boolean AFEMiFareCard::listener() {
  boolean _ret = _statusChanged;
  if (_ret) {
    _statusChanged = false;
    _timer = millis();
  }
  if (_timer > 0 && millis() - _timer > configuration.howLongKeepState) {
    state = AFE_HARDWARE_MIFARE_CARD_ACTION_OFF;
    _timer = 0;
    _ret = true;
  }
  return _ret;
}

void AFEMiFareCard::set() {
  state = AFE_HARDWARE_MIFARE_CARD_ACTION_ON;
  _statusChanged = true;
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
