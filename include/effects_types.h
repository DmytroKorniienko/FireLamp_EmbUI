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

typedef enum : uint8_t {ST_BASE=0,ST_END, ST_IDX, ST_AB, ST_AB2, ST_MIC} SORT_TYPE; // виды сортировки

static const char PGidxtemplate[] PROGMEM  = "%s{\"nb\":%d,\"fl\":%d}";

typedef enum : uint8_t {
EFF_NONE = (0U),                              // Специальный служебный эффект, не комментировать и индекс не менять константу!
EFF_WHITE_COLOR,                              // Белый свет
EFF_COLORS,                                   // Смена цвета
EFF_RAINBOW_2D,                               // Радуга универсальная
EFF_SPARKLES,                                 // Конфетти
EFF_SNOW,                                     // Снегопад
EFF_SNOWSTORMSTARFALL,                        // Метель + Звездопад
EFF_MATRIX,                                   // Матрица
EFF_LIGHTERS,                                 // Светлячки
EFF_LIGHTER_TRACES,                           // Светлячки со шлейфом
EFF_CUBE,                                     // Блуждающий кубик
EFF_PULSE,                                    // Пульс
EFF_EVERYTHINGFALL,                           // Водо/огне/лава/радуга/хренопад
EFF_FIRE,                                     // Огонь
EFF_PAINTBALL,                                // Пейнтбол
EFF_MADNESS,                                  // Безумие 3D
EFF_CLOUDS,                                   // Облака 3D
EFF_LAVA,                                     // Лава 3D
EFF_PLASMA,                                   // Плазма 3D
EFF_RAINBOW,                                  // Радуга 3D
EFF_RAINBOW_STRIPE,                           // Павлин 3D
EFF_ZEBRA,                                    // Зебра 3D
EFF_FOREST,                                   // Лес 3D
EFF_OCEAN,                                    // Океан 3D
EFF_BBALS,                                    // Прыгающие мячики
EFF_SINUSOID3,                                // Синусоид 3
EFF_METABALLS,                                // Метасферы
EFF_SPIRO,                                    // Спираль
EFF_RAINBOWCOMET,                             // Радужная комета
EFF_RAINBOWCOMET3,                            // 3 кометы
EFF_PRIZMATA,                                 // Призмата
EFF_FLOCK,                                    // Стая и Хищник
EFF_SWIRL,                                    // Водоворот
EFF_DRIFT,                                    // Дрифт
EFF_POPCORN,                                  // Попкорн
EFF_TWINKLES,                                 // Мерцание
EFF_RADAR,                                    // Радар
EFF_WAVES,                                    // Волны
EFF_FIRE2012,                                 // Огонь 2012
EFF_RAIN,                                     // Дождь
EFF_CRAIN,                                    // Дождь за окном...
EFF_DNA,                                      // ДНК
EFF_FIRE2018,                                 // Огонь 2018
EFF_RINGS,                                    // Кодовый замок
EFF_CUBE2,                                    // Куб 2D
EFF_SMOKE,                                    // Дым
EFF_PICASSO,                                  // Пикассо
EFF_PICASSO2,                                 // Пикассо2
EFF_PICASSO3,                                 // Пикассо3
EFF_LEAPERS,                                  // Прыгуны
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
EFF_ARROWS,                                   // Стрелки
EFF_NBALLS,                                   // Дикие шарики
EFF_ATTRACT,                                  // Притяжение
EFF_SNAKE,                                    // Змейки by kDn
EFF_SNAKE2,                                   // Змеиный Остров
EFF_FLINE,                                    // Парящий огонь
EFF_FFIRE,                                    // Северное сияние
EFF_FLSNAKE,                                  // Радужный Змей
EFF_FLOWER,                                   // Цветение
EFF_TEST,                                     //  (Тест)
EFF_TEST2,                                    // Пока пусто
EFF_PICASSO4,                                 // Меташары
EFF_LIQUIDLAMP,                               // Лаваламп
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
  TEFF_064, TEFF_065, TEFF_066, TEFF_067, TEFF_068, TEFF_069, TEFF_070, TEFF_071, TEFF_072, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 64 - 79
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
  1, 2, 2, 2, 2, 1, 5, 1, 5, 5, 3, 4, 1, 5, 3, 2, // 0-15
  2, 2, 2, 2, 2, 2, 2, 2, 5, 1, 1, 5, 3, 1, 3, 3, // 16-31
  1, 3, 3, 3, 3, 3, 6, 5, 7, 5, 2, 1, 5, 5, 1, 1, // 32 - 47
  3, 4, 4, 4, 2, 5, 3, 2, 3, 3, 4, 7, 3, 3, 5, 2, // 48 - 63
  4, 1, 1, 1, 2, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, // 64 - 79
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
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 4, // 240 - 255
};


