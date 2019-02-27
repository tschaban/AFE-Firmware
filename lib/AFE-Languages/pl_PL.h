#ifndef _LANG_PL_h
#define _LANG_PL_h

#define L_LANGUAGE                  "pl-PL"
#define L_LANGUAGE_SHORT            "pl"

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
#define L_DEVICE                      "Urządzenie"
#define L_NETWORK                     "Siec WiFi"
#define L_MQTT_BROKER                 "MQTT Broker"
#define L_DOMOTICZ_SERVER             "Serwer Domoticz"
#define L_LEDS                        "LEDy"
#define L_RELAYS_CONFIGURATION        "Konfiguracja przekaźników"
#define L_RELAY                       "Przekaźnik"
#define L_BUTTONS_SWITCHES            "Przyciski"
#define L_SWITCH                      "Przycisk"
#define L_ANALOG_INPUT                "Wejście analogowe"
#define L_FIRMWARE_UPGRADE            "Aktualizacja oprogramowania"
#define L_RESET_DEVICE                "Ustawienia początkowe"
#define L_FINISH_CONFIGURATION        "Zakoncz konfigurację"
#define L_INFORMATION                 "INFORMACJE"
#define L_DOCUMENTATION               "Dokumentacja"
#define L_SETTINGS                    "Ustawienia"
#define L_SETTINGS_HOT_SPOT           "Ustawienia (Tryb HotSpot)"

/* Forms commons */
#define L_NUMBER_OF_CHARS             "znaków"
#define L_ENABLED                     "włączone"
#define L_USERNAME                    "Nazwa użytkownika"
#define L_PASSWORD                    "Hasło"
#define L_SECONDS                     "sekundy"
#define L_IP_ADDRESS                  "Adres IP"
#define L_NONE                        "Brak"

/* Form: device coniguration */
#define L_DEVICE_NAME                 "Nazwa urządzenia"
#define L_DEVICE_SECTION_INFO         "Nazwa jest wykorzystywana jako nazwa urządzenia w Twojej lokalnej sieci WiFi oraz jako nazwa hotspot'a urządzenia"
#define L_HARDWARE_CONFIGURATION      "Konfiguracja urządzenia"
#define L_HARDWARE_CONFIGURATION_INFO "Wybierz podłączone elementy do urządzenia"
#define L_NUMBER_OF_LEDS              "Ilość Led'ów"
#define L_NUMBER_OF_RELAYS            "Ilość przekaźników"
#define L_NUMBER_OF_SWITCHES          "Ilość przycisków"
#define L_DO_MEASURE_ADC              "Pomiary z wejścia analogowego"
#define L_DEVICE_CONTROLLING          "Sterowanie urządzeniem"
#define L_DEVICE_CONTROLLING_INFO     "Włączanie / Wyłączanie mechanizmów sterowania urządzeniem"

/* Form: network configuration */
#define L_NETWORK_CONFIGURATION_INFO  "Urządzenie bez dostępu do sieci będzie działać tylko w trybie sterowania ręcznego"
#define L_SSID                        "Nazwa sieci WiFi"
#define L_DHCP_ENABLED                "Konfiguracja przez DHCP"
#define L_GATEWAY                     "Bramka"
#define L_SUBNET                      "Maska sieci"
#define L_DEVICE_IP                   "IP urządzenia"
#define L_DEVICE_IP_INFO              "Możesz skonfigurować adres IP urządzenia ręcznie lub automatycznie za pośrednictwem DHCP"
#define L_NUMBER_OF_CONNECTIONS       "Ilość prób łączenia się"
#define L_TIME_BETWEEN_CONNECTIONS    "Czas między próbami łączenia się"
#define L_SLEEP_TIME                  "Czas po jakim powtórzyć próby łączenia się"
#define L_ADVANCED                    "Zaawansowane"

/* Form: MQTT Broker */
#define L_MQTT_TOPIC                  "Temat wiadomości MQTT"
#define L_EXAMPLE                     "Przykład"
#define L_MQTT_CONFIGURATION_INFO     "Wprowadź adres hosta np. localhost lub adres IP"

/* Form:Domoticz server configuration */
#define L_PROTOCOL                    "Protokół"
#define L_DOMOTICZ_CONFIGURATION      "Serwer Domoticz"
#define L_DOMOTICZ_CONFIGURATION_INFO "Wprowadź adres hosta np. localhost lub adres IP"

/* Form: Led configuration */
#ifdef CONFIG_HARDWARE_LED
/* Form: Led configuration */
#define L_CHANGE_LED_INDICATION       "Zmień świecenie diody LED na odwrotne"

/* Form: System LED configuration */
#define L_SYSTEM_LED                  "LED systemowy"
#define L_SYSTEM_LED_INFO             "Wybierz diodę LED, która będzie sygnalizowała stan urządzenia oraz występujące zdarzenia"

#endif


#endif
