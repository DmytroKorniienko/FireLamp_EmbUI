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

//#pragma once
#ifndef __INTERFACE_H
#define __INTERFACE_H

typedef enum _remote_action {
    RA_UNKNOWN,
    RA_ON,
    RA_OFF,
    RA_DEMO,
    RA_DEMO_NEXT,
    RA_ALARM,
    RA_ALARM_OFF,
    RA_LAMP_CONFIG,
    RA_EFF_CONFIG,
#ifdef ESP_USE_BUTTON
    RA_BUTTONS_CONFIG,
#endif
    RA_EVENTS_CONFIG,
#ifdef AUX_PIN
    RA_AUX_ON,
    RA_AUX_OFF,
    RA_AUX_TOGLE,
#endif
#ifdef OTA
    RA_OTA,
#endif
    RA_REBOOT,
    RA_EFF_NEXT,
    RA_EFF_PREV,
    RA_EFF_RAND,
    RA_BRIGHT_NF,
    RA_CONTROL,
    RA_MIC,
#ifdef MP3PLAYER
    RA_MP3_PREV,
    RA_MP3_NEXT,
    RA_MP3_SOUND,
    RA_PLAYERONOFF,
#endif
#ifdef MIC_EFFECTS
    RA_MICONOFF,
#endif
    RA_EFFECT,
    RA_SEND_TEXT,
    RA_SEND_TIME,
    RA_SEND_IP,
    RA_WHITE_HI,
    RA_WHITE_LO,
    RA_WIFI_REC,
    RA_GLOBAL_BRIGHT,
    RA_WARNING,
    RA_DRAW,
    RA_FILLMATRIX,
    RA_RGB,
    RA_MODECMD,
    RA_WHITE_CUR
} RA;

void remote_action(RA action, ...);
String httpCallback(const String &param, const String &value, bool isset);
#ifdef ESP_USE_BUTTON
void default_buttons();
#endif

// ---------------------

void section_main_frame(Interface *interf, JsonObject *data);
void section_effects_frame(Interface *interf, JsonObject *data);
void section_text_frame(Interface *interf, JsonObject *data);
// реализация настроек тут своя, отличная от фреймворка
void section_settings_frame(Interface *interf, JsonObject *data);
void pubCallback(Interface *interf);
void set_onflag(Interface *interf, JsonObject *data);
void save_lamp_flags();

uint8_t uploadProgress(size_t len, size_t total);

void delayedcall_show_effects();

// Вывод значка микрофона в списке эффектов
#ifdef MIC_EFFECTS
    #define MIC_SYMBOL (micSymb ? (pgm_read_byte(T_EFFVER + (uint8_t)eff->eff_nb) % 2 == 0 ? " \U0001F399" : "") : "")
    #define MIC_SYMB bool micSymb = myLamp.getLampSettings().effHasMic
#else
    #define MIC_SYMBOL ""
    #define MIC_SYMB
#endif

// Вывод номеров эффектов в списке, в WebUI
//#define EFF_NUMBER (numList ? (String(eff->eff_nb) + ". ") : "")
#define EFF_NUMBER   (numList ? (eff->eff_nb <= 255 ? (String(eff->eff_nb) + ". ") : (String((byte)(eff->eff_nb & 0xFF)) + "." + String((byte)(eff->eff_nb >> 8) - 1U) + ". ")) : "")

#ifdef DELAYED_EFFECTS
 #define INDEX_BUILD_DELAY 5
#else
 #define INDEX_BUILD_DELAY 1
#endif

#endif
