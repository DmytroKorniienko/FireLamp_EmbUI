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

#ifndef __LAMP_H
#define __LAMP_H

#include "misc.h"
#include "config.h" // подключаем эффекты, там же их настройки
#include "effects.h"
#include "OTA.h"
#include "events.h"
#include "LList.h"
#include "interface.h"
#include "extra_tasks.h"

#ifdef XY_EXTERN
#include "XY.h"
#endif

#ifdef MIC_EFFECTS
#include "micFFT.h"
#endif

#ifndef DEFAULT_MQTTPUB_INTERVAL
    #define DEFAULT_MQTTPUB_INTERVAL 30
#endif

typedef enum _LAMPMODE {
  MODE_NORMAL = 0,
  MODE_DEMO,
  MODE_WHITELAMP,
  MODE_ALARMCLOCK,
  MODE_OTA
} LAMPMODE;

// смена эффекта
typedef enum _EFFSWITCH {
    SW_NONE = 0,    // пустой
    SW_NEXT,        // следующий
    SW_PREV,        // предыдущий
    SW_RND,         // случайный
    SW_DELAY,       // сохраненный (для фейдера)
    SW_SPECIFIC,    // переход на конкретный эффект по индексу/имени
    SW_NEXT_DEMO,    // следующий для ДЕМО, исключая отключенные
    SW_WHITE_HI,
    SW_WHITE_LO,
} EFFSWITCH;

// управление Тикером
typedef enum _SCHEDULER {
    T_DISABLE = 0,    // Выкл
    T_ENABLE,         // Вкл
    T_FRAME_ENABLE,   // Вкл
    T_RESET,          // сброс
} SCHEDULER;

// Timings from FastLED chipsets.h
// WS2812@800kHz - 250ns, 625ns, 375ns
// время "отправки" кадра в матрицу, мс. где 1.5 эмпирический коэффициент
//#define FastLED_SHOW_TIME = WIDTH * HEIGHT * 24 * (0.250 + 0.625) / 1000 * 1.5

/*
 минимальная задержка между обсчетом и выводом кадра, мс
 нужна для обработки других задач в loop() между длинными вызовами
 калькулятора эффектов и его отрисовки. С другой стороны это время будет
 потеряно в любом случае, даже если остальные таски будут обработаны быстрее
 пока оставим тут, это крутилка не для общего конфига
 */
#define LED_SHOW_DELAY 1

