/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
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
#include "main.h"

// Опциональные настройки (показаны по умолчанию)
#define EB_FAST 65 // таймаут быстрого поворота, мс 30
// #define EB_DEB 80      // дебаунс кнопки, мс
// #define EB_HOLD 1000   // таймаут удержания кнопки, мс
// #define EB_STEP 500    // период срабатывания степ, мс
// #define EB_CLICK 400   // таймаут накликивания, мс

#include "EncButton.h"

class Encoder : public EncButton<EB_TICK, DT, CLK, SW>
{
public:
  Encoder() : EncButton() {}
  void init();
  void handle();
  uint8_t getEncTxtDelay() { return txtDelay; }
  void setTxtDelay(const uint8_t speed) { txtDelay = speed; }
  CRGB getTxtColor() { return txtColor; }
  void setTxtColor(const CRGB color) { txtColor = color; }
  void exitSettings();

private:
  enum ENC_ACTION
  {
    WAIT,
    SET_BRIGHT,
    SET_EFFECT,
    SET_CONTROL,
  } currAction = WAIT; // идент текущей операции: 0 - ничего, 1 - крутим яркость, 2 - меняем эффекты, 3 - меняем динамические контролы

  enum EncState
  {
    NONE,
    RIGHT,
    LEFT,
    RIGHT_HOLD,
    LEFT_HOLD,
    CLICK,
    HOLD,
    STEP
  };

  void turn(EncState turnType);
  void click();
  void hold();
  void setBri(int val);
  void setEffect(int val);
  void setDynCtrl(int val);
  void display(uint16_t value, String type = "");
  void display(String str);
  void resetTimers();
  void sendString(String str, CRGB color, bool force = true, uint8_t delay = 40U);
  void sendStringNumEff(String str, CRGB color);
  bool validControl(const CONTROL_TYPE ctrlCaseType);
  void myClicks();

  void toggleDemo();
  void toggleGBright();
  void toggleMic();
  void toggleAUX();
  void sendTime();
  void sendIP();

  static void IRAM_ATTR isrEnc();
  void interruptEn();
  void interruptOff();

  #if LAMP_DEBUG == 1
  static const char* getStateName(EncState state);
  #endif

  uint8_t speed = 0U, fade = 0U;
  uint8_t txtDelay = 40U;
  uint8_t currDynCtrl = 0U; // текущий контрол, с которым работаем
  uint8_t loops = 0U;       // счетчик псевдотаймера
  bool done = false;        // true == все отложенные до enc_loop операции выполнены.
  bool inSettings = false;  // флаг - мы в настройках эффекта
  uint16_t currEffNum = 0U; // текущий номер эффекта
  uint16_t anyValue = 0U;   // просто любое значение, которое крутим прямо сейчас, очищается в enc_loop

  CRGB txtColor = CRGB::Orange;
  CRGB gaugeCol = CRGB::Orange;
};

extern Encoder enc; // encoder with button <A, B, KEY>

#endif
#endif