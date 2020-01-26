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
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPI::addClass(AFERelay *Relay) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the relays added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPI::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the switches added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPI::addClass(AFEAnalogInput *Analog) {
  _AnalogInput = Analog;
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the ADC added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPI::addClass(AFESensorBMEX80 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    _BMx80Sensor[i] = Sensor + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the BMEX80 added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPI::addClass(AFESensorHPMA115S0 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    _HPMA115S0Sensor[i] = Sensor + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the HPMA115S0 added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPI::addClass(AFESensorBH1750 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    _BH1750Sensor[i] = Sensor + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the BH1750 added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPI::addClass(AFESensorAS3935 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    _AS3935Sensor[i] = Sensor + i;
  }
#ifdef DEBUG
  Serial << endl << "INFO: The reference to the AS3935 added";
#endif
}
#endif // AFE_CONFIG_HARDWARE_AS3935