//#pragma pack(push,2)
typedef union _LAMPFLAGS {
struct {
    // ВНИМАНИЕ: порядок следования не менять, флаги не исключать, переводить в reserved!!! используется как битовый массив в конфиге!
    bool MIRR_V:1; // отзрекаливание по V
    bool MIRR_H:1; // отзрекаливание по H
    bool isDraw:1; // режим рисования
    bool ONflag:1; // флаг включения/выключения
    bool isFaderON:1; // признак того, что фейдер используется для эффектов
    bool isGlobalBrightness:1; // признак использования глобальной яркости для всех режимов
    bool tm24:1;   // 24х часовой формат
    bool tmZero:1;  // ведущий 0
    bool limitAlarmVolume:1; // ограничивать громкость будильника
    bool isEventsHandled:1; // глобальный признак обработки событий
    bool isEffClearing:1; // признак очистки эффектов при переходе с одного на другой
    bool isDebug:1; // признак режима отладки
    bool numInList:1; // нумерация в списке
    bool effHasMic:1; // значек микрофона в списке
    bool dRand:1; // случайный порядок демо
    bool showName:1; // отображение имени в демо
    //--------16 штук граница-------------------
    // ВНИМАНИЕ: порядок следования не менять, флаги не исключать, переводить в reserved!!! используется как битовый массив в конфиге!
    bool isMicOn:1; // глобальное включение/выключение микрофона
    uint8_t MP3eq:3; // вид эквалайзера
    bool isShowSysMenu:1; // показывать ли системное меню
    bool isOnMP3:1; // включен ли плеер?
    bool isBtn:1; // включена ли кнопка?
    bool playName:1; // воспроизводить имя?
    bool playEffect:1; // воспроизводить эффект?
    uint8_t alarmSound:3; // звук будильника ALARM_SOUND_TYPE
    bool playMP3:1; // режим mp3-плеера
    uint8_t playTime:3; // воспроизводить время?
    // ВНИМАНИЕ: порядок следования не менять, флаги не исключать, переводить в reserved!!! используется как битовый массив в конфиге!
    //--------16 штук граница-------------------
    bool isGaugeOn:1;
    bool isTempOn:1;
    bool isStream:1;
    bool isDirect:1;
    bool isMapping:1;
};
uint64_t lampflags; // набор битов для конфига
_LAMPFLAGS(){
    MIRR_V = false; // отзрекаливание по V
    MIRR_H = false; // отзрекаливание по H
    ONflag = false; // флаг включения/выключения
    isDebug = false; // флаг отладки
    isFaderON = true; // признак того, что используется фейдер для смены эффектов
    isEffClearing = false; // нужно ли очищать эффекты при переходах с одного на другой
    isGlobalBrightness = false; // признак использования глобальной яркости для всех режимов
    isEventsHandled = true;
    isMicOn = true; // глобальное испльзование микрофона
    numInList = false;
    effHasMic = false;
    dRand = false;
    isShowSysMenu = false;
    isOnMP3 = false;
    isBtn = true;
    showName = false;
    playTime = TIME_SOUND_TYPE::TS_NONE; // воспроизводить время?
    playName = false; // воспроизводить имя?
    playEffect = false; // воспроизводить эффект?
    alarmSound = ALARM_SOUND_TYPE::AT_NONE;
    MP3eq = 0;
    playMP3 = false;
    limitAlarmVolume = false;
    isDraw = false;
    tm24 = true;
    tmZero = false;
    isGaugeOn = true;
    isTempOn = true;
    isStream = false;
    isDirect = false;
    isMapping = true;
}
} LAMPFLAGS;
//#pragma pack(pop)

class LAMP {
    friend class LEDFader;
private:
    LAMPFLAGS flags;
    LAMPSTATE lampState; // текущее состояние лампы, которое передается эффектам

    uint8_t txtOffset = 0; // смещение текста относительно края матрицы
    uint8_t globalBrightness = 127; // глобальная яркость, пока что будет использоваться для демо-режимов
    uint8_t fps = 0;        // fps counter
#ifdef LAMP_DEBUG
    uint16_t avgfps = 0;    // avarage fps counter
#endif
    //int mqtt_int = DEFAULT_MQTTPUB_INTERVAL;
    uint8_t bPin = BTN_PIN;        // пин кнопки
    uint16_t curLimit = CURRENT_LIMIT; // ограничение тока

    LAMPMODE mode = MODE_NORMAL; // текущий режим
    LAMPMODE storedMode = MODE_NORMAL; // предыдущий режим
    uint16_t storedEffect = (uint16_t)EFF_ENUM::EFF_NONE;
    uint8_t storedBright;

    typedef struct {
        uint8_t alarmP;
        uint8_t alarmT;
        String msg;
        bool isStartSnd;
        bool isLimitVol;
        ALARM_SOUND_TYPE type;

        void clear() { alarmP = 5; alarmT = 5; msg=""; isStartSnd = true; isLimitVol = true; type = ALARM_SOUND_TYPE::AT_RANDOM; }
    } ALARM_DATA;
    
    ALARM_DATA curAlarm;

#ifdef MIC_EFFECTS
    MICWORKER *mw = nullptr;
    void micHandler();
#endif

    uint8_t BFade; // затенение фона под текстом

    uint8_t alarmPT; // время будильника рассвет - старшие 4 бита и свечения после рассвета - младшие 4 бита
#ifdef TM1637_CLOCK
    uint8_t tmBright; // яркость дисплея при вкл - старшие 4 бита и яркость дисплея при выкл - младшие 4 бита
#endif
    DynamicJsonDocument *docArrMessages = nullptr; // массив сообщений для вывода на лампу

