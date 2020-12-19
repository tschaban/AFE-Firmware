---
title: 'Standardowe MQTT'
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

Urządzeniem można sterować wiadomościami MQTT. Jest to zalecana przeze mnie metoda sterowaniem urządzeniem z AFE Firmware.

W przypadku korzystania z system do automatyki Domoticz, przejdź do instrukcji konfiguracji [Domoticz API](/integracja-api/domoticz-api)

MQTT to bardzo prosty i lekki protokół do wymiany informacji oparty o mechanizm publikowania wiadomości i subskrybowania się do tematów. Został specjalnie opracowany do wymiany danych w sieciach, gdzie mogą występować niskie przepustowości, długie czasy oczekiwań, czy zwyczajnie problem z jej działaniem. Protokół mimo potencjalnych problemów z siecią zapewnia niezawodność. Idealnie nadaje się do wymiany danych między urządzeniami, IoT, ma szerokie zastosowanie w automatyce.

Wymiana danych między urządzeniami odbywa się za pomocą MQTT Brokera. Broker jest pośrednikiem wymiany informacji między urządzeniami, serwisami, oprogramowaniem.

Do prawidłowego działania MQTT API konieczne jest [skonfigurowanie połaczenie do MQTT Brokera](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-mechanizmow-sterowania/mqtt-broker).

> Przeczytaj również o [dobrych praktykach dotyczących tematów MQTT](/integracja-api/mqtt/tematy-mqtt-najlepsze-praktyki)

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

---

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

##### Tematy do otwierania/zamykania oraz monitorowania stanu otwarcia/zamknięcia bramy/drzwi (AFE T5)

