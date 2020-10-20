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
#include "timeProcessor.h"
#include "events.h"
#include <Ticker.h>
#include "../../include/LList.h"
#include "interface.h"

#ifdef MIC_EFFECTS
#include "micFFT.h"
#endif

/*
 * Статические константы и строки
*/
static const char NY_MDG_STRING1[] PROGMEM = "До нового года осталось %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "C новым %d годом!";



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

#pragma pack(push,1)
typedef union {
struct {
    // ВНИМАНИЕ: порядок следования не менять, флаги не исключать, переводить в reserved!!! используется как битовый массив в конфиге!
    bool MIRR_V:1; // отзрекаливание по V
    bool MIRR_H:1; // отзрекаливание по H
    bool reserved0:1;
    bool ONflag:1; // флаг включения/выключения
    bool isFaderON:1; // признак того, что фейдер используется для эффектов
    bool isGlobalBrightness:1; // признак использования глобальной яркости для всех режимов
    bool reserved1:1;
    bool reserved2:1;
    bool reserved3:1;
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
    bool playTime:1; // воспроизводить время?
    bool playName:1; // воспроизводить имя?
    bool playEffect:1; // воспроизводить эффект?
    uint8_t alarmSound:3; // звук будильника ALARM_SOUND_TYPE
    bool playMP3:1; // режим mp3-плеера
    uint8_t reserved:3;
    // ВНИМАНИЕ: порядок следования не менять, флаги не исключать, переводить в reserved!!! используется как битовый массив в конфиге!
};
uint32_t lampflags; // набор битов для конфига
} LAMPFLAGS;
#pragma pack(pop)

// тут флаги, которые не нужно сохранять в конфиг
#pragma pack(push,1)
typedef union {
struct {
    bool dawnFlag:1; // флаг устанавливается будильником "рассвет"
    bool isStringPrinting:1; // печатается ли прямо сейчас строка?
    bool isEffectsDisabledUntilText:1; // признак отключения эффектов, пока выводится текст
    bool isOffAfterText:1; // признак нужно ли выключать после вывода текста
    uint8_t micAnalyseDivider:2; // делитель анализа микрофона 0 - выключен, 1 - каждый раз, 2 - каждый четвертый раз, 3 - каждый восьмой раз
    bool isCalibrationRequest:1; // находимся ли в режиме калибровки микрофона
};
uint16_t lampflags; // набор битов для конфига
} INTERNALFLAGS;
#pragma pack(pop)


class LAMP {
private:
    LAMPFLAGS flags;
    INTERNALFLAGS iflags;
    LAMPSTATE lampState; // текущее состояние лампы, которое передается эффектам

    byte txtOffset = 0; // смещение текста относительно края матрицы
    byte globalBrightness = 127; // глобальная яркость, пока что будет использоваться для демо-режимов
#ifdef LAMP_DEBUG
    uint8_t fps = 0;        // fps counter
    uint16_t avgfps = 0;    // avarage fps counter
#endif
    int mqtt_int;
    uint8_t bPin = BTN_PIN;        // пин кнопки
    uint16_t curLimit = CURRENT_LIMIT; // ограничение тока

    const uint16_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);
    const uint16_t minDim = ((WIDTH<HEIGHT)?WIDTH:HEIGHT);

    LAMPMODE mode = MODE_NORMAL; // текущий режим
    LAMPMODE storedMode = MODE_NORMAL; // предыдущий режим
    uint16_t storedEffect = (uint16_t)EFF_ENUM::EFF_NONE;

    PERIODICTIME enPeriodicTimePrint; // режим периодического вывода времени

#ifdef MIC_EFFECTS
    MICWORKER *mw = nullptr;
    float mic_noise = 0.0; // уровень шума в ед.
    float mic_scale = 1.0; // коэф. смещения
    float last_freq = 0.0; // последняя измеренная часота
    float samp_freq = 0.0; // часота семплирования
    uint8_t last_max_peak = 0; // последнее максимальное амплитудное значение (по модулю)
    uint8_t last_min_peak = 0; // последнее минимальное амплитудное значение (по модулю)
    MIC_NOISE_REDUCE_LEVEL noise_reduce = MIC_NOISE_REDUCE_LEVEL::NR_NONE; // уровень шумодава
    void micHandler();
