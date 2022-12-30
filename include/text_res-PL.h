/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

#ifndef __TEXT_RES_H
#define __TEXT_RES_H
#include <Arduino.h>

// Здесь строковые константы общего назначения, включая константы из интерфейса, что позволит локализировать при необходимости
// Ничего иного в данном файле не предполагается

/** набор строк с именами эффектов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
static const char TEFF_000[] PROGMEM = ""; // "Puste"
static const char TEFF_001[] PROGMEM = "Biała lampa";
static const char TEFF_002[] PROGMEM = "Kolory";
static const char TEFF_003[] PROGMEM = "Tęcza 2D";
static const char TEFF_004[] PROGMEM = "Konfetti";
static const char TEFF_005[] PROGMEM = "Kolorowe drażetki";
static const char TEFF_006[] PROGMEM = "Burza, Spadająca gwiazda, Meteory";//nie burza
static const char TEFF_007[] PROGMEM = "Macierz";
static const char TEFF_008[] PROGMEM = "Akwarela";
static const char TEFF_009[] PROGMEM = "Świetliki z pióropuszem";//
static const char TEFF_010[] PROGMEM = "Wędrująca kostka";
static const char TEFF_011[] PROGMEM = "Puls";
static const char TEFF_012[] PROGMEM = "Spadające efekty";
static const char TEFF_013[] PROGMEM = "Niespalony krzak";
static const char TEFF_014[] PROGMEM = "Paintball";
static const char TEFF_015[] PROGMEM = "Szum 3D";
static const char TEFF_016[] PROGMEM = "Komórki-Choinki";
static const char TEFF_017[] PROGMEM = "Plamy";
static const char TEFF_018[] PROGMEM = "Tixie Land";
static const char TEFF_019[] PROGMEM = "Nikola Tesla";
static const char TEFF_020[] PROGMEM = "Oscylator";
static const char TEFF_021[] PROGMEM = "Burza";
static const char TEFF_022[] PROGMEM = "Сzarodziejka";
static const char TEFF_023[] PROGMEM = "Źródło";
static const char TEFF_024[] PROGMEM = "Odbijające się piłki";
static const char TEFF_025[] PROGMEM = "Sinusoida";
static const char TEFF_026[] PROGMEM = "Metasfery";
static const char TEFF_027[] PROGMEM = "Spirala";
static const char TEFF_028[] PROGMEM = "Komety, pływające światła ++";
static const char TEFF_029[] PROGMEM = "Bubbler";
static const char TEFF_030[] PROGMEM = "Pryzmat";
static const char TEFF_031[] PROGMEM = "Stado i Drapieżnik";
static const char TEFF_032[] PROGMEM = "Wir";
static const char TEFF_033[] PROGMEM = "Dryft";
static const char TEFF_034[] PROGMEM = "Popcorn";
static const char TEFF_035[] PROGMEM = "Migotanie";
static const char TEFF_036[] PROGMEM = "Radar";
static const char TEFF_037[] PROGMEM = "Fale";
static const char TEFF_038[] PROGMEM = "Ogień 2012";
static const char TEFF_039[] PROGMEM = "Światła bengalskie";
static const char TEFF_040[] PROGMEM = "Kulki";
static const char TEFF_041[] PROGMEM = "DNA";
static const char TEFF_042[] PROGMEM = "Ogień 2018";
static const char TEFF_043[] PROGMEM = "Zamek szyfrowy";
static const char TEFF_044[] PROGMEM = "Cube 2D";
static const char TEFF_045[] PROGMEM = "Magma";
static const char TEFF_046[] PROGMEM = "Picasso";
static const char TEFF_047[] PROGMEM = "Gwiezdny Desant";
static const char TEFF_048[] PROGMEM = "Flagi";
static const char TEFF_049[] PROGMEM = "Zworki";
static const char TEFF_050[] PROGMEM = "Wiry";
static const char TEFF_051[] PROGMEM = "Gwiazdy";
static const char TEFF_052[] PROGMEM = "Odbicia wody, kolory";
static const char TEFF_053[] PROGMEM = "Fajerwerki";
static const char TEFF_054[] PROGMEM = "Ocean Spokojny";
static const char TEFF_055[] PROGMEM = "Wyszywanka";
static const char TEFF_056[] PROGMEM = "Kolorowy szum";
static const char TEFF_057[] PROGMEM = "Motyle";
static const char TEFF_058[] PROGMEM = "Cienie";
static const char TEFF_059[] PROGMEM = "Wzory";
static const char TEFF_060[] PROGMEM = "Strzałki";
static const char TEFF_061[] PROGMEM = "Dzikie kule";
static const char TEFF_062[] PROGMEM = "Atrakcja";
static const char TEFF_063[] PROGMEM = "Węże";
static const char TEFF_064[] PROGMEM = "Nexus";
static const char TEFF_065[] PROGMEM = "Labirynt";
static const char TEFF_066[] PROGMEM = "Kręcone kolory";
static const char TEFF_067[] PROGMEM = "Zorza polarna";
static const char TEFF_068[] PROGMEM = "Kwitnienie";
static const char TEFF_069[] PROGMEM = "Wyspa Węży";
static const char TEFF_070[] PROGMEM = "Marzenia dzieci";
static const char TEFF_071[] PROGMEM = "Metabule";
static const char TEFF_072[] PROGMEM = "Lampa lawowa";
static const char TEFF_073[] PROGMEM = "Cosmo-Racer";
static const char TEFF_074[] PROGMEM = "Dym";
static const char TEFF_075[] PROGMEM = "Lampa ognia";
static const char TEFF_076[] PROGMEM = "Miraż";
static const char TEFF_250[] PROGMEM = "Zegar";
static const char TEFF_251[] PROGMEM = "Odtwarzacz RGB 332/565";
static const char TEFF_254[] PROGMEM = "Analizator częstotliwości";
static const char TEFF_255[] PROGMEM = "Oscyloskop";

#define DFTINTF_00A "Efekt"
#define DFTINTF_020 "Mikrofon"
#define DFTINTF_084 "Paleta"
#define DFTINTF_088 "Skala"
#define DFTINTF_0D0 "Kolor"
#define DFTINTF_0D1 "Paleta/Skala"
#define DFTINTF_0D2 "Efekt (0: cykliczny)"
#define DFTINTF_0D3 "Przesunięcie koloru (0: cykliczne)"
#define DFTINTF_0D4 "Subpiksel"
#define DFTINTF_0D5 "Ilość"
#define DFTINTF_0D6 "Generator kolorów"
#define DFTINTF_0D7 "Odcień"
#define DFTINTF_0D8 "Pożar!"
#define DFTINTF_0D9 "Ilość/Kolor (podświetlenie nocne)"
#define DFTINTF_0DA "Nasycenie"
#define DFTINTF_0DB "Wzmocnienie"
#define DFTINTF_0DC "Przemiatanie"
#define DFTINTF_0DD "Równoległość"
#define DFTINTF_0DE "Rozmycie"
#define DFTINTF_0DF "Fliter"
#define DFTINTF_0E0 "Lampa nocna"
#define DFTINTF_0E1 "Pierścienie"
#define DFTINTF_0E2 "Odblask (0: Wyłącz)"
#define DFTINTF_0E3 "Tryb"
#define DFTINTF_0E4 "Prędkość liniowa"
#define DFTINTF_0E5 "Tryb (1: losowy)"
#define DFTINTF_0E6 "Przesunięcie"
#define DFTINTF_0E7 "Ilość/Kierunek/Pozycja"
#define DFTINTF_0E8 "Predator"
#define DFTINTF_0E9 "Masa"
#define DFTINTF_0EA "Prędkość (pion)"
#define DFTINTF_0EB "Prędkość (poziom)"
#define DFTINTF_0EC "Wzór (0: losowy)"
#define DFTINTF_0ED "Zaciemnianie"
#define DFTINTF_0EE "Energia"
#define DFTINTF_0EF "Gęstość"
#define DFTINTF_0F0 "Kolor/Obrót"
#define DFTINTF_0F1 "Pętla (0: brak)"
#define DFTINTF_0F2 "Częstotliwość uruchomień"
#define DFTINTF_0F3 "Obróć"
#define DFTINTF_0F4 "Paleta (0: generator)"
#define DFTINTF_0F5 "Wygładzanie"
#define DFTINTF_0F6 "Wypełnij"
#define DFTINTF_0F7 "Grubość"
#define DFTINTF_0F9 "Zachowanie: 1/2"
#define DFTINTF_0FA "Kolor: 1/2"
#define DFTINTF_0FB "Paleta (0: brak koloru)"
#define DFTINTF_0FC "Kolor (1: losowy, 255: biały)"
#define DFTINTF_0FD "Opad śniegu/opad gwiazd/meteor"
#define DFTINTF_0FE "Ilość"
//#define DFTINTF_0FF "Typ 1/2, Rozmycie 3/4"
#define DFTINTF_100 "Paleta (0: losowy kolor)"
#define DFTINTF_101 "Kolor 1 (0: cykliczny)"
#define DFTINTF_102 "Chmura"
#define DFTINTF_103 "Rozmycie (0: brak)"
#define DFTINTF_104 "Rozpiętość"
#define DFTINTF_105 "Filtr"
#define DFTINTF_106 "Fizyka"
#define DFTINTF_107 "Kręgi"
#define DFTINTF_108 "Opcja 1/2"
#define DFTINTF_109 "Błyski"
#define DFTINTF_10A "Usterka"
#define DFTINTF_10B "Błyskawica"
#define DFTINTF_10C "Liczba pocisków"
#define DFTINTF_10D "Rozmiar (szerokość)"
#define DFTINTF_10E "Przesunięcie koloru (0: losowo)"
#define DFTINTF_10F "Rozmiar (wysokość)"
#define DFTINTF_110 "Klasyczny"
#define DFTINTF_111 "Jeden kolor"
#define DFTINTF_112 "Odwróć kolory"
#define DFTINTF_113 "Kolor 2 (0: cykliczny)"
#define DFTINTF_114 "Jakość / Szybkość"
#define DFTINTF_115 "Rozmycie (1-4 z wiatrem)"
#define DFTINTF_116 "Przesunięcie sinusoidalne"
#define DFTINTF_117 "Kierunek (0: cykliczny)"
#define DFTINTF_118 "Próg sygnału"
#define DFTINTF_11A "Kolor (1 = cykliczny)"


/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Efekty";
static const char TINTF_001[] PROGMEM = "Wyjście tekstowe";
static const char TINTF_002[] PROGMEM = "Ustawienia";
static const char TINTF_003[] PROGMEM = "Na liście glównej";
static const char TINTF_004[] PROGMEM = "Na liście \"Demo\"";
static const char TINTF_005[] PROGMEM = "Kopiuj";
static const char TINTF_006[] PROGMEM = "Usuń";
static const char TINTF_007[] PROGMEM = "Aktualizuj listę";
static const char TINTF_008[] PROGMEM = "Zapisz";
static const char TINTF_009[] PROGMEM = "Zarządzanie listą efektów";
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
static const char TINTF_00B[] PROGMEM = "Wyjście";
static const char TINTF_00C[] PROGMEM = "Globalna jasność";
static const char TINTF_00D[] PROGMEM = "Jasność";
static const char TINTF_00E[] PROGMEM = "Włącz";
static const char TINTF_00F[] PROGMEM = "\"Demo\"";
static const char TINTF_010[] PROGMEM = "Zobacz jasność";
static const char TINTF_011[] PROGMEM = "Wydarzenia";
static const char TINTF_012[] PROGMEM = "Mikrofon";
static const char TINTF_013[] PROGMEM = "Przycisk";
static const char TINTF_014[] PROGMEM = "Dodatkowo...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "Czekaj...";
static const char TINTF_018[] PROGMEM = "Konfiguracje";
static const char TINTF_019[] PROGMEM = "Pobierz";
static const char TINTF_01A[] PROGMEM = "Konfiguracja";
static const char TINTF_01B[] PROGMEM = "Utwórz";
static const char TINTF_01C[] PROGMEM = "Wyjświetlić na lampie";
static const char TINTF_01D[] PROGMEM = "Tekst";
static const char TINTF_01E[] PROGMEM = "Kolor wiadomości";
static const char TINTF_01F[] PROGMEM = "Wyślij";
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Mikrofon aktywny";
static const char TINTF_022[] PROGMEM = "Współczynnik korekcji zera";
static const char TINTF_023[] PROGMEM = "Poziom hałasu";
static const char TINTF_024[] PROGMEM = "Filtr szumów";
static const char TINTF_025[] PROGMEM = "Kalibracja mikrofonu";
static const char TINTF_026[] PROGMEM = "Włącz mikrofon";
static const char TINTF_027[] PROGMEM = "... w procesie ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Klient (STA)";
static const char TINTF_02A[] PROGMEM = "Ustawienia klienta WiFi";
static const char TINTF_02B[] PROGMEM = "Nazwa lampy (nazwa hosta mDNS/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "Sieć WiFi";
static const char TINTF_02D[] PROGMEM = "Hasło";
static const char TINTF_02E[] PROGMEM = "Połącz";
static const char TINTF_02F[] PROGMEM = "Punkt dostępu (AP)";
static const char TINTF_030[] PROGMEM = "Wyświetlanie";
static const char TINTF_031[] PROGMEM = "Ustawienia WiFi i punktu dostępu";
static const char TINTF_032[] PROGMEM = "W trybie AP lampa zawsze działa jako punkt dostępowy i nie łączy się z innymi sieciami WiFi";
static const char TINTF_033[] PROGMEM = "Tryb WiFi";
static const char TINTF_034[] PROGMEM = "Chroń AP hasłem";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "Host MQTT";
static const char TINTF_037[] PROGMEM = "Port MQTT";
static const char TINTF_038[] PROGMEM = "Użytkownik";
static const char TINTF_039[] PROGMEM = "Interwał MQTT sek.";
static const char TINTF_03A[] PROGMEM = "Połącz";
static const char TINTF_03B[] PROGMEM = "Odbicie poziomowe";
static const char TINTF_03C[] PROGMEM = "Odbicie pionowe";
static const char TINTF_03D[] PROGMEM = "Płynna zmiana efektów";
static const char TINTF_03E[] PROGMEM = "Losowy efekt w Demo";
static const char TINTF_03F[] PROGMEM = "Zmiana efektu w Demo. (sek.)";
static const char TINTF_040[] PROGMEM = "Sortowanie efektów";
static const char TINTF_041[] PROGMEM = "Kopie pod oryginałem";
static const char TINTF_042[] PROGMEM = "Kopie na końcu listy";
static const char TINTF_043[] PROGMEM = "Według liczby";
static const char TINTF_044[] PROGMEM = "Prędkość odtwarzania tekstu";
static const char TINTF_045[] PROGMEM = "Przenieś tekst w górę";
static const char TINTF_046[] PROGMEM = "Mieszany (AP+STA)";
static const char TINTF_047[] PROGMEM = "Nie pokazuj";
static const char TINTF_048[] PROGMEM = "Co godzinę";
static const char TINTF_049[] PROGMEM = "Co pół godziny";
static const char TINTF_04A[] PROGMEM = "Co 15 minut";
static const char TINTF_04B[] PROGMEM = "Co 10 minut";
static const char TINTF_04C[] PROGMEM = "Co 5 minut";
static const char TINTF_04D[] PROGMEM = "Co minutę";
static const char TINTF_04E[] PROGMEM = "Życzenia noworoczne";
static const char TINTF_04F[] PROGMEM = "Okres wyświetlania w minutach (0 - nie wyświetlaj)";
static const char TINTF_050[] PROGMEM = "Data/godzina nowego roku w formacie YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Data / Czas / Strefa czasowa";
static const char TINTF_052[] PROGMEM = "Ustawienia strefy czasowej. Reguły strefy czasowej i czasu sezonowego są stosowane automatycznie, ręczna korekta nie jest wymagana. Jeśli twoja strefa nie znajduje się na liście, wybierz wspólną strefę przesunięcia z Greenwich";
static const char TINTF_053[] PROGMEM = "Strefa czasowa";
static const char TINTF_054[] PROGMEM = "Zapasowy serwer NTP (opcjonalnie)";
static const char TINTF_055[] PROGMEM = "Data/czas w formacie RRRR-MM-DDThh:mm:ss (Pobierać od klienta, jeśli pole jest puste i nie ma internetu)";
static const char TINTF_056[] PROGMEM = "Aktualizuj";
static const char TINTF_057[] PROGMEM = "Aktualizuj OTA-PIO";
static const char TINTF_058[] PROGMEM = "Start";
static const char TINTF_059[] PROGMEM = "Ładowanie oprogramowania";
static const char TINTF_05A[] PROGMEM = "Prześlij";
static const char TINTF_05B[] PROGMEM = "Zdarzenie";
static const char TINTF_05C[] PROGMEM = "Edytuj";
static const char TINTF_05D[] PROGMEM = "Dodaj";
static const char TINTF_05E[] PROGMEM = "Aktywny";
static const char TINTF_05F[] PROGMEM = "Typ zdarzenia";
static const char TINTF_060[] PROGMEM = "Włącz lampę";
static const char TINTF_061[] PROGMEM = "Wyłącz lampę";
static const char TINTF_062[] PROGMEM = "Tryb \"Demo\"";
static const char TINTF_063[] PROGMEM = "Budzik";
static const char TINTF_064[] PROGMEM = "Wczytaj ustawienia lampy";
static const char TINTF_065[] PROGMEM = "Wczytaj ustawienia efektów";
static const char TINTF_066[] PROGMEM = "Wczytaj ustawienia zdarzenia";
static const char TINTF_067[] PROGMEM = "Wyświetl tekst";
static const char TINTF_068[] PROGMEM = "Wyświetl czas";
static const char TINTF_069[] PROGMEM = "Stan pinu";
static const char TINTF_06A[] PROGMEM = "Włącz AUX";
static const char TINTF_06B[] PROGMEM = "Wyłącz AUX";
static const char TINTF_06C[] PROGMEM = "Przełącz stan AUX";
static const char TINTF_06D[] PROGMEM = "Data/czas zdarzenia";
static const char TINTF_06E[] PROGMEM = "Powtórz, min";
static const char TINTF_06F[] PROGMEM = "Zatrzymaj po, min";
static const char TINTF_070[] PROGMEM = "Parametr (tekst)";
static const char TINTF_071[] PROGMEM = "Powtórz";
static const char TINTF_072[] PROGMEM = "Poniedziałek";
static const char TINTF_073[] PROGMEM = "Wtorek";
static const char TINTF_074[] PROGMEM = "Środa";
static const char TINTF_075[] PROGMEM = "Czwartek";
static const char TINTF_076[] PROGMEM = "Piątek";
static const char TINTF_077[] PROGMEM = "Sobota";
static const char TINTF_078[] PROGMEM = "Niedziela";
static const char TINTF_079[] PROGMEM = "Aktualizuj";
static const char TINTF_07A[] PROGMEM = "Akcja";
static const char TINTF_07B[] PROGMEM = "Przycisk jest aktywny";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Wstrzymaj";
static const char TINTF_07E[] PROGMEM = "Naciśnięcie";
static const char TINTF_07F[] PROGMEM = "Razowe";
static const char TINTF_080[] PROGMEM = "Lampa ognia";
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Inne";
static const char TINTF_083[] PROGMEM = "Wyczyść lampę podczas zmiany efektów";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "W porządku alfabetycznym (wewnętrznie)";
static const char TINTF_086[] PROGMEM = "Zdarzenia są aktywne";
static const char TINTF_087[] PROGMEM = "Prędkość";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
static const char TINTF_089[] PROGMEM = "Nazwa efektu";
static const char TINTF_08A[] PROGMEM = "W porządku alfabetycznym (konfiguracja)";
static const char TINTF_08B[] PROGMEM = "Zresetuj ustawienia efektu";
static const char TINTF_08C[] PROGMEM = "Prefiks tematów";
static const char TINTF_08D[] PROGMEM = "Sortuj według mikrofonu";
static const char TINTF_08E[] PROGMEM = "Debugowanie";
static const char TINTF_08F[] PROGMEM = "Ustawienia ESP";
static const char TINTF_090[] PROGMEM = "Numerowanie";
static const char TINTF_091[] PROGMEM = "Mikrofon";
static const char TINTF_092[] PROGMEM = "Ustawienia pinów";
static const char TINTF_093[] PROGMEM = "Pokaż ustawienia ESP";
static const char TINTF_094[] PROGMEM = "Przypinka";
static const char TINTF_095[] PROGMEM = "Limit prądu (mA)";
static const char TINTF_096[] PROGMEM = "Uruchom ponownie";
static const char TINTF_097[] PROGMEM = "Przypnij RX DFPlayer";
static const char TINTF_098[] PROGMEM = "Przypnij TX DFPlayer";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Nazwa efektu w demo";
static const char TINTF_09B[] PROGMEM = "Głośność";
static const char TINTF_09C[] PROGMEM = "Czas";
static const char TINTF_09D[] PROGMEM = "Nazwa efektu";
static const char TINTF_09E[] PROGMEM = "Zgodnie z efektem";
static const char TINTF_09F[] PROGMEM = "Wyłączone";
static const char TINTF_0A0[] PROGMEM = "Pierwszy";
static const char TINTF_0A1[] PROGMEM = "Losowo";
static const char TINTF_0A2[] PROGMEM = "Losowe MP3";
static const char TINTF_0A3[] PROGMEM = "Dźwięk budzika";
static const char TINTF_0A4[] PROGMEM = "Drugi";
static const char TINTF_0A5[] PROGMEM = "Trzeci";
static const char TINTF_0A6[] PROGMEM = "Czwarty";
static const char TINTF_0A7[] PROGMEM = "Piąty";
static const char TINTF_0A8[] PROGMEM = "Wyrównywacz";
static const char TINTF_0A9[] PROGMEM = "Normalny";
static const char TINTF_0AA[] PROGMEM = "Pop";
static const char TINTF_0AB[] PROGMEM = "Rok";
static const char TINTF_0AC[] PROGMEM = "Jazz";
static const char TINTF_0AD[] PROGMEM = "Klasyka";
static const char TINTF_0AE[] PROGMEM = "Bass";
static const char TINTF_0AF[] PROGMEM = "Odtwarzacz MP3";
static const char TINTF_0B0[] PROGMEM = "Liczba plików w folderze MP3";
static const char TINTF_0B1[] PROGMEM = "Tryby dźwięku";
static const char TINTF_0B2[] PROGMEM = "Folder\\numer pliku dźwiękowego (przykłady: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Limit głośności budzika";
static const char TINTF_0B4[] PROGMEM = "Domyślny";
static const char TINTF_0B5[] PROGMEM = "Usuń z listy";
static const char TINTF_0B6[] PROGMEM = "Wyłączone";
static const char TINTF_0B7[] PROGMEM = "Opcja 1";
static const char TINTF_0B8[] PROGMEM = "Opcja 2";
static const char TINTF_0B9[] PROGMEM = "Parametr";
static const char TINTF_0BA[] PROGMEM = "Budzik \"Świt\"";
static const char TINTF_0BB[] PROGMEM = "Czas trwania świtu";
static const char TINTF_0BC[] PROGMEM = "Świat po świcie";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "sekundy";
static const char TINTF_0C2[] PROGMEM = "minuty";
static const char TINTF_0C3[] PROGMEM = "godzina";
static const char TINTF_0C4[] PROGMEM = "dni";
static const char TINTF_0C5[] PROGMEM = "dni";
static const char TINTF_0C6[] PROGMEM = "dzień";
static const char TINTF_0C7[] PROGMEM = "godzin";
static const char TINTF_0C8[] PROGMEM = "godzina";
static const char TINTF_0C9[] PROGMEM = "Przypadek.";
static const char TINTF_0CA[] PROGMEM = "Przyciemnianie tła";
static const char TINTF_0CB[] PROGMEM = "Ostrzeżenie";
static const char TINTF_0CC[] PROGMEM = "minuty";
static const char TINTF_0CD[] PROGMEM = "minuta";
static const char TINTF_0CE[] PROGMEM = "Rysunek";
static const char TINTF_0CF[] PROGMEM = "Wypełnianie";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Najpierw dźwięk";
static const char TINTF_0D2[] PROGMEM = "Ogranicz głośność";
static const char TINTF_0D3[] PROGMEM = "Współczynnik prędkości";
static const char TINTF_0D4[] PROGMEM = "Ustawienia wyświetlania";
static const char TINTF_0D5[] PROGMEM = "Jasność (0 - wyłącz)";
static const char TINTF_0D6[] PROGMEM = "Jasność przy wyłączonej lampie (0 - wyłącz)";
static const char TINTF_0D7[] PROGMEM = "format 24-godzinny";
static const char TINTF_0D8[] PROGMEM = "Lider 0";
static const char TINTF_0D9[] PROGMEM = "Wyczyść";
static const char TINTF_0DA[] PROGMEM = "Skanuj";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Kontrola";
static const char TINTF_0DD[] PROGMEM = "Typ skali";
static const char TINTF_0DE[] PROGMEM = "Kolor skali";
static const char TINTF_0DF[] PROGMEM = "Kolor tekstu";
static const char TINTF_0E0[] PROGMEM = "Pokaż temperaturę";
static const char TINTF_0E1[] PROGMEM = "Jesteś pewien?";
static const char TINTF_0E2[] PROGMEM = "Nadawanie";
static const char TINTF_0E3[] PROGMEM = "Tryb";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Blok.Efekt";
static const char TINTF_0E7[] PROGMEM = "Markup";
static const char TINTF_0E8[] PROGMEM = "Początkowy Universe";
static const char TINTF_0E9[] PROGMEM = "Wczytaj konfigurację przycisku";
static const char TINTF_0EA[] PROGMEM = "Lampa nocna";
static const char TINTF_0EB[] PROGMEM = "Lampa";
static const char TINTF_0EC[] PROGMEM = "Mimo to";
static const char TINTF_0ED[] PROGMEM = "Powiedzieć czas";
static const char TINTF_0EE[] PROGMEM = "Wyłączone";
static const char TINTF_0EF[] PROGMEM = "Pionowo";
static const char TINTF_0F0[] PROGMEM = "Poziomo";
static const char TINTF_0F1[] PROGMEM = "Tryb odtwarzacza";
static const char TINTF_0F2[] PROGMEM = "Odtwarzacz MP3 + reset";
static const char TINTF_0F3[] PROGMEM = "Lista";
static const char TINTF_0F4[] PROGMEM = "Resetowanie ustawień lampy";
static const char TINTF_0F5[] PROGMEM = "Operacja anulowana";
static const char TINTF_0F6[] PROGMEM = "Sprawdzanie efektów";

static const char NY_MDG_STRING1[] PROGMEM = "%d %s pozostało do nowego roku";
static const char NY_MDG_STRING2[] PROGMEM = "Szczęśliwego Nowego Roku %d!";
//-------------------------

#endif