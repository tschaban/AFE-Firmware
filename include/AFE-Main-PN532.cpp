/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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
  for (uint8_t i = 0; i < Device.configuration.noOfPN532Sensors; i++) {
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
    PN532Sensor[i].begin(i, &Data, &Device, &WirePort0, &WirePort1);
#else
    PN532Sensor[i].begin(i, &Data, &Device, &WirePort0);
#endif
#else
    PN532Sensor[i].begin(i, &Data, &Device);
#endif // AFE_CONFIG_HARDWARE_I2C
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: PN532: ") << i << F(" initialized");
#endif
  }

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing MiFare Cards");
#endif
  for (uint8_t i = 0; i < Device.configuration.noOfMiFareCards; i++) {
    MiFareCard[i].begin(i, &Data);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: MiFare: ") << i << F(" initialized");
#endif
  }
}

/* Method processes Contactron's related events */
void PN532EventsListener() {
  boolean actionTaken = false;
  for (uint8_t i = 0; i < Device.configuration.noOfPN532Sensors; i++) {

    switch (PN532Sensor[i].listener()) {

    /* Card detected */
    case AFE_HARDWARE_PN532_LISTENER_EVENT_FOUND:

      if (PN532Sensor[i].readTag()) {
#ifdef AFE_CONFIG_HARDWARE_CLED
        /* Changing the CLED Effect card detected, but not authorized yet */
        CLed[AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR].setCustomEffectColor(
            AFE_CONFIG_HARDWARE_EFFECT_WAVE,
            AFE_CONFIG_HARDWARE_CLED_COLOR_MIFARE_UNAUTHORIZE);
        CLed[AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR].effectOn(
            AFE_CONFIG_HARDWARE_EFFECT_WAVE);
#endif
        for (uint8_t j = 0; j < Device.configuration.noOfMiFareCards; j++) {
          if (strcmp(MiFareCard[j].configuration.cardId,
                     PN532Sensor[i].tag.block[0].value) == 0) {

#ifdef AFE_CONFIG_HARDWARE_CLED
            /* Changing the CLED Effect color to authorized */
            CLed[AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR].setCustomEffectColor(
                AFE_CONFIG_HARDWARE_EFFECT_WAVE,
                CLed[AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR]
                    .effects.effect[AFE_CONFIG_HARDWARE_EFFECT_WAVE]
                    .color);
#endif

            actionTaken = false;
#ifdef AFE_CONFIG_HARDWARE_GATE
            // Checking if the controlls gate
            for (uint8_t k = 0; k < Device.configuration.noOfGates; k++) {
              if (Gate[k].configuration.relayId ==
                  MiFareCard[j].configuration.relayId) {
                Gate[k].toggle();
                actionTaken = true;
                break;
              }
            }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
            if (!actionTaken &&
                Device.configuration.noOfRelays >=
                    MiFareCard[j].configuration.relayId + 1) {
              switch (MiFareCard[j].configuration.action) {
              case AFE_HARDWARE_MIFARE_CARD_ACTION_ON:
                Relay[MiFareCard[j].configuration.relayId].on();
                break;
              case AFE_HARDWARE_MIFARE_CARD_ACTION_OFF:
                Relay[MiFareCard[j].configuration.relayId].off();
                break;
              case AFE_HARDWARE_MIFARE_CARD_ACTION_TOGGLE:
                Relay[MiFareCard[j].configuration.relayId].toggle();
                break;
              }
              MqttAPI.publishRelayState(MiFareCard[j].configuration.relayId);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
              HttpDomoticzAPI.publishRelayState(
                  MiFareCard[j].configuration.relayId);
#endif
              actionTaken = true;
            }
#endif
            MiFareCard[j].set();
            /* Publishing status of MiFareCard */
          }
          if (actionTaken) {
            break;
          }
        }

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
        MqttAPI.publishPN532SensorData(i);
#endif
      }
      break;

/* End of processing time of a request. Used only by CLED */
#ifdef AFE_CONFIG_HARDWARE_CLED
    case AFE_HARDWARE_PN532_LISTENER_EVENT_PROCESSING_FINISHED:
      /* Changing the CLED Effect to listening mode */
      CLed[AFE_CONFIG_HARDWARE_CLED_ID_PN532_SENSOR].effectOn(
          AFE_CONFIG_HARDWARE_EFFECT_FADE_IN_OUT);
      break;
#endif
    }

    for (uint8_t j = 0; j < Device.configuration.noOfMiFareCards; j++) {
      if (MiFareCard[j].listener()) {

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        for (uint8_t idxId = 0; idxId < AFE_HARDWARE_PN532_TAG_SIZE; idxId++) {

          MqttAPI.publishMiFareCardState(j, idxId, MiFareCard[j].state,
                                         PN532Sensor[i].tag.block[idxId].value);

          HttpDomoticzAPI.publishMiFareCardState(
              j, idxId, MiFareCard[j].state,
              PN532Sensor[i].tag.block[idxId].value);
        }
#else
        MqttAPI.publishMiFareCardState(j, MiFareCard[j].state);
#endif
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
