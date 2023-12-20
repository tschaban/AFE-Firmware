#include "AFE-Firmware-Main.h"

void setup() {

  boolean _success = false;

#ifdef DEBUG
  Serial.begin(AFE_CONFIG_SERIAL_SPEED);
  delay(10);
  Firmware->Debugger->printHeader(2,0);
  Firmware->Debugger->printInformation(
      F("All classes and global variables initialized"), F("BOOT"));
  Firmware->Debugger->printInformation(F("Initializing device"), F("BOOT"));
  Firmware->Debugger->getESPHardwareInformation();
  Firmware->Debugger->printHeader(1, 1);
#endif

#ifdef AFE_ESP32
#else // ESP8266
// Erase all config
#ifdef DEBUG
  Firmware->Debugger->printInformation(
      F("Internally stored configuration: erased: "), F("ESP"));
#endif
  _success = ESP.eraseConfig();
#ifdef DEBUG
  if (_success) {
    Firmware->Debugger->printValue(F("success"), 0, 1);
  } else {
    Firmware->Debugger->printValue(F("FAILURE"), 0, 1);
  }
#endif
#endif // ESP32/ESP8266

#ifdef DEBUG
  Firmware->Debugger->printInformation(F("Configuring global Objects"),
                                       F("BOOT"));
#endif

  Firmware->begin();

/**
 * @brief Initializing system LED (if exists) and turning it on
 *
 */
#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->initializeSystemLED();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef DEBUG
  Firmware->Debugger->printInformation(F("Checking, if WiFi was configured"),
                                       F("WIFI"));
#endif
  if (Firmware->Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Firmware->Debugger->printValue(F("... No"), 0);
#endif
  } else {
/**
 * @brief Checking if the firmware has been upgraded
 *
 */
#ifdef DEBUG
    Firmware->Debugger->printValue(F("... Yes"), 0);
    Firmware->Debugger->printInformation(
        F("Checking if firmware should be upgraded?"), F("FIRMWARE"));
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
    attacheInteruptToAnenoMeter();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER


#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    attacheInteruptToRainMeter();
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
  Firmware->Debugger->getFreeMemorySize();
  Firmware->Debugger->printHeader(2, 1, 72, AFE_DEBUG_HEADER_TYPE_HASH);
  Firmware->Debugger->printValue(F("BOOTING COMPLETED"));
  Firmware->Debugger->printValue(F("Starting ..."), 1);
  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
    Firmware->Debugger->printValue(F("Mode:  Operating"), 1);
  } else {
    Firmware->Debugger->printValue(F("Mode: Configuration mode"), 1);
  }

 Firmware->Debugger->printHeader();

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
}
