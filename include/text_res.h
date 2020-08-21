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

// Здесь строковые константы общего назначения, включая константы из интерфейса, что позволит локализировать при необходимости
// Ничего иного в данном файле не предполагается

/** набор строк с именами эффектов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
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
static const char TEFF_049[] PROGMEM = "Прыгуны";
static const char TEFF_050[] PROGMEM = "Вихри";
static const char TEFF_051[] PROGMEM = "Звезды";
static const char TEFF_052[] PROGMEM = "Блики на воде, цвета";
static const char TEFF_053[] PROGMEM = "Фейерверк";
static const char TEFF_054[] PROGMEM = "Тихий Океан";
static const char TEFF_055[] PROGMEM = "Вышиванка";
static const char TEFF_056[] PROGMEM = "Цветной шум";
static const char TEFF_057[] PROGMEM = "2D Анимация (Тест. будет удален)";
static const char TEFF_253[] PROGMEM = "Часы";
static const char TEFF_254[] PROGMEM = "Частотный анализатор";
static const char TEFF_255[] PROGMEM = "Осциллограф";
