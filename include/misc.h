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

#ifndef __MISC_H
#define __MISC_H

#include <Arduino.h>
#include <vector>

#if defined(PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED)
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
#endif

#include "EmbUI.h"        //  Файлова система опреділяється в хедері EmbUI

typedef enum : uint8_t {AT_NONE=0, AT_FIRST, AT_SECOND, AT_THIRD, AT_FOURTH, AT_FIFTH, AT_RANDOM, AT_RANDOMMP3} ALARM_SOUND_TYPE; // виды будильников (8 вариантов максимум)
typedef enum : uint8_t {TS_NONE=0, TS_VER1, TS_VER2} TIME_SOUND_TYPE; // виды озвучки времени (8 вариантов максимум)

//----------------------------------------------------
// мини-класс таймера, версия 1.0

class timerMinim
{
  public:
	timerMinim() {_interval = 0;} // по дефолту - отключен

    timerMinim(uint32_t interval)				                  // объявление таймера с указанием интервала
    {
      _interval = interval;
      _timer = millis();
    }

    uint32_t getInterval()	                  						 // получение интервала работы таймера
    {
    	return _interval;
    }

    void setInterval(uint32_t interval)	                   // установка интервала работы таймера
    {
      _interval = interval;
    }

    bool isReady()						                             // возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
    {
      if ((uint32_t)millis() - _timer >= _interval && _interval!=0){
        _timer = millis();
        return true;
      }
      else {
        return false;
      }
    }

    bool isReadyManual()                                   // возвращает true, когда пришло время. Без сбороса
    {
      if ((uint32_t)millis() - _timer >= _interval && _interval!=0){
        return true;
      }
      else {
        return false;
      }
    }

    void reset()							                              // ручной сброс таймера на установленный интервал
    {
      _timer = millis();
    }

  private:
    uint32_t _timer = 0;
    uint32_t _interval = 0;
};


//----------------------------------------------------
#if defined(LOG)
  #undef LOG
#endif
#if defined(LAMP_DEBUG)
	//#define LOG                   Serial
	#define LOG(func, ...) Serial.func(__VA_ARGS__)
#else
	#define LOG(func, ...) ;
#endif


#endif