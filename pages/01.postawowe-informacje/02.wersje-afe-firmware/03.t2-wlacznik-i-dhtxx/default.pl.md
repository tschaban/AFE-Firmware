---
title: 'T2 - włącznik i DHT'
published: true
recaptchacontact:
    enabled: false
---

**Firmware umożliwia**

* sterowanie bezprzewodowo do czterech urządzeń / układów elektrycznych
* obsługę do czterech czujników DHT (DHT11,DHT22,AM2303,RHT03)

**Sprzęt:**
* ESP8286/8285
* 4 przekaźnik
* do 5 przycisków: mono/bi-stabilnych
* do 5 diod LED
* do 4 czujników DHT
* 1 wejście do przetwarzania sygnałów analogowo-cyfrowych - [w wersji PRO](/postawowe-informacje/wersja-pro)

! W wersji MEGA, dostępnych jest 8 przkaźników, 8 przycisków, 8 czujników DHT oraz diod LED. Natomiast uwaga, nie wszystkie GPIO można swobodnie wykorzystywać w ESP. Niektóre GPIO mają swoje specyficzne funkcje i zadania. Podłączanie do nich przekaźników, przycisków itp. mogą powodować niestabilność urządzenia

**API**
* HTTP
* MQTT
* Domoticz HTTP API
* Domoticz MQTT API

** Funkcje **
* Możliwość dokonania korekty wartości czujnika DHT
* Do czterech niezależnie działających regulatorów sterowanych temperatura (termostat), wilgotnością (higrostat), wilgotnościa aboslutną, temperaturą odczuwalną, punktem rosy
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