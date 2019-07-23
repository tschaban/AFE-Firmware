/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _LANG_PL_h
#define _LANG_PL_h

#define L_LANGUAGE "pl-PL"
#define L_LANGUAGE_SHORT "pl"

/* Firmware names */
#if defined(DEVICE_SHELLY_1)
#define L_FIRMWARE_NAME "dla Shelly-1"
#elif defined(DEVICE_SONOFF_BASIC_V1)
#define L_FIRMWARE_NAME "dla Sonoff Basic"
#elif defined(T0_CONFIG)
#define L_FIRMWARE_NAME "WŁĄCZNIK WIFI"
#elif defined(T1_CONFIG)
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury"
#elif defined(T2_CONFIG)
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury i wilgotności"
#elif defined(T3_CONFIG)
#define L_FIRMWARE_NAME "dla 4 włączników i czujników ruchu"
#elif defined(T4_CONFIG)
#define L_FIRMWARE_NAME "dla 4 włączników WiFi"
#elif defined(T5_CONFIG)
#define L_FIRMWARE_NAME "do kontrolowania sterownika bramy"
#elif defined(T6_CONFIG)
#define L_FIRMWARE_NAME "Stacja Pogody"
#endif

/* Menu */
#define L_DEVICE "Urządzenie"
#define L_NETWORK "Sieć WiFi"
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
//#define L_INFORMATION "INFORMACJE"
#define L_DOCUMENTATION "Dokumentacja"
#define L_SETTINGS "Ustawienia"
#define L_SETTINGS_HOT_SPOT "Ustawienia (Tryb HotSpot)"
#define L_PRO_VERSION "Wersja PRO"

/* Forms commons */
#define L_NUMBER_OF_CHARS "znaków"
#define L_ENABLED "włączone"
#define L_USERNAME "Nazwa użytkownika"
#define L_PASSWORD "Hasło"
#define L_SECONDS "sekundy"
#define L_MILISECONDS "milisekundy"
#define L_IP_ADDRESS "Adres IP"
#define L_NONE "Brak"
#define L_NAME "Nazwa"
#define L_SAVE "Zapisz"
#define L_HELP "Pomoc"
#define L_VERSION "Wersja"
#define L_YES "Tak"
#define L_NO "Nie"
#define L_SENSOR "Czujnik"

/* Form: index */
#define L_WRONG_PASSWORD "Hasło nie jest poprawne"
#define L_NORMAL_MODE "Tryb normalny"
#define L_HOTSPOT_MODE "Tryb HotSpot"
#define L_LAUNCH_CONFIGURATION_PANEL "Uruchom: Panel Konfiguracyjny"
#define L_OPENING_CONFIG_PANEL "Otwieram panel konfiguracyjny AFE Firmware"

/* Form: device coniguration */
#define L_DEVICE_NAME "Nazwa urządzenia"
#define L_DEVICE_SECTION_INFO                                                  \
  "Nazwa jest wykorzystywana jako nazwa urządzenia w Twojej lokalnej sieci "  \
  "WiFi oraz jako nazwa hotspot'a urządzenia"
#define L_HARDWARE_CONFIGURATION "Konfiguracja urządzenia"
#define L_HARDWARE_CONFIGURATION_INFO                                          \
  "Wybierz podłączone elementy do urządzenia"
#define L_NUMBER_OF_LEDS "Ilość Led'ów"
#define L_NUMBER_OF_RELAYS "Ilość przekaźników"
#define L_NUMBER_OF_SWITCHES "Ilość przycisków"
#define L_DO_MEASURE_ADC "Pomiary z wejścia analogowego"
#define L_DEVICE_CONTROLLING "Sterowanie urządzeniem"
#define L_DEVICE_CONTROLLING_INFO                                              \
  "Włączanie / Wyłączanie mechanizmów sterowania urządzeniem"

#ifdef CONFIG_HARDWARE_GATE
#define L_CONTROLLED_GATES "Kontrolowane bramy/drzwi"
#endif

/* Form: network configuration */
#define L_NETWORK_CONFIGURATION "Konfiguracja sieci WiFi"
#define L_NETWORK_CONFIGURATION_INFO                                               \
  "Urządzenie bez dostępu do sieci będzie działać tylko w trybie sterowania " \
  "ręcznego"
#define L_SSID "Nazwa sieci WiFi"
#define L_REFRESH "Odśwież"
#define L_DHCP_ENABLED "Konfiguracja przez DHCP"
#define L_GATEWAY "Bramka"
#define L_SUBNET "Maska sieci"
#define L_DEVICE_IP "IP urządzenia"
#define L_DEVICE_IP_INFO                                                       \
  "Możesz skonfigurować adres IP urządzenia ręcznie lub automatycznie za " \
  "pośrednictwem DHCP"
