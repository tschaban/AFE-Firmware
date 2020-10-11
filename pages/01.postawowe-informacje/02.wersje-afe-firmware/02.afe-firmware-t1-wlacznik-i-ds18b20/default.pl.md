---
title: 'T1 - włącznik i DS18B20'
published: true
recaptchacontact:
    enabled: false
---

**Firmware umożliwia**

* sterowanie bezprzewodowo do czterech urządzeń / układów elektrycznych
* obsługę do ośmiu czujników DS18B20

**Sprzęt:**
* ESP8286/8285
* 4 przekaźnik
* do 5 przycisków: mono/bi-stabilnych
* do 5 diod LED
* do 8 czujników DS18B20
* 1 wejście do przetwarzania sygnałów analogowo-cyfrowych - [w wersji PRO](/postawowe-informacje/wersja-pro)

! W wersji MEGA, dostępnych jest 8 przkaźników, 8 przycisków oraz diod LED. Natomiast nie wszystkie GPIO można swobodnie wykorzystywać w ESP. Niektóre GPIO mają swoje specyficzne funkcje i zadania. Podłączanie do nich przekaźników, przycisków itp. mogą powodować niestabilność urządzenia

**API**
* HTTP
* MQTT
* Domoticz HTTP API
* Domoticz MQTT API

** Funkcje **
* Możliwość dokonania korekty wartości czujnika DS18B20
* Do czterech niezależnie działających termostatów (regulatorów)
* Do czterech niezależnie działających zabezpiczeń termicznych przekaźników
* Sterowanie przekaźnikiem bezprzewodowo przez sieć WiFi 
* Ustawiania stanu początkowego przekaźnika po uruchomieniu
* Ustawienie stany przekaźnika po podłączeniu się do brokera MQTT
* Automatyczne wyłączanie przekaźnika po zadanym czasie
* Sterowanie przekaźnikiem zewnętrznym włącznikiem/przyciskiem mono (dzwonkowym) lub bistabilnymi
* LED może informwać o stanie przekaźnika
* Odczyty z przetwornika analogowo-cyfrowego [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Obsługa wiadomości LWT (dla MQTT)
* Konfiguracja przez przeglądarkę w języku PL lub EN
* Możliwość nadania stałego adresu IP dla urządzenia
* Aktualizacja oprogramowania przez przeglądarkę




---