/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
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
static const char TEFF_000[] PROGMEM = "";  // "пустышка"
static const char TEFF_001[] PROGMEM = "Lampada bianca";
static const char TEFF_002[] PROGMEM = "Colori";
static const char TEFF_003[] PROGMEM = "Arcobaleno 2D";
static const char TEFF_004[] PROGMEM = "Coriandoli";
static const char TEFF_005[] PROGMEM = "Confetti colorati";
static const char TEFF_006[] PROGMEM = "Bufera, Stelle cadenti, Meteorite";
static const char TEFF_007[] PROGMEM = "Matrice";
static const char TEFF_008[] PROGMEM = "Acquerello";
static const char TEFF_009[] PROGMEM = "Lucciole";
static const char TEFF_010[] PROGMEM = "Un cubo errante";
static const char TEFF_011[] PROGMEM = "Polso";
static const char TEFF_012[] PROGMEM = "Effetti in caduta";
static const char TEFF_013[] PROGMEM = "Roveto ardente"; 
static const char TEFF_014[] PROGMEM = "Paintball";
static const char TEFF_015[] PROGMEM = "Rumore 3D";
static const char TEFF_016[] PROGMEM = "Celle-alberi di Natale";
static const char TEFF_017[] PROGMEM = "Macchie";
static const char TEFF_018[] PROGMEM = "Tixie Land";
static const char TEFF_019[] PROGMEM = "Nikola Tesla";
static const char TEFF_020[] PROGMEM = "Oscillatore";
static const char TEFF_021[] PROGMEM = "Tempesta";
static const char TEFF_022[] PROGMEM = "Fata";
static const char TEFF_023[] PROGMEM = "Fonte";
static const char TEFF_024[] PROGMEM = "Palle rimbalzanti";
static const char TEFF_025[] PROGMEM = "Sinusoide";
static const char TEFF_026[] PROGMEM = "Metasfere";
static const char TEFF_027[] PROGMEM = "Spirale";
static const char TEFF_028[] PROGMEM = "Comete, Luci Galleggianti ++";
static const char TEFF_029[] PROGMEM = "Bulburatore";
static const char TEFF_030[] PROGMEM = "Prisma";
static const char TEFF_031[] PROGMEM = "Mandria e predatore";
static const char TEFF_032[] PROGMEM = "Vortice";
static const char TEFF_033[] PROGMEM = "Deriva";
static const char TEFF_034[] PROGMEM = "Popcorn";
static const char TEFF_035[] PROGMEM = "Sfarfallio";
static const char TEFF_036[] PROGMEM = "Radar";
static const char TEFF_037[] PROGMEM = "Onde";
static const char TEFF_038[] PROGMEM = "Fuoco 2012";
static const char TEFF_039[] PROGMEM = "Luci del Bengala";
static const char TEFF_040[] PROGMEM = "Palle";
static const char TEFF_041[] PROGMEM = "DNA";
static const char TEFF_042[] PROGMEM = "Fuoco 2018";
static const char TEFF_043[] PROGMEM = "Blocco codice";
static const char TEFF_044[] PROGMEM = "Cubo 2D";
static const char TEFF_045[] PROGMEM = "Magma";
static const char TEFF_046[] PROGMEM = "Picasso";
static const char TEFF_047[] PROGMEM = "Stella Trooper";
static const char TEFF_048[] PROGMEM = "Bandiere";
static const char TEFF_049[] PROGMEM = "Saltatori";
static const char TEFF_050[] PROGMEM = "Vortici";
static const char TEFF_051[] PROGMEM = "Stelle";
static const char TEFF_052[] PROGMEM = "Riflesso dell'acqua, Colori";
static const char TEFF_053[] PROGMEM = "Fuochi d'artificio";
static const char TEFF_054[] PROGMEM = "L'oceano Pacifico";
static const char TEFF_055[] PROGMEM = "Camicia ricamata";
static const char TEFF_056[] PROGMEM = "Rumore di colore";
static const char TEFF_057[] PROGMEM = "Farfalle";
static const char TEFF_058[] PROGMEM = "Ombre";
static const char TEFF_059[] PROGMEM = "Modelli";
static const char TEFF_060[] PROGMEM = "Frecce";
static const char TEFF_061[] PROGMEM = "Palle selvagge";
static const char TEFF_062[] PROGMEM = "Attrazione";
static const char TEFF_063[] PROGMEM = "Serpenti";
static const char TEFF_064[] PROGMEM = "Nesso";
static const char TEFF_065[] PROGMEM = "Labirinto";
static const char TEFF_066[] PROGMEM = "Colori ricci";
static const char TEFF_067[] PROGMEM = "Aurora boreale";
static const char TEFF_068[] PROGMEM = "Sapone";
static const char TEFF_069[] PROGMEM = "Isola dei serpenti";
static const char TEFF_070[] PROGMEM = "Sogni di fuoco";
static const char TEFF_071[] PROGMEM = "Metaballs";
static const char TEFF_072[] PROGMEM = "Lampada di lava";
static const char TEFF_073[] PROGMEM = "Pilota spaziale";
static const char TEFF_074[] PROGMEM = "Fumo";
static const char TEFF_075[] PROGMEM = "Lampada da fuoco";
static const char TEFF_076[] PROGMEM = "Miraggio";
static const char TEFF_077[] PROGMEM = "Caleidoscopio";
static const char TEFF_250[] PROGMEM = "Orologio";
static const char TEFF_251[] PROGMEM = "Lettore RGB 332/565";
static const char TEFF_254[] PROGMEM = "Analizzatore di frequenza";
static const char TEFF_255[] PROGMEM = "Oscillografo";

