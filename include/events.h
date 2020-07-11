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
#include "misc.h"
#include "timeProcessor.h"
//#include <Ticker.h>

// закрепляем за событиями номера, чтобы они не ломались после сборки с другими дефайнами
typedef enum _EVENT_TYPE {
    ON                      = 1,
    OFF                     = 2,
    ALARM                   = 3,
    DEMO_ON                 = 4,
    LAMP_CONFIG_LOAD        = 5,
    EFF_CONFIG_LOAD         = 6,
    EVENTS_CONFIG_LOAD      = 7,
    SEND_TEXT               = 8,
    SEND_TIME               = 9,
    PIN_STATE               = 10,
#ifdef AUX_PIN
    AUX_ON                  = 11,
    AUX_OFF                 = 12,
    AUX_TOGGLE              = 13,
#endif
} EVENT_TYPE;

static const char T_EVENT_DAYS[] PROGMEM = "ПНВТСРЧТПТСБВС";

struct EVENT {
    union {
        struct {
            bool isEnabled:1;
            bool d1:1;
            bool d2:1;
            bool d3:1;
            bool d4:1;
            bool d5:1;
            bool d6:1;
            bool d7:1;
        };
        uint8_t raw_data;
    };
    uint8_t repeat;
    uint8_t stopat;
    //uint32_t unixtime;
    time_t unixtime;
    EVENT_TYPE event;
    char *message;
    EVENT *next = nullptr;
    EVENT(const EVENT &event) {this->raw_data=event.raw_data; this->repeat=event.repeat; this->stopat=event.stopat; this->unixtime=event.unixtime; this->event=event.event; this->message=event.message; this->next = nullptr;}
    EVENT() {this->raw_data=0; this->isEnabled=true; this->repeat=0; this->stopat=0; this->unixtime=0; this->event=_EVENT_TYPE::ON; this->message=nullptr; this->next = nullptr;}
    const bool operator==(const EVENT&event) {return (this->raw_data==event.raw_data && this->event==event.event && this->unixtime==event.unixtime);}
    String getDateTime(int offset = 0) {
        String tmpBuf;
        TimeProcessor::getDateTimeString(tmpBuf);
        /*
        char tmpBuf[]="9999-99-99T99:99";
        time_t tm = unixtime+offset;
        sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u"),year(tm),month(tm),day(tm),hour(tm),minute(tm));
        return String(tmpBuf);
        */
        return tmpBuf;
    }

    String getName(int offset = 0) {
        String buffer;
        //char tmpBuf[]="9999-99-99T99:99";
        String day_buf(T_EVENT_DAYS);

        buffer.concat(isEnabled?F(" "):F("!"));
        //time_t tm = unixtime+offset;
        //sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u"),year(tm),month(tm),day(tm),hour(tm),minute(tm));
        //buffer.concat(tmpBuf);
        TimeProcessor::getDateTimeString(buffer);
        buffer.concat(F(","));

        switch (event)
        {
        case EVENT_TYPE::ON:
            buffer.concat(F("ON"));
            break;
        case EVENT_TYPE::OFF:
            buffer.concat(F("OFF"));
            break;
        case EVENT_TYPE::ALARM:
            buffer.concat(F("ALARM"));
            break;
        case EVENT_TYPE::DEMO_ON:
            buffer.concat(F("DEMO ON"));
            break;
        case EVENT_TYPE::LAMP_CONFIG_LOAD:
            buffer.concat(F("LMP_GFG"));
            break;
        case EVENT_TYPE::EFF_CONFIG_LOAD:
            buffer.concat(F("EFF_GFG"));
            break;
        case EVENT_TYPE::EVENTS_CONFIG_LOAD:
            buffer.concat(F("EVT_GFG"));
            break;
        case EVENT_TYPE::SEND_TEXT:
            buffer.concat(F("TEXT"));
            break;
        case EVENT_TYPE::SEND_TIME:
            buffer.concat(F("TIME"));
            break;
        case EVENT_TYPE::PIN_STATE:
            buffer.concat(F("PIN"));
            break;
#ifdef AUX_PIN
        case EVENT_TYPE::AUX_ON:
            buffer.concat(F("AUX ON"));
            break;
        case EVENT_TYPE::AUX_OFF:
            buffer.concat(F("AUX OFF"));
            break;
        case EVENT_TYPE::AUX_TOGGLE:
            buffer.concat(F("AUX TOGGLE"));
            break;
#endif
        default:
            break;
        }
        buffer.concat(F(","));

        if(repeat) {buffer.concat(repeat); buffer.concat(F(","));}
        if(repeat && stopat) {buffer.concat(stopat); buffer.concat(F(","));}

        uint8_t t_raw_data = raw_data>>1;
        for(uint8_t i=1;i<8; i++){
            if(t_raw_data&1){
                //Serial.println, day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                buffer.concat(day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                buffer.concat(F(","));
            }
            t_raw_data >>= 1;
        }
        //return buffer;
        char tmpBuf[]="9999-99-99T99:99";
        if(message[0]){     // что делает эта секция, зачем копировать время второй раз??
            memcpy(tmpBuf,message,5*2);
            strcpy_P(tmpBuf+5*2,PSTR("..."));
        }
        buffer.concat(tmpBuf);

        return buffer;
    }
};

class EVENT_MANAGER {
private:
    EVENT_MANAGER(const EVENT_MANAGER&);  // noncopyable
    EVENT_MANAGER& operator=(const EVENT_MANAGER&);  // noncopyable
    EVENT *root = nullptr;
    void(*cb_func)(const EVENT *) = nullptr; // функция обратного вызова

    void check_event(EVENT *event);

public:
    EVENT_MANAGER() {}
    ~EVENT_MANAGER() { EVENT *next=root; EVENT *tmp_next=root; while(next!=nullptr) { tmp_next=next->next; if(next->message) {free(next->message);} delete next; next=tmp_next;} }

    void addEvent(const EVENT&event);

    void delEvent(const EVENT&event);

    void setEventCallback(void(*func)(const EVENT *))
    {
        cb_func = func;
    }

    EVENT *getNextEvent(EVENT *next=nullptr)
    {
        if(next==nullptr) return root; else return next->next;
    }

    void events_handle();

    void loadConfig(const char *cfg = nullptr);

    void saveConfig(const char *cfg = nullptr);
};
