---
title: 'Konfiguracja diody LED'
---

##### GPIO
* Numer GPIO do którego podłączona jest dioda LED
* Zakres od 0 do 16

!!!! Aby dodać kolejną diodę LED należy wprowadzić najpierw ilość podłączonych do modułu diod LED w [konfiguracji urządzenia](/konfiguracja/konfiguracja-urzadzenia).

##### Zmienić świecenie diody LED na odwrotne
* Parametrem można odwrócić działanie diody LED, przykład
	* jeśli dioda sygnalizuje stan przekaźnika to może świecić, gdy przekaźnik jest właćzony lub wyłączony
	* jeśli dioda jest ma funkcję diody systemowej, to może świecić lub gasnąć, gdy zajdzie zdarzenie systemowe 

#### Sekcja: dioda systemowa

Dioda Systemowa wykorzystywana jest do informowania o stanie urządzenia. [Więcej informacji o diodzie systemowej znajdzesz tutaj.](/konfiguracja/konfiguracja-diody-led/dioda-systemowa)

##### Dioda Systemowa
* Do wyboru jest jedna z dostępny diod LED

! Nie zalecam, aby jedna dioda LED posiadała jednocześnie funkcję diody LED oraz informowania o stanie przekaźnika.