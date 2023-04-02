#ifndef _AFE_files_h
#define _AFE_files_h

#include <Arduino.h>

/* ***************** FILESYSTEM *********************/
#define AFE_FS_SPIFFS 0
#define AFE_FS_LITTLEFS 1

/* Filesystem per ESP type */
#ifdef AFE_ESP32
#define AFE_FILE_SYSTEM AFE_FS_LITTLEFS
#else 
#define AFE_FILE_SYSTEM AFE_FS_SPIFFS
#endif


/* Configuration files names */
#ifndef AFE_ESP32 /* ESP82xx */
#define AFE_FILE_DOMOTICZ_CONFIGURATION "cfg-domoticz-server.json"
#define AFE_FILE_DEVICE_UID "cfg-device-uid.json"
#define AFE_FILE_MQTT_BROKER_CONFIGURATION "cfg-mqtt-broker.json"
#define AFE_FILE_DEVICE_MODE "cfg-device-mode.json"
#define AFE_FILE_PRO_VERSION_CONFIGURATION "cfg-pro-version.json"
#define AFE_FILE_PASSWORD_CONFIGURATION "cfg-password.json"
#define AFE_FILE_DEVICE_CONFIGURATION "cfg-device.json"
#define AFE_FILE_FIRMWARE_CONFIGURATION "cfg-firmware.json"
#define AFE_FILE_NETWORK_CONFIGURATION "cfg-network.json"
#define AFE_FILE_I2C_CONFIGURATION "cfg-i2c.json"
#define AFE_FILE_LED_CONFIGURATION "cfg-led-%d.json"
#define AFE_FILE_SYSTEM_LED_CONFIGURATION "cfg-system-led.json"
#define AFE_FILE_RELAY_CONFIGURATION "cfg-relay-%d.json"
#define AFE_FILE_RELAY_STATE_CONFIGURATION "cfg-relay-state-%d.json"
#define AFE_FILE_SWITCH_CONFIGURATION "cfg-switch-%d.json"
#define AFE_FILE_ADC_CONFIGURATION "cfg-analog-input.json"
#define AFE_FILE_UART_CONFIGURATION "cfg-uart.json"
#define AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION "cfg-anemometer-sensor-0.json"
#define AFE_FILE_RAINMETER_SENSOR_CONFIGURATION "cfg-rainmeter-sensor-0.json"
#define AFE_FILE_CONTACTRON_CONFIGURATION "cfg-contactron-%d.json"
#define AFE_FILE_GATE_CONFIGURATION "cfg-gate-%d.json"
#define AFE_FILE_GATE_STATE_CONFIGURATION "cfg-gate-state-%d.json"
#define AFE_FILE_HPMA114S0_CONFIGURATION "cfg-hpma115s0-%d.json"
#define AFE_FILE_BMX680_CONFIGURATION "cfg-BMEX80-%d.json"
#define AFE_FILE_BH1750_CONFIGURATION "cfg-bh1750-%d.json"
#define AFE_FILE_AS3935_CONFIGURATION "cfg-as3935-%d.json"
#else /* ESP32 */
#define AFE_FILE_DOMOTICZ_CONFIGURATION "/cfg-domoticz-server.json"
#define AFE_FILE_DEVICE_UID "/cfg-device-uid.json"
#define AFE_FILE_MQTT_BROKER_CONFIGURATION "/cfg-mqtt-broker.json"
#define AFE_FILE_DEVICE_MODE "/cfg-device-mode.json"
#define AFE_FILE_PRO_VERSION_CONFIGURATION "/cfg-pro-version.json"
#define AFE_FILE_PASSWORD_CONFIGURATION "/cfg-password.json"
#define AFE_FILE_DEVICE_CONFIGURATION "/cfg-device.json"
#define AFE_FILE_FIRMWARE_CONFIGURATION "/cfg-firmware.json"
#define AFE_FILE_NETWORK_CONFIGURATION "/cfg-network.json"
#define AFE_FILE_I2C_CONFIGURATION "/cfg-i2c-%d.json"
#define AFE_FILE_LED_CONFIGURATION "/cfg-led-%d.json"
#define AFE_FILE_SYSTEM_LED_CONFIGURATION "/cfg-system-led.json"
#define AFE_FILE_RELAY_CONFIGURATION "/cfg-relay-%d.json"
#define AFE_FILE_RELAY_STATE_CONFIGURATION "/cfg-relay-state-%d.json"
#define AFE_FILE_SWITCH_CONFIGURATION "/cfg-switch-%d.json"
#define AFE_FILE_ADC_CONFIGURATION "/cfg-analog-input-%d.json"
#define AFE_FILE_UART_CONFIGURATION "/cfg-uart.json"
#define AFE_FILE_ANEMOMETER_SENSOR_CONFIGURATION "/cfg-anemometer-sensor-0.json"
#define AFE_FILE_RAINMETER_SENSOR_CONFIGURATION "/cfg-rainmeter-sensor-0.json"
#define AFE_FILE_CONTACTRON_CONFIGURATION "/cfg-contactron-%d.json"
#define AFE_FILE_GATE_CONFIGURATION "/cfg-gate-%d.json"
#define AFE_FILE_GATE_STATE_CONFIGURATION "/cfg-gate-state-%d.json"
#define AFE_FILE_HPMA114S0_CONFIGURATION "/cfg-hpma115s0-%d.json"
#define AFE_FILE_BMX680_CONFIGURATION "/cfg-BMEX80-%d.json"
#define AFE_FILE_BH1750_CONFIGURATION "/cfg-bh1750-%d.json"
#define AFE_FILE_AS3935_CONFIGURATION "/cfg-as3935-%d.json"

