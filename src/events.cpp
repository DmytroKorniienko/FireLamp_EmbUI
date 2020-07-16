
#include "events.h"
#include "LittleFS.h"
#include "ArduinoJson.h"


void EVENT_MANAGER::check_event(EVENT *event)
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
        if(!((( tm->tm_hour * 60 + tm->tm_min )-( tme->tm_hour * 60 + tme->tm_min ))%event->repeat)){
            if(((tm->tm_hour * 60 + tm->tm_min)<(tme->tm_hour * 60 + tme->tm_min + event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                cb_func(event); // сработало событие
                return;
            }
        }
    }
    uint8_t cur_day = tm->tm_wday-1; // 1 == Sunday
    if(!cur_day) cur_day = 7; // 7 = Sunday
    if((event->raw_data>>cur_day)&1) { // обрабатывать сегодня
        if(event->unixtime<=_now){ // время события было раньше/равно текущего
            //LOG(printf_P, PSTR("%d %d\n"),hour(current_time)*60+minute(current_time), event->repeat);
            if(tm->tm_hour==tme->tm_hour && tm->tm_min==tme->tm_min){ // точное совпадение
                cb_func(event); // сработало событие
                return;
            }
            if(event->repeat && tm->tm_hour==tme->tm_hour){ // периодический в сегодняшний день
                if(!((( tm->tm_hour * 60 + tm->tm_min )-( tme->tm_hour * 60 + tme->tm_min ))%event->repeat)){
                    if(((tm->tm_hour * 60 + tm->tm_min)<(tme->tm_hour * 60 + tme->tm_min+event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                        cb_func(event); // сработало событие
                        return;
                    }
                }
            }
        }
    }
}

void EVENT_MANAGER::addEvent(const EVENT&event)
{
    EVENT *next=root;
    EVENT *new_event = new EVENT(event);
    if(event.message!=nullptr){
        new_event->message = (char *)malloc(strlen(event.message)+1);
        strcpy(new_event->message, event.message);
    }
    if(next!=nullptr){
        while(next->next!=nullptr){
            next=next->next;
        }
        next->next = new_event;
    }
    else {
        root = new_event;
    }
}

void EVENT_MANAGER::delEvent(const EVENT&event) {
    EVENT *next=root;
    EVENT *prev=root;
    if(next!=nullptr){
        while(next){
            EVENT *tmp_next = next->next;
            if(*next==event){
                if(next->message!=nullptr)
                    free(next->message);
                delete next;
                if(next==root) root=tmp_next; else prev->next=tmp_next;
            } else {
                prev = next;
            }
            next=tmp_next;
        }
    }
}

void EVENT_MANAGER::events_handle()
{
  // пропускаем все ненулевые секунды
  if(!TimeProcessor::seconds00())
    return;

    EVENT *next = getNextEvent(nullptr);
    while (next!=nullptr)
    {
        check_event(next);
        next = getNextEvent(next);
    }
}

void EVENT_MANAGER::loadConfig(const char *cfg)
{
    if(LittleFS.begin()){
        File configFile;
        if(cfg == nullptr)
            configFile = LittleFS.open(F("/events_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
        else
            configFile = LittleFS.open(cfg, "r"); // PSTR("r") использовать нельзя, будет исключение!
        String cfg_str = configFile.readString();
        if (cfg_str == F("")){
            LOG(println, F("Failed to open events config file"));
            saveConfig();
            return;
        }
        LOG(println, F("\nStart desialization of events\n\n"));
        DynamicJsonDocument doc(8192);
        DeserializationError error = deserializeJson(doc, cfg_str);
        if (error) {
            LOG(print, F("deserializeJson error: "));
            LOG(println, error.code());
            LOG(println, cfg_str);
            return;
        }
        JsonArray arr = doc.as<JsonArray>();
        EVENT event;
        for (size_t i=0; i<arr.size(); i++) {
            JsonObject item = arr[i];
            event.raw_data = item[F("raw")].as<int>();
            event.unixtime = item[F("ut")].as<unsigned long>();
            event.event = (EVENT_TYPE)(item[F("ev")].as<int>());
            event.repeat = item[F("rp")].as<int>();
            event.stopat = item[F("sa")].as<int>();
            String tmpStr = item[F("msg")].as<String>();
            event.message = (char *)tmpStr.c_str();
            addEvent(event);
            LOG(printf_P, PSTR("[%u - %u - %u - %u - %u - %s]\n"), event.raw_data, event.unixtime, event.event, event.repeat, event.stopat, event.message);
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
        EVENT *next=root;
        int i=1;
        while(next!=nullptr){
            configFile.printf_P(PSTR("%s{\"raw\":%u,\"ut\":%u,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                (char*)(i>1?F(","):F("")), next->raw_data, next->unixtime, next->event, next->repeat, next->stopat,
                ((next->message!=nullptr)?next->message:(char*)F("")));
            LOG(printf_P, PSTR("%s{\"raw\":%u,\"ut\":%u,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                (char*)(i>1?F(","):F("")), next->raw_data, next->unixtime, next->event, next->repeat, next->stopat,
                ((next->message!=nullptr)?next->message:(char*)F("")));
            i++;
            next=next->next;
        }     
        configFile.print("]");
        configFile.flush();
        configFile.close();
        LOG(println, F("\nSave events config"));
    }
}
