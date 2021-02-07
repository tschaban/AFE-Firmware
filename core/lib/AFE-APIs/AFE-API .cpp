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
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the relay: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPI::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the switch: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPI::addClass(AFEAnalogInput *Analog) {
  if (_Device->configuration.isAnalogInput) {
    _AnalogInput = Analog;
#ifdef DEBUG
    Serial << endl << F("INFO: The reference to the ADC added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPI::addClass(AFESensorBMEX80 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    _BMx80Sensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the BMEX80: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPI::addClass(AFESensorHPMA115S0 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    _HPMA115S0Sensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the HPMA115S0 added: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPI::addClass(AFESensorBH1750 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    _BH1750Sensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the BH1750 added: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPI::addClass(AFESensorAS3935 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    _AS3935Sensor[i] = Sensor + i;
  }
#ifdef DEBUG
  Serial << endl
         << F("INFO: The reference to the AS3935 added: ") << i + 1
         << F(" added");
#endif
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPI::addClass(AFEAnemometer *AnemometerSensor) {
  if (_Device->configuration.noOfAnemometerSensors > 0) {
    _AnemometerSensor = AnemometerSensor;
#ifdef DEBUG
    Serial << endl << F("INFO: The reference to the Anemometer added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPI::addClass(AFERainmeter *RainmeterSensor) {
  _RainmeterSensor = RainmeterSensor;
#ifdef DEBUG
  Serial << endl << F("INFO: The reference to the Rain added");
#endif
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPI::addClass(AFEGate *Item) {
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    _Gate[i] = Item + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the Gate: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPI::addClass(AFEContactron *Item) {
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    _Contactron[i] = Item + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the Contactron: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPI::addClass(AFESensorDS18B20 *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfDS18B20s; i++) {
    _DS18B20Sensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the DS18B20: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPI::addClass(AFERegulator *Regulator) {
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    _Regulator[i] = Regulator + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the Regulator: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPI::addClass(AFEThermalProtector *Protector) {
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    _ThermalProtector[i] = Protector + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the Thermal Protector: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPI::addClass(AFESensorDHT *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfDHTs; i++) {
    _DHTSensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the DHT: ") << i + 1 << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPI::addClass(AFEImpulseCatcher *Sensor) {
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    _BinarySensor[i] = Sensor + i;
#ifdef DEBUG
    Serial << endl
           << F("INFO: The reference to the Binary sensor: ") << i + 1
           << F(" added");
#endif
  }
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR