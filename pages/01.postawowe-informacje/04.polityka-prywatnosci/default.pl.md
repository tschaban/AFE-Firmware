---
title: 'Polityka prywatności'
process:
    markdown: true
    twig: true
recaptchacontact:
    enabled: false
---

Na tej stronie znajdziesz informacje dotyczące gromadzenia, przetwarzania i wykorzystywania informacji o użytkownikach oprogramowania AFE Fimware

{% if config.get('plugins.page-toc.active') or attribute(page.header, 'page-toc').active %}
<div class="page-toc">
    {% set table_of_contents = toc(page.content, 5,2) %}
    {% if table_of_contents is not empty %}
    {{ table_of_contents }}
    {% endif %}
</div>
{% endif %}

---

##### Czy gromadzone są jakiekolwiek dane osobowe użytkowników oprogramowania AFE Firmware?

> **Nie.** Nie gromadzone są, żadne dane osobowe użytkowników oprogramowania AFE Firmware w tym identyfikatory internetowe, jak np.: adres IP

##### Czy gromadzone są jakiekolwiek dane dotyczące konfiguracją urządzenia z oprogramowaniem AFE Firmware?

> **Nie.** Dane konfiguracji urządzenia z zainstalowanym oprogramowanie AFE Firmware są przechowywane jedynie w pamięci urządzenia.

##### Czy gromadzone są jakiekolwiek dane podczas użytkowania oprogramowaniem AFE Firmware?

> **Nie.** Dane związane z użytkowaniem urządzenia z AFE Firmware np. włączanie / wyłączanie przekaźnika, nie są w żaden sposób gromadzone

##### Czy gromadzone są jakiekolwiek dane dotyczące oprogramowania AFE Firmware.

> **Tak.** Po zainstalowaniu oprogramowania, zapisywane są w bazie danych autora oprogramowania AFE Firmware dane nieosobowe, takie jak:

* Typ zainstalowanego AFE Firmware
* Wersja zainstalowanego AFE Firmware
* Data instalacji
* Data aktualizacji oprogramowania do innej  wersji
* Kraj oraz miejscowość, w którym AFE Firmware został zainstalowany
* Numer seryjny oprogramowania

##### Czy gromadzone dane umożliwiają identyfikację bezpośrednio lub pośrednio użytkownika oprogramowania AFE Firmware?

> **Nie.** Nie ma możliwości, jakiekolwiek identyfikacji użytkownika na podstawie informacji wymienionych powyżej.

!! Zapisywane dane nie pozwalają na identyfikację użytkownika oprogramowania AFE Firmware.

##### Do czego wykorzystywane są gromadzone dane?

> Autor oprogramowania zbiera informacje o typie oraz wersji oprogramowania na potrzeby uzyskania odpowiedzi na pytania 

* jakie wersje oprogramowania są instalowane oraz używane
* kiedy oprogramowanie zostało zainstalowane lub zaktualizowane do innej wersji

> oraz wprowadzenia mechanizmów automatycznej aktualizacji oprogramowania; udostępniania dodatkowych funkcji rozszerzających możliwości urządzenia z oprogramowaniem AFE (w przygotowaniu)


##### Czy gromadzone dane są udostępniane?

> Zagregowanie dane statystyczne o zainstalowanych typach i wersjach oprogramowania AFE Firmware mogą zostać publicznie udostępnione na stronie: [Smart'ny Dom](https://www.smartnydom.pl)


[aaaa]: test
