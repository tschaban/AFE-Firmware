---
title: 'Domoticz API'
media_order: 'domoticz-dodawanie-urzadzenia.png,domoticz-dodawanie-wirtualnego-czujnika.png,domoticz-dodawanie-wirtualnego-czujnika-formularz.png,domoticz-dodawanie-wirtualnego-przycisku-formularz.png,domoticz-przelaczniki.png,domoticz-urzadzenia.png,domoticz-edytowane-przycisku.png,domoticz-ustawianie-akcji-przekaznika.png,domoticz-zmiana-typu-przelacznika.png,afe-firmware-konfiguracja-przekaznik-domoticz.png'
recaptchacontact:
    enabled: false
---

* Domoticz API umożliwia dwukierunkową integrację między urządzeniem z AFE Firmware, a Domoticz
* Integracja opiera się o protokół HTTP
* Do poprawnego działania Domoticz API, należy go włączyć w [konfiguracji urządzenia](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-urzadzenia) oraz [skonfigurować połączenie do serwera](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-mechanizmow-sterowania/serwer-domoticz) z zainstalowanym Domoticz.

##### Procedura konfiguracji wygląda następująco:
* w Domoticz dodajemy urządzenie typu Dummy, do którego dodajemy wirtualne elementy, jak włączniki, czy czujniki np.: temperatury
* każdy z takich wirtualnych elementów otrzymuje unikalny identyfikator IDX
* następnie w AFE Firmware 
	* konfigurujemy połączenie do serwera Domoticz oraz 
	* wprowadzamy identyfikatory IDX w odpowiednich ekranach konfiguracyjnych

#### Konfiguracja przekaźnika oraz przycisku/włącznika

* Wchodzimy do panelu sterującego Domoticz, a następnie do **Konfiguracja -> Sprzęt**. 
* Dodajemy wirtualne urządzenie. Jak na poniższym ekranie

![](domoticz-dodawanie-urzadzenia.png)

* Po wciśnięciu przycisku **Dodaj** pokaże nam się urządzenie na liście.
* Wciskamy przycisk: **Utwórz wirtualny czujnik**

![](domoticz-dodawanie-wirtualnego-czujnika.png)

* Konfigurujemy wirtualny czujnik
* W przykładzien, nazywam go: **Lampa w kuchni**, bo będzie włączał / wyłączał lampę
* Wirtualny czujnik będzie miał typ: **przełącznik**

![](domoticz-dodawanie-wirtualnego-czujnika-formularz.png)

> W tym kroku tworzymy wirtualny przełączik do monitorowania wciśnięcia przycisku np. w ścianie do włączania / wyłączania lampy. W większości przypadków nie mam potrzeby monitorowania stanu przycisku, więc ten krok konfiguracji jest opcjonalny.

* Dodajemy kolejny wirtualny czujnik typu: **przełącznik**
* Nazywam go: **Przycisk lampy w kuchni**

![](domoticz-dodawanie-wirtualnego-przycisku-formularz.png)

* Oba wirtualne elementy widoczne są teraz na liście urządzeń
* Dla każdego z nich nadany został unikalny numer **IDX** - zapamiętujemy je

![](domoticz-urzadzenia.png)

* Przechodzimy teraz na zakładkę **Przełączniki**, gdzie powinny znajdować się dodane elementy
* W kolejnym kroku zmienimy typ przełącznika, który został dodany do monitorowania stanu włącznika (np. w ścianie), na taki który lepiej obrazuje i zachowuje się w takim zastosowaniu
* Wciskamy przycisk **Edytuj**

![](domoticz-przelaczniki.png)

* Ustawiamy typ przełącznik na: **Contact**
* Zmieniamy ikonę na np. taką, jak na poniższym przykładzie
* Zapisujemy zmiany

![](domoticz-zmiana-typu-przelacznika.png)

* W kolejnych krokach wprowadzimy komendy sterujące przekaźnikem
* Wciskamy przycisk **Edytuj** w elemencie sterującym naszą Lampą

![](domoticz-edytowane-przycisku.png)

* Wprowadzamy w polach: **Włącza akcję** i **Wyłącz akcje** następujące komendy w formacie

**włącz**: `http://ADRES-IP-URZADZENIA/?device=relay&source=domoticz&name=NAZWA-PRZEKAZNIKA&command=on`

**wyłącz**: `http://ADRES-IP-URZADZENIA/?device=relay&source=domoticz&name=NAZWA-PRZEKAZNIKA&command=off`

**Parametery**: 
* _ADRES-IP-URZADZENIA_ - to adres IP urządzenia z zainstlowanym AFE Firmware
* _NAZWA-PRZEKAZNIKA_, definiowany jest w AFE Firmware w [konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika)

! Z doświadczenia wiem, że wielu użytkowników AFE Firmware zapomina dodawać ten fragment **&source=domoticz** do komendy sterującej. Jeśli ten fragment nie zostanie dodany do komendy sterującej, może to spowodować, że Domoticz + Urządzenie z AFE Firmware wpadnie w pętlę włączania / wyłączania się

! Kolejną częstą pomyłką jest wpisanie w adres URL nazwę urządzenia zamiast nazwę przekaźnika. Konieczne jest wpisanie **nazwę przekaźnika**, którą wprowadza się w [konfiguracji przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika)

![](domoticz-ustawianie-akcji-przekaznika.png)

* W tym momencie zakończona została konfiguracja w Domoticz
* W kolejnych krokach wpisane zostaną identyfikatory IDX w konfiguracji AFE Firmware
* Otwieramy Panel Konfiguracyjny AFE Firmware
* Otwieramy f[ormularz konfiguracyjny przekaźnika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przekaznika), a następnie w polu IDX wprowadzamy identyfikator zapamiętany w poprzednich krokach

![](afe-firmware-konfiguracja-przekaznik-domoticz.png)

* Podobną czynność przeprowadzamy dla przycisku/włącznika, jeśli takowy dodaliśmy
* Otwieramy [formularz konfiguracji przycisku/włącznika](/konfiguracja/konfiguracja-urzadzenia/konfiguracja-przycisku-wlacznika), a następnie w polu IDX wprowadzamy identyfikator zapamiętany w poprzednich krokach

---

W tym momencie jeśli wszystko dobrze poszło, Domoticz i urządzenie z AFE Firmware powinny ze sobą współpracować.

! Chcęc zwrócić uwagę, że Domoticz odświeża dane w przeglądarce w interwałach ok 8-9 sekundowych. Jeśli przekaźnik w urządzeniu z AFE Firmware zostanie przełączony ręcznie, to status w Domoticz, będzie odświeżony z maksymalnie 8-9 sekundowym opóźnieniem.

