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

#ifndef __EXTRATASKS_H
#define __EXTRATASKS_H

#include "EmbUI.h"
#include <FastLED.h>
// TaskScheduler
extern Scheduler ts;

class StringTask : public Task {
    char *_data = nullptr;
    INLINE char *makeCopy(const char *data) {
        if(!data)
          return nullptr;
        size_t size = strlen(data);
        char *storage = new char[size+1];
        strncpy(storage,data,size);
        if(!storage) return nullptr;
        return storage;
    }
    INLINE void setNewData(char *newData) {if(_data) delete []_data; _data=newData;}
public:
    INLINE char *getData() {return _data;}
    INLINE StringTask(const char *data = nullptr, unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL)
    : Task(aInterval, aIterations, aCallback, aScheduler, aEnable, aOnEnable, aOnDisable){
        //LOG(println, F("StringTask constructor"));
        _data = makeCopy(data);
    }
    ~StringTask() {if(_data) delete[] _data;}
};

class WarningTask : public StringTask {
    CRGB _warn_color;
    uint32_t _warn_duration;
    uint16_t _warn_blinkHalfPeriod;
public:
    INLINE uint32_t getWarn_duration() {return _warn_duration;}
    INLINE uint32_t getWarn_blinkHalfPeriod() {return _warn_blinkHalfPeriod;}
    INLINE CRGB& getWarn_color() {return _warn_color;}
    INLINE WarningTask(CRGB &warn_color, uint32_t warn_duration, uint16_t warn_blinkHalfPeriod, const char *data = nullptr, unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL)
    : StringTask(data, aInterval, aIterations, aCallback, aScheduler, aEnable, aOnEnable, aOnDisable)
    , _warn_color(warn_color), _warn_duration(warn_duration), _warn_blinkHalfPeriod(warn_blinkHalfPeriod) {
      //LOG(println, F("WarningTask constructor"));
    }
};

class CtrlsTask : public Task {
    DynamicJsonDocument *_data = nullptr;
    INLINE DynamicJsonDocument *makeDoc(JsonObject *data) {
        DynamicJsonDocument *storage = new DynamicJsonDocument(data->size()*2+32);
        if(!storage) return nullptr;
        String tmp; serializeJson(*data,tmp); //LOG(printf_P, PSTR("makeDoc: %s\n"), tmp.c_str());
        deserializeJson((*storage), tmp);
        return storage;
    }
    INLINE void setNewData(DynamicJsonDocument *newData) {if(_data) delete _data; _data=newData;}
public:
    INLINE JsonObject getData() {return (_data ? _data->as<JsonObject>() : JsonObject());}
    bool replaceIfSame(JsonObject *test) {
        // сравниваем предыдущий и текущий ответы на совпадение пар
        if(!test) return false;
        JsonObject obj = getData();
        bool same=!obj.isNull();
        for (JsonPair kv : obj) {
            if(!((*test)).containsKey(kv.key())){
                same=false;
                break;
            }
        }
        if(same){
            DynamicJsonDocument *doc = makeDoc(test);
            setNewData(doc);
        }
        return same;
    }
    INLINE CtrlsTask(JsonObject *data = nullptr, unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL)
    : Task(aInterval, aIterations, aCallback, aScheduler, aEnable, aOnEnable, aOnDisable){
        _data = makeDoc(data);
    }
    ~CtrlsTask() {if(_data) delete _data;}
};

#endif