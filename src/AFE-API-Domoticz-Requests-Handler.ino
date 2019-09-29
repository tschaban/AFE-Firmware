/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.api.domoticz) {
    Domoticz.begin();
#ifdef DEBUG
    Serial << endl << "API: Domoticz initialized";
#endif
  }
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
/* It publishes relay state to Domoticz */
void DomoticzPublishRelayState(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    unsigned long idx = Relay[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendSwitchCommand(idx, Relay[id].get() == AFE_RELAY_ON ? "On"
                                                                      : "Off");
    }
  }
}
#endif

/* Temperature and Humiditity sensors */
#ifdef AFE_CONFIG_TEMPERATURE
/* It publishes temperature to Domoticz */
void DomoticzPublishTemperature(unsigned long idx, float temperature) {
  if (Device.configuration.api.domoticz && idx > 0) {
    Domoticz.sendTemperatureCommand(idx, temperature);
  }
}
#endif

#ifdef AFE_CONFIG_HUMIDITY
/* It publishes humidity to Domoticz */
void DomoticzPublishHumidity(unsigned long idx, float humidity) {
  if (Device.configuration.api.domoticz && idx > 0) {
    Domoticz.sendHumidityCommand(idx, humidity);
  }
}
#endif

#if (defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY))
/* It publishes temperature and humidity to Domoticz */
void DomoticzPublishTemperatureAndHumidity(unsigned long idx, float temperature,
                                           float humidity) {
  if (Device.configuration.api.domoticz && idx > 0) {
    Domoticz.sendTemperatureAndHumidityCommand(idx, temperature, humidity);
  }
}

#endif

/* Pir */
#ifdef AFE_CONFIG_HARDWARE_PIR
/* It publishes gate state to Domoticz */
void DomoticzPublishPirState(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    unsigned long idx = Pir[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendPirCommand(idx, Pir[id].motionDetected() ? "On" : "Off");
    }
  }
}
#endif

