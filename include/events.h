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
#include "ArduinoJson.h"

#define EVENT_TSTAMP_LENGTH 17  // для строки вида "YYYY-MM-DDThh:mm"

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
    SET_EFFECT              = 14,
    SET_WARNING             = 15
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
    time_t unixtime;    // timestamp для события в локальной часовой зоне
    EVENT_TYPE event;
    char *message = nullptr;
    EVENT *next = nullptr;
    EVENT(const EVENT &event) {this->raw_data=event.raw_data; this->repeat=event.repeat; this->stopat=event.stopat; this->unixtime=event.unixtime; this->event=event.event; this->message=event.message; this->next = nullptr;}
    EVENT() {this->raw_data=0; this->isEnabled=true; this->repeat=0; this->stopat=0; this->unixtime=0; this->event=_EVENT_TYPE::ON; this->message=nullptr; this->next = nullptr;}
    const bool operator==(const EVENT&event) {return (this->raw_data==event.raw_data && this->event==event.event && this->unixtime==event.unixtime);}

    String getDateTime() {
        String tmpBuf((char *)0);
        TimeProcessor::getDateTimeString(tmpBuf, unixtime);
        return tmpBuf;
    }

    String getName() {
        String buffer;
        String day_buf = FPSTR(T_EVENT_DAYS);

        buffer.concat(isEnabled?F(" "):F("!"));
        
        TimeProcessor::getDateTimeString(buffer, unixtime);

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
        case EVENT_TYPE::SET_EFFECT:
            buffer.concat(F("EFFECT"));
            break;
        case EVENT_TYPE::SET_WARNING:
            buffer.concat(F("WARNING"));
            break;
        default:
            break;
        }

        if(repeat) {buffer.concat(F(",")); buffer.concat(repeat);}
        if(repeat && stopat) { buffer.concat(F(",")); buffer.concat(stopat);}

        uint8_t t_raw_data = raw_data>>1;
        if(t_raw_data)
            buffer.concat(F(","));
        for(uint8_t i=1;i<8; i++){
            if(t_raw_data&1){
                //Serial.println, day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                buffer.concat(day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                if(t_raw_data >> 1)
                    buffer.concat(F(",")); // кроме последнего
            }
            t_raw_data >>= 1;
        }

        if(message && message[0]){     // время тут никто и не копирует, а усекается текст
            uint8_t UTFNsymbols = 0; // кол-во симоволов UTF-8 уже скопированных
            uint8_t i = 0;
            char tmpBuf[EVENT_TSTAMP_LENGTH];
            while(UTFNsymbols < 5 && message[i] && i < sizeof(tmpBuf)-4)
            {
                if(message[i]&0x80){
                    // это префикс многобайтного
                    uint8_t nbS = 0; uint8_t chk = message[i];
                    //LOG(printf_P,PSTR("nbS=%d,%x\n"),nbS,chk);
                    while(chk&0x80) {
                        chk=chk<<1; // проверяем сколько символов нужно копировать
                        nbS++;
                    }
                    //LOG(printf_P,PSTR("nbS=%d\n"),nbS);
                    while(nbS){
                        tmpBuf[i]=message[i];
                        nbS--; i++;
                    }
                    //LOG(printf_P,PSTR("UTF8 lastchar=%d, UTFNsymbols=%d\n"),i, UTFNsymbols);
                    UTFNsymbols++; // один UTF-8 скопировали
                } else {
                    tmpBuf[i]=message[i];
                    //LOG(printf_P,PSTR("ASCII lastchar=%d, UTFNsymbols=%d\n"),i, UTFNsymbols);
                    UTFNsymbols++; // один UTF-8 скопировали
                    i++;
                }
            }
            strcpy_P(tmpBuf+i,PSTR("..."));
            //LOG(printf_P,PSTR("lastchar=%d, UTFNsymbols=%d, message=%s\n"),i, UTFNsymbols,tmpBuf);
            buffer.concat(F(","));
            buffer.concat(tmpBuf);
        }
                
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
    /**
     *  метод загружает и пробует десериализовать джейсон из файла в предоставленный документ,
     *  возвращает true если загрузка и десериализация прошла успешно
     *  @param doc - DynamicJsonDocument куда будет загружен джейсон
     *  @param jsonfile - файл, для загрузки
    **/
    bool deserializeFile(DynamicJsonDocument& doc, const char* filepath);
    void clear_events();

public:
    EVENT_MANAGER() {}
    ~EVENT_MANAGER() { EVENT *next=root; EVENT *tmp_next=root; while(next!=nullptr) { tmp_next=next->next; if(next->message) {free(next->message);} delete next; next=tmp_next;} }

    EVENT *addEvent(const EVENT&event);
    void delEvent(const EVENT&event);
    bool isEnumerated(const EVENT&event); // проверка того что эвент в списке

    void setEventCallback(void(*func)(const EVENT *))
    {
        cb_func = func;
    }
    
    EVENT *getNextEvent(EVENT *next=nullptr)
    {
        if(next==nullptr) return root; else return next->next;
    }

    void events_handle();
    
    // конфиги событий
    void loadConfig(const char *cfg = nullptr);
    void saveConfig(const char *cfg = nullptr);
};
