/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _LANG_PL_h
#define _LANG_PL_h

#define L_LANGUAGE "pl-PL"
#define L_LANGUAGE_SHORT "pl"

/* Firmware names */
#if defined(AFE_DEVICE_SHELLY_1)
#define L_FIRMWARE_NAME "dla Shelly-1"
#elif defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define L_FIRMWARE_NAME "dla Sonoff Basic"
#elif defined(T0_CONFIG)
#define L_FIRMWARE_NAME "Wersja podstawowa"
#elif defined(T1_CONFIG)
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury"
#elif defined(T2_CONFIG)
#define L_FIRMWARE_NAME "Włącznik z czujnikiem temperatury i wilgotności"
#elif defined(T3_CONFIG)
#define L_FIRMWARE_NAME "dla 4 włączników i czujników ruchu"
#elif defined(T4_CONFIG)
#define L_FIRMWARE_NAME "z expanderem MCP23017"
#elif defined(T5_CONFIG)
#define L_FIRMWARE_NAME "do kontrolowania sterownika bramy"
#elif defined(T6_CONFIG)
#define L_FIRMWARE_NAME "Stacja Pogody"
#endif

/* HTTP API */
#define L_DEVICE_NOT_EXIST "\"Urządzenie o takiej nazwie nie istnieje\""
#define L_COMMAND_NOT_IMPLEMENTED "\"Komenda nie jest zaimplementowana\""
#define L_404 "AFE FIRMWARE: Bład 404: Strona o takim adresie nie istnieje."
#define L_COMMNAD_NO_COMMAND "\"Brak komendy\""

/* Menu */
#define L_HARDWARE "Sprzęt"
#define L_FUNCTIONS "Funkcje"
#define L_FIRMWARE "Firmware"
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
#define L_FINISH_CONFIGURATION "&#10004; Zakończ konfigurację"
#define L_DOCUMENTATION "Dokumentacja"
#define L_SETTINGS "Ustawienia"
#define L_SETTINGS_HOT_SPOT "Ustawienia (Tryb HotSpot)"
#define L_PRO_VERSION "Wersja PRO"
#define L_NETWORK_CONNECTED "Podłączony"
#define L_DISCONNECTED "Odłączony"
#define L_CONNECTIONS "Połączenia"
#define L_CLEDS "LEDy RGB"
#define L_HOME_ASSISTANT_INTEGRATION "Integracja z Home Assistant"

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
#define L_DISTANCE_UNIT "Jednostka odległości"
#define L_CM "Centrymetry"
#define L_M "Metry"
#define L_KM "Kilometry"
#define L_MILES "Mile"
#define L_RANGE "Zakres"
#define L_SENSITIVENESS "Czułość"
#define L_MEASURMENTS_INTERVAL "Interwał odczytów"
#define L_ADDRESS "Adres"
#define L_CORRECTIONS "Korekty"
#define L_UNITS "Jednostki"
#define L_FREQUENCY "Częstotliwość"
#define L_I2C_PORT "Port I2C"
#define L_GAIN "Wzmocnienie"
#define L_FILE_NOT_FOUND_1 "Brak pliku konfiguracyjnego lub wystąpił problem z jego odczytem."
#define L_FILE_NOT_FOUND_2 "Odśwież stronę"
#define L_FILE_NOT_FOUND_3 "Jeśli problem będzie występował to ustaw ponownie konfigurację lub przywróć ustawienia początkowe"
#define L_ATTENTION "Uwaga"

/* Form: index */
#define L_INDEX_WRONG_PASSWORD "Hasło nie jest poprawne"
#define L_INDEX_NORMAL_MODE "Tryb normalny"
#define L_INDEX_HOTSPOT_MODE "Tryb HotSpot"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL "Urządzenie jest w trybie operacyjnym"
#define L_INDEX_LAUNCH_CONFIGURATION_PANEL_HINT "Aby uruchomić panel konfiguracyjny użyj jednego z poniższych przycisków"
#define L_INDEX_OPENING_CONFIG_PANEL "Otwieram panel konfiguracyjny AFE Firmware"


/* Form: device coniguration */
#define L_DEVICE_NAME "Nazwa urządzenia"
#define L_DEVICE_SECTION_INFO "Nazwa jest wykorzystywana jako nazwa urządzenia w Twojej lokalnej sieci WiFi oraz jako nazwa hotspot'a urządzenia"
#define L_DEVICE_HARDWARE_CONFIGURATION "Konfiguracja urządzenia"
#define L_DEVICE_HARDWARE_CONFIGURATION_INFO "Wybierz podłączone elementy do urządzenia"
#define L_DEVICE_NUMBER_OF_LEDS "Ilość Led'ów"
#define L_DEVICE_NUMBER_OF_CLEDS "Ilość Led'ów RGB"
#define L_DEVICE_NUMBER_OF_RELAYS "Ilość przekaźników"
#define L_DEVICE_NUMBER_OF_SWITCHES "Ilość przycisków"
#define L_DEVICE_NUMBER_OF_CONTACTRONS "Ilość czujników magnetycznych"
#define L_DEVICE_NUMBER_OF_THERMAL_PROTECTORS "Ilość zabezpieczeń termicznych"
#define L_DEVICE_NUMBER_OF_CONTROLLED_GATES "Ilość sterowanych bram/drzwi"
#define L_DEVICE_NUMBER_OF_HPMA115S0_SENSORS "Czujnik HPMA115S0"
#define L_DEVICE_NUMBER_OF_BMEX80_SENSORS "Ilość czujników BMEx80"
#define L_DEVICE_NUMBER_OF_BH1750_SENSORS "Ilość czujników BH1750"
#define L_DEVICE_NUMBER_OF_AS3935_SENSORS "Liczba czujników AS3935"
#define L_DEVICE_NUMBER_OF_DS18B20_SENSORS "Liczba czujników DS18B20"
#define L_DEVICE_NUMBER_OF_REGULATORS "Ilość regulatorów"
#define L_DEVICE_NUMBER_OF_BINARY_SENSORS "Ilość czujników binarnych"
#define L_DEVICE_NUMBER_OF_PN532_SENSORS "Ilość czujników PN532"
#define L_DEVICE_NUMBER_OF_MIFARE_CARDS "Ilość obsługiwanych kart MiFare"
#define L_DEVICE_NUMBER_OF_I2CS "Ilość portów I2C"
#define L_DEVICE_NUMBER_OF_TSL2561_SENSORS "Ilość czujników TSL2561"
#define L_DEVICE_NUMBER_OF_ADC "Ilość wejść ADC"

