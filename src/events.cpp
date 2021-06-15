
#include "events.h"

#ifdef ESP8266
 #include <LittleFS.h>
#endif

#ifdef ESP32
 #include <LITTLEFS.h>
 #define FORMAT_LITTLEFS_IF_FAILED true
 #define LittleFS LITTLEFS
#endif


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

EVENT *EVENT_MANAGER::addEvent(const EVENT&event)
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
    return new_event;
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

bool EVENT_MANAGER::isEnumerated(const EVENT&event) {
    EVENT *next=root;
    bool res = false;
    if(next!=nullptr){
        while(next){
            EVENT *tmp_next = next->next;
            if(*next==event){
                res = true;
                break;
            }
            next=tmp_next;
        }
    }
    return res;
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

void EVENT_MANAGER::clear_events()
{
    EVENT *next = root;
    EVENT *cur = root;
    while (next!=nullptr)
    {
        next=next->next;
        if(cur->message)
            free(cur->message);
        delete cur;
        cur=next;
    }
    root = nullptr;
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
        EVENT event;
        for (size_t i=0; i<arr.size(); i++) {
            JsonObject item = arr[i];
            event.raw_data = item[F("raw")].as<int>();
            event.unixtime = item[F("ut")].as<unsigned long>();
            event.event = (EVENT_TYPE)(item[F("ev")].as<int>());
            event.repeat = item[F("rp")].as<int>();
            event.stopat = item[F("sa")].as<int>();
            event.message = (char *)item[F("msg")].as<const char *>();
            EVENT *new_event = addEvent(event);
            //LOG(printf_P, PSTR("[%u - %ld - %u - %u - %u - %s]\n"), new_event->raw_data, new_event->unixtime, new_event->event, new_event->repeat, new_event->stopat, new_event->message);
            LOG(printf_P, PSTR("[%u - %ld - %u - %u - %u - ]\n"), new_event->raw_data, new_event->unixtime, new_event->event, new_event->repeat, new_event->stopat);
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
            configFile.printf_P(PSTR("%s{\"raw\":%u,\"ut\":%ld,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                (char*)(i>1?F(","):F("")), next->raw_data, next->unixtime, next->event, next->repeat, next->stopat,
                ((next->message!=nullptr)?next->message : ""));
            LOG(printf_P, PSTR("%s{\"raw\":%u,\"ut\":%ld,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
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
