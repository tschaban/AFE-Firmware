#ifndef _LANG_EN_h
#define _LANG_EN_h

#define L_LANGUAGE "en-EN"

/* Firmware names */
#ifdef T0_CONFIG
#define L_FIRMWARE_NAME "WiFi Switch"
#elif T0_SHELLY_1_CONFIG
#define L_FIRMWARE_NAME "for Shelly-1"
#elif T1_CONFIG
#define L_FIRMWARE_NAME "Switch with temperature sensor"
#elif T2_CONFIG
#define L_FIRMWARE_NAME "Switch with temperature and humidity sensor"
#elif T3_CONFIG
#define L_FIRMWARE_NAME "for 4 WiFi switches and PIR sensors"
#elif T4_CONFIG
#define L_FIRMWARE_NAME "for 4 WiFi switches"
#elif T5_CONFIG
#define L_FIRMWARE_NAME "to control gate controller"
#elif T6_CONFIG
#define L_FIRMWARE_NAME "Wheater Station"
#endif

/* Menu */
#define L_DEVICE "Device"
#define L_NETWORK "Network"
#define L_MQTT_BROKER "MQTT Broker"
#define L_DOMOTICZ_SERVER "Domoticz Server"
#define L_LEDS "LEDs"
#define L_RELAYS_CONFIGURATION "Relays configuration"
#define L_RELAY "Relay"
#define L_BUTTONS_SWITCHES "Button / Switches"
#define L_SWITCH "Switch"
#define L_ANALOG_INPUT "Analog Input"
#define L_FIRMWARE_UPGRADE "Firmware upgrade"
#define L_RESET_DEVICE "Reset to orginal state"
#define L_FINISH_CONFIGURATION "Finish configuration"
#define L_INFORMATION "INFORMATION"
#define L_DOCUMENTATION "Documentation"

#endif
