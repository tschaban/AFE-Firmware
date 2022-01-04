#ifndef _AFE_Webforms_h
#define _AFE_Webforms_h

#define AFE_MAX_PAGE_SIZE 12000

/* Types of URL requests */
#define AFE_SERVER_CMD_SAVE 1
#define AFE_SERVER_CMD_NONE 0

#ifndef AFE_ESP32 /* ESP82xx */
#define AFE_SITE_REBOOT 10
#define AFE_SITE_REBOOT_POST_UPGRADE 15
#define AFE_SITE_REBOOT_POST_WAN_UPGRADE 30
#else /* ESP32 */
#define AFE_SITE_REBOOT 5
#define AFE_SITE_REBOOT_POST_UPGRADE 5
#define AFE_SITE_REBOOT_POST_WAN_UPGRADE 30
#endif

/* Form constants */
#define AFE_FORM_ITEM_TYPE_NUMBER "number"
#define AFE_FORM_ITEM_TYPE_TEXT "text"
#define AFE_FORM_ITEM_TYPE_PASSWORD "password"
#define AFE_FORM_ITEM_SKIP_PROPERTY "?"

/* Config sites IDs */
#define AFE_CONFIG_SITE_INDEX 0
#define AFE_CONFIG_SITE_DEVICE 1
#define AFE_CONFIG_SITE_NETWORK 2
#define AFE_CONFIG_SITE_MQTT 3
#define AFE_CONFIG_SITE_MQTT_TOPICS 4
#define AFE_CONFIG_SITE_DOMOTICZ 5
#define AFE_CONFIG_SITE_LED 6
#define AFE_CONFIG_SITE_SYSTEM_LED 7
#define AFE_CONFIG_SITE_RELAY 8
#define AFE_CONFIG_SITE_SWITCH 9
#define AFE_CONFIG_SITE_PASSWORD 10
#define AFE_CONFIG_SITE_PRO_VERSION 11
#define AFE_CONFIG_SITE_ANALOG_INPUT 12
#define AFE_CONFIG_SITE_CONTACTRON 13
#define AFE_CONFIG_SITE_GATE 14
#define AFE_CONFIG_SITE_EXIT 15
#define AFE_CONFIG_SITE_RESET 16
#define AFE_CONFIG_SITE_POST_RESET 17
#define AFE_CONFIG_SITE_FIRST_TIME 18
#define AFE_CONFIG_SITE_FIRST_TIME_CONNECTING 19
#define AFE_CONFIG_SITE_UPGRADE 20
#define AFE_CONFIG_SITE_POST_UPGRADE 21
#define AFE_CONFIG_SITE_UART 22
#define AFE_CONFIG_SITE_HPMA115S0 23
#define AFE_CONFIG_SITE_BMEX80 24
#define AFE_CONFIG_SITE_BH1750 25
#define AFE_CONFIG_SITE_AS3935 26
#define AFE_CONFIG_SITE_I2C 27
#define AFE_CONFIG_SITE_DS18B20 28
#define AFE_CONFIG_SITE_ANEMOMETER_SENSOR 29
#define AFE_CONFIG_SITE_RAINMETER_SENSOR 30
#define AFE_CONFIG_SITE_REGULATOR 31
#define AFE_CONFIG_SITE_THERMAL_PROTECTOR 32
#define AFE_CONFIG_SITE_DHT 33
#define AFE_CONFIG_SITE_BINARY_SENSOR 34
#define AFE_CONFIG_SITE_WAN_UPGRADE 35
#define AFE_CONFIG_SITE_PN532_SENSOR 36
#define AFE_CONFIG_SITE_MIFARE_CARDS 37
#define AFE_CONFIG_SITE_PN532_SENSOR_ADMIN 38
#define AFE_CONFIG_SITE_CLED 39
#define AFE_CONFIG_SITE_CLED_EFFECT_BLINKING 40
#define AFE_CONFIG_SITE_CLED_EFFECT_FADE_IN_OUT 41
#define AFE_CONFIG_SITE_CLED_EFFECT_WAVE 42
#define AFE_CONFIG_SITE_CLED_PN532_SENSOR 43
#define AFE_CONFIG_SITE_TSL2561 44
#define AFE_CONFIG_SITE_FIRMWARE 45
#define AFE_CONFIG_SITE_HOME_ASSISTANT_INTEGRATION 46


#endif // _AFE_Webforms_h