#endif

    DynamicJsonDocument docArrMessages; // массив сообщений для вывода на лампу

    timerMinim tmConfigSaveTime;    // таймер для автосохранения
    timerMinim tmStringStepTime;    // шаг смещения строки, в мс
    timerMinim tmNewYearMessage;    // период вывода новогоднего сообщения

    time_t NEWYEAR_UNIXDATETIME=1609459200U;    // дата/время в UNIX формате, см. https://www.cy-pr.com/tools/time/ , 1609459200 => Fri, 01 Jan 2021 00:00:00 GMT

    // async fader and brightness control vars and methods
    uint8_t _brt, _steps;
    int8_t _brtincrement;
    Ticker _fadeTicker;             // планировщик асинхронного фейдера
    Ticker _fadeeffectTicker;       // планировщик затухалки между эффектами
    Ticker _demoTicker;             // планировщик Смены эффектов в ДЕМО
    Ticker _effectsTicker;          // планировщик обработки эффектов
    //Ticker _nextLoop;               // планировщик для тасок на ближайший луп
    void brightness(const uint8_t _brt, bool natural=true);     // низкоуровневая крутилка глобальной яркостью для других методов
    void fader(const uint8_t _tgtbrt, std::function<void(void)> callback=nullptr);          // обработчик затуания, вызывается планировщиком в цикле

    void effectsTick(); // обработчик эффектов

#ifdef VERTGAUGE
    byte xStep; byte xCol; byte yStep; byte yCol; // для индикатора
    unsigned long gauge_time = 0;
    unsigned gauge_val = 0;
    unsigned gauge_max = 0;
    byte gauge_hue = 0;
    void GaugeMix();
#endif
    void ConfigSaveCheck(){ if(tmConfigSaveTime.isReady()) { if(effects.autoSaveConfig()) tmConfigSaveTime.setInterval(0); } }

#ifdef OTA
    OtaManager otaManager;
#endif
    static void showWarning(const CRGB &color, uint32_t duration, uint16_t blinkHalfPeriod); // Блокирующая мигалка

    void doPrintStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, const int8_t textOffset = -128, const int16_t fixedPos = 0);
    bool fillStringManual(const char* text,  const CRGB &letterColor, bool stopText = false, bool isInverse = false, int32_t pos = 0, int8_t letSpace = LET_SPACE, int8_t txtOffset = TEXT_OFFSET, int8_t letWidth = LET_WIDTH, int8_t letHeight = LET_HEIGHT); // -2147483648
    void drawLetter(uint16_t letter, int16_t offset,  const CRGB &letterColor, uint8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight);
    uint8_t getFont(uint8_t asciiCode, uint8_t row);

    void alarmWorker();

    /*
     * вывод готового кадра на матрицу,
     * и перезапуск эффект-процессора
     */
    void frameShow(const uint32_t ticktime);

public:
    void lamp_init(const uint16_t curlimit);       // первичная инициализация Лампы
    EffectWorker effects; // объект реализующий доступ к эффектам
    EVENT_MANAGER events; // Объект реализующий доступ к событиям
    uint32_t getLampFlags() {return flags.lampflags;} // возвращает упакованные флаги лампы
    const LAMPFLAGS &getLampSettings() {return flags;} // возвращает упакованные флаги лампы
    //void setLampFlags(uint32_t _lampflags) {flags.lampflags=_lampflags;} // устананавливает упакованные флаги лампы
    void setbPin(uint8_t val) {bPin = val;}
    uint8_t getbPin() {return bPin;}
    void setcurLimit(uint16_t val) {curLimit = val;}
    uint16_t getcurLimit() {return curLimit;}
#ifdef MIC_EFFECTS
    void setMicCalibration() {iflags.isCalibrationRequest = true;}
    bool isMicCalibration() {return iflags.isCalibrationRequest;}
    float getMicScale() {return mic_scale;}
    void setMicScale(float scale) {mic_scale = scale;}
    float getMicNoise() {return mic_noise;}
    void setMicNoise(float noise) {mic_noise = noise;}
    void setMicNoiseRdcLevel(MIC_NOISE_REDUCE_LEVEL lvl) {noise_reduce = lvl;}
    MIC_NOISE_REDUCE_LEVEL getMicNoiseRdcLevel() {return noise_reduce;}
    uint8_t getMicMaxPeak() {return flags.isMicOn?last_max_peak:0;}
    uint8_t getMicMapMaxPeak() {return flags.isMicOn?((last_max_peak>(uint8_t)mic_noise)?(last_max_peak-(uint8_t)mic_noise)*2:1):0;}
    float getMicFreq() {return flags.isMicOn?last_freq:0;}
    uint8_t getMicMapFreq() {
        float minFreq=(log((float)(SAMPLING_FREQ>>1)/MICWORKER::samples));
        float scale = 255.0 / (log((float)HIGH_MAP_FREQ) - minFreq); 
        return (uint8_t)(flags.isMicOn?(log(last_freq)-minFreq)*scale:0);
    }
    void setMicOnOff(bool val) {
        flags.isMicOn = val;
        lampState.isMicOn = val;
        LList<UIControl*>&controls = effects.getControls();
        if(val){
            for(int i=3; i<controls.size(); i++) {
                if(controls[i]->getId()==7)
                    effects.worker->setDynCtrl(controls[i]);
            }
        } else {
            UIControl *ctrl = new UIControl(7,(CONTROL_TYPE)18,String(FPSTR(TINTF_020)), FPSTR(TCONST_FFFE), String(""), String(""), String(""));
            effects.worker->setDynCtrl(ctrl);
            delete ctrl;
        }
    }
    bool isMicOnOff() {return flags.isMicOn;}
    void setMicAnalyseDivider(uint8_t val) {iflags.micAnalyseDivider = val&3;}
