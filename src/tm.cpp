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

#include "config.h"
#ifdef TM1637
#include "tm.h"

#if TM_SHOW_BANNER
byte welcome_banner[] = {TM_F, TM_i, TM_r, TM_e, TM_empty, TM_empty, TM_S, TM_t, TM_a, TM_r, TM_t};
#endif



void tm_setup() {
  disp.clear();
  disp.brightness(TM_BRIGHTNESS);
#if TM_SHOW_BANNER
  disp.runningString(welcome_banner, sizeof(welcome_banner), 300);  //время в миллисекундах!
#endif
  LOG(printf_P, PSTR("TM1637 was initialized \n"));
}

void tm_loop() {
  static bool showPoints = false;

  const tm* t = localtime(embui.timeProcessor.now());

  disp.point(showPoints);
  showPoints = !showPoints;
  disp.displayClock(t->tm_hour, t->tm_min);
  //if (showPoints) LOG(printf_P, PSTR("TM1637 updated \n"));
}

#endif