#define L_DEVICE_ADDITIONAL_FUNCTIONALITIES "Dodatkowe funkcje"
#define L_DEVICE_DO_MEASURE_ADC "Pomiary z wejścia analogowego"
#define L_DEVICE_CONTROLLED_GATES "Kontrolowane bramy/drzwi"
#define L_DEVICE_CONTROLLING "Sterowanie urządzeniem"
#define L_DEVICE_CONTROLLING_INFO "Włączanie / Wyłączanie mechanizmów sterowania urządzeniem"
#define L_DEVICE_DOMOTICZ_VERSION_410 "4.10x i wcześniejsze"
#define L_DEVICE_DOMOTICZ_VERSION_2020 "2020.x i późniejsze"
#define L_DEVICE_AUTOLOGOUT_TITLE "Automatyczne zakończenie konfiguracji"
#define L_DEVICE_AUTOLOGOUT_DESCRIPTION "Konfiguracja urządzenia zostanie automatycznie wyłączona po 10min. braku akcji w panelu konfiguracyjnym"

/* Form: network configuration */
#define L_NETWORK_CONFIGURATION "Konfiguracja sieci WiFi"
#define L_NETWORK_CONFIGURATION_INFO "Urządzenie bez dostępu do sieci będzie działać tylko w trybie sterowania ręcznego"
#define L_NETWORK_SSID "Nazwa sieci WiFi"
#define L_NETWORK_REFRESH "Odśwież"
#define L_NETWORK_DHCP_ENABLED "Konfiguracja przez DHCP"
#define L_NETWORK_GATEWAY "Bramka"
#define L_NETWORK_SUBNET "Maska sieci"
#define L_NETWORK_DEVICE_IP "IP urządzenia"
#define L_NETWORK_DEVICE_IP_INFO "Możesz skonfigurować adres IP urządzenia ręcznie lub automatycznie za pośrednictwem DHCP"
#define L_NETWORK_NUMBER_OF_CONNECTIONS "Ilość prób łączenia się"
#define L_NETWORK_TIME_BETWEEN_CONNECTIONS "Czas między próbami łączenia się"
#define L_NETWORK_SLEEP_TIME "Czas po jakim powtórzyć próby łączenia się"
#define L_NETWORK_ADVANCED "Zaawansowane"
#define L_NETWORK_ALMOST "Prawie jesteśmy na miejscu!"
#define L_NETWORK_DEVICE_CONNECTS "Urządzenie łaczy się z siecią"
#define L_NETWORK_CONNECT_TO "Połącz się z siecią WiFi"
//#define L_NETWORK_CONNECT "Połącz"
#define L_NETWORK_SEARCH_FOR_IP_ADDRESS "Następnie w konfigurację routera WiFi, wyszukaj adres IP urządzenia o adresie MAC"
#define L_NETWORK_FINISH_NETWORK_CONFIGURATION  "Dokończ konfigurację wpisując adres IP urządzenia w przeglądarce"
#define L_NETWOK_NONE_BACKUP_SSID "--- Brak ---"
#define L_NETWORK_BACKUP_CONFIGURATION "Zapasowa konfiguracja WiFi"
#define L_NETWORK_SWITCH_TO_BACKUP "Liczba błędów połączeń przed uruchomieniem konfiguracji zapasowej"



/* Form: MQTT Broker */
#define L_MQTT_TOPIC "Temat"
#define L_MQTT_TOPIC_LWT "Temat wiadomości LWT"
#define L_MQTT_IDX_LWT "IDX dla wiadomości LWT"
#define L_MQTT_CONFIGURATION_INFO "Wprowadź adres hosta np. localhost lub adres IP"
#define L_MQTT_TOPIC_EMPTY "Jeśli nie zostanie wprowadzone, wiadomość nie będzie wysłana do MQTT Brokera"
#define L_MQTT_TIMEOUT "Jak długo czekać na odpowiedź z serwera"
#define L_MQTT_RETAIN_SECTION "Zapamiętywanie wiadomości w MQTT Broker"
#define L_MQTT_RETAIN_SECTION_INFO "Wysłane wiadomości zostaną zapamiętana przez serwer i będą automatycznie wysłana do każdego kto zasubskrybuje dany temat."
#define L_MQTT_RETAIN_LWT "Pamiętaj LWT"
#define L_MQTT_RETAIN_ALL "Pamiętaj wszystkie wiadomości"
#define L_MQTT_CONNECTION "Konfiguracja łączenia"
#define L_MQTT_CONNECTION_HINT "Włącz sprawdzanie dostępności serwera w sieci LAN, jeśli urządzenie musi działać, gdy serwer jest niedostępny. Możesz dodatkowo zmnieszyć lub zwiększyć czas oczekiwania na odpowiedź z serwera"
#define L_MQTT_USE_PING "Sprawdzić dostępność serwera przed połączeniem?"
#define L_MQTT_USE_PING_HINT "Serwer nie może blokować poleceń PING"

/* Form: Domoticz server configuration */
#define L_DOMOTICZ_PROTOCOL "Protokół"
#define L_DOMOTICZ_CONFIGURATION "Serwer Domoticz"
#define L_DOMOTICZ_CONFIGURATION_INFO "Wprowadź adres hosta np. localhost lub adres IP"
#define L_DOMOTICZ_NO_IF_IDX_0 "Jeśli IDX jest 0 to wartość nie będzie wysyłana do Domoticz"
#define L_DOMOTICZ_VERSION "Wersja Domoticz"