// младшие 4 бита отдаются на выбор контрола, что даст 16 различных типов контроллов
// старшие 4 бита используются как набор управляющих кодов, к примеру - отображать при включенном микрофоне, при выключенном и т.д., тоже 16 вариантов
typedef enum : uint8_t {ALWAYS=0,ISMICON,ISMICOFF,HIDE} CONTROL_CASE; // старшие 4 бита
typedef enum : uint8_t {RANGE=0,EDIT,CHECKBOX} CONTROL_TYPE; // младшие 4 бита

/**
 * Набор статических строк, определяющих UI эффекта если он отличается от базового
 *
 *  GUI для эффекта по-умолчанию
 * Полный формат для пользовательского (id=3...7) параметра имеет вид: {\"id\":3,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр\"}
 * @nb@ - будет заменен на реальный номер эффекта, @name@ - на дефолтное имя эффекта
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48813
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48848
 */

// Дефолтные контролы
static const char E_DEFUI[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"}]}";
// Добавил ко всем конфигам чекбокс "Микрофон" с индексом 7. Ввиду модифированного поведения функции myLamp.isMicOnOff(), он будет появляться только если глобально
// микрофон включен, и эффект обладает зависимостью от микрофона в принципе.
static const char E_DEFMICUI[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
// Общая конфигурация для эффектов с 3-им ползунком для палитр
static const char E_3PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_3PAL_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
// Общая конфигурация для эффектов только с 2-мя ползунками "Яркость" и "Скорость", пример - эффект"Тихий Океан"
static const char E_2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}]}"; // 3*16+0 для 2 контрола
static const char E_2_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}"; // 3*16+0 для 2 контрола

// Общая конфигурация для эффектов с 4-им ползунком для палитр
static const char E_4PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}]}";

