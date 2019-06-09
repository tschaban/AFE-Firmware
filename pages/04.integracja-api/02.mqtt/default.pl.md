---
title: MQTT
---

Urządzeniem można sterować wiadomościami MQTT. Jest to zalecana przeze mnie metoda sterowaniem urządzeniem z AFE Firmware.

MQTT to bardzo prosty i lekki protokół do wymiany informacji oparty o mechanizm publikowania wiadomości i subskrybowania się do tematów. Został specjalnie opracowany do wymiany danych w sieciach, gdzie mogą występować niskie przepustowości, długie czasy oczekiwań, czy zwyczajnie problem z jej działaniem. Protokół mimo potencjalnych problemów z siecią zapewnia niezawodność. Idealnie nadaje się do wymiany danych między urządzeniami, IoT, ma szerokie zastosowanie w automatyce.

Wymiana danych między urządzeniami odbywa się za pomocą MQTT Brokera. Broker jest pośrednikiem wymiany informacji między urządzeniami, serwisami, oprogramowaniem