#include "AFE-Hardware.h"

AFEHardware::AFEHardware(AFEFirmware *_Firmware) { Firmware = _Firmware; };

void AFEHardware::begin(void) {}

void AFEHardware::initPriorityHardwareItems(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Starting priortized hardware items");
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  WirePort->initialize();
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  initMCP23XXX();
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  if (Firmware->Device->getMode() != AFE_MODE_ACCESS_POINT) {
    initRGBLED();
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
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

  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
    initADC();
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    initContactron();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    initGate();
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

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initRegulator();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    initThermalProtector();
#endif
  }
}

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEHardware::initSwitch(void) {
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfSwitches; i++) {
    Switch[i] = new AFESwitch();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    Switch[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#ifdef AFE_CONFIG_HARDWARE_LED
    Switch[i]->begin(i, Firmware->API->Flash, Firmware->Hardware->SystemLed);
#else
    Switch[i]->begin(i, Firmware->API->Flash);
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEHardware::initRelay(void) {

#ifdef DEBUG
  Serial << endl << F("INFO: HARDWARE: Initializing Relays");
#endif

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfRelays; i++) {
    Relay[i] = new AFERelay();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    Relay[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

    Relay[i]->begin(Firmware->API->Flash, i);

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
  if (Firmware->Configuration->Pro->valid) {
#ifdef AFE_ESP32
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfAnalogInputs;
         i++) {

#ifdef DEBUG
      AnalogInput[i] = new AFEAnalogInput(Firmware->API->Flash, Firmware->Debugger);
#else
      AnalogInput[i] = new AFEAnalogInput(Firmware->API->Flash);
#endif

      AnalogInput[i]->begin(i);
    }
#else
    if (Firmware->Device->configuration.isAnalogInput) {
#ifdef DEBUG
      AnalogInput =
          new AFEAnalogInput(Firmware->API->Flash, Firmware->Debugger);
#else
      AnalogInput = new AFEAnalogInput(Firmware->API->Flash);
#endif

      AnalogInput->begin();
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEHardware::initRGBLED(void) {
  if (Firmware->Device->configuration.noOfCLEDs > 0) {
    RGBLEDStrip->begin(Firmware->API->Flash, Firmware->Device);
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEHardware::initGate(void) {
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfGates; i++) {
    Gate[i] = new AFEGate();
    Gate[i]->begin(i, Firmware->Device, Firmware->API->Flash);
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
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfContactrons;
       i++) {
    Contactron[i] = new AFEContactron();
    Contactron[i]->begin(i, Firmware->Device, Firmware->API->Flash);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: Contactron: ") << i << F(" initialized");
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEHardware::initTSL2562(void) {
  if (Firmware->Device->configuration.noOfTSL2561s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfTSL2561s; i++) {
#ifdef DEBUG
      TSL2561Sensor[i] = new AFESensorTSL2561(Firmware->API->Flash, WirePort,
                                              Firmware->Debugger);
#else
      TSL2561Sensor[i] = new AFESensorTSL2561(Firmware->API->Flash, WirePort);
#endif
      TSL2561Sensor[i]->begin(i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEHardware::initBH1750(void) {
  if (Firmware->Device->configuration.noOfBH1750s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBH1750s; i++) {
#ifdef DEBUG
      BH1750Sensor[i] = new AFESensorBH1750(Firmware->API->Flash, WirePort,
                                            Firmware->Debugger);
#else
      BH1750Sensor[i] = new AFESensorBH1750(Firmware->API->Flash, WirePort);
#endif

      BH1750Sensor[i]->begin(i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEHardware::initBMX80(void) {
  if (Firmware->Device->configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBMEX80s; i++) {
#ifdef DEBUG
      BMEX80Sensor[i] = new AFESensorBMEX80(Firmware->API->Flash, WirePort,
                                            Firmware->Debugger);
#else
      BMEX80Sensor[i] = new AFESensorBMEX80(Firmware->API->Flash, WirePort);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEHardware::initBinarySensor(void) {
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBinarySensors;
       i++) {
    BinarySensor[i] = new AFESensorBinary();
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    BinarySensor[i]->addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
    BinarySensor[i]->begin(i, Firmware->API->Flash);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Binary sensors initialized");
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEHardware::initDS18B20(void) {
  if (Firmware->Device->configuration.noOfDS18B20s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfDS18B20s; i++) {
      DS18B20Sensor[i] = new AFESensorDS18B20();
#ifdef DEBUG
      DS18B20Sensor[i]->begin(Firmware->API->Flash, Firmware->Debugger, i);
#else
      DS18B20Sensor[i]->begin(Firmware->API->Flash, i);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEHardware::initDHT(void) {
  if (Firmware->Device->configuration.noOfDHTs > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfDHTs; i++) {
      DHTSensor[i] = new AFESensorDHT();
#ifdef DEBUG
      DHTSensor[i]->begin(Firmware->API->Flash, Firmware->Debugger, i);
#else
      DHTSensor[i]->begin(Firmware->API->Flash, i);
#endif
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEHardware::initHPMA115S0(void) {
  if (Firmware->Device->configuration.noOfHPMA115S0s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfHPMA115S0s;
         i++) {
      HPMA115S0Sensor[i] = new AFESensorHPMA115S0();
      HPMA115S0Sensor[i]->begin(i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
void AFEHardware::initFS3000(void) {
  if (Firmware->Device->configuration.noOfFS3000s > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfFS3000s; i++) {
#ifdef DEBUG
      FS3000Sensor[i] = new AFESensorFS3000(Firmware->API->Flash, WirePort,
                                            Firmware->Debugger);
#else
      FS3000Sensor[i] = new AFESensorFS3000(Firmware->API->Flash, WirePort);
#endif
      FS3000Sensor[i]->begin(i);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER

boolean AFEHardware::initRainmeter(void) {
  boolean status = false;
  if (Firmware->Device->configuration.noOfRainmeterSensors > 0) {
#ifdef DEBUG
    status = RainmeterSensor->begin(Firmware->API->Flash, RainImpulse,
                                    Firmware->Debugger);
#else
    status = RainmeterSensor->begin(Firmware->API->Flash, RainImpulse)
#endif

    if (status) {
      pinMode(RainmeterSensor->configuration->gpio, INPUT_PULLUP);
#ifdef DEBUG
      Firmware->Debugger->printInformation(
          F("Rainmeter sensor configured on GPIO: "), F("RAINMETER"));
      Firmware->Debugger->printValue(RainmeterSensor->configuration->gpio);
#endif
    }
  }
  return status;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER

boolean AFEHardware::initAnemometer(void) {
  boolean status = false;
  if (Firmware->Device->configuration.noOfAnemometerSensors > 0) {
#ifdef DEBUG
    status = AnemometerSensor->begin(Firmware->API->Flash, WindImpulse,
                                     Firmware->Debugger);
#else
    status = AnemometerSensor->begin(Firmware->API->Flash, WindImpulse)
#endif

    if (status) {
      pinMode(AnemometerSensor->configuration->gpio, INPUT_PULLUP);
#ifdef DEBUG
      Firmware->Debugger->printInformation(
          F("Rainmeter sensor configured on GPIO: "), F("ANENOMETER"));
      Firmware->Debugger->printValue(AnemometerSensor->configuration->gpio);
#endif
    }
  }
  return status;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
void AFEHardware::initMCP23XXX(void) { MCP23017Broker->begin(); }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEHardware::initRegulator(void) {
  if (Firmware->Device->configuration.noOfRegulators > 0) {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfRegulators;
         i++) {

#ifdef DEBUG
      Regulator[i] = new AFERegulator(Firmware->API->Flash, Firmware->Debugger);
#else
      Regulator[i] = new AFERegulator(Firmware->API->Flash);
#endif

      if (Regulator[i]->configuration->relayId != AFE_HARDWARE_ITEM_NOT_EXIST &&
          Firmware->Device->configuration.noOfRelays >=
              Regulator[i]->configuration->relayId + 1) {
        Regulator[i]->begin(i, Relay[Regulator[i]->configuration->relayId]);
      }
    }
  }
}

#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEHardware::initThermalProtector(void) {
  if (Firmware->Device->configuration.noOfThermalProtectors > 0) {
    for (uint8_t i = 0;
         i < Firmware->Device->configuration.noOfThermalProtectors; i++) {

#ifdef DEBUG
      ThermalProtector[i] =
          new AFEThermalProtector(Firmware->API->Flash, Firmware->Debugger);
#else
      ThermalProtector[i] = new AFEThermalProtector(Firmware->API->Flash);
#endif
      ThermalProtector[i]->begin(i);
    }
  }
}
#endif