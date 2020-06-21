/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-AS3935.h"

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
    mqttCommandTopic[0] = '\0';
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

      AS3935LightingSensor.configuration.ADDRESS = configuration.i2cAddress;
      AS3935LightingSensor.configuration.SCL = I2C.SCL;
      AS3935LightingSensor.configuration.SDA = I2C.SDA;
      AS3935LightingSensor.configuration.IRQ = configuration.irqGPIO;
      AS3935LightingSensor.configuration.AFE_GB =
          configuration.indoor ? AFE_GAIN_INDOOR : AFE_GAIN_OUTDOOR;
      AS3935LightingSensor.configuration.MIN_NUM_LIGH =
          configuration.minimumNumberOfLightningSpikes;
      AS3935LightingSensor.configuration.NF_LEV = configuration.noiseFloor;
      AS3935LightingSensor.configuration.SREJ =
          configuration.spikesRejectionLevel;
      AS3935LightingSensor.configuration.TUN_CAP =
          configuration.tuningCapacitor;
      AS3935LightingSensor.configuration.WDTH = configuration.watchdogThreshold;

      AS3935LightingSensor.begin();

#ifdef DEBUG
      Serial << endl << F("INFO: AS3935 is initialized");
      Serial << endl << F("INFO: AS3935 reading current settings");

      Serial << endl
             << F("INFO: AS3935: AFE Gain: ")
             << (AS3935LightingSensor.readAFEGainBoost() == AFE_GAIN_INDOOR
                     ? "Indoor"
                     : AS3935LightingSensor.readAFEGainBoost() ==
                               AFE_GAIN_OUTDOOR
                           ? "Outdoor"
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
      Serial << endl
             << "INFO: AS3935: Tuning Capacitor: "
             << AS3935LightingSensor.readTuningCapacitors() *
                    AFE_CONFIG_HARDWARE_TUNING_CAPACITOR_STEP
             << "pF";

      Serial << endl << endl << "INFO: AS3935 Configuring the sensor";

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
/*
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
*/
boolean AFESensorAS3935::listener() {
  if (AS3935LightingSensor.detected()) {
    event = AS3935LightingSensor.get();

#ifdef DEBUG
    Serial << endl
           << "INFO: AS3935: Event: " << event.reason
           << ", distance: " << event.distance;
#endif

    ready = true;
  } else {
    ready = false;
  }

  return ready;
}

void AFESensorAS3935::getJSON(char *json) {

  /*

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

    */
}