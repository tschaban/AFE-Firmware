---
title: 'Zanim zainstalujesz firmware'
---

* Urządzenie z firmware AFE wymaga systemu/aplkacji do sterowania tym urządzeniem. Będzie potrzebny własny serwer z oprogramowaniem np. openHAB, DomotiGa, HomeAssistant, Domoticz. Tego typu oprogramowanie może zostać zainstalowane na mikrokomputerach typu Raspberry PI. Możliwe jest również napisanie własnej aplikacji do sterowania urządzeniem z oprogramowaniem AFE, gdyż AFE daje otwarte mechanizmy kontroli przez HTTP lub MQTT
* Jeśli AFE wgrywane jest do urządzeń Sonoff, to nie jest możliwe sterowanie tymi urządzeniami oryginalną aplikacją Ewelink
* AFE nie współpracuje z oprogramowaniem Supla. Nie można sterować urządzeniami z firmware AFE oprogramowaniem Supla. Supla działa tylko z urządzeniami z zainstalowanym firmware Supla.
* Jeśli nie zgrasz zawartość pamięci flash z urządzenia Sonoff, to po wgraniu AFE, jak każdego innego alternatywnego firmware, nie będzie możliwe przywrócenie oryginalnego oprogramowania w Sonoff.
* LICENCJA: Oprogramowanie dostępne jest na licencji otwartego oprogramowania MIT
* POLITYKA PRYWATNOŚCI: treść znajdziesz tutaj.

!! Jeśli nie rozumiesz powyższych punktów lub z którymiś nie zgadzasz się, to nie możesz instalować oprogramowania AFE Firmware