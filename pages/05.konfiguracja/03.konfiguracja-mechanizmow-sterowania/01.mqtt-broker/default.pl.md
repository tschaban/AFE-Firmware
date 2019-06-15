---
title: 'MQTT Broker'
media_order: 'afe-firmware-konfiguracja-mqtt-broker.png,afe-firmware-konfiguracja-mqtt-broker-formularz.png,afe-firmware-konfiguracja-mqtt-broker-lwt.png'
---

MQTT Broker to oprogramowanie, które w dużym skrócie zajmuje się odbieraniem widomości od klientów publikujących wiadomości, a następnie rozsyłanie ich do klinetów, którzy te wiadomości subskrybują. 

MQTT to jeden z [mechanizmów sterowania urządzeniem](/integracja-api) w ramach AFE Firmware.

Formularz ten służy do
* konfiguracji połączenia do Brokera MQTT oraz 
* ustawienia tematu LWT


Aby skonfigurować połaczenie do Brokera MQTT, należy wybrać **MQTT Broker** w menu konfiguracyjnym AFE Firmware

![](afe-firmware-konfiguracja-mqtt-broker.png)

### Sekcja: konfiguracja połaczenia do brokera MQTT

![](afe-firmware-konfiguracja-mqtt-broker-formularz.png)

#### Hostname
* Nazwa hosta / domena serwera, na którym pracuje broker MQTT
* Maksymalnie: 32 znaki
* Informacja jest wymagana jeśli nie jest wprowadzony adres IP serwera (poniższy parametr)

!!!! Alternatywnie można wprowadzić adres IP serwera, na którym pracuje broker MQTT (parametr poniżej)

#### Adres IP
* Adres IP serwera, na którym pracuje broker MQTT
* Informacja jest wymagana jeśli nie jest wprowadzona nazwa hosta / domeny (powyższy parametr)

!!!! Alternatywnie można wprowadzić nazwę hosta / domenę serwera, na którym pracuje broker MQTT (parametr powyżej)

#### Port
* Port, na którym nasłuchuje broker MQTT
* Zakres od: 1 do 65535
* Wartość domyślna to: 1883
* Informacja jest wymagana

#### Użytkownik
* Nazwa użytkownika do brokera MQTT
* Maksymalnie 32 znaki
* Informacja jest wymagana jeśli broker MQTT wymaga autentykacji

#### Hasło
* Hasło użytkownika do brokera MQTT
* Maksymalnie 32 znaki
* Informacja jest wymagana jeśli broker MQTT wymaga autentykacji

### Sekcja: Konfigurcja wiadomości LWT

Wiadomość LWT (Last Will and Testament), wykorzystywana jest, aby powiadomić innych klientów o niespodziewanym odłączeniu urządzenia od brokera MQTT. 
W AFE Firmware z tym tematem wysyłana jest wiadomośc do brokera MQTT o podłączeniu lub rozłączeniu się urządzenia od brokera MQTT

> Funkcjonalność można wykorzystać między innymi do: monitorowania podłączenia urządzenia do brokera WiFi,  utraty połączenia z siecią WiFi, niespodziewanych brakach zasilania, czy "zawieszeniu" się urządzenia

![](afe-firmware-konfiguracja-mqtt-broker-lwt.png)

#### Temat 

* temat MQTT
* Informacja nie jest wymagana. W przypadku nie wprowadzenia tematu wiadomości LWT, nie będzie do brokera MQTT wysyłana informacja o połączeniu / rozłączeniu się urządzenia od brokera MQTT
* Więcej o tematach MQTT moższ znaleśc w rodziale: [Integracja API MQTT](/integracja-api/mqtt)