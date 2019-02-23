#ifndef _LANG_PL_h
#define _LANG_PL_h

#define L_LANGUAGE "pl-PL"

/* Firmware names */
#ifdef T0_CONFIG
#define L_FIRMWARE_NAME "Włącznik WiFi"
#elif T0_SHELLY_1_CONFIG
#define L_FIRMWARE_NAME "dla Shelly-1"
#elif T1_CONFIG
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury"
#elif T2_CONFIG
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury i wilgotności"
#elif T3_CONFIG
#define L_FIRMWARE_NAME "dla 4 włączników i czujników ruchu"
#elif T4_CONFIG
#define L_FIRMWARE_NAME "dla 4 włączników WiFi"
#elif T5_CONFIG
#define L_FIRMWARE_NAME "do kontrolowania sterownika bramy"
#elif T6_CONFIG
#define L_FIRMWARE_NAME "Stacja Pogody"
#endif

/* Menu */
#define L_DEVICE "Urządzenie"
#define L_NETWORK "Siec WiFi"
#define L_MQTT_BROKER "MQTT Broker"
#define L_DOMOTICZ_SERVER "Serwer Domoticz"
#define L_LEDS "LEDy"
#define L_RELAYS_CONFIGURATION "Konfiguracja przekaźników"
#define L_RELAY "Przekaźnik"
#define L_BUTTONS_SWITCHES "Przyciski"
#define L_SWITCH "Przycisk"
#define L_ANALOG_INPUT "Wejście analogowe"
#define L_FIRMWARE_UPGRADE "Aktualizacja oprogramowania"
#define L_RESET_DEVICE "Ustawienia początkowe"
#define L_FINISH_CONFIGURATION "Zakoncz konfigurację"
#define L_INFORMATION "INFORMACJE"
#define L_DOCUMENTATION "Dokumentacja"

#endif
