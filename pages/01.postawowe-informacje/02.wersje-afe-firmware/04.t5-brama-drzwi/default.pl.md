---
title: 'T5 - Brama/Drzwi'
recaptchacontact:
    enabled: false
---

**Firmware umożliwia**

* dodanie sterowania bezprzewodowego do istniejącego systemu do sterowania bramą/drzwiami oraz 
* zintegrowania sterowania bramą/drzwiami z systemem do automatyki

!!!! Urządzenie z AFE Firmware nie jest samodzielnym sterownikiem bramy. Urządzenie dodane do istniejącego sterownika bramy wzbogaca go o bezprzewodową funkcję i możliwość podłączenie bramy do systemów automatyki, jak: Domoticz, openHAB itp

**Sprzęt:**
* ESP8286/8285
* do 2 przekaźników
* do 3 przycisków: mono/bi-stabilnych
* do 4 diod LED
* Do 4 kontaktronów (czujników magnetycznych, krańcowych itp)
* Czujnik [Bosch BMP180/BME280/BME680](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx) (temperatura, wilgotność, ciśnienie, inne) [[W wersji PRO](/postawowe-informacje/wersja-pro)]
* Czujnik [BH1750](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750) - natężenie światła [[W wersji PRO](/postawowe-informacje/wersja-pro)]

! Czujniki Bosch BMx80 oraz BH1750 dostępne są **tylko** w 4Mb wersji AFE Firmware T5. 

**API**
* HTTP
* MQTT
* Domoticz HTTP API
* Domoticz MQTT API

** Funkcje **
* Sterowanie do dwóch bram/drzwi/furtek lub dwóch przekaźników lub kombinacji: jedna brama i jeden przekaźnik
* Bezprzewodowe sterowanie bramą/drzwiami/przekaźnikiem (przez sieć WiFi) 
* Funkcje związane z sterowaniem bramą/drzwiami
	*  Stan otwarcia/zamknięcia bramy może być monitorowany przez jeden lub dwa kontaktrony,czujniki magnetyczne, krańcowe itp
	*  Długość podtrzymania przekaźnika (długość impulsu) ustawiany jest w zakresie od 1 milisekundy do 100 sekundy
	*  Stan bramy/drzwi jest wysyałany do systemu automatyki
	*  Stan każdego kontaktronu może być wysyałany do systemu automatyki
	*  Stan kontraktonu może być sygnalizowany przed diodę LED  
* Funkcje przekaźnika
	* Ustawiania stanu początkowego przekaźników po uruchomieniu
	* Ustawienie stany przekaźników po podłączeniu się do brokera MQTT
	* Automatyczne wyłączanie przekaźników po zadanym czasie
	* Sterowanie przekaźnikami zewnętrznymi włącznikami/przyciskami mono (dzwonkowym) lub bistabilnymi
	* LED może informwać o stanie przekaźnika
	* Przekaźnik ustawiany sygnałem LOW lub HIGH
* Odczyty z przetwornika analogowo-cyfrowego [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Pomiar temperatury, wilgotności, ciśnienia względnego i bezwzględnego, punktu rosy, IAQ, BVOC - w zależności od zainstalowanego czujnika Bosch [[w wersji PRO](/postawowe-informacje/wersja-pro)]
* Pomiar natężeni światła (lux) [[W wersji PRO](/postawowe-informacje/wersja-pro)]
* Obsługa wiadomości LWT
* Konfiguracja przez przeglądarkę w języku PL lub EN
* Możliwość nadania stałego adresu IP dla urządzenia
* Aktualizacja oprogramowania przez przeglądarkę

!!! Wersja 2.x nie obsługuje czujnika temperatury oraz wilgotności DHTxx.

** Gotowe moduły **
* Idealnie pasuje do [iECS Sterownik Bramą v2](https://www.smartnydom.pl/sterownik-bramy-wersja-v2/?target=_blank), [iECS Sterownik bramą v1](https://www.smartnydom.pl/forum/uklady-esp8266-i-inne-zrob-to-sam/sterownik-bramy-dla-afe-firmware-t5/?target=_blank), własnych konstrukcji opartych na ESP8266/8285

!! Firmware nie powinien być instalowany do urządzeń Itead Sonoff, gdyż standardowo na wyjściu tych urządzeń jest napięcie. Oczywiście jest możliwość przerobienia tych urządzeń tak, aby wyjście było beznapięciowe. Niemniej jednak nie zachęcam do takich przeróbek.

---