#define DFTINTF_00A "Effetto"
#define DFTINTF_020 "Microfono"
#define DFTINTF_084 "Tavolozza"
#define DFTINTF_088 "Scala"
#define DFTINTF_0D0 "Colore"
#define DFTINTF_0D1 "Tavolozza/Scala"
#define DFTINTF_0D2 "Effetto (0: ciclico)"
#define DFTINTF_0D3 "Offset colore (0: ciclico)"
#define DFTINTF_0D4 "Subpixel"
#define DFTINTF_0D5 "Quantità"
#define DFTINTF_0D6 "Generatore colore"
#define DFTINTF_0D7 "Tinta"
#define DFTINTF_0D8 "Fuoco!"
#define DFTINTF_0D9 "Quantità/Colore (luce notturna)"
#define DFTINTF_0DA "Saturazione"
#define DFTINTF_0DB "Aumenta"
#define DFTINTF_0DC "spazzare"
#define DFTINTF_0DD "Pulsazione"
#define DFTINTF_0DE "Sfocatura"
#define DFTINTF_0DF "Richiesta"
#define DFTINTF_0E0 "Luce notturna"
#define DFTINTF_0E1 "Anelli"
#define DFTINTF_0E2 "Riflesso (0: Disattiva)"
#define DFTINTF_0E3 "Modalità"
#define DFTINTF_0E4 "Velocità lineare"
#define DFTINTF_0E5 "Modalità (1: casuale)"
#define DFTINTF_0E6 "Frana"
#define DFTINTF_0E7 "Quantità/Direzione/Posizione"
#define DFTINTF_0E8 "Predatore"
#define DFTINTF_0E9 "Predatore"
#define DFTINTF_0EA "Velocità (Vert.)"
#define DFTINTF_0EB "Velocità (Orizz.)"
#define DFTINTF_0EC "Modello (0: casuale)"
#define DFTINTF_0ED "Eclisse"
#define DFTINTF_0EE "Energia"
#define DFTINTF_0EF "Densità"
#define DFTINTF_0F0 "Colore/Rotazione"
#define DFTINTF_0F1 "Ciclo (0: nessuno)"
#define DFTINTF_0F2 "Frequenza dei lanci"
#define DFTINTF_0F3 "Giro"
#define DFTINTF_0F4 "Tavolozza (0: Generatore)"
#define DFTINTF_0F5 "Levigante"
#define DFTINTF_0F6 "Riempimento"
#define DFTINTF_0F7 "Spessore"
#define DFTINTF_0F9 "Comportamento: 1/2"
#define DFTINTF_0FA "Colore: 1/2"
#define DFTINTF_0FB "Tavolozza (0: nessun colore)"
#define DFTINTF_0FC "Colore (1: casuale, 255: bianco)"
#define DFTINTF_0FD "Nevicate/Stelle/Meteore"
#define DFTINTF_0FE "Numero"
//#define DFTINTF_0FF "Tipo 1/2, Sfocatura 3/4"
#define DFTINTF_100 "Tavolozza (0: colore casuale)"
#define DFTINTF_101 "Colore 1 (0: ciclico)"
#define DFTINTF_102 "Nube"
#define DFTINTF_103 "Sfocatura (0: nessuno)"
#define DFTINTF_104 "Oscillazione"
#define DFTINTF_105 "Filtro"
#define DFTINTF_106 "Fisica"
#define DFTINTF_107 "Cerchi"
#define DFTINTF_108 "Opzione 1/2"
#define DFTINTF_109 "Lampeggia"
#define DFTINTF_10A "Glitch"
#define DFTINTF_10B "Fulmine"
#define DFTINTF_10C "Numero di proiettili"
#define DFTINTF_10D "Taglia (larghezza)"
#define DFTINTF_10E "Compensazione colore (0: casuale)"
#define DFTINTF_10F "Taglia (altezza)"
#define DFTINTF_110 "Classico"
#define DFTINTF_111 "Un colore"
#define DFTINTF_112 "Colori invertiti"
#define DFTINTF_113 "Colore 2 (0: ciclico)"
#define DFTINTF_114 "Qualità/Velocità"
#define DFTINTF_115 "Sfocatura (1-4 con vento)"
#define DFTINTF_116 "Muoversi lungo il seno"
#define DFTINTF_117 "Direzione (0: ciclico)"
#define DFTINTF_118 "Soglia del segnale"
#define DFTINTF_11A "Colore (1 = ciclico)"


