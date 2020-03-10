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

#include <FastLed.h>

typedef enum _LAMPMODE {
  MODE_NORMAL = 0,
  MODE_DEMO,
  MODE_WHITELAMP,
  MODE_ALARMCLOCK
} LAMPMODE;

class LAMP {
private:
    const int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    void(*updateParmFunc)() = nullptr; // функтор обновления параметров

    bool MIRR_V; // отзрекаливание по V
    bool MIRR_H; // отзрекаливание по H

    LAMPMODE mode = MODE_NORMAL; // текущий режим
    LAMPMODE storedMode = MODE_NORMAL; // предыдущий режим
    EFF_ENUM storedEffect = EFF_NONE;
    LAMP(const LAMP&);  // noncopyable
    LAMP& operator=(const LAMP&);  // noncopyable

    CRGB leds[NUM_LEDS];
#ifdef USELEDBUF
    CRGB ledsbuff[NUM_LEDS]; // буфер под эффекты
#endif
    bool dawnFlag = false; // флаг устанавливается будильником "рассвет"
    bool ONflag = false; // флаг включения/выключения
    bool manualOff = false;
    bool loadingFlag = true; // флаг для начальной инициализации эффекта
    bool isFaderOn = false; // признак того, что выполняется фейдер текущего эффекта
    bool manualFader = true; // ручной или автоматический фейдер
    uint32_t effTimer; // таймер для эффекта, сравнивается со скоростью текущего эффекта
    uint32_t effDelay; // доп. задержка для эффектов

    //Button
    bool isFirstHoldingPress = false; // флаг: только начали удерживать?
    bool startButtonHolding = false; // кнопка удерживается
    byte numHold = 0; // режим удержания
    bool buttonEnabled = true; // кнопка обрабатывается если true

    //tmFaderTimeout(0), tmFaderStepTime(FADERSTEPTIME), tmDemoTimer(DEMO_TIMEOUT*1000), tmConfigSaveTime(0)

    timerMinim tmFaderTimeout;
    timerMinim tmFaderStepTime;
    timerMinim tmDemoTimer;    // смена эффекта в демо режиме по дабл-клику из выключенного состояния, таймаут N секунд
    timerMinim tmConfigSaveTime; // таймер для автосохранения
    timerMinim tmNumHoldTimer;             // таймаут удержания кнопки в мс

    byte globalBrightness = BRIGHTNESS; // глобальная яркость, пока что будет использоваться для демо-режимов
#ifdef ESP_USE_BUTTON
    GButton touch;               
    void buttonTick(); // обработчик кнопки
    bool brightDirection, speedDirection, scaleDirection;
    timerMinim tmChangeDirectionTimer;     // таймаут смены направления увеличение-уменьшение при удержании кнопки
    bool setDirectionTimeout = false;                    // флаг: начало отсчета таймаута на смену направления регулировки
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

public:
    EffectWorker effects; // объект реализующий доступ к эффектам
    
    bool isLoading() {if(!loadingFlag) return loadingFlag; else {loadingFlag=false; return true;}}
    void setLoading(bool flag=true) {loadingFlag = flag;}
    byte getLampBrightness() { return (mode==MODE_DEMO)?globalBrightness:effects.getBrightness();}
    byte getNormalizedLampBrightness() { return (byte)(((uint32)BRIGHTNESS)*((mode==MODE_DEMO)?globalBrightness:effects.getBrightness())/255);}
    void setLampBrightness(byte brg) { if(mode==MODE_DEMO) globalBrightness = brg; else effects.setBrightness(brg);}
    void restartDemoTimer() {tmDemoTimer.reset();}

    void updateParm(void(*f)()) { updateParmFunc=f; }

    LAMP();

    void handle(); // главная функция обработки эффектов
    void init();

    void ConfigSaveSetup(int in){ tmConfigSaveTime.setInterval(in); tmConfigSaveTime.reset(); }
    void setOnOff(bool flag) {ONflag = flag; changePower(flag);}
    bool isLampOn() {return ONflag;}
    void setMIRR_V(bool flag) {if (flag!=MIRR_V) { MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=MIRR_H) { MIRR_H = flag; FastLED.clear();}}

    void startFader(bool isManual);

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
    ~LAMP() {}
};

