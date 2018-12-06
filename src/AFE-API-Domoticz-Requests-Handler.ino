/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
  }
}

#if !defined(T5_CONFIG) // Not required for T5
/* It publishes relay state to Domoticz */
void DomoticzPublishRelayState(uint8_t id) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = Relay[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendSwitchCommand(idx,
                                 Relay[id].get() == RELAY_ON ? "On" : "Off");
    }
  }
}
#endif

/* Temperature and Humiditity sensors */
#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)

/* It publishes temperature to Domoticz */
void DomoticzPublishTemperature(float temperature) {

  if (Device.configuration.domoticzAPI) {
#if defined(T1_CONFIG)
    unsigned long idx = Sensor.getDomoticzIDX();
#elif defined(T2_CONFIG) || defined(T5_CONFIG)
    unsigned long idx = Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE);
#endif

    if (idx > 0) {
      Domoticz.sendTemperatureCommand(idx, temperature);
    }
  }
}

#if defined(T2_CONFIG) || defined(T5_CONFIG)
/* It publishes humidity to Domoticz */
void DomoticzPublishHumidity(float humidity) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = Sensor.getDomoticzIDX(IDX_TYPE_HUMIDITY);
    if (idx > 0) {
      Domoticz.sendHumidityCommand(idx, humidity);
    }
  }
}

/* It publishes temperature and humidity to Domoticz */
void DomoticzPublishTemperatureAndHumidity(float temperature, float humidity) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx =
        Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE_AND_HUMIDITY);
    if (idx > 0) {
      Domoticz.sendTemperatureAndHumidityCommand(idx, temperature, humidity);
    }
  }
}
#endif

#endif

/* Pir */
#if defined(T3_CONFIG)
/* It publishes gate state to Domoticz */
void DomoticzPublishPirState(uint8_t id) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = Pir[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendPirCommand(idx, Pir[id].motionDetected() ? "On" : "Off");
    }
  }
}
#endif

/* Gate and Contactron */
#if defined(T5_CONFIG)
/* It publishes gate state to Domoticz */
void DomoticzPublishGateState() {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = Gate.getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendGateCommand(idx, Gate.get() == GATE_OPEN ? "On" : "Off");
    }
  }
}

/* It publishes contactron state to Domoticz */
void DomoticzPublishContactronState(uint8_t id) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = Gate.Contactron[id].getDomoticzIDX();
    if (idx > 0) {
      Domoticz.sendContactronCommand(
          idx, Gate.Contactron[id].get() == CONTACTRON_OPEN ? "On" : "Off");
    }
  }
}
#endif

#if defined(T6_CONFIG)
/* It publishes gate state to Domoticz */
void DomoticzPublishParticleSensorData(HPMA115S0_DATA data) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = ParticleSensor.getDomoticzIDX(HPMA115S0_TYPE_PM25);
    if (idx > 0) {
      Domoticz.sendCustomSensorCommand(idx, data.pm25);
    }
    idx = ParticleSensor.getDomoticzIDX(HPMA115S0_TYPE_PM10);
    if (idx > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx, data.pm10);
    }
  }
}

void DomoticzPublishBME680SensorData(BME680_DATA data) {
  if (Device.configuration.domoticzAPI) {
    BME680_DOMOTICZ idx;
    BME680Sensor.getDomoticzIDX(&idx);
    if (idx.temperatureAndHumidity > 0) {
      Domoticz.sendTemperatureAndHumidityAndPressureCommand(
          idx.temperatureAndHumidity, data.temperature, data.humidity,
          data.pressure);
    }
    if (idx.gasResistance > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx.gasResistance, data.gasResistance);
    }
  }
}

#endif