#endif

#ifdef VERTGAUGE
    void GaugeShow(unsigned val, unsigned max, byte hue = 10);
#endif

    // Lamp brightness control (здесь методы работы с конфигурационной яркостью, не с LED!)
    byte getLampBrightness() { return flags.isGlobalBrightness? globalBrightness : (effects.getControls()[0]->getVal()).toInt();}
    byte getNormalizedLampBrightness() { return (byte)(((unsigned int)BRIGHTNESS) * (flags.isGlobalBrightness? globalBrightness : (effects.getControls()[0]->getVal()).toInt()) / 255);}
    void setLampBrightness(byte brg) { if (flags.isGlobalBrightness) setGlobalBrightness(brg); else effects.getControls()[0]->setVal(String(brg)); }
    void setGlobalBrightness(byte brg) {globalBrightness = brg;}
    void setIsGlobalBrightness(bool val) {flags.isGlobalBrightness = val;}
    bool IsGlobalBrightness() {return flags.isGlobalBrightness;}
    bool isAlarm() {return mode == MODE_ALARMCLOCK;}
    int getmqtt_int() {return mqtt_int;}
    void semqtt_int(int val) {mqtt_int = val;}

    LAMPMODE getMode() {return mode;}

    TimeProcessor timeProcessor;
    void refreshTimeManual() { timeProcessor.timerefresh(); }

    void sendString(const char* text, const CRGB &letterColor);
    void sendStringToLamp(const char* text = nullptr,  const CRGB &letterColor = CRGB::Black, bool forcePrint = false, const int8_t textOffset = -128, const int16_t fixedPos = 0);
    bool isPrintingNow() { return iflags.isStringPrinting; }
    LAMP();

    void handle();          // главная функция обработки эффектов

    void DelayedAutoEffectConfigSave(int in){ tmConfigSaveTime.setInterval(in); tmConfigSaveTime.reset(); effects.autoSaveConfig(false,true); }
    void setFaderFlag(bool flag) {flags.isFaderON = flag;}
    bool getFaderFlag() {return flags.isFaderON;}
    void setClearingFlag(bool flag) {flags.isEffClearing = flag;}
    bool getClearingFlag() {return flags.isEffClearing;}
    void disableEffectsUntilText() {iflags.isEffectsDisabledUntilText = true; FastLED.clear();}
    void setOffAfterText() {iflags.isOffAfterText = true;}
    void setIsEventsHandled(bool flag) {flags.isEventsHandled = flag;}
    bool IsEventsHandled() {return flags.isEventsHandled;}
    bool isLampOn() {return flags.ONflag;}
    bool isDebugOn() {return flags.isDebug;}
    void setDebug(bool flag) {flags.isDebug=flag; lampState.isDebug=flag;}
    bool isONMP3() {return flags.isOnMP3;}
    void setONMP3(bool flag) {flags.isOnMP3=flag;}
    bool isShowSysMenu() {return flags.isShowSysMenu;}
    void setIsShowSysMenu(bool flag) {flags.isShowSysMenu=flag;}
    void setMIRR_V(bool flag) {if (flag!=flags.MIRR_V) { flags.MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=flags.MIRR_H) { flags.MIRR_H = flag; FastLED.clear();}}
    void setTextMovingSpeed(uint8_t val) {tmStringStepTime.setInterval(val);}
    void setTextOffset(uint8_t val) { txtOffset=val;}
    void setPeriodicTimePrint(PERIODICTIME val) { enPeriodicTimePrint = val; }

    void setPlayTime(bool flag) {flags.playTime = flag;}
    void setPlayName(bool flag) {flags.playName = flag;}
    void setPlayEffect(bool flag) {flags.playEffect = flag;}
    void setPlayMP3(bool flag) {flags.playMP3 = flag;}
    void setAlatmSound(ALARM_SOUND_TYPE val) {flags.alarmSound = val;}
    void setEqType(uint8_t val) {flags.MP3eq = val;}

    void periodicTimeHandle();

    void startAlarm();
    void stopAlarm();
    void startDemoMode(byte tmout = 60); // дефолтное значение, настраивается из UI
    void startNormalMode();