    timerMinim tmStringStepTime;    // шаг смещения строки, в мс
    timerMinim tmNewYearMessage;    // период вывода новогоднего сообщения

    time_t NEWYEAR_UNIXDATETIME=1609459200U;    // дата/время в UNIX формате, см. https://www.cy-pr.com/tools/time/ , 1609459200 => Fri, 01 Jan 2021 00:00:00 GMT

    Task *demoTask = nullptr;    // динамический планировщик Смены эффектов в ДЕМО
    Task *effectsTask;           // динамический планировщик обработки эффектов
    WarningTask *warningTask = nullptr; // динамический планировщик переключалки флага lampState.isWarning
    Task *tmqtt_pub = nullptr;   // динамический планировщик публикации через mqtt
    void brightness(const uint8_t _brt, bool natural=true);     // низкоуровневая крутилка глобальной яркостью для других методов

    void effectsTick(); // обработчик эффектов

#ifdef VERTGAUGE
    uint8_t xStep; uint8_t xCol; uint8_t yStep; uint8_t yCol; // для индикатора
    unsigned long gauge_time = 0;
    unsigned gauge_val = 0;
    unsigned gauge_max = 0;
    uint8_t gauge_hue = 0;
    CRGB gauge_color = 0;
    void GaugeMix();
#endif

#ifdef OTA
    OtaManager otaManager;
#endif
    String &prepareText(String &source);
    void doPrintStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, const int8_t textOffset = -128, const int16_t fixedPos = 0);
    bool fillStringManual(const char* text,  const CRGB &letterColor, bool stopText = false, bool isInverse = false, int32_t pos = 0, int8_t letSpace = LET_SPACE, int8_t txtOffset = TEXT_OFFSET, int8_t letWidth = LET_WIDTH, int8_t letHeight = LET_HEIGHT); // -2147483648
    void drawLetter(uint8_t bcount, uint16_t letter, int16_t offset,  const CRGB &letterColor, uint8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight, uint8_t flSymb=0);
    uint8_t getFont(uint8_t bcount, uint8_t asciiCode, uint8_t row);

    void alarmWorker();

    /*
     * вывод готового кадра на матрицу,
     * и перезапуск эффект-процессора
     */
    void frameShow(const uint32_t ticktime);
public:
    void showWarning(const CRGB &color, uint32_t duration, uint16_t blinkHalfPeriod, uint8_t warnType=0, bool forcerestart=true, const char *msg = nullptr); // Неблокирующая мигалка
    void warningHelper();

    void lamp_init(const uint16_t curlimit);       // первичная инициализация Лампы
    EffectWorker effects; // объект реализующий доступ к эффектам
    EVENT_MANAGER events; // Объект реализующий доступ к событиям
    uint64_t getLampFlags() {return flags.lampflags;} // возвращает упакованные флаги лампы
    const LAMPFLAGS &getLampSettings() {return flags;} // возвращает упакованные флаги лампы
    //void setLampFlags(uint32_t _lampflags) {flags.lampflags=_lampflags;} // устананавливает упакованные флаги лампы
    void setbPin(uint8_t val) {bPin = val;}
    uint8_t getbPin() {return bPin;}
    void setcurLimit(uint16_t val) {curLimit = val;}
    uint16_t getcurLimit() {return curLimit;}
    LAMPSTATE &getLampState() {return lampState;}
    LList<UIControl*>&getEffControls() { LList<UIControl*>&controls = effects.getControls(); return controls; }