/* Form: Led configuration */
#define L_LED_CHANGE_INDICATION "Zmień świecenie diody LED na odwrotne"
#define L_LED_SYSTEM "LED systemowy"
#define L_LED_SYSTEM_INFO "Wybierz diodę LED, która będzie sygnalizowała stan urządzenia oraz występujące zdarzenia"

/* Form: relay */
#define L_RELAY_DEFAULT_VALUES "Wartości domyślne"
#define L_RELAY_DEFAULT_POWER_RESTORED "Po przywróceniu zasilania"
#define L_RELAY_NO_ACTION "Brak akcji"
#define L_RELAY_ON "Włączony"
#define L_RELAY_OFF "Wyłączony"
#define L_RELAY_LAST_KNOWN_STATE "Ostatnia zapamiętana wartość"
#define L_RELAY_OPPOSITE_TO_LAST_KNOWN_STATE "Przeciwna do ostatniej zapamiętanej"
#define L_RELAY_DEFAULT_MQTT_CONNECTED "Po podłączeniu do brokera MQTT"
#define L_RELAY_DEFAULT_GET_FROM_MQTT "Wartość z systemu sterowania przekaźnikiem (przez MQTT)"
#define L_RELAY_AUTOMATIC_SWITCHING_OFF "Automatyczne wyłączenie przekaźnika"
#define L_RELAY_IMPULSE_DURATION "Długośc impulsu"
#define L_RELAY_SWITCH_OFF_AFTER "Wyłącz po"
#define L_RELAY_NO_ACTION_IF_0 "Brak akcji jeśli jest 0"
#define L_RELAY_SWITCH_OFF_ABOVE "Wyłącz powyżej"
#define L_RELAY_SELECT_LED_4_RELAY "LED sygnalizujący stan przekaźnika"
#define L_RELAY_MQTT_TOPIC "Temat MQTT sterujący przekaźnikiem"
#define L_RELAY_SELECT_SENSOR "Wybierz czujnik"
#define L_RELAY_TRIGGERED "Wyzwalany"
#define L_RELAY_TRIGGERED_HIGH_SIGNAL "Sygnałem HIGH"
#define L_RELAY_TRIGGERED_LOW_SIGNAL "Sygnałem LOW"


/* Form: Thermal protection */
#define L_THERMAL_PROTECTOR_ENABLED "Włączony przy uruchomieniu"
#define L_THERMAL_PROTECTORS "Zabezpieczenia termiczne"
#define L_THERMAL_PROTECTOR "Zabezpieczenie"
#define L_THERMAL_PROTECTOR_AUTOMATIC_SWITCHING_OFF "Automatyczne wyłączenie przekaźnika po przekroczeniu zadanej temperatury"
#define L_THERMAL_PROTECTOR_MQTT_TOPIC "Temat MQTT sterujący zabezpieczeniem termicznym"

/* Form: Switch */
#define L_SWITCH_FUNCTIONALITY "Funkcja"
#define L_SWITCH_SYSTEM_BUTTON "Przycisk systemowy"
#define L_SWITCH_CONTROL_RELAY "Tylko sterowanie przekaźnikiem"
#define L_SWITCH_CONTROL_GATE "Tylko sterowanie bramą"
#define L_SWITCH_RELAY_CONTROLLED "Steruje przekaźnikiem"
#define L_SWITCH_TYPE "Typ"
#define L_SWITCH_MONOSTABLE "Monostabilny"
#define L_SWITCH_BISTABLE "Bistabilny"
#define L_SWITCH_SENSITIVENESS_HINT "Czułość należy ustawić metodą prób, aż uzyska się pożądane działanie przycisku podczas jego wciskania"
#define L_SWITCH_BUTTON "Przycisk / Włącznik"
#define L_SWITCH_MQTT_TOPIC "Temat MQTT do monitorowania stanu przycisku"

/* Form: Binary sensor */
#define L_BINARY_SENSOR "Czujnik binarny"
#define L_BINARY_SENSORS "Czujniki binarne"
#define L_BINARY_SENSOR_MCP23017_CONNECTION "Czujnik może zostać podłączony bezpośrednio do GPIO ESP lub przez ekspander MCP23017"
#define L_BINARY_SENSOR_MQTT_TOPIC "Temat MQTT do monitorowania stanu czujnika binarnego"
#define L_BINARY_SENSOR_SENT_REVERTED_STATE "Odwróć sygnał na wejściu"
#define L_BINARY_PULLUP_RESISTOR "Włącz wewnętrzny rezystor (PULLUP)"
#define L_BINARY_SEND_AS_SWITCH "Wysyłaj stan czujnika wartościami: ON/OFF"
#define L_BINARY_SEND_AS_SWITCH_HINT "Domyślnie: OPEN/CLOSED"



/* Form: Analog Input */
#define L_ADC_CANT_CHANGE "brak możliwości zmiany"
#define L_ADC_MEASURED_VOLTAGE "Max napięcie wejściowe na ADC"
#define L_ADC_NUMBER_OF_SAMPLES "Ilość próbek do uśrednienia odczytu"
#define L_ADC_IDX_RAW_DATA "IDX Surowe Dane"
#define L_ADC_IDX_PERCENT "IDX Procent"
#define L_ADC_IDX_VOLTAGE "IDX Napięcie na wejściu"
#define L_ADC_IDX_VOLTAGE_CALCULATED "IDX Napięcie przeliczone"
#define L_ADC_RESISTOR "Rezystor"
#define L_ADC_VOLTAGE_DIVIDER "Dzielnik napięcia"
#define L_ADC_MQTT_TOPIC "Temat MQTT dla informacji z ADC"
#define L_ADC_INPUT "Wejście"

#define L_ADC_HA_VOLTAGE "Napięcie na wejściu"
#define L_ADC_HA_VOLTAGE_CALCULATED "Napięcie"
#define L_ADC_HA_VOLTAGE_RAW "Wartość na wejściu"
#define L_ADC_HA_VOLTAGE_PERCENT "Wartość na wejściu"

