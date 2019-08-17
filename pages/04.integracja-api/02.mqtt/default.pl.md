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
    {% set table_of_contents = toc(page.content, 5,2) %}
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

Informacja na wyjściu, jedna z poniższych

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

z jedną z wiadomości

* on
* off

---

##### Tematy do monitorowania stanu przycisku

* Temat definiowany jest osobno da każdego przycisku w [konfiguracji przycisku/włącznika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przycisku-wlacznika)

Przykład:
`dom/kuchnia/przycisk/lampa`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **state** lub **cmd**

Używając przykładu tematu powyżej, aby odczytać aktualny stan przycisku, do MQTT Brokera należy wysłać następujący temat

`dom/kuchnia/przycisk/lampa/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat przy każdej zmianie stanu przycisku lub po otrzymaniu żądania odczytania stanu przycisku

`dom/kuchnia/przycisk/lampa/state`

z jedną z wiadomości

* open
* closed

---

##### Tematy do monitorowania stanu otwarcia/zamknięcia bramy/drzwi (AFE T5)

* Teamt definiowany jest osobno dla każdej bramy w formularzu do [konfiguracji bramy](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#sekcja-konfiguracja-bramy-drzwi)

Przykład:
`dom/brama/wjazdowa`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **state** lub **cmd**

Używając przykładu tematu powyżej, aby odczytać aktualny stan otwarcia/zamknięcia bramy, do MQTT Brokera należy wysłać następujący temat

`dom/brama/wiazdowa/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat przy każdej zmianie stanu otwarcia/zamknięcia bramy lub po otrzymaniu żądania odczytania stanu bramy

`dom/brama/wjazdowa/state`

z jedną z wiadomości

* open
* closed
* partiallyOpen
* unknown

---

##### Tematy do monitorowania czujnika magnetycznego (AFE T5)

* Temat definiowany jest osobno da każdego czujnika magnetycznego w formularzu do [konfiguracji czujnika magnetycznego](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#konfiguracja-czujnika-magnetycznego)

Przykład:
`dom/brama/wiazdowa/czujnik/C1`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **state** lub **cmd**

Używając przykładu tematu powyżej, aby odczytać aktualny stan czujnika magnetycznego, do MQTT Brokera należy wysłać następujący temat

`dom/brama/wiazdowa/czujnik/C1/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat przy każdej zmianie stanu czujnika magnetycznego lub po otrzymaniu żądania odczytania stanu czujnika

`dom/brama/wiazdowa/czujnik/C1/state`

z jedną z wiadomości

* open
* closed

