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
#ifdef ENCODER
#include "enc.h"

void callEncTick () {
  enc.tick();
}


void IRAM_ATTR isrEnc() {
  enc.tick();  // отработка в прерывании
}

void myTurn() {
  Serial.print(F("turn "));
  Serial.println(enc.counter);    // вывод счётчика

  // также можно опросить здесь (isRight, isLeft, isRightH, isLeftH)
    if (enc.isLeft()) {
        if (enc.isFast()) Serial.println(F("Fast left"));
        else Serial.println(F("Left"));
    } else if (enc.isLeftH()) {
        if (enc.isFast()) Serial.println(F("Fast left hold"));
        else Serial.println(F("Hold left"));
    } else if (enc.isRight()) {
        if (enc.isFast()) Serial.println(F("Fast right"));
        else Serial.println(F("Right"));
    } else if (enc.isRightH()) {
        if (enc.isFast()) Serial.println(F("Fast right hold"));
        else Serial.println(F("Hold right"));
    }
}

void myClick() {
    Serial.println(F("Click"));
}

void myHolded() {
    Serial.println(F("Holded"));
}

void myStep() {
    Serial.println(F("Step"));
}

void myClicks() {
    Serial.print(F("Click: "));
    Serial.println(enc.clicks);
}

void fiveClicks() {
  Serial.println(F("Enc: 5 clicks! What may I to do?"));
}

void enc_setup() {
  attachInterrupt(digitalPinToInterrupt(DT), isrEnc, CHANGE);   // прерывание на DT пине
  attachInterrupt(digitalPinToInterrupt(CLK), isrEnc, CHANGE);  // прерывание на CLK пине
  attachInterrupt(digitalPinToInterrupt(SW), isrEnc, CHANGE);   // прерывание на SW пине
  enc.counter = 100;      // изменение счётчика
  enc.attach(TURN_HANDLER, myTurn);
  enc.attach(CLICK_HANDLER, myClick);
  enc.attach(HOLDED_HANDLER, myHolded);
  enc.attach(STEP_HANDLER, myStep);
  enc.attach(CLICKS_HANDLER, myClicks);
  enc.attachClicks(5, fiveClicks);
}

Task encTask(30 * TASK_MILLISECOND, TASK_FOREVER, &callEncTick, &ts, true);

#endif