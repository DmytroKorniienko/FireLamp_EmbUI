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
//#include "time.h"
#include "timeProcessor.h"

#include <FastLed.h>

typedef enum _LAMPMODE {
  MODE_NORMAL = 0,
  MODE_DEMO,
  MODE_WHITELAMP,
  MODE_ALARMCLOCK,
  MODE_OTA
} LAMPMODE;

class LAMP {
private:
#pragma pack(push,1)
 struct {
    bool MIRR_V:1; // отзрекаливание по V
    bool MIRR_H:1; // отзрекаливание по H
    bool dawnFlag:1; // флаг устанавливается будильником "рассвет"
    bool ONflag:1; // флаг включения/выключения
    bool manualOff:1;
    bool loadingFlag:1; // флаг для начальной инициализации эффекта
    bool isFaderOn:1; // признак того, что выполняется фейдер текущего эффекта
    bool manualFader:1; // ручной или автоматический фейдер
    bool isGlobalBrightness:1; // признак использования глобальной яркости для всех режимов
    bool isFirstHoldingPress:1; // флаг: только начали удерживать?
    bool startButtonHolding:1; // кнопка удерживается
    bool buttonEnabled:1; // кнопка обрабатывается если true
    bool brightDirection:1;
    bool speedDirection:1;
    bool scaleDirection:1;
    bool setDirectionTimeout:1; // флаг: начало отсчета таймаута на смену направления регулировки
    bool isStringPrinting:1; // печатается ли прямо сейчас строка?
 };
 #pragma pack(pop)
    //Button
    byte numHold = 0; // режим удержания
    byte txtOffset = 0; // смещение текста относительно края матрицы
    byte globalBrightness = BRIGHTNESS; // глобальная яркость, пока что будет использоваться для демо-режимов

    const int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    LAMPMODE mode = MODE_NORMAL; // текущий режим
    LAMPMODE storedMode = MODE_NORMAL; // предыдущий режим
    EFF_ENUM storedEffect = EFF_NONE;

    uint32_t effTimer; // таймер для эффекта, сравнивается со скоростью текущего эффекта
    uint32_t effDelay; // доп. задержка для эффектов

    PERIODICTIME enPeriodicTimePrint; // режим периодического вывода времени

    void(*updateParmFunc)() = nullptr; // функтор обновления параметров

    DynamicJsonDocument docArrMessages; // массив сообщений для вывода на лампу

    timerMinim tmFaderTimeout;
    timerMinim tmFaderStepTime;
    timerMinim tmDemoTimer;         // смена эффекта в демо режиме по дабл-клику из выключенного состояния, таймаут N секунд
    timerMinim tmConfigSaveTime;    // таймер для автосохранения
    timerMinim tmNumHoldTimer;      // таймаут удержания кнопки в мс
    timerMinim tmStringStepTime;    // шаг смещения строки, в мс
    timerMinim tmNewYearMessage;    // период вывода новогоднего сообщения
    
    time_t NEWYEAR_UNIXDATETIME=1609459200U;    // дата/время в UNIX формате, см. https://www.cy-pr.com/tools/time/ , 1609459200 => Fri, 01 Jan 2021 00:00:00 GMT


#ifdef ESP_USE_BUTTON
    GButton touch;               
    void buttonTick(); // обработчик кнопки
    timerMinim tmChangeDirectionTimer;     // таймаут смены направления увеличение-уменьшение при удержании кнопки
    void changeDirection(byte numHold);
    void debugPrint();
#endif
    void effectsTick(); // обработчик эффектов

#ifdef VERTGAUGE
    byte xStep; byte xCol; byte yStep; byte yCol; // для индикатора
    void GaugeShow();
#endif
    void ConfigSaveCheck(){ if(tmConfigSaveTime.isReady()) {if(effects.autoSaveConfig()) tmConfigSaveTime.setInterval(0); } }
    bool faderTick();

#ifdef OTA
    OtaManager otaManager;
#endif
    static void showWarning(CRGB::HTMLColorCode color, uint32_t duration, uint16_t blinkHalfPeriod); // Блокирующая мигалка

    void doPrintStringToLamp(const char* text = nullptr, CRGB::HTMLColorCode letterColor = CRGB::Black);
    bool fillStringManual(const char* text, CRGB::HTMLColorCode letterColor, bool stopText = false, bool isInverse = false, int8_t letSpace = LET_SPACE, int8_t txtOffset = TEXT_OFFSET, int8_t letWidth = LET_WIDTH, int8_t letHeight = LET_HEIGHT);
    void drawLetter(uint16_t letter, int16_t offset, CRGB::HTMLColorCode letterColor, int8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight);
    uint8_t getFont(uint8_t asciiCode, uint8_t row);

public:
    EffectWorker effects; // объект реализующий доступ к эффектам
    
