---
title: MQTT
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

Urządzeniem można sterować wiadomościami MQTT. Jest to zalecana przeze mnie metoda sterowaniem urządzeniem z AFE Firmware.

MQTT to bardzo prosty i lekki protokół do wymiany informacji oparty o mechanizm publikowania wiadomości i subskrybowania się do tematów. Został specjalnie opracowany do wymiany danych w sieciach, gdzie mogą występować niskie przepustowości, długie czasy oczekiwań, czy zwyczajnie problem z jej działaniem. Protokół mimo potencjalnych problemów z siecią zapewnia niezawodność. Idealnie nadaje się do wymiany danych między urządzeniami, IoT, ma szerokie zastosowanie w automatyce.

Wymiana danych między urządzeniami odbywa się za pomocą MQTT Brokera. Broker jest pośrednikiem wymiany informacji między urządzeniami, serwisami, oprogramowaniem.

Do prawidłowego działania MQTT API konieczne jest [skonfigurowanie połaczenie do MQTT Brokera](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-mechanizmow-sterowania/mqtt-broker).

---

#### Tematy oraz wiadomości MQTT

{% if config.get('plugins.page-toc.active') or attribute(page.header, 'page-toc').active %}
<div class="page-toc">
    {% set table_of_contents = toc(page.content, 5,1) %}
    {% if table_of_contents is not empty %}
    {{ table_of_contents }}
    {% endif %}
</div>
{% endif %}

##### Temat LWT (Last Will and Testament)

LWT temat umożlwia informowanie, że urządzenie jest lub nie jest podłączone do MQTT Brokera

* Temat definiowany jest w[ konfuguracji połaczenia do MQTT Brokera.](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-mechanizmow-sterowania/mqtt-broker)

Przykład definicji tematu LWT

`dom/kuchnia/lampa/lwt`

Informacja na wyjściu

* connected
* disconnected

---

##### Tematy do sterowania przekaźnikiem

* Temat definiowany jest osobno dla każdego przekaźnika w [konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika)

Przykład:
`dom/kuchnia/lampa`

! Temat nie może kończyć się znakiem / ponieważ do tematu sterującego przekaźnikem dodawany jest sufix **state** lub **cmd**

Używając przykładu tematu powyżej, aby sterować przekaźnikem, do MQTT Brokera należy wysłać następujący temat

`dom/kuchnia/lampa/cmd`

z jedną z wiadomości:
* on - włączenie przekaźnika
* off - wyłączenie przekaźnika
* toggle - przełączenia przekaźnika w stan odwrotny do aktualnego
* get - żądanie wysłania stanu przekaźnika do MQTT Brokera

AFE Firmware wysła następujący temat przy każdej zmianie stanu przekaźnika lub po otrzymaniu żądania stanu przekaźnika

`dom/kuchnia/lampa/state`

z wiadomością

* on
* off

---

##### Tematy do monitorowania stanu przycisku

* Teamt definiowany jest osobno da każdego przycisku w[ konfiguracji przycisku/włącznika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przycisku-wlacznika)

Przykład:
`dom/kuchnia/przycisk/lampa`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **state**

Używając przykładu tematu powyżej, przy każdej zmianie stany przycisku, do MQTT Brokera urządzenie wysyła MQTT temat 

`dom/kuchnia/przycisk/lampa/state`

z wiadomością

* open
* closed
