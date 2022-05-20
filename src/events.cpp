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
#include "events.h"

void EVENT_MANAGER::check_event(DEV_EVENT *event)
{
    if(!event->isEnabled || cb_func==nullptr) return;
    if(event->unixtime > *TimeProcessor::now() ) return;

    time_t _now = *(TimeProcessor::now());
    // точно попадает в период времени 1 минута, для однократных событий
    if(event->unixtime==_now)
    {
        cb_func(event); // сработало событие
        return;
    }

    struct tm t;
    tm *tme=&t;
    localtime_r(&(event->unixtime), tme);
    //LOG(printf_P, PSTR("%d %d\n"),current_time, eventtime);
    const tm* tm = localtime(&_now);

    // если сегодня + периодический
    if(event->repeat && event->unixtime <= _now && tm->tm_yday == tme->tm_yday){
        //LOG(printf_P, PSTR("%d %d\n"),hour(current_time)*60+minute(current_time), event->repeat);
        int tmdiff = ( tm->tm_hour * 60 + tm->tm_min )-( tme->tm_hour * 60 + tme->tm_min ); 
        if(tmdiff>=0 && !(tmdiff%event->repeat)){
            if(((tm->tm_hour * 60 + tm->tm_min)<=(tme->tm_hour * 60 + tme->tm_min + event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                cb_func(event); // сработало событие
                return;
            }
        }
    }
    //uint8_t cur_day = tm->tm_wday-1; // 1 == Sunday
    uint8_t cur_day = tm->tm_wday; // 1 == Sunday
    if(!cur_day) cur_day = 7; // 7 = Sunday
    if((event->raw_data>>cur_day)&1) { // обрабатывать сегодня
        if(event->unixtime<=_now){ // время события было раньше/равно текущего
            //LOG(printf_P, PSTR("%d %d\n"),hour(current_time)*60+minute(current_time), event->repeat);
            if(tm->tm_hour==tme->tm_hour && tm->tm_min==tme->tm_min){ // точное совпадение
                cb_func(event); // сработало событие
                return;
            }
            if(event->repeat && tm->tm_hour>=tme->tm_hour){ // периодический в сегодняшний день
                int tmdiff = ( tm->tm_hour * 60 + tm->tm_min )-( tme->tm_hour * 60 + tme->tm_min ); 
                if(tmdiff>=0 && !(tmdiff%event->repeat)){
                    if(((tm->tm_hour * 60 + tm->tm_min)<=(tme->tm_hour * 60 + tme->tm_min+event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                        cb_func(event); // сработало событие
                        return;
                    }
                }
            }
        }
    }
}

DEV_EVENT *EVENT_MANAGER::addEvent(const DEV_EVENT&event)
{
    DEV_EVENT *new_event = new DEV_EVENT(event);
    events->add(new_event);
    return new_event;
}

void EVENT_MANAGER::delEvent(const DEV_EVENT&event) {
    for(int i=0;i<events->size();i++){
        if(*((*events)[i])==event){
            events->remove(i); // удаляет из списка и чистит память
        }
    }
}

bool EVENT_MANAGER::isEnumerated(const DEV_EVENT&event) {
    for(int i=0;i<events->size();i++){
        if(*((*events)[i])==event){
            return true;
        }
    }
    return false;
}

void EVENT_MANAGER::events_handle()
{
    // пропускаем все ненулевые секунды
    if(!TimeProcessor::seconds00())
        return;

    for(int i=0;i<events->size();i++){
        check_event((*events)[i]);
    }
}

void EVENT_MANAGER::clear_events()
{
    events->clear();
}

/**
 *  метод загружает и пробует десериализовать джейсон из файла в предоставленный документ,
 *  возвращает true если загрузка и десериализация прошла успешно
 *  @param doc - DynamicJsonDocument куда будет загружен джейсон
 *  @param jsonfile - файл, для загрузки
 */
bool EVENT_MANAGER::deserializeFile(DynamicJsonDocument& doc, const char* filepath){
  if (!filepath || !*filepath)
    return false;

  File jfile = LittleFS.open(filepath, "r");
  DeserializationError error;
  if (jfile){
    error = deserializeJson(doc, jfile);
    jfile.close();
  } else {
    return false;
  }

  if (error) {
    LOG(printf_P, PSTR("File: failed to load json file: %s, deserializeJson error: "), filepath);
    LOG(println, error.code());
    return false;
  }
  //LOG(printf_P,PSTR("File: %s deserialization took %d ms\n"), filepath, millis() - timest);
  return true;
}

void EVENT_MANAGER::loadConfig(const char *cfg)
{
    if(LittleFS.begin()){
        clear_events();
        DynamicJsonDocument doc(4096);
        String filename = cfg ? String(cfg) : String(F("/events_config.json"));
        if (!deserializeFile(doc, filename.c_str())){
            LOG(print, F("deserializeJson error: "));
            LOG(println, filename);
            return;
        }
        JsonArray arr = doc.as<JsonArray>();
        DEV_EVENT event;
        for (size_t i=0; i<arr.size(); i++) {
            JsonObject item = arr[i];
            event.raw_data = item[F("raw")].as<int>();
            event.unixtime = item[F("ut")].as<time_t>();
            event.event = (EVENT_TYPE)(item[F("ev")].as<int>());
            event.repeat = item[F("rp")].as<int>();
            event.stopat = item[F("sa")].as<int>();
            event.message = item[F("msg")].as<String>();
            DEV_EVENT *new_event = addEvent(event);
            if(new_event){
                //LOG(println,F("event"));
                LOG(printf_P, PSTR("[%u - %lu - %u - %u - %u - %s]\n"), new_event->raw_data, (unsigned long)new_event->unixtime, new_event->event, new_event->repeat, new_event->stopat, new_event->message.c_str());
            }
        }

        LOG(println, F("Events config loaded"));
        doc.clear();
    }
}

void EVENT_MANAGER::saveConfig(const char *cfg)
{
    if(LittleFS.begin()){
        File configFile;
        if(cfg == nullptr)
            configFile = LittleFS.open(F("/events_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
        else
            configFile = LittleFS.open(cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
        configFile.print("[");
        
        LOG(println, F("Save events config"));
        bool firstLine=true;
        DEV_EVENT *next;
        for(int i=0;i<events->size();i++){
            next = (*events)[i];
            LOG(printf_P, PSTR("%s{\"raw\":%u,\"ut\":%lu,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                (!firstLine?",":""), next->raw_data, (unsigned long)next->unixtime, next->event, next->repeat, next->stopat, next->message.c_str());
            configFile.printf_P(PSTR("%s{\"raw\":%u,\"ut\":%lu,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                (!firstLine?",":""), next->raw_data, (unsigned long)next->unixtime, next->event, next->repeat, next->stopat, next->message.c_str());
            firstLine=false;
        }
        configFile.print("]");
        configFile.flush();
        configFile.close();
        LOG(println, F("\nSave events config"));
    }
}