#define L_NUMBER_OF_CONNECTIONS "Ilość prób łączenia się"
#define L_TIME_BETWEEN_CONNECTIONS "Czas między próbami łączenia się"
#define L_SLEEP_TIME "Czas po jakim powtórzyć próby łączenia się"
#define L_ADVANCED "Zaawansowane"
#define L_DEVICE_CONNECTS "Urządzenie łaczy się z siecią"
#define L_CONNECT_TO "Połącz się z siecią WiFi"
#define L_CONNECT "Połącz"
#define L_SEARCH_FOR_IP_ADDRESS                                                \
  "Następnie w konfigurację routera WiFi, wyszukaj adres IP urządzenia o "  \
  "adresie MAC"
#define L_FINISH_NETWORK_CONFIGURATION                                         \
  "Dokończ konfigurację wpisując adres IP urządzenia w przeglądarce"

/* Form: MQTT Broker */
#define L_MQTT_TOPIC "Temat"
#define L_MQTT_TOPIC_LWT "Temat wiadomości LWT"
#define L_MQTT_CONFIGURATION_INFO                                              \
  "Wprowadź adres hosta np. localhost lub adres IP"
#define L_MQTT_TOPIC_EMPTY                                                     \
  "Jeśli nie zostanie wprowadzone, wiadomość nie będzie wysłana do MQTT " \
  "Brokera"

/* Form:Domoticz server configuration */
#define L_PROTOCOL "Protokół"
#define L_DOMOTICZ_CONFIGURATION "Serwer Domoticz"
#define L_DOMOTICZ_CONFIGURATION_INFO                                          \
  "Wprowadź adres hosta np. localhost lub adres IP"
#define L_NO_IF_IDX_0                                                          \
  "Jeśli IDX jest 0 to wartość nie będzie wysyłana do Domoticz"

/* Form: Led configuration */
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
/* Form: Led configuration */
#define L_CHANGE_LED_INDICATION "Zmień świecenie diody LED na odwrotne"

/* Form: System LED configuration */
#define L_SYSTEM_LED "LED systemowy"
#define L_SYSTEM_LED_INFO                                                      \
  "Wybierz diodę LED, która będzie sygnalizowała stan urządzenia oraz "   \
  "występujące zdarzenia"

#endif

/* Form: relay */
#define L_DEFAULT_VALUES "Wartości domyślne"
#define L_DEFAULT_POWER_RESTORED "Po przywróceniu zasilania"
#define L_NO_ACTION "Brak akcji"
#define L_ON "Włączony"
#define L_OFF "Wyłączony"
#define L_LAST_KNOWN_STATE "Ostatnia zapamiętana wartość"
#define L_OPPOSITE_TO_LAST_KNOWN_STATE "Przeciwna do ostatniej zapamiętanej"
#define L_DEFAULT_MQTT_CONNECTED "Po podłączeniu do brokera MQTT"
#define L_DEFAULT_GET_FROM_MQTT                                                \
  "Wartość z systemu sterowania przekaźnikiem (przez MQTT)"
#define L_AUTOMATIC_SWITCHING_OFF "Automatyczne wyłączenie przekaźnika"
#define L_IMPULSE_DURATION "Długośc impulsu"
#define L_SWITCH_OFF_AFTER "Wyłącz po"
#define L_NO_ACTION_IF_0 "Brak akcji jeśli jest 0"
#define L_THERMAL_PROTECTION "Zabezpieczenie termiczne"
#define L_SWITCH_OFF_ABOVE "Wyłącz powyżej"
#define L_RANGE "Zakres"
#define L_SELECT_LED_4_RELAY "Wybierz LED sygnalizujący stan przekaźnika"
#define L_RELAY_MQTT_TOPIC "Temat MQTT sterujący przekaźnikiem"

/* Form: Switch */
#define L_FUNCTIONALITY "Funkcja"
#define L_SYSTEM_BUTTON "Przycisk systemowy"
#ifdef CONFIG_FUNCTIONALITY_RELAY
#define L_CONTROL_RELAY "Tylko sterowanie przekaźnikiem"
#endif
#ifdef CONFIG_FUNCTIONALITY_GATE
#define L_CONTROL_GATE "Tylko sterowanie bramą"
#endif
#define L_RELAY_CONTROLLED_BY_SWITCH "Sterowanie"
#define L_TYPE "Typ"
#define L_MONOSTABLE "Monostabilny"
#define L_BISTABLE "Bistabilny"
#define L_SENSITIVENESS "Czułość"
#define L__SENSITIVENESS_HINT                                                         \
  "Czułość należy ustawić metodą prób, aż uzyska się pożądane działanie " \
  "przycisku podczas jego wciskania"
#define L_SWITCH_BUTTON "Przycisk / Włącznik"
#define L_SWITCH_MQTT_TOPIC "Temat MQTT do monitorowania stanu przycisku"

/* Form: Analog Input */
#define L_CANT_CHANGE "brak możliwości zmiany"
#define L_MEASURMENTS_INTERVAL "Interwał odczytów"
#define L_MEASURED_VOLTAGE "Max napięcie wejściowe na ADC"
#define L_NUMBER_OF_SAMPLES "Ilość próbek do uśrednienia odczytu"
#define L_RAW_DATA "IDX Surowe Dane"
#define L_PERCENT "IDX Procent"
#define L_VOLTAGE "IDX Napięcie na wejściu"
#define L_VOLTAGE_CALCULATED "IDX Napięcie przeliczone"
#define L_RESISTOR "Rezystor"
#define L_VOLTAGE_DIVIDER "Dzielnik napięcia"

