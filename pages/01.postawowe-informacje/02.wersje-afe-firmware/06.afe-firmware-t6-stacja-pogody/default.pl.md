---
title: 'T6 - Stacja pogody'
recaptchacontact:
    enabled: false
---

**Firmware umożliwia**

* odczytywanie warunków atmosferycznych 
* sterowanie bezprzewodowo 1 urządzeniem / układem elektrycznym

**Sprzęt:**
* ESP8286/8285
	* Dla wersji 2.x wymagany jest ESP: ESP12, 12E, 12F, 13, 14, 07s lub inny z min 4MB pamięci Flash 
* 1 przekaźnik
* do 3 przycisków: mono/bi-stabilnych
* do 2 diod LED
* 1 czujnik Bosch BME680, BME280, BMP180 lub BMP085
	* BMP085, BMP180 - temperatura, ciśnienie, ciśnienie względne (zrobione)
	* BME280 - temperatura, wilgotność, wilgotność absolutna, ciśnienie względne oraz bezwzgędne, punkt rosy, temperatura odczuwalna, poziom percepcji oraz komfortu odbioru warunków atmosferycznych
	* BME680 - wszystko co BME280 oraz IAQ, Statyczne IAQ, Ekwiwalent CO2, Breath VOC Ekwiwalent, poziom percepcji oraz komfortu odbioru warunków atmosferycznych 
* 1 czujnik BH1750 (natężenia światła)
* 1 czujnik Honeywell HPMA115S0 (pyłów zawieszonych 2.5 oraz 10) oraz poziom przekroczeń stężenia pyłów wg zakresów WHO
* 1 czujnik DS18B20
* 1 czujnik DHT11,21 lub 22
* 1 czujnik AS3935 (odległości od burzy) - [w wersji PRO: dodane do testów](/postawowe-informacje/wersja-pro)
* 1 czujnik deszczu
* 1 czujnik wiatru
* 1 wejście do przetwarzania sygnałów analogowo-cyfrowych - [w wersji PRO](/postawowe-informacje/wersja-pro)

**API**
* HTTP
* MQTT
* Domoticz HTTP API
* Domoticz MQTT API

** Funkcje **
* Pomiary warunków atmosferycznych w zależności od wykorzystanych czujników, patrzy wyżej
* Informacje o ratingu warunków atmosferycznych (HTTP oraz Standardowe MQTT)
* Możliwość dokonania korekty wartości z czujników
* Wykrywanie odległości od burzy do 50km [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Usypianie czujnika Honeywell HPMA115S0
* Ustawianie dowolnych GPIO dla portu I2C oraz UART
* Sterowanie przekaźnikiem bezprzewodowo przez sieć WiFi 
* Ustawiania stanu początkowego przekaźnika po uruchomieniu
* Ustawienie stany przekaźnika po podłączeniu się do brokera MQTT
* Automatyczne wyłączanie przekaźnika po zadanym czasie
* Sterowanie przekaźnikiem zewnętrznym włącznikiem/przyciskiem mono (dzwonkowym) lub bistabilnymi
* LED może informwać o stanie przekaźnika
* Odczyty z przetwornika analogowo-cyfrowego [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Pomiar stanu naładowania baterii [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Obsługa wiadomości LWT (dla MQTT)
* Konfiguracja przez przeglądarkę w języku PL lub EN
* Możliwość nadania stałego adresu IP dla urządzenia
* Aktualizacja oprogramowania przez przeglądarkę

** Gotowe moduły **
* [Stacja Pogody v2](https://www.smartnydom.pl/weather-station-stacja-pogodowa-v2/)


!! Wersja 2.0 nie obsługuje ESP8266 1MB, ESP8285 ze względu na ograniczone rozmiary pamięci Flash. Jest możliwość wgrania AFE T6 2.x do ESP z rozmiarem pamięći 1MB natomiast nie będzie możliwa aktualizacja firmware przez przeglądarkę www




---