#define L_BATTERY_METER "Pomiar naładowania baterii"
#define L_BATTERY_MQTT_TOPIC "Temat MQTT Stanu baterii"
#define L_BATTERY_MIN_LEVEL "Minimalny poziom baterii"
#define L_BATTERY_MAX_LEVEL "Maksymalny poziom baterii"


/* Form: upgrade and post upgrade */
#define L_UPGRADE_SELECT_FIRMWARE "Wybierz Firmware"
#define L_UPGRADE_READ_BEFORE "Przeczytaj przed aktualizacją oprogramowania"

#define L_UPGRADE_INTERUPTED "Nie odświeżaj przeglądarki podczas aktualizacji"
#define L_UPGRADE_DONT_PLUG_OFF "Nie odłączaj urządzenia od źródła zasilania"
#define L_UPGRADE_TIME "Proces aktualizacji potrwa do minuty"
#define L_UPGRADE_AUTO_REBOOT "Urządzenie zostanie automatycznie uruchomione po ukończeniu aktualizacji"

#define L_UPGRADE_VIA_WAN "Pobierz i uaktualnij"
#define L_UPGRADE_VIA_WAN_HINT "Firmware zostanie automatycznie pobrane i wgrane do urządzenia"

#define L_UPGRADE_FROM_FILE "Załaduj plik *.bin"
#define L_UPGRADE "Aktualizuj"

#define L_UPGRADE_FIRMWAR_YOUR_CURRENT_FIRMWARE "Twoje aktualne oprogramowanie"
#define L_UPGRADE_FIRMWARE_DEVICE_NAME "Urządzenie: {{f.d}}"
#define L_UPGRADE_FIRMWARE_DEVICE_ID "ID urządzenia: {{f.n}}"
#define L_UPGRADE_FIRMWARE_TYPE "AFE T{{f.t}}"
#define L_UPGRADE_FIRMWARE_VERSION "Wersja {{f.v}}"
#define L_UPGRADE_FIRMWARE_CHIP "Chip: ESP{{f.e}}"
#define L_UPGRADE_FIRMWARE_FLASH_SIZE "Flash: {{f.s}} sformatowany do: {{f.f}}"
#define L_UPGRADE_FIRMWARE_API "API: {{f.a}}"
#define L_UPGRADE_FIRMWARE_PRO_YES "Wersja Pro: Tak"
#define L_UPGRADE_FIRMWARE_PRO_NO "Wersja Pro: Nie"



#define L_UPGRADE_IN_PROGRESS "Aktualizacja w toku"

#define L_UPGRADE_FAILED "<span style=\"color:red\">Aktualizacja nie powiodła się</span>"
#define L_UPGRADE_SUCCESSFUL "Aktualizacja zakończona pomyślnie"
#define L_UPGRADE_DEVICE_WILL_BE_REBOOTED                                              \
  "Po 15 sekundach urządzenie zostanie zrestartowane. Proszę czekać ..."
#define L_UPGRADE_RESTORE_DEFAULT_SETTINGS "Przywróć ustawienia początkowe"
#define L_RESTORE_CONFIGURATION_WILL_BE_REMOVED                                        \
  "<strong> Ostrzeżenie:</strong> przywrócenie ustawień domyślnych spowoduje usunięcie aktualnej konfiguracji, w tym konfiguracja WiFi"
#define L_RESTORE_IN_PROGRESS "Trwa przywracanie konfiguracji początkowej"
#define L_RESTORE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE "Po 20 sekundach połącz się z siecią Wi-Fi o nazwie twojego urządzenia" 
#define L_RESTORE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE_AFE "Po 20 sekundach połącz się z siecią WiFi o nazwie <strong>AFE Device</strong>" 
#define L_RESTORE_NETWORK_OPEN_PANEL  "Następnie otwórz panel konfiguracyjny : <a href=\"http://192.168.5.1\">http://192.168.5.1</a>"
#define L_UPGRADE_RESTORING_DEFAULT_SETTING "Przywracanie ustawień domyślnych"
#define L_UPGRADE_REBOOT_IN_PROGRESS "Trwa ponowne uruchamianie"
#define L_UPGRADE_SITE_WILL_BE_RELOADED "Strona zostanie automatycznie przeładowana ... czekaj"
#define L_UPGRADE_REBOOT "Ponowne uruchamianie"

#define L_UPGRADE_TIMEOUT "Aktualizacja nie powiodła się. Problem z połączeniem z serwerem oprogramowania (timeout)"
#define L_UPGRADE_CANNOT_CONNECT_TO_SERVER "Aktualizacja nie powiodła się. Problem z połączeniem z serwerem oprogramowania"
#define L_UPGRADE_SERVER_NONE_200 "Aktualizacja nie powiodła się. Błąd z serwerem oprogramowania [HTTP<>200]"
#define L_UPGRADE_FIRMWARE_SIZE_0 "Aktualizacja nie powiodła się. Błąd z serwerem oprogramowania  [Firmware: 0kB]"
#define L_UPGRADE_WRONG_CONTENT_TYPE "Aktualizacja nie powiodła się. Błąd z serwerem oprogramowania [Content type]"
#define L_UPGRADE_SUCCESS_MESSAGE "Firmware: %s został załadowany. Rozmiar: %dkB"
#define L_UPGRADE_NOT_FULL_LOADED "Aktualizacja nie powiodła się. Zapisane tylko %dkB z %dkB"
#define L_UPGRADE_SOMETHING_WRONG "Aktualizacja nie powiodła się. Coś poszło nie tak [Bład: %d]"
#define L_UPGRADE_NO_SPACE "Aktualizacja nie powiodła się. Za mało miejsca, aby rozpocząć ładowanie firmware"
#define L_UPGRADE_NO_CONTENT "Aktualizacja nie powiodła się. Błąd z serwerem oprogramowania [Brak odpowiedzi]"




/* Form: password */
#define L_PASSWORD_SET_PASSWORD "Ustaw hasło"
#define L_PASSWORD_SET_PASSWORD_TO_PANEL "Ustaw hasło do Panelu Konfiguracyjnego"
#define L_PASSWORD_SET_PASSWORD_INFO "Jeśli nie pamiętasz hasła, zresetuj urządzenie do stanu początkowego"
#define L_PASSWORD_PROTECT_BY_PASSWORD "Zabezpiecz hasłem"
#define L_PASWORD_PROTECTED  "\"Operacja niemożliwa. Konfiguracja zabezpieczona hasłem\""

