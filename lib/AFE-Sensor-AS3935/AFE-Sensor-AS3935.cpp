/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-AS3935.h"

AFESensorAS3935::AFESensorAS3935(){};

void AFESensorAS3935::begin() {
  AFEDataAccess Data;
  configuration = Data.getAS3935SensorConfiguration();

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }

#ifdef DEBUG
  Serial << endl << endl << "----- AS3935: Initializing -----";
#endif
  if (configuration.i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "Address: 0x" << _HEX(configuration.i2cAddress);
#endif

    pinMode(configuration.irqGPIO, INPUT);

    Wire.begin();
    if (!AS3935Sensor.begin(configuration.i2cAddress, Wire)) {
#ifdef DEBUG
      Serial << endl << "ERROR: AS3935 is not initialized";
#endif
      while (1)
        ;
    } else {
#ifdef DEBUG
      Serial << endl << "AS3935 is not initialized";
#endif
      _initialized = true;
      AS3935Sensor.maskDisturber(true);

#ifdef DEBUG
      int maskVal = AS3935Sensor.readMaskDisturber();
      Serial << endl << "Are disturbers being masked: ";
      if (maskVal == 1)
        Serial << "YES";
      else if (maskVal == 0)
        Serial << "NO";
#endif

      AS3935Sensor.setIndoorOutdoor(configuration.indoor);

#ifdef DEBUG
      int enviVal = AS3935Sensor.readIndoorOutdoor();
      Serial << endl << "Sensor localisation: ";
      if (enviVal == INDOOR)
        Serial << "Indoor";
      else if (enviVal == OUTDOOR)
        Serial << "Outdoor";
      else
        Serial.println(enviVal, BIN);
#endif

      if (!configuration.setNoiseFloorAutomatically) {
        AS3935Sensor.setNoiseLevel(configuration.noiseFloor);
#ifdef DEBUG
        Serial << endl
               << "Noise Level is set at: " << AS3935Sensor.readNoiseLevel();

#endif
      }
#ifdef DEBUG
      else {
        Serial << endl << "Noise Level automatically managed";
      }
#endif

#ifdef DEBUG
      // AS3935Sensor.watchdogThreshold(watchDogVal);
      Serial << endl
             << "Watchdog Threshold is set to: "
             << AS3935Sensor.readWatchdogThreshold();

#endif

#ifdef DEBUG
      //      AS3935Sensor.spikeRejection(spike);
      Serial << endl
             << "Spike Rejection is set to: "
             << AS3935Sensor.readSpikeRejection();
#endif

#ifdef DEBUG
      //  AS3935Sensor.lightningThreshold(lightningThresh);
      Serial << endl
             << "The number of strikes before interrupt is triggerd: "
             << AS3935Sensor.readLightningThreshold();
#endif
    }

  }
#ifdef DEBUG
  else {
    Serial << endl << "Error: Address not set";
  }
#endif

#ifdef DEBUG
  if (_initialized) {
    Serial << endl << "IRQ GPIO : " << configuration.irqGPIO;
    Serial << endl
           << "Auto Noise: " << configuration.setNoiseFloorAutomatically;
    Serial << endl << "Noise Level : " << configuration.noiseFloor;
    Serial << endl << "Indoor? : " << configuration.indoor;
    Serial << endl << "IDX: " << configuration.domoticz.idx;
  }
  Serial << endl
         << "Device: " << (_initialized ? "Found" : "Not found: check wiring");
  Serial << endl << "---------------------------------";
#endif
}

boolean AFESensorAS3935::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorAS3935::listener() {

  if (_initialized && digitalRead(configuration.irqGPIO) == HIGH) {

    switch (AS3935Sensor.readInterruptReg()) {
    case NOISE_TO_HIGH:
#ifdef DEBUG
      Serial << endl << "AS3935: Interuption detected: NOISE";
#endif
      break;
    case DISTURBER_DETECT:
#ifdef DEBUG
      Serial << endl << "AS3935: Interuption detected: DISTURBER";
#endif
      break;
    case LIGHTNING:
      byte distance = AS3935Sensor.distanceToStorm();
#ifdef DEBUG
      Serial << endl << "AS3935: Interuption detected: STRIKE";
      Serial << endl << "AS3935: Distance: " << distance;
#endif

      break;
    }

    ready = true;
  }
}

void AFESensorAS3935::getJSON(char *json) {
  sprintf(json, "{\"illuminance\":{\"value\":%d,\"unit\":\"lux\"}}", 0);
}