* Teamt definiowany jest osobno dla każdej bramy w formularzu do [konfiguracji bramy](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-wejscie-analogowe-2#sekcja-konfiguracja-bramy-drzwi)

Przykład:
`dom/brama/wjazdowa`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **state** lub **cmd**

Używając przykładu tematu powyżej, **aby otworzyć/zamknąć bramę**, do MQTT Brokera należy wysłać następujący temat

`dom/brama/wiazdowa/cmd`

z wiadomością:

* toggle

Używając przykładu tematu powyżej,** aby odczytać aktualny stan otwarcia/zamknięcia bramy**, do MQTT Brokera należy wysłać następujący temat

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

---

##### Tematy do odczytu danych z czujników [Bosch BME680,BME280,BMP180,BMP085](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx)

* Temat definiowany jest osobno da każdego czujnika w formularzu do [konfiguracji czujnika Bosch](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx#sekcja-temat-mqtt-czujnika-bmex80)

Przykład:
`dom/salon/czujnik`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **cmd** dla komend sterujących

Używając przykładu tematu powyżej, aby odczytać dane z czujnika, do MQTT Brokera należy wysłać następujący temat

`dom/salon/czujnik/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat z z wiadomością w formacie JSON z danymi odczytanymi przez czujnik

`dom/salon/czujnik`

Przykładowy JSON dla czujnika BME680

`
{"temperature":{"value":0,"unit":"C","correction":0},"pressure":{"value":0,"unit":"hPa","correction":0},"relativePressure":{"value":0,"unit":"hPa"},"dewPoint":{"value":0,"unit":"C"},"humidity":{"value":0,"unit":"%","correction":0,"rating":0},"heatIndex":{"value":0,"unit":"C"},"iaq":{"value":0,"rating":0,"accuracy":0},"staticIaq":{"value":0,"rating":0,"accuracy":0},"co2Equivalent":{"value":0,"unit":"ppm","rating":0,"accuracy":0},"breathVocEquivalent":{"value":0,"unit":"?","accuracy":0},"gasResistance":{"value":0,"unit":"kOm"}}
`

!!!! W zależności, który czujnik BME680, BME280, BMP180, BMP085 mamy podłączony to zwracany JSON będzie się różnił co do ilości zwrcanych danych.

> AFE Firmware wysyła automatycznie dane z czujnika, z wykorzystaniem MQTT, ze zdefiniowaym konfiguracji firmware [interwałem](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx#sekcja-czujnik-bmex80) 

---

##### Tematy do odczytu danych z czujnika [BH1750](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750)

* Temat definiowany jest osobno da każdego czujnika w formularzu do [konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750#sekcja-temat-mqtt-czujnika-bh1750)

Przykład:
`podworko/czujnik/PoziomSwiatla`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **cmd** dla komend sterujących

Używając przykładu tematu powyżej, aby odczytać dane z czujnika, do MQTT Brokera należy wysłać następujący temat

`podworko/czujnik/PoziomSwiatlak/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat z z wiadomością w formacie JSON z danymi odczytanymi przez czujnik

`podworko/czujnik/PoziomSwiatla`

Przykładowy JSON dla czujnika BH1750

`
{"illuminance":{"value":200.25,"unit":"lux"}}
`

> AFE Firmware wysyła automatycznie dane z czujnika, z wykorzystaniem MQTT, ze zdefiniowaym konfiguracji firmware [interwałem](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750#sekcja-czujnik-bh1750) 

---

##### Tematy do odczytu danych z czujnika [Honeywell HPMA115S0](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0)

* Temat definiowany jest osobno da każdego czujnika w formularzu do [konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0#sekcja-temat-mqtt-czujnika-hpma115s0)

Przykład:
`dom/czujnik/pylow`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **cmd** dla komend sterujących

Używając przykładu tematu powyżej, aby odczytać dane z czujnika, do MQTT Brokera należy wysłać następujący temat

`dom/czujnik/pylow/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat z z wiadomością w formacie JSON z danymi odczytanymi przez czujnik

`dom/czujnik/pylow`

Przykładowy JSON dla czujnika Honeywell HPMA115S0

`
{"PM25":{"value":10,"unit":"µg/m3"},"PM10":{"value":12,"unit":"µg/m3"}}
`

> AFE Firmware wysyła automatycznie dane z czujnika, z wykorzystaniem MQTT, ze zdefiniowaym konfiguracji firmware [interwałem](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0#sekcja-czujnik-czastek-pm2-5-pm10)

---

##### Temat informujący o odległości do burzy z wykorzystaniem czujnika [AS3935](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/as3935)

* Temat definiowany jest osobno da każdego czujnika w formularzu do [konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/as3935#sekcja-temat-mqtt-czujnika-as3935)

Przykład:
`burza/odleglosc`

Dla przykładu powyżej AFE Firmware wysła następujący temat z wiadomością w formacie JSON po wykryciu piorunów
`burza/odleglosc`

Przykładowe JSON dla czujnika AS3935

* Wykrycie pioruna: `{"event":{"type":"lightning strike","distance":10,"unit":"km"}}`
* Wykrycie szumu: `{"event":{"type":"noise"}`
* Wykrycie zakłócenia: `{"event":{"type":"disruption"}`


> AFE Firmware wysyła automatycznie dane z czujnika, z wykorzystaniem MQTT, po wykryciu zdarzenia.


##### Tematy do odczytu danych z czujnika [DHT](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/dht)

* Temat definiowany jest osobno da każdego czujnika w formularzu do [konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/dht#sekcja-temat-mqtt-czujnikadht)

Przykład:
`podworko/czujnik/dht`

! Temat nie może kończyć się znakiem / ponieważ do tematu dodawany jest sufix **cmd** dla komend sterujących

Używając przykładu tematu powyżej, aby odczytać dane z czujnika, do MQTT Brokera należy wysłać następujący temat

`podworko/czujnik/dht/cmd`

z wiadomością:

* get

AFE Firmware wysła następujący temat z z wiadomością w formacie JSON z danymi odczytanymi przez czujnik

`podworko/czujnik/dht`

Przykładowy JSON dla czujnika DHT

`
{temperature: {value: 24,unit: "C"},humidity: {value: 39.9,unit: "%"},absoluteHumidity: {value: 8.68,unit: "%"},heatIndex: {value: 23.5,unit: "C"},dewPoint: {value: 9.55,unit: "C"},perception: {value: 0,description: "Dla niektórych trochę "},comfort: {value: 0,ratio: 100,unit: "%",description: "OK"}
`
