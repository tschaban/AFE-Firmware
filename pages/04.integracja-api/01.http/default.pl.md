---
title: HTTP
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

HTTP API umożliwia sterowanie urządzeniem wysyłając odpowiednio sparametryzowane żądania / adresy url bezpośrednio do urządzenia.

!!!! Urządzenie powinno mieć zawsze ten sam adres IP. Do skonfigurowania w routerze WiFi lub AFE Firmware.

{% if config.get('plugins.page-toc.active') or attribute(page.header, 'page-toc').active %}
<div class="page-toc">
    {% set table_of_contents = toc(page.content, 5,1) %}
    {% if table_of_contents is not empty %}
    {{ table_of_contents }}
    {% endif %}
</div>
{% endif %}

---

##### Sterowanie przekaźnikiem

**Format komendy sterującej**

`http://adres-ip-urzadzenia/?device=relay&name=NAZWA_PRZEKAZNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_PRZEKAZNIKA
	* Nazwa przekaźnika wprowadzona w [konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika)
![](/user/pages/03.konfiguracja/04.konfiguracja-przekaznika/afe-firmware-konfiguracja-przekaznik-gpio.png)
* KOMENDA
	* on - włączenie przekaźnika
	* off - wyłączenie przekaźnika
	* toggle - przełączenie przekaźnika w stan przeciwny
	* get - uzyskanie informacji w jakim stanie jest przekaźni


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"relay", "name":"NAZWA_PRZEKAZNIKA","command":"KOMENDA","value":"STAN_PRZEKAZNIKA","status":"STATUS_WYKONANIA"}`

gdzie 
STAN_PRZEKAZNIKA
* on
* off

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=relay&name=lampa&command=on
* http://192.168.1.2/?device=relay&name=lampa&command=off
* http://192.168.1.2/?device=relay&name=lampa&command=toggle
* http://192.168.1.2/?device=relay&name=lampa&command=get

---

##### Sterowanie bramą/dzwiami (AFE T5)

**Format komendy sterującej**

`http://adres-ip-urzadzenia/?device=gate&name=NAZWA_BRAMY&command=KOMENDA`

**Parametery:**
* NAZWA_BRAMY
	* Nazwa bramy wprowadzona w [konfiguracji bramy](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#konfiguracja-bramy)
* KOMENDA
	* toggle - włącza przekaźnik na czas określony w [konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#konfiguracja-przekaznika)
	* get - uzyskanie informacji w jakim stanie otwarcia/zamknięcia jest brama/drzwi


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"gate", "name":"NAZWA_BRAMY","command":"KOMENDA","value":"STAN_BRAMY","status":"STATUS_WYKONANIA"}`

gdzie 
STAN_BRAMY
* open
* closed
* partiallyOpen
* unknown

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=gate&name=BramaWjazdowa&command=toggle
* http://192.168.1.2/?device=gate&name=BramaWjazdowa&command=get

---

##### Odczyt stanów czujników magnetycznych (AFE T5)

**Format komendy**

`http://adres-ip-urzadzenia/?device=contactron&name=NAZWA_CZUJNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_CZUJNIKA
	* Nazwa czujnika magnetycznego wprowadzona w [konfiguracji czujników magnetycznych](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#konfiguracja-czujnika-magnetycznego)
* KOMENDA
	* get - uzyskanie informacji w jakim stanie jest czujnik magnetyczny


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"contactron", "name":"NAZWA_CZUJNIKA","command":"KOMENDA","value":"STAN_CZUJNIKA","status":"STATUS_WYKONANIA"}`

gdzie 
STAN_CZUJNIKA
* open
* closed

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=contactron&name=czujnik&command=get

---

##### Odczyt danych z czujnika [BME680/BME280/BMP180/BMP085](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bm)

**Format komendy**

`http://adres-ip-urzadzenia/?device=BMEX80&name=NAZWA_CZUJNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_CZUJNIKA
	* Nazwa czujnika wprowadzona w [konfiguracji czujnika Bosch BMx80](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx#sekcja-czujnik-bmex80)
* KOMENDA
	* get - uzyskanie informacji z czujnika


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"BMEX80", "name":"NAZWA_CZUJNIKA","command":"KOMENDA","data":{JSON_STRING},"status":"STATUS_WYKONANIA"}`

gdzie JSON_STRING
* zawiera dane odczytane z czujnika w formacie JSON

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=BMEX80&name=WarunkiPowietrzne&command=get

---

##### Odczyt danych z czujnika [BH1750](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750)

**Format komendy**

`http://adres-ip-urzadzenia/?device=BH1750&name=NAZWA_CZUJNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_CZUJNIKA
	* Nazwa czujnika wprowadzona w [konfiguracji czujnika BH1750](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750#sekcja-czujnik-bh1750)
* KOMENDA
	* get - uzyskanie informacji z czujnika


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"BH1750", "name":"NAZWA_CZUJNIKA","command":"KOMENDA","data":{JSON_STRING},"status":"STATUS_WYKONANIA"}`

gdzie JSON_STRING
* zawiera dane odczytane z czujnika w formacie JSON

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=BH1750&name=NatezenieSwiatla&command=get

---

##### Odczyt danych z czujnika [Honeywell HPMA115S0](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0)

**Format komendy**

`http://adres-ip-urzadzenia/?device=HPMA115S0&name=NAZWA_CZUJNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_CZUJNIKA
	* Nazwa czujnika wprowadzona w [konfiguracji czujnika HPMA115S0](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0#sekcja-czujnik-czastek-pm2-5-pm10)
* KOMENDA
	* get - uzyskanie informacji z czujnika


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"HPMA115S0", "name":"NAZWA_CZUJNIKA","command":"KOMENDA","data":{JSON_STRING},"status":"STATUS_WYKONANIA"}`

gdzie JSON_STRING
* zawiera dane odczytane z czujnika w formacie JSON

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=HPMA115S0&name=JakoscPowietrza&command=get

---

##### Odczyt danych z czujnika [AS3935](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/as3935)

**Format komendy**

`http://adres-ip-urzadzenia/?device=AS3935&name=NAZWA_CZUJNIKA&command=KOMENDA`

**Parametery:**
* NAZWA_CZUJNIKA
	* Nazwa czujnika wprowadzona w [konfiguracji czujnika AS3935](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/as3935#sekcja-czujnik-as3935)
* KOMENDA
	* get - uzyskanie informacji z czujnika


**Wywołanie komendy zwraca odpowiedź w postaci JSON:**

`{"device":"AS3935", "name":"NAZWA_CZUJNIKA","command":"KOMENDA","data":{JSON_STRING},"status":"STATUS_WYKONANIA"}`

gdzie JSON_STRING
* zawiera dane odczytane z czujnika w formacie JSON

STATUS_WYKONANIA
* success
* error

**Przykłady:**

* http://192.168.1.2/?device=AS3935&name=Burza&command=get