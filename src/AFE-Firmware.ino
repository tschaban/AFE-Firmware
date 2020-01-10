/*
AFE Firmware for smarthome devices based on ESP8266/ESP8285 chips

This code combains AFE Firmware versions:
   - T0 and T0 for Shelly-1
   - T1 (DS18B20)
   - T2 (DHTxx)
   - T3 (PIRs)
   - T4 - decommissioned, T0 took over 100% of it's functionality
   - T5 Gate
   - T6 Wheater station

More info: https://afe.smartnydom.pl
LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
*/

#include <AFE-Configuration.h>

/* Includes libraries for debugging in development compilation only */
#ifdef DEBUG
#include <Streaming.h>
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>

#include <AFE-API-HTTP.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#include <AFE-API-MQTT-Standard.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
#endif

/* Shelly-1 device does not have LED. Excluding LED related code */
#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
AFELED Led;
#endif

/* T1 Set up, DS18B20 sensor */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
AFESensorDS18B20 Sensor;
#endif

/* T2 Setup, DHxx sensor */
#ifdef AFE_CONFIG_HARDWARE_DHXX
#include <AFE-Sensor-DHT.h>
#include <PietteTech_DHT.h>
void dht_wrapper();
PietteTech_DHT dht;
AFESensorDHT Sensor;
#endif

#ifdef AFE_CONFIG_TEMPERATURE
float temperature;
#endif

#ifdef AFE_CONFIG_HUMIDITY
float humidity;
#endif

AFEDataAccess Data;
AFEFirmwarePro Firmware;
AFEDevice Device;
AFEWiFi Network;
AFEAPIHTTP HttpAPI;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
AFEAPIMQTTDomoticz MqttAPI;
AFEAPIHTTPDomoticz HttpDomoticzAPI;
#else
AFEAPIMQTTStandard MqttAPI;
#endif

AFEWebServer WebServer;

