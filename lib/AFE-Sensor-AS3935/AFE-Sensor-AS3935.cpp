/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-AS3935.h"

AFESensorAS3935::AFESensorAS3935(){};

boolean AFESensorAS3935::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getAS3935SensorConfiguration(id);
  I2CPORT I2C = Data.getI2CPortConfiguration();
  boolean _initialize = false;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
#endif


#ifdef DEBUG
  Serial << endl << endl << "----- AS3935: Initializing -----";
  Serial << endl << "IRQ GPIO : " << configuration.irqGPIO;
  Serial << endl << "Auto Noise: " << configuration.setNoiseFloorAutomatically;
  Serial << endl << "Noise Level : " << configuration.noiseFloor;
  Serial << endl << "Watchdog Threshold : " << configuration.watchdogThreshold;
  Serial << endl << "Strike Rejection : " << configuration.spikesRejectionLevel;
  Serial << endl
         << "Min.strikes level : "
         << configuration.minimumNumberOfLightningSpikes;
  Serial << endl << "Indoor? : " << configuration.indoor;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  Serial << endl << "IDX: " << configuration.domoticz.idx;
#endif

#endif
  if (configuration.i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "Address: 0x" << _HEX(configuration.i2cAddress);
    Serial << endl << "Scanning I2C Port for a device";
#endif

    AFEI2CScanner I2CScanner;
    I2CScanner.begin();
    if (I2CScanner.scan(configuration.i2cAddress)) {

#ifdef DEBUG
      Serial << endl << "Initializing IRQ GPIO";
#endif

      pinMode(configuration.irqGPIO, INPUT);

      Wire.begin(I2C.SDA, I2C.SCL);
      if (!AS3935Sensor.begin(configuration.i2cAddress, Wire)) {
#ifdef DEBUG
        Serial << endl << "ERROR: AS3935 is not initialized";
#endif
        while (1)
          ;
      } else {
#ifdef DEBUG
        Serial << endl << "AS3935 is initialized";
#endif
        AS3935Sensor.maskDisturber(true);

#ifdef DEBUG
        int maskVal = AS3935Sensor.readMaskDisturber();
        Serial << endl << "Are disturbers being masked: ";
        if (maskVal == 1)
          Serial << "YES";
        else if (maskVal == 0)
          Serial << "NO";
#endif

#ifdef DEBUG
        int enviVal = AS3935Sensor.readIndoorOutdoor();
        Serial << endl << endl << "Default: Sensor localisation: ";
        if (enviVal == INDOOR)
          Serial << "Indoor";
        else if (enviVal == OUTDOOR)
          Serial << "Outdoor";
        else
          Serial.print(enviVal, BIN);
#endif

        AS3935Sensor.setIndoorOutdoor(configuration.indoor ? INDOOR : OUTDOOR);

#ifdef DEBUG
        enviVal = AS3935Sensor.readIndoorOutdoor();
        Serial << endl << " - New value set to: ";
        if (enviVal == INDOOR)
          Serial << "Indoor";
        else if (enviVal == OUTDOOR)
          Serial << "Outdoor";
        else
          Serial.print(enviVal, BIN);
#endif

        if (!configuration.setNoiseFloorAutomatically) {
#ifdef DEBUG
          Serial << endl
                 << endl
                 << "Default: Noise Level: " << AS3935Sensor.readNoiseLevel();
#endif

          AS3935Sensor.setNoiseLevel(configuration.noiseFloor);
#ifdef DEBUG
          Serial << endl
                 << " - New value set to: " << AS3935Sensor.readNoiseLevel();

#endif
        }
#ifdef DEBUG
        else {
          Serial << endl << endl << "Noise Level automatically managed";
        }
#endif

#ifdef DEBUG
        Serial << endl
               << endl
               << "Default: Watchdog Threshold: "
               << AS3935Sensor.readWatchdogThreshold();
#endif

        AS3935Sensor.watchdogThreshold(configuration.watchdogThreshold);
#ifdef DEBUG
        Serial << endl
               << " - New value set to: "
               << AS3935Sensor.readWatchdogThreshold();
#endif

#ifdef DEBUG
        Serial << endl
               << endl
               << "Default: Spike Rejection: "
               << AS3935Sensor.readSpikeRejection();
#endif
        AS3935Sensor.spikeRejection(configuration.spikesRejectionLevel);
#ifdef DEBUG
        Serial << endl
               << " - New value set to: " << AS3935Sensor.readSpikeRejection();
#endif

#ifdef DEBUG
        Serial << endl
               << endl
               << "Default number of strikes before interrupt is triggerd: "
               << AS3935Sensor.readLightningThreshold();
#endif
        AS3935Sensor.lightningThreshold(
            configuration.minimumNumberOfLightningSpikes);
#ifdef DEBUG
        Serial << endl
               << " - New value set to: "
               << AS3935Sensor.readLightningThreshold();
#endif
        _initialize = true;
      }
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "Error: Device not found under I2C Address: 0x"
             << _HEX(configuration.i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << "Error: Address not set";
  }
#endif

#ifdef DEBUG
  Serial << endl << "---------------------------------";
#endif
  return _initialize;
}

void AFESensorAS3935::interruptionReported() {
  ready = true;
  distance = AFE_CONFIG_HARDWARE_AS3935_DEFAULT_UNKNOWN_DISTANCE;
  eventType = AS3935Sensor.readInterruptReg();
  switch (eventType) {
  case NOISE_TO_HIGH:
#ifdef DEBUG
    Serial << endl << "AS3935: Interuption detected: NOISE";
#endif
    if (configuration.setNoiseFloorAutomatically) {
      increaseNoiseLevel();
    }
    break;
  case DISTURBER_DETECT:
#ifdef DEBUG
    Serial << endl << "AS3935: Interuption detected: DISTURBER";
#endif
    break;
  case LIGHTNING:
    distance = AS3935Sensor.distanceToStorm();
#ifdef DEBUG
    Serial << endl << "AS3935: Interuption detected: STRIKE";
    Serial << endl << "AS3935: Distance: " << distance;
#endif
    break;
  default:
    ready = false;
#ifdef DEBUG
    Serial << endl << "AS3935: Warning: Unknown interruption!";
#endif
  }
}

boolean AFESensorAS3935::strikeDetected() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorAS3935::getJSON(char *json) {
  if (eventType == LIGHTNING) {
    sprintf(json,
            "{\"event\":{\"type\":\"lightning "
            "strike\",\"distance\":%d,\"unit\":\"%s\"}}",
            distance, configuration.unit == AFE_DISTANCE_KM ? "km" : "mil");
  } else if (eventType == NOISE_TO_HIGH) {
    sprintf(json, "{\"event\":{\"type\":\"noise\"}}");
  } else if (eventType == NOISE_TO_HIGH) {
    sprintf(json, "{\"event\":{\"type\":\"disruption\"}}");
  } else {
    sprintf(json, "{\"event\":{\"type\":\"unknown\"}}");
  }
}

void AFESensorAS3935::increaseNoiseLevel() {
  int level = AS3935Sensor.readNoiseLevel() + 1;
  if (level <= AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MAX_NOISE_FLOOR) {
    AS3935Sensor.setNoiseLevel(level);
#ifdef DEBUG
    Serial << endl << "AS3935: Noise level has been increased to: " << level;
  } else {
    Serial << endl
           << "AS3935: Warning: Noise level has NOT been increased. It's "
              "already at its MAX";
#endif
  }
}
