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
    HPMA115S0_DATA data = ParticleSensor[id].get();
    if (ParticleSensor[id].configuration.domoticz.pm25.idx > 0) {
      Domoticz.sendCustomSensorCommand(
          ParticleSensor[id].configuration.domoticz.pm25.idx, data.pm25);
    }

    if (ParticleSensor[id].configuration.domoticz.pm10.idx > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(
          ParticleSensor[id].configuration.domoticz.pm10.idx, data.pm10);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void DomoticzPublishBMEX80SensorData(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    BMEX80_DATA data;
    data = BMEX80Sensor[id].get();
    if (BMEX80Sensor[id].configuration.type != AFE_BMP180_SENSOR &&
        BMEX80Sensor[id].configuration.domoticz.temperatureHumidityPressure.idx >
            0) {
      Domoticz.sendTemperatureAndHumidityAndPressureCommand(
          BMEX80Sensor[id]
              .configuration.domoticz.temperatureHumidityPressure.idx,
          data.temperature, data.humidity, data.pressure);
    }
    if (BMEX80Sensor[id].configuration.type == AFE_BME680_SENSOR &&
        BMEX80Sensor[id].configuration.domoticz.gasResistance.idx > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(
          BMEX80Sensor[id].configuration.domoticz.gasResistance.idx,
          data.gasResistance);
    }

    DomoticzPublishTemperature(
        BMEX80Sensor[id].configuration.domoticz.temperature.idx,
        data.temperature);

    if (BMEX80Sensor[id].configuration.type != AFE_BMP180_SENSOR) {
      DomoticzPublishHumidity(
          BMEX80Sensor[id].configuration.domoticz.humidity.idx, data.humidity);
    }

    if (BMEX80Sensor[id].configuration.domoticz.pressure.idx > 0) {
      delay(10);
      Domoticz.sendPressureCommand(
          BMEX80Sensor[id].configuration.domoticz.pressure.idx, data.pressure);
    }
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void DomoticzPublishLightLevel(uint8_t id) {
  if (Device.configuration.api.domoticz) {
    if (BH1750Sensor[id].configuration.domoticz.idx > 0) {
      Domoticz.sendSValueCommand(BH1750Sensor[id].configuration.domoticz.idx,
                                 BH1750Sensor[id].get());
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
      delay(10);
      Domoticz.sendCustomSensorCommand(domoticz.percent, data.percent);
    }
    if (domoticz.voltage > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(domoticz.voltage,
                                       data.voltageCalculated);
    }
    if (domoticz.voltageCalculated > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(domoticz.voltageCalculated,
                                       data.voltageCalculated);
    }
  }
}
#endif
