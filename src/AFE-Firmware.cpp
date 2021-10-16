#include "AFE-Firmware.h"

void setup() {

#ifdef DEBUG
  Serial.begin(AFE_CONFIG_SERIAL_SPEED);
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

#ifndef AFE_ESP32 /* ESP82xx */

  Serial << endl << "INFO: ESP: ID " << ESP.getFlashChipId();
  Serial << endl << "INFO: ESP: Real flash size: ";
  if (ESP.getFlashChipRealSize() >= 1048576) {
    Serial << (ESP.getFlashChipRealSize() / 1048576) << " Mbits";
  } else {
    Serial << (ESP.getFlashChipRealSize() / 1024) << " Kbits";
  }

  Serial << endl << "INFO: ESP: Flesh size: ";
  if (ESP.getFlashChipSize() >= 1048576) {
    Serial << (ESP.getFlashChipSize() / 1048576) << " Mbits";
  } else {
    Serial << (ESP.getFlashChipSize() / 1024) << " Kbits";
  }

  Serial << endl
         << "INFO: ESP: Speed " << (ESP.getFlashChipSpeed() / 1000000)
         << " MHz";
  Serial << endl << "INFO: ESP: Mode " << ESP.getFlashChipMode() << endl;

#else  /* ESP32 */
// @TODO ESP32
#endif // ESP32
#endif // DEBUG

#ifdef AFE_ESP32
#else // ESP8266
  // Erase all config
  ESP.eraseConfig();
#ifdef DEBUG
  Serial << F("INFO: ESP: Erasing internally stored configuration") << endl;
#endif
#endif // ESP32/ESP8266

/* Initializing SPIFFS file system */
#if AFE_FILE_SYSTEM_USED == AFE_FS_LITTLEFS
  bool _fileSystemReady = LITTLEFS.begin();
#else
  bool _fileSystemReady = SPIFFS.begin();
#endif

  if (_fileSystemReady) {
#ifdef DEBUG
    Serial << F("INFO: FILES SYSTEM: Mounted. Performs a quick garbage "
                "collection operation on SPIFFS");
  } else {
    Serial << F("ERROR:  FILES SYSTEM: NOT mounted");
#endif

#if AFE_FILE_SYSTEM_USED == AFE_FS_SPIFFS
    yield();
    SPIFFS.gc();
#endif
  }

  Device.begin();

/* Checking if the device is launched for a first time. If so it loades
 * default configuration */
#ifdef DEBUG
  Serial << endl << F("INFO: FIRMWARE: Checking if first time launch ... ");
#endif

  if (Device.getMode() == AFE_MODE_FIRST_TIME_LAUNCH) {
#ifdef DEBUG
    Serial << F("YES");
#endif
    if (Device.setDevice()) {
      Device.begin();
    } else {
      Device.reboot();
    }
  }
#ifdef DEBUG
  else {
    Serial << F("NO");
  }
#endif

/* Initializing MCP23017 expanders */
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  initializeMCP23017();
#endif // AFE_CONFIG_HARDWARE_MCP23017

/* Initializing system LED (if exists) and turning it on */
#ifdef AFE_CONFIG_HARDWARE_LED
  initializeLED();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_CLED
  initializeCLed();
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Checking, if WiFi was configured: ");
#endif
  if (Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << F("YES");
#endif
  } else {
/* Checking if the firmware has been upgraded */
#ifdef DEBUG
    Serial << F("NO") << endl
           << F("INFO: FIRMWARE: Checking if firmware should be upgraded?");
#endif
    AFEUpgrader *Upgrader = new AFEUpgrader(&Data, &Device);

    if (Upgrader->upgraded()) {
#ifdef DEBUG
      Serial << endl << F("WARN: FIRMWARE: Up2date. Upgrading...");
#endif
      Upgrader->upgrade();
#ifdef DEBUG
      Serial << endl << F("INFO: FIRMWARE: Upgraded");
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("INFO: FIRMWARE: up2date");
    }
#endif
    delete Upgrader;
    Upgrader = NULL;

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.getMode() == AFE_MODE_NORMAL) {
      /* Initializing relay */
      initializeRelay();
#ifdef DEBUG
      Serial << endl << F("INFO: BOOT: Relay initialized");
#endif
    }
#endif // AFE_CONFIG_HARDWARE_RELAY
  }

/* Initialzing network */
#ifdef AFE_CONFIG_HARDWARE_LED
  Network.begin(Device.getMode(), &Device, &Data, &Led);
#else
  Network.begin(Device.getMode(), &Device, &Data);
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Network initialized");
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Starting network");
#endif
  Network.listener();

/* Initializing I2C BUS */
#ifdef AFE_CONFIG_HARDWARE_I2C
  initializeI2CBUS();