#ifdef MIC_EFFECTS
    void setMicCalibration() {lampState.isCalibrationRequest = true;}
    bool isMicCalibration() {return lampState.isCalibrationRequest;}

    void setMicOnOff(bool val) {
        bool found=false;
        flags.isMicOn = val;
        lampState.isMicOn = val;
        if(effects.getEn()==EFF_NONE) return;
        LList<UIControl*>&controls = effects.getControls();
        UIControl *c7 = nullptr;
        if(val){
            for(int i=3; i<controls.size(); i++) {
                if(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1){
                    if(effects.worker) effects.worker->setDynCtrl(controls[i]);
                    found=true;
                } else if(controls[i]->getId()==7) {
                    c7 = controls[i];
                }
            }
        } 
        if(!val || !found){
            UIControl *ctrl = new UIControl(7,(CONTROL_TYPE)18,String(FPSTR(TINTF_020)), val ? "1" : "0", "0", "1", "1");
            if(effects.worker) effects.worker->setDynCtrl(ctrl);
            delete ctrl;
            if(c7){ // был найден 7 контрол, но не микрофон
                if(effects.worker) effects.worker->setDynCtrl(c7);
            }
        }
    }
    
    bool isMicOnOff() {return flags.isMicOn;}
#endif

#ifdef VERTGAUGE
    void GaugeShow(unsigned val, unsigned max, uint8_t hue = 0);
    void setGaugeColor(CRGB color) { gauge_color = color;}
#endif

    void setSpeedFactor(float val) {
        lampState.speedfactor = val;
        if(effects.worker) effects.worker->setDynCtrl(effects.getControls()[1]);
    }

    // Lamp brightness control (здесь методы работы с конфигурационной яркостью, не с LED!)
    uint8_t getLampBrightness() { return flags.isGlobalBrightness? globalBrightness : (effects.getControls()[0]->getVal()).toInt();}
    uint8_t getNormalizedLampBrightness() { return (uint8_t)(BRIGHTNESS * (flags.isGlobalBrightness? globalBrightness : (effects.getControls()[0]->getVal()).toInt()) / 255);}
    void setLampBrightness(uint8_t brg) { lampState.brightness=brg; if (flags.isGlobalBrightness) setGlobalBrightness(brg); else effects.getControls()[0]->setVal(String(brg)); }
    void setGlobalBrightness(uint8_t brg) {globalBrightness = brg;}
    void setIsGlobalBrightness(bool val) {flags.isGlobalBrightness = val; if(effects.worker) { lampState.brightness=getLampBrightness(); effects.worker->setDynCtrl(effects.getControls()[0]);} }
    bool IsGlobalBrightness() {return flags.isGlobalBrightness;}
    bool isAlarm() {return mode == MODE_ALARMCLOCK;}
    bool isWarning() {return lampState.isWarning;}
    //int getmqtt_int() {return mqtt_int;}

#ifdef EMBUI_USE_MQTT
    void setmqtt_int(int val=DEFAULT_MQTTPUB_INTERVAL) {
        //mqtt_int = val;
        if(tmqtt_pub)
            tmqtt_pub->cancel(); // cancel & delete

        extern void sendData();
        if(val){
            tmqtt_pub = new Task(val * TASK_SECOND, TASK_FOREVER, [this](){ if(embui.isMQTTconected()) sendData(); }, &ts, true, nullptr, [this](){TASK_RECYCLE; tmqtt_pub=nullptr;});
        }
    }
