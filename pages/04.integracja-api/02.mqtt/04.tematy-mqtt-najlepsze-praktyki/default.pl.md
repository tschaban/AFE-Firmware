---
title: 'Tematy MQTT: najlepsze praktyki'
media_order: mqtt-temat-przyklad.png
recaptchacontact:
    enabled: false
---

Temat MQTT to łańcuch tekstowy w formacie UTF-8, który MQTT Broker używa do filtrowania wiadomości od różnych podłączonych do niego klientów (np. urządzeń)

Temat MQTT zawiera jeden lub wiele poziomów tematów. Każdy temat oddzielony jest znakiem **/** (forward slash)

![](mqtt-temat-przyklad.png)

Przykłady tematów
``` 
dom/lazienka/wilgotnosc
 warszawa/mokotow/domaniewska/temperatura
 12ad-1646-543/samochod/toyota/polozenie
```

* Każdy temat powinien zawierać przynajmniej jeden znak
* Dozwolony jest znak spacji w temacie
* Tematy MQTT mogą mieć duże i małe znaki, które są rozróżnialne

> Te dwa tematy są różnymi tematami MQTT
> * dom/kuchnia/lampa
> * dom/Kuchnia/lampa

! Nie używaj na początku tematu znaku $ oraz znaków #. Mają one specjalną funkcję 

##### Najlepsze praktyki

**Nie używaj znaku / (forward slash) na początku wiadomości **

> Aczkolwiek format wiadomości MQTT nie zabrania używania tego znaku na początku, ale dodanie go powoduje, że do teamtu dodawany jest dodatkowy poziom ze znakiem 0. Jest to niepotrzebne i może wprowadzać zamieszanie

**Nie używaj znaku spacji**
    
> Jak w przypadku Forward Slash znak spacji jest dozwolony, ale ponieważ, gdy coś jest dozwolone, nie oznacza to, że powinno być używane. Używanie znaku spacji to zmora programistów i powinna, jeśli jest to możiwe, być unikana. Ten znak przeszkadza np. w debugowaniu kodu. 
 
**Twórz tematy krótkie i zwięzłe**
    
> Temat jest zawarty w każdej wiadomości, która jest wysyłana. Zalecam, aby Twoje tematy były jak najkrótsze i zwięzłe. Jeśli chodzi o urządzenia w automatyce to na ogół mają swoje ograniczenia jeśli chodzi o pamięć. Dla takich urządzeń liczy się każdy bajt i długość tematu. Dodatkowo im dłuższy temat tym więcej informacji musi zostać przesłane po sieci WiFi 
  
**Używaj tylko znaków ASCII, unikaj znaków specjalnych **

 >Ponieważ znaki UTF-8 spoza ASCII często wyświetlają się niepoprawnie, bardzo trudno jest znaleźć literówki lub problemy związane z zestawem znaków. Jeśli nie jest to absolutnie konieczne, zalecam unikanie używania znaków spoza ASCII w temacie, w tym na przykłąd polskich znaków: óęćść itp.
 
**Nie zapomnij o przyszłości rozwoju Twojego systemu**
  
 > Zastanów się, w jaki sposób tematy mogą zostać rozszerzane / dodawane, aby uwzględnić nowe funkcje lub urządzenia. Na przykład, jeśli dodasz nowe urządzenie do Twojego inteligentnego domu np. czujniki, to powinno być możliwe dodanie go do drzewa tematów bez zmiany całej hierarchii tematów.

**Używaj konkretnych tematów, a nie ogólnych**
    
>  Zalecam tworzenie konkretnych tematów. Na przykład, jeśli masz trzy czujniki w salonie, utwórz tematy  
>  * dom/salon/lampa
>  * dom/salon/tempetatura
>  * dom/salon/przycisk
> Nie wysyłaj wszystkich wartości przez 
>  * dom/salon
>  Użycie pojedynczego tematu dla wszystkich wiadomości nie jest dobrą praktyką