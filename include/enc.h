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

#ifndef _ENC_H
#define _ENC_H

#include "config.h"

#ifdef ENCODER
#include "misc.h"
#include "main.h"
#include "tm.h"
#include "interface.h"
#include "effects.h"
#include "ui.h"

// Опциональные настройки (показаны по умолчанию)
//#define EB_FAST 30     // таймаут быстрого поворота, мс
//#define EB_DEB 80      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

#include "EncButton.h"

static EncButton<EB_CALLBACK, DT, CLK, SW> enc;   // энкодер с кнопкой <A, B, KEY>

#ifndef EXIT_TIMEOUT
#define EXIT_TIMEOUT 3U
#endif

#ifndef ENC_STRING_DELAY
#define ENC_STRING_DELAY 30
#endif

#ifndef ENC_STRING_EFFNUM_DELAY
#define ENC_STRING_EFFNUM_DELAY 17
#endif

#ifndef ENC_STRING_COLOR
#define ENC_STRING_COLOR CRGB::Orange
#endif

#ifndef VERTGAUGE_COLOR
#define VERTGAUGE_COLOR 10
#endif

void callEncTick ();
void IRAM_ATTR isrEnc();
void interrupt();
void noInterrupt();

void isTurn();
void isClick();
void isHolded();
//void myStep();
void encSetBri(int val);
void encSetEffect(int val);
void encSetDynCtrl(int val);
void encDisplay(uint16_t value, String type = "");
void encDisplay(float value);
void encDisplay(String str);
void resetTimers();
void exitSettings();
void encSendString(String str, CRGB color, bool force = true, uint8_t delay = ENC_STRING_DELAY);
void encSendStringNumEff(String str, CRGB color);
bool validControl(const CONTROL_TYPE ctrlCaseType);

void enc_setup(); 
extern void encLoop();

void toggleDemo();
void toggleGBright();
void toggleMic();
void toggleAUX();
void sendTime();
void sendIP();
#endif
#endif