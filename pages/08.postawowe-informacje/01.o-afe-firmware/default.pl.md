---
title: 'O AFE Firmware'
media_order: '18-10-05 18-59-28 7188-1.jpg,donate-logo.jpg'
---

![](donate-logo.jpg)

AFE Firmware to oprogramowanie dla urządzeń zbudowanych na chip’ie ESP8266 lub ESP8285 jak na przykład [Sonoff](https://www.smartnydom.pl/przelacznik-sonoff/?target=_blank), [Shelly](https://www.smartnydom.pl/wlacznik-wifi-shelly-1/?target=_blank), [GeekCreit](https://www.smartnydom.pl/geekcreit-4ch-afe-firmware/?target=_blank), ... (długa lista), czy we [własnych konstrukcjach](https://www.smartnydom.pl/bezprzewodowy-czujnik-temperatury-oraz-wilgotnosci?target=_blank). Wykorzystuję je w swoich instalacjach do różnych zastosowań.

Dlaczego nie używam oryginalnego oprogramowania, które dostarczają producenci wraz z urządzeniami? Otóż w niektórych przypadkach urządzenie  + oryginalne oprogramowanie oznacza ograniczone możliwości lub konieczność używania jednej gamy produktów. Ktoś kto używał przykładowo oryginalnego oprogramowania dostarczonego z urządzeniem Sonoff wie, że z oryginalnej aplikacji można korzystać tylko z produktami Sonoff. Otrzymuje się  funkcjonalności tylko te, które dostarcza producent. Nie podłączy się do niej TV, „inteligentnej” żarówki innej firmy, kamery IP itp. Gdy nazbiera się w domu trochę tych sprzętów, okazuje się, że konieczne jest posiadanie X aplikacji do sterowania różnymi urządzeniami, a w dodatku urządzenia różnych producentów nie „gadają” ze sobą. Nie da się nimi wszystkimi sterować jako jedną całością.
![](18-10-05%2018-59-28%207188-1.jpg)
Dlatego głównie korzystam z rozwiązań, które są na tyle otwarte, aby można było nimi sterować z jednego systemu, jednej aplikacji, aby każde urządzenie niezależnie od producenta mogło „porozumiewać” się z innymi urządzeniami. Oznacza to tyle, że oprogramowanie w tych urządzenia powinno udostępniać standardowy i łatwy w dostępie mechanizm sterowania nimi. To daje możliwość podłączania tych urządzeń do bardziej lub mniej zaawansowanych systemów automatyki, inteligentnych domów, biur, jak kto woli, a nie bycie skazanym na X aplikacji.

AFE Firmware jest takim oprogramowaniem. Urządzenie z zainstalowanym AFE Firmware może być sterowany przez systemy do automatyki np. openHAB, Domoticz, Domotiga, HomeAssistant i pewnie przez wiele innych systemów do automatyki. Napisane jest z myślą o układzie WiFi serii ESP, który coraz częściej spotykany jest w komercyjnych produktach. Dodatkowo jest bardzo tani i łatwy w konstruowaniu niemalże, jak z klocków własnych urządzeń.

##### Podczas projektowania i rozwijania tego oprogramowania korzystałem z następujących zasad

* urządzenie powinno być kontrolowane przez dobrze znane protokoły
> wykorzystuje protokół HTTP oraz MQTT. Dodatkowo zaimplementowałem dedykowany mechanizm integracji (API) z Domoticz
* mechanizmy integracji powinny być jak najbardziej otwarte
> po to, aby można było je zintegrować z jak największą liczbą systemów automatyki domowej
* dedykowane pod konkretne potrzeby, a nie "kombajn do wszystkiego" 
> postanowiłem nie tworzyć jednego oprogramowania obsługującego mnóstwo czujników i posiadającego mnóstwo funkcji. Firmware zaprojektowałem w taki sposób, aby możliwe było stworzenie wielu wersji oprogramowania AFE obsługującego tylko dedykowane potrzeby
* logika sterowaniać w systemie do automatyki domowej, a nie w urządzeniu
> głęboko wierzę, że urządzenia automatyki domowej powinny być kontrolowane przez system automatyki domowej, który powinien być centralnym miejscem do sterowania i monitorowania naszego domu. Oznacza to, że w moim oprogramowaniu nie znajdziesz wiele funkcji zapewniających logikę np. timery, kalendarze, itp - z pewnymi wyjątkami 😉
* prosta konfiguracja w intuicyjnym panelu konfiguracyjna
> co do zasady, zależy mi żeby AFE Firmware było łatwe i proste w konfiguracji. Sama materia konfiguracji nie jest łatwa dla osoby zupełnie oderwanej od IT/Elektroniki. Dodatkowo możliwości techniczne głównie pojemność pamięci do zapisywania danych uniemożliwiają budowanie zaawansowanych, przez to bardziej intuicyjnych interfejsów do konfiguracji urządzenia.