/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
    /*
    #ifdef DEBUG
        Serial << endl << "API: Domoticz initialized";
    #endif
    */
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
#ifdef CONFIG_TEMPERATURE

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
#endif

#ifdef CONFIG_HUMIDITY
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
#endif

#if (defined(CONFIG_TEMPERATURE) && defined(CONFIG_HUMIDITY))
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

#ifdef CONFIG_HARDWARE_HPMA115S0
void DomoticzPublishParticleSensorData(HPMA115S0_DATA data) {
  if (Device.configuration.domoticzAPI) {
    HPMA115S0_DOMOTICZ idx;
    ParticleSensor.getDomoticzIDX(&idx);
    if (idx.pm25 > 0) {
      Domoticz.sendCustomSensorCommand(idx.pm25, data.pm25);
    }

    if (idx.pm10 > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx.pm10, data.pm10);
    }
  }
}
#endif

#ifdef CONFIG_HARDWARE_BMX80
void DomoticzPublishBMx80SensorData(BMx80_DATA data) {
  if (Device.configuration.domoticzAPI) {
    BMx80_DOMOTICZ idx;
    BMx80Sensor.getDomoticzIDX(&idx);
    if (Device.configuration.isBMx80 != TYPE_BMP180_SENSOR &&
        idx.temperatureHumidityPressure > 0) {
      Domoticz.sendTemperatureAndHumidityAndPressureCommand(
          idx.temperatureHumidityPressure, data.temperature, data.humidity,
          data.pressure);
    }
    if (Device.configuration.isBMx80 == TYPE_BME680_SENSOR &&
        idx.gasResistance > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx.gasResistance, data.gasResistance);
    }
    if (idx.temperature > 0) {
      delay(10);
      Domoticz.sendTemperatureCommand(idx.temperature, data.temperature);
    }
    if (Device.configuration.isBMx80 != TYPE_BMP180_SENSOR &&
        idx.humidity > 0) {
      delay(10);
      Domoticz.sendHumidityCommand(idx.humidity, data.humidity);
    }
    if (idx.pressure > 0) {
      delay(10);
      Domoticz.sendPressureCommand(idx.pressure, data.pressure);
    }
  }
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
void DomoticzPublishLightLevel(float lux) {
  if (Device.configuration.domoticzAPI) {
    unsigned long idx = BH1750Sensor.getDomoticzIDX();

    if (idx > 0) {
      Domoticz.sendSValueCommand(idx, lux);
    }
  }
}
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
void DomoticzPublishAnalogInputData(ADCINPUT_DATA data) {
  if (Device.configuration.domoticzAPI) {
    ADCINPUT_DOMOTICZ idx;
    AnalogInput.getDomoticzIDX(&idx);

    if (idx.raw > 0) {
      Domoticz.sendCustomSensorCommand(idx.raw, data.raw);
    }
    if (idx.percent > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx.percent, data.percent);
    }
    if (idx.voltage > 0) {
      delay(10);
      Domoticz.sendCustomSensorCommand(idx.voltage, data.voltage);
    }
  }
}
#endif