#ifdef AFE_CONFIG_HARDWARE_SWITCH
AFESwitch Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
AFERelay Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#if defined(T3_CONFIG)
#include <AFE-PIR.h>
AFEPIR Pir[sizeof(Device.configuration.isPIR)];
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
AFEGate Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
GATES_CURRENT_STATE GatesCurrentStates;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Contactron.h>
AFEContactron Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
byte lastPublishedContactronState[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
#endif

#if defined(DEBUG) && defined(AFE_CONFIG_HARDWARE_I2C)
#include <AFE-I2C-Scanner.h>
AFEI2CScanner I2CScanner;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Sensor-BH1750.h>
AFESensorBH1750 BH1750Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-BMEX80.h>
AFESensorBMEX80 BMEX80Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-HPMA115S0.h>
AFESensorHPMA115S0 ParticleSensor[AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0];
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-Sensor-AS3935.h>
AFESensorAS3935 AS3935Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
AFEAnalogInput AnalogInput;
#endif

void setup() {

#ifdef DEBUG
  Serial.begin(AFE_CONFIG_SERIAL_SPEED);
  delay(10);
#endif

#ifdef DEBUG
  Serial << endl
         << endl
         << "################################ BOOTING "
            "################################"
         << endl
         << "INFO: All classes and global variables initialized" << endl
         << "INFO: Initializing device" << endl
         << "INFO: File system: ";
#endif

  /* Initializing SPIFFS file system */
  if (SPIFFS.begin()) {
#ifdef DEBUG
    Serial << "mounted";
  } else {
    Serial << "ERROR: not mounted";
#endif
    yield();
  }

  Device.begin();

/* Checking if the device is launched for a first time. If so it loades
 * default configuration */
#ifdef DEBUG
  Serial << endl
         << "INFO: Checking if first time launch: Device.getMode()= "
         << Device.getMode() << " : ";
#endif

  if (Device.getMode() == AFE_MODE_FIRST_TIME_LAUNCH) {
#ifdef DEBUG
    Serial << "YES";
#endif
    Device.setDevice();
    Device.begin();
  }
#ifdef DEBUG
  else {
    Serial << "NO";
  }
#endif

/* Initializing system LED (if exists) and turning it on */
#ifdef AFE_CONFIG_HARDWARE_LED
  uint8_t systemLedID = Data.getSystemLedID();
  yield();

#ifdef DEBUG
  Serial << endl << "INFO: System LED ID: " << systemLedID;
#endif

  if (systemLedID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Led.begin(systemLedID);
  }
  Led.on();
#ifdef DEBUG
  Serial << endl << "INFO: System LED initialized";
#endif
#endif

#ifdef DEBUG
  Serial << endl << "INFO: Checking, if WiFi hasn't been configured: ";
#endif
  if (Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << "YES";
#endif
  } else {
/* Checking if the firmware has been upgraded */
#ifdef DEBUG
    Serial << "NO" << endl << "INFO: Checking if firmware should be upgraded: ";
#endif
    AFEUpgrader *Upgrader = new AFEUpgrader(&Data, &Device);

    if (Upgrader->upgraded()) {
#ifdef DEBUG
      Serial << endl << "- Firmware is not up2date. Upgrading...";
#endif
      Upgrader->upgrade();
#ifdef DEBUG
      Serial << endl << " - Firmware upgraded";
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << "- Firmware is up2date";
    }
#endif
    delete Upgrader;
    Upgrader = NULL;

#ifdef AFE_CONFIG_HARDWARE_RELAY
    /* Initializing relay */
    initializeRelay();
#ifdef DEBUG
    Serial << endl << "INFO: Relay(s) initialized";
#endif
#endif
  }

  /* Initialzing network */
  Network.begin(Device.getMode(), &Device);
#ifdef DEBUG
  Serial << endl << "INFO: Network initialized";
#endif

#ifdef DEBUG
  Serial << endl << "INFO: Starting network";
#endif
  Network.listener();

  /* Initializing HTTP WebServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.handleFirmwareUpgrade("/upgrade", handleHTTPRequests, handleUpload);
  if (Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
    WebServer.onNotFound(handleOnNotFound);
  }

  /* Initializing Firmware: version PRO */
  Firmware.begin();

  WebServer.begin(&Device, &Firmware);
#ifdef AFE_CONFIG_HARDWARE_LED
  WebServer.initSystemLED(&Led);
#endif

#ifdef DEBUG
  Serial << endl << "INFO: WebServer initialized";
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Initializing switches */
  initializeSwitch();
#endif

  if (Device.getMode() == AFE_MODE_NORMAL) {

/* Initializing Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    initializeContractons();
#endif

/* Initializing Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
    initializeGate();
#endif

/* Initializing DS18B20 or DHTxx sensor */
#if defined(AFE_CONFIG_HARDWARE_DS18B20) || defined(AFE_CONFIG_HARDWARE_DHXX)
    initalizeSensor();
#ifdef DEBUG
    Serial << endl << "Sensors initialized";
#endif
#endif

/* Initializing T6 sensor */
#ifdef AFE_CONFIG_HARDWARE_I2C
    Wire.begin();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    initializeHPMA115S0Sensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    initializeBMEX80Sensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    initializeBH1750Sensor();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    initializeAS3935Sensor();
#endif

#if defined(T3_CONFIG)
    /* Initializing PIRs */
    initPIR();
#ifdef DEBUG
    Serial << endl << "PIR initialized";
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    if (Device.configuration.isAnalogInput && Firmware.Pro.valid) {
      AnalogInput.begin();
    }
#endif

    /* Initializing APIs */
    initializeMQTTAPI();

/* Initializing HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    initializeHTTPDomoticzAPI();
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI, &HttpDomoticzAPI);
#else
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    HttpAPI.addClass(&Relay[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    HttpAPI.addClass(&AnalogInput);
#endif
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

#ifdef DEBUG
  Serial << endl
         << "########################### BOOTING COMPLETED "
            "###########################"
         << endl
         << endl
         << "###########################  STARTING DEVICE  "
            "###########################";
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

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
        relayEventsListener();
#endif

#if defined(AFE_CONFIG_HARDWARE_DS18B20) || defined(AFE_CONFIG_HARDWARE_DHXX)
        /* Sensor: DS18B20 or DHT related code */
        mainSensor();
#endif

/* Sensor: HPMA115S0 related code  */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
        HPMA115S0SensorEventsListener();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
        BMEX80SensorEventsListener();
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

#if defined(T3_CONFIG)
        mainPIR();
#endif

        /* Checking if Key is still valid */
        Firmware.listener();

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
    Network.listener();
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
    debugListener();
  }
#endif
}
