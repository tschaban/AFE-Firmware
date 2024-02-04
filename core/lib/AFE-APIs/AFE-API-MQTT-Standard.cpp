
/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Standard.h"

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ

AFEAPIMQTTStandard::AFEAPIMQTTStandard(){};

void AFEAPIMQTTStandard::begin(AFEFirmware *Firmware, AFEHardware *Hardware) {

  _Firmware = Firmware;
  _Hardware = Hardware;

#ifdef DEBUG
  Mqtt->addReference(_Firmware->Debugger);
#endif

  if (_Firmware->Device->configuration.api.mqtt) {
#ifdef AFE_CONFIG_HARDWARE_LED
    enabled = Mqtt->begin(_Firmware->API->Flash, _Firmware->Device,
                          _Hardware->SystemLed);
#else
    enabled = Mqtt->begin(_Firmware->API->Flash, _Firmware->Device);
#endif
  }
}

void AFEAPIMQTTStandard::synchronize() {

#ifdef DEBUG
  _Firmware->Debugger->printInformation(
      F("Sending current device state to MQTT Broker"), F("MQTT"));
#endif

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++) {
    if (strlen(_Hardware->Relay[i]->configuration->mqtt.topic) > 0) {
      sprintf(mqttStateTopic, "%s/state",
              _Hardware->Relay[i]->configuration->mqtt.topic);
      if (!_Hardware->Relay[i]->setRelayAfterRestoringMQTTConnection()) {
        /* Requesting state from MQTT Broker / service */
        Mqtt->publish(mqttStateTopic, "get");
      } else {
        /* Updating relay state after setting default value after MQTT connected
         */
        publishRelayState(i);
      }
    }
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches; i++) {
    publishSwitchState(i);
  }
#endif

/* Synchronize: Binary sensor */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
       i++) {
    publishBinarySensorState(i);
  }
#endif