/* Form: Pro version */
#define L_PRO_VERSION "Wersja PRO"
#define L_PRO_KEY "Klucz"
#define L_PRO_VALID "Aktualny"
#define L_PRO_CANNOT_BE_CONFIGURED "Konieczne jest podłączenie do Internetu, aby skonfigurować wersję PRO"

/* Form: Contactron */
#define L_CONTACTRON_LED_ASSIGNED_TO_SENSOR "LED przypisany do czujnika"
#define L_CONTACTRON_SET_SENSITIVENESS "Czułość należy ustawić eksperymentalnie, aż uzyska się pożądane działanie czujnika magnetycznego"
#define L_CONTACTRON "Czujnik magnetyczny"
#define L_CONTACTRONS "Czujniki magnetyczne"
#define L_CONTACTRON_MQTT_TOPIC "Temat MQTT do monitorowania kontaktronu"

/* Gate */
#define L_GATE "Brama/Drzwi"
#define L_GATE_IF_MAGNETIC_SENSOR "Jeśli czujnik magnetyczny"
#define L_GATE_AND_SENSOR "oraz czujnik"
#define L_GATE_IS_OPEN "jest otwarty"
#define L_GATE_ARE_OPEN "są otwarte"
#define L_GATE_THEN "to"
#define L_GATE_IS_CLOSED "jest zamknięty"
#define L_GATE_ARE_CLOSED "są zamknięte"
#define L_GATE_STATES_CONFIGURATION "Konfiguracja stanów bramy"
#define L_GATE_SET_STATE "Ustaw stan bramy na"
#define L_GATE_OPENED "Otwarta"
#define L_GATE_CLOSED "Zamknięta"
#define L_GATE_PARTIALLY_OPENED "Częściowo otwarta"
#define L_GATE_UNKNOWN_STATE "Nieznany stan"
#define L_GATE_CONFIGURATION "Konfiguracja bramy/drzwi"
#define L_GATE_RELAY_ID_CONTROLLING_GATE "Przekaźnik sterujący bramą/drzwiami"
#define L_GATE_MQTT_TOPIC "Temat MQTT sterujący bramą/drzwiami"
#define L_GATE_IDX_STATE "IDX Stan bramy/drzwi"

/* HPMA11BS0 */
#define L_PARTICLE_SENSORS "Czujniki pyłów"
#define L_HPMA115S0_SENSOR "HPMA115S0"
#define L_HPMA115S0_POST_SLEEP_INTERVAL                                              \
  "Jeśli poniższa wartość jest większa od 0 to czujnik będzie usypiany "    \
  "między odczytami. Wartość poniżej definiuje na ile sekund przed odczytem " \
  "czujnik ma zostać uruchomiony. Wartość musi być mniejsza niż interwał "  \
  "pomiarów"
#define L_HPMA115S0_MEASURE_AFTER "Pomiar po czasie"
#define L_HPMA115S0_MQTT_TOPIC "Temat MQTT czujnika HPMA115S0"
#define L_HPMA115S0_WHO_NORMS "Normy WHO średniego dobowego stężenia pyłów"
#define L_HPMA115S0_WHO_NORMS_HINT "Normy WHO możesz nadpisać własnymi lub obowiązującymi w danym kraju"
#define L_HPMA115S0_WHO_NORM_UNIT "średnie dobowe w µg/m3"




/* BMEX80 */
#define L_BMEX80_SENSOR "Czujnik BMEx80"
#define L_BMEX80_SENSORS "Czujniki BMEx80"
#define L_BMEX80_IDX_GAS_SENSOR "IDX Czujnik gazu"
#define L_BMEX80_IDX_IQA "IDX IQA"
#define L_BMEX80_IDX_STATIC_IAQ "IDX Statyczny IQA"
#define L_BMEX80_IDX_CO2_EQUVALENT "IDX CO2"
#define L_BMEX80_IDX_BVOC_EQUIVALENT "IDX BVOC"
#define L_BMEX80_MQTT_TOPIC "Temat MQTT czujnika BMEx80"
#define L_BMEX80_SENSOR_TYPE "Typ czujnika BMEx80"
#define L_BMEX80_REFRESH_SETTINGS "Pokaż/Odśwież parametery konfiguracyjne po ustawieniu/zmianie typu czujnika"
#define L_BMEX80_ALTITIDE "Wysokość nad poziomem morza"
#define L_BMEX80_METERS "Metrów"

/* BH1750 */
#define L_BH1750_SENSOR "Czujnik BH1750"
#define L_BH1750_SENSORS "Czujniki BH1750"
#define L_BH1750_MQTT_TOPIC "Temat MQTT czujnika BH1750"
#define L_BH1750_CONTINUES_1_LUX "Ciągły odczyt: ~1 lux 120ms"
#define L_BH1750_CONTINUES_05_LUX "Ciągły odczyt: ~0.5 lux 120ms"
#define L_BH1750_CONTINUES_4_LUX "Ciągły odczyt: 4 lux 16ms"
#define L_BH1750_ONE_READ_1_LUX "Jeden odczyt: 1 lux 120ms"
#define L_BH1750_ONE_READ_05_LUX "Jeden odczyt: 0.5 lux 120ms"
#define L_BH1750_ONE_READ_4_LUX "Jeden odczyt: 4 lux 16mss"

/* TSL2561 */
#define L_TSL2561_SENSOR "Czujnik TSL2561"
#define L_TSL2561_SENSORS "Czujniki TSL2561"
#define L_TSL2561_MQTT_TOPIC "Temat MQTT czujnika TSL2561"
#define L_TSL2561_SENSITIVENESS_LOW "Niska; szybkie odczyty: 14ms"
#define L_TSL2561_SENSITIVENESS_MID "Średnia; dość szybkie odczyty: 100ms"
#define L_TSL2561_SENSITIVENESS_HIGH "Wysoka; wolne odczyty: 400ms"
#define L_TSL2561_GAIN_AUTO "Automatyczne"
#define L_TSL2561_GAIN_NONE "1x (brak)"
#define L_TSL2561_GAIN_16 "16x"
#define L_TSL2561_GAIN_ILUMINANCE "IDX Natężenie światła"
#define L_TSL2561_GAIN_BROADBAND "IDX Światło widzialne"
#define L_TSL2561_GAIN_IR "IDX Podczerwień"

/* AS3935 */
#define L_AS3935_SENSOR "Czujnik AS3935"
#define L_AS3935_NOISE_FLOOR "Poziom hałasów"
#define L_AS3935_NOISE_FLOOR_HINT "(0-najmniejszy, 7-największy)"
#define L_AS3935_WATCHDOG_THRESHOLD "Czułość"
#define L_AS3935_WATCHDOG_THRESHOLD_HINT "(1-najmniejsza, 10-największa)"
#define L_AS3935_SPIKES_REJECTION "Poziom odrzucenia skoków"
#define L_AS3935_SPIKES_REJECTION_HINT "(1-najmniejszy, 11-największy)"
#define L_AS3935_MIN_SPIKES "Minimalna ilość piorunów, aby zgłosić detekcję burzy"
#define L_AS3935_AUTOMATIC_NOISE_FLOOR_CONTROL "Inteligentna kontrola poziomu hałasu"
#define L_AS3935_SET_LEVEL_OF_NOISE_FLOOR "Ustaw poziom hałasu, w przypadku wyłączonej opcji Inteligentnej kontroli hałasu"
#define L_AS3935_SENSOR_INDOOR_OUTDOOR "Lokalizacja czujnika"
#define L_AS3935_INDOOR "Wewnątrz budynku"
#define L_AS3935_OUTDOOR "Na zewnątrz"
#define L_AS3935_MQTT_TOPIC "Temat MQTT czujnika AS3935"

/* DS18B20 */
#define L_DS18B20_SENSOR "Czujnik temperatury DS18B20"
#define L_DS18B20_SENSORS "Czujniki DS18B20"
#define L_DS18B20_SENT_ONLY_CHANGES "Wysyłać dane tylko, gdy wartość temperatury zmieni się"
#define L_DS18B20_TEMPERATURE_CORRECTION "Korekta wartości o"
#define L_DS18B20_MQTT_TOPIC "MQTT Temat informacji z czujnika DS18B20"
#define L_DS18B20_SEARCH "Skanuj GPIO"
#define L_DS18B20_NO_SENSOR "Żaden czujnik nie został jeszcze znaleziony. Sprawdź, czy jest poprawnie podłączony, a następnie przeskanuj GPIO"
#define L_DS18B20_RESOLUTION "Rozdzielczość"
#define L_DS18B20_RESOLUTION_9B "Niska (9bit)"
#define L_DS18B20_RESOLUTION_10B "Dostateczna (10bit)"
#define L_DS18B20_RESOLUTION_11B "Dobra (11bit)"
#define L_DS18B20_RESOLUTION_12B "Wysoka (12bit)"

/* DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
#define L_DHT_SENSORS "Czujniki DHT"
#define L_DHT_CONTROLLING_PARAMETER "Parametr sterujący"
#define L_DHT_SENT_ONLY_CHANGES "Wysyłać dane tylko, gdy wartość temperatury lub wilgotności zmieniły się"
#define L_DHT_MQTT_TOPIC "MQTT Temat informacji z czujnika DHT"
#define L_DHT_SENSOR_TYPE "Typ czujnika"
#define L_DHT_AUTO_DETECT "Wykryj automatycznie"
#endif // AFE_CONFIG_HARDWARE_DHT


#if defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY) 
const char dewPointPerception_Dry[] PROGMEM = "Dla niektórych trochę sucho";
const char dewPointPerception_VeryComfy[] PROGMEM = "Bardzo komfortowo";
const char dewPointPerception_Comfy[] PROGMEM = "Komfortowo";
const char dewPointPerception_Ok[] PROGMEM = "W porządku dla większości, ale wszyscy odczuwają wilgoć przy górnej krawędzi";
const char dewPointPerception_UnComfy[] PROGMEM = "Trochę niekomfortowo dla większości ludzi na górnej krawędzi";
const char dewPointPerception_QuiteUnComfy[] PROGMEM = "Bardzo wilgotne, dość niekomfortowo";
const char dewPointPerception_VeryUnComfy[] PROGMEM = "Niezwykle niekomfortowo, przytłaczająco";
const char dewPointPerception_SevereUnComfy[] PROGMEM = "Groźnie, nawet śmiertelny w przypadku chorób związanych z astmą";
const char* const dewPointPerception[] PROGMEM = { dewPointPerception_Dry, dewPointPerception_VeryComfy,dewPointPerception_Comfy,dewPointPerception_Ok,dewPointPerception_UnComfy,dewPointPerception_QuiteUnComfy,dewPointPerception_VeryUnComfy,dewPointPerception_SevereUnComfy};

const char comfort_OK[] PROGMEM           = "OK";
const char comfort_TooHot[] PROGMEM       = "Za gorąco";
const char comfort_TooCold[] PROGMEM      = "Za zimno";
const char comfort_TooDry[] PROGMEM       = "Za sucho";
const char comfort_HotAndDry[] PROGMEM    = "Gorąco i sucho";
const char comfort_ColdAndDry[] PROGMEM   = "Zimno i sucho";
const char comfort_Unknown[] PROGMEM      = "?";
const char comfort_TooHumid[] PROGMEM     = "Za wilgotno";
const char comfort_HotAndHumid[] PROGMEM  = "Gorąco i wilgotno";
const char comfort_ColdAndHumid[] PROGMEM = "Zimno i wilgotno";
const char* const Comfort[] PROGMEM = {comfort_OK,comfort_TooHot,comfort_TooCold,comfort_TooDry,comfort_HotAndDry,comfort_ColdAndDry,comfort_Unknown,comfort_TooHumid,comfort_HotAndHumid,comfort_ColdAndHumid};
#endif


/* Anemometer Sensor */
#define L_ANEMOMETER_SENSOR "Czujnik siły wiatru"
#define L_ANEMOMETER_SENSITIVENESS_HINT "Czułość czujnika należy ustawić metodą prób i błędów, aż do uzyskania zamierzonego efektu. Im wartość jest mniejsza tym czułość jest większa"
#define L_ANEMOMETER_CALIBRATION "Kalibracja anemometru"
#define L_ANEMOMETER_IMPULSE_DISTANCE_HINT  "Odległość jaką wiatr przebędzie w jednym impulsie anemometra"
#define L_ANEMOMETER_IMPULSE_DISTANCE "Odległość"

/* Rainmeter Sensor */
#define L_RAINMETER "Czujnik deszczu"
#define L_RAINMETER_CALIBRATION "Rozdzielczość czujnika deszczu"
#define L_RAINMETER_RESOLUTION "Rozdzielczość"

/* REGULATOR */
#define L_REGULATOR_REGULATION "Regulacja"
#define L_REGULATORS "Regulatory"
#define L_REGULATOR "Regulator"
#define L_REGULATOR_TURN_ON_IF "Włącz jeśli wartość jest"
#define L_REGULATOR_TURN_OFF_IF "Wyłącz jeśli wartość jest"
#define L_REGULATOR_LOWER "mniejsza"
#define L_REGULATOR_HIGHER "większa"
#define L_REGULATOR_ENABLED "Włączony po uruchomieniu"
#define L_REGULATOR_MQTT_TOPIC "Temat MQTT sterujący regulatorem"

#define L_REGULATOR_CP_TEMPERATURE "Temperatura"
#define L_REGULATOR_CP_HEAT_INDEX "Temperatura odczuwalna"
#define L_REGULATOR_CP_HUMIDITY "Wilgotność"
#define L_REGULATOR_CP_ABSOLOUTE_HUMIDITY "Wilgotność absolutna"
#define L_REGULATOR_CP_DEW_POINT "Punkt rosy"


/* Generic Sensor: Temperature */
#define L_TEMPERATURE "Temperatura"
#define L_TEMPERATURE_IDX "IDX Temperatura"


/* Generic Sensor: Humidity */
#define L_HUMIDITY "Wilgotność"
#define L_HUMIDITY_IDX "IDX Wilgotność"
#define L_HUMIDITY_IDX_TEMP_HUM "IDX Temp/Wilgotność"
#define L_HUMIDITY_IDX_TEMP_HUM_BAR "IDX Temp/Wilg/Bar"
#define L_HUMIDITY_IDX_ABSOLUTE "IDX Wilgotność absolutna"
#define L_HUMIDITY_IDX_DEW_POINT "IDX Punkt rosy"
#define L_HUMIDITY_IDX_HEAT_INDEX "IDX Temp.odczuwalna"

#define L_COMFORT_IDX "IDX Komfort"
#define L_PERCEPTION_IDX "IDX Percepcja"

/* Generic Sensor: Pressure */
#define L_PRESSURE "Ciśnienie"
#define L_PRESSURE_IDX "IDX Ciśnienie"
#define L_PRESSURE_IDX_ALT "Czujnik: wysokość n.p.m"
#define L_PRESSURE_IDX_RELATIVE_PRESSURE "IDX Ciśnienie względne"


/* MCP32017 */
#define L_MCP23017_CONNECTION "Podłączenie"
#define L_MCP23017_LED_CONNECTION "LED może zostać podłączony bezpośrednio do GPIO lub przez ekspander MCP23017"
#define L_MCP23107_LED_ADDITIONAL_SETTINGS "Dodatkowe ustawienia"
#define L_MCP23017_RELAY_CONNECTION "Przekaźnik może zostać podłączony bezpośrednio do GPIO lub przez ekspander MCP23017"
#define L_MCP23017_RELAY_TRIGGERED "Wyzwalanie przekaźnika"
#define L_MCP23017_SWITCH_CONNECTION "Przycisk może zostać podłączony bezpośrednio do GPIO lub przez ekspander MCP23017"
#define L_MCP23017_CONNECTION_VIA_MCP "Podłączenie przez MCP23017 (GPIO powyżej musi zostać ustawione na BRAK)"


/* PN532 */

#define L_PN532_SENSOR "Czujnik PN532"
#define L_PN532_MIFARE_ADMIN "Zarządzanie kartą MiFare Classic"
#define L_PN532_CARD_FORMAT "Formatowanie karty"
#define L_PN532_CARD_FORMAT_HINT "Formatowanie karty usuwa wszystkie zapisane na niej informacje."
#define L_PN532_FORMAT_NFC "<a href=\"/?o=38&i={{o}}\">Formatuj NFC</a>"
#define L_PN532_FORMAT_MINIFARE "<a href=\"/?o=38&i={{o}}\">Formatuj Classic MiFare</a>"
#define L_PN532_READ_TAG "<div class=\"cf\"><label> </label><a class=\"b bc\" href=\"/?o=38&i={{o}}\">Odczytaj aktualny TAG</a></div>"
#define L_PN532_SAVE_TAG "Zapisywanie informacji na karcie"
#define L_PN532_INFO_1 "Kartę powinna zostać sformatowana do NFC"
#define L_PN532_INFO_2 "Informacje zapisywane na karcie mogą zawierać cyfry, znaki alfabetu (ale bez PL liter) oraz podstawowe znaki specjalne"
#define L_PN532_INFO_3 "Kartę należy umieścić przy czytniku i NIE ruszać podczas formatowania, odczytywania oraz zapisywania informacji"
#define L_PN532_CURRENT_TAG "Aktualny inforacje zapisane na karcie"
#define L_PN532_CURRENT_PRIMARY_TAG "TAG"
#define L_PN532_CURRENT_BACKUP_TAG "TAG: zapasowa kopia"
#define L_PN532_TAG_ID "Identyfikator"
#define L_PN532_TAG_WHO "Użytkownik"
#define L_PN532_LED "LED sygnalizujące stany czujnika PN582"
#define L_PN532_REQUEST_PROCESSING "Czas przetwarzania żądania po detekcji karty"
#define L_PN532_TIMEOUT "Czas oczekiwania na kartę"
#define L_PN532_INTERFACE "Interface"
#define L_PN532_LISTENER_TIMEOUT "Interwał odczytów karty"
#define L_PN532_SHOW_INTERFACE_CONFIGURATION "Pokaż parametery konfiguracyjne interface'u"
#define L_PN532_SEARCH_I2C_CONNECTED_DEVICES "Szukaj urządzeń na porcie I2C"
#define L_PN532_TIMEOUTS "Konfiguracja odczytów"
#define L_PN532_MQTT_TOPIC "Temat MQTT do wysyłania danych odczytanej kary MiFare"


#define L_MIFARE_CARD "Karta MiFare Classic"
#define L_MIFARE_CARDS "Karty MiFare Classic"
#define L_MIFARE "MiFare"
#define L_MIFARE_CARD_ID "Identyfikator Karty"
#define L_MIFARE_CARD_CONTROLS_RELAY_GATE "Sterowanie przekaźnikem / bramą"
#define L_MIFARE_CARD_CONTROLS "Steruje"
#define L_MIFARE_CARD_ACTION "Akcja"
#define L_MIFARE_CARD_ACTION_ON "ON"
#define L_MIFARE_CARD_ACTION_OFF "OFF"
#define L_MIFARE_CARD_ACTION_TOGGLE "Przełącz"
#define L_MIFARE_CARD_INTEGRATION "Integracja"
#define L_MIFARE_CARD_INTEGRATION_HINT "Konfiguracja integracji z systemem automatyki"
#define L_MIFARE_CARD_SEND_AS_SWITCH "Wysyłaj detekcję kart wartościami: ON/OFF"
#define L_MIFARE_CARD_SEND_AS_SWITCH_HINT "Domyślnie: OPEN/CLOSED"
#define L_MIFARE_CARD_MQTT_TOPIC "Temat do monitorowania odczytów karty MiFare"
#define L_MIFARE_CARD_HOW_LONG_KEEP_STATE "Jak długo utrzymywać stan CLOSED (ON) po wykryciu karty"
#define L_MIFARE_CARD_TIME "Czas"
#define L_MIFARE_CARD_NONE_PRO "Wersja standardowa AFE Firmware obsługuje do 4 kart MiFare Classic.<br>W wersji AFE Pro dostępna jest obsługa do 10 kart.<br>Więcej informacji o <a href=\"https://afe.smartnydom.pl/pl/postawowe-informacje/wersja-pro\">AFE Pro</a>"
#define L_MIFARE_CARD_BACKUP_TAG "Zapasowa kopia informacji zapisana na karcie"


#define L_MIFARE_ADMIN_INFO "Informacje"


#define L_CLED_STRIP "Pasek RGB LED"
#define L_CLED_BRIGHTNESS "Jasność"
#define L_CLED_COLOR_RGB "Kolor"
#define L_CLED_CONFIGURATION "Konfiguracja RGB LED"
#define L_CLED_ONOFF_CONFIGURATION "Konfiguracja wł/wył"
#define L_CLED_ONOFF_CONFIGURATION_ON "RGB LED włączony"
#define L_CLED_ONOFF_CONFIGURATION_OFF "RGB LED wyłączony"
#define L_CLED_EFFECTS_CONFIGURATION "Konfiguracja efektów świetlnych"
#define L_CLED_EFFECTS_URL "<a href=\"/?o={{u.u}}&i={{u.i}}\">{{u.l}}</a>"
#define L_CLED_TIMEOUT "Jak długo utrzymywać stan"

#define L_CLED_EFFECT_LABEL "Efekty świetlne"
#define L_CLED_EFFECT_OFF_COMMAND "Wyłącz"
#define L_CLED_EFFECT_BLINKING_CONFIGURATION "Efekt: migania"
#define L_CLED_EFFECT_FADE_IN_OUT_CONFIGURATION "Efekt: zanikanie"
#define L_CLED_EFFECT_WAVE_CONFIGURATION "Efekt: fali"

#define L_CLED_EFFECT_WAVE_ON_LED_COLOR "Kolor aktywnej diody"
#define L_CLED_EFFECT_WAVE_OFF_LED_COLOR "Kolor nieaktywnych diod"
#define L_CLED_EFFECT_WAVE_SPEED "Szybkość fali"

#define L_CLED_MAX_BRIGHTNESS "Max jasność"
#define L_CLED_MIN_BRIGHTNESS "Min jasność"
#define L_CLED_FADE_IN_OUT_TIMEOUT "Szybkość zanikania"

#define L_CLED_ON_OFF_IDX "IDX Wł/Wył"
#define L_CLED_EFFECTS_IDX "IDX Efekty"
#define L_CLED_MQTT_TOPIC "Temat MQTT kontrolujący RGB LED"

#define L_CLED_MQTT_ON_OFF_TOPIC "Temat Wł/Wył"
#define L_CLED_MQTT_EFFECTS_TOPIC "Temat efektów"

#define L_CLEDS_HINT "Ta wersja firmware nie umożliwia zmianę GPIO paska RGB LED: WS281x. Pasek musi zostać podłączony jak poniżej"
#define L_CLED_NUMBER_OF_LEDS "Ilość diod w pasku LED"

#define L_CLED_API_BRIGHTNESS_CONVERSION "Zakres wartości jasności"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_255 "0 .. 255"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_100 "0..100"
#define L_CLED_API_BRIGHTNESS_CONVERSION_0_1 "0..1"

/* Home Assistant */
#define L_HA_DISCOVERY_TOPIC "Temat nasłuchu konfiguracji urządzeń"
#define L_HA_PUBLISHING "Publikować/aktualizować konfiguracje urządzenia?"
#define L_HA_UPDATE_REMOVE "Usuwać nieaktualne konfiguracje?"


#endif // _LANG_PL_h

