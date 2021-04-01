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

  (Этот файл — часть FireLamp_JeeUI.

   FireLamp_JeeUI - свободная программа: вы можете перераспространять ее и/или
   изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
   в каком она была опубликована Фондом свободного программного обеспечения;
   либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
   версии.

   FireLamp_JeeUI распространяется в надежде, что она будет полезной,
   но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
   или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
   общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU
   вместе с этой программой. Если это не так, см.
   <https://www.gnu.org/licenses/>.)
*/

#ifndef __EFFECTS_TYPES_H
#define __EFFECTS_TYPES_H

#include "config.h"
#include LANG                //"text_res.h"


/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

static const char PGidxtemplate[] PROGMEM  = "%s{\"n\":%d,\"f\":%d}";

/** набор служебных текстовых констант (не для локализации)
 */
static const char TCONST_0000[] PROGMEM = "effects";
static const char TCONST_0001[] PROGMEM = "pTime";
static const char TCONST_0002[] PROGMEM = "pMem";
static const char TCONST_0003[] PROGMEM = "lamptext";
static const char TCONST_0004[] PROGMEM = "settings";
static const char TCONST_0005[] PROGMEM = "set_effect";
static const char TCONST_0006[] PROGMEM = "eff_sel";
static const char TCONST_0007[] PROGMEM = "eff_fav";
static const char TCONST_0008[] PROGMEM = "gray";
static const char TCONST_0009[] PROGMEM = "copy";
static const char TCONST_000A[] PROGMEM = "del*";
static const char TCONST_000B[] PROGMEM = "makeidx";
static const char TCONST_000C[] PROGMEM = "red";
static const char TCONST_000D[] PROGMEM = "black";
static const char TCONST_000E[] PROGMEM = "AUX";
static const char TCONST_000F[] PROGMEM = "effects_config";
static const char TCONST_0010[] PROGMEM = "effListConf";
static const char TCONST_0011[] PROGMEM = "effects_param";
static const char TCONST_0012[] PROGMEM = "bright";
static const char TCONST_0013[] PROGMEM = "speed";
static const char TCONST_0014[] PROGMEM = "scale";
static const char TCONST_0015[] PROGMEM = "dynCtrl";
static const char TCONST_0016[] PROGMEM = "effListMain";
static const char TCONST_0017[] PROGMEM = "nofade";
static const char TCONST_0018[] PROGMEM = "GlobBRI";
static const char TCONST_0019[] PROGMEM = "flags";
static const char TCONST_001A[] PROGMEM = "ONflag";
static const char TCONST_001B[] PROGMEM = "Demo";
static const char TCONST_001C[] PROGMEM = "GBR";
static const char TCONST_001D[] PROGMEM = "Events";
static const char TCONST_001E[] PROGMEM = "Mic";
static const char TCONST_001F[] PROGMEM = "Btn";
static const char TCONST_0020[] PROGMEM = "show_flags";
static const char TCONST_0021[] PROGMEM = "mode";
static const char TCONST_0022[] PROGMEM = "eff_prev";
static const char TCONST_0023[] PROGMEM = "eff_next";
static const char TCONST_0024[] PROGMEM = "#708090";
static const char TCONST_0025[] PROGMEM = "#5f9ea0";
static const char TCONST_0026[] PROGMEM = "DTimer";
static const char TCONST_0027[] PROGMEM = "OTA";
static const char TCONST_0028[] PROGMEM = "lamp_config";
static const char TCONST_0029[] PROGMEM = "edit_lamp_config";
static const char TCONST_002A[] PROGMEM = "fileName";
static const char TCONST_002B[] PROGMEM = "/backup/idx";
static const char TCONST_002C[] PROGMEM = "/backup/idx/";
static const char TCONST_002D[] PROGMEM = "load";
static const char TCONST_002E[] PROGMEM = "save";
static const char TCONST_002F[] PROGMEM = "green";
static const char TCONST_0030[] PROGMEM = "add_lamp_config";
static const char TCONST_0031[] PROGMEM = "/backup/glb/";
static const char TCONST_0032[] PROGMEM = "/backup/evn/";
static const char TCONST_0033[] PROGMEM = "/backup/btn/";
static const char TCONST_0034[] PROGMEM = "textsend";
static const char TCONST_0035[] PROGMEM = "msg";
static const char TCONST_0036[] PROGMEM = "txtColor";
static const char TCONST_0037[] PROGMEM = "settings_mic";
static const char TCONST_0038[] PROGMEM = "set_mic";
static const char TCONST_0039[] PROGMEM = "micScale";
static const char TCONST_003A[] PROGMEM = "micNoise";
static const char TCONST_003B[] PROGMEM = "micnRdcLvl";
static const char TCONST_003C[] PROGMEM = "mic_cal";
static const char TCONST_003D[] PROGMEM = "settings_wifi";
static const char TCONST_003E[] PROGMEM = "set_wifi";
static const char TCONST_003F[] PROGMEM = "hostname";
static const char TCONST_0040[] PROGMEM = "wcssid";
static const char TCONST_0041[] PROGMEM = "wcpass";
static const char TCONST_0042[] PROGMEM = "set_wifiAP";
static const char TCONST_0043[] PROGMEM = "APonly";
static const char TCONST_0044[] PROGMEM = "APpwd";
static const char TCONST_0045[] PROGMEM = "set_mqtt";
static const char TCONST_0046[] PROGMEM = "m_host";
static const char TCONST_0047[] PROGMEM = "m_port";
static const char TCONST_0048[] PROGMEM = "m_user";
static const char TCONST_0049[] PROGMEM = "m_pass";
static const char TCONST_004A[] PROGMEM = "mqtt_int";
static const char TCONST_004B[] PROGMEM = "set_other";
static const char TCONST_004C[] PROGMEM = "MIRR_H";
static const char TCONST_004D[] PROGMEM = "MIRR_V";
static const char TCONST_004E[] PROGMEM = "isFaderON";
static const char TCONST_004F[] PROGMEM = "DRand";
static const char TCONST_0050[] PROGMEM = "effSort";
static const char TCONST_0051[] PROGMEM = "txtSpeed";
static const char TCONST_0052[] PROGMEM = "txtOf";
static const char TCONST_0053[] PROGMEM = "spdcf";
static const char TCONST_0054[] PROGMEM = "ny_period";
static const char TCONST_0055[] PROGMEM = "ny_unix";
static const char TCONST_0056[] PROGMEM = "set_time";
static const char TCONST_0057[] PROGMEM = "TZSET";
static const char TCONST_0058[] PROGMEM = "userntp";
static const char TCONST_0059[] PROGMEM = "setdatetime";
static const char TCONST_005A[] PROGMEM = "update";
static const char TCONST_005B[] PROGMEM = "blue";
static const char TCONST_005C[] PROGMEM = "show_event";
static const char TCONST_005D[] PROGMEM = "event_conf";
static const char TCONST_005E[] PROGMEM = "eventList";
static const char TCONST_005F[] PROGMEM = "edit";
static const char TCONST_0060[] PROGMEM = "enabled";
static const char TCONST_0061[] PROGMEM = "d1";
static const char TCONST_0062[] PROGMEM = "d2";
static const char TCONST_0063[] PROGMEM = "d3";
static const char TCONST_0064[] PROGMEM = "d4";
static const char TCONST_0065[] PROGMEM = "d5";
static const char TCONST_0066[] PROGMEM = "d6";
static const char TCONST_0067[] PROGMEM = "d7";
static const char TCONST_0068[] PROGMEM = "evList";
static const char TCONST_0069[] PROGMEM = "repeat";
static const char TCONST_006A[] PROGMEM = "stopat";
static const char TCONST_006B[] PROGMEM = "tmEvent";
static const char TCONST_006C[] PROGMEM = "set_event";
static const char TCONST_006D[] PROGMEM = "show_button";
static const char TCONST_006E[] PROGMEM = "butt_conf";
static const char TCONST_006F[] PROGMEM = "buttList";
static const char TCONST_0070[] PROGMEM = "on";
static const char TCONST_0071[] PROGMEM = "hold";
static const char TCONST_0072[] PROGMEM = "onetime";
static const char TCONST_0073[] PROGMEM = "clicks";
static const char TCONST_0074[] PROGMEM = "bactList";
static const char TCONST_0075[] PROGMEM = "set_butt";
static const char TCONST_0076[] PROGMEM = "show_butt";
static const char TCONST_0077[] PROGMEM = "show_time";
static const char TCONST_0078[] PROGMEM = "show_wifi";
static const char TCONST_0079[] PROGMEM = "show_mic";
static const char TCONST_007A[] PROGMEM = "show_other";
static const char TCONST_007B[] PROGMEM = "m_pref";
static const char TCONST_007C[] PROGMEM = "#ffffff";
static const char TCONST_007D[] PROGMEM = "1609459200";
static const char TCONST_007E[] PROGMEM = "main";
static const char TCONST_007F[] PROGMEM = "dynCtrl*";
static const char TCONST_0080[] PROGMEM = "STA";
static const char TCONST_0081[] PROGMEM = "off";
static const char TCONST_0082[] PROGMEM = "effect";
static const char TCONST_0083[] PROGMEM = "move_next";
static const char TCONST_0084[] PROGMEM = "move_prev";
static const char TCONST_0085[] PROGMEM = "move_rnd";
static const char TCONST_0086[] PROGMEM = "reboot";
static const char TCONST_0087[] PROGMEM = "alarm";
static const char TCONST_0088[] PROGMEM = "aux_on";
static const char TCONST_0089[] PROGMEM = "aux_off";
static const char TCONST_008A[] PROGMEM = "aux_toggle";
static const char TCONST_008B[] PROGMEM = "embui/pub/";
static const char TCONST_008C[] PROGMEM = "pin";
static const char TCONST_008D[] PROGMEM = "act";
static const char TCONST_008E[] PROGMEM = "isClearing";
static const char TCONST_008F[] PROGMEM = "pUptime";
static const char TCONST_0090[] PROGMEM = "numInList";
static const char TCONST_0091[] PROGMEM = "effHasMic";
static const char TCONST_0092[] PROGMEM = "effname";
static const char TCONST_0093[] PROGMEM = "delEffCfg";
static const char TCONST_0094[] PROGMEM = "syslampFlags";
static const char TCONST_0095[] PROGMEM = "debug";
static const char TCONST_0096[] PROGMEM = "isShowSysMenu";
static const char TCONST_0097[] PROGMEM = "PINB"; // пин кнопки
static const char TCONST_0098[] PROGMEM = "CLmt"; // лимит тока
static const char TCONST_0099[] PROGMEM = "sysSettings";
static const char TCONST_009A[] PROGMEM = "ESPsysSettings";
static const char TCONST_009B[] PROGMEM = "PINMP3RX";
static const char TCONST_009C[] PROGMEM = "PINMP3TX";
static const char TCONST_009D[] PROGMEM = "isOnMP3";
static const char TCONST_009E[] PROGMEM = "showName";
static const char TCONST_009F[] PROGMEM = "show_mp3";
static const char TCONST_00A0[] PROGMEM = "set_mp3";
static const char TCONST_00A1[] PROGMEM = "settings_mp3";
static const char TCONST_00A2[] PROGMEM = "mp3volume";
static const char TCONST_00A3[] PROGMEM = "playTime";
static const char TCONST_00A4[] PROGMEM = "playName";
static const char TCONST_00A5[] PROGMEM = "playEffect";
static const char TCONST_00A6[] PROGMEM = "alarmSound";
static const char TCONST_00A7[] PROGMEM = "eqSetings";
static const char TCONST_00A8[] PROGMEM = "playMP3";
static const char TCONST_00A9[] PROGMEM = "mp3count";
static const char TCONST_00AA[] PROGMEM = "demo";
static const char TCONST_00AB[] PROGMEM = "soundfile";
static const char TCONST_00AC[] PROGMEM = "embui/get/";
static const char TCONST_00AD[] PROGMEM = "state";
static const char TCONST_00AE[] PROGMEM = "eff_config";
static const char TCONST_00AF[] PROGMEM = "limitAlarmVolume";
static const char TCONST_00B0[] PROGMEM = "delfromlist";
static const char TCONST_00B1[] PROGMEM = "delall";
static const char TCONST_00B2[] PROGMEM = "delCfg";
static const char TCONST_00B3[] PROGMEM = "orange";
static const char TCONST_00B4[] PROGMEM = "gbright";
static const char TCONST_00B5[] PROGMEM = "bparam";
static const char TCONST_00B6[] PROGMEM = "delete";
static const char TCONST_00B7[] PROGMEM = "warning";
static const char TCONST_00B8[] PROGMEM = "devicedatetime";
static const char TCONST_00B9[] PROGMEM = "text_config";
static const char TCONST_00BA[] PROGMEM = "edit_text_config";
static const char TCONST_00BB[] PROGMEM = "alarmP";
static const char TCONST_00BC[] PROGMEM = "alarmT";
static const char TCONST_00BD[] PROGMEM = "alarmPT";
static const char TCONST_00BE[] PROGMEM = "mp3_prev";
static const char TCONST_00BF[] PROGMEM = "mp3_next";
static const char TCONST_00C0[] PROGMEM = "mp3_p5";
static const char TCONST_00C1[] PROGMEM = "mp3_n5";
static const char TCONST_00C2[] PROGMEM = "pFS";
static const char TCONST_00C3[] PROGMEM = "txtBfade";
static const char TCONST_00C4[] PROGMEM = "drawbuff";
static const char TCONST_00C5[] PROGMEM = "draw";
static const char TCONST_00C6[] PROGMEM = "matrix";
static const char TCONST_00C7[] PROGMEM = "fillmatrix";
static const char TCONST_00C8[] PROGMEM = "drawing";
static const char TCONST_00C9[] PROGMEM = "drawing_ctrl";
static const char TCONST_00CA[] PROGMEM = "drawing_ctrl*";
static const char TCONST_00CB[] PROGMEM = "blabel";
static const char TCONST_00CC[] PROGMEM = "height";
static const char TCONST_00CD[] PROGMEM = "width";
static const char TCONST_00CE[] PROGMEM = "pRSSI";
static const char TCONST_00CF[] PROGMEM = "fileName2";
static const char TCONST_00D0[] PROGMEM = "control";
static const char TCONST_00D1[] PROGMEM = "afS";
static const char TCONST_00D2[] PROGMEM = "lV";
static const char TCONST_00D3[] PROGMEM = "sT";

/*
заменено на "0" и "1" глобально с поддержкой в .js фреймворка
static const char TCONST_FFFE[] PROGMEM = "0";
static const char TCONST_FFFF[] PROGMEM = "1";
*/

typedef enum : uint8_t {
EFF_NONE = (0U),                              // Специальный служебный эффект, не комментировать и индекс не менять константу!
EFF_WHITE_COLOR,                              // Белый свет
EFF_COLORS,                                   // Смена цвета
EFF_RAINBOW_2D,                               // Радуга универсальная
EFF_SPARKLES,                                 // Конфетти
EFF_PILE,                                     // Куча
EFF_SNOWSTORMSTARFALL,                        // Метель + Звездопад
EFF_MATRIX,                                   // Матрица
EFF_LIGHTERS,                                 // Светлячки
EFF_LIGHTER_TRACES,                           // Светлячки со шлейфом
// 10
EFF_CUBE,                                     // Блуждающий кубик
EFF_PULSE,                                    // Пульс
EFF_EVERYTHINGFALL,                           // Водо/огне/лава/радуга/хренопад
EFF_FIRE,                                     // Огонь
EFF_PAINTBALL,                                // Пейнтбол
EFF_3DNOISE,                                  // 3D-шум
EFF_CELL,                                     // Клеточки
EFF_F_LYING,                                  // Геометрический вальс
EFF_T_LEND,                                   // Тикси Ленд
EFF_LDIRKO,                                   // LDIRKO Ленд 
//20
EFF_OSCIL,                                    // Осцилятор 
EFF_WRAIN,                                    // Шторм 
EFF_FAIRY,                                    // Фея
EFF_FOUNT,                                    // Источник
EFF_BBALS,                                    // Прыгающие мячики
EFF_SINUSOID3,                                // Синусоид 3
EFF_METABALLS,                                // Метасферы
EFF_SPIRO,                                    // Спираль
EFF_COMET,                                    // Кометы, Парящие огни +
EFF_CIRCLES,                                  // Бульбулятор
//30
EFF_PRIZMATA,                                 // Призмата
EFF_FLOCK,                                    // Стая и Хищник
EFF_SWIRL,                                    // Водоворот
EFF_DRIFT,                                    // Дрифт
EFF_POPCORN,                                  // Попкорн
EFF_TWINKLES,                                 // Мерцание
EFF_RADAR,                                    // Радар
EFF_WAVES,                                    // Волны
EFF_FIRE2012,                                 // Огонь 2012
EFF_BENGALL,                                  // Бенгальские Огни
//40
EFF_BALLS,                                    // Шары
EFF_DNA,                                      // ДНК
EFF_FIRE2018,                                 // Огонь 2018
EFF_RINGS,                                    // Кодовый замок
EFF_CUBE2,                                    // Куб 2D
EFF_MAGMA,                                    // Магма
EFF_PICASSO,                                  // Пикассо
EFF_PICASSO2,                                 // Пикассо2
EFF_PICASSO3,                                 // Пикассо3
EFF_LEAPERS,                                  // Прыгуны
//50
EFF_WHIRL,                                    // Вихри
EFF_STAR,                                     // Звезды
EFF_AQUARIUM,                                 // Аквариум
EFF_FIREWORKS,                                // Фейерверк
EFF_PACIFIC,                                  // Тихий Океан
EFF_MUNCH,                                    // Вышиванка
EFF_NOISE,                                    // Цветной шум
EFF_BUTTERFLY,                                // Мотыльки
EFF_SHADOWS,                                  // Тени
EFF_PATTERNS,                                 // Узоры
//60
EFF_ARROWS,                                   // Стрелки
EFF_NBALLS,                                   // Дикие шарики
EFF_ATTRACT,                                  // Притяжение
EFF_SNAKE,                                    // Змейки by kDn
EFF_NEXUS,                                    // Nexus
EFF_MAZE,                                     // Лабиринт
EFF_FRIZZLES,                                 // Вьющийся цвет (с) Stepko
EFF_POLARL,                                   // Северное Сияние
EFF_FLOWER,                                   // Цветение
EFF_TEST,                                     // Змеиный Остров
//70
EFF_SMOKEBALLS,                               // Детские Сны (stepko)
EFF_PICASSO4,                                 // Меташары
EFF_LIQUIDLAMP,                               // Лаваламп 
EFF_RACER,                                    // Космо-Гонщик
EFF_SMOKER,                                   // Дым (Тест) 
EFF_TIME = (253U)                             // Часы (служебный, смещаем в конец)
#ifdef MIC_EFFECTS
,EFF_FREQ = (254U)                            // Частотный анализатор (служебный, смещаем в конец)
,EFF_OSC = (255U)                             // Осциллограф
#endif
} EFF_ENUM;

/** набор указателей на строки с именами эффектов
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов ставим указатель на 0-ю пустышку
 */
static const char* const T_EFFNAMEID[] PROGMEM = {
  TEFF_000, TEFF_001, TEFF_002, TEFF_003, TEFF_004, TEFF_005, TEFF_006, TEFF_007, TEFF_008, TEFF_009, TEFF_010, TEFF_011, TEFF_012, TEFF_013, TEFF_014, TEFF_015, // 0-15
  TEFF_016, TEFF_017, TEFF_018, TEFF_019, TEFF_020, TEFF_021, TEFF_022, TEFF_023, TEFF_024, TEFF_025, TEFF_026, TEFF_027, TEFF_028, TEFF_029, TEFF_030, TEFF_031, // 16-31
  TEFF_032, TEFF_033, TEFF_034, TEFF_035, TEFF_036, TEFF_037, TEFF_038, TEFF_039, TEFF_040, TEFF_041, TEFF_042, TEFF_043, TEFF_044, TEFF_045, TEFF_046, TEFF_047, // 32 - 47
  TEFF_048, TEFF_049, TEFF_050, TEFF_051, TEFF_052, TEFF_053, TEFF_054, TEFF_055, TEFF_056, TEFF_057, TEFF_058, TEFF_059, TEFF_060, TEFF_061, TEFF_062, TEFF_063, // 48 - 63
  TEFF_064, TEFF_065, TEFF_066, TEFF_067, TEFF_068, TEFF_069, TEFF_070, TEFF_071, TEFF_072, TEFF_073, TEFF_074, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 64 - 79
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 80 - 95
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 96 - 111
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 112 - 127
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 128 - 143
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 144 - 159
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 160 - 175
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 176 - 191
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 192 - 207
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 208 - 223
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 224 - 39
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_253, TEFF_254, TEFF_255  // 240 - 255
};

/**
 *  массив с версиями эффектов, порядковый номер элемента массива
 *  соответсвует ENUM эффекта
 *  парный номер версии означает, что у эффекта есть поддержка микрофона
 *  непарный - нет. 127 вариантов версий "должно хватить всем" (с) Джобс
 *  Не хочу создавать дополнительные массивы и лайеры существующих - kostyamat
 */
static const uint8_t T_EFFVER[] PROGMEM = {
  3, 6, 6, 4, 4, 5, 3, 3, 7, 5, 3, 6, 5, 3, 5, 8, // 0-15
  1, 5, 3, 7, 3, 7, 5, 5, 7, 3, 3, 7, 7, 5, 1, 5, // 16-31
  5, 5, 5, 3, 7, 3, 4, 3, 3, 9, 4, 5, 7, 1, 3, 3, // 32 - 47
  5, 6, 6, 6, 4, 7, 5, 4, 5, 5, 6, 3, 1, 5, 7, 6, // 48 - 63
  5, 1, 1, 5, 4, 1, 7, 5, 3, 1, 3, 0, 0, 0, 0, 0, // 64 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80 - 95
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96 - 111
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 112 - 127
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 128 - 143
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160 - 175
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176 - 191
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 192 - 207
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208 - 223
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 224 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 6, // 240 - 255
};


// младшие 4 бита отдаются на выбор контрола, что даст 16 различных типов контроллов
// старшие 4 бита используются как набор управляющих кодов, к примеру - отображать при включенном микрофоне, при выключенном и т.д., тоже 16 вариантов
typedef enum : uint8_t {ALWAYS=0,ISMICON,ISMICOFF,HIDE} CONTROL_CASE; // старшие 4 бита
typedef enum : uint8_t {RANGE=0,EDIT,CHECKBOX} CONTROL_TYPE; // младшие 4 бита
/**
 * типы/определения для палитр
 */
typedef const TProgmemRGBPalette16 PGMPalette;
#define FASTLED_PALETTS_COUNT 22
// все установленные биты для EFFFLAGS
#define SET_ALL_EFFFLAGS 3

/**
 * Набор статических строк, определяющих UI эффекта если он отличается от базового
 *
 *  GUI для эффекта по-умолчанию
 * Полный формат для пользовательского (id=0...7) параметра имеет вид: {"id":0,"type":0,"name":"Параметр","val":"127","min":"1","max":"255","step":"1"}
 * если какой-то из параметров стандартный из этого списка, то его можно опустить и не указывать
 * @nb@ - будет заменен на реальный номер эффекта, @name@ - на дефолтное имя эффекта, @ver@ - версия
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48813
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48848
 */

#define COTNROLS_PREFIX "\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":"

// Дефолтные контролы
static const char E_DEFUI[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[]}";
// Добавил ко всем конфигам чекбокс {Микрофон} с индексом 7. Ввиду модифированного поведения функции myLamp.isMicOnOff(), он будет появляться только если глобально
// микрофон включен, и эффект обладает зависимостью от микрофона в принципе.
static const char E_DEFMICUI[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
// Общая конфигурация для эффектов с 3-им ползунком для палитр
static const char E_3PAL[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_3PAL_MIC[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":16},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";

// Общая конфигурация для эффектов только с 2-мя ползунками "Яркость" и "Скорость", пример - эффект"Тихий Океан"
static const char E_2[]         PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48}]}"; // 3*16+0 для 2 контрола
static const char E_2_MIC[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}"; // 3*16+0 для 2 контрола

// Общая конфигурация для эффектов с 4-им ползунком для палитр
static const char E_4PAL[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_LLEND[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":0,\"min\":0,\"max\":10,\"name\":\"" DFTINTF_0D2 "\"},  {\"id\":4,\"val\":0,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":5,\"val\":16,\"min\":0,\"max\":32,\"name\":\"" DFTINTF_0D3 "\"}]}";
static const char E_SPARCLES[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"type\":16,\"max\":10,\"name\":\"" DFTINTF_0D0 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_LIGHTERS[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":16,\"max\":32,\"name\":\"" DFTINTF_0D5 "\"}, {\"id\":4,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0D4 "\"}]}";
static const char E_RADAR255[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0D1 "\"}, {\"id\":3,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0D4 "\"}]}";
static const char E_WHITE[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":1,\"name\":\"" DFTINTF_0D7 "\"}, {\"id\":3,\"name\":\"" DFTINTF_0E6 "\"}, {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_LEAPERS[]   PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"max\":50,\"name\":\"" DFTINTF_0D8 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_BUTTERFLY[] PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":64,\"name\":\"" DFTINTF_0D9 "\"}, {\"id\":4,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0DF "\"},  {\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0E0 "\"}]}";
static const char E_PULS[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0D6 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_BBALLS[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":32,\"name\":\"" DFTINTF_088 "\"},  {\"id\":4,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0E1 "\"}]}";
static const char E_PRIZMATA[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48}, {\"id\":3,\"name\":\"" DFTINTF_0DE "\"}, {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_AQUARIUM[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0D0 "\"},  {\"id\":3,\"name\":\"" DFTINTF_0DA "\"},  {\"id\":4,\"min\":0,\"max\":15,\"name\":\"" DFTINTF_0E2 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_FREQ[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0DB "\"},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":4,\"val\":2,\"max\":3,\"name\":\"" DFTINTF_0E3 "\"}, {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_OSC[]       PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":1,\"name\":\"" DFTINTF_0DC "\"},  {\"id\":2,\"name\":\"" DFTINTF_0DD "\"},  {\"id\":3,\"name\":\"" DFTINTF_0DB "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
// размерность ползунка "Узор" должна быть MAX_PATTERN + 1 (patterns.h). При добавлении паттернов - менять и тут.
static const char E_PATT[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":1,\"type\":48},  {\"id\":2,\"type\":48},  {\"id\":3,\"val\":0,\"min\":-32,\"max\":32,\"name\":\"" DFTINTF_0EA "\"},  {\"id\":4,\"val\":0,\"min\":-32,\"max\":32,\"name\":\"" DFTINTF_0EB "\"},  {\"id\":5,\"min\":0,\"max\":38,\"val\":0,\"name\":\"" DFTINTF_0EC "\"},  {\"id\":6,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0D4 "\"},  {\"id\":7,\"type\":2,\"val\":0,\"name\":\"" DFTINTF_116 "\"}]}";
static const char E_SHAD[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0ED "\"},  {\"id\":3,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0E4 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_ARR[]       PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":6,\"name\":\"" DFTINTF_0E5 "\"}]}";
static const char E_F2018[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0D7 "\"},  {\"id\":3,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0E4 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_NBAL[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":25,\"name\":\"" DFTINTF_0E6 "\"}]}";
static const char E_ATTRACT[]   PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0EE "\"},  {\"id\":3,\"val\":100,\"name\":\"" DFTINTF_0E9 "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_FLOCK[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":4,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0E8 "\"}]}";
static const char E_WAVES[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":4,\"max\":8,\"name\":\"" DFTINTF_0E7 "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_MUNCH[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":4,\"val\":4,\"min\":0,\"max\":8,\"name\":\"" DFTINTF_10E "\"} ,{\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_DRIFT[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":4,\"max\":4,\"name\":\"" DFTINTF_0FF "\"}]}";
static const char E_CUBE2D[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_100 "\"},  {\"id\":4,\"val\":4,\"max\":7,\"name\":\"" DFTINTF_10D "\"},  {\"id\":5,\"val\":4,\"max\":7,\"name\":\"" DFTINTF_10F "\"} ,{\"id\":6,\"type\":2,\"val\":0,\"name\":\"" DFTINTF_110 "\"}]}";
static const char E_FLAMP[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0D0 "\"}]}";
static const char E_LIGHT2[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":8,\"max\":16,\"name\":\"" DFTINTF_0D5 "\"}]}";
static const char E_CUBE[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"name\":\"" DFTINTF_0F1 "\"},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_STARFAIL[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":5,\"max\":10,\"name\":\"" DFTINTF_0EF "\"} , {\"id\":4,\"max\":3,\"name\":\"" DFTINTF_0FD "\"} ,{\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_108 "\"}]}"; 
static const char E_SNAKE[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":4,\"val\":4,\"max\":16,\"name\":\"" DFTINTF_0FE "\"},  {\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0D4 "\"},  {\"id\":6,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_111 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_NEXUS[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":5,\"max\":10,\"name\":\"" DFTINTF_0EF "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_FWORK[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":1,\"name\":\"" DFTINTF_0F2 "\"},  {\"id\":2,\"type\":48},  {\"id\":3,\"max\":8,\"name\":\"" DFTINTF_10C "\"},  {\"id\":4,\"type\":2,\"val\":0,\"name\":\"" DFTINTF_109 "\"}]}";
static const char E_MATRIX[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":12,\"max\":32,\"name\":\"" DFTINTF_0EF "\"},  {\"id\":4,\"val\":90,\"name\":\"" DFTINTF_0FC "\"},  {\"id\":5,\"val\":20,\"max\":60,\"name\":\"" DFTINTF_10A "\"}]}";
static const char E_NFIRE[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":50,\"max\":100,\"name\":\"" DFTINTF_088 "\"},  {\"id\":4,\"max\":12,\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_POPCORN[]   PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":16,\"max\":32,\"name\":\"" DFTINTF_0EF "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0DE "\"},  {\"id\":6,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_112 "\"}]}";
static const char E_MBL[]       PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_0F4 "\"},  {\"id\":4,\"val\":0,\"min\":0,\"name\":\"" DFTINTF_0D6 "\"}]}";
static const char E_LIQLAM[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"min\":0,\"max\":17,\"name\":\"" DFTINTF_0F4 "\"},  {\"id\":4,\"val\":0,\"min\":0,\"name\":\"" DFTINTF_0D6 "\"},  {\"id\":5,\"val\":0,\"min\":0,\"max\":4,\"name\":\"" DFTINTF_105 "\"},  {\"id\":6,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_106 "\"}]}";
static const char E_F2012_MIC[] PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":64,\"max\":128,\"name\":\"" DFTINTF_088 "\"},  {\"id\":4,\"val\":6,\"max\":10,\"name\":\"" DFTINTF_084 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_DNA[]       PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0F3 "\"}]}";
static const char E_SNOW[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"val\":\"1\",\"max\":3,\"name\":\"" DFTINTF_0F5 "\"}]}";
static const char E_SMOKBALLS[] PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":8,\"max\":16,\"name\":\"" DFTINTF_0D5 "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_PALMICUI[]  PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_0FB "\"},  {\"id\":4,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_0F5 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_COLORS[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"type\":32,\"val\":\"2\",\"max\":3,\"name\":\"" DFTINTF_0E3 "\"},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";
static const char E_TLAND[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":25,\"min\":0,\"max\":35,\"name\":\"" DFTINTF_0D2 "\"},  {\"id\":4,\"val\":160,\"min\":0,\"name\":\"" DFTINTF_101 "\"},  {\"id\":5,\"val\":0,\"min\":0,\"name\":\"" DFTINTF_113 "\"},  {\"id\":6,\"max\":4,\"name\":\"" DFTINTF_114 "\"}]}";
static const char E_FLYING[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"max\":128,\"name\":\"" DFTINTF_0DE "\"},  {\"id\":4,\"val\":7,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"},  {\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_107 "\"}]}";
static const char E_WRAIN[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":25,\"max\":45,\"name\":\"" DFTINTF_0EF "\"},  {\"id\":4,\"val\":4,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_100 "\"},  {\"id\":5,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_102 "\"},  {\"id\":6,\"type\":2,\"val\":1,\"name\":\"" DFTINTF_10B "\"},  {\"id\":7,\"max\":8,\"name\":\"" DFTINTF_115 "\"}]}";
static const char E_CLOCK[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":4,\"max\":8,\"name\":\"" DFTINTF_0F7 "\"},  {\"id\":4,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_COMET[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":4,\"max\":6,\"name\":\"" DFTINTF_00A "\"},  {\"id\":4,\"name\":\"" DFTINTF_0FC "\"},  {\"id\":5,\"val\":6,\"max\":12,\"name\":\"" DFTINTF_104 "\"},  {\"id\":6,\"val\":32,\"max\":64,\"name\":\"" DFTINTF_0DE "\"}]}";
static const char E_CELL[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"min\":0,\"max\":6,\"name\":\"" DFTINTF_0D2 "\"}]}"; 
static const char E_FAIRY[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"" DFTINTF_0F9 "\"}]}";
static const char E_FOUNT[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"" DFTINTF_0FA "\"},  {\"id\":4,\"val\":0,\"min\":0,\"max\":10,\"name\":\"" DFTINTF_103 "\"}]}";
static const char E_POLAR[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"val\":1,\"max\":14,\"name\":\"" DFTINTF_084 "\"}]}";
static const char E_SMOKER[]    PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"val\":160,\"max\":255,\"name\":\"" DFTINTF_0D0 "\"}, {\"id\":3,\"val\":85,\"max\":255,\"name\":\"" DFTINTF_0DA "\"}]}";
static const char E_DRAGE[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48}, {\"id\":3,\"val\":5,\"max\":10,\"name\":\"" DFTINTF_0EF "\"}, {\"id\":4,\"val\":5,\"min\":1,\"max\":9,\"name\":\"" DFTINTF_0F6 "\"}, {\"id\":5,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_084 "\"}, {\"id\":6,\"type\":2,\"val\":\"true\",\"name\":\"" DFTINTF_0F9 "\"}]}"; //DFTINTF_117

// Инженерный
//static const char E_TEST[]      PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":2,\"type\":48},  {\"id\":3,\"val\":8,\"max\":16,\"name\":\"" DFTINTF_0D5 "\"}]}";
static const char E_TEST2[]     PROGMEM = "{" COTNROLS_PREFIX SF(SET_ALL_EFFFLAGS) ",\"ctrls\":[{\"id\":3,\"name\":\"" DFTINTF_084 "\"},  {\"id\":4},  {\"id\":5},  {\"id\":6},  {\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}]}";

/** набор указателей на строки с UI-конфигом для эффектов по-умолчанию
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов или эффектов с дефолтными ползунками ставим указатель на базовый конфиг
 * не обязательно каждому эффекту лепить свой конфиг, можно ставить указатели на эффекты с похожими контролами
 */
static const char* const T_EFFUICFG[] PROGMEM = {
  E_DEFUI, E_WHITE,   E_COLORS,   E_DEFMICUI, E_SPARCLES, E_DRAGE, E_STARFAIL,   E_MATRIX, E_LIGHTERS,  E_LIGHT2,    E_CUBE,   E_PULS,  E_4PAL,   E_NFIRE,   E_2,        E_PALMICUI, // 0-15
  E_CELL,  E_FLYING,  E_TLAND,    E_LLEND,    E_3PAL,     E_WRAIN, E_FAIRY,      E_FOUNT,  E_BBALLS,    E_DEFUI,     E_DEFUI,  E_3PAL,  E_COMET,  E_4PAL,    E_PRIZMATA, E_FLOCK, // 16-31
  E_3PAL,  E_DRIFT,   E_POPCORN,  E_4PAL,     E_RADAR255, E_WAVES, E_F2012_MIC,  E_FAIRY,  E_4PAL,      E_DNA,       E_F2018,  E_CLOCK, E_CUBE2D, E_NFIRE,   E_DEFUI,    E_DEFUI, // 32 - 47
  E_DEFUI, E_LEAPERS, E_3PAL_MIC, E_3PAL_MIC, E_AQUARIUM, E_FWORK, E_2,          E_MUNCH,  E_3PAL,      E_BUTTERFLY, E_SHAD,   E_PATT,  E_ARR,    E_NBAL,    E_ATTRACT,  E_SNAKE, // 48 - 63
  E_NEXUS, E_DEFUI,   E_DEFUI,    E_POLAR,    E_2_MIC,    E_NEXUS, E_SMOKBALLS,  E_MBL,    E_LIQLAM,    E_3PAL,      E_SMOKER, E_DEFUI, E_DEFUI,  E_DEFUI,   E_DEFUI,    E_DEFUI, // 64 - 79
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 80 - 95
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 96 - 111
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 112 - 127
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 128 - 143
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 144 - 159
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 160 - 175
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 176 - 191
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 192 - 207
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 208 - 223
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 224 - 39
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_3PAL, E_FREQ, E_OSC  // 240 - 255
};

/**
 * константы/определения
 */
#define MIN_RANGE 1     // заложим дейфан пока нет динамических ползунков
#define MAX_RANGE 255   // заложим дейфан пока нет динамических ползунков

/**
 * Набор палитр в дополнение к тем что идут с FastLED
 * новые палитры добавляем в алфавитном порядке
 */
static const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
static const TProgmemRGBPalette16 NormalFire3_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3010, 0xff3c20, 0xff4835, 0xff5440, 0xff6055, 0xff6c60, 0xff7866};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 MagmaColor_p FL_PROGMEM = {CRGB::Black, 0x240000, 0x480000, 0x660000, 0x9a1100, 0xc32500, 0xd12a00, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400, 0xffffff};
static const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, 0x560000, 0x6b0000, 0x820000, 0x9a0011, CRGB::FireBrick, 0xc22520, 0xd12a1c, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400};                      // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x240707, 0x470e0e, 0x6b1414, 0x8e1b1b, CRGB::FireBrick, 0xc14244, 0xd16166, 0xe08187, 0xf0a0a9, CRGB::Pink, 0xff9ec0, 0xff7bb5, 0xff59a9, 0xff369e, CRGB::DeepPink};        //* Red
static const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x332100, 0x664200, 0x996300, 0xcc8400, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold, 0xf8cd06, 0xf0c30d, 0xe9b913, 0xe1af1a, CRGB::Goldenrod};           //* Yellow
static const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, 0x001a00, 0x003300, 0x004d00, 0x006600, CRGB::Green, 0x239909, 0x45b313, 0x68cc1c, 0x8ae626, CRGB::GreenYellow, 0x94f530, 0x7ceb30, 0x63e131, 0x4bd731, CRGB::LimeGreen};     //* Green
static const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, 0x000033, 0x000066, 0x000099, 0x0000cc, CRGB::Blue, 0x0026ff, 0x004cff, 0x0073ff, 0x0099ff, CRGB::DeepSkyBlue, 0x1bc2fe, 0x36c5fd, 0x51c8fc, 0x6ccbfb, CRGB::LightSkyBlue};  //* Blue
static const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 0x3c0084, 0x2d0086, 0x1e0087, 0x0f0089, CRGB::DarkBlue};        //* Indigo
static const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, 0x591694, 0x682da6, 0x7643b7, 0x855ac9, CRGB::MediumPurple, 0xa95ecd, 0xbe4bbe, 0xd439b0, 0xe926a1, CRGB::DeepPink}; //* Violet
static const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {0x000000, 0x060707, 0x101110, 0x151717, 0x1C1D22, 0x242A28, 0x363B3A, 0x313634, 0x505552, 0x6B6C70, 0x98A4A1, 0xC1C2C1, 0xCACECF, 0xCDDEDD, 0xDEDFE0, 0xB2BAB9};        //* Orange
static const TProgmemRGBPalette16 ZeebraColors_p FL_PROGMEM = {CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black};
// Добавил "белую" палитру для "Огонь 2012", в самом конце 4-го ползунка, огонь горит белым цветом, - красиво для белой динамической подсветки
static const TProgmemRGBPalette16 WhiteBlackColors_p FL_PROGMEM = {CRGB::Black, CRGB::Gray, CRGB::White, CRGB::WhiteSmoke};
// stepko нашел и перерисовал палитры в https://colorswall.com/
static const TProgmemRGBPalette16 AcidColors_p FL_PROGMEM = {0xffff00, 0xd2b328, 0xd25228, 0x711323, 0x6c000c, 0x5a0c00, 0x6d373a, 0xaa5a62, 0x604564, 0x313164, 0x332765, 0x3a2465, 0x4b1665, 0x4b0069, 0x31004c, 0x200046};
static const TProgmemRGBPalette16 StepkosColors_p FL_PROGMEM = {0x0000ff, 0x0f00f0, 0x1e00e1, 0x2d00d2, 0x3c00c3, 0x4b00b4, 0x5a00a5, 0x690096, 0x780087, 0x870078, 0x9600cd, 0xa50050, 0xb40041, 0xc30032, 0xd20023, 0xe10014};
static const TProgmemRGBPalette16 AutumnColors_p FL_PROGMEM = {0xbc2300, 0xc84416, 0xdc642c, 0xe69664, 0xfbb979, 0xca503d, 0x882c1c, 0x9a3631, 0xa9624e, 0xcc9762, 0xdcc0b5, 0xc1a29f, 0x826468, 0x4a3334, 0x231a1a, 0x161113};
static const TProgmemRGBPalette16 NeonColors_p FL_PROGMEM = {0x00b1d0, 0x0f93ec, 0x3572ff, 0x4157ff, 0x6162ff, 0x686cff, 0x7473ff, 0x8689e5, 0x9e9dc6, 0x9694ac, 0x979b9b, 0x888b8c, 0x767680, 0x596160, 0x6c736f, 0x7b7359};
static const TProgmemRGBPalette16 EveningColors_p FL_PROGMEM = {0x1e0443, 0x6d0081, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x7900a1, 0x820055, 0xc80000, 0xe57b00, 0xff9d5a, 0xc58b32, 0xd8d400, 0xffff00};
static const TProgmemRGBPalette16 OrangeColors_p FL_PROGMEM = {0xffff00, 0xfff100, 0xffe100, 0xffd100, 0xffc100, 0xffb100, 0xffa100, 0xff9100, 0xff8100, 0xff7100, 0xff6100, 0xff5100, 0xff4100, 0xff3100, 0xff2100, 0xff1100};

// Удачная палитра от Stepko заменю ка я ею RainbowStripeColors_p, которая "рябит" на большинстве эффектов
static const TProgmemRGBPalette16 AuroraColors_p FL_PROGMEM = {0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040, 0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00};
static const TProgmemRGBPalette16 HolyLightsColors_p FL_PROGMEM = {0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00, 0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040};


#if  576U < (WIDTH * HEIGHT)
  #define BIGMATRIX
#endif

// ==== Константы для эффектов ====

// Эффекты на базе "3D Noise"
#define NOISE_SCALE_AMP  (58.)    // амплификатор шкалы (влияет на машстаб "пятен" эффекта, большие пятна выглядят красивее чем куча мелких)
#define NOISE_SCALE_ADD  (8.)     // корректор шкалы


#define BALLS_AMOUNT           (7U)                 // максимальное количество "шариков"
#define LIGHTERS_AM            (32U)                // светлячки
#define NUM_LAYERS             (1U)                 // The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS2            (2U)                 // The coordinates for 3 16-bit noise spaces.
#define AVAILABLE_BOID_COUNT   (10U)                // стая, кол-во птиц

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
#define e_com_TAILSPEED             (500)         // скорость смещения хвоста
#define e_com_BLUR                  (12U)         // размытие хвоста
#define e_com_3DCOLORSPEED          (3U)          // скорость случайного изменения цвета (0й - режим)

// ------------- светлячки со шлейфом -------------
//#define BALLS_AMOUNT          (7U)                          // максимальное количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)

// --------------------------- эффект мячики ----------------------
#define bballsMaxNUM_BALLS     (WIDTH-1)             // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
#define bballsGRAVITY          (-9.7f)               // Downward (negative) acceleration of gravity in m/s^2
#define bballsH0               (2)                   // Starting height, in meters, of the ball (strip length)
#define bballsVImpact0         (EffectMath::sqrt(-2 * bballsGRAVITY * bballsH0))

// ------------- метель -------------
#define SNOW_DENSE            (60U)                         // плотность снега
#define SNOW_TAIL_STEP        (100U)                        // длина хвоста
#define SNOW_SATURATION       (0U)                          // насыщенность (от 0 до 255)

// ------------- звездопад -------------
#define STAR_DENSE            (60U)                         // плотность комет
#define STAR_TAIL_STEP        (100U)                        // длина хвоста кометы
#define STAR_SATURATION       (150U)                        // насыщенность кометы (от 0 до 255)

// ============= DRIFT / ДРИФТ ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h
#define CENTER_max  max(WIDTH / 2, HEIGHT / 2) // Наибольшее значение центра
#define WIDTH_steps  256U / WIDTH   // диапазон значений приходящихся на 1 пиксель ширины матрицы
#define HEIGHT_steps 256U / HEIGHT // диапазон значений приходящихся на 1 пиксель высоты матрицы



template <class T>
class Vector2 {
public:
    T x, y;

    Vector2() :x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}
    Vector2(const Vector2& v) : x(v.x), y(v.y) {}

    Vector2& operator=(const Vector2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    bool isEmpty() {
        return x == 0 && y == 0;
    }

    bool operator==(Vector2& v) {
        return x == v.x && y == v.y;
    }

    bool operator!=(Vector2& v) {
        return !(x == y);
    }

    Vector2 operator+(Vector2& v) {
        return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(Vector2& v) {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2& operator+=(Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2& operator-=(Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2 operator+(double s) {
        return Vector2(x + s, y + s);
    }
    Vector2 operator-(double s) {
        return Vector2(x - s, y - s);
    }
    Vector2 operator*(double s) {
        return Vector2(x * s, y * s);
    }
    Vector2 operator/(double s) {
        return Vector2(x / s, y / s);
    }

    Vector2& operator+=(double s) {
        x += s;
        y += s;
        return *this;
    }
    Vector2& operator-=(double s) {
        x -= s;
        y -= s;
        return *this;
    }
    Vector2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }
    Vector2& operator/=(double s) {
        x /= s;
        y /= s;
        return *this;
    }

    void set(T x, T y) {
        this->x = x;
        this->y = y;
    }

    void rotate(double deg) {
        double theta = deg / 180.0 * M_PI;
        double c = cos(theta);
        double s = sin(theta);
        double tx = x * c - y * s;
        double ty = x * s + y * c;
        x = tx;
        y = ty;
    }

    Vector2& normalize() {
        if (length() == 0) return *this;
        *this *= (1.0 / length());
        return *this;
    }

    float dist(Vector2 v) const {
        Vector2 d(v.x - x, v.y - y);
        return d.length();
    }
    float length() const {
        return sqrt(x * x + y * y);
    }

    float mag() const {
        return length();
    }

    float magSq() {
        return (x * x + y * y);
    }

    void truncate(double length) {
        double angle = atan2f(y, x);
        x = length * cos(angle);
        y = length * sin(angle);
    }

    Vector2 ortho() const {
        return Vector2(y, -x);
    }

    static float dot(Vector2 v1, Vector2 v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
    static float cross(Vector2 v1, Vector2 v2) {
        return (v1.x * v2.y) - (v1.y * v2.x);
    }

    void limit(float max) {
        if (magSq() > max*max) {
            normalize();
            *this *= max;
        }
    }
};

typedef Vector2<float> PVector;

// Flocking
// Daniel Shiffman <http://www.shiffman.net>
// The Nature of Code, Spring 2009

// Boid class
// Methods for Separation, Cohesion, Alignment added

class Boid {
  public:

    PVector location;
    PVector velocity;
    PVector acceleration;
    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed

    float desiredseparation = 4;
    float neighbordist = 8;
    byte colorIndex = 0;
    float mass;

    boolean enabled = true;

    Boid() {}

    Boid(float x, float y) {
      acceleration = PVector(0, 0);
      velocity = PVector(randomf(), randomf());
      location = PVector(x, y);
      maxspeed = 1.5;
      maxforce = 0.05;
    }

    static float randomf() {
      return mapfloat(random(0, 255), 0, 255, -.5, .5);
    }

    static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    void run(Boid boids [], uint8_t boidCount) {
      flock(boids, boidCount);
      update();
      // wrapAroundBorders();
      // render();
    }

    // Method to update location
    void update() {
      // Update velocity
      velocity += acceleration;
      // Limit speed
      velocity.limit(maxspeed);
      location += velocity;
      // Reset acceleration to 0 each cycle
      acceleration *= 0;
    }

    void applyForce(PVector force) {
      // We could add mass here if we want A = F / M
      acceleration += force;
    }

    void repelForce(PVector obstacle, float radius) {
      //Force that drives boid away from obstacle.

      PVector futPos = location + velocity; //Calculate future position for more effective behavior.
      PVector dist = obstacle - futPos;
      float d = dist.mag();

      if (d <= radius) {
        PVector repelVec = location - obstacle;
        repelVec.normalize();
        if (d != 0) { //Don't divide by zero.
          // float scale = 1.0 / d; //The closer to the obstacle, the stronger the force.
          repelVec.normalize();
          repelVec *= (maxforce * 7);
          if (repelVec.mag() < 0) { //Don't let the boids turn around to avoid the obstacle.
            repelVec.y = 0;
          }
        }
        applyForce(repelVec);
      }
    }

    // We accumulate a new acceleration each time based on three rules
    void flock(Boid boids [], uint8_t boidCount) {
      PVector sep = separate(boids, boidCount);   // Separation
      PVector ali = align(boids, boidCount);      // Alignment
      PVector coh = cohesion(boids, boidCount);   // Cohesion
      // Arbitrarily weight these forces
      sep *= 1.5;
      ali *= 1.0;
      coh *= 1.0;
      // Add the force vectors to acceleration
      applyForce(sep);
      applyForce(ali);
      applyForce(coh);
    }

    // Separation
    // Method checks for nearby boids and steers away
    PVector separate(Boid boids [], uint8_t boidCount) {
      PVector steer = PVector(0, 0);
      int count = 0;
      // For every boid in the system, check if it's too close
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation)) {
          // Calculate vector pointing away from neighbor
          PVector diff = location - other.location;
          diff.normalize();
          diff /= d;        // Weight by distance
          steer += diff;
          count++;            // Keep track of how many
        }
      }
      // Average -- divide by how many
      if (count > 0) {
        steer /= (float) count;
      }

      // As long as the vector is greater than 0
      if (steer.mag() > 0) {
        // Implement Reynolds: Steering = Desired - Velocity
        steer.normalize();
        steer *= maxspeed;
        steer -= velocity;
        steer.limit(maxforce);
      }
      return steer;
    }

    // Alignment
    // For every nearby boid in the system, calculate the average velocity
    PVector align(Boid boids [], uint8_t boidCount) {
      PVector sum = PVector(0, 0);
      int count = 0;
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        if ((d > 0) && (d < neighbordist)) {
          sum += other.velocity;
          count++;
        }
      }
      if (count > 0) {
        sum /= (float) count;
        sum.normalize();
        sum *= maxspeed;
        PVector steer = sum - velocity;
        steer.limit(maxforce);
        return steer;
      }
      else {
        return PVector(0, 0);
      }
    }

    // Cohesion
    // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
    PVector cohesion(Boid boids [], uint8_t boidCount) {
      PVector sum = PVector(0, 0);   // Start with empty vector to accumulate all locations
      int count = 0;
      for (int i = 0; i < boidCount; i++) {
        Boid other = boids[i];
        if (!other.enabled)
          continue;
        float d = location.dist(other.location);
        if ((d > 0) && (d < neighbordist)) {
          sum += other.location; // Add location
          count++;
        }
      }
      if (count > 0) {
        sum /= count;
        return seek(sum);  // Steer towards the location
      }
      else {
        return PVector(0, 0);
      }
    }

    // A method that calculates and applies a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    PVector seek(PVector target) {
      PVector desired = target - location;  // A vector pointing from the location to the target
      // Normalize desired and scale to maximum speed
      desired.normalize();
      desired *= maxspeed;
      // Steering = Desired minus Velocity
      PVector steer = desired - velocity;
      steer.limit(maxforce);  // Limit to maximum steering force
      return steer;
    }

    // A method that calculates a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    void arrive(PVector target) {
      PVector desired = target - location;  // A vector pointing from the location to the target
      float d = desired.mag();
      // Normalize desired and scale with arbitrary damping within 100 pixels
      desired.normalize();
      if (d < 4) {
        float m = map(d, 0, 100, 0, maxspeed);
        desired *= m;
      }
      else {
        desired *= maxspeed;
      }

      // Steering = Desired minus Velocity
      PVector steer = desired - velocity;
      steer.limit(maxforce);  // Limit to maximum steering force
      applyForce(steer);
      //Serial.println(d);
    }

    void wrapAroundBorders() {
      if (location.x < 0) location.x = WIDTH - 1;
      if (location.y < 0) location.y = HEIGHT - 1;
      if (location.x >= WIDTH) location.x = 0;
      if (location.y >= HEIGHT) location.y = 0;
    }

    void avoidBorders() {
      PVector desired = velocity;

      if (location.x < 8) desired = PVector(maxspeed, velocity.y);
      if (location.x >= WIDTH - 8) desired = PVector(-maxspeed, velocity.y);
      if (location.y < 8) desired = PVector(velocity.x, maxspeed);
      if (location.y >= HEIGHT - 8) desired = PVector(velocity.x, -maxspeed);

      if (desired != velocity) {
        PVector steer = desired - velocity;
        steer.limit(maxforce);
        applyForce(steer);
      }

      if (location.x < 0) location.x = 0;
      if (location.y < 0) location.y = 0;
      if (location.x >= WIDTH) location.x = WIDTH - 1;
      if (location.y >= HEIGHT) location.y = HEIGHT - 1;
    }

    bool bounceOffBorders(float bounce) {
      bool bounced = false;

      if (location.x >= WIDTH) {
        location.x = WIDTH - 1;
        velocity.x *= -bounce;
        bounced = true;
      }
      else if (location.x < 0) {
        location.x = 0;
        velocity.x *= -bounce;
        bounced = true;
      }

      if (location.y >= HEIGHT) {
        location.y = HEIGHT - 1;
        velocity.y *= -bounce;
        bounced = true;
      }
      else if (location.y < 0) {
        location.y = 0;
        velocity.y *= -bounce;
        bounced = true;
      }

      return bounced;
    }

    void render() {
      // // Draw a triangle rotated in the direction of velocity
      // float theta = velocity.heading2D() + radians(90);
      // fill(175);
      // stroke(0);
      // pushMatrix();
      // translate(location.x,location.y);
      // rotate(theta);
      // beginShape(TRIANGLES);
      // vertex(0, -r*2);
      // vertex(-r, r*2);
      // vertex(r, r*2);
      // endShape();
      // popMatrix();
      // backgroundLayer.drawPixel(location.x, location.y, CRGB::Blue);
    }
};

// ------------------------------ ЭФФЕКТ КУБИК 2D ----------------------
// (c) SottNick
// refactored by Vortigont
#define CUBE2D_MAX_SIZE 7       // максимальный размер обрабатываемых квадратов (AxA)
#define CUBE2D_PAUSE_FRAMES 20   // число кадров паузы между трансформациями
#endif

// ----------- Эффект "Звезды" адаптация SottNick
#define CENTER_DRIFT_SPEED 6U         // скорость перемещения плавающего центра возникновения звёзд
#define STAR_BLENDER 128U             // хз что это

//---------- Эффект "Фейерверк" адаптация kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373

#define MODEL_BORDER (HEIGHT - 4U)  // как далеко за экран может вылетить снаряд, если снаряд вылетает за экран, то всышка белого света (не особо логично)
#define MODEL_WIDTH  (MODEL_BORDER + WIDTH  + MODEL_BORDER) // не трогать, - матиматика
#define MODEL_HEIGHT (MODEL_BORDER + HEIGHT + MODEL_BORDER) // -//-
#define PIXEL_X_OFFSET ((MODEL_WIDTH  - WIDTH ) / 2) // -//-
#define PIXEL_Y_OFFSET ((MODEL_HEIGHT - HEIGHT) / 2) // -//-

#define SPARK 8U // максимальное количество снарядов
#define NUM_SPARKS WIDTH // количество разлитающихся питард (частей снаряда)
const saccum78 gGravity = 10;
const fract8  gBounce = 127;
const fract8  gDrag = 255;

typedef struct _DOTS_STORE {
    accum88 gBurstx;
    accum88 gBursty;
    saccum78 gBurstxv;
    saccum78 gBurstyv;
    CRGB gBurstcolor;
    bool gSkyburst = false;
} DOTS_STORE;

class Dot {    // класс для создания снарядов и питард
private:
  CRGB empty = CRGB(0,0,0);
public:
  byte    show;
  byte    theType;
  accum88 x;
  accum88 y;
  saccum78 xv;
  saccum78 yv;
  accum88 r;
  CRGB color;

  Dot() {
    show = 0;
    theType = 0;
    x =  0;
    y =  0;
    xv = 0;
    yv = 0;
    r  = 0;
    color.setRGB( 0, 0, 0);
  }

  void Draw(CRGB *leds);
  void Move(DOTS_STORE &store, bool Flashing);
  void GroundLaunch(DOTS_STORE &store);
  void Skyburst( accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor, uint8_t dim);
  CRGB &piXY(CRGB *leds, byte x, byte y);

  int16_t scale15by8_local( int16_t i, fract8 _scale )
  {
    int16_t result;
    result = (int32_t)((int32_t)i * _scale) / 256;
    return result;
  };

  void screenscale(accum88 a, byte N, byte &screen, byte &screenerr)
  {
    byte ia = a >> 8;
    screen = scale8(ia, N);
    byte m = screen * (256 / N);
    screenerr = (ia - m) * scale8(255, N);
    return;
  };
};

// ------------ Эффект "Тихий Океан"
//  "Pacifica" перенос кода kostyamat
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
static const TProgmemRGBPalette16 pacifica_palette_1 FL_PROGMEM =
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
static const TProgmemRGBPalette16 pacifica_palette_2 FL_PROGMEM =
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
static const TProgmemRGBPalette16 pacifica_palette_3 FL_PROGMEM =
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// ---- Эффект "Мотыльки"
// (с) Сотнег, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49262
#define BUTTERFLY_MAX_COUNT           (64U) // максимальное количество мотыльков
#define BUTTERFLY_FIX_COUNT           (28U) // количество мотыльков для режима, конда бегунок Масштаб регулирует цвет

//------------ Эффект "Змеиный Остров"
#define SNAKE_LENGTH  (HEIGHT / 2U)
#define SNAKE2_LENGTH (HEIGHT / 2U + WIDTH / 4U)
#define MAX_SNAKES    (16U) 

// SottNick константы
//константы размера матрицы вычисляется только здесь и не меняется в эффектах
const uint8_t CENTER_X_MINOR =  (WIDTH / 2) -  ((WIDTH - 1) & 0x01); // центр матрицы по ИКСУ, сдвинутый в меньшую сторону, если ширина чётная
const uint8_t CENTER_Y_MINOR = (HEIGHT / 2) - ((HEIGHT - 1) & 0x01); // центр матрицы по ИГРЕКУ, сдвинутый в меньшую сторону, если высота чётная
const uint8_t CENTER_X_MAJOR =   WIDTH / 2  + (WIDTH % 2);           // центр матрицы по ИКСУ, сдвинутый в большую сторону, если ширина чётная
const uint8_t CENTER_Y_MAJOR =  HEIGHT / 2  + (HEIGHT % 2);          // центр матрицы по ИГРЕКУ, сдвинутый в большую сторону, если высота чётная


// ***************** НАСТРОЙКИ ГЕНЕРАЦИИ ЛАБИРИНТА *****************
#define GAMEMODE 0        // режим игры: 0 - видим весь лабиринт, 1 - видим вокруг себя часть
#define FOV 3             // область видимости в режиме игры 1

// размеры лабиринта ДОЛЖНЫ БЫТЬ НЕЧЁТНЫЕ независимо от размеров матрицы!
// при SHIFT 1 размер лабиринта можно ставить на 1 длиннее матрицы (матрица 16х16 лабиринт 17х17)
#if (WIDTH % 2 == 0)
#define MAZE_WIDTH (WIDTH-1)      // ширина лабиринта
#else
#define MAZE_WIDTH WIDTH          // ширина лабиринта
#endif

#if (HEIGHT % 2 == 0)
#define MAZE_HEIGHT (HEIGHT-1)    // высота лабиринта
#else
#define MAZE_HEIGHT HEIGHT        // высота лабиринта
#endif
#define MAZE_SHIFT 0                   // (1 да / 0 нет) смещение лабиринта (чтобы не видеть нижнюю и левую стену)
