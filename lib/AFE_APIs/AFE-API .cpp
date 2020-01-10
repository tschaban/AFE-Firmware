/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API.h"

AFEAPI::AFEAPI() {};

void AFEAPI::begin(AFEDataAccess *Data, AFEDevice *Device) {
  _Data = Data;
  _Device = Device;
  Mqtt.begin(_Data, _Device->configuration.name);
  enabled = true;
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPI::addClass(AFERelay *Relay) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
  }
#ifdef DEBUG
  Serial << endl << " - reference to the relays added";
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPI::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
  }
#ifdef DEBUG
  Serial << endl << " - reference to the switches added";
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPI::addClass(AFEAnalogInput *Analog) {
  _AnalogInput = Analog;
#ifdef DEBUG
  Serial << endl << " - reference to the ADC added";
#endif
}
#endif