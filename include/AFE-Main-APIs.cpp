/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include <AFE-API-HTTP.h>
#include <AFE-Configuration.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else // Standards and Home Assistant API
#include <AFE-API-MQTT-Standard.h>
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
#include <AFE-API-HomeAssistant-Integration.h>
#endif
#endif

/* ---------Headers ---------*/

void initializeMQTTAPI(void);
void initializeHTTPAPI(void);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void initializeHTTPDomoticzAPI(void);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* --------- Body -----------*/

AFEAPIHTTP HttpAPI;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
AFEAPIMQTTDomoticz MqttAPI;
AFEAPIHTTPDomoticz HttpDomoticzAPI;
#else
AFEAPIMQTTStandard MqttAPI;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
AFEAPIHomeAssistantIntegration *HomeAssistantDiscoveryAPI = new AFEAPIHomeAssistantIntegration();
#endif // Home Assistant
#endif

/* Initializing MQTT API */
void initializeMQTTAPI(void) {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.mqtt) {

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: Initializing MQTT");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    MqttAPI.begin(&Data, &Device, &Led);
#else
    MqttAPI.begin(&Data, &Device);
#endif

/* Inititializing Home Assistant Discovery */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
    HomeAssistantDiscoveryAPI->begin(&Data, &Device, &MqttAPI);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.configuration.noOfRelays > 0) {
      MqttAPI.addClass(&Relay[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    if (Device.configuration.noOfSwitches > 0) {
      MqttAPI.addClass(&Switch[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
    if (Device.configuration.noOfAnalogInputs > 0) {
      MqttAPI.addClass(&AnalogInput[0]);
    }
#else  // ESP8266
    if (Device.configuration.isAnalogInput) {
      MqttAPI.addClass(&AnalogInput);
    }
#endif // AFE_ESP32
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    if (Device.configuration.noOfBMEX80s > 0) {
      MqttAPI.addClass(&BMEX80Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    if (Device.configuration.noOfHPMA115S0s > 0) {
      MqttAPI.addClass(&HPMA115S0Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    if (Device.configuration.noOfBH1750s > 0) {
      MqttAPI.addClass(&BH1750Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    if (Device.configuration.noOfAS3935s > 0) {
      MqttAPI.addClass(&AS3935Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    if (Device.configuration.noOfAnemometerSensors > 0) {
      MqttAPI.addClass(&AnemometerSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    if (Device.configuration.noOfRainmeterSensors > 0) {
      MqttAPI.addClass(&RainSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Device.configuration.noOfGates > 0) {
      MqttAPI.addClass(&Gate[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    if (Device.configuration.noOfContactrons > 0) {
      MqttAPI.addClass(&Contactron[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    if (Device.configuration.noOfDS18B20s > 0) {
      MqttAPI.addClass(&DS18B20Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    if (Device.configuration.noOfDHTs > 0) {
      MqttAPI.addClass(&DHTSensor[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    if (Device.configuration.noOfRegulators > 0) {
      MqttAPI.addClass(&Regulator[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    if (Device.configuration.noOfThermalProtectors > 0) {
      MqttAPI.addClass(&ThermalProtector[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    if (Device.configuration.noOfBinarySensors > 0) {
      MqttAPI.addClass(&BinarySensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device.configuration.noOfPN532Sensors > 0) {
      MqttAPI.addClass(&PN532Sensor[0]);
    }
#endif
    if (Device.configuration.noOfMiFareCards > 0) {
      MqttAPI.addClass(&MiFareCard[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
    if (Device.configuration.noOfCLEDs > 0 ) {
      MqttAPI.addClass(&CLEDStrip);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    if (Device.configuration.noOfTSL2561s > 0) {
      MqttAPI.addClass(&TSL2561Sensor[0]);
    }
#endif

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: MQTT init completed");
#endif
  }
}

/* Initializing HTTP API */
void initializeHTTPAPI(void) {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT) {

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: Initializing MQTT");
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    HttpAPI.begin(&Device, &HTTPServer, &Data, &MqttAPI, &HttpDomoticzAPI);
#else
    HttpAPI.begin(&Device, &HTTPServer, &Data, &MqttAPI);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.configuration.noOfRelays > 0) {
      HttpAPI.addClass(&Relay[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
    if (Device.configuration.noOfAnalogInputs > 0) {
      HttpAPI.addClass(&AnalogInput[0]);
    }
#else  // ESP8266
    if (Device.configuration.isAnalogInput) {
      HttpAPI.addClass(&AnalogInput);
    }
#endif // AFE_ESP32
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    if (Device.configuration.noOfBMEX80s > 0) {
      HttpAPI.addClass(&BMEX80Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    if (Device.configuration.noOfHPMA115S0s > 0) {
      HttpAPI.addClass(&HPMA115S0Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    if (Device.configuration.noOfBH1750s > 0) {
      HttpAPI.addClass(&BH1750Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    if (Device.configuration.noOfAS3935s > 0) {
      HttpAPI.addClass(&AS3935Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    if (Device.configuration.noOfAnemometerSensors > 0) {
      HttpAPI.addClass(&AnemometerSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    if (Device.configuration.noOfRainmeterSensors > 0) {
      HttpAPI.addClass(&RainSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Device.configuration.noOfGates > 0) {
      HttpAPI.addClass(&Gate[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    if (Device.configuration.noOfContactrons > 0) {
      HttpAPI.addClass(&Contactron[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    if (Device.configuration.noOfDS18B20s > 0) {
      HttpAPI.addClass(&DS18B20Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    if (Device.configuration.noOfDHTs > 0) {
      HttpAPI.addClass(&DHTSensor[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    if (Device.configuration.noOfRegulators > 0) {
      HttpAPI.addClass(&Regulator[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    if (Device.configuration.noOfThermalProtectors > 0) {
      HttpAPI.addClass(&ThermalProtector[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    if (Device.configuration.noOfBinarySensors > 0) {
      HttpAPI.addClass(&BinarySensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    if (Device.configuration.noOfBinarySensors > 0) {
      HttpAPI.addClass(&BinarySensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    if (Device.configuration.noOfTSL2561s > 0) {
      HttpAPI.addClass(&TSL2561Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
    if (Device.configuration.noOfCLEDs > 0 ) {
      HttpAPI.addClass(&CLEDStrip);
    }
#endif


#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: HTTP init completed");
#endif
  }
}

/* Initializing Domoticz HTTP API */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

void initializeHTTPDomoticzAPI(void) {
  if (Device.getMode() != AFE_MODE_ACCESS_POINT &&
      Device.configuration.api.domoticz) {

#ifdef AFE_CONFIG_HARDWARE_LED
    HttpDomoticzAPI.begin(&Data, &Device, &Led);
#else
    HttpDomoticzAPI.begin(&Data, &Device);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.configuration.noOfRelays > 0) {
      HttpDomoticzAPI.addClass(&Relay[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    if (Device.configuration.noOfSwitches > 0) {
      HttpDomoticzAPI.addClass(&Switch[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
    if (Device.configuration.noOfAnalogInputs > 0) {
      HttpDomoticzAPI.addClass(&AnalogInput[0]);
    }
#else  // ESP8266
    if (Device.configuration.isAnalogInput) {
      HttpDomoticzAPI.addClass(&AnalogInput);
    }
#endif // ESP32/8266
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    if (Device.configuration.noOfBMEX80s > 0) {
      HttpDomoticzAPI.addClass(&BMEX80Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    if (Device.configuration.noOfHPMA115S0s > 0) {
      HttpDomoticzAPI.addClass(&HPMA115S0Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    if (Device.configuration.noOfBH1750s > 0) {
      HttpDomoticzAPI.addClass(&BH1750Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    if (Device.configuration.noOfAS3935s > 0) {
      HttpDomoticzAPI.addClass(&AS3935Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    if (Device.configuration.noOfAnemometerSensors > 0) {
      HttpDomoticzAPI.addClass(&AnemometerSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    if (Device.configuration.noOfRainmeterSensors > 0) {
      HttpDomoticzAPI.addClass(&RainSensor);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    if (Device.configuration.noOfContactrons > 0) {
      HttpDomoticzAPI.addClass(&Contactron[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Device.configuration.noOfGates > 0) {
      HttpDomoticzAPI.addClass(&Gate[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    if (Device.configuration.noOfDS18B20s > 0) {
      HttpDomoticzAPI.addClass(&DS18B20Sensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    if (Device.configuration.noOfDHTs > 0) {
      HttpDomoticzAPI.addClass(&DHTSensor[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    if (Device.configuration.noOfRegulators > 0) {
      HttpDomoticzAPI.addClass(&Regulator[0]);
    }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    if (Device.configuration.noOfThermalProtectors > 0) {
      HttpDomoticzAPI.addClass(&ThermalProtector[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    if (Device.configuration.noOfBinarySensors > 0) {
      HttpDomoticzAPI.addClass(&BinarySensor[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
    if (Device.configuration.noOfMiFareCards > 0) {
      HttpDomoticzAPI.addClass(&MiFareCard[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
    if (Device.configuration.noOfTSL2561s > 0) {
      HttpDomoticzAPI.addClass(&TSL2561Sensor[0]);
    }
#endif

/* RGB LED are not supported in HTTP Domoticz API 
#ifdef AFE_CONFIG_HARDWARE_CLED
    if (Device.configuration.noOfCLEDs > 0) {
      HttpDomoticzAPI.addClass(&CLEDStrip);
    }
#endif
*/
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: MQTT init completed");
#endif
  }
}

#endif // Domoticz API