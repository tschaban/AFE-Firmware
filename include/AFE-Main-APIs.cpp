/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include <AFE-Configuration.h>

#include <AFE-API-HTTP.h>
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#include <AFE-API-MQTT-Standard.h>
#endif

/* ---------Headers ---------*/

void initializeMQTTAPI(void);
void initializeHTTPAPI(void);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void initializeHTTPDomoticzAPI(void);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* --------- Body -----------*/

AFEAPIHTTP *HttpAPI = new AFEAPIHTTP();
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
AFEAPIMQTTDomoticz *MqttAPI = new AFEAPIMQTTDomoticz();
AFEAPIHTTPDomoticz *HttpDomoticzAPI = new AFEAPIHTTPDomoticz();
#else
AFEAPIMQTTStandard *MqttAPI = new AFEAPIMQTTStandard();
#endif

/* Initializing MQTT API */
void initializeMQTTAPI(void) {
  if (Firmware->Device->getMode() != AFE_MODE_ACCESS_POINT &&
      Firmware->Device->configuration.api.mqtt) {

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: Initializing MQTT");
#endif

    MqttAPI->begin(Firmware, Hardware);

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: MQTT init completed");
#endif
  }
}

/* Initializing HTTP API */
void initializeHTTPAPI(void) {
  if (Firmware->Device->getMode() != AFE_MODE_ACCESS_POINT) {

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: Initializing MQTT");
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    HttpAPI->begin(Firmware, Hardware, HTTPServer, MqttAPI, HttpDomoticzAPI);
#else
    HttpAPI->begin(Firmware, Hardware, HTTPServer, MqttAPI);
#endif

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: HTTP init completed");
#endif
  }
}

/* Initializing Domoticz HTTP API */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

void initializeHTTPDomoticzAPI(void) {
  if (Firmware->Device->getMode() != AFE_MODE_ACCESS_POINT &&
      Firmware->Device->configuration.api.domoticz) {

    HttpDomoticzAPI->begin(Firmware, Hardware);

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: MQTT init completed");
#endif
  }
}

#endif // Domoticz API