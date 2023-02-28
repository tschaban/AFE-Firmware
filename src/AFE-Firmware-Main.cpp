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

  Device->begin();

/**
 * @brief Checking if the device is launched for a first time. If so it loads
 * default configuration
 *
 */
#ifdef DEBUG
  Serial << endl << F("INFO: FIRMWARE: Checking if first time launch ... ");
#endif

  if (Device->getMode() == AFE_MODE_FIRST_TIME_LAUNCH) {
#ifdef DEBUG
    Serial << F("YES");
#endif
    if (Device->setDevice()) {
      Device->begin();
    } else {
      Device->reboot();
    }
#ifdef DEBUG
  } else {
    Serial << F("NO");
#endif
  }
  /**
   * @brief saving information how many times firmare has been rebooted. For
   * debug purpose
   *
   */
  unsigned long _counter = Data->getRebootCounter();
#ifdef DEBUG
  Serial << endl << F("INFO: Firmware rebooted: ") << _counter << F(" times");
#endif

/**
 * @brief Initializing I2C BUS
 *
 */
#ifdef AFE_CONFIG_HARDWARE_I2C
  initializeI2CBUS();
#endif // ESP_CONFIG_HARDWARE_I2C

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
  initializeLED();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_CLED
  if (Device->getMode() != AFE_MODE_ACCESS_POINT) {
    initializeCLed();
  }
#endif // AFE_CONFIG_HARDWARE_CLED

/**
 * @brief Initializating REST API
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  RestAPI->begin(Data, Device, Led);
#else
  RestAPI->begin(Data, Device);
#endif // AFE_CONFIG_HARDWARE_LED

  /**
   * @brief Initializing Firmware Pro and generic config
   *
   */
  FirmwarePro->begin(Data, RestAPI);

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Checking, if WiFi was configured: ");
#endif
  if (Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
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
    AFEUpgrader *Upgrader = new AFEUpgrader(Data, Device, FirmwarePro);

    if (Upgrader->upgraded()) {
      Upgrader->upgrade();
    }
    delete Upgrader;
    Upgrader = NULL;

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device->getMode() == AFE_MODE_NORMAL) {
      /**
       * @brief Initializing relay
       *
       */
      initializeRelay();
    }
#endif // AFE_CONFIG_HARDWARE_RELAY
  }

/**
 * @brief Initialzing network
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  Network->begin(Device->getMode(), Device, Data, Led);
#else
  Network->begin(Device->getMode(), Device, Data);
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Starting network");
#endif
  Network->listener();

  /**
   * @brief Initializing HTTP WebServer
   *
   */
  initializeHTTPServer();

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /**
   * @brief Initializing switches
   *
   */
  initializeSwitch();
#endif // AFE_CONFIG_HARDWARE_SWITCH

  if (Device->getMode() == AFE_MODE_NORMAL) {

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    /**
     * @brief Initializing Contactrons
     *
     */
    initializeContracton();
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
    /**
     * @brief Initializing Gate
     *
     */
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

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
    initializeADC();
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

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

/**
 * @brief End of initialization for operating mode. Initialization for all
 * devices modes
 *
 */

#ifdef AFE_CONFIG_HARDWARE_LED
  Led->off();
  /**
   * @brief If device in configuration mode then it starts LED blinking
   *
   */
  if (Device->getMode() == AFE_MODE_ACCESS_POINT ||
      Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
    Led->blinkingOn(100);
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
  if (Device->getMode() != AFE_MODE_NORMAL) {
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

  Object->Core->Data = Data;
  Object->Core->Device = Device;
  Object->Core->Network = Network;
  Object->Core->RestAPI = RestAPI;
  Object->Core->Firmware = FirmwarePro;
  Object->Core->MqttAPI = MqttAPI;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  Object->Core->HttpDomoticzAPI = HttpDomoticzAPI;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  Object->Hardware->SystemLed = Led;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    Object->Hardware->Relay[i] = &Relay[i];
  }
#endif

  Event->begin(Object);
}

void loop() {

  if (Device->getMode() == AFE_MODE_NORMAL ||
      Device->getMode() == AFE_MODE_CONFIGURATION) {
    if (Network->connected()) {
      if (Device->getMode() == AFE_MODE_NORMAL) {

        /**
         * @brief If MQTT API is on it listens for MQTT messages. If the device
         * is not connected to MQTT Broker, it connects the device to it
         *
         */
        if (Device->configuration.api.mqtt) {
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

/**
 * @brief Listenings and processing PN532 events
 *
 */
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
        PN532EventsListener();
#endif

        /**
         * @brief Checking if Key is still valid
         *
         */
        FirmwarePro->listener();

      } else { /**
       * @brief Device runs in configuration mode over WiFi
       *
       */
#ifdef AFE_CONFIG_HARDWARE_LED
        if (!Led->isBlinking()) {
          Led->blinkingOn(100);
        }
#endif

        HTTPServer->listener();
      }
    }

#ifdef AFE_CONFIG_HARDWARE_LED
    else {
      if (Device->getMode() == AFE_MODE_CONFIGURATION && Led->isBlinking()) {
        Led->blinkingOff();
      }
    }
#endif
    // yield();
    Network->listener();

    /**
     * @brief Here: Code that will be run no matter if connected or disconnected
     * from Network / MQTT Broker Works for device in Normal or Configuration
     * mode: (excluding: HotSpot mode)
     *
     */

    if (Device->getMode() == AFE_MODE_NORMAL) {
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
  Led->loop();
#endif

/**
 * @brief Debug information
 *
 */
#ifdef DEBUG
  if (Device->getMode() == AFE_MODE_NORMAL) {
    //  debugListener();
  }
#endif // DEBUG
}