/* Synchronize: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++) {
    publishGateState(i);
  }
#endif

/* Synchronize: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
       i++) {
    publishContactronState(i);
  }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
       i++) {
    publishRegulatorState(i);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0;
       i < _Firmware->Device->configuration.noOfThermalProtectors; i++) {
    publishThermalProtectorState(i);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {
    publishCLEDState(i);
    publishCLEDEffectsState(i);
  }
#endif // AFE_CONFIG_HARDWARE_CLED
}

void AFEAPIMQTTStandard::subscribe() {

#ifdef DEBUG
  _Firmware->Debugger->printHeader(1, 1, 50, AFE_DEBUG_HEADER_TYPE_DASH);
  _Firmware->Debugger->printValue(F("Subscribing to MQTT Topics"));

#endif

/* Subscribe: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++) {
    subscribeToCommand(_Hardware->Relay[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches; i++) {
    subscribeToCommand(_Hardware->Switch[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: Binary sensors */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
       i++) {
    subscribeToCommand(_Hardware->BinarySensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: ADC */
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  if (_Firmware->Configuration->Pro->valid) {
#ifdef AFE_ESP32
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAnalogInputs;
         i++) {
      subscribeToCommand(_Hardware->AnalogInput[i]->configuration->mqtt.topic);
    }
#else
    if (_Firmware->Device->configuration.isAnalogInput) {
      subscribeToCommand(_Hardware->AnalogInput->configuration->mqtt.topic);
    }
#endif // AFE_ESP32
  }
#endif

/* Subscribe: BMx80 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBMEX80s; i++) {
    subscribeToCommand(_Hardware->BMEX80Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: BH1750 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBH1750s; i++) {
    subscribeToCommand(_Hardware->BH1750Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: TSL2561 */
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfTSL2561s; i++) {
    subscribeToCommand(_Hardware->TSL2561Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: FS3000 */
#ifdef AFE_CONFIG_HARDWARE_FS3000
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfFS3000s; i++) {
    subscribeToCommand(_Hardware->FS3000Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: AS3935 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAS3935s; i++) {
    subscribeToCommand(_AS3935Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: HPMA115S0 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfHPMA115S0s;
       i++) {
    subscribeToCommand(
        _Hardware->HPMA115S0Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: ANEMOMETER */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  if (_Firmware->Device->configuration.noOfAnemometerSensors > 0) {
    subscribeToCommand(_Hardware->AnemometerSensor->configuration->mqtt.topic);
  }
#endif

/* Subscribe: RAIN */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  if (_Firmware->Device->configuration.noOfRainmeterSensors > 0) {
    subscribeToCommand(_Hardware->RainmeterSensor->configuration->mqtt.topic);
  }
#endif

/* Subscribe: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
       i++) {
    subscribeToCommand(_Hardware->Contactron[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++) {
    subscribeToCommand(_Hardware->Gate[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDS18B20s; i++) {
    subscribeToCommand(_Hardware->DS18B20Sensor[i]->configuration->mqtt.topic);
  }
#endif

/* Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
       i++) {
    subscribeToCommand(_Hardware->Regulator[i]->configuration->mqtt.topic);
  }
#endif

/* Thermal protector */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0;
       i < _Firmware->Device->configuration.noOfThermalProtectors; i++) {
    subscribeToCommand(
        _Hardware->ThermalProtector[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDHTs; i++) {
    subscribeToCommand(_Hardware->DHTSensor[i]->configuration->mqtt.topic);
  }
#endif

/* Subscribe: CLED */
#ifdef AFE_CONFIG_HARDWARE_CLED

  char _brightnessTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {

    subscribeToCommand(_Hardware->RGBLEDStrip->configuration[i].cled.topic);

    sprintf(_brightnessTopic, "%s%s",
            _Hardware->RGBLEDStrip->configuration[i].cled.topic,
            AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);

    subscribeToCommand(_brightnessTopic);

    subscribeToCommand(_Hardware->RGBLEDStrip->configuration[i].effect.topic);
  }

#endif

#ifdef DEBUG
  _Firmware->Debugger->printHeader(1, 0, 50, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
}

void AFEAPIMQTTStandard::listener() {
  if (Mqtt->listener()) {
#ifdef DEBUG

    _Firmware->Debugger->printHeader(1, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
    _Firmware->Debugger->printValue(F("MQTT message recieved"));

    _Firmware->Debugger->printBulletPoint(F("Topic: "));
    _Firmware->Debugger->printValue(Mqtt->message.topic);
    _Firmware->Debugger->printBulletPoint(F("Content: "));
    _Firmware->Debugger->printValue(Mqtt->message.content, 1);
    _Firmware->Debugger->printValue(F("Checking incoming message:"));
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Relay: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++) {
      if (strlen(_Hardware->Relay[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->Relay[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processRelay(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Switch: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches;
         i++) {
      if (strlen(_Hardware->Switch[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->Switch[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processSwitch(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Binary sensor: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
         i++) {
      if (strlen(_Hardware->BinarySensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->BinarySensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processBinarySensor(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("ADC: "));
#endif

#ifdef AFE_ESP32
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfAnalogInputs;
         i++) {
      if (strlen(_Hardware->AnalogInput[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->AnalogInput[i]->configuration->mqtt.topic);
#else // ESP82xx
    if (_Firmware->Device->configuration.isAnalogInput) {
      if (strlen(_Hardware->AnalogInput->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->AnalogInput->configuration->mqtt.topic);
#endif

        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
#ifdef AFE_ESP32
          processADC(&i);
#else // ESP82xx
          processADC();
#endif
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("DS18B20: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDS18B20s;
         i++) {
      if (strlen(_Hardware->DS18B20Sensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->DS18B20Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processDS18B20(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_CLED
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("RGB Led: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {
      if (strlen(_Hardware->RGBLEDStrip->configuration[i].cled.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->RGBLEDStrip->configuration[i].cled.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processCLED(&i);
          return;
        }

        sprintf(mqttCommandTopic, "%s/brightness/cmd",
                _Hardware->RGBLEDStrip->configuration[i].cled.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processCLEDBrigtness(&i);
          return;
        }
      }

      if (strlen(_Hardware->RGBLEDStrip->configuration[i].effect.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->RGBLEDStrip->configuration[i].effect.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processCLEDEffect(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif

#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_BH1750
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("BH1750: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBH1750s; i++) {
      if (strlen(_Hardware->BH1750Sensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->BH1750Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processBH1750(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Bosch BMx: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBMEX80s; i++) {
      if (strlen(_Hardware->BMEX80Sensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->BMEX80Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processBMEX80(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Anenometer: "));
#endif
    if (_Firmware->Device->configuration.noOfAnemometerSensors > 0) {
      if (strlen(_Hardware->AnemometerSensor->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->AnemometerSensor->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processAnemometerSensor();
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Rainmeter: "));
#endif
    if (_Firmware->Device->configuration.noOfRainmeterSensors > 1) {
      if (strlen(_Hardware->RainmeterSensor->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->RainmeterSensor->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processRainSensor();
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("HPMA115S0: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfHPMA115S0s;
         i++) {
      if (strlen(_Hardware->HPMA115S0Sensor[i]->configuration->mqtt.topic) >
          0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->HPMA115S0Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processHPMA115S0(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Regulator: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
         i++) {
      if (strlen(_Hardware->Regulator[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->Regulator[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processRegulator(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Thermal Protector: "));
#endif
    for (uint8_t i = 0;
         i < _Firmware->Device->configuration.noOfThermalProtectors; i++) {
      if (strlen(_Hardware->ThermalProtector[i]->configuration->mqtt.topic) >
          0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->ThermalProtector[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processThermalProtector(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("DHTxx: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfDHTs; i++) {
      if (strlen(_Hardware->DHTSensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->DHTSensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processDHT(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("TLS2561: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfTSL2561s;
         i++) {
      if (strlen(_Hardware->TSL2561Sensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->TSL2561Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processTSL2561(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_FS3000
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("FS3000: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfFS3000s; i++) {
      if (strlen(_Hardware->FS3000Sensor[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->FS3000Sensor[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processFS3000(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_FS3000

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Gate: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++) {
      if (strlen(_Hardware->Gate[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->Gate[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processGate(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("Contactron: "));
#endif
    for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
         i++) {
      if (strlen(_Hardware->Contactron[i]->configuration->mqtt.topic) > 0) {
        sprintf(mqttCommandTopic, "%s/cmd",
                _Hardware->Contactron[i]->configuration->mqtt.topic);
        if (strcmp(Mqtt->message.topic, mqttCommandTopic) == 0) {
#ifdef DEBUG
          printYesNo(true);
#endif
          processContactron(&i);
          return;
        }
      }
    }
#ifdef DEBUG
    printYesNo(false);
#endif
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(
        F("Warning: Topic is NOT defined in this device"));
    _Firmware->Debugger->printHeader(1, 0, 40, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
  }
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTStandard::publishRelayState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Hardware->Relay[id]->configuration->mqtt.topic,
                       _Hardware->Relay[id]->get() == AFE_RELAY_ON ? AFE_ON
                                                                   : AFE_OFF)
                 : false;
}

void AFEAPIMQTTStandard::processRelay(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Relay"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _Hardware->Relay[*id]->on();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _Hardware->Relay[*id]->off();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _Hardware->Relay[*id]->toggle();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG

    printCommandNotImpementedWarning();
#endif
  }
  if (publishState) {
    publishRelayState(*id);
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTStandard::processSwitch(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Switch"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishSwitchState(*id);
  }
#ifdef DEBUG
  else {

    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishSwitchState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Hardware->Switch[id]->configuration->mqtt.topic,
                       _Hardware->Switch[id]->getPhisicalState() ? AFE_OPEN
                                                                 : AFE_CLOSED,
                       true)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
void AFEAPIMQTTStandard::publishADCValues(uint8_t id) {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _Hardware->AnalogInput[id]->getJSON(message);
    Mqtt->publish(_Hardware->AnalogInput[id]->configuration->mqtt.topic,
                  message);
  }
}
void AFEAPIMQTTStandard::processADC(uint8_t *id) {
#ifdef DEBUG

  _Firmware->Debugger->printProcessingRequest(F("ADC"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishADCValues(*id);
  }
#ifdef DEBUG
  else {

    printCommandNotImpementedWarning();
  }
#endif
}
#else // AFE_ESP8266
void AFEAPIMQTTStandard::publishADCValues() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _Hardware->AnalogInput->getJSON(message);
    Mqtt->publish(_Hardware->AnalogInput->configuration->mqtt.topic, message);
  }
}
void AFEAPIMQTTStandard::processADC() {
#ifdef DEBUG

  _Firmware->Debugger->printProcessingRequest(F("ADC"), AFE_NONE, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishADCValues();
  }
#ifdef DEBUG
  else {

    printCommandNotImpementedWarning();
  }
#endif
}
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
void AFEAPIMQTTStandard::processBatteryMeter(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("BatteryMeter"), *id,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBatteryMeterValues(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBatteryMeterValues(uint8_t id) {
  boolean _ret = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _Hardware->AnalogInput[id]->getBatteryMeterJSON(message);
    _ret = Mqtt->publish(
        _Hardware->AnalogInput[id]->configuration->battery.mqtt.topic, message);
  }
  return _ret;
}

#else // AFE_ESP8266
void AFEAPIMQTTStandard::processBatteryMeter() {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("BatteryMeter"), AFE_NONE,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBatteryMeterValues();
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBatteryMeterValues() {
  boolean _ret = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _Hardware->AnalogInput->getBatteryMeterJSON(message);
    _ret = Mqtt->publish(
        _Hardware->AnalogInput->configuration->battery.mqtt.topic, message);
  }
  return _ret;
}
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIMQTTStandard::processBMEX80(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Bosch BMx"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBoschBMSensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishBoschBMSensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    /**
     * @brief Real JSON string size used. Workaround as ASyncMQTTCrashes with
     * larger string crashes @TODO T6 investigate the problem furhter
     *
     */
    char message[AFE_CONFIG_API_JSON_BMEX80_DATA_REAL_LENGTH];
    _Hardware->BMEX80Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->BMEX80Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIMQTTStandard::processHPMA115S0(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("HPMA115S0"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishHPMA115S0SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishHPMA115S0SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_HPMA115S0_DATA_LENGTH];
    _Hardware->HPMA115S0Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->HPMA115S0Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIMQTTStandard::processBH1750(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("BH1750"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBH1750SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBH1750SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BH1750_DATA_LENGTH];
    _Hardware->BH1750Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->BH1750Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEAPIMQTTStandard::processTSL2561(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("TSL2561"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishTSL2561SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishTSL2561SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_TSL2561_DATA_LENGTH];
    _Hardware->TSL2561Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->TSL2561Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_FS3000
void AFEAPIMQTTStandard::processFS3000(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("FS3000"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishFS3000SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishFS3000SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_FS3000_DATA_LENGTH];
    _Hardware->FS3000Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->FS3000Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_FS3000

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIMQTTStandard::processAS3935(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("AS3935"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishAS3935SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
boolean AFEAPIMQTTStandard::publishAS3935SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_AS3935_DATA_LENGTH];
    _AS3935Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt->publish(_AS3935Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIMQTTStandard::publishAnemometerSensorData() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ANEMOMETER_DATA_LENGTH];
    _Hardware->AnemometerSensor->getJSON(message);
    Mqtt->publish(_Hardware->AnemometerSensor->configuration->mqtt.topic,
                  message);
  }
}

void AFEAPIMQTTStandard::processAnemometerSensor() {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Anemometer"), AFE_NONE,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishAnemometerSensorData();
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIMQTTStandard::publishRainSensorData() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_RAINMETER_DATA_LENGTH];
    _Hardware->RainmeterSensor->getJSON(message);
    Mqtt->publish(_Hardware->RainmeterSensor->configuration->mqtt.topic,
                  message);
  }
}

void AFEAPIMQTTStandard::processRainSensor() {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Rainmeter"), AFE_NONE,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishRainSensorData();
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIMQTTStandard::processGate(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Gate"), *id, F("MQTT"));
#endif

  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) == 0) {
    _Hardware->Gate[*id]->toggle();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishGateState(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishGateState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Hardware->Gate[id]->configuration->mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _Hardware->Gate[id]->configuration->mqtt.topic);
      uint8_t _state = _Hardware->Gate[id]->get();
      publishStatus = Mqtt->publish(
          mqttStateTopic, _state == AFE_GATE_OPEN
                              ? AFE_MQTT_GATE_OPEN
                              : _state == AFE_GATE_CLOSED
                                    ? AFE_MQTT_GATE_CLOSED
                                    : _state == AFE_GATE_PARTIALLY_OPEN
                                          ? AFE_MQTT_GATE_PARTIALLY_OPEN
                                          : AFE_MQTT_GATE_UNKNOWN);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIMQTTStandard::processContactron(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Contactron"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishContactronState(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishContactronState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Hardware->Contactron[id]->configuration->mqtt.topic,
                       _Hardware->Contactron[id]->get() == AFE_OPEN ? AFE_OFF
                                                                    : AFE_ON,
                       true)
                 : false;
}
#endif //  AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIMQTTStandard::processDS18B20(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("DS18B20"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishDS18B20SensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishDS18B20SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_DS18B20_DATA_LENGTH];
    _Hardware->DS18B20Sensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->DS18B20Sensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
boolean AFEAPIMQTTStandard::publishRegulatorState(uint8_t id) {
  return enabled
             ? publishOnOffState(
                   _Hardware->Regulator[id]->configuration->mqtt.topic,
                   _Hardware->Regulator[id]->configuration->enabled ? AFE_ON
                                                                    : AFE_OFF)
             : false;
}

void AFEAPIMQTTStandard::processRegulator(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Regulator"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _Hardware->Regulator[*id]->on();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _Hardware->Regulator[*id]->off();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _Hardware->Regulator[*id]->toggle();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG
    printCommandNotImpementedWarning();
#endif
  }
  if (publishState) {
    publishRegulatorState(*id);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
boolean AFEAPIMQTTStandard::publishThermalProtectorState(uint8_t id) {
  return enabled
             ? publishOnOffState(
                   _Hardware->ThermalProtector[id]->configuration->mqtt.topic,
                   _Hardware->ThermalProtector[id]->configuration->enabled
                       ? AFE_ON
                       : AFE_OFF)
             : false;
}

void AFEAPIMQTTStandard::processThermalProtector(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Themrnal Protector"), *id,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _Hardware->ThermalProtector[*id]->on();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _Hardware->ThermalProtector[*id]->off();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _Hardware->ThermalProtector[*id]->toggle();
  } else if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG
    printCommandNotImpementedWarning();
#endif
  }
  if (publishState) {
    publishThermalProtectorState(*id);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPIMQTTStandard::processDHT(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("DHT"), *id, F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishDHTSensorData(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishDHTSensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_DHT_DATA_LENGTH];
    _Hardware->DHTSensor[id]->getJSON(message);
    publishStatus = Mqtt->publish(
        _Hardware->DHTSensor[id]->configuration->mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIMQTTStandard::processBinarySensor(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("Binary Sensor"), *id,
                                              F("MQTT"));
#endif
  if (strcmp(Mqtt->message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBinarySensorState(*id);
  }
#ifdef DEBUG
  else {
    printCommandNotImpementedWarning();
  }
#endif
}

boolean AFEAPIMQTTStandard::publishBinarySensorState(uint8_t id) {
  return enabled
             ? publishOnOffState(
                   _Hardware->BinarySensor[id]->configuration->mqtt.topic,
                   _Hardware->BinarySensor[id]->get() == AFE_BINARY_SENSOR_OPEN
                       ? AFE_OPEN
                       : AFE_CLOSED,
                   !_Hardware->BinarySensor[id]->configuration->sendAsSwitch)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
boolean AFEAPIMQTTStandard::publishPN532SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_PN532Sensor[id]->configuration.mqtt.topic) > 0) {
      char message[AFE_CONFIG_API_JSON_PN582_DATA_LENGTH];
      _PN532Sensor[id]->getJSON(message);
      publishStatus =
          Mqtt->publish(_PN532Sensor[id]->configuration.mqtt.topic, message);
    }
  }
  return publishStatus;
}
boolean AFEAPIMQTTStandard::publishMiFareCardState(uint8_t id, uint8_t state) {
  return enabled
             ? publishOnOffState(_MiFareCard[id]->configuration.mqtt.topic,
                                 state == AFE_HARDWARE_MIFARE_CARD_ACTION_ON
                                     ? AFE_ON
                                     : AFE_OFF,
                                 !_MiFareCard[id]->configuration.sendAsSwitch)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIMQTTStandard::processCLED(uint8_t *id) {
  boolean _success = true;
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("RGB LED"), *id, F("MQTT"));
#endif

  char _command[strlen(Mqtt->message.content) + 1];
  sprintf(_command, "%s", Mqtt->message.content);

  StaticJsonBuffer<AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(_command);
#ifdef DEBUG
  _Firmware->API->Flash->printBufforSizeInfo(
      AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH, jsonBuffer.size());
#endif
  if (!root.success()) {
#ifdef DEBUG
    _Firmware->API->Flash->printJSONNotPharsed();
#endif
  } else {

    CLED_PARAMETERS _color;
    char _state[4];

    sprintf(_state, root["command"] | "");

    if (strcmp(_state, "on") == 0) {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Processing ON"), F("RGB LED"));
#endif
      _Hardware->RGBLEDStrip->on(*id, true);
    } else if (strcmp(_state, "off") == 0) {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Processing OFF"), F("RGB LED"));

#endif
      _Hardware->RGBLEDStrip->off(*id, false);
    } else {
      _color.color.blue = root["color"]["blue"];
      _color.color.green = root["color"]["green"];
      _color.color.red = root["color"]["red"];
      _color.brightness =
          root["brightness"] |
          _Hardware->RGBLEDStrip->currentState[*id].on.brightness;

#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Changing color: RGB["),
                                            F("RGB LED"));
      Serial << _color.color.red << F(",") << _color.color.green << F(",")
             << _color.color.blue << F("], Brightness: ") << _color.brightness;
#endif

      /**
       * @brief if color is set to RGB=0 then turning OFF LED strip, otherwise
       * turn it ON
       *
       */

      if (_color.color.blue == 0 && _color.color.green == 0 &&
          _color.color.red == 0) {
        _Hardware->RGBLEDStrip->off(*id, false);
      } else {
        _Hardware->RGBLEDStrip->on(*id, _color, true, true);
      }
    }
  }

  if (_success) {
    if (_Hardware->RGBLEDStrip->isStateUpdated(*id)) {
      publishCLEDState(*id);
    }
    if (_Hardware->RGBLEDStrip->isEffectStateUpdated(*id)) {
      publishCLEDEffectsState(*id);
    }
  }
}

void AFEAPIMQTTStandard::processCLEDEffect(uint8_t *id) {
  boolean _success = true;
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("RGB LED"), *id, F("MQTT"));
#endif

  char _command[strlen(Mqtt->message.content) + 1];
  sprintf(_command, "%s", Mqtt->message.content);

  if (strcmp(_command, AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF) == 0) {
    _Hardware->RGBLEDStrip->deactivateEffect(*id);
  } else if (strcmp(_command,
                    _Hardware->RGBLEDStrip->configurationEffectBlinking[*id]
                        .name) == 0) {
    _Hardware->RGBLEDStrip->activateEffect(
        *id, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);
  } else if (strcmp(_command,
                    _Hardware->RGBLEDStrip->configurationEffectFadeInOut[*id]
                        .name) == 0) {
    _Hardware->RGBLEDStrip->activateEffect(
        *id, AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
  } else if (strcmp(
                 _command,
                 _Hardware->RGBLEDStrip->configurationEffectWave[*id].name) ==
             0) {
    _Hardware->RGBLEDStrip->activateEffect(
        *id, AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
  } else {
    _success = false;
  }

  if (_success) {
    if (_Hardware->RGBLEDStrip->isEffectStateUpdated(*id)) {
      publishCLEDEffectsState(*id);
    }
    if (_Hardware->RGBLEDStrip->isStateUpdated(*id)) {
      publishCLEDState(*id);
    }
  }
}

void AFEAPIMQTTStandard::processCLEDBrigtness(uint8_t *id) {
#ifdef DEBUG
  _Firmware->Debugger->printProcessingRequest(F("RGB LED"), *id, F("MQTT"));
#endif

  char _command[strlen(Mqtt->message.content) + 1];
  sprintf(_command, "%s", Mqtt->message.content);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  uint8_t _brightness =
      _Hardware->RGBLEDStrip->convertBrightnessFromAPI(*id, atof(_command));
#else
  uint8_t _brightness = atoi(_command);
#endif // AFE_FIRMWARE_API_STANDARD

  if (_brightness > 0) {
    CLED_PARAMETERS _color = _Hardware->RGBLEDStrip->currentState[*id].on;
    _color.brightness = _brightness;
    _Hardware->RGBLEDStrip->on(*id, _color, true, true);
  } else {
    _Hardware->RGBLEDStrip->off(*id, true);
  }

  if (_Hardware->RGBLEDStrip->isStateUpdated(*id)) {
    publishCLEDState(*id);
  }
  if (_Hardware->RGBLEDStrip->isEffectStateUpdated(*id)) {
    publishCLEDEffectsState(*id);
  }
}

boolean AFEAPIMQTTStandard::publishCLEDEffectsState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Hardware->RGBLEDStrip->configuration[id].effect.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _Hardware->RGBLEDStrip->configuration[id].effect.topic);
      publishStatus = Mqtt->publish(
          mqttStateTopic,
          _Hardware->RGBLEDStrip->currentState[id].effect.id ==
                  AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING
              ? _Hardware->RGBLEDStrip->configurationEffectBlinking[id].name
              : _Hardware->RGBLEDStrip->currentState[id].effect.id ==
                        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT
                    ? _Hardware->RGBLEDStrip->configurationEffectFadeInOut[id]
                          .name
                    : _Hardware->RGBLEDStrip->currentState[id].effect.id ==
                              AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE
                          ? _Hardware->RGBLEDStrip->configurationEffectWave[id]
                                .name
                          : AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF);
    }
  }
  return publishStatus;
}

boolean AFEAPIMQTTStandard::publishCLEDState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Hardware->RGBLEDStrip->configuration[id].cled.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      char _message[AFE_CONFIG_HARDWARE_CLED_STATE_JSON_LENGTH];

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
      sprintf(
          _message, "{\"state\":\"%s\",\"color\":{\"red\":%d,\"green\":%d,"
                    "\"blue\":%d},\"brightness\":%.2f}",
          _Hardware->RGBLEDStrip->currentState[id].state ? "on" : "off",
          _Hardware->RGBLEDStrip->currentState[id].state
              ? _Hardware->RGBLEDStrip->currentState[id].on.color.red
              : _Hardware->RGBLEDStrip->currentState[id].off.color.red,
          _Hardware->RGBLEDStrip->currentState[id].state
              ? _Hardware->RGBLEDStrip->currentState[id].on.color.green
              : _Hardware->RGBLEDStrip->currentState[id].off.color.green,
          _Hardware->RGBLEDStrip->currentState[id].state
              ? _Hardware->RGBLEDStrip->currentState[id].on.color.blue
              : _Hardware->RGBLEDStrip->currentState[id].off.color.blue,
          _Hardware->RGBLEDStrip->convertBrigtnessToAPI(
              id, _Hardware->RGBLEDStrip->currentState[id].config.brightness));
#else
      sprintf(_message, "{\"state\":\"%s\",\"color\":{\"red\":%d,\"green\":%d,"
                        "\"blue\":%d},\"brightness\":%d}",
              _Hardware->RGBLEDStrip->currentState[id].state ? "on" : "off",
              _Hardware->RGBLEDStrip->currentState[id].state
                  ? _Hardware->RGBLEDStrip->currentState[id].on.color.red
                  : _Hardware->RGBLEDStrip->currentState[id].off.color.red,
              _Hardware->RGBLEDStrip->currentState[id].state
                  ? _Hardware->RGBLEDStrip->currentState[id].on.color.green
                  : _Hardware->RGBLEDStrip->currentState[id].off.color.green,
              _Hardware->RGBLEDStrip->currentState[id].state
                  ? _Hardware->RGBLEDStrip->currentState[id].on.color.blue
                  : _Hardware->RGBLEDStrip->currentState[id].off.color.blue,
              _Hardware->RGBLEDStrip->currentState[id].config.brightness);
#endif
      sprintf(mqttStateTopic, "%s/state",
              _Hardware->RGBLEDStrip->configuration[id].cled.topic);
      publishStatus = Mqtt->publish(mqttStateTopic, _message);
    }
  }
  return publishStatus;
}

#endif // AFE_CONFIG_HARDWARE_CLED

/**** Private methods ****/

void AFEAPIMQTTStandard::subscribeToCommand(const char *topic) {
  if (strlen(topic) > 0) {
    char mqttCommandTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
    sprintf(mqttCommandTopic, "%s/cmd", topic);
    Mqtt->subscribe(mqttCommandTopic);
  }
}

boolean AFEAPIMQTTStandard::publishOnOffState(const char *topic, uint8_t state,
                                              boolean sendAsOpenClosed) {
  boolean publishStatus = false;

  if (enabled && (state == AFE_ON || state == AFE_OFF)) {
    if (strlen(topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state", topic);
      publishStatus = Mqtt->publish(
          mqttStateTopic,
          state == AFE_ON ? sendAsOpenClosed ? AFE_CLOSED_LABEL : AFE_ON_LABEL
                          : sendAsOpenClosed ? AFE_OPEN_LABEL : AFE_OFF_LABEL);
    }
  }
  return publishStatus;
}

uint8_t AFEAPIMQTTStandard::processOnOffCommand(byte *command,
                                                uint16_t *length) {
#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Processing: ON/OFF Command"),
                                        F("MQTT"));
#endif
  uint8_t _return = AFE_NONE;
  if ((char)command[1] == 'n' && *length == 2) { /* ON */
    _return = AFE_ON;
  } else if ((char)command[1] == 'f' && *length == 3) { /* OFF */
    _return = AFE_OFF;
#ifdef DEBUG
  } else {
    printCommandNotImpementedWarning();
#endif
  }
  return _return;
}

#ifdef DEBUG
void AFEAPIMQTTStandard::printYesNo(boolean state) {
  if (state) {
    Serial << F("Yes");
  } else {
    Serial << F("No");
  }
}

void AFEAPIMQTTStandard::printCommandNotImpementedWarning() {
  _Firmware->Debugger->printWarning(F("Command not implemented"), F("MQTT"));
}
#endif

#endif