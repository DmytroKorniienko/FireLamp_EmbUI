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

#ifndef __MAIN_H_
#define __MAIN_H_

#if __cplusplus >= 201703L
#define register // keyword 'register' is banned with c++17
#endif

#include <Arduino.h>

#ifdef PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
#endif

#include <SPIFFSEditor.h>

#include "config.h"
#include "EmbUI.h"
#include "lamp.h"
#include "buttons.h"


#ifdef USE_FTP
  #include "ftpSrv.h"
#endif
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
#endif
void sendData();

void create_parameters();
void sync_parameters();
void event_worker(const EVENT *);
bool notfound_handle(AsyncWebServerRequest *request, const String& req); // кастомный обработчик, для поддержки приложения WLED APP ( https://play.google.com/store/apps/details?id=com.aircoookie.WLED )
bool ws_action_handle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif
