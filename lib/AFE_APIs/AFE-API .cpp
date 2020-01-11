/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API.h"

AFEAPI::AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED

void AFEAPI::begin(AFEDataAccess *Data, AFEDevice *Device, AFELED *Led) {
  _Data = Data;
  _Device = Device;
  _Led = Led;
  if (_Device->configuration.api.mqtt) {
    Mqtt.begin(Data, Device->configuration.name, Led);
  }
  enabled = true;
}

#else

void AFEAPI::begin(AFEDataAccess *Data, AFEDevice *Device) {
  _Data = Data;
  _Device = Device;
  if (_Device->configuration.api.mqtt) {
    Mqtt.begin(Data, Device->configuration.name);
  }
  enabled = true;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPI::addClass(AFERelay *Relay) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the relays added";
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPI::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the switches added";
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPI::addClass(AFEAnalogInput *Analog) {
  _AnalogInput = Analog;
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the ADC added";
#endif
}
#endif