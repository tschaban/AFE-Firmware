---
title: 'AFE Firmware'
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

{% if config.get('plugins.page-toc.active') or attribute(page.header, 'page-toc').active %}
<div class="page-toc">
    {% set table_of_contents = toc(page.content, 3,1) %}
    {% if table_of_contents is not empty %}
    {{ table_of_contents }}
    {% endif %}
</div>
{% endif %}

---


### AFE Firmware T0: Włącznik WiFi
* Podstawowa wersja z opcją automatycznego wyłączania do 4 przekaźników
* Idealnie pasuje do urządzeń jak Sonoff Basic, 4CH, 4CH Pro, GeekCreit, Shelly-1, itp.
* [Informacje szczegółowe](/postawowe-informacje/wersje-afe-firmware/afe-firmware-t0-wlacznik-wifi) dotyczące AFE Firmware T0
* Paczka do ściągnięcia 
	* zawiera wersje generyczne oraz skonfigurowane pod konkretne urządzenia
	* zawiera dedykowane API dla użytkowników Domoticz (v4 oraz v2020). Jeśli posiadasz inny system do automatyki zainstaluj wersję Standard.


##### [Pobierz najnowszą wersję 2.2.0](https://www.smartnydom.pl/download/afe-firmware-t0-2-2-0/?target=_blank)

