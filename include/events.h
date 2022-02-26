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

#ifndef _EVENTS_H
#define _EVENTS_H

#include "ArduinoJson.h"
#include "timeProcessor.h"
#include "misc.h"
#include "LList.h"

#define EVENT_TSTAMP_LENGTH 17  // для строки вида "YYYY-MM-DDThh:mm"

// закрепляем за событиями номера, чтобы они не ломались после сборки с другими дефайнами
typedef enum _EVENT_TYPE {
    ON                      = 1,
    OFF                     = 2,
    ALARM                   = 3,
    DEMO                    = 4,
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
    SET_WARNING             = 15,
    SET_GLOBAL_BRIGHT       = 16,
    SET_WHITE_HI            = 17,
    SET_WHITE_LO            = 18,
#ifdef ESP_USE_BUTTON
    BUTTONS_CONFIG_LOAD     = 19
#endif
} EVENT_TYPE;

static const char T_EVENT_DAYS[] PROGMEM = "ПНВТСРЧТПТСБВС";
class EVENT_MANAGER;
class DEV_EVENT {
    friend EVENT_MANAGER;
private:
    uint8_t repeat;
    uint8_t stopat;
    time_t unixtime;    // timestamp для события в локальной часовой зоне
    EVENT_TYPE event;
    String message;
public:
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
    const EVENT_TYPE getEvent() {return event;}
    void setEvent(EVENT_TYPE _event) {event = _event;}

    const uint8_t getRepeat() {return repeat;}
    void setRepeat(uint8_t _repeat) {repeat = _repeat;}
    
    const uint8_t getStopat() {return stopat;}
    void setStopat(uint8_t _stopat) {stopat = _stopat;}
    
    void setUnixtime(time_t _unixtime) {unixtime = _unixtime;}
    const String&getMessage() {return message;}
    void setMessage(const String& _message) {message = _message;}
    DEV_EVENT(const DEV_EVENT &event) {this->raw_data=event.raw_data; this->repeat=event.repeat; this->stopat=event.stopat; this->unixtime=event.unixtime; this->event=event.event; this->message=event.message;}
    DEV_EVENT() {this->raw_data=0; this->isEnabled=true; this->repeat=0; this->stopat=0; this->unixtime=0; this->event=_EVENT_TYPE::ON; this->message = "";}
    const bool operator==(const DEV_EVENT&event) {return (this->raw_data==event.raw_data && this->event==event.event && this->unixtime==event.unixtime);}

    String getDateTime() {
        String tmpBuf;
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
        case EVENT_TYPE::DEMO:
            buffer.concat(F("DEMO"));
            break;
        case EVENT_TYPE::LAMP_CONFIG_LOAD:
            buffer.concat(F("LMP_GFG"));
            break;
        case EVENT_TYPE::EFF_CONFIG_LOAD:
            buffer.concat(F("EFF_GFG"));
            break;
#ifdef ESP_USE_BUTTON
        case EVENT_TYPE::BUTTONS_CONFIG_LOAD:
            buffer.concat(F("BUT_GFG"));
            break;
#endif
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
        case EVENT_TYPE::SET_GLOBAL_BRIGHT:
            buffer.concat(F("GLOBAL BR"));
            break;
        case EVENT_TYPE::SET_WHITE_HI:
            buffer.concat(F("WHITE HI"));
            break;
        case EVENT_TYPE::SET_WHITE_LO:
            buffer.concat(F("WHITE LO"));
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

        if(message){
            buffer.concat(F(","));
            if(message.length()>5){
                buffer.concat(message.substring(0,4)+"...");
            } else {
                buffer.concat(message);
            }
        }
        return buffer;
    }
};

class EVENT_MANAGER {
private:
    EVENT_MANAGER(const EVENT_MANAGER&);  // noncopyable
    EVENT_MANAGER& operator=(const EVENT_MANAGER&);  // noncopyable
    LList<DEV_EVENT *> *events = nullptr;

    void(*cb_func)(DEV_EVENT *) = nullptr; // функция обратного вызова

    void check_event(DEV_EVENT *event);
    /**
     *  метод загружает и пробует десериализовать джейсон из файла в предоставленный документ,
     *  возвращает true если загрузка и десериализация прошла успешно
     *  @param doc - DynamicJsonDocument куда будет загружен джейсон
     *  @param jsonfile - файл, для загрузки
    **/
    bool deserializeFile(DynamicJsonDocument& doc, const char* filepath);
    void clear_events();

public:
    EVENT_MANAGER() { events = new LList<DEV_EVENT *>; }
    ~EVENT_MANAGER() { if(events) delete events; }

    LList<DEV_EVENT *> *getEvents() {return events;}
    
    DEV_EVENT *addEvent(const DEV_EVENT&event);
    void delEvent(const DEV_EVENT&event);
    bool isEnumerated(const DEV_EVENT&event); // проверка того что эвент в списке

    void setEventCallback(void(*func)(DEV_EVENT *))
    {
        cb_func = func;
    }
    
    void events_handle();
    
    // конфиги событий
    void loadConfig(const char *cfg = nullptr);
    void saveConfig(const char *cfg = nullptr);
};

#endif