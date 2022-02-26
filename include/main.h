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

#ifndef __MAIN_H_
#define __MAIN_H_

#if __cplusplus >= 201703L
#define register // keyword 'register' is banned with c++17
#endif

#include <Arduino.h>
#include "EmbUI.h"

#ifdef PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
#endif

#include <SPIFFSEditor.h>

#include "config.h"
#include "lamp.h"
#include "buttons.h"

#ifdef TM1637_CLOCK
  #include "tm.h"
#endif

#ifdef ENCODER
  #include "enc.h"
#endif

#ifdef RTC
  #include "rtc.h"
#endif

#ifdef USE_STREAMING
  #include "ledStream.h"
#endif

// TaskScheduler
extern Scheduler ts;

// глобальные переменные для работы с ними в программе
extern LAMP myLamp; // Объект лампы
#ifdef ESP_USE_BUTTON
extern Buttons *myButtons;
extern GButton touch;
#endif
#ifdef MP3PLAYER
#include "mp3player.h"
extern MP3PLAYERDEVICE *mp3;
#endif
#ifdef DS18B20
#include "DS18B20.h"
#endif

#ifdef ENCODER
#include "enc.h"
#endif
#ifdef EMBUI_USE_MQTT
void mqttCallback(const String &topic, const String &payload);
void mqttConnect();
void sendData();
#endif

void create_parameters();
void sync_parameters();
void event_worker(DEV_EVENT *);
bool notfound_handle(AsyncWebServerRequest *request, const String& req); // кастомный обработчик, для поддержки приложения WLED APP ( https://play.google.com/store/apps/details?id=com.aircoookie.WLED )
bool ws_action_handle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif
