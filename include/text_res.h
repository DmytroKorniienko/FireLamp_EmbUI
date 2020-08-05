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
#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>

// набор строк с именами эффектов
static const char TEFF_000[] PROGMEM = "";  // "пустышка"
static const char TEFF_001[] PROGMEM = "Белая лампа";
static const char TEFF_002[] PROGMEM = "Цвета";
static const char TEFF_003[] PROGMEM = "Радуга 2D";
static const char TEFF_004[] PROGMEM = "Конфетти";
static const char TEFF_005[] PROGMEM = "Снегопад";
static const char TEFF_006[] PROGMEM = "Метель + Звездопад";
static const char TEFF_007[] PROGMEM = "Матрица";
static const char TEFF_008[] PROGMEM = "Светлячки";
static const char TEFF_009[] PROGMEM = "Светлячки со шлейфом";
static const char TEFF_010[] PROGMEM = "Блуждающий кубик";
static const char TEFF_011[] PROGMEM = "Пульс";
static const char TEFF_012[] PROGMEM = "Эффектопад";
static const char TEFF_013[] PROGMEM = "Огненная лампа";
static const char TEFF_014[] PROGMEM = "Пейнтбол";
static const char TEFF_015[] PROGMEM = "Безумие 3D";
static const char TEFF_016[] PROGMEM = "Облака 3D";
static const char TEFF_017[] PROGMEM = "Лава 3D";
static const char TEFF_018[] PROGMEM = "Плазма 3D";
static const char TEFF_019[] PROGMEM = "Радуга 3D";
static const char TEFF_020[] PROGMEM = "Павлин 3D";
static const char TEFF_021[] PROGMEM = "Зебра 3D";
static const char TEFF_022[] PROGMEM = "Лес 3D";
static const char TEFF_023[] PROGMEM = "Океан 3D";
static const char TEFF_024[] PROGMEM = "Прыгающие мячики";
static const char TEFF_025[] PROGMEM = "Синусоид 3";
static const char TEFF_026[] PROGMEM = "Метасферы";
static const char TEFF_027[] PROGMEM = "Спираль";
static const char TEFF_028[] PROGMEM = "Радужная комета";
static const char TEFF_029[] PROGMEM = "Три кометы";
static const char TEFF_030[] PROGMEM = "Призмата";
static const char TEFF_031[] PROGMEM = "Стая";
static const char TEFF_032[] PROGMEM = "Водоворот";
static const char TEFF_033[] PROGMEM = "Дрифт";
static const char TEFF_034[] PROGMEM = "Дрифт 2";
static const char TEFF_035[] PROGMEM = "Мерцание";
static const char TEFF_036[] PROGMEM = "Радар";
static const char TEFF_037[] PROGMEM = "Волны";
static const char TEFF_038[] PROGMEM = "Огонь 2012";
static const char TEFF_039[] PROGMEM = "Дождь";
static const char TEFF_040[] PROGMEM = "Цветной дождь";
static const char TEFF_041[] PROGMEM = "Тучка в банке";
static const char TEFF_042[] PROGMEM = "Огонь 2018";
static const char TEFF_043[] PROGMEM = "Кодовый замок";
static const char TEFF_044[] PROGMEM = "Куб 2D";
static const char TEFF_045[] PROGMEM = "Дым";
static const char TEFF_046[] PROGMEM = "Пикассо";
static const char TEFF_047[] PROGMEM = "Пикассо2";
static const char TEFF_048[] PROGMEM = "Пикассо3";
static const char TEFF_049[] PROGMEM = "Прыгуны"; // Автор сказал прыгуны, значит по дефолту будут прыгуны :)
static const char TEFF_253[] PROGMEM = "Часы";
static const char TEFF_254[] PROGMEM = "Частотный анализатор";

/* набор указателей на строки с именами эффектов
 * индекс элемента массива составляет id из EFF_ENUM
 * вместо несуществующих эффектов ставим указатель на 0-ю пустышку
 */
static const char* const T_EFFNAMEID[] PROGMEM = { 
  TEFF_000, TEFF_001, TEFF_002, TEFF_003, TEFF_004, TEFF_005, TEFF_006, TEFF_007, TEFF_008, TEFF_009, TEFF_010, TEFF_011, TEFF_012, TEFF_013, TEFF_014, TEFF_015, // 0-15
  TEFF_016, TEFF_017, TEFF_018, TEFF_019, TEFF_020, TEFF_021, TEFF_022, TEFF_023, TEFF_024, TEFF_025, TEFF_026, TEFF_027, TEFF_028, TEFF_029, TEFF_030, TEFF_031, // 16-31
  TEFF_032, TEFF_033, TEFF_034, TEFF_035, TEFF_036, TEFF_037, TEFF_038, TEFF_039, TEFF_040, TEFF_041, TEFF_042, TEFF_043, TEFF_044, TEFF_045, TEFF_046, TEFF_047, // 32 - 47
  TEFF_048, TEFF_049, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 48 - 63
  TEFF_000, TEFF_049, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, TEFF_000, // 64 - 79
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
