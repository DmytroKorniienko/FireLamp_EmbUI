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
#define EB_FAST 65     // таймаут быстрого поворота, мс 30
//#define EB_DEB 80      // дебаунс кнопки, мс
//#define EB_HOLD 1000   // таймаут удержания кнопки, мс
//#define EB_STEP 500    // период срабатывания степ, мс
//#define EB_CLICK 400   // таймаут накликивания, мс

#include "EncButton.h"

static EncButton<EB_CALLBACK, DT, CLK, SW> enc;   // энкодер с кнопкой <A, B, KEY>

#ifndef EXIT_TIMEOUT
#define EXIT_TIMEOUT 3U
#endif

#ifndef ENC_STRING_EFFNUM_DELAY
#define ENC_STRING_EFFNUM_DELAY 17
#endif

void callEncTick ();
//void IRAM_ATTR isrEnc();
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
void encSendString(String str, CRGB color, bool force = true, uint8_t delay = 40U);
void encSendStringNumEff(String str, CRGB color);
bool validControl(const CONTROL_TYPE ctrlCaseType);

void enc_setup(); 
extern void encLoop();
uint8_t getEncTxtDelay();
void setEncTxtDelay(const uint8_t speed);
CRGB getEncTxtColor();
void setEncTxtColor(const CRGB color);

void toggleDemo();
void toggleGBright();
void toggleMic();
void toggleAUX();
void sendTime();
void sendIP();
#endif
#endif