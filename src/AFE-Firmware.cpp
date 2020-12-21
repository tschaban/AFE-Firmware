#include "AFE-Firmware.h"

void setup() {

#ifdef DEBUG
  Serial.begin(AFE_CONFIG_SERIAL_SPEED);
  // Serial.setDebugOutput(true);
  delay(10);
#endif

#ifdef DEBUG
  Serial << endl
         << endl
         << F("################################ BOOTING "
              "################################")
         << endl
         << F("INFO: All classes and global variables initialized") << endl
         << F("INFO: Initializing device") << endl;
#endif

  // Erase all config
  ESP.eraseConfig();
#ifdef DEBUG
  Serial << F("INFO: ESP Config erased");
#endif

  /* Initializing SPIFFS file system */
  if (SPIFFS.begin()) {
#ifdef DEBUG
    Serial << F("INFO: File system: mounted");
  } else {
    Serial << F("ERROR: File system not mounted");
#endif
    yield();
  }

  Device.begin();

/* Checking if the device is launched for a first time. If so it loades
 * default configuration */
#ifdef DEBUG
  Serial << endl << F("INFO: Checking if first time launch ... ");
#endif

  if (Device.getMode() == AFE_MODE_FIRST_TIME_LAUNCH) {
#ifdef DEBUG
    Serial << F("YES");
#endif
    Device.setDevice();
    Device.begin();
  }
#ifdef DEBUG
  else {
    Serial << F("NO");
  }
#endif

/* Initializing MCP23017 expanders */
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  initializeMCP23017();
#endif

/* Initializing system LED (if exists) and turning it on */
#ifdef AFE_CONFIG_HARDWARE_LED
  initializeLED();
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Checking, if WiFi was configured: ");
#endif
  if (Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << F("YES");
#endif
  } else {
/* Checking if the firmware has been upgraded */
#ifdef DEBUG
    Serial << F("NO") << endl
           << F("INFO: Checking if firmware should be upgraded?");
#endif
    AFEUpgrader *Upgrader = new AFEUpgrader(&Data, &Device);

    if (Upgrader->upgraded()) {
#ifdef DEBUG
      Serial << endl << F("WARN: Firmware is not up2date. Upgrading...");
#endif
      Upgrader->upgrade();
#ifdef DEBUG
      Serial << endl << F("INFO: Firmware upgraded");
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("INFO: Firmware is up2date");
    }
#endif
    delete Upgrader;
    Upgrader = NULL;

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.getMode() == AFE_MODE_NORMAL) {
      /* Initializing relay */
      initializeRelay();
#ifdef DEBUG
      Serial << endl << F("INFO: Relay(s) initialized");
#endif
    }
#endif
  }

/* Initialzing network */
#ifdef AFE_CONFIG_HARDWARE_LED
  Network.begin(Device.getMode(), &Device, &Data, &Led);
#else
  Network.begin(Device.getMode(), &Device, &Data);
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Network initialized");
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Starting network");
#endif
  Network.listener();

  FirmwarePro.begin(&Network);

  /* Initializing HTTP WebServer */
  initializeHTTPServer();

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Initializing switches */
  initializeSwitch();
#endif

  if (Device.getMode() == AFE_MODE_NORMAL) {

/* Initializing Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeContracton();
    }
#endif

/* Initializing Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeGate();
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeRegulator();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeThermalProtector();
#endif

/* Initializing DS18B20  */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
    initializeDS18B20Sensor();
#endif

/* Initializing DHT  */
#ifdef AFE_CONFIG_HARDWARE_DHT
    initializeDHTSensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeHPMA115S0Sensor();
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeBMX80Sensor();
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeBH1750Sensor();
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    if (Device.getMode() == AFE_MODE_NORMAL) {
      initializeAS3935Sensor();
    }
#endif

#if defined(T3_CONFIG)
    /* Initializing PIRs */
    initPIR();
#ifdef DEBUG
    Serial << endl << F("PIR initialized");
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    if (Device.getMode() == AFE_MODE_NORMAL &&
        Device.configuration.isAnalogInput && FirmwarePro.Pro.valid) {
      AnalogInput.begin();
    }
#endif

    /* Initializing APIs */
    initializeMQTTAPI();

/* Initializing Domoticz HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    initializeHTTPDomoticzAPI();
#endif

    /* Initializing HTTP API */
    initializeHTTPAPI();
  }

