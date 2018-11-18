#if defined(T6_CONFIG)

/* Initializing sensor */
void initHPMA115S0Sensor() { ParticleSensor.begin(); }

/* Main code for processing sesnor */
void mainHPMA115S0Sensor() {
  {
    /* Sensor: listener */
    ParticleSensor.listener();
    if (ParticleSensor.isReady()) {
      uint16_t pm25, pm10;
      ParticleSensor.get(&pm25, &pm10);

#ifdef DEBUG
      Serial << endl
             << "Return data from sensor: PM2.5 = " << pm25 << "ug/m3, "
             << "PM10 = " << pm10 << "ug/m3";
#endif
    }
  }
}
#endif
