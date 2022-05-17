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

/** набор строк с именами Effectов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
static const char TEFF_000[] PROGMEM = "";  // "пустышка"
static const char TEFF_001[] PROGMEM = "White lamp";
static const char TEFF_002[] PROGMEM = "Colors";
static const char TEFF_003[] PROGMEM = "Rainbow 2D";
static const char TEFF_004[] PROGMEM = "Confetti";
static const char TEFF_005[] PROGMEM = "M&M`s";
static const char TEFF_006[] PROGMEM = "Blizzard, Meteor Shower, Meteors";
static const char TEFF_007[] PROGMEM = "New Matrix";
static const char TEFF_008[] PROGMEM = "Aquarelle";
static const char TEFF_009[] PROGMEM = "Fireflies with a trace";
static const char TEFF_010[] PROGMEM = "Wandering cube";
static const char TEFF_011[] PROGMEM = "Pulse";
static const char TEFF_012[] PROGMEM = "Effectfall";
static const char TEFF_013[] PROGMEM = "Burning bush"; 
static const char TEFF_014[] PROGMEM = "Paintball";
static const char TEFF_015[] PROGMEM = "Noise 3D effects";
static const char TEFF_016[] PROGMEM = "Cells and other";
static const char TEFF_017[] PROGMEM = "Puzzles";
static const char TEFF_018[] PROGMEM = "Tixy.Land";
static const char TEFF_019[] PROGMEM = "N.Tesla";
static const char TEFF_020[] PROGMEM = "Oscillator";
static const char TEFF_021[] PROGMEM = "Storm";
static const char TEFF_022[] PROGMEM = "Fairy";
static const char TEFF_023[] PROGMEM = "Fount";
static const char TEFF_024[] PROGMEM = "Bouncing balls";
static const char TEFF_025[] PROGMEM = "Sinusoid";
static const char TEFF_026[] PROGMEM = "Metasphere";
static const char TEFF_027[] PROGMEM = "Spiral";
static const char TEFF_028[] PROGMEM = "Comets, Soaring Fires +";
static const char TEFF_029[] PROGMEM = "Bulbulator";
static const char TEFF_030[] PROGMEM = "Prismata";
static const char TEFF_031[] PROGMEM = "Flock and Predator";
static const char TEFF_032[] PROGMEM = "Whirlpool";
static const char TEFF_033[] PROGMEM = "Drift";
static const char TEFF_034[] PROGMEM = "Popcorn";
static const char TEFF_035[] PROGMEM = "Flicker";
static const char TEFF_036[] PROGMEM = "Radar";
static const char TEFF_037[] PROGMEM = "Waves";
static const char TEFF_038[] PROGMEM = "Fire 2012";
static const char TEFF_039[] PROGMEM = "Bengal Lights";
static const char TEFF_040[] PROGMEM = "Balls";
static const char TEFF_041[] PROGMEM = "DNA";
static const char TEFF_042[] PROGMEM = "Fire 2018";
static const char TEFF_043[] PROGMEM = "Combination lock";
static const char TEFF_044[] PROGMEM = "Cube 2D";
static const char TEFF_045[] PROGMEM = "Magma";
static const char TEFF_046[] PROGMEM = "Picasso";
static const char TEFF_047[] PROGMEM = "Starship Troopers";
static const char TEFF_048[] PROGMEM = "Flags";
static const char TEFF_049[] PROGMEM = "Jumpers";
static const char TEFF_050[] PROGMEM = "Vortexes";
static const char TEFF_051[] PROGMEM = "Stars";
static const char TEFF_052[] PROGMEM = "Glare on Water, Colors";
static const char TEFF_053[] PROGMEM = "Fireworks";
static const char TEFF_054[] PROGMEM = "Pacific";
static const char TEFF_055[] PROGMEM = "Munch";
static const char TEFF_056[] PROGMEM = "Color noise";
static const char TEFF_057[] PROGMEM = "Moths";
static const char TEFF_058[] PROGMEM = "Shadows";
static const char TEFF_059[] PROGMEM = "Patterns";
static const char TEFF_060[] PROGMEM = "Arrows";
static const char TEFF_061[] PROGMEM = "Wild balls";
static const char TEFF_062[] PROGMEM = "Attract";
static const char TEFF_063[] PROGMEM = "Snakes";
static const char TEFF_064[] PROGMEM = "Nexus";
static const char TEFF_065[] PROGMEM = "Maze";
static const char TEFF_066[] PROGMEM = "Curly Colors";
static const char TEFF_067[] PROGMEM = "Polar Lights";
static const char TEFF_068[] PROGMEM = "Flowering";
static const char TEFF_069[] PROGMEM = "Snakes Island";
static const char TEFF_070[] PROGMEM = "Children's Dreams";
static const char TEFF_071[] PROGMEM = "Metasphares";
static const char TEFF_072[] PROGMEM = "Lava lamp";
static const char TEFF_073[] PROGMEM = "Space Racer";
static const char TEFF_074[] PROGMEM = "Smoke" ;
static const char TEFF_075[] PROGMEM = "Fire Lamp";
static const char TEFF_076[] PROGMEM = "Mirage";
static const char TEFF_250[] PROGMEM = "Clock";
static const char TEFF_251[] PROGMEM = "RGB 332/565 Player";
static const char TEFF_254[] PROGMEM = "Frequency Analyzer";
static const char TEFF_255[] PROGMEM = "Oscilloscope";

// -------------------------
#define DFTINTF_00A "Effect"
#define DFTINTF_020 "Microphone"
#define DFTINTF_084 "Palette"
#define DFTINTF_088 "Scale"
#define DFTINTF_0D0 "Color"
#define DFTINTF_0D1 "Palette/Scale"
#define DFTINTF_0D2 "Effect (0: cyclical)"
#define DFTINTF_0D3 "Color offset (0: cyclical)"
#define DFTINTF_0D4 "Subpixel"
#define DFTINTF_0D5 "Quantity"
#define DFTINTF_0D6 "Color generator"
#define DFTINTF_0D7 "Tint"
#define DFTINTF_0D8 "Foolery!"
#define DFTINTF_0D9 "Quantity/Color (night lamp)"
#define DFTINTF_0DA "Saturation"
#define DFTINTF_0DB "Gain"
#define DFTINTF_0DC "Sweep"
#define DFTINTF_0DD "Ripple"
#define DFTINTF_0DE "Blur"
#define DFTINTF_0DF "Flutter"
#define DFTINTF_0E0 "Night lamp"
#define DFTINTF_0E1 "Rings"
#define DFTINTF_0E2 "Glare (0: Off)"
#define DFTINTF_0E3 "Mode"
#define DFTINTF_0E4 "Linear Velocity"
#define DFTINTF_0E5 "Mode (1: random)"
#define DFTINTF_0E6 "Offset"
#define DFTINTF_0E7 "Quantity/Direction/Position"
#define DFTINTF_0E8 "Predator"
#define DFTINTF_0E9 "Mass"
#define DFTINTF_0EA "Speed ​​(Vertical)"
#define DFTINTF_0EB "Speed ​​(Horizontal)"
#define DFTINTF_0EC "Pattern (0: random)"
#define DFTINTF_0ED "Shading"
#define DFTINTF_0EE "Energy"
#define DFTINTF_0EF "Density"
#define DFTINTF_0F0 "Color/Rotation"
#define DFTINTF_0F1 "Size/Track (1-85-170-255)"
#define DFTINTF_0F2 "Start frequency"
#define DFTINTF_0F3 "Rotate"
#define DFTINTF_0F4 "Palette (0: generator)"
#define DFTINTF_0F5 "Smoothing"
#define DFTINTF_0F6 "Fill"
#define DFTINTF_0F7 "Thickness"
#define DFTINTF_0F9 "Behavior: 1/2"
#define DFTINTF_0FA "Color: 1/2"
#define DFTINTF_0FB "Palette (0: no color)"
#define DFTINTF_0FC "Color (1: random, 255: white)"
#define DFTINTF_0FD "Snowfall/Starfall/Meteors"
#define DFTINTF_0FE "Quantity"
#define DFTINTF_0FF "Type 1/2, Blur 3/4"
#define DFTINTF_100 "Palette (0: random color)"
#define DFTINTF_101 "Color 1 (0: cyclical)"
#define DFTINTF_102 "Cloud"
#define DFTINTF_103 "Blurring (0: no)"
#define DFTINTF_104 "Swipe"
#define DFTINTF_105 "Filter"
#define DFTINTF_106 "Physics"
#define DFTINTF_107 "Circles"
#define DFTINTF_108 "Option 1/2"
#define DFTINTF_109 "Flashes"
#define DFTINTF_10A "Glitch"
#define DFTINTF_10B "Storm"
#define DFTINTF_10C "Quantity of shells"
#define DFTINTF_10D "Size (width)"
#define DFTINTF_10E "Color shift (0: random)"
#define DFTINTF_10F "Size (height)"
#define DFTINTF_110 "Classic"
#define DFTINTF_111 "One color"
#define DFTINTF_112 "Inverse Colors"
#define DFTINTF_113 "Color 2 (0: cyclical)"
#define DFTINTF_114 "Quality/Speed"
#define DFTINTF_115 "Blur (1-4 with wind)"
#define DFTINTF_116 "Sin function moving"
#define DFTINTF_117 "Direction (0: cyclical)"
#define DFTINTF_118 "Signal threshold"
#define DFTINTF_11A "Color (1: cyclical)"

/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Effects";
static const char TINTF_001[] PROGMEM = "Text";
static const char TINTF_002[] PROGMEM = "Settings";
static const char TINTF_003[] PROGMEM = "In selection list";
static const char TINTF_004[] PROGMEM = "In Demo list";
static const char TINTF_005[] PROGMEM = "Copy effect";
static const char TINTF_006[] PROGMEM = "Remove";
static const char TINTF_007[] PROGMEM = "Refresh effects-list";
static const char TINTF_008[] PROGMEM = "Save";
static const char TINTF_009[] PROGMEM = "Managing effects-list";
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
static const char TINTF_00B[] PROGMEM = "Exit";
static const char TINTF_00C[] PROGMEM = "Global Brightness";
static const char TINTF_00D[] PROGMEM = "Brightness";
static const char TINTF_00E[] PROGMEM = "OFF/ON";
static const char TINTF_00F[] PROGMEM = "Demo";
static const char TINTF_010[] PROGMEM = "Gl. Bright";
static const char TINTF_011[] PROGMEM = "Events";
static const char TINTF_012[] PROGMEM = "Mic";
static const char TINTF_013[] PROGMEM = "Button";
static const char TINTF_014[] PROGMEM = "More...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "Waiting...";
static const char TINTF_018[] PROGMEM = "Configs";
static const char TINTF_019[] PROGMEM = "Download";
static const char TINTF_01A[] PROGMEM = "Config";
static const char TINTF_01B[] PROGMEM = "Create";
static const char TINTF_01C[] PROGMEM = "Show on lamp";
static const char TINTF_01D[] PROGMEM = "Text";
static const char TINTF_01E[] PROGMEM = "Text color";
static const char TINTF_01F[] PROGMEM = "Send";
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Microphone On";
static const char TINTF_022[] PROGMEM = "Zero correction";
static const char TINTF_023[] PROGMEM = "Noise level, units";
static const char TINTF_024[] PROGMEM = "Denoising";
static const char TINTF_025[] PROGMEM = "Microphone calibration";
static const char TINTF_026[] PROGMEM = "Turn microphone On";
static const char TINTF_027[] PROGMEM = "... in progress ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Client (STA)";
static const char TINTF_02A[] PROGMEM = "WiFi client settings";
static const char TINTF_02B[] PROGMEM = "Lamp name (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "WiFi SSID";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Connect";
static const char TINTF_02F[] PROGMEM = "Access poin (AP)";
static const char TINTF_030[] PROGMEM = "Display";
static const char TINTF_031[] PROGMEM = "WiFi settings";
static const char TINTF_032[] PROGMEM = "In AP mode, the lamp always works as an Access Point and will not connect to other WiFi networks.";
static const char TINTF_033[] PROGMEM = "WiFi mode";
static const char TINTF_034[] PROGMEM = "Protect AP with a password";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "MQTT host";
static const char TINTF_037[] PROGMEM = "MQTT port";
static const char TINTF_038[] PROGMEM = "User";
static const char TINTF_039[] PROGMEM = "Interval mqtt sec.";
static const char TINTF_03A[] PROGMEM = "Connect";
static const char TINTF_03B[] PROGMEM = "Mirroring H";
static const char TINTF_03C[] PROGMEM = "Mirroring V";
static const char TINTF_03D[] PROGMEM = "Smooth effects switching";
static const char TINTF_03E[] PROGMEM = "Randomize effect in Demo";
static const char TINTF_03F[] PROGMEM = "Switch effect in Demo. (sec)";
static const char TINTF_040[] PROGMEM = "Sorting effects";
static const char TINTF_041[] PROGMEM = "Copies under original";
static const char TINTF_042[] PROGMEM = "Copies at the end of the list";
static const char TINTF_043[] PROGMEM = "Sort by index";
static const char TINTF_044[] PROGMEM = "Text scrolling speed";
static const char TINTF_045[] PROGMEM = "Offset of text output";
static const char TINTF_046[] PROGMEM = "Mixed (AP+STA)";
static const char TINTF_047[] PROGMEM = "Don`t show";
static const char TINTF_048[] PROGMEM = "Every hour";
static const char TINTF_049[] PROGMEM = "Every half hour";
static const char TINTF_04A[] PROGMEM = "Every 15 minutes";
static const char TINTF_04B[] PROGMEM = "Every 10 minutes";
static const char TINTF_04C[] PROGMEM = "Every 5 minutes";
static const char TINTF_04D[] PROGMEM = "Every minute";
static const char TINTF_04E[] PROGMEM = "New Year greeting";
static const char TINTF_04F[] PROGMEM = "Show period, minutes (0 - do not output)";
static const char TINTF_050[] PROGMEM = "New Year's Date/Time at format YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Date / Time / Time Zone";
static const char TINTF_052[] PROGMEM = "TimeZone shift/daylight saving rules applied automatically, no need to adjust manually.";
static const char TINTF_053[] PROGMEM = "Time zone";
static const char TINTF_054[] PROGMEM = "Spare NTP server (optional)";
static const char TINTF_055[] PROGMEM = "Date/Time, YYYY-MM-DDThh:mm:ss (without internet connection, if empty - from device)";
static const char TINTF_056[] PROGMEM = "Firmware update";
static const char TINTF_057[] PROGMEM = "OTA-PIO firmware update";
static const char TINTF_058[] PROGMEM = "Start";
static const char TINTF_059[] PROGMEM = "Downloading firmware";
static const char TINTF_05A[] PROGMEM = "Upload";
static const char TINTF_05B[] PROGMEM = "Event";
static const char TINTF_05C[] PROGMEM = "Edit";
static const char TINTF_05D[] PROGMEM = "Add";
static const char TINTF_05E[] PROGMEM = "Actively";
static const char TINTF_05F[] PROGMEM = "Event type";
static const char TINTF_060[] PROGMEM = "Lamp On";
static const char TINTF_061[] PROGMEM = "Lamp Off";
static const char TINTF_062[] PROGMEM = "Mode DEMO";
static const char TINTF_063[] PROGMEM = "Alarm";
static const char TINTF_064[] PROGMEM = "Upload lamp configuration";
static const char TINTF_065[] PROGMEM = "Loading effects configurations";
static const char TINTF_066[] PROGMEM = "Loading events configurations";
static const char TINTF_067[] PROGMEM = "Show the Text";
static const char TINTF_068[] PROGMEM = "Show the Time";
static const char TINTF_069[] PROGMEM = "State of Pin";
static const char TINTF_06A[] PROGMEM = "Turn AUX On";
static const char TINTF_06B[] PROGMEM = "Turn AUX Off";
static const char TINTF_06C[] PROGMEM = "Toggle AUX";
static const char TINTF_06D[] PROGMEM = "Event Date/Time";
static const char TINTF_06E[] PROGMEM = "Repeat, min";
static const char TINTF_06F[] PROGMEM = "Stop in, min";
static const char TINTF_070[] PROGMEM = "Parameter (text)";
static const char TINTF_071[] PROGMEM = "Replay";
static const char TINTF_072[] PROGMEM = "Monday";
static const char TINTF_073[] PROGMEM = "Tuesday";
static const char TINTF_074[] PROGMEM = "Wednesday";
static const char TINTF_075[] PROGMEM = "Thursday";
static const char TINTF_076[] PROGMEM = "Friday";
static const char TINTF_077[] PROGMEM = "Saturday";
static const char TINTF_078[] PROGMEM = "Sunday";
static const char TINTF_079[] PROGMEM = "Update";
static const char TINTF_07A[] PROGMEM = "Action";
static const char TINTF_07B[] PROGMEM = "Button activated";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Hold";
static const char TINTF_07E[] PROGMEM = "Press";
static const char TINTF_07F[] PROGMEM = "Singly";
static const char TINTF_080[] PROGMEM = "Fire Lamp";
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Other";
static const char TINTF_083[] PROGMEM = "Clean the lamp when switching effects";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "Sort by Alphabet (internal)";
static const char TINTF_086[] PROGMEM = "Events activated";
static const char TINTF_087[] PROGMEM = "Speed";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
static const char TINTF_089[] PROGMEM = "Effect`s name";
static const char TINTF_08A[] PROGMEM = "Sort by Alphabet (config)";
static const char TINTF_08B[] PROGMEM = "Reset effect settings";
static const char TINTF_08C[] PROGMEM = "Topic prefix";
static const char TINTF_08D[] PROGMEM = "Sort by microphone";
static const char TINTF_08E[] PROGMEM = "Debug";
static const char TINTF_08F[] PROGMEM = "ESP settings";
static const char TINTF_090[] PROGMEM = "Effect list numbering";
static const char TINTF_091[] PROGMEM = "Mic. symbol in effect list";
static const char TINTF_092[] PROGMEM = "Setting up pins";
static const char TINTF_093[] PROGMEM = "Show the system menu";
static const char TINTF_094[] PROGMEM = "Button pin";
static const char TINTF_095[] PROGMEM = "Current limit (mA)";
static const char TINTF_096[] PROGMEM = "Reboot";
static const char TINTF_097[] PROGMEM = "Pin of the RX player";
static const char TINTF_098[] PROGMEM = "Pin of the TX player";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Show effect name in Demo";
static const char TINTF_09B[] PROGMEM = "Volume";
static const char TINTF_09C[] PROGMEM = "Time";
static const char TINTF_09D[] PROGMEM = "Effect name";
static const char TINTF_09E[] PROGMEM = "Effect (reset)";
static const char TINTF_09F[] PROGMEM = "Disabled";
static const char TINTF_0A0[] PROGMEM = "First";
static const char TINTF_0A1[] PROGMEM = "Random";
static const char TINTF_0A2[] PROGMEM = "Random MP3";
static const char TINTF_0A3[] PROGMEM = "The sound of the alarm clock";
static const char TINTF_0A4[] PROGMEM = "Second";
static const char TINTF_0A5[] PROGMEM = "Third";
static const char TINTF_0A6[] PROGMEM = "Fourth";
static const char TINTF_0A7[] PROGMEM = "Fifth";
static const char TINTF_0A8[] PROGMEM = "Equalizer";
static const char TINTF_0A9[] PROGMEM = "Normal";
static const char TINTF_0AA[] PROGMEM = "Pop";
static const char TINTF_0AB[] PROGMEM = "Rock";
static const char TINTF_0AC[] PROGMEM = "Jazz";
static const char TINTF_0AD[] PROGMEM = "Classic";
static const char TINTF_0AE[] PROGMEM = "Bass";
static const char TINTF_0AF[] PROGMEM = "Mp3 player";
static const char TINTF_0B0[] PROGMEM = "Number of files in MP3 folder";
static const char TINTF_0B1[] PROGMEM = "Voice-over modes";
static const char TINTF_0B2[] PROGMEM = "Folder\\the number of the sound file (examples: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Limit Alarm volume";
static const char TINTF_0B4[] PROGMEM = "Default";
static const char TINTF_0B5[] PROGMEM = "Remove from the list";
static const char TINTF_0B6[] PROGMEM = "Disabled";
static const char TINTF_0B7[] PROGMEM = "Option 1";
static const char TINTF_0B8[] PROGMEM = "Option 2";
static const char TINTF_0B9[] PROGMEM = "Parameter";
static const char TINTF_0BA[] PROGMEM = "Alarm Clock Dawn";
static const char TINTF_0BB[] PROGMEM = "Dawn period";
static const char TINTF_0BC[] PROGMEM = "After down period";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "seconds";
static const char TINTF_0C2[] PROGMEM = "minutes";
static const char TINTF_0C3[] PROGMEM = "hours";
static const char TINTF_0C4[] PROGMEM = "days";
static const char TINTF_0C5[] PROGMEM = "days";
static const char TINTF_0C6[] PROGMEM = "day";
static const char TINTF_0C7[] PROGMEM = "hours";
static const char TINTF_0C8[] PROGMEM = "hour";
static const char TINTF_0C9[] PROGMEM = "Rand.";
static const char TINTF_0CA[] PROGMEM = "Darkening the background";
static const char TINTF_0CB[] PROGMEM = "Warning";
static const char TINTF_0CC[] PROGMEM = "minutes";
static const char TINTF_0CD[] PROGMEM = "minute";
static const char TINTF_0CE[] PROGMEM = "Drawing";
static const char TINTF_0CF[] PROGMEM = "Fill";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Sound from begin";
static const char TINTF_0D2[] PROGMEM = "Limit volume";
static const char TINTF_0D3[] PROGMEM = "Speed factor (1.0 as default)";
static const char TINTF_0D4[] PROGMEM = "Display settings";
static const char TINTF_0D5[] PROGMEM = "Brightness (0 - OFF)";
static const char TINTF_0D6[] PROGMEM = "Brightness with OFF lamp (0 - OFF)";
static const char TINTF_0D7[] PROGMEM = "24 hour format";
static const char TINTF_0D8[] PROGMEM = "Leading 0";
static const char TINTF_0D9[] PROGMEM = "Clear";
static const char TINTF_0DA[] PROGMEM = "Scan";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Control";
static const char TINTF_0DD[] PROGMEM = "Gauge type";
static const char TINTF_0DE[] PROGMEM = "Gauge color";
static const char TINTF_0DF[] PROGMEM = "Text color";
static const char TINTF_0E0[] PROGMEM = "Show temperature";
static const char TINTF_0E1[] PROGMEM = "Sure?";
static const char TINTF_0E2[] PROGMEM = "Streaming";
static const char TINTF_0E3[] PROGMEM = "Mode";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Block Effect";
static const char TINTF_0E7[] PROGMEM = "Mapping";
static const char TINTF_0E8[] PROGMEM = "Starting Universe";
static const char TINTF_0E9[] PROGMEM = "Loading buttons configurations";
static const char TINTF_0EA[] PROGMEM = "Night light";
static const char TINTF_0EB[] PROGMEM = "Bright lamp";
static const char TINTF_0EC[] PROGMEM = "Anyways";
static const char TINTF_0ED[] PROGMEM = "Tell the time";
static const char TINTF_0EE[] PROGMEM = "Disabled";
static const char TINTF_0EF[] PROGMEM = "Vertical";
static const char TINTF_0F0[] PROGMEM = "Horizontal";

static const char NY_MDG_STRING1[] PROGMEM = "Until the new year is left %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "Happy new %d year!";
//-------------------------

#endif
