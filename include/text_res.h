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

const char T_SPARKLES[] PROGMEM = "Конфетти";
const char T_FIRE[] PROGMEM = "Огненная лампа";
const char T_EVERYTHINGFALL[] PROGMEM = "Эффектопад";
const char T_RAINBOW_2D[] PROGMEM = "Радуга 2D";
const char T_COLORS[] PROGMEM = "Цвета";
const char T_PULSE[] PROGMEM = "Пульс";
const char T_MATRIX[] PROGMEM = "Матрица";
const char T_SNOW[] PROGMEM = "Снегопад";
const char T_SNOWSTORMSTARFALL[] PROGMEM = "Метель + Звездопад";
const char T_LIGHTERS[] PROGMEM = "Светлячки";
const char T_LIGHTER_TRACES[] PROGMEM = "Светлячки со шлейфом";
const char T_PAINTBALL[] PROGMEM = "Пейнтбол";
const char T_CUBE[] PROGMEM = "Блуждающий кубик";
const char T_BBALS[] PROGMEM = "Прыгающие мячики";
const char T_MADNESS[] PROGMEM = "Безумие 3D";
const char T_RAINBOW[] PROGMEM = "Радуга 3D";
const char T_RAINBOW_STRIPE[] PROGMEM = "Павлин 3D";
const char T_ZEBRA[] PROGMEM = "Зебра 3D";
const char T_FOREST[] PROGMEM = "Лес 3D";
const char T_OCEAN[] PROGMEM = "Океан 3D";
const char T_PLASMA[] PROGMEM = "Плазма 3D";
const char T_CLOUDS[] PROGMEM = "Облака 3D";
const char T_LAVA[] PROGMEM = "Лава 3D";
const char T_SINUSOID3[] PROGMEM = "Синусоид 3";
const char T_METABALLS[] PROGMEM = "Метасферы";
const char T_SPIRO[] PROGMEM = "Спираль";
const char T_RAINBOWCOMET[] PROGMEM = "Радужная комета";
const char T_RAINBOWCOMET3[] PROGMEM = "Три кометы";
const char T_WHITE_COLOR[] PROGMEM = "Белая лампа";
const char T_PRIZMATA[] PROGMEM = "Призмата";
const char T_FLOCK[] PROGMEM = "Стая";
const char T_SWIRL[] PROGMEM = "Водоворот";
const char T_DRIFT[] PROGMEM = "Дрифт";
const char T_DRIFT2[] PROGMEM = "Дрифт 2";
const char T_TWINKLES[] PROGMEM = "Мерцание";
const char T_RADAR[] PROGMEM = "Радар";
const char T_WAVES[] PROGMEM = "Волны";
const char T_FIRE2012[] PROGMEM = "Огонь 2012";
const char T_RAIN[] PROGMEM = "Дождь";
const char T_COLORRAIN[] PROGMEM = "Цветной дождь";
const char T_STORMYRAIN[] PROGMEM = "Тучка в банке";
const char T_FIRE2018[] PROGMEM = "Огонь 2018";
const char T_RINGS[] PROGMEM = "Кодовый замок";
const char T_CUBE2[] PROGMEM = "Куб 2D";
const char T_SMOKE[] PROGMEM = "Дым";
const char T_PICASSO[] PROGMEM = "Пикассо";
const char T_PICASSO2[] PROGMEM = "Пикассо2";
const char T_PICASSO3[] PROGMEM = "Пикассо3";
const char T_LEAPERS[] PROGMEM = "Акробаты";
const char T_TIME[] PROGMEM = "Часы";

#ifdef MIC_EFFECTS
const char T_FREQ[] PROGMEM = "Частотный анализатор";
#endif