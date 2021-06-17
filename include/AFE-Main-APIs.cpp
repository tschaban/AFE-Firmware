/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include <AFE-API-HTTP.h>
#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLE
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#include <AFE-API-MQTT-Standard.h>
#endif

/* ---------Headers ---------*/

void initializeMQTTAPI(void);
void initializeHTTPAPI(void);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void initializeHTTPDomoticzAPI(void);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

/* --------- Body -----------*/

AFEAPIHTTP HttpAPI;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
AFEAPIMQTTDomoticz MqttAPI;
AFEAPIHTTPDomoticz HttpDomoticzAPI;
#else
AFEAPIMQTTStandard MqttAPI;
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

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    if (Device.configuration.isAnalogInput) {
      MqttAPI.addClass(&AnalogInput);
    }
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
    if (Device.configuration.noOfPN532Sensors > 0) {
      MqttAPI.addClass(&PN532Sensor[0]);
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
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI, &HttpDomoticzAPI);
#else
    HttpAPI.begin(&Device, &WebServer, &Data, &MqttAPI);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    if (Device.configuration.noOfRelays > 0) {
      HttpAPI.addClass(&Relay[0]);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    if (Device.configuration.isAnalogInput) {
      HttpAPI.addClass(&AnalogInput);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    // @TODO Missing noOf check
    HttpAPI.addClass(&BMEX80Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    HttpAPI.addClass(&HPMA115S0Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    // @TODO Missing noOf check
    HttpAPI.addClass(&BH1750Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    HttpAPI.addClass(&AS3935Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    HttpAPI.addClass(&AnemometerSensor);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    HttpAPI.addClass(&RainSensor);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    // @TODO Missing noOf check
    HttpAPI.addClass(&Gate[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    // @TODO Missing noOf check
    HttpAPI.addClass(&Contactron[0]);
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

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: HTTP init completed");
#endif
  }
}

/* Initializing Domoticz HTTP API */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

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

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    if (Device.configuration.isAnalogInput) {
      HttpDomoticzAPI.addClass(&AnalogInput);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    // @TODO Missing noOf check
    HttpDomoticzAPI.addClass(&BMEX80Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    HttpDomoticzAPI.addClass(&HPMA115S0Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    // @TODO Missing noOf check
    HttpDomoticzAPI.addClass(&BH1750Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    HttpDomoticzAPI.addClass(&AS3935Sensor[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    HttpDomoticzAPI.addClass(&AnemometerSensor);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    HttpDomoticzAPI.addClass(&RainSensor);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    // @TODO Missing noOf check
    HttpDomoticzAPI.addClass(&Contactron[0]);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    // @TODO Missing noOf check
    HttpDomoticzAPI.addClass(&Gate[0]);
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

#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: API: MQTT init completed");
#endif
  }
}

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED