---
title: 'T4 - ekspander MCP23017'
published: true
recaptchacontact:
    enabled: false
---

**Firmware umożliwia**

* sterowanie bezprzewodowo do trzydziestu dwóch urządzeń / układów elektrycznych z wykorzystaniem ekspandera portów MCP23017 pracującego na sznine IIC
* obsługę jeden czujnik DS18B20 [w wersji PRO][/postawowe-informacje/wersja-pro]

**Sprzęt:**
* ESP8286/8285 1Mb do 4Mb
* do 2 expanderów MCP23017 portów GPIO
* do 32 przekaźników
* do 32 przycisków: mono/bi-stabilnych
* do 32 diod LED
* do 32 czujników dwustanowych (PIR, Magnetyczny, kontaktron itp)
* do 1 czujnika DS18B20  [w wersji PRO][/postawowe-informacje/wersja-pro]
* 1 wejście do przetwarzania sygnałów analogowo-cyfrowych - [w wersji PRO](/postawowe-informacje/wersja-pro)

**API**
* HTTP
* MQTT
* Domoticz HTTP API
* Domoticz MQTT API

** Funkcje **
* Możliwość dokonania korekty wartości czujnika DS18B20
* Sterowanie przekaźnikiem bezprzewodowo przez sieć WiFi 
* Ustawiania stanu początkowego przekaźnika po uruchomieniu
* Ustawienie stany przekaźnika po podłączeniu się do brokera MQTT
* Automatyczne wyłączanie przekaźnika po zadanym czasie
* Sterowanie przekaźnikiem zewnętrznym włącznikiem/przyciskiem mono (dzwonkowym) lub bistabilnymi
* LED może informwać o stanie przekaźnika
* Odczyty z przetwornika analogowo-cyfrowego [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Obsługa wiadomości LWT (dla MQTT)
* Konfiguracji przechowywania wiadomości MQTT przez broker (MQTT Retain)
* Konfiguracja przez przeglądarkę w języku PL lub EN
* Możliwość nadania stałego adresu IP dla urządzenia
* Aktualizacja oprogramowania przez przeglądarkę




---