#endif // ESP_CONFIG_HARDWARE_I2C

/* Initializating REST API */
#ifdef AFE_CONFIG_HARDWARE_LED
  RestAPI.begin(&Data, &Device, &Led);
#else
  RestAPI.begin(&Data, &Device);
#endif // AFE_CONFIG_HARDWARE_LED

  /* Initializing FirmwarePro */
  FirmwarePro.begin(&Data, &RestAPI);

  /* Initializing HTTP WebServer */
  initializeHTTPServer();

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Initializing switches */
  initializeSwitch();
#endif // AFE_CONFIG_HARDWARE_SWITCH

  if (Device.getMode() == AFE_MODE_NORMAL) {

/* Initializing Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    initializeContracton();
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

/* Initializing Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
    initializeGate();
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeRegulator();
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeThermalProtector();
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* Initializing DS18B20  */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
    initializeDS18B20Sensor();
#endif // AFE_CONFIG_HARDWARE_DS18B20

/* Initializing DHT  */
#ifdef AFE_CONFIG_HARDWARE_DHT
    initializeDHTSensor();
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    initializeHPMA115S0Sensor();
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    initializeBMX80Sensor();
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
    initializeBH1750Sensor();
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    initializeTSL2561Sensor();
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
    initializeAS3935Sensor();
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    initializeBinarySensor();
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    initializeAnemometer();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    initializeRainmeter();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    initializeADC();
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
    initializePN532Sensor();
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

    /* Initializing APIs */
    initializeMQTTAPI();

/* Initializing Domoticz HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    initializeHTTPDomoticzAPI();
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

    /* Initializing HTTP API */
    initializeHTTPAPI();
  }

/* End of initialization for operating mode. Initialization for all devices
 * modes */

#ifdef AFE_CONFIG_HARDWARE_LED
  Led.off();
  /* If device in configuration mode then it starts LED blinking */
  if (Device.getMode() == AFE_MODE_ACCESS_POINT ||
      Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
    Led.blinkingOn(100);
  }
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef DEBUG
  Serial << endl
         << F("############################################################"
              "####"
              "########")
         << endl
         << F("#                           BOOTING COMPLETED               "
              "    "
              "       #")
         << endl
         << F("#                            STARTING DEVICE                "
              "    "
              "       #");
  if (Device.getMode() != AFE_MODE_NORMAL) {
    Serial << endl
           << F("#                           CONFIGURATION MODE            "
                "    "
                "         #");
  }

  Serial << endl
         << F("############################################################"
              "####"
              "########");
#ifndef AFE_ESP32
  Serial << endl
         << "INFO: MEMORY: Free: [Boot end] : "
         << String(system_get_free_heap_size() / 1024) << "kB";
#endif
#endif
}

void loop() {

  if (Device.getMode() == AFE_MODE_NORMAL ||
      Device.getMode() == AFE_MODE_CONFIGURATION) {
    if (Network.connected()) {
      if (Device.getMode() == AFE_MODE_NORMAL) {

        /* If MQTT API is on it listens for MQTT messages. If the device is
         * not connected to MQTT Broker, it connects the device to it */
        if (Device.configuration.api.mqtt) {
          MqttAPI.listener();
        }

        /* Listens for HTTP requsts. Both for configuration panel HTTP
         * requests or HTTP API requests if it's turned on */
        HTTPServer.listener();

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

#ifdef AFE_CONFIG_HARDWARE_TSL2561
        TSL2561SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
        AS3935SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
        analogInputEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
        anemometerEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
        rainmeterEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
        binarySensorEventsListener();
#endif

/* Listenings and processing PN532 events */
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
        PN532EventsListener();
#endif

        /* Checking if Key is still valid */
        FirmwarePro.listener();

      } else { /* Device runs in configuration mode over WiFi */
#ifdef AFE_CONFIG_HARDWARE_LED
        if (!Led.isBlinking()) {
          Led.blinkingOn(100);
        }
#endif

        HTTPServer.listener();
      }
    }

#ifdef AFE_CONFIG_HARDWARE_LED
    else {
      if (Device.getMode() == AFE_MODE_CONFIGURATION && Led.isBlinking()) {
        Led.blinkingOff();
      }
    }
#endif
    //yield();
    Network.listener();

    /** Here: Code that will be run no matter if connected or disconnected
     * from
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

#ifdef AFE_CONFIG_HARDWARE_CLED
      CLedEventsListener();
#endif
    }

    /* Trigger actions triggered by WiFi: connected/disconnected or MQTT
     * Connected */
    eventsListener();

  } else { /* Deviced runs in Access Point mode */
    HTTPServer.listener();
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
#ifdef DEBUG
  if (Device.getMode() == AFE_MODE_NORMAL) {
    //  debugListener();
  }
#endif // DEBUG
}
