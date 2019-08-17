/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Analog-Input.h"

AFEAnalogInput::AFEAnalogInput(){};

void AFEAnalogInput::begin() {
  AFEDataAccess Data;
  configuration = Data.getADCInputConfiguration();
  _initialized = true;

#ifdef DEBUG
  Serial << endl << endl << "------------ AC VCC Input ------------";
  Serial << endl
         << "- Initialized" << endl
         << "- GPIO: " << configuration.gpio << endl
         << "- Interval: " << configuration.interval << endl
         << "- Max VCC: " << configuration.maxVCC << endl
         << "- No of Samples: " << configuration.numberOfSamples << endl
         << "- R[A]: " << configuration.divider.Ra << endl
         << "- R[B]: " << configuration.divider.Rb << endl
         << "-------------------------------------";

#endif
}

ADCINPUT_DATA AFEAnalogInput::get() {
  ADCINPUT_DATA data;
  data.raw = analogData;
  data.percent = (float)analogData * 100 / 1024;
  data.voltage = (double)(configuration.maxVCC * analogData / 1024);
  if (configuration.divider.Rb > 0) {
    data.voltageCalculated =
        (data.voltage * (configuration.divider.Ra + configuration.divider.Rb)) /
        configuration.divider.Rb;
  } else {
    data.voltageCalculated = data.voltage;
  }
  return data;
}

boolean AFEAnalogInput::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFEAnalogInput::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

      if (counterOfSamplings < configuration.numberOfSamples) {
        temporaryAnalogData += analogRead(configuration.gpio);
        counterOfSamplings++;
      } else {

        analogData =
            (uint16_t)(temporaryAnalogData / configuration.numberOfSamples);

#ifdef DEBUG
        Serial << endl
               << " - Number of samples: " << counterOfSamplings << endl
               << " - Analog value = " << analogData << endl
               << " - Sampling time = "
               << millis() - startTime - configuration.interval * 1000
               << "msec.";
#endif

        counterOfSamplings = 0;
        temporaryAnalogData = 0;
        ready = true;
        startTime = 0;

#ifdef DEBUG
        Serial << endl << "-------------------------------------------";
#endif
      }
    }
  }
}

void AFEAnalogInput::getDomoticzIDX(ADCINPUT_DOMOTICZ *idx) {
  *idx = configuration.domoticz;
}

/* Method returns MQTT topic for this relay */
const char *AFEAnalogInput::getMQTTCommandTopic() {
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
  return mqttCommandTopic;
}
const char *AFEAnalogInput::getMQTTStateTopic() {
  return configuration.mqtt.topic;
}