/* Gate and Contactron */
#ifdef AFE_CONFIG_HARDWARE_GATE
/* It publishes gate state to Domoticz */
void DomoticzPublishGateState(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    unsigned long idx = Gate[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendGateCommand(idx,
                               Gate[id].get() == AFE_GATE_OPEN ? "On" : "Off");
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
/* It publishes contactron state to Domoticz */
void DomoticzPublishContactronState(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    unsigned long idx = Contactron[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendContactronCommand(
          idx, Contactron[id].get() == AFE_CONTACTRON_OPEN ? "On" : "Off");
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void DomoticzPublishParticleSensorData(uint8_t id) {
  if (Device.configuration.api.domoticz) {

    if (ParticleSensor[id].configuration.domoticz.pm25.idx > 0) {
      Domoticz.sendCustomSensorCommand(
          ParticleSensor[id].configuration.domoticz.pm25.idx, ParticleSensor[id].data.pm25);
    }

    if (ParticleSensor[id].configuration.domoticz.pm10.idx > 0) {
      Domoticz.sendCustomSensorCommand(
          ParticleSensor[id].configuration.domoticz.pm10.idx, ParticleSensor[id].data.pm10);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void DomoticzPublishBMEX80SensorData(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    if (BMEX80Sensor[id].configuration.type != AFE_BMP180_SENSOR) {
      if (BMEX80Sensor[id]
              .configuration.domoticz.temperatureHumidityPressure.idx > 0) {
        /* Temperature, Humidity, Pressure */
        Domoticz.sendTemperatureAndHumidityAndPressureCommand(
            BMEX80Sensor[id]
                .configuration.domoticz.temperatureHumidityPressure.idx,
            BMEX80Sensor[id].sensorData.temperature.value,
            BMEX80Sensor[id].sensorData.humidity.value,
            BMEX80Sensor[id].sensorData.pressure.value);
      }

      /* Temperature and Humidity */
      DomoticzPublishTemperatureAndHumidity(
          BMEX80Sensor[id].configuration.domoticz.temperatureHumidity.idx,
          BMEX80Sensor[id].sensorData.temperature.value,
          BMEX80Sensor[id].sensorData.humidity.value);

      /* Humidity */
      DomoticzPublishHumidity(
          BMEX80Sensor[id].configuration.domoticz.humidity.idx,
          BMEX80Sensor[id].sensorData.humidity.value);

      /* DewPoint */
      DomoticzPublishTemperature(
          BMEX80Sensor[id].configuration.domoticz.dewPoint.idx,
          BMEX80Sensor[id].sensorData.dewPoint.value);

      /* HeatIndex */
      DomoticzPublishTemperature(
          BMEX80Sensor[id].configuration.domoticz.heatIndex.idx,
          BMEX80Sensor[id].sensorData.heatIndex.value);
    } // end of excuded for BMP180

    /* Temperature */
    DomoticzPublishTemperature(
        BMEX80Sensor[id].configuration.domoticz.temperature.idx,
        BMEX80Sensor[id].sensorData.temperature.value);

    /* Pressure */
    if (BMEX80Sensor[id].configuration.domoticz.pressure.idx > 0) {
      Domoticz.sendPressureCommand(
          BMEX80Sensor[id].configuration.domoticz.pressure.idx,
          BMEX80Sensor[id].sensorData.pressure.value);
    }

    /* Relative pressure */
    if (BMEX80Sensor[id].configuration.domoticz.relativePressure.idx > 0) {
      Domoticz.sendPressureCommand(
          BMEX80Sensor[id].configuration.domoticz.relativePressure.idx,
          BMEX80Sensor[id].sensorData.relativePressure.value);
    }

    if (BMEX80Sensor[id].configuration.type == AFE_BME680_SENSOR) {
      /* Gas Resistance */
      if (BMEX80Sensor[id].configuration.domoticz.gasResistance.idx > 0) {
        Domoticz.sendCustomSensorCommand(
            BMEX80Sensor[id].configuration.domoticz.gasResistance.idx,
            BMEX80Sensor[id].sensorData.gasResistance.value);
      }

      /* IAQ */
      if (BMEX80Sensor[id].configuration.domoticz.iaq.idx > 0) {
        Domoticz.sendCustomSensorCommand(
            BMEX80Sensor[id].configuration.domoticz.iaq.idx,
            BMEX80Sensor[id].sensorData.iaq.value);
      }

      /* Static IAQ */
      if (BMEX80Sensor[id].configuration.domoticz.staticIaq.idx > 0) {
        Domoticz.sendCustomSensorCommand(
            BMEX80Sensor[id].configuration.domoticz.staticIaq.idx,
            BMEX80Sensor[id].sensorData.staticIaq.value);
      }

      /* CO2 */
      if (BMEX80Sensor[id].configuration.domoticz.co2Equivalent.idx > 0) {
        Domoticz.sendCustomSensorCommand(
            BMEX80Sensor[id].configuration.domoticz.co2Equivalent.idx,
            BMEX80Sensor[id].sensorData.co2Equivalent.value);
      }

      /* BVOC */
      if (BMEX80Sensor[id].configuration.domoticz.breathVocEquivalent.idx > 0) {
        Domoticz.sendCustomSensorCommand(
            BMEX80Sensor[id].configuration.domoticz.breathVocEquivalent.idx,
            BMEX80Sensor[id].sensorData.breathVocEquivalent.value);
      }
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void DomoticzPublishLightLevel(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    if (BH1750Sensor[id].configuration.domoticz.idx > 0) {
      Domoticz.sendSValueCommand(BH1750Sensor[id].configuration.domoticz.idx,
                                 BH1750Sensor[id].currentLightLevel);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void DomoticzPublishAnalogInputData(ADCINPUT_DATA data) {
  if (Device.configuration.api.domoticz) {
    ADCINPUT_DOMOTICZ domoticz;
    AnalogInput.getDomoticzIDX(&domoticz);

    if (domoticz.raw > 0) {
      Domoticz.sendCustomSensorCommand(domoticz.raw, data.raw);
    }
    if (domoticz.percent > 0) {
      Domoticz.sendCustomSensorCommand(domoticz.percent, data.percent);
    }
    if (domoticz.voltage > 0) {
      Domoticz.sendCustomSensorCommand(domoticz.voltage,
                                       data.voltageCalculated);
    }
    if (domoticz.voltageCalculated > 0) {
      Domoticz.sendCustomSensorCommand(domoticz.voltageCalculated,
                                       data.voltageCalculated);
    }
  }
}
#endif