static const char E_SPARCLES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":16,\"val\":1,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Цвет\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_LIGHTERS[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Количество\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_RADAR[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_WHITE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Оттенок\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_LEAPERS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":50,\"step\":1,\"name\":\"Жогово!\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_BUTTERFLY_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":64,\"step\":1,\"name\":\"Количество / Цвет(ночнник)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Порхание\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Ночник\"}]}";
static const char E_PULS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_BBALLS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Масштаб\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Кольца\"}]}";
static const char E_PRIZMATA_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Размытие\"}]}";
static const char E_AQUARIUM[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Насыщенность\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":0,\"max\":15,\"step\":1,\"name\":\"Блики (0:Откл. 1:Случ.)\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_FREQ_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Усиление\"}, {\"id\":3,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_OSC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Развертка\"},{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Пульсация\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Уcиление\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
// размерность ползунка "Узор" должна быть MAX_PATTERN + 1 (patterns.h). При добавлении паттернов - менять и тут.
static const char E_PATT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Скорость (Верт.)\"}, {\"id\":2,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Скорость (Гориз.)\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":39,\"step\":1,\"name\":\"Узор (1 = случайный)\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_SHAD_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Скорость\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Затенение\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Линейная скорость\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_ARR_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Режим (1 = случайный)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_F2018_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Оттенок\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Линейная скорость\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_NBAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":25,\"step\":1,\"name\":\"Смещение\"}]}";
static const char E_ATTRACT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Энергия\"}, {\"id\":3,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Масса\"}, {\"id\":4,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_FLOCK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Хищник\"}]}";
static const char E_WAVES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Цвета / Количество\"}]}";
static const char E_MUNCH_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":0,\"max\":8,\"step\":1,\"name\":\"Сдвиг цвета (0 = случайный)\"} ,{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_COM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет (1-127 случайный)\"}]}";
static const char E_DRIFT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":4,\"step\":1,\"name\":\"Тип 1/2, Размытие 3/4\"}]}";
static const char E_SMOKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет/Вращение\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Заполнение\"}]}";
static const char E_CUBE2D_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":0,\"max\":22,\"step\":1,\"name\":\"Палитра (0 = случайный цвет)\"},{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Размер (ширина)\"},{\"id\":4,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Размер (высота)\"} ,{\"id\":5,\"type\":2,\"val\":\"false\",\"name\":\"Классика\"}]}";
static const char E_FLAMP_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет\"}]}";
static const char E_LIGHT2[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Размер/Цвет\"}]}";
static const char E_CUBE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Размер/Шлейф(1-85-170-255)\"}]}";
static const char E_STARFAIL[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"} ,{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Снег/Звезды\"} ,{\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Метеоры\"}]}"; // 3*16+0 для 2 контрола
static const char E_SNAKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Кол-во\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_SNAKE2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":2,\"val\":\"false\",\"name\":\"Цвета Диско\"}, {\"id\":4,\"type\":2,\"val\":\"false\",\"name\":\"Субпиксель\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_FWORK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Частота запусков\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Количество снарядов\"}, {\"id\":3,\"type\":2,\"val\":\"false\",\"name\":\"Вспышки\"}]}";
static const char E_CRAIN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет (1 = случайный, 255 = белый)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Тучка\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Шторм\"}]}";
static const char E_RAIN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Тучка\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Шторм\"}]}";
static const char E_FIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Вид задержки (текущая/dryrun/delay)\"}]}";
static const char E_NFIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":50,\"min\":1,\"max\":100,\"step\":1,\"name\":\"Масштаб\"}, {\"id\":3,\"type\":0,\"val\":8,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_TEST[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Количество\"}]}";
static const char E_POPCORN[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":0,\"val\":8,\"min\":1,\"max\":21,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Размытие\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Обратные Цвета\"}]}";
static const char E_MBL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":21,\"step\":1,\"name\":\"Палитра (0 = генератор)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"}]}";
static const char E_LIQLAM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":18,\"step\":1,\"name\":\"Палитра (0 = генератор)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"}, {\"id\":5,\"type\":0,\"val\":0,\"min\":0,\"max\":4,\"step\":1,\"name\":\"Фильтр\"}, {\"id\":6,\"type\":2,\"val\":\"true\",\"name\":\"Физика\"}]}";
static const char E_F2012_MIC[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":64,\"min\":1,\"max\":128,\"step\":1,\"name\":\"Масштаб\"},{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Палитра\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";

// Инженерный
static const char E_TEST_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":8,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 1\"}, {\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 2\"}, {\"id\":6,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 3\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";

/** набор указателей на строки с UI-конфигом для эффектов по-умолчанию
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов или эффектов с дефолтными ползунками ставим указатель на базовый конфиг
 * не обязательно каждому эффекту лепить свой конфиг, можно ставить указатели на эффекты с похожими контролами
 */
static const char* const T_EFFUICFG[] PROGMEM = {
  E_DEFUI, E_WHITE_CFG, E_DEFMICUI, E_DEFMICUI, E_SPARCLES_CFG, E_DEFUI, E_STARFAIL, E_DEFUI, E_LIGHTERS, E_LIGHT2, E_CUBE_CFG, E_PULS_CFG, E_DEFUI, E_NFIRE_CFG, E_2_CFG, E_DEFMICUI, // 0-15
  E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_DEFMICUI, E_BBALLS_CFG, E_DEFUI, E_DEFUI, E_3PAL_CFG, E_COM_CFG, E_DEFUI, E_PRIZMATA_CFG, E_FLOCK_CFG, // 16-31
  E_DEFUI, E_DRIFT_CFG, E_POPCORN, E_3PAL_CFG, E_RADAR, E_WAVES_CFG, E_F2012_MIC, E_RAIN_CFG, E_CRAIN_CFG, E_2_CFG, E_F2018_CFG, E_DEFUI, E_CUBE2D_CFG, E_SMOKE_CFG, E_DEFUI, E_DEFUI, // 32 - 47
  E_DEFUI, E_LEAPERS_CFG, E_3PAL_MIC_CFG, E_3PAL_MIC_CFG, E_AQUARIUM, E_FWORK_CFG, E_2_CFG, E_MUNCH_CFG, E_3PAL_CFG, E_BUTTERFLY_CFG, E_SHAD_CFG, E_PATT_CFG, E_ARR_CFG, E_NBAL_CFG, E_ATTRACT_CFG, E_SNAKE_CFG, // 48 - 63
  E_SNAKE2_CFG, E_DEFUI, E_DEFUI, E_DEFUI, E_2_MIC_CFG, E_TEST, E_TEST_CFG, E_MBL_CFG, E_LIQLAM_CFG, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, // 64 - 79
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
  E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_DEFUI, E_3PAL_CFG, E_FREQ_CFG, E_OSC_CFG  // 240 - 255
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
#define FASTLED_PALETTS_COUNT 21

/**
 * Набор палитр в дополнение к тем что идут с FastLED
 * новые палитры добавляем в алфавитном порядке
 */
static const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
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
// stepko где-то нашел пару палитр
static const TProgmemRGBPalette16 AutumnColors_p FL_PROGMEM = {0xfa9422, 0xf8cc73, 0xa16456, 0xd6bcc4, 0x2b2c54};
static const TProgmemRGBPalette16 EveningColors_p FL_PROGMEM = {0xdf0450, 0xfc7ca9, 0x900562, 0xad4c8c, 0xc2b2b5, 0x312f54};
static const TProgmemRGBPalette16 StepkosColors_p FL_PROGMEM = {0xc83160, 0x8c2682, 0x5b0bac, 0x2d00ff, 0x0012d8, 0x0000ac, 0x03006b};
static const TProgmemRGBPalette16 VioletColors_p FL_PROGMEM = {0xce9bdd, 0x84349b, 0x642474, 0x612672, 0x040404};
/*


static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::OrangeRed, CRGB::Orange, CRGB::Gold};
static const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Blue, CRGB::DeepSkyBlue, CRGB::LightSkyBlue};  // Blue
static const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Green, CRGB::GreenYellow, CRGB::LimeGreen};     // Green
static const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, CRGB::Pink, CRGB::DeepPink};        // Redstatic
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, CRGB::Red, 0xff3c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, 0xff3c00, 0xff7800};                      // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::Indigo, CRGB::DarkBlue};        // Indigo
static const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::MediumPurple, CRGB::DeepPink}; // Violet
static const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Orange, CRGB::Gold, CRGB::Goldenrod};           // Yellow
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
*/


// Еще одно представление высоты и ширины матрицы, часто встречаеться последнее время
#define LED_ROWS HEIGHT
#define LED_COLS WIDTH

// И еще одно представление высоты и ширины матрицы, часто встречаеться последнее время
#define NUM_ROWS HEIGHT
#define NUM_COLS WIDTH

// ==== Константы для эффектов ====

// Эффекты на базе "3D Noise"
#define NOISE_SCALE_AMP  (58UL)    // амплификатор шкалы (влияет на машстаб "пятен" эффекта, большие пятна выглядят красивее чем куча мелких)
#define NOISE_SCALE_ADD  (8UL)     // корректор шкалы


#define BALLS_AMOUNT           (7U)                 // максимальное количество "шариков"
#define LIGHTERS_AM            (32U)                // светлячки
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
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)

// ------------- блуждающий кубик -------------
#define RANDOM_COLOR          (1U)                          // случайный цвет при отскоке

// --------------------------- эффект мячики ----------------------
#define bballsMaxNUM_BALLS     (WIDTH)                // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
#define bballsGRAVITY          (-9.7f)              // Downward (negative) acceleration of gravity in m/s^2
#define bballsH0               (2)                 // Starting height, in meters, of the ball (strip length)
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

// ----------- Эффект "Аквариум" (с) SottNick
static const byte aquariumGIF[32][32] PROGMEM =
{
    {0x00, 0x00, 0x00, 0x00, 0x34, 0x8f, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x89, 0xe1, 0x77, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x37, 0x9b, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xdd, 0x77, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x37, 0x94, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x88, 0x78, 0x7a, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x4a, 0x71, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x17, 0x38, 0x87, 0x62, 0x00, 0x21, 0x67, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x77, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x20, 0x1b, 0x19, 0x46, 0x2e, 0x00, 0x00, 0x3e, 0x73, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x02, 0x55, 0xc7, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x1f, 0x07, 0x00, 0x00, 0x02, 0x14, 0x00, 0x00, 0x1a, 0xa6, 0x8f, 0x4c, 0x22, 0x04, 0x00, 0x00, 0x00},
    {0x0d, 0x20, 0x62, 0xb3, 0xc8, 0x97, 0x47, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x47, 0x74, 0x6a, 0x70, 0x77, 0x6c, 0x4d, 0x29, 0x11},
    {0x7b, 0x95, 0x9b, 0x60, 0x31, 0x42, 0x61, 0x67, 0x5d, 0x28, 0x00, 0x00, 0x02, 0x23, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x59, 0x4a, 0x3e, 0x0e, 0x01, 0x0a, 0x27, 0x4d, 0x6a, 0x79, 0x7c},
    {0xc5, 0xb1, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x69, 0x82, 0x44, 0x0f, 0x2e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0xb6, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x25, 0x6c},
    {0x82, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x82, 0x83, 0x79, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xc2, 0x83, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e},
    {0x5e, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xab, 0xec, 0x8f, 0x31, 0x21, 0x28, 0x39, 0x59, 0xa2, 0xc4, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x5d, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0xed, 0xb9, 0x7d, 0x6e, 0x6d, 0x65, 0x55, 0x43, 0x66, 0x5e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x5c, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x99, 0x3d, 0x1d, 0x18, 0x10, 0x02, 0x00, 0x00, 0x13, 0x5a, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x56, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x83, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x4d, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x98, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x41, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xb4, 0x97, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x54, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x31, 0x61, 0x07, 0x00, 0x00, 0x00, 0x00, 0x12, 0x5b, 0x90, 0x77, 0x60, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x38, 0x78, 0x2d, 0x00, 0x00, 0x0b, 0x43, 0x7e, 0x79, 0x35, 0x06, 0x0d, 0x35, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x54, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x70, 0xc3, 0x96, 0x48, 0x4d, 0x79, 0x81, 0x4c, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x2f, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x7b, 0x54, 0x2f, 0x27, 0x22, 0x1f, 0x30},
    {0xbf, 0xc9, 0xa6, 0xad, 0xae, 0x72, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x33, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0xa3, 0xac, 0x81, 0x7a, 0x78, 0x78, 0x8e},
    {0x92, 0x5a, 0x22, 0x5b, 0x83, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x35, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x74, 0x71, 0x3a, 0x27, 0x27, 0x2d, 0x37, 0x61},
    {0x46, 0x18, 0x00, 0x00, 0x4d, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x34, 0x07, 0x00, 0x00, 0x00, 0x00, 0x26, 0x62, 0x51, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10},
    {0x2f, 0x09, 0x00, 0x00, 0x32, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x33, 0x03, 0x00, 0x04, 0x30, 0x50, 0x3a, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    {0x2f, 0x0d, 0x00, 0x00, 0x1e, 0x48, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x3b, 0x22, 0x3d, 0x39, 0x1e, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03},
    {0x46, 0x1f, 0x00, 0x00, 0x0f, 0x4f, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x65, 0x6c, 0x2f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    {0x74, 0x5f, 0x10, 0x00, 0x16, 0x69, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19},
    {0x4b, 0x42, 0x3a, 0x39, 0x5b, 0x91, 0x6f, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x5b, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x4e},
    {0x00, 0x00, 0x09, 0x2e, 0x79, 0x81, 0x75, 0x72, 0x53, 0x43, 0x3d, 0x37, 0x2f, 0x29, 0x24, 0x1e, 0x20, 0x4e, 0x7b, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x2f, 0x3f, 0x1f},
    {0x00, 0x00, 0x00, 0x00, 0x37, 0x65, 0x60, 0x81, 0x72, 0x67, 0x69, 0x6b, 0x6d, 0x6f, 0x6f, 0x6c, 0x71, 0x98, 0xb2, 0x55, 0x17, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3e, 0x30, 0x06, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x11, 0x6c, 0x75, 0x32, 0x0a, 0x08, 0x0a, 0x0d, 0x12, 0x18, 0x1f, 0x24, 0x2d, 0x4a, 0x8e, 0x93, 0x4a, 0x16, 0x00, 0x00, 0x00, 0x16, 0x36, 0x39, 0x16, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x0e, 0x7d, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x88, 0x9c, 0x45, 0x11, 0x18, 0x32, 0x3a, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x1f, 0x84, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0xb1, 0xa5, 0x60, 0x46, 0x26, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

#define CAUSTICS_BR                     (100U)                // яркость бликов в процентах (от чистого белого света)

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
#define MAX_SNAKES    (16U) // 16 хватит всем :)

// Gamma corection
static const uint8_t PROGMEM gamma8[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
        1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
        2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
        5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
       10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
       17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
       25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
       37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
       51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
       69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
       90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
      115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
      144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
      177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
      215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
