#include "AFE-Firmware-Main.h"

void setup() {

  boolean _success = false;

#ifdef DEBUG
  Serial.begin(AFE_CONFIG_SERIAL_SPEED);
  delay(10);

  Serial << endl
         << endl
         << F("################################ BOOTING "
              "################################")
         << endl
         << F("INFO: All classes and global variables initialized") << endl
         << F("INFO: Initializing device") << endl;

  getESPInformation();
#endif

#ifdef AFE_ESP32
#else // ESP8266
  // Erase all config
  _success = ESP.eraseConfig();
#ifdef DEBUG

  if (_success) {
    Serial << F("INFO: ESP: Internally stored configuration: erased") << endl;
  } else {
    Serial << F("ERROR: ESP: Internally stored configuration NOT erased")
           << endl;
  }
#endif
#endif // ESP32/ESP8266

/**
 * @brief Initializing file system
 *
 */
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
  _success = LITTLEFS.begin();
#else
  _success = SPIFFS.begin();
#endif
#ifdef DEBUG
  if (_success) {
    Serial << endl
           << F("INFO: FILES SYSTEM: Mounted. Performs a quick garbage "
                "collection operation on SPIFFS");
  } else {
    Serial << endl << F("WARN: FILES SYSTEM: Not mounted") << endl;
  }
#endif

#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
  yield();
  SPIFFS.gc();
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Setting AFE Global Objects");
#endif

  Firmware->begin();

/**
 * @brief Initializing MCP23017 expanders
 *
 */
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  initializeMCP23017();
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

/**
 * @brief Initializing system LED (if exists) and turning it on
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->initializeSystemLED();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Checking, if WiFi was configured: ");
#endif
  if (Firmware->Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << F("NO");
#endif
  } else {
/**
 * @brief Checking if the firmware has been upgraded
 *
 */
#ifdef DEBUG
    Serial << F("YES") << endl
           << F("INFO: FIRMWARE: Checking if firmware should be upgraded?");
#endif
    AFEUpgrader *Upgrader = new AFEUpgrader(Firmware);
    Upgrader->upgraded();
    delete Upgrader;
    Upgrader = NULL;

    /* Initializing: First Hardware item components */
    Hardware->initPriorityHardwareItems();
  }
  /**
   * @brief Initialzing network
   *
   */
  Firmware->initializeNetwork();

  /**
   * @brief Initializing HTTP WebServer
   *
   */
  initializeHTTPServer();

  /* Initializing: Remaining Hardware item components */
  Hardware->initHardwareItems();

  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeRegulator();
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    initializeThermalProtector();
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_HARDWARE_AS3935
    initializeAS3935Sensor();
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    initializeAnemometer();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    initializeRainmeter();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
    initializePN532Sensor();
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

    /**
     * @brief Initializing APIs
     *
     */
    initializeMQTTAPI();

/**
 * @brief Initializing Domoticz HTTP API
 *
 */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    initializeHTTPDomoticzAPI();
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

    /**
     * @brief Initializing HTTP API
     *
     */
    initializeHTTPAPI();
  }

// Firmware->API->REST = RestAPI;

/**
 * @brief Initializing Events handler
 *
 */

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  Event->begin(Firmware, Hardware, MqttAPI, HttpDomoticzAPI);
#else
  Event->begin(Firmware, Hardware, MqttAPI);
#endif

/**
 * @brief End of initialization for operating mode. Initialization for all
 * devices modes
 *
 */

#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->Hardware->SystemLed->off();
  /**
   * @brief If device in configuration mode then it starts LED blinking
   *
   */
  if (Firmware->Device->getMode() == AFE_MODE_ACCESS_POINT ||
      Firmware->Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
    Firmware->Hardware->SystemLed->blinkingOn(100);
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
  if (Firmware->Device->getMode() != AFE_MODE_NORMAL) {
    Serial << endl
           << F("#                           CONFIGURATION MODE            "
                "    "
                "         #");
  }

  Serial << endl
         << F("############################################################"
              "####"
              "########");
  getAvailableMem();
  Serial << F(": Booting completed");

#endif
}

void loop() {

  if (Firmware->Device->getMode() == AFE_MODE_NORMAL ||
      Firmware->Device->getMode() == AFE_MODE_CONFIGURATION) {
    if (Firmware->API->Network->connected()) {
      if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {

        /**
         * @brief If MQTT API is on it listens for MQTT messages. If the
         * device
         * is not connected to MQTT Broker, it connects the device to it
         *
         */
        if (Firmware->Device->configuration.api.mqtt) {
          MqttAPI->listener();
        }

        /**
         * @brief Listens for HTTP requsts. Both for configuration panel HTTP
         * requests or HTTP API requests if it's turned on
         *
         */
        HTTPServer->listener();

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

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
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

#ifdef AFE_CONFIG_HARDWARE_FS3000
        FS3000SensorEventsListener();
#endif


/**
 * @brief Listenings and processing PN532 events
 *
 */
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
        PN532EventsListener();
#endif

      } else { /**
       * @brief Device runs in configuration mode over WiFi
       *
       */
#ifdef AFE_CONFIG_HARDWARE_LED
        if (!Firmware->Hardware->SystemLed->isBlinking()) {
          Firmware->Hardware->SystemLed->blinkingOn(100);
        }
#endif

        HTTPServer->listener();
      }
    }

#ifdef AFE_CONFIG_HARDWARE_LED
    else {
      if (Firmware->Device->getMode() == AFE_MODE_CONFIGURATION &&
          Firmware->Hardware->SystemLed->isBlinking()) {
        Firmware->Hardware->SystemLed->blinkingOff();
      }
    }
#endif
    // yield();
    Firmware->API->Network->listener();

    /**
     * @brief Here: Code that will be run no matter if connected or
     * disconnected
     * from Network / MQTT Broker Works for device in Normal or Configuration
     * mode: (excluding: HotSpot mode)
     *
     */

    if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
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
      RGBLEDEventsListener();
#endif
    }

    /**
     * @brief Processing various events
     *
     */

    Event->listener();

  } else {
    /**
     * @brief Device runs in Access Point mode
     *
     */
    HTTPServer->listener();
  }

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /**
   * @brief Listens and processes switch events
   *
   */
  switchEventsListener();
  processSwitchEvents();
#endif

/**
 * @brief Led listener
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->Hardware->SystemLed->loop();
#endif

/**
 * @brief Debug information
 *
 */
#ifdef DEBUG
  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
    //  debugListener();
  }
#endif // DEBUG
}
