#include "AFE-Hardware.h"

AFEHardware::AFEHardware(AFEFirmware *Firmware) { _Firmware = Firmware; };

void AFEHardware::begin(void) {}

void AFEHardware::initPriorityHardwareItems(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Starting priortized hardware items");
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  if (_Firmware->Device->getMode() != AFE_MODE_ACCESS_POINT) {
    initRGBLED();
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL) {
    initRelay();
  }
#endif // AFE_CONFIG_HARDWARE_RELAY
}

void AFEHardware::initHardwareItems(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Starting hardware");
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  initSwitch();
#endif

  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL) {
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
    initADC();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    initGate();
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    initContactron();
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    initTSL2562();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    initBH1750();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    initBMX80();
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    initBinarySensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    initDS18B20();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    initDHT();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    initHPMA115S0();
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
    initFS3000();
#endif
  }
}

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEHardware::initSwitch(void) {
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches; i++) {
    Switch[i] = new AFESwitch();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    Switch[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#ifdef AFE_CONFIG_HARDWARE_LED
    Switch[i]->begin(i, _Firmware->API->Flash, _Firmware->Hardware->SystemLed);
#else
    Switch[i]->begin(i, _Firmware->API->Flash);
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEHardware::initRelay(void) {

#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Initializing Relays");
#endif

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++) {
    Relay[i] = new AFERelay();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    Relay[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    Relay[i]->begin(_Firmware->API->Flash, i);

/* Setting relay state after restoring power is not required when the relay is
 * assigned to a gate*/
#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Relay[i]->gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      Relay[i]->setRelayAfterRestoringPower();

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
    } else {
      Serial << endl
             << F("INFO: RELAY: Restoring relay state is not required for a "
                  "relay assigned to a gate");
#endif
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEHardware::initADC(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Initializing ADC");
#endif
  if (_Firmware->Configuration->Pro->valid) {
#ifdef AFE_ESP32
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAnalogInputs;
         i++) {
      AnalogInput[i] = new AFEAnalogInput();
      AnalogInput[i]->begin(i);
    }
#else
    if (_Firmware->Device->configuration.isAnalogInput) {
      AnalogInput = new AFEAnalogInput();
      AnalogInput->begin();
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEHardware::initRGBLED(void) {
  if (_Firmware->Device->configuration.noOfCLEDs > 0) {
    RGBLEDStrip->begin(_Firmware->API->Flash, _Firmware->Device);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEHardware::initGate(void) {
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++) {
    Gate[i] = new AFEGate();
    Gate[i]->begin(i, _Firmware->Device, _Firmware->API->Flash);
    GatesCurrentStates->state[i] = AFE_GATE_UNKNOWN;

    /* Assigning Gate ID to a contactron */
    for (uint8_t j = 0; j < Gate[i]->getNoOfContactrons(); j++) {
      Contactron[Gate[i]->getContactronId(j)]->gateId = i;
#ifdef DEBUG
      Serial << endl
             << F("INFO: GATE: For a ContactronID: ")
             << Gate[i]->getContactronId(j) << F(" setting GateID: ") << i;
#endif
    }

#ifdef DEBUG
    Serial << endl << F("INFO: GATE: ") << i << F(" initialized");
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEHardware::initContactron(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing contractons");
#endif
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
       i++) {
    Contactron[i] = new AFEContactron();
    Contactron[i]->begin(i, _Firmware->Device, _Firmware->API->Flash);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: Contactron: ") << i << F(" initialized");
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEHardware::initTSL2562(void) {
  if (_Firmware->Device->configuration.noOfTSL2561s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfTSL2561s;
         i++) {
      TSL2561Sensor[i] = new AFESensorTSL2561();
#ifdef AFE_ESP32
      TSL2561Sensor[i]->begin(i, _Firmware->Hardware->WirePort0,
                              _Firmware->Hardware->WirePort1);
#else
      TSL2561Sensor[i]->begin(i, _Firmware->Hardware->WirePort0);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEHardware::initBH1750(void) {
  if (_Firmware->Device->configuration.noOfBH1750s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBH1750s; i++) {
      BH1750Sensor[i] = new AFESensorBH1750();
#ifdef AFE_ESP32
      BH1750Sensor[i]->begin(i, _Firmware->Hardware->WirePort0,
                             _Firmware->Hardware->WirePort1);
#else
      BH1750Sensor[i]->begin(i, _Firmware->Hardware->WirePort0);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEHardware::initBMX80(void) {
  if (_Firmware->Device->configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i] = new AFESensorBMEX80();
#ifdef AFE_ESP32
      BMEX80Sensor[i]->begin(i, _Firmware->Hardware->WirePort0,
                             _Firmware->Hardware->WirePort1);
#else
      BMEX80Sensor[i]->begin(i, _Firmware->Hardware->WirePort0);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEHardware::initBinarySensor(void) {
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
       i++) {
    BinarySensor[i] = new AFESensorBinary();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    BinarySensor[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
    BinarySensor[i]->begin(i, _Firmware->API->Flash);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Binary sensors initialized");
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEHardware::initDS18B20(void) {
  if (_Firmware->Device->configuration.noOfDS18B20s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDS18B20s;
         i++) {
      DS18B20Sensor[i] = new AFESensorDS18B20();
      DS18B20Sensor[i]->begin(_Firmware->API->Flash, i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEHardware::initDHT(void) {
  if (_Firmware->Device->configuration.noOfDHTs > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDHTs; i++) {
      DHTSensor[i] = new AFESensorDHT();
      DHTSensor[i]->begin(_Firmware->API->Flash, i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEHardware::initHPMA115S0(void) {
  if (_Firmware->Device->configuration.noOfHPMA115S0s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfHPMA115S0s;
         i++) {
      HPMA115S0Sensor[i] = new AFESensorHPMA115S0();
      HPMA115S0Sensor[i]->begin(i);
    }
  }
}
#endif


#ifdef AFE_CONFIG_HARDWARE_FS3000
void AFEHardware::initFS3000(void) {
  if (_Firmware->Device->configuration.noOfFS3000s > 0) {
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfFS3000s; i++) {
      FS3000Sensor[i] = new AFESensorFS3000();
      FS3000Sensor[i]->begin(i, _Firmware->API->Flash);
    }
  }
}
#endif