/* Form: upgrade and post upgrade */
#define L_SELECT_FIRMWARE "Wybierz Firmware"
#define L_UPGRADE_INFO                                                             \
  "Po zakończeniu aktualizacji urządzenie zostanie automatycznie "               \
  "zresetowane<br><br><strong>Uwaga:</strong> po aktualizacji nie odłączaj "     \
  "urządzenia przez conajmniej 1min.<br>Urządzenie formatuje pamięć i wgrywa " \
  "domyślne ustawienia.<br><strong>Ten proces nie może zostać "                 \
  "przerwany</strong>"
#define L_UPGRADE "Aktualizuj"
#define L_DONT_PLUG_OFF                                                        \
  "<strong>UWAGA</strong>: nie odłączaj urządzenia od źródła zasilania " \
  "podczas aktualizacji"
#define L_UPGRADE_FAILED "Aktualizacja nie powiodła się"
#define L_UPGRADE_SUCCESSFUL "Aktualizacja zakończona pomyślnie"
#define L_DEVICE_WILL_BE_REBOOTED                                              \
  "Po 15 sekundach oprogramowanie zostanie przeładowane. Proszę czekać"
#define L_RESTORE_DEFAULT_SETTINGS "Przywróć ustawienia początkowe"
#define L_WARNING "Uwaga"
#define L_CONFIGURATION_WILL_BE_REMOVED                                            \
  "przywrócenie ustawień początkowych usuwa wszystkie ustawienia urządzenia, " \
  "włącznie z konfiguracją sieci WiFi"
#define L_UPGRADE_IN_PROGRESS "Trwa przywracanie ustawień początkowych"
#define L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE                                     \
  "Po 20 sekundach połącz się z siecią WiFi o nazwie: "                    \
  "<strong>AFE-Device</strong>, a następnie połącz się z panelem "         \
  "konfiguracyjnym pod adresem"
#define L_RESTORING_DEFAULT_SETTING "Przywracanie ustawień początkowych"
#define L_REBOOT_IN_PROGRESS "Trwa restart urządzenia"
#define L_SITE_WILL_BE_RELOADED                                                \
  "Strona zostanie przeładowana automatycznie... czekaj"
#define L_REBOOT "Restart urządzenia"

/* Form: password */
#define L_SET_PASSWORD "Ustaw hasło"
#define L_SET_PASSWORD_TO_PANEL "Ustaw hasło do Panelu Konfiguracyjnego"
#define L_SET_PASSWORD_INFO                                                    \
  "Jeśli nie pamiętasz hasła, zresetuj urządzenie do stanu początkowego"
#define L_PROTECT_BY_PASSWORD "Zabezpiecz hasłem"

/* Form: Pro version */
#define L_PRO_VERSION "Wersja PRO"
#define L_KEY "Klucz"
#define L_VALID "Aktualny"
#define L_PRO_CANNOT_BE_COFIGURED                                              \
  "Konieczne jest podłączenie do Internetu, aby skonfigurować wersję PRO"

/* Form: Contactron */
#ifdef CONFIG_HARDWARE_CONTACTRON
#define L_LED_ASSIGNED_TO_SENSOR "LED przypisany do czujnika"
#define L_SET_CONTACTRON_SENSITIVENESS                                                   \
  "Czułość należy ustawić eksperymentalnie, aż uzyska się pożądane działanie " \
  "czujnika magnetycznego"
#define L_MAGNETIC_SENSOR "Czujnik magnetyczny"
#define L_MAGNETIC_SENSORS "Czujniki magnetyczne"
#define L_NUMBER_OF_MAGNETIC_SENSORS "Ilość czujników magnetycznych"
#define L_CONTACTRON_MQTT_TOPIC "Temat MQTT do monitorowania kontaktronu"
#endif

#ifdef CONFIG_HARDWARE_GATE
#define L_GATE "Brama/Drzwi"
#define L_IF_MAGNETIC_SENSOR "Jeśli czujnik magnetyczny"
#define L_AND_SENSOR "oraz czujnik"
#define L_IS_OPEN "jest otwarty"
#define L_ARE_OPEN "są otwarte"
#define L_THEN "to"
#define L_IS_CLOSED "jest zamknięty"
#define L_ARE_CLOSED "są zamknięte"
#define L_GATES_STATES_CONFIGURATION "Konfiguracja stanów bramy"
#define L_SET_GATE_STATE "Ustaw stan bramy na"
#define L_OPENED "Otwarta"
#define L_CLOSED "Zamknięta"
#define L_PARTIALLY_OPENED "Częściowo otwarta"
#define L_GATE_CONFIGURATION "Konfiguracja bramy/drzwi"
#define L_NUMBER_OF_CONTROLLED_GATES "Ilość sterowanych bram/drzwi"
#define L_RELAY_ID_CONTROLLING_GATE "Przekaźnik sterujący bramą/drzwiami"
#define L_GATE_MQTT_TOPIC "Temat MQTT sterujący bramą/drzwiami"
#endif

#endif