#endif /* ESP8266/ESP32 */

#define AFE_FILE_HOME_ASSISTANT_CONFIGURATION "/cfg-home-assistant.json"
#define AFE_FILE_RAINMETER_SENSOR_DATA "/cfg-rainmeter-data-0.json"
#define AFE_FILE_REGULATOR_CONFIGURATION "/cfg-regulator-%d.json"
#define AFE_FILE_THERMAL_PROTECTOR_CONFIGURATION "/cfg-theremal-protector-%d.json"
#define AFE_FILE_DS18B20_SENSOR_CONFIGURATION "/cfg-ds18b20-%d.json"
#define AFE_FILE_DHT_SENSOR_CONFIGURATION "/cfg-dht-%d.json"
#define AFE_FILE_BINARY_SENSOR_CONFIGURATION "/cfg-binary-sensor-%d.json"
#define AFE_FILE_PN532_SENSOR_CONFIGURATION "/cfg-pn532-sensor-%d.json"
#define AFE_FILE_MIFARE_CARD_CONFIGURATION "/cfg-mifare-card-%d.json"
#define AFE_FILE_WELCOME_MESSAGE "/welcome-message.txt"
#define AFE_FILE_CLED_CONFIGURATION "/cfg-cled-%d.json"
#define AFE_FILE_CLED_EFFECT_BLINKING_CONFIGURATION "/cfg-cled-e-blinking%d.json"
#define AFE_FILE_CLED_EFFECT_WAVE_CONFIGURATION "/cfg-cled-e-wave%d.json"
#define AFE_FILE_CLED_EFFECT_FADE_INOUT_CONFIGURATION "/cfg-cled-e-fade_inout%d.json"

#define AFE_FILE_CLED_EFFECTS_CONFIGURATION "/cfg-cled-effects-%d.json"
#define AFE_FILE_CLED_BACKLIGHT_CONFIGURATION "/cfg-cled-backlight-%d.json"
#define AFE_FILE_TSL2561_CONFIGURATION "/cfg-tls2561-%d.json"
#define AFE_FILE_REBOOTS_COUNTER "/cfg-reboots_counter.json"
#define AFE_FILE_MCP23XXX_CONFIGURATION "/cfg-mcp23xxx-%d.json"
#define AFE_FILE_FS3000_CONFIGURATION "/cfg-fs3000-%d.json"


//const char AFE_FILE_MCP23XXX_CONFIGURATION[] PROGMEM = "/cfg-mcp23xxx-%d.json";

/* Configuration files, JSON Buffers.  */
#define AFE_CONFIG_FILE_BUFFER_DEVICE_UID 46   // AJ checked

/**
 * @brief JSON for MQTT Broker
 * {"host":"12345678901234567890123456789011","ip":"255.255.255.255","port":65222,"user":"12345678901234567890123456789011","password":"1234567890123456789012345678901112345678901234567890123456789011","lwt":"12345678901234567890123456789011123456789012345678901234567890111","s":"12345678901234567890123456789011123456789012345678901234567890111","retainAll":false,"retainLWT":false,"qos":1}
 * 
 */

#define AFE_CONFIG_FILE_BUFFER_MQTT_BROKER 524 //  AJ checked
#define AFE_CONFIG_FILE_BUFFER_DOMOTICZ 404 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_BH1750 221      // AJ checked
#ifdef AFE_ESP32
#define AFE_CONFIG_FILE_BUFFER_I2C 100          // AJ checked
#else
#define AFE_CONFIG_FILE_BUFFER_I2C 54          // AJ checked
#endif
#define AFE_CONFIG_FILE_BUFFER_DEVICE_MODE 41  // AJ checked
#define AFE_CONFIG_FILE_BUFFER_PRO_VERSION 78  // AJ checked
#define AFE_CONFIG_FILE_BUFFER_PASSWORD 72     // AJ checked
#define AFE_CONFIG_FILE_BUFFER_DEVICE 712      // AJ checked
/**
 * @brief JSON for Firmware version
 * {"type":0,"api":0,"version":"1234567890","latest_version":"1234567890"}
 */