/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Effetti";
static const char TINTF_001[] PROGMEM = "Emissione di testo";
static const char TINTF_002[] PROGMEM = "Impostazioni";
static const char TINTF_003[] PROGMEM = "Sulla lista";
static const char TINTF_004[] PROGMEM = "In \"Demo\"";
static const char TINTF_005[] PROGMEM = "Copia";
static const char TINTF_006[] PROGMEM = "Rimuovere";
static const char TINTF_007[] PROGMEM = "Aggiorna la lista";
static const char TINTF_008[] PROGMEM = "Salva";
static const char TINTF_009[] PROGMEM = "Gestione elenco effetti";
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
static const char TINTF_00B[] PROGMEM = "Entrata";
static const char TINTF_00C[] PROGMEM = "Luminosità globale";
static const char TINTF_00D[] PROGMEM = "Luminosita";
static const char TINTF_00E[] PROGMEM = "Accendere";
static const char TINTF_00F[] PROGMEM = "\"Demo\"";
static const char TINTF_010[] PROGMEM = "Luminosità globale";
static const char TINTF_011[] PROGMEM = "Eventi";
static const char TINTF_012[] PROGMEM = "Microfono";
static const char TINTF_013[] PROGMEM = "Pulsante";
static const char TINTF_014[] PROGMEM = "Inoltre...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "Attesa...";
static const char TINTF_018[] PROGMEM = "Configurazioni";
static const char TINTF_019[] PROGMEM = "Scarica";
static const char TINTF_01A[] PROGMEM = "Configurazione";
static const char TINTF_01B[] PROGMEM = "Creare";
static const char TINTF_01C[] PROGMEM = "Mettilo sulla lampada";
static const char TINTF_01D[] PROGMEM = "Testo";
static const char TINTF_01E[] PROGMEM = "Colore del messaggio";
static const char TINTF_01F[] PROGMEM = "Inviare";
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Il microfono è attivo";
static const char TINTF_022[] PROGMEM = "Coefficiente di correzione zero";
static const char TINTF_023[] PROGMEM = "Livello di rumore, unità";
static const char TINTF_024[] PROGMEM = "Soppressione del rumore";
static const char TINTF_025[] PROGMEM = "Calibrazione del microfono";
static const char TINTF_026[] PROGMEM = "Accendi il microfono";
static const char TINTF_027[] PROGMEM = "... in corso ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Cliente (STA)";
static const char TINTF_02A[] PROGMEM = "Impostazioni del cliente WiFi";
static const char TINTF_02B[] PROGMEM = "Il nome della lampada (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "Catena WiFi";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Collegare";
static const char TINTF_02F[] PROGMEM = "Punto di accesso (AP)";
static const char TINTF_030[] PROGMEM = "Riflessione";
static const char TINTF_031[] PROGMEM = "Impostazioni WiFi e punto di accesso";
static const char TINTF_032[] PROGMEM = "In modalità AP, la lampada funziona sempre come punto di accesso e non si connette ad altre reti WiFi";
static const char TINTF_033[] PROGMEM = "Regime WiFi";
static const char TINTF_034[] PROGMEM = "Proteggi l'AP con una password";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "Host MQTT";
static const char TINTF_037[] PROGMEM = "Porta MQTT";
static const char TINTF_038[] PROGMEM = "Utente";
static const char TINTF_039[] PROGMEM = "Intervallo MQTT sec.";
static const char TINTF_03A[] PROGMEM = "Collegare";
static const char TINTF_03B[] PROGMEM = "Specchiatura orizzontale";
static const char TINTF_03C[] PROGMEM = "Specchiatura verticale";
static const char TINTF_03D[] PROGMEM = "Cambio fluido degli effetti";
static const char TINTF_03E[] PROGMEM = "Effetto casuale nella Demo";
static const char TINTF_03F[] PROGMEM = "Modifica dell'effetto nella Demo. (sec)";
static const char TINTF_040[] PROGMEM = "Effetti di ordinamento";
static const char TINTF_041[] PROGMEM = "Copie sotto l'originale";
static const char TINTF_042[] PROGMEM = "Copie alla fine dell'elenco";
static const char TINTF_043[] PROGMEM = "Secondo il numero";
static const char TINTF_044[] PROGMEM = "Velocità di riproduzione del testo";
static const char TINTF_045[] PROGMEM = "Sposta il testo verso l'alto";
static const char TINTF_046[] PROGMEM = "Misto (AP+STA)";
static const char TINTF_047[] PROGMEM = "Non mostrare";
static const char TINTF_048[] PROGMEM = "Ogni ora";
static const char TINTF_049[] PROGMEM = "Ogni mezz'ora";
static const char TINTF_04A[] PROGMEM = "Ogni 15 minuti";
static const char TINTF_04B[] PROGMEM = "Ogni 10 minuti";
static const char TINTF_04C[] PROGMEM = "Ogni 5 minuti";
static const char TINTF_04D[] PROGMEM = "Ogni minuto";
static const char TINTF_04E[] PROGMEM = "Auguri di Capodanno";
static const char TINTF_04F[] PROGMEM = "Periodo di visualizzazione, minuti (0 - non visualizzare)";
static const char TINTF_050[] PROGMEM = "Data/ora di Capodanno nel formato YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Data/Ora/Fuso orario";
static const char TINTF_052[] PROGMEM = "Impostazione del fuso orario. Le regole relative al fuso orario e all'ora stagionale vengono applicate automaticamente, non è richiesta la correzione manuale. Se la tua zona non è elencata, seleziona la zona di offset comune da Greenwich";
static const char TINTF_053[] PROGMEM = "Fuso orario";
static const char TINTF_054[] PROGMEM = "Server NTP di backup (opzionale)";
static const char TINTF_055[] PROGMEM = "Data/ora nel formato YYYY-MM-DDThh:mm:ss (se non c'è Internet, se è vuota, scaricala dal client)";
static const char TINTF_056[] PROGMEM = "Rinnovo";
static const char TINTF_057[] PROGMEM = "Rinnovo ОТА-PIO";
static const char TINTF_058[] PROGMEM = "Inizio";
static const char TINTF_059[] PROGMEM = "Завантаження прошивки";
static const char TINTF_05A[] PROGMEM = "Сaricamento";
static const char TINTF_05B[] PROGMEM = "Evento";
static const char TINTF_05C[] PROGMEM = "Modificare";
static const char TINTF_05D[] PROGMEM = "Aggiungere";
static const char TINTF_05E[] PROGMEM = "Attivamente";
static const char TINTF_05F[] PROGMEM = "Tipo di evento";
static const char TINTF_060[] PROGMEM = "Accendi la lampada";
static const char TINTF_061[] PROGMEM = "Spegni la lampada";
static const char TINTF_062[] PROGMEM = "Modalità \"demo\"";
static const char TINTF_063[] PROGMEM = "Sveglia";
static const char TINTF_064[] PROGMEM = "Carica le impostazioni della lampada";
static const char TINTF_065[] PROGMEM = "Scarica le impostazioni degli effetti";
static const char TINTF_066[] PROGMEM = "Carica le impostazioni dell'evento";
static const char TINTF_067[] PROGMEM = "Visualizza testo";
static const char TINTF_068[] PROGMEM = "Tempo di visualizzazione";
static const char TINTF_069[] PROGMEM = "Lo stato della schiuma";
static const char TINTF_06A[] PROGMEM = "Accendere AUX";
static const char TINTF_06B[] PROGMEM = "Spegnere AUX";
static const char TINTF_06C[] PROGMEM = "Cambia stato AUX";
static const char TINTF_06D[] PROGMEM = "Data/ora dell'evento";
static const char TINTF_06E[] PROGMEM = "Ripeti, min";
static const char TINTF_06F[] PROGMEM = "Fermati dopo, min";
static const char TINTF_070[] PROGMEM = "Parametro (testo)";
static const char TINTF_071[] PROGMEM = "Ripetere";
static const char TINTF_072[] PROGMEM = "Lunedi";
static const char TINTF_073[] PROGMEM = "Martedì";
static const char TINTF_074[] PROGMEM = "Mercoledì";
static const char TINTF_075[] PROGMEM = "Giovedì";
static const char TINTF_076[] PROGMEM = "Venerdì";
static const char TINTF_077[] PROGMEM = "Sabato";
static const char TINTF_078[] PROGMEM = "Domenica";
static const char TINTF_079[] PROGMEM = "Aggiornare";
static const char TINTF_07A[] PROGMEM = "Azione";
static const char TINTF_07B[] PROGMEM = "Il pulsante è attivo";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Manutenzione";
static const char TINTF_07E[] PROGMEM = "Premere";
static const char TINTF_07F[] PROGMEM = "Una volta";
static const char TINTF_080[] PROGMEM = "Lampada da fuoco";
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Altri";
static const char TINTF_083[] PROGMEM = "Cancella la lampada quando si cambiano gli effetti";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "In ordine alfabetico (interno)";
static const char TINTF_086[] PROGMEM = "Gli eventi sono attivi";
static const char TINTF_087[] PROGMEM = "Velocità";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
static const char TINTF_089[] PROGMEM = "Il nome dell'effetto";
static const char TINTF_08A[] PROGMEM = "In ordine alfabetico (configurazione)";
static const char TINTF_08B[] PROGMEM = "Ripristina le impostazioni dell'effetto";
static const char TINTF_08C[] PROGMEM = "Prefisso argomento";
static const char TINTF_08D[] PROGMEM = "Ordina per microfono";
static const char TINTF_08E[] PROGMEM = "Debug";
static const char TINTF_08F[] PROGMEM = "Impostazioni ESP";
static const char TINTF_090[] PROGMEM = "Numerazione";
static const char TINTF_091[] PROGMEM = "Microfono";
static const char TINTF_092[] PROGMEM = "Impostazioni dei pin";
static const char TINTF_093[] PROGMEM = "Mostra impostazioni ESP";
static const char TINTF_094[] PROGMEM = "Pulsante Pin";
static const char TINTF_095[] PROGMEM = "Limitazione di corrente (mA)";
static const char TINTF_096[] PROGMEM = "Riavvio";
static const char TINTF_097[] PROGMEM = "Pin RX DFPlayer";
static const char TINTF_098[] PROGMEM = "Pin TX DFPlayer";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Il nome dell'effetto nella Demo";
static const char TINTF_09B[] PROGMEM = "Volume";
static const char TINTF_09C[] PROGMEM = "Ora";
static const char TINTF_09D[] PROGMEM = "Il nome dell'effetto";
static const char TINTF_09E[] PROGMEM = "Secondo l'effetto";
static const char TINTF_09F[] PROGMEM = "Disconnesso";
static const char TINTF_0A0[] PROGMEM = "Primo";
static const char TINTF_0A1[] PROGMEM = "A caso";
static const char TINTF_0A2[] PROGMEM = "A caso MP3";
static const char TINTF_0A3[] PROGMEM = "Suono della sveglia";
static const char TINTF_0A4[] PROGMEM = "Secondo";
static const char TINTF_0A5[] PROGMEM = "Terzo";
static const char TINTF_0A6[] PROGMEM = "Quarto";
static const char TINTF_0A7[] PROGMEM = "Quinto";
static const char TINTF_0A8[] PROGMEM = "Equalizzatore";
static const char TINTF_0A9[] PROGMEM = "Normale";
static const char TINTF_0AA[] PROGMEM = "Pop";
static const char TINTF_0AB[] PROGMEM = "Rok";
static const char TINTF_0AC[] PROGMEM = "Jazz";
static const char TINTF_0AD[] PROGMEM = "Classico";
static const char TINTF_0AE[] PROGMEM = "Basso";
static const char TINTF_0AF[] PROGMEM = "Lettore MP3";
static const char TINTF_0B0[] PROGMEM = "Il numero di file nella cartella MP3";
static const char TINTF_0B1[] PROGMEM = "Modalità audio";
static const char TINTF_0B2[] PROGMEM = "Cartella\\numero file audio (esempi: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Limita il volume della sveglia";
static const char TINTF_0B4[] PROGMEM = "Predefinito";
static const char TINTF_0B5[] PROGMEM = "Rimuovere dalla lista";
static const char TINTF_0B6[] PROGMEM = "Disconnesso";
static const char TINTF_0B7[] PROGMEM = "Versione 1";
static const char TINTF_0B8[] PROGMEM = "Versione 2";
static const char TINTF_0B9[] PROGMEM = "Parametro";
static const char TINTF_0BA[] PROGMEM = "Sveglia \"Alba\"";
static const char TINTF_0BB[] PROGMEM = "Durata dell'alba";
static const char TINTF_0BC[] PROGMEM = "Il bagliore dopo l'alba";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "secondi";
static const char TINTF_0C2[] PROGMEM = "minuti";
static const char TINTF_0C3[] PROGMEM = "ore";
static const char TINTF_0C4[] PROGMEM = "giorni";
static const char TINTF_0C5[] PROGMEM = "giorni";
static const char TINTF_0C6[] PROGMEM = "giorno";
static const char TINTF_0C7[] PROGMEM = "ore";
static const char TINTF_0C8[] PROGMEM = "ora";
static const char TINTF_0C9[] PROGMEM = "Випадк.";
static const char TINTF_0CA[] PROGMEM = "Oscuramento dello sfondo";
static const char TINTF_0CB[] PROGMEM = "Avvertimento";
static const char TINTF_0CC[] PROGMEM = "minuti";
static const char TINTF_0CD[] PROGMEM = "minuto";
static const char TINTF_0CE[] PROGMEM = "La pittura";
static const char TINTF_0CF[] PROGMEM = "Versare";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Suona prima";
static const char TINTF_0D2[] PROGMEM = "Limita il volume";
static const char TINTF_0D3[] PROGMEM = "Cof. velocità";
static const char TINTF_0D4[] PROGMEM = "Impostazioni di visualizzazione";
static const char TINTF_0D5[] PROGMEM = "Luminosità (0 - spenta)";
static const char TINTF_0D6[] PROGMEM = "Luminosità spenta lampada (0 - spenta)";
static const char TINTF_0D7[] PROGMEM = "Formato 24 ore";
static const char TINTF_0D8[] PROGMEM = "Ospite 0";
static const char TINTF_0D9[] PROGMEM = "Pulire";
static const char TINTF_0DA[] PROGMEM = "Scansione";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Gestione";
static const char TINTF_0DD[] PROGMEM = "Tipo di scala";
static const char TINTF_0DE[] PROGMEM = "Colore della scala";
static const char TINTF_0DF[] PROGMEM = "Colore del testo";
static const char TINTF_0E0[] PROGMEM = "Mostra la temperatura";
static const char TINTF_0E1[] PROGMEM = "Sicuri?";
static const char TINTF_0E2[] PROGMEM = "Trasmissione";
static const char TINTF_0E3[] PROGMEM = "Regime";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Blocco.Effetto";
static const char TINTF_0E7[] PROGMEM = "Marcatura";
static const char TINTF_0E8[] PROGMEM = "Iniziale Universe";
static const char TINTF_0E9[] PROGMEM = "Завантажити конфіг кнопок";
static const char TINTF_0EA[] PROGMEM = "Luce notturna";
static const char TINTF_0EB[] PROGMEM = "Lampada";
static const char TINTF_0EC[] PROGMEM = "In ogni caso";
static const char TINTF_0ED[] PROGMEM = "È ora di parlare";
static const char TINTF_0EE[] PROGMEM = "Spento";
static const char TINTF_0EF[] PROGMEM = "Verticale";
static const char TINTF_0F0[] PROGMEM = "Orizzontale";
static const char TINTF_0F1[] PROGMEM = "Modalità giocatore";
static const char TINTF_0F2[] PROGMEM = "Lettore MP3 + ripristino";
static const char TINTF_0F3[] PROGMEM = "Elenco";
static const char TINTF_0F4[] PROGMEM = "Ripristino delle impostazioni della lampada";
static const char TINTF_0F5[] PROGMEM = "L'operazione è stata annullata";
static const char TINTF_0F6[] PROGMEM = "Controllo degli effetti";
static const char TINTF_0F7[] PROGMEM = "Puoi trovare il tutorial sulla configurazione della matrice Jinx";
static const char TINTF_0F8[] PROGMEM = "qui";

static const char NY_MDG_STRING1[] PROGMEM = "Fino a quando il nuovo anno non sarà lasciato %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "Felice anno nuovo %d!";
//-------------------------

#endif