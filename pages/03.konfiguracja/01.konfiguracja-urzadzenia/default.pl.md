---
title: 'Konfiguracja urządzenia'
media_order: 'afe-firmware-konfiguracja-urzadzenia-api.png,afe-firmware-konfiguracja-urzadzenia-nazwa.png,afe-firmware-konfiguracja-urzadzenia-podlaczone-elementy.png'
---

Konfiguracja nazwy urządzenia, sprzętu oraz mechanizmów sterowania urządzeniem

![](afe-firmware-konfiguracja-urzadzenia-nazwa.png)

Nazwa urządzenia
Powinna to być unikalną nazwą urządzenia w ramach twojej lokalnej sieci (LAN).
Podczas konfiguracji urządzenia w trybie Access Point wpisana wartość będzie nazwa sieci WiFi urządzenia
Maksymalnie: 16 znaków
Wymagane
Uwaga: jeśli używane jest MQTT do sterowania urządzeniem konieczne jest, aby każde urządzenie łączące się do MQTT Brokera miało unikalną nazwę. Nie jest to konieczne dla sterowania przez HTTP API


![](afe-firmware-konfiguracja-urzadzenia-podlaczone-elementy.png)

Sprzęt
Tutaj wybierasz jakie elementy typu przekaźniki, czujniki, diody LED itp są podłączone do urządzania. Jakie elementy są do wyboru uzależnione jest od wersji AFE Firmware. Włączenie danego elementu powoduj uaktywnienie sekcji konfiguracji w menu panelu konfiguracji dla danego elementu urządzenia.

![](afe-firmware-konfiguracja-urzadzenia-api.png)

Włączanie / wyłączanie mechanizmów sterowania urządzeniem (API). Włączanie danego API, jeśli tego wymaga, uaktywnia sekcję konfiguracyjną w menu Panelu Konfiguracyjnym. W zależności od wersji oprogramowania dostępne są do wybrania odpowiednie API np. MQTT, HTTP