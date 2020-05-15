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

#pragma once
#include <Arduino.h>
#include "JeeUI2.h"
#include "config.h"
#include "lamp.h"

#if (PULL_MODE == LOW_PULL)
#define BUTTON_PRESS_TRANSITION RISING
#define BUTTON_RELEASE_TRANSITION FALLING
#else
#define BUTTON_PRESS_TRANSITION FALLING
#define BUTTON_RELEASE_TRANSITION RISING
#endif

class INTRFACE_GLOBALS{
public:
#pragma pack(push,1)
 struct { // набор глобальных флагов
    bool isSetup:1;
    bool isTmSetup:1;
    bool isAddSetup:1;
    bool isEdEvent:1;
    bool isMicCal:1;
    bool pinTransition:1;  // ловим "нажатие" кнопки
 };
 #pragma pack(pop)
 uint8_t addSList = 1;
 EFFECT *prevEffect = nullptr;
 int mqtt_int; // интервал отправки данных по MQTT в секундах 
 INTRFACE_GLOBALS() { // инициализация значениями по умолчанию
    isSetup = false;
    isTmSetup = false;
    isAddSetup = false;
    isEdEvent = false;
    isMicCal = false;
    pinTransition = true;
}
};

// глобальные переменные для работы с ними в программе
extern SHARED_MEM GSHMEM; // Глобальная разделяемая память эффектов
extern INTRFACE_GLOBALS iGLOBAL; // объект глобальных переменных интерфейса
extern jeeui2 jee; // Создаем объект класса для работы с JeeUI2 фреймворком
extern LAMP myLamp; // Объект лампы
#ifdef ESP_USE_BUTTON
extern GButton touch;
#endif

void mqttCallback(const String &topic, const String &payload);
void sendData();
void update();
void interface();
void create_parameters();
void updateParm();
void jeebuttonshandle();
void event_worker(const EVENT *);
void httpCallback(const char *param, const char *value);
void setEffectParams(EFFECT *curEff);
ICACHE_RAM_ATTR void buttonpinisr();    // обработчик прерываний пина кнопки
void buttonhelper(bool state);