#endif

    LAMPMODE getMode() {return mode;}
    void setMode(LAMPMODE _mode) {mode=_mode;}

    void sendString(const char* text, const CRGB &letterColor, bool forcePrint = true, bool clearQueue = false);
    void sendStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, bool forcePrint = false, bool clearQueue = false, const int8_t textOffset = -128, const int16_t fixedPos = 0);
    void sendStringToLampDirect(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, bool forcePrint = false, bool clearQueue = false, const int8_t textOffset = -128, const int16_t fixedPos = 0);
    bool isPrintingNow() { return lampState.isStringPrinting; }
    LAMP();

    void handle();          // главная функция обработки эффектов

    void setFaderFlag(bool flag) {flags.isFaderON = flag;}
    bool getFaderFlag() {return flags.isFaderON;}
    void setClearingFlag(bool flag) {flags.isEffClearing = flag;}
    bool getClearingFlag() {return flags.isEffClearing;}
    void disableEffectsUntilText() {lampState.isEffectsDisabledUntilText = true; FastLED.clear();}
    void setOffAfterText() {lampState.isOffAfterText = true;}
    void setIsEventsHandled(bool flag) {flags.isEventsHandled = flag;}
    bool IsEventsHandled() {return flags.isEventsHandled;} // LOG(printf_P,PSTR("flags.isEventsHandled=%d\n"), flags.isEventsHandled);
    bool isLampOn() {return flags.ONflag;}
    bool isDebugOn() {return flags.isDebug;}
    bool isDrawOn() {return flags.isDraw;}
    void setDebug(bool flag) {flags.isDebug=flag; lampState.isDebug=flag;}
    void setButton(bool flag) {flags.isBtn=flag;}
    void setDrawBuff(bool flag) {
        flags.isDraw=flag;
        if(!flag){
            if (!drawbuff.empty()) {
                drawbuff.resize(0);
                drawbuff.shrink_to_fit();
            }
        } else if(drawbuff.empty()){
            drawbuff.resize(NUM_LEDS);
            //for(uint16_t i=0; i<NUM_LEDS; i++) {drawbuff[i] = CHSV(random(0,255),0,255);} // тест :)
        }
    }
    void writeDrawBuf(CRGB &color, uint16_t x, uint16_t y) { if(!drawbuff.empty()) { drawbuff[getPixelNumber(x,y)]=color; } }
    void writeDrawBuf(CRGB &color, uint16_t num) { if(!drawbuff.empty()) { drawbuff[num]=color; } }
    void fillDrawBuf(CRGB &color) { for(uint16_t i=0; i<drawbuff.size(); i++) drawbuff[i]=color; }
    void clearDrawBuf() { for(uint16_t i=0; i<drawbuff.size(); i++) drawbuff[i]=CRGB::Black; }
#ifdef USE_STREAMING
    bool isStreamOn() {return flags.isStream;}
    bool isDirect() {return flags.isDirect;}
    bool isMapping() {return flags.isMapping;}
    void setStream(bool flag) {flags.isStream = flag;}
    void setDirect(bool flag) {flags.isDirect = flag;}
    void setMapping(bool flag) {flags.isMapping = flag;}
#ifdef EXT_STREAM_BUFFER
    void setStreamBuff(bool flag) {
        if(!flag){
            if (!streambuff.empty()) {
                streambuff.resize(0);
                streambuff.shrink_to_fit();
            }
        } else if(streambuff.empty()){
            streambuff.resize(NUM_LEDS);
            //for(uint16_t i=0; i<NUM_LEDS; i++) {drawbuff[i] = CHSV(random(0,255),0,255);} // тест :)
        }
    }
    void writeStreamBuff(CRGB &color, uint16_t x, uint16_t y) { if(!streambuff.empty()) { streambuff[getPixelNumber(x,y)]=color; } }
    void writeStreamBuff(CRGB &color, uint16_t num) { if(!streambuff.empty()) { streambuff[num]=color; } }
    void fillStreamBuff(CRGB &color) { for(uint16_t i=0; i<streambuff.size(); i++) streambuff[i]=color; }
    void clearStreamBuff() { for(uint16_t i=0; i<streambuff.size(); i++) streambuff[i]=CRGB::Black; }
#endif
#endif
    bool isONMP3() {return flags.isOnMP3;}
    void setONMP3(bool flag) {flags.isOnMP3=flag;}
    bool isShowSysMenu() {return flags.isShowSysMenu;}
    void setIsShowSysMenu(bool flag) {flags.isShowSysMenu=flag;}
    void setMIRR_V(bool flag) {if (flag!=flags.MIRR_V) { flags.MIRR_V = flag; matrixflags.MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=flags.MIRR_H) { flags.MIRR_H = flag; matrixflags.MIRR_H = flag; FastLED.clear();}}
    void setTextMovingSpeed(uint8_t val) {tmStringStepTime.setInterval(val);}
    uint32_t getTextMovingSpeed() {return tmStringStepTime.getInterval();}
    void setTextOffset(uint8_t val) { txtOffset=val;}

    void setPlayTime(uint8_t val) {flags.playTime = val;}
    void setPlayName(bool flag) {flags.playName = flag;}
    void setPlayEffect(bool flag) {flags.playEffect = flag;}
    void setPlayMP3(bool flag) {flags.playMP3 = flag;}
    void setLimitAlarmVolume(bool flag) {flags.limitAlarmVolume = flag;}
    void setAlatmSound(ALARM_SOUND_TYPE val) {flags.alarmSound = val;}
    void setEqType(uint8_t val) {flags.MP3eq = val;}

    void periodicTimeHandle(bool force=false);

#ifdef TM1637_CLOCK
    void settm24 (bool flag) {flags.tm24 = flag;}
    void settmZero (bool flag) {flags.tmZero = flag;}
    bool isTm24() {return flags.tm24;}
    bool isTmZero() {return flags.tmZero;}
    void setTmBright(uint8_t val) {tmBright = val;}
    uint8_t getBrightOn() { return tmBright>>4; }
    uint8_t getBrightOff() { return tmBright&0x0F; }
#endif
#ifdef DS18B20
bool isTempDisp() {return flags.isTempOn;}
void setTempDisp(bool flag) {flags.isTempOn = flag;}
#endif
#ifdef VERTGAUGE
    bool isGauge() {return flags.isGaugeOn;}
    void setGauge(bool flag) {flags.isGaugeOn = flag;}
#endif
    void startAlarm(char *value = nullptr);
    void stopAlarm();
    void startDemoMode(uint8_t tmout = DEFAULT_DEMO_TIMER); // дефолтное значение, настраивается из UI
    void startNormalMode(bool forceOff=false);
    void restoreStored();
    void storeEffect();
#ifdef OTA
    void startOTAUpdate();
#endif
    void newYearMessageHandle();
    void setBFade(uint8_t val){ BFade = val; }
    uint8_t getBFade(){ return BFade; }
    void setNYMessageTimer(int in){ tmNewYearMessage.setInterval(in*60*1000); tmNewYearMessage.reset(); }
    void setNYUnixTime(time_t tm){ NEWYEAR_UNIXDATETIME = tm; }
    void setNumInList(bool flag) {flags.numInList = flag;}
    void setEffHasMic(bool flag) {flags.effHasMic = flag;}
    void setDRand(bool flag) {flags.dRand = flag; lampState.isRandDemo = (flag && mode==LAMPMODE::MODE_DEMO); }
    void setShowName(bool flag) {flags.showName = flag;}

    void setAlarmPT(uint8_t val) {alarmPT = val;}
    uint8_t getAlarmP() { return alarmPT>>4; }
    uint8_t getAlarmT() { return alarmPT&0x0F; }

    // ---------- служебные функции -------------

    void changePower(); // плавное включение/выключение
    void changePower(bool);

    /**
     * @brief - Change global brightness with or without fade effect
     * fade applied in non-blocking way
     * FastLED dim8 function applied internaly for natural brightness controll
     * @param uint8_t _tgtbrt - target brigtness level 0-255
     * @param bool fade - use fade effect on brightness change
     * @param bool natural - apply dim8 function for natural brightness controll
     */
    void setBrightness(const uint8_t _tgtbrt, const bool fade=false, const bool natural=true);

    /**
     * @brief - Get current brightness
     * FastLED brighten8 function applied internaly for natural brightness compensation
     * @param bool natural - return compensated or absolute brightness
     */
    uint8_t getBrightness(const bool natural=true);

    /**
     * @brief - переключатель эффектов для других методов,
     * может использовать фейдер, выбирать случайный эффект для демо
     * @param EFFSWITCH action - вид переключения (пред, след, случ.)
     * @param fade - переключаться через фейдер или сразу
     * @param effnb - номер эффекта
     * skip - системное поле - пропуск фейдера
     */
    void switcheffect(EFFSWITCH action = SW_NONE, bool fade = false, uint16_t effnb = EFF_ENUM::EFF_NONE, bool skip = false);

    /*
     * включает/выключает "демо"-таймер
     * @param SCHEDULER action - enable/disable/reset
     */
    void demoTimer(SCHEDULER action, uint8_t tmout = DEFAULT_DEMO_TIMER); // дефолтное значение, настраивается из UI

    /*
     * включает/выключает "эффект"-таймер
     * @param SCHEDULER action - enable/disable/reset
     */
    void effectsTimer(SCHEDULER action, uint32_t _begin = 0);


    ~LAMP() {}
private:
    LAMP(const LAMP&);  // noncopyable
    LAMP& operator=(const LAMP&);  // noncopyable
    std::vector<CRGB> ledsbuff; // вспомогательный буфер для слоя после эффектов
    std::vector<CRGB> drawbuff; // буфер для рисования
#if defined(USE_STREAMING) && defined(EXT_STREAM_BUFFER)
    std::vector<CRGB> streambuff; // буфер для трансляции
#endif
};

// Fader object
class LEDFader;
extern LEDFader *fader;
class LEDFader : public Task {
    LAMP *lmp;
    uint8_t _brt, _brtincrement;
    bool isSkipBrightness = false;
    LEDFader *f = nullptr;
    std::function<void(void)> _cb = nullptr;    // callback func to call upon completition
    LEDFader() = delete; 
public:
    inline LEDFader *getInstance() {return f;}
    void skipBrightness() { 
        isSkipBrightness = true;
        LOG(println,F("Fading canceled"));
        fader = nullptr;
        if(_cb)
            _cb();
        _cb = nullptr;
        this->cancel();        
    }
    LEDFader(Scheduler* aS, LAMP *_l, const uint8_t _targetbrightness, const uint32_t _duration, std::function<void(void)> callback)
        : Task((unsigned long)FADE_STEPTIME,
        (abs(_targetbrightness - _l->getBrightness()) > FADE_MININCREMENT * _duration / FADE_STEPTIME) ? (long)(_duration / FADE_STEPTIME) : (long)(abs(_targetbrightness - _l->getBrightness())/FADE_MININCREMENT) + 1,
        [this](){_brt += _brtincrement; lmp->brightness(_brt);},
        aS,
        false,
        nullptr,
        [this, _targetbrightness](){
            if(!isSkipBrightness){
                lmp->brightness(_targetbrightness);
                LOG(printf_P, PSTR("Fading to %d done\n"), _targetbrightness);
            }
            TASK_RECYCLE;
            fader = nullptr;
            if(_cb)
                _cb();
            _cb = nullptr;
        })
    {
        this->_cb = callback;
        this->lmp = _l;
        this->restart();

        this->_brt = lmp->getBrightness();
        int _steps = (abs(_targetbrightness - _brt) > FADE_MININCREMENT * _duration / FADE_STEPTIME) ? (long)(_duration / FADE_STEPTIME) : (long)(abs(_targetbrightness - _brt)/FADE_MININCREMENT);
        if (_steps < 3) {
            this->cancel();
            return;
        }
        f = this;
        _brtincrement = (_targetbrightness - _brt) / _steps;
        LOG(printf_P, PSTR("Fading to: %d\n"), _targetbrightness);
    }
    //~LEDFader() {LOG(println, F("Fader destructor"));}
};

/**
 * @brief - Non-blocking light fader, uses scheduler to globaly fade FastLED brighness within specified duration
 * @param LAMP *lamp - lamp instance
 * @param uint8_t _targetbrightness - end value for the brighness to fade to, FastLED dim8
 *                                   function applied internaly for natiral dimming
 * @param uint32_t _duration - fade effect duraion, ms
 * @param callback  -  callback-функция, которая будет выполнена после окончания затухания
 */
void fadelight(LAMP *lamp, const uint8_t _targetbrightness=0, const uint32_t _duration=FADE_TIME, std::function<void()> callback=nullptr);
#endif