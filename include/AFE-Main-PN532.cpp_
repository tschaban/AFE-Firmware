/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

/* ---------Headers ---------*/

void initializePN532Sensor(void);
void PN532EventsListener(void);

/* --------- Body -----------*/

/* Method initialize contactrons */
void initializePN532Sensor() {
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing PN532");
#endif
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfPN532Sensors; i++) {
    PN532Sensor[i].begin(i, Firmware);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: PN532: ") << i << F(" initialized");
#endif
  }
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  if (Firmware->Device->configuration.noOfPN532Sensors > 0 &&
      Firmware->Device->configuration.effectPN532) {
    /* Changing the CLED Effect to listening mode */
    CLEDStrip->effectOn(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                       AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
  }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing MiFare Cards");
#endif
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfMiFareCards; i++) {
    MiFareCard[i].begin(i, Firmware->API->Flash);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: MiFare: ") << i << F(" initialized");
#endif
  }
}

/* Method processes Contactron's related events */
void PN532EventsListener() {
  boolean actionTaken = false;
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfPN532Sensors; i++) {

    switch (PN532Sensor[i].listener()) {

    /* Card detected */
    case AFE_HARDWARE_PN532_LISTENER_EVENT_FOUND:

      if (PN532Sensor[i].readTag()) {
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
        /* Changing the CLED Effect card detected, but not authorized yet */
        if (Firmware->Device->configuration.effectPN532) {
          CLEDStrip->setCustomEffectColor(
              AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
              AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE,
              AFE_CONFIG_HARDWARE_CLED_COLOR_MIFARE_UNAUTHORIZE);

          CLEDStrip->effectOn(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                             AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
        }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
        for (uint8_t j = 0; j < Firmware->Device->configuration.noOfMiFareCards; j++) {
          if (strcmp(Hardware->MiFareCard[j]->configuration.cardId,
                     PN532Sensor[i].tag.block[0].value) == 0) {

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
            /* Changing the CLED Effect color to authorized */
            if (Firmware->Device->configuration.effectPN532) {

              CLEDStrip->setCustomEffectColor(
                  AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                  AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE,
                  CLEDStrip->effects.effect[AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE]
                      .color);
            }
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT

            actionTaken = false;
#ifdef AFE_CONFIG_HARDWARE_GATE
            // Checking if the controlls gate
            for (uint8_t k = 0; k < Firmware->Device->configuration.noOfGates; k++) {
              if (Hardware->Gate[k]->configuration.relayId ==
                  Hardware->MiFareCard[j]->configuration.relayId) {
                Gate[k].toggle();
                actionTaken = true;
                break;
              }
            }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
            if (!actionTaken &&
                Firmware->Device->configuration.noOfRelays >=
                    Hardware->MiFareCard[j]->configuration.relayId + 1) {
              switch (Hardware->MiFareCard[j]->configuration.action) {
              case AFE_HARDWARE_MIFARE_CARD_ACTION_ON:
                Hardware->Relay[Hardware->MiFareCard[j]->configuration.relayId].on();
                break;
              case AFE_HARDWARE_MIFARE_CARD_ACTION_OFF:
                Hardware->Relay[Hardware->MiFareCard[j]->configuration.relayId].off();
                break;
              case AFE_HARDWARE_MIFARE_CARD_ACTION_TOGGLE:
                Hardware->Relay[Hardware->MiFareCard[j]->configuration.relayId].toggle();
                break;
              }
              MqttAPI->publishRelayState(Hardware->MiFareCard[j]->configuration.relayId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
              HttpDomoticzAPI->publishRelayState(
                  Hardware->MiFareCard[j].configuration.relayId);
#endif
              actionTaken = true;
            }
#endif
            Hardware->MiFareCard[j].set();
            /* Publishing status of MiFareCard */
          }
          if (actionTaken) {
            break;
          }
        }

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
        MqttAPI->publishPN532SensorData(i);
#endif
      }
      break;
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
    /* End of processing time of a request. Used only by CLED */
    case AFE_HARDWARE_PN532_LISTENER_EVENT_PROCESSING_FINISHED:
      if (Firmware->Device->configuration.effectPN532) {
        /* Changing the CLED Effect to listening mode */
        CLEDStrip->effectOn(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT_ID,
                           AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
      }
      break;
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
    }

    for (uint8_t j = 0; j < Firmware->Device->configuration.noOfMiFareCards; j++) {
      if (Hardware->MiFareCard[j]->listener()) {

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        for (uint8_t idxId = 0; idxId < AFE_HARDWARE_PN532_TAG_SIZE; idxId++) {

          MqttAPI->publishMiFareCardState(j, idxId, Hardware->MiFareCard[j].state,
                                         PN532Sensor[i].tag.block[idxId].value);

          HttpDomoticzAPI->publishMiFareCardState(
              j, idxId, Hardware->MiFareCard[j].state,
              PN532Sensor[i].tag.block[idxId].value);

#ifdef DEBUG
          Serial << endl
                 << F("INFO: MiFare: TAG[") << (idxId + 1)
                 << F("]: MQTT publishing processed");
#endif
        }
#else
        MqttAPI->publishMiFareCardState(j, Hardware->MiFareCard[j].state);
#endif
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
