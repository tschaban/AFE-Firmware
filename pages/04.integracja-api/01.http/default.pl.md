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
	* Nazwa czujnika magnetycznego wprowadzoba w [konfiguracji czujników magnetycznych](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#konfiguracja-czujnika-magnetycznego)
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