    bool isLoading() {if(!loadingFlag) return loadingFlag; else {loadingFlag=false; return true;}}
    void setLoading(bool flag=true) {loadingFlag = flag;}
    byte getLampBrightness() { return (mode==MODE_DEMO || isGlobalBrightness)?globalBrightness:effects.getBrightness();}
    byte getNormalizedLampBrightness() { return (byte)(((unsigned int)BRIGHTNESS)*((mode==MODE_DEMO || isGlobalBrightness)?globalBrightness:effects.getBrightness())/255);}
    void setLampBrightness(byte brg) { if(mode==MODE_DEMO || isGlobalBrightness) setGlobalBrightness(brg); else effects.setBrightness(brg);}
    void setGlobalBrightness(byte brg) {globalBrightness = brg;}
    void setIsGlobalBrightness(bool val) {isGlobalBrightness = val;}
    bool IsGlobalBrightness() {return isGlobalBrightness;}
    void restartDemoTimer() {tmDemoTimer.reset();}
    LAMPMODE getMode() {return mode;}
    void updateParm(void(*f)()) { updateParmFunc=f; }

    TimeProcessor timeProcessor;
    void refreshTimeManual() { timeProcessor.handleTime(true); }

    void sendStringToLamp(const char* text = nullptr, CRGB::HTMLColorCode letterColor = CRGB::Black);
    
    LAMP();

    void handle(); // главная функция обработки эффектов
    void init();

    void ConfigSaveSetup(int in){ tmConfigSaveTime.setInterval(in); tmConfigSaveTime.reset(); }
    void setOnOff(bool flag) {ONflag = flag; changePower(flag);}
    bool isLampOn() {return ONflag;}
    void setMIRR_V(bool flag) {if (flag!=MIRR_V) { MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=MIRR_H) { MIRR_H = flag; FastLED.clear();}}
    void setTextMovingSpeed(uint8_t val) {tmStringStepTime.setInterval(val);}
    void setTextOffset(uint8_t val) { txtOffset=val;}
    void setPeriodicTimePrint(PERIODICTIME val) { enPeriodicTimePrint = val; }

    void periodicTimeHandle();

    void startFader(bool isManual);

    void startDemoMode();
    void startNormalMode();
    void startOTAUpdate();

    void startOTA() { otaManager.RequestOtaUpdate(); if (otaManager.RequestOtaUpdate()) { startOTAUpdate(); } }

    void newYearMessageHandle();
    void setNYMessageTimer(int in){ tmNewYearMessage.setInterval(in*60*1000); tmNewYearMessage.reset(); }
    void setNYUnixTime(time_t tm){ NEWYEAR_UNIXDATETIME = tm; }

    // ---------- служебные функции -------------
    uint32_t getEffDelay() {return effDelay;}
    uint8_t getmaxDim() {return maxDim;}
    void setEffDelay(uint32_t dl) {effDelay=dl;}

    void changePower(); // плавное включение/выключение
    void changePower(bool);

    uint32_t getPixelNumber(uint16_t x, uint16_t y); // получить номер пикселя в ленте по координатам
    uint32_t getPixColor(uint32_t thisSegm); // функция получения цвета пикселя по его номеру
    uint32_t getPixColorXY(uint16_t x, uint16_t y) { return getPixColor(getPixelNumber(x, y)); } // функция получения цвета пикселя в матрице по его координатам
    void fillAll(CRGB color); // залить все
    void drawPixelXY(int16_t x, int16_t y, CRGB color); // функция отрисовки точки по координатам X Y
    CRGB *getUnsafeLedsArray(){return leds;}
    void setLeds(uint16_t idx, CHSV val) { leds[idx] = val; }
    void setLeds(uint16_t idx, CRGB val) { leds[idx] = val; }
    void setLedsfadeToBlackBy(uint16_t idx, uint8_t val) { leds[idx].fadeToBlackBy(val); }
    void setLedsNscale8(uint16_t idx, uint8_t val) { leds[idx].nscale8(val); }
    //fadeToBlackBy
    void dimAll(uint8_t value) { for (uint16_t i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(value); } }
    CRGB getLeds(uint16_t idx) { return leds[idx]; }
    CRGB *getLeds() { return leds; }
    void blur2d(uint8_t val) {::blur2d(leds,WIDTH,HEIGHT,val);}
    ~LAMP() {}
private:
    LAMP(const LAMP&);  // noncopyable
    LAMP& operator=(const LAMP&);  // noncopyable
    CRGB leds[NUM_LEDS];
#ifdef USELEDBUF
    CRGB ledsbuff[NUM_LEDS]; // буфер под эффекты
#endif
};

