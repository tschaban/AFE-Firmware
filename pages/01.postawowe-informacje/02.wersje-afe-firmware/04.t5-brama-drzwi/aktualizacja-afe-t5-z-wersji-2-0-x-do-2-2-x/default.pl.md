---
title: 'Aktualizacja AFE T5 z wersji 2.0.x do 2.2.x'
recaptchacontact:
    enabled: false
---

AFE Firmware T5 2.2.x dostępne jest w dwóch wersjach jeśli chodzi o rozmiar pamięci Flash ESP8266
* 1Mb
* 4Mb

AFE Firmware T5 4Mb dodatkowo zawiera obsługę czujników
* Bosch z serii BMx80 oraz
* czujnika natężenia światła BH1750

!!! Jeśli nie będziesz używać czujników Bosch lub BH1750 to zaktualizuj AFE Firmware do wersję AFE T5 1Mb

!!! Jeśli wgrywasz wersję za pomocą adaptera USB (TTL) wgraj wersję 4Mb

##### Aktualizacja AFE Firmware T5 z wersji 2.0.x do wersji 2.2.x 4Mb przez WiFi

!! **Uwaga** Aktualizacja z wersji 2.0.x do 2.2.x 4Mb powoduję utratę konfiguracji. Po aktualizacji konieczna jest ponowna konfiguracja urządzenia, [jak za pierwszym razem](https://afe.smartnydom.pl/pl/instalacja/instalacja-zanim-zainstalujesz-firmware/pierwsze-uruchomienie).

Aby dokonać aktualizację przez WiFi
1. Upewnij się, że masz wgrane do urządzenia AFE Firmware T5 w wersji 2.0.x. Nie może to być wersja deweloperska z włączonymi log'ami
2. W panelu konfiguracyjnym zaktualizuj AFE Firmware do wersji T5 2.2.x. Nie może to być wersja deweloperska z włączonymi log'ami
3. Po aktualizacji ESP8266 będzie formatowane do 4Mb. 
4. Reset urządzenia potrwa dłużej niż standardowa aktualizacja. Poczekaj ok 1-2min
5. Jeśli LED w ESP8266 zacznie migać, urządzenie gotowe jest do konfiguracji
6. Połącz się z AccessPoint'em o nazwie AFE-Device, a następnie uruchom przeglądarkę internetową z adresem: http://192.168.5.1
7. Więcej o pierwszym uruchomieniu znajdziesz [tutaj](/instalacja/instalacja-zanim-zainstalujesz-firmware/pierwsze-uruchomienie)

W razie pytań zadaj je [tutaj](https://www.smartnydom.pl/forum/firmware-do-przelacznika-sonoff/)


