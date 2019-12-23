---
title: 'Kanały (Channels)'
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

* Poniżej uzupełniam opis konfiguracji[ AFE Firmware z MQTT Bidning 2.x dla openHAB](/system-do-automatyki/openhab/konfiguracja-openhab-oraz-afe-firmware)
* Opis zawiera konfigurację wszystkich Channels w ramach poszczególnych możliwości AFE Firmware przez Paper UI
* Poniżej są wprowadzone wartości jakie trzeba wprowadzić w konfiguracji Channels dla Things
* Jak konfiguruje się Channel w Thing w ramach Paper UI znajduje się w rozdziale [Dodawanie nowych Things (Rzeczy)](/system-do-automatyki/openhab/konfiguracja-openhab-oraz-afe-firmware#dodawanie-nowego-thing-rzecz-urzadzenie)
* [Lektura](https://www.openhab.org/docs/concepts/) uzupełniająca jeśli: Thing, Channel, Item jest czarną magią

---

{% if config.get('plugins.page-toc.active') or attribute(page.header, 'page-toc').active %}
**Spis treści**
<div class="page-toc">
    {% set table_of_contents = toc(page.content, 3,2) %}
    {% if table_of_contents is not empty %}
    {{ table_of_contents }}
    {% endif %}
</div>
{% endif %}

---

### Urządzenie jest podłączone / rozłączone do/z MQTT Brokera

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji Brokera MQTT](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-mechanizmow-sterowania/mqtt-broker#sekcja-konfigurcja-wiadomosci-lwt)

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | connected |
| **Off/Closed value** | disconnected |

---

### Kanały sterowania przekaźnikiem

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika#sekcja-konfiguracja-tematu-mqtt-sterujacego-przekaznikiem)

#### Włącz / Wyłacz przekaźnik

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | on |
| **Off/Closed value** | off |

#### Przełączanie przekaźnika na stan przeciwny do aktualnego

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | toggle |
| **Off/Closed value** | toggle |

#### Pobierz w jakim stanie jest przekaźnika

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

#### Wartość domyślna przekaźnika

* Komenda wysyłana do MQTT Brokera po podłączeniu się urządzenia do MQTT Brokera.
* Więcej na ten temat znajdziesz [tutaj](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika#po-podlaczeniu-do-brokera-mqtt)

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | |

---

### Kanały dotyczące sterownika bramy/furtki (AFE T5)

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji bramy](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-funkcji-brama)

#### Start/Stop otwieranie bramy

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | toggle |
| **Off/Closed value** | toggle |

#### Stan otwarcia bramy (contact)

| Parameter | Wartość |
|-|-|
| **Channel Type** | Open/Close contact |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | |
| **Incomming value transformation** | MAP:gate-states-contact.map |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Stan otwarcia bramy (switch)

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | |
| **Incomming value transformation** | MAP:gate-states-switch.map |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Pobierz w jakim stanie otwarcia jest brama

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

### Kanały dotyczące monitorowania stanu czujników magnetycznych (AFE T5)

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji czujnika magnetycznego](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-funkcji-brama#konfiguracja-czujnika-magnetycznego)

#### Stan czujnika magnetycznego

| Parameter | Wartość |
|-|-|
| **Channel Type** | Open/Close contact |
| **MQTT State topic** | mqttTopic/state |
| **MQTT Command topic** | |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | open |
| **Off/Closed value** | closed |

#### Pobierz stan czujnika magnetycznego

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch |
| **MQTT State topic** | |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

---

### Kanały dotyczące czujnika cząstek stałych PM2.5PM10 – HPMA115S0

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/honeywell-hpma115s0#sekcja-temat-mqtt-czujnika-hpma115s0)

#### Wartość pyłów zawieszonych PM2.5

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.PM25.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość pyłów zawieszonych PM10

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.PM10.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Pobierz dane z czujnika HPMA115S0

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch  |
| **MQTT State topic** |  |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

---

### Kanały dotyczące czujnika natężenia światła BH1750

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bh1750#sekcja-temat-mqtt-czujnika-bh1750)

#### Wartość natężenia światła

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.illuminance.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Pobierz dane z czujnika BH1750

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch  |
| **MQTT State topic** |  |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

---

### Kanały dotyczące czujnika odległości od burzy AS3935

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/as3935#sekcja-temat-mqtt-czujnika-as3935)

#### Odległość od czoła burzy

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.event.distance |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Typ zdarzenia

| Parameter | Wartość |
|-|-|
| **Channel Type** | Text value  |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.event.type |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** |  |

---

### Kanały dotyczące czujnika warunków atmosferycznych BME680/BME280/BMP180/BMP085

**_mqttTopic_** - MQTT Temat definiowany w [Konfiguracji czujnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-czujnikow/bosch-bmx#sekcja-temat-mqtt-czujnika-bmex80)

#### Pobierz dane z czujnika

| Parameter | Wartość |
|-|-|
| **Channel Type** | On/Off Switch  |
| **MQTT State topic** |  |
| **MQTT Command topic** | mqttTopic/cmd |
| **Incomming value transformation** | |
| **Outgoing value transformation** | |
| **On/Open value** | get |
| **Off/Closed value** | get |

#### Temperatura

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.temperature.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Punkt rosy

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.dewPoint.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wilgotność

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.humidity.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Temperatura odczuwalna

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.heatIndex.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Ciśnienie atmosferyczne

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.pressure.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Względne ciśnienie atmosferyczne

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.relativePressure.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość gazów czujnika

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.gasResistance.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość IAQ

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.iaq.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość Statycznego IAQ

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.staticIaq.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość ekwiwalentu CO2

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.co2Equivalent.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |

#### Wartość ekwiwalentu BVOC

| Parameter | Wartość |
|-|-|
| **Channel Type** | Number value |
| **MQTT State topic** | mqttTopic |
| **MQTT Command topic** | |
| **Incomming value transformation** | JSONPATH:$.breathVocEquivalent.value |
| **Outgoing value transformation** | |
| **On/Open value** | |
| **Off/Closed value** | |