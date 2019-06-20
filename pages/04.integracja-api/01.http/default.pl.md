---
title: HTTP
---

HTTP API umożliwia sterowanie urządzeniem wysyłając odpowiednio sparametryzowane żądania / adresy url bezpośrednio do urządzenia.

!!!! Urządzenie powinno mieć zawsze ten sam adres IP. Do skonfigurowania w routerze WiFi lub AFE Firmware.

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