#ifdef OTA
    void startOTAUpdate();
#endif
    void newYearMessageHandle();
    void setNYMessageTimer(int in){ tmNewYearMessage.setInterval(in*60*1000); tmNewYearMessage.reset(); }
    void setNYUnixTime(time_t tm){ NEWYEAR_UNIXDATETIME = tm; }
    void setNumInList(bool flag) {flags.numInList = flag;}
    void setEffHasMic(bool flag) {flags.effHasMic = flag;}
    void setDRand(bool flag) {flags.dRand = flag;}
    void setShowName(bool flag) {flags.showName = flag;}

    // ---------- служебные функции -------------
    uint16_t getmaxDim() {return maxDim;}
    uint16_t getminDim() {return minDim;}

    void changePower(); // плавное включение/выключение
    void changePower(bool);

    CRGB *getUnsafeLedsArray(){return leds;}

    // ключевая функция с подстройкой под тип матрицы, использует MIRR_V и MIRR_H
        uint32_t getPixelNumber(uint16_t x, uint16_t y) // получить номер пикселя в ленте по координатам
    {
        // хак с макроподстановкой, пусть живет пока
        #define MIRR_H flags.MIRR_H
        #define MIRR_V flags.MIRR_V
        
        if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
        {
            return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + THIS_X)%NUM_LEDS;
        }
        else                                                      // если нечётная строка
        {
            return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + _WIDTH - THIS_X - 1)%NUM_LEDS;
        }
    
        #undef MIRR_H
        #undef MIRR_V
    }

    // перегрузка для работы с буффером
    uint32_t getPixelNumberBuff(uint16_t x, uint16_t y, uint8_t W , uint8_t H, uint16_t NL) // получить номер пикселя в ленте по координатам
    {
        NL = W*H;

        uint16_t _THIS_Y = y;
        uint16_t _THIS_X = x;
        
        if ((_THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
        {
            return ((uint32_t)_THIS_Y * SEGMENTS * W + _THIS_X)%NL;
        }
        else                                                      // если нечётная строка
        {
            return ((uint32_t)_THIS_Y * SEGMENTS * W + W - _THIS_X - 1)%NL;
        }
    
    }

    /*
     * Change global brightness with or without fade effect
     * fade applied in non-blocking way
     * FastLED dim8 function applied internaly for natural brightness controll
     * @param uint8_t _tgtbrt - target brigtness level 0-255
     * @param bool fade - use fade effect on brightness change
     * @param bool natural - apply dim8 function for natural brightness controll
     */
    void setBrightness(const uint8_t _tgtbrt, const bool fade=false, const bool natural=true);

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
     * переключатель эффектов для других методов,
     * может использовать фейдер, выбирать случайный эффект для демо
     * @param EFFSWITCH action - вид переключения (пред, след, случ.)
     * @param fade - переключаться через фейдер или сразу
     * @param effnb - номер эффекта
     * skip - системное поле - пропуск фейдера
     */
    void switcheffect(EFFSWITCH action = SW_NONE, bool fade = false, uint16_t effnb = EFF_ENUM::EFF_NONE, bool skip = false);

    /*
     * включает/выключает "демо"-таймер
     * @param TICKER action - enable/disable/reset
     */
    void demoTimer(SCHEDULER action, byte tmout = 60); // дефолтное значение, настраивается из UI

    /*
     * включает/выключает "эффект"-таймер
     * @param TICKER action - enable/disable/reset
     */
    void effectsTimer(SCHEDULER action);


    ~LAMP() {}
private:
    LAMP(const LAMP&);  // noncopyable
    LAMP& operator=(const LAMP&);  // noncopyable
    CRGB leds[NUM_LEDS];
#ifdef USELEDBUF
    //CRGB ledsbuff[NUM_LEDS]; // буфер под эффекты
    std::vector<CRGB> ledsbuff;
#endif
};

#endif