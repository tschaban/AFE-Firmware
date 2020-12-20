/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-AS3935.h"
#ifdef AFE_CONFIG_HARDWARE_AS3935

AFESensorAS3935::AFESensorAS3935(){};

boolean AFESensorAS3935::begin(uint8_t id) {
  AFEDataAccess Data;
  Data.getConfiguration(id, &configuration);
  I2CPORT I2C;
  Data.getConfiguration(&I2C);
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef DEBUG
  Serial << endl << endl << F("----- AS3935: Initializing -----");
  Serial << endl << F("IRQ GPIO : ") << configuration.irqGPIO;
  Serial << endl
         << F("Auto Noise: ") << configuration.setNoiseFloorAutomatically;
  Serial << endl << F("Noise Level : ") << configuration.noiseFloor;
  Serial << endl
         << F("Watchdog Threshold : ") << configuration.watchdogThreshold;
  Serial << endl
         << F("Strike Rejection : ") << configuration.spikesRejectionLevel;
  Serial << endl
         << F("Min.strikes level : ")
         << configuration.minimumNumberOfLightningSpikes;
  Serial << endl << F("Indoor? : ") << configuration.indoor;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  Serial << endl << F("IDX: ") << configuration.domoticz.idx;
#endif

#endif

  if (configuration.i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << F("Address: 0x") << _HEX(configuration.i2cAddress);
    Serial << endl << F("---------------------------------");

    Serial << endl << endl << F("INFO: Scanning I2C Port for a device");
#endif

    AFEI2CScanner I2CScanner;
    I2CScanner.begin();
    if (I2CScanner.scan(configuration.i2cAddress)) {

#ifdef DEBUG
      Serial << endl << F("INFO: Initializing IRQ GPIO");
#endif

      AS3935LightingSensor.begin(I2C.SDA, I2C.SCL, configuration.i2cAddress,
                                 configuration.irqGPIO);

#ifdef DEBUG
      Serial << endl << F("INFO: AS3935 is initialized");
      Serial << endl << F("INFO: AS3935 reading current settings");

      Serial << endl
             << F("INFO: AS3935: AFE Gain: ")
             << (AS3935LightingSensor.isIndoor()
                     ? "Indoor"
                     : AS3935LightingSensor.isOutdoor() ? "Outdoor"
                                                        : "Unknown");

      Serial << endl
             << "INFO: AS3935: Minimum Number Of Lightning: "
             << AS3935LightingSensor.readMinimumNumberOfLightning();
      Serial << endl
             << "INFO: AS3935: Noise Floor Level: "
             << AS3935LightingSensor.readNoiseFloorLevel();
      Serial << endl
             << "INFO: AS3935: Spike rejection: "
             << AS3935LightingSensor.readSpikeRejection();
      Serial << endl
             << "INFO: AS3935: Watchdog threshold: "
             << AS3935LightingSensor.readWatchdogThreshold();
      Serial << endl
             << "INFO: AS3935: Mask Disturber: "
             << AS3935LightingSensor.readMaskDisturber();
      Serial << endl
             << "INFO: AS3935: Frequency division ration for antenna tuning: "
             << AS3935LightingSensor.readFrequencyDivisionForAntennaTuning();

      Serial << endl << endl << "INFO: AS3935 Configuring the sensor";

#endif

#ifdef DEBUG
      Serial << endl << " - resetting to defalut values";
#endif
      AS3935LightingSensor.clearStatistics();
      AS3935LightingSensor.setDefautSettings();

      AS3935LightingSensor.setIndoor(configuration.indoor);

      if (!configuration.setNoiseFloorAutomatically) {
        AS3935LightingSensor.setNoiseFloorLevel(configuration.noiseFloor);
      }

      AS3935LightingSensor.setMinimumNumberOfLightning(
          configuration.minimumNumberOfLightningSpikes);

      AS3935LightingSensor.setSpikeRejection(
          configuration.spikesRejectionLevel);

      AS3935LightingSensor.setWatchdogThreshold(
          configuration.watchdogThreshold);

      AS3935LightingSensor.calibrateInternalRSOscillators();

      AS3935LightingSensor.setFrequencyDivisionForAntennaTuning(16);

#ifdef DEBUG
      Serial << endl
             << F("INFO: AS3935: AFE Gain: ")
             << (AS3935LightingSensor.isIndoor()
                     ? "Indoor"
                     : AS3935LightingSensor.isOutdoor() ? "Outdoor"
                                                        : "Unknown");

      Serial << endl
             << "INFO: AS3935: Minimum Number Of Lightning: "
             << AS3935LightingSensor.readMinimumNumberOfLightning();
      Serial << endl
             << "INFO: AS3935: Noise Floor Level: "
             << AS3935LightingSensor.readNoiseFloorLevel();
      Serial << endl
             << "INFO: AS3935: Spike rejection: "
             << AS3935LightingSensor.readSpikeRejection();
      Serial << endl
             << "INFO: AS3935: Watchdog threshold: "
             << AS3935LightingSensor.readWatchdogThreshold();
      Serial << endl
             << "INFO: AS3935: Mask Disturber: "
             << AS3935LightingSensor.readMaskDisturber();
      Serial << endl
             << "INFO: AS3935: Frequency division ration for antenna tuning: "
             << AS3935LightingSensor.readFrequencyDivisionForAntennaTuning();

#endif

    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: Device not found under I2C Address: 0x")
             << _HEX(configuration.i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: Address not set");
  }
#endif
}

void AFESensorAS3935::interruptionReported() {
  ready = true;
  distance = AFE_CONFIG_HARDWARE_AS3935_DEFAULT_UNKNOWN_DISTANCE;

#ifdef DEBUG
  Serial << endl << F("INFO: AS3935, Reading event type ");
#endif
  eventType = AS3935LightingSensor.readIntrruptReason();
  switch (eventType) {
  case INT_NOISE:
#ifdef DEBUG
    Serial << F("NOISE");
#endif
    if (configuration.setNoiseFloorAutomatically) {
#ifdef DEBUG
      Serial << endl << F("INFO: Increasing noise level ");
#endif
      AS3935LightingSensor.increaseNoiseFloorLevel();
#ifdef DEBUG
      Serial << endl
             << F("INFO: Noise level set to  ") << endl
             << AS3935LightingSensor.readNoiseFloorLevel();
#endif
    }
    break;
  case INT_DISTURBER:
#ifdef DEBUG
    Serial << F("DISTURBER");
#endif
    break;
  case INT_STRIKE:
    distance = AS3935LightingSensor.readDistanceToStorm();
#ifdef DEBUG
    Serial << F("STRIKE");
    Serial << endl << F("INFO: AS3935: Distance: ") << distance;
#endif
    break;
  default:
    ready = false;
#ifdef DEBUG
    Serial << F("?") << endl
           << F("WARN: AS3935: Unknown interruption: ") << eventType;
#endif
  }
}

boolean AFESensorAS3935::strikeDetected() {
  /*
  if (digitalRead(configuration.irqGPIO) == HIGH) {

  #ifdef DEBUG
  // Serial << endl << F("INFO: AS3935: Interuption");
  #endif

    interruptionReported();
   }
  */

  // interruptionReported();

  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorAS3935::getJSON(char *json) {

  if (eventType == INT_STRIKE) {
    sprintf(json, "{\"event\":{\"type\":\"lightning "
                  "strike\",\"distance\":%d,\"unit\":\"%s\"}}",
            distance, configuration.unit == AFE_DISTANCE_KM ? "km" : "mil");
  } else if (eventType == INT_NOISE) {
    sprintf(json, "{\"event\":{\"type\":\"noise\"}}");
  } else if (eventType == INT_DISTURBER) {
    sprintf(json, "{\"event\":{\"type\":\"disruption\"}}");
  } else {
    sprintf(json, "{\"event\":{\"type\":\"unknown\"}}");
  }
}

#endif // AFE_CONFIG_HARDWARE_AS3935