#if defined(DEBUG) && defined(AFE_CONFIG_HARDWARE_I2C)
  /* Scanning I2C for devices */
  if (Device.getMode() == AFE_MODE_NORMAL) {
    I2CScanner.begin();
    I2CScanner.scanAll();
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  Led.off();
  /* If device in configuration mode then it starts LED blinking */
  if (Device.getMode() == AFE_MODE_ACCESS_POINT ||
      Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
    Led.blinkingOn(100);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  if (Device.getMode() == AFE_MODE_NORMAL) {
    initializeAnemometerSensor();
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  if (Device.getMode() == AFE_MODE_NORMAL) {
    initializeRainSensor();
  }
#endif

#ifdef DEBUG
  Serial << endl
         << F("################################################################"
              "########")
         << endl
         << F("#                           BOOTING COMPLETED                   "
              "       #")
         << endl
         << F("#                            STARTING DEVICE                    "
              "       #");
  if (Device.getMode() != AFE_MODE_NORMAL) {
    Serial << endl
           << F("#                           CONFIGURATION MODE                "
                "         #");
  }

  Serial << endl
         << F("################################################################"
              "########");
#endif
}

void loop() {

  if (Device.getMode() == AFE_MODE_NORMAL ||
      Device.getMode() == AFE_MODE_CONFIGURATION) {
    if (Network.connected()) {
      if (Device.getMode() == AFE_MODE_NORMAL) {

        /* Triggerd when connectes/reconnects to WiFi */
        eventsListener();

        /* If MQTT API is on it listens for MQTT messages. If the device is
         * not connected to MQTT Broker, it connects the device to it */
        if (Device.configuration.api.mqtt) {
          MqttAPI.listener();
        }

        /* Listens for HTTP requsts. Both for configuration panel HTTP
         * requests or HTTP API requests if it's turned on */
        WebServer.listener();

        /* Checking if there was received HTTP API Command */
        HttpAPI.listener();

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
        contractonEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
        gateEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
        HPMA115S0SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
        BMX80SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
        BH1750SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
        AS3935SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
        analogInputEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
        windSensorListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
        rainSensorListener();
#endif

#if defined(T3_CONFIG)
        mainPIR();
#endif

        /* Checking if Key is still valid */
        FirmwarePro.listener();

      } else { /* Device runs in configuration mode over WiFi */
#ifdef AFE_CONFIG_HARDWARE_LED
        if (!Led.isBlinking()) {
          Led.blinkingOn(100);
        }
#endif
        WebServer.listener();
      }
    }

#ifdef AFE_CONFIG_HARDWARE_LED
    else {
      if (Device.getMode() == AFE_MODE_CONFIGURATION && Led.isBlinking()) {
        Led.blinkingOff();
      }
    }
#endif
    yield();
    Network.listener();

    /** Here: Code that will be run no matter if connected or disconnected from
     * Network / MQTT Broker
     * Works for device in Normal or Configuration mode: (excluding: HotSpot
     * mode) */

    if (Device.getMode() == AFE_MODE_NORMAL) {
#ifdef AFE_CONFIG_HARDWARE_DS18B20
      DS18B20SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
      DHTSensorEventsListener();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
      relayEventsListener();
#endif
    }

  } else { /* Deviced runs in Access Point mode */
    WebServer.listener();
  }

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Listens and processes switch events */
  switchEventsListener();
  processSwitchEvents();
#endif

/* Led listener */
#ifdef AFE_CONFIG_HARDWARE_LED
  Led.loop();
#endif

/* Debug information */
#if defined(DEBUG)
  if (Device.getMode() == AFE_MODE_NORMAL) {
    //  debugListener();
  }
#endif
}
