---
title: 'Komendy sterujące HTTP API'
recaptchacontact:
    enabled: false
---

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