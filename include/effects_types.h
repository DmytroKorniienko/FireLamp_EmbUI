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


/** набор указателей на строки с именами эффектов
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов ставим указатель на 0-ю пустышку
 */
static const char* const T_EFFNAMEID[] PROGMEM = { 
  TEFF_000, TEFF_001, TEFF_002, TEFF_003, TEFF_004, TEFF_005, TEFF_006, TEFF_007, TEFF_008, TEFF_009, TEFF_010, TEFF_011, TEFF_012, TEFF_013, TEFF_014, TEFF_015, // 0-15
  TEFF_016, TEFF_017, TEFF_018, TEFF_019, TEFF_020, TEFF_021, TEFF_022, TEFF_023, TEFF_024, TEFF_025, TEFF_026, TEFF_027, TEFF_028, TEFF_029, TEFF_030, TEFF_031, // 16-31
  TEFF_032, TEFF_033, TEFF_034, TEFF_035, TEFF_036, TEFF_037, TEFF_038, TEFF_039, TEFF_040, TEFF_041, TEFF_042, TEFF_043, TEFF_044, TEFF_045, TEFF_046, TEFF_047, // 32 - 47
  TEFF_048, TEFF_049, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 48 - 63
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 64 - 79
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
  TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_253, TEFF_254, TEFF_000  // 240 - 255
};

/**
 *  массив с версиями эффектов, порядковый номер элемента массива
 *  соответсвует ENUM эффекта
 */
static const uint8_t T_EFFVER[] PROGMEM = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0-15
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 16-31
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 32 - 47
  1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 48 - 63
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64 - 79
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
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, // 240 - 255
};


/**
 * Набор статических строк, определяющих UI эффекта если он отличается от базового
 *
 *  GUI для эффекта по-умолчанию
 * Полный формат для пользовательского (id=3...7) параметра имеет вид: {\"id\":3,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр\"}
 * @nb@ - будет заменен на реальный номер эффекта, @name@ - на дефолтное имя эффекта
 */
static const char E_DEFUI[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"}]}";
// пример для эффекта "конфети" или как там его...
static const char E_SPARCLES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":10,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Эффект\"}]}";

/** набор указателей на строки с UI-конфигом для эффектов по-умолчанию
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов или эффектов с дефолтными ползунками ставим указатель на базовый конфиг
 * не обязательно каждому эффекту лепить свой конфиг, можно ставить указатели на эффекты с похожими контролами
 */
static const char* const T_EFFUICFG[] PROGMEM = {
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_SPARCLES_CFG, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 0-15
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 16-31
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 32 - 47
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 48 - 63
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 64 - 79
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
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI  // 240 - 255
};

/**
 * константы/определения
 */
#define MAP_SIN 1
#define MAP_COS 0
#define MOVE_X 1
#define MOVE_Y 0
#define MIN_RANGE 1     // заложим дейфан пока нет динамических ползунков
#define MAX_RANGE 255   // заложим дейфан пока нет динамических ползунков

/**
 * типы/определения для палитр
 */
typedef const TProgmemRGBPalette16 PGMPalette;
#define FASTLED_PALETTS_COUNT 8

/**
 * Набор палитр в дополнение к тем что идут с FastLED
 * новые палитры добавляем в алфавитном порядке
 */
static const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Blue, CRGB::DeepSkyBlue, CRGB::LightSkyBlue};  //* Blue
static const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Green, CRGB::GreenYellow, CRGB::LimeGreen};     //* Green
static const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000,
    0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
    0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,
    0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
static const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, CRGB::Pink, CRGB::DeepPink};        //* Redstatic
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, CRGB::Red, 0xff3c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, 0xff3c00, 0xff7800};                      // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::Indigo, CRGB::DarkBlue};        //* Indigo
static const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::MediumPurple, CRGB::DeepPink}; //* Violet
static const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Orange, CRGB::Gold, CRGB::Goldenrod};           //* Yellow
static const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {
  CRGB::Black,
  CRGB::DarkSlateGray,
  CRGB::DimGray,
  CRGB::LightSlateGray,

  CRGB::DimGray,
  CRGB::DarkSlateGray,
  CRGB::Silver,
  CRGB::DarkCyan,

  CRGB::Lavender,
  CRGB::Silver,
  CRGB::Azure,
  CRGB::LightGrey,

  CRGB::GhostWhite,
  CRGB::Silver,
  CRGB::White,
  CRGB::RoyalBlue
};
static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::OrangeRed, CRGB::Orange, CRGB::Gold};             //* Orange
static const TProgmemRGBPalette16 ZeebraColors_p FL_PROGMEM = {
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black
};

// Добавил "белую" палитру для "Огонь 2012", в самом конце 4-го ползунка, огонь горит белым цветом, - красиво для белой динамической подсветки 
static const TProgmemRGBPalette16 WhiteBlackColors_p FL_PROGMEM = {CRGB::Black, CRGB::Gray, CRGB::White, CRGB::WhiteSmoke};
// ==== Константы для эффектов ====

// Эффекты на базе "3D Noise"
#define NOISE_SCALE_AMP  (58UL)    // амплификатор шкалы (влияет на машстаб "пятен" эффекта, большие пятна выглядят красивее чем куча мелких)
#define NOISE_SCALE_ADD  (8UL)     // корректор шкалы


#define BALLS_AMOUNT           (7U)                 // максимальное количество "шариков"
#define LIGHTERS_AM            (64U)                // светлячки
#define NUM_LAYERS             (1U)                 // The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS2            (2U)                 // The coordinates for 3 16-bit noise spaces.
#define AVAILABLE_BOID_COUNT   (10U)                // стая, кол-во птиц

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
#define e_com_TAILSPEED             (500)         // скорость смещения хвоста
#define e_com_BLUR                  (24U)         // размытие хвоста
#define e_com_3DCOLORSPEED          (5U)          // скорость случайного изменения цвета (0й - режим)

// ------------- светлячки со шлейфом -------------
//#define BALLS_AMOUNT          (7U)                          // максимальное количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define BALL_TRACK            (1U)                          // (0 / 1) - вкл/выкл следы шариков
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)

// ------------- блуждающий кубик -------------
#define RANDOM_COLOR          (1U)                          // случайный цвет при отскоке

// --------------------------- эффект мячики ----------------------
#define bballsMaxNUM_BALLS     (16U)                // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
#define bballsGRAVITY          (-10)                // Downward (negative) acceleration of gravity in m/s^2
#define bballsH0                (2)                 // Starting height, in meters, of the ball (strip length)
#define bballsVImpact0          (sqrt(-2 * bballsGRAVITY * bballsH0))

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

// ============= Fire Effect =================
#define SPARKLES              (1U)                     // вылетающие угольки вкл выкл
#define UNIVERSE_FIRE                                  // универсальный огонь 2-в-1 Цветной+Белый

//these values are substracetd from the generated values to give a shape to the animation
static const uint8_t valueMask[8][16] PROGMEM =
{
  {0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  },
  {0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  },
  {0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  },
  {0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  },
  {32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 },
  {64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 },
  {96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 },
  {128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
static const uint8_t hueMask[8][16] PROGMEM =
{
  {25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25 },
  {25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19 },
  {19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16 },
  {13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13 },
  {11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11 },
  {8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8  },
  {5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5  },
  {1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1  }
};


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