**Poprzednie wersje**
* [Wersje 2.1.1](https://www.smartnydom.pl/download/afe-firmware-t0-2-1-1/?target=_blank)
* [Wersje 2.1.0](https://www.smartnydom.pl/download/afe-firmware-t0-2-1/?target=_blank)
* [Wersje 2.0.2](https://www.smartnydom.pl/download/afe-firmware-t0/?target=_blank)
* Wersje 2.0.0 - 2.0.1: [Archiwum](https://drive.google.com/drive/folders/1dKf5vOTjD53RPvnlTQLNaxTWL-N3hKNC/?target=_blank)
* Wersja 1.4.0 [ESP8266](https://www.smartnydom.pl/download/afe-firmware-t0-esp8266-1mb/?target=_blank), [ESP8285](https://www.smartnydom.pl/download/afe-firmware-t0-esp8285-1mb/?target=_blank)

---

### AFE Firmware T1: Włącznik WiFi z czujnikami DS18B20
* Wersja obsługująca do czterech przekaźników oraz do ośmiu czujników D18B20 z termostatem oraz zabezieczeniem termicznym
* Idealnie pasuje do urządzeń jak Sonoff Basic, 4CH, 4CH Pro, GeekCreit, Shelly-1, itp.
* [Informacje szczegółowe](/postawowe-informacje/wersje-afe-firmware/afe-firmware-t1-wlacznik-i-ds18b20) dotyczące AFE Firmware T1
* Paczka do ściągnięcia 
	* zawiera wersje 1Mb oraz 4Mb
	* zawiera dedykowane API dla użytkowników Domoticz (v4 oraz v2020). Jeśli posiadasz inny system do automatyki zainstaluj wersję Standard.


##### [Pobierz najnowszą wersję 2.3.0](https://www.smartnydom.pl/download/afe-firmware-t1-2-3-0/?target=_blank)

**Poprzednie wersje**
* Wersja 1.4.0 [ESP8266](https://www.smartnydom.pl/download/afe-firmware-t1-esp8266/?target=_blank), [ESP8285](https://www.smartnydom.pl/download/afe-firmware-t1-esp8285-1mb/?target=_blank)

---

### AFE Firmware T5: Sterowanie kontrolerem bramy/drzwi/furtki
* Sterowanie max dwoma bramami, furtkami oraz monitorowanie stanu otwarcia bramy z wykorzystaniem czujników magnetycznych
* Idealnie pasuje do urządzeń jak [Sterowniki Furtki iECS](https://www.smartnydom.pl/sterownik-bramy-wersja-v2/)
* [Informacje szczegółowe](/postawowe-informacje/wersje-afe-firmware/t5-brama-drzwi) dotyczące AFE Firmware T5
* Obsługa czujników Bosch BMx80 oraz czujnika natężenia światła BH1750
* Paczka do ściągnięcia 
	* zawiera wersje generyczne oraz skonfigurowane pod konkretne urządzenia
	* pakiet do pobrania zawiera dedykowane API dla użytkowników Domoticz (v4 oraz v2020). Jeśli posiadasz inny system do automatyki zainstaluj wersję Standard.

! AFE Firmware T5 dostępne jest w wersji 1Mb oraz 4Mb. Czujniki Bosch oraz BH1750 dostępne są tylko w wersji AFE 4Mb. AFE 4Mb wymaga ESP8266 z 4Mb rozmiarem pamięci flash 

##### [Pobierz najnowszą wersję 2.2.1](https://www.smartnydom.pl/download/afe-firmware-t5-2-2-1/?target=_blank)

!! Przeczytaj koniecznie: [Aktualizacjia do wersji 4Mb](/postawowe-informacje/wersje-afe-firmware/t5-brama-drzwi/aktualizacja-afe-t5-z-wersji-2-0-x-do-2-2-x) 

**Poprzednie wersje**
* Wersja [2.2.0](https://www.smartnydom.pl/download/afe-firmware-t5-2-2-0/?target=_blank)
* Wersja [2.0.1](https://www.smartnydom.pl/download/afe-firmware-t5-v2/?target=_blank)
* Wersje 1.3.3 - 2.0.0: [Archiwum](https://drive.google.com/drive/folders/10RpEzzgYVWdH9vvdMTF1w1KjRRnwsEv_/?target=_blank)
* Wersja 1.3.2 [ESP8266](https://www.smartnydom.pl/download/afe-firmware-t5-esp8266/?target=_blank), [ESP8285](https://www.smartnydom.pl/download/afe-firmware-t5-esp8285/?target=_blank)

---

### AFE Firmware T6: Stacja pogody
* Odczyt i przesyłanie do systemu automatyki szeregu parametrów atmosferycznych
* Idealnie pasuje do urządzeń jak [Stacja Pogody](https://www.smartnydom.pl/weather-station-stacja-pogodowa-v2/)
* [Informacje szczegółowe](/postawowe-informacje/wersje-afe-firmware/afe-firmware-t6-stacja-pogody) dotyczące AFE Firmware T6
* Paczka do ściągnięcia 
	* zawiera wersje generyczne oraz skonfigurowane pod konkretne urządzenia
	* pakiet do pobrania zawiera dedykowane API dla użytkowników Domoticz (v4 oraz v2020). Jeśli posiadasz inny system do automatyki zainstaluj wersję Standard.


##### [Pobierz najnowszą wersję 2.3.1](https://www.smartnydom.pl/download/afe-firmware-t6-stacja-pogody-2-3-1/?target=_blank)

**Poprzednie wersje**
* Wersja [2.3.0](https://www.smartnydom.pl/download/afe-firmware-t6-stacja-pogody-2-3-0/?target=_blank)
* Wersja [2.2.2](https://www.smartnydom.pl/download/afe-firmware-t6-2-2-2/?target=_blank)
* Wersja [2.2.1](https://www.smartnydom.pl/download/afe-firmware-t6-2-2-1/?target=_blank)
* Wersja [2.2.0](https://www.smartnydom.pl/download/afe-firmware-t6-2-2-0/?target=_blank)
* Wersja [2.1.0](https://www.smartnydom.pl/download/afe-firmware-t6-2-1-0/?target=_blank)
* Wersja [2.0.0](https://www.smartnydom.pl/download/afe-firmware-t6/?target=_blank)
* Wersja [1.0.1](https://www.smartnydom.pl/download/afe-firmware-t6-1-0-1/?target=_blank)

---

! Jeśli znajdziesz jakiś błąd / problem z oprogramowaniem, zgłoś go [tutaj](https://www.smartnydom.pl/forum/zglaszanie-problemow/?target=_blank). Bez tego ciężko będzie zrobić firmware lepszym. Dzięki.