#define AFE_CONFIG_FILE_BUFFER_FIRMWARE 134  // AJ checked
/**
 * @brief JSON for Network 
 * {"ssid":"12345678901234567890123456789011","password":"12345678901234567890123456789011","isDHCP":0,"ip":"192.168.112.164","gateway":"192.168.112.254","subnet":"255.255.255.111","dns1":"612.179.111.161","dns2":"621.179.111.162","ssidBackup":"12345678901234567890123456789011","passwordBackup":"12345678901234567890123456789011","isDHCPBackup":1,"ipBackup":"111.111.111.111","gatewayBackup":"111.111.111.111","subnetBackup":"111.111.111.111","dns1Backup":"111.111.111.111","dns2Backup":"111.111.111.111","noConnectionAttempts":110,"waitTimeConnections":111,"waitTimeSeries":115,"noFailuresToSwitchNetwork":112}
 */
#define AFE_CONFIG_FILE_BUFFER_NETWORK 868     // AJ checked 
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#define AFE_CONFIG_FILE_BUFFER_LED 156 //  AJ checked
#else
#define AFE_CONFIG_FILE_BUFFER_LED 156         // AJ checked
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#define AFE_CONFIG_FILE_BUFFER_SYSTEM_LED 35  // AJ checked
#define AFE_CONFIG_FILE_BUFFER_RELAY 287      // AJ checked for T6 only
#define AFE_CONFIG_FILE_BUFFER_RELAY_STATE 46 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_SWITCH 238     // AJ checked for T6 only
#define AFE_CONFIG_FILE_BUFFER_ADC 489        // AJ checked
#define AFE_CONFIG_FILE_BUFFER_HPMA115S0 202  // AJ checked
#define AFE_CONFIG_FILE_BUFFER_UART 54        // AJ checked
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#define AFE_CONFIG_FILE_BUFFER_BMEX80 984 // AJ checked
#else
#define AFE_CONFIG_FILE_BUFFER_BMEX80 572 // AJ checked
#endif
#define AFE_CONFIG_FILE_BUFFER_AS3935 439     // AJ checked
#define AFE_CONFIG_FILE_BUFFER_CONTACTRON 241 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_GATE 319       // AJ checked
#define AFE_CONFIG_FILE_BUFFER_GATE_STATE 42
#define AFE_CONFIG_FILE_BUFFER_ANEMOMETER 303        // AJ checked
#define AFE_CONFIG_FILE_BUFFER_RAINMETER 259         // AJ checked
#define AFE_CONFIG_FILE_BUFFER_RAINMETER_DATA 1100   // Not verfied
#define AFE_CONFIG_FILE_BUFFER_DS18B20 407           // AJ checked
#define AFE_CONFIG_FILE_BUFFER_REGULATOR 337         // AJ checked
#define AFE_CONFIG_FILE_BUFFER_THERMAL_PROTECTOR 261 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_DHT 643               // AJ checked
#define AFE_CONFIG_FILE_BUFFER_BINARY_SENSOR 238     // AJ checked for T6 only
#define AFE_CONFIG_FILE_BUFFER_PN532_SENSOR 408      // AJ checked
#define AFE_CONFIG_FILE_BUFFER_MIFARE_CARD 317 // AJ Checked
#define AFE_CONFIG_FILE_BUFFER_CLED 614 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_BLINKING 407 // AJ checked
#define AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_WAVE 354 //  AJ checked
#define AFE_CONFIG_FILE_BUFFER_CLED_EFFECT_FADE_INOUT 235 //  AJ checked

// @TODO T5 to be removed
#define AFE_CONFIG_FILE_BUFFER_CLED_EFFECTS 228 // AJ Checked
#define AFE_CONFIG_FILE_BUFFER_CLED_BACKIGHT 418 // AJ Checked

#define AFE_CONFIG_FILE_BUFFER_TSL2561 290 // AJ Checked
#define AFE_CONFIG_FILE_BUFFER_HOME_ASSISTANT 190 // AJ Checked
#define AFE_FILE_BUFFER_REBOOTS_COUNTER 43 // AJ Checked
#define AFE_CONFIG_FILE_BUFFER_MCP23XXX 200 // @TODO T4

#define AFE_CONFIG_FILE_BUFFER_FS3000 400 // @TODO 3.6.0


const char AFE_CONFIG_BODY_NETWORK[] PROGMEM = "{\"ssid\":\"%s\",\"password\":\"%s\",\"ssidBackup\":\"%s\",\"passwordBackup\":\"%s\",\"isDHCP\":%d,\"ip\":\"%s\",\"gateway\":\"%s\",\"subnet\":\"%s\",\"isDHCPBackup\":%d,\"ipBackup\":\"%s\",\"gatewayBackup\":\"%s\",\"subnetBackup\":\"%s\",\"noConnectionAttempts\":%d,\"waitTimeConnections\":%d,\"waitTimeSeries\":%d,\"noFailuresToSwitchNetwork\":%d,\"radioMode\":%d,\"outputPower\":255}";


#endif