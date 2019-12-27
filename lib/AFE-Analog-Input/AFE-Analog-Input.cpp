/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Analog-Input.h"

AFEAnalogInput::AFEAnalogInput(){};

void AFEAnalogInput::begin() {
  AFEDataAccess Data;
  configuration = Data.getADCInputConfiguration();
  _initialized = true;

#ifdef AFE_CONFIG_API_MQTT_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
#endif

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


boolean AFEAnalogInput::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFEAnalogInput::listener() {
  uint16_t temporaryAnalogData = 0;
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

        data.raw = (uint16_t)(temporaryAnalogData / configuration.numberOfSamples);
        data.percent = (float)data.raw  * 100 / 1024;
        data.voltage = (double)(configuration.maxVCC * data.raw  / 1024);
        if (configuration.divider.Rb > 0) {
          data.voltageCalculated =
            (data.voltage * (configuration.divider.Ra + configuration.divider.Rb)) /
            configuration.divider.Rb;
        } else {
            data.voltageCalculated = data.voltage;
        }
            

#ifdef DEBUG
        Serial << endl
               << " - Number of samples: " << counterOfSamplings << endl
               << " - Analog value = " << data.raw  << endl
               << " - Percent = " << data.percent  << endl
               << " - Voltage = " << data.voltage  << endl
               << " - VoltageCalculated = " << data.voltageCalculated  << endl
               << " - Sampling time = "
               << millis() - startTime - configuration.interval * 1000
               << "msec.";
#endif

        counterOfSamplings = 0;
        ready = true;
        startTime = 0;

#ifdef DEBUG
        Serial << endl << "-------------------------------------------";
#endif
      }
    }
  }
}


void AFEAnalogInput::getJSON(char *json) {
 sprintf(json,
          "{\"raw\":%d,\"percent\":%f,\"voltage\":%f,\"voltageCalculated\":%f}",
          data.raw, data.percent,data.voltage,data.voltageCalculated);
}