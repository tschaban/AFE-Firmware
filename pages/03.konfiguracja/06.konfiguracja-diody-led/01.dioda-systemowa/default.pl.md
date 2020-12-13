---
title: 'Dioda systemowa'
recaptchacontact:
    enabled: false
---

Systemowa dioda LED w AFE Firmware sygnalizuje:
* Mruganie z częstotliwością ok 1sek: nawiązywanie połączania z siecią WiFi lub brokerem MQTT
* Mruganie 2x na sekundę: urządzenie jest w trybie konfiguracji.
* Dioda LED mruga przy każdym zdarzeniu. Wciśnięcie przycisku, odebranie / wysłanie wiadomość MQTT, HTTP
* Dioda nie świeci, urządzenie jest w trybie normalnego działania – oczekiwania na zdarzenie

!! **Uwaga**:  Dioda LED w konfiguracji AFE Firmware musi być wcześniej skonfigurowana. Czytaj [tutaj](/konfiguracja/konfiguracja-diody-led).

Jeśli w module skonfigurowany jes przycisk systemowy to:
* dioda LED mignie 1x po wciśnięciu i trzymaniu przycisku przez 5 sekund - puszczenie przycisku spowoduje wejście modułu w tryb konfiguracji
* dioda LED mignie 2x po wciśnięciu i trzymaniu przycisku przez 10 sekund - puszczenie przycisku spowoduje wejście modułu w tryb konfiguracji w trybie HotSpot
* dioda LED mignie 3x po wciśnięciu i trzymaniu przycisku przez 30 sekund - puszczenie przycisku uruchomi [przywrócenie ustawień domyślnych](/funkcje/przywracanie-ustawien-poczatkowych)
* dioda LED będzie migać bardzo szybko po wciśnięciu i trzymaniu przycisku przez 35 sekund - puszczenie przycisku uniemożliwi wejście w jedną z powyższych opcji

> AFE Firmware umożliwia odwrócenie sygnalizacji LED tzn. że dioda LED świeci kiedy urządzenie jest w trybie normalnego działania i gaśnie kiedy wystąpi jakieś zdarznie.

https://www.youtube.com/watch?v=byy56ObY7-4

