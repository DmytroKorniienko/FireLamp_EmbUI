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
#include "config.h" // подключаем эффекты, там же их настройки
#include "effects.h"
#include "OTA.h"
#include "timeProcessor.h"
// Task Scheduler lib   https://github.com/arkhipenko/TaskScheduler
//#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STD_FUNCTION
#include <TaskSchedulerDeclarations.h>
#include <Ticker.h>

#ifdef MIC_EFFECTS
#include "micFFT.h"
#endif

typedef enum _LAMPMODE {
  MODE_NORMAL = 0,
  MODE_DEMO,
  MODE_WHITELAMP,
  MODE_ALARMCLOCK,
  MODE_OTA
} LAMPMODE;

typedef enum _EVENT_TYPE {ON, OFF, ALARM, DEMO_ON, LAMP_CONFIG_LOAD, EFF_CONFIG_LOAD, EVENTS_CONFIG_LOAD, SEND_TEXT, PIN_STATE} EVENT_TYPE;

const char T_EVENT_DAYS[] PROGMEM = "ПНВТСРЧТПТСБВС";

// смена эффекта
typedef enum _EFFSWITCH {
    SW_NONE = 0,    // пустой
    SW_NEXT,        // следующий
    SW_PREV,        // предыдущий
    SW_RND,         // случайный
    SW_DELAY,       // сохраненный (для фейдера)
    SW_SPECIFIC     // переход на конкретный эффект по индексу/имени
} EFFSWITCH;

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
    uint32_t unixtime;
    EVENT_TYPE event;
    char *message;
    EVENT *next = nullptr;
    EVENT(const EVENT &event) {this->raw_data=event.raw_data; this->repeat=event.repeat; this->stopat=event.stopat; this->unixtime=event.unixtime; this->event=event.event; this->message=event.message; this->next = nullptr;}
    EVENT() {this->raw_data=0; this->isEnabled=true; this->repeat=0; this->stopat=0; this->unixtime=0; this->event=_EVENT_TYPE::ON; this->message=nullptr; this->next = nullptr;}
    const bool operator==(const EVENT&event) {return (this->raw_data==event.raw_data && this->event==event.event && this->unixtime==event.unixtime);}
    String getDateTime(int offset = 0) {
        char tmpBuf[]="9999-99-99T99:99";
        time_t tm = unixtime+offset;
        sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u"),year(tm),month(tm),day(tm),hour(tm),minute(tm));
        return String(tmpBuf);
    }
    
    String getName(int offset = 0) {
        String buffer;
        char tmpBuf[]="9999-99-99T99:99";
        String day_buf(T_EVENT_DAYS);

        buffer.concat(isEnabled?F(" "):F("!"));

        time_t tm = unixtime+offset;
        sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u"),year(tm),month(tm),day(tm),hour(tm),minute(tm));
        buffer.concat(tmpBuf); buffer.concat(F(","));

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
        case EVENT_TYPE::PIN_STATE:
            buffer.concat(F("PIN"));
            break; 
        default:
            break;
        }
        buffer.concat(F(","));

        if(repeat) {buffer.concat(repeat); buffer.concat(F(","));}
        if(repeat && stopat) {buffer.concat(stopat); buffer.concat(F(","));}

        uint8_t t_raw_data = raw_data>>1;
        for(uint8_t i=1;i<8; i++){
            if(t_raw_data&1){
                //Serial.println(day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                buffer.concat(day_buf.substring((i-1)*2*2,i*2*2)); // по 2 байта на символ UTF16
                buffer.concat(F(","));
            }
            t_raw_data >>= 1;
        }
        //return buffer;

        if(message[0]){
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

    void check_event(EVENT *event, time_t current_time, int offset){
        if(!event->isEnabled) return;
        time_t eventtime = event->unixtime;// + offset;

        //LOG.printf_P(PSTR("%d %d\n"),current_time, eventtime);
        if(eventtime>current_time) return;

        if(eventtime==current_time) // точно попадает в период времени 1 минута, для однократных событий
        {
            if(cb_func!=nullptr) cb_func(event); // сработало событие
            return;
        }

        // если сегодня + периодический
        if(event->repeat && eventtime<=current_time && year(eventtime)==year(current_time) && month(eventtime)==month(current_time) && day(eventtime)==day(current_time)){
            //LOG.printf_P(PSTR("%d %d\n"),hour(current_time)*60+minute(current_time), event->repeat);
            if(!(((hour(current_time)*60+minute(current_time))-(hour(eventtime)*60+minute(eventtime)))%event->repeat)){
                if(((hour(current_time)*60+minute(current_time))<(hour(eventtime)*60+minute(eventtime)+event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                    if(cb_func!=nullptr) cb_func(event); // сработало событие
                    return;
                }
            }
        }

        uint8_t cur_day = dayOfWeek(current_time)-1; // 1 == Sunday
        if(!cur_day) cur_day = 7; // 7 = Sunday

        if((event->raw_data>>cur_day)&1) { // обрабатывать сегодня
            if(eventtime<=current_time){ // время события было раньше/равно текущего
                //LOG.printf_P(PSTR("%d %d\n"),hour(current_time)*60+minute(current_time), event->repeat);
                if(hour(eventtime)==hour(current_time) && minute(eventtime)==minute(current_time)){ // точное совпадение
                    if(cb_func!=nullptr) cb_func(event); // сработало событие
                    return;
                }
                if(event->repeat && hour(eventtime)<=hour(current_time)){ // периодический в сегодняшний день
                    if(!(((hour(current_time)*60+minute(current_time))-(hour(eventtime)*60+minute(eventtime)))%event->repeat)){
                        if(((hour(current_time)*60+minute(current_time))<(hour(eventtime)*60+minute(eventtime)+event->stopat)) || !event->stopat){ // еще не вышли за ограничения окончания события или его нет
                            if(cb_func!=nullptr) cb_func(event); // сработало событие
                            return;
                        }
                    }
                }
            }
        }
    }

public:
    EVENT_MANAGER() {}
    ~EVENT_MANAGER() { EVENT *next=root; EVENT *tmp_next=root; while(next!=nullptr) { tmp_next=next->next; if(next->message) {free(next->message);} delete next; next=tmp_next;} }
    void addEvent(const EVENT&event) {
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
    
    void delEvent(const EVENT&event) {
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

    void setEventCallback(void(*func)(const EVENT *))
    {
        cb_func = func;
    }
    
    EVENT *getNextEvent(EVENT *next=nullptr)
    {
        if(next==nullptr) return root; else return next->next;
    }

    void events_handle(time_t current_time, int offset)
    {
        EVENT *next = getNextEvent(nullptr);
        while (next!=nullptr)
        {
            check_event(next, current_time, offset);
            next = getNextEvent(next);
        }
    }
    
    void loadConfig(const char *cfg = nullptr) {
        if(SPIFFS.begin()){
            File configFile;
            if(cfg == nullptr)
                configFile = SPIFFS.open(F("/events_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
            else
                configFile = SPIFFS.open(cfg, "r"); // PSTR("r") использовать нельзя, будет исключение!
            String cfg_str = configFile.readString();

            if (cfg_str == F("")){
#ifdef LAMP_DEBUG
                LOG.println(F("Failed to open events config file"));
#endif
                saveConfig();
                return;
            }

#ifdef LAMP_DEBUG
                LOG.println(F("\nStart desialization of events\n\n"));
#endif

            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, cfg_str);
            if (error) {
#ifdef LAMP_DEBUG
                LOG.print(F("deserializeJson error: "));
                LOG.println(error.code());
                LOG.println(cfg_str);
#endif
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
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("[%u - %u - %u - %u - %u - %s]\n"), event.raw_data, event.unixtime, event.event, event.repeat, event.stopat, event.message);
#endif
            }
            // JsonArray::iterator it;
            // for (it=arr.begin(); it!=arr.end(); ++it) {
            //     const JsonObject& elem = *it;
            // }
#ifdef LAMP_DEBUG
            LOG.println(F("Events config loaded"));
#endif
            doc.clear();
        }
    }

    void saveConfig(const char *cfg = nullptr) {
        if(SPIFFS.begin()){
            File configFile;
            if(cfg == nullptr)
                configFile = SPIFFS.open(F("/events_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
            else
                configFile = SPIFFS.open(cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!

            configFile.print("[");
            EVENT *next=root;
            int i=1;
            while(next!=nullptr){
                configFile.printf_P(PSTR("%s{\"raw\":%u,\"ut\":%u,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), next->raw_data, next->unixtime, next->event, next->repeat, next->stopat,
                    ((next->message!=nullptr)?next->message:(char*)F("")));
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("%s{\"raw\":%u,\"ut\":%u,\"ev\":%u,\"rp\":%u,\"sa\":%u,\"msg\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), next->raw_data, next->unixtime, next->event, next->repeat, next->stopat,
                    ((next->message!=nullptr)?next->message:(char*)F("")));
#endif
                i++;
                next=next->next;
            }     
            configFile.print("]");
            configFile.flush();
            configFile.close();
            LOG.println(F("\nSave events config"));
        }
    }
};

class LAMP {
private:
#pragma pack(push,1)
 struct {
    bool MIRR_V:1; // отзрекаливание по V
    bool MIRR_H:1; // отзрекаливание по H
    bool dawnFlag:1; // флаг устанавливается будильником "рассвет"
    bool ONflag:1; // флаг включения/выключения
    bool manualOff:1; // будильник, разпознавание действия пользователя во время работы будильника, либо факта окончания действия рассвета
    bool loadingFlag:1; // флаг для начальной инициализации эффекта
    bool isFaderON:1; // признак того, что фейдер используется для эффектов
    bool isGlobalBrightness:1; // признак использования глобальной яркости для всех режимов
    bool isFirstHoldingPress:1; // флаг: только начали удерживать?
    bool startButtonHolding:1; // кнопка удерживается
    bool buttonEnabled:1; // кнопка обрабатывается если true
    bool brightDirection:1; // направление изменения яркости для кнопки
    bool speedDirection:1; // направление изменения скорости для кнопки
    bool scaleDirection:1; // направление изменения масштаба для кнопки
    bool setDirectionTimeout:1; // флаг: начало отсчета таймаута на смену направления регулировки
    bool isStringPrinting:1; // печатается ли прямо сейчас строка?
    bool isEffectsDisabledUntilText:1; // признак отключения эффектов, пока выводится текст
    bool isOffAfterText:1; // признак нужно ли выключать после вывода текста
    bool isEventsHandled:1; // глобальный признак обработки событий
#ifdef MIC_EFFECTS
    bool isCalibrationRequest:1; // находимся ли в режиме калибровки микрофона
    bool isMicOn:1; // глобальное включение/выключение микрофона
    uint8_t micAnalyseDivider:2; // делитель анализа микрофона 0 - выключен, 1 - каждый раз, 2 - каждый четвертый раз, 3 - каждый восьмой раз
#endif
 };
 #pragma pack(pop)
    //Button
    byte numHold = 0; // режим удержания
    byte txtOffset = 0; // смещение текста относительно края матрицы
    byte globalBrightness = BRIGHTNESS; // глобальная яркость, пока что будет использоваться для демо-режимов

    const int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint16_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);
    const uint16_t minDim = ((WIDTH<HEIGHT)?WIDTH:HEIGHT);

    LAMPMODE mode = MODE_NORMAL; // текущий режим
    LAMPMODE storedMode = MODE_NORMAL; // предыдущий режим
    EFF_ENUM storedEffect = EFF_NONE;
    EFFSWITCH _postponedSW = SW_NONE;       // отложенное действие смены ээфекта для федера

    uint32_t effTimer; // таймер для эффекта, сравнивается со скоростью текущего эффекта
    uint32_t effDelay; // доп. задержка для эффектов

    PERIODICTIME enPeriodicTimePrint; // режим периодического вывода времени

    void(*updateParmFunc)() = nullptr; // функтор обновления параметров

#ifdef MIC_EFFECTS
    MICWORKER *mw = nullptr;
    float mic_noise = 0.0; // уровень шума в ед.
    float mic_scale = 1.0; // коэф. смещения
    float last_freq = 0.0; // последняя измеренная часота
    float samp_freq = 0.0; // часота семплирования
    uint8_t last_max_peak = 0; // последнее максимальное амплитудное значение (по модулю)
    uint8_t last_min_peak = 0; // последнее минимальное амплитудное значение (по модулю)
    MIC_NOISE_REDUCE_LEVEL noise_reduce = MIC_NOISE_REDUCE_LEVEL::NONE; // уровень шумодава
    void micHandler();
#endif

    DynamicJsonDocument docArrMessages; // массив сообщений для вывода на лампу

    timerMinim tmDemoTimer;         // смена эффекта в демо режиме по дабл-клику из выключенного состояния, таймаут N секунд
    timerMinim tmConfigSaveTime;    // таймер для автосохранения
    timerMinim tmNumHoldTimer;      // таймаут удержания кнопки в мс
    timerMinim tmStringStepTime;    // шаг смещения строки, в мс
    timerMinim tmNewYearMessage;    // период вывода новогоднего сообщения
    time_t NEWYEAR_UNIXDATETIME=1609459200U;    // дата/время в UNIX формате, см. https://www.cy-pr.com/tools/time/ , 1609459200 => Fri, 01 Jan 2021 00:00:00 GMT

    // async fader and brightness control vars and methods
    uint8_t _brt, _steps;
    int8_t _brtincrement;
    void brightness(const uint8_t _brt, bool natural=true);     // низкоуровневая крутилка глобальной яркостью для других методов
    void fader(const uint8_t _tgtbrt, std::function<void(void)> callback=nullptr);          // обработчик затуания, вызывается планировщиком в цикле

    Scheduler ts;                   // TaskScheduler
    Task _fadeTicker;               // планировщик асинхронного фейдера

#ifdef ESP_USE_BUTTON
    GButton touch;               
    void buttonTick(); // обработчик кнопки
    timerMinim tmChangeDirectionTimer;     // таймаут смены направления увеличение-уменьшение при удержании кнопки
    Task _buttonTicker;             // Button poll scheduled task
    void changeDirection(byte numHold);
    void debugPrint();
#endif
    void effectsTick(); // обработчик эффектов

#ifdef VERTGAUGE
    byte xStep; byte xCol; byte yStep; byte yCol; // для индикатора
    void GaugeShow();
#endif
    void ConfigSaveCheck(){ if(tmConfigSaveTime.isReady()) {if(effects.autoSaveConfig()) tmConfigSaveTime.setInterval(0); } }

#ifdef OTA
    OtaManager otaManager;
#endif
    static void showWarning(CRGB::HTMLColorCode color, uint32_t duration, uint16_t blinkHalfPeriod); // Блокирующая мигалка

    void doPrintStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black);
    bool fillStringManual(const char* text,  const CRGB &letterColor, bool stopText = false, bool isInverse = false, int8_t letSpace = LET_SPACE, int8_t txtOffset = TEXT_OFFSET, int8_t letWidth = LET_WIDTH, int8_t letHeight = LET_HEIGHT);
    void drawLetter(uint16_t letter, int16_t offset,  const CRGB &letterColor, int8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight);
    uint8_t getFont(uint8_t asciiCode, uint8_t row);

    void alarmWorker();

public:
    EffectWorker effects; // объект реализующий доступ к эффектам
    EVENT_MANAGER events; // Объект реализующий доступ к событиям

#ifdef MIC_EFFECTS
    void setMicCalibration() {isCalibrationRequest = true;}
    bool isMicCalibration() {return isCalibrationRequest;}
    float getMicScale() {return mic_scale;}
    void setMicScale(float scale) {mic_scale = scale;}
    float getMicNoise() {return mic_noise;}
    void setMicNoise(float noise) {mic_noise = noise;}
    void setMicNoiseRdcLevel(MIC_NOISE_REDUCE_LEVEL lvl) {noise_reduce = lvl;}
    MIC_NOISE_REDUCE_LEVEL getMicNoiseRdcLevel() {return noise_reduce;}
    uint8_t getMicMaxPeak() {return isMicOn?last_max_peak:0;}
    uint8_t getMicMapMaxPeak() {return isMicOn?((last_max_peak>(uint8_t)mic_noise)?(last_max_peak-(uint8_t)mic_noise)*2:1):0;}
    float getMicFreq() {return isMicOn?last_freq:0;}
    uint8_t getMicMapFreq() {
        float minFreq=(log((float)(SAMPLING_FREQ>>1)/MICWORKER::samples));
        float scale = 255.0/(log(20000.0)-minFreq);
        return (uint8_t)(isMicOn?(log(last_freq)-minFreq)*scale:0);
    }
    void setMicOnOff(bool val) {isMicOn = val;}
    bool isMicOnOff() {return isMicOn;}
    void setMicAnalyseDivider(uint8_t val) {micAnalyseDivider = val&3;}
#endif


    bool isLoading() {if(!loadingFlag) return loadingFlag; else {loadingFlag=false; return true;}}
    void setLoading(bool flag=true) {loadingFlag = flag;}

    // Lamp brightness control (здесь методы работы с конфигурационной яркостью, не с LED!)
    byte getLampBrightness() { return (mode==MODE_DEMO || isGlobalBrightness)?globalBrightness:effects.getBrightness();}
    byte getNormalizedLampBrightness() { return (byte)(((unsigned int)BRIGHTNESS)*((mode==MODE_DEMO || isGlobalBrightness)?globalBrightness:effects.getBrightness())/255);}
    void setLampBrightness(byte brg) { if(mode==MODE_DEMO || isGlobalBrightness) {setGlobalBrightness(brg);} else {effects.setBrightness(brg);} }
    void setGlobalBrightness(byte brg) {globalBrightness = brg;}
    void setIsGlobalBrightness(bool val) {isGlobalBrightness = val;}
    bool IsGlobalBrightness() {return isGlobalBrightness;}

    void restartDemoTimer() {tmDemoTimer.reset();}
    LAMPMODE getMode() {return mode;}
    void updateParm(void(*f)()) { updateParmFunc=f; }

    TimeProcessor timeProcessor;
    void refreshTimeManual() { timeProcessor.handleTime(true); }

    void sendStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, bool forcePrint = false);
    
    LAMP();

    void handle(); // главная функция обработки эффектов
    void lamp_init();

    void ConfigSaveSetup(int in){ tmConfigSaveTime.setInterval(in); tmConfigSaveTime.reset(); }
    void setFaderFlag(bool flag) {isFaderON = flag;}
    bool getFaderFlag() {return isFaderON;}
    void setButtonOn(bool flag) {buttonEnabled = flag;}
    void setOnOff(bool flag) {changePower(flag); manualOff = true;} // любая активность в интерфейсе - отключаем будильник
    void disableEffectsUntilText() {isEffectsDisabledUntilText = true; FastLED.clear();}
    void setOffAfterText() {isOffAfterText = true;}
    void setIsEventsHandled(bool flag) {isEventsHandled = flag;}
    bool IsEventsHandled() {return isEventsHandled;}
    bool isLampOn() {return ONflag;}
    void setMIRR_V(bool flag) {if (flag!=MIRR_V) { MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=MIRR_H) { MIRR_H = flag; FastLED.clear();}}
    void setTextMovingSpeed(uint8_t val) {tmStringStepTime.setInterval(val);}
    void setTextOffset(uint8_t val) { txtOffset=val;}
    void setPeriodicTimePrint(PERIODICTIME val) { enPeriodicTimePrint = val; }

    void periodicTimeHandle();

    void startAlarm();
    void startDemoMode();
    void startNormalMode();
#ifdef OTA
    void startOTAUpdate();
    void startOTA() { otaManager.RequestOtaUpdate(); if (otaManager.RequestOtaUpdate()) { startOTAUpdate(); } }
#endif
    void newYearMessageHandle();
    void setNYMessageTimer(int in){ tmNewYearMessage.setInterval(in*60*1000); tmNewYearMessage.reset(); }
    void setNYUnixTime(time_t tm){ NEWYEAR_UNIXDATETIME = tm; }

    // ---------- служебные функции -------------
    uint32_t getEffDelay() {return effDelay;}
    uint16_t getmaxDim() {return maxDim;}
    uint16_t getminDim() {return minDim;}
    void setEffDelay(uint32_t dl) {effDelay=dl;}

    void changePower(); // плавное включение/выключение
    void changePower(bool);

    uint32_t getPixelNumber(uint16_t x, uint16_t y); // получить номер пикселя в ленте по координатам
    uint32_t getPixColor(uint32_t thisSegm); // функция получения цвета пикселя по его номеру
    uint32_t getPixColorXY(uint16_t x, uint16_t y) { return getPixColor(getPixelNumber(x, y)); } // функция получения цвета пикселя в матрице по его координатам
    void fillAll(CRGB color); // залить все
    void drawPixelXY(int16_t x, int16_t y, CRGB color); // функция отрисовки точки по координатам X Y
    CRGB *getUnsafeLedsArray(){return leds;}
    CRGB *setLeds(uint16_t idx, CHSV val) { leds[idx] = val; return &leds[idx]; }
    CRGB *setLeds(uint16_t idx, CRGB val) { leds[idx] = val; return &leds[idx]; }
    void setLedsfadeToBlackBy(uint16_t idx, uint8_t val) { leds[idx].fadeToBlackBy(val); }
    void setLedsNscale8(uint16_t idx, uint8_t val) { leds[idx].nscale8(val); }
    //fadeToBlackBy
    void dimAll(uint8_t value) { for (uint16_t i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(value); } }
    CRGB getLeds(uint16_t idx) { return leds[idx]; }
    CRGB *getLeds() { return leds; }
    void blur2d(uint8_t val) {::blur2d(leds,WIDTH,HEIGHT,val);}

    /*
     * Change global brightness with or without fade effect
     * fade applied in non-blocking way
     * FastLED dim8 function applied internaly for natural brightness controll
     * @param uint8_t _tgtbrt - target brigtness level 0-255
     * @param bool fade - use fade effect on brightness change
     * @param bool natural - apply dim8 function for natural brightness controll
     */
    void setBrightness(const uint8_t _tgtbrt, const bool fade=FADE, const bool natural=true);

    /*
     * Get current brightness
     * FastLED brighten8 function applied internaly for natural brightness compensation
     * @param bool natural - return compensated or absolute brightness
     */
    uint8_t getBrightness(const bool natural=true);

    /*
     * Non-blocking light fader, uses system ticker to globaly fade FastLED brighness
     * within specified duration
     * @param uint8_t _targetbrightness - end value for the brighness to fade to, FastLED dim8
     *                                   function applied internaly for natiral dimming
     * @param uint32_t _duration - fade effect duraion, ms
     * @param callback  -  callback-функция, которая будет выполнена после окончания затухания (без блокировки)
     */
    void fadelight(const uint8_t _targetbrightness=0, const uint32_t _duration=FADE_TIME, std::function<void()> callback=nullptr);

    /*
     *   хук обработчика прерываний для кнопки
     */
    void buttonPress(bool state);

    /*
     * переключатель эффектов для других методов,
     * может использовать фейдер, выбирать случайный эффект для демо
     * @param EFFSWITCH action - вид переключения (пред, след, случ.)
     * @param fade - переключаться через фейдер или сразу
     * @param effnb - номер эффекта
     */
    void switcheffect(EFFSWITCH action = SW_NONE, bool fade=FADE, EFF_ENUM effnb = EFF_ENUM::EFF_NONE);

    ~LAMP() {}
private:
    LAMP(const LAMP&);  // noncopyable
    LAMP& operator=(const LAMP&);  // noncopyable
    CRGB leds[NUM_LEDS];
#ifdef USELEDBUF
    CRGB ledsbuff[NUM_LEDS]; // буфер под эффекты
#endif
};

