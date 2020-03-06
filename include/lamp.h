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
    void buttonTick(); // обработчик кнопки
#endif
    void effectsTick(); // обработчик эффектов

#ifdef VERTGAUGE
    byte xStep; byte xCol; byte yStep; byte yCol; // для индикатора
#endif

#ifdef ESP_USE_BUTTON
    bool brightDirection, speedDirection, scaleDirection;
    timerMinim tmChangeDirectionTimer;     // таймаут смены направления увеличение-уменьшение при удержании кнопки
    bool setDirectionTimeout = false;                    // флаг: начало отсчета таймаута на смену направления регулировки

    void changeDirection(byte numHold){
      if(!startButtonHolding || (tmChangeDirectionTimer.isReady() && setDirectionTimeout)){
        switch(numHold){
          case 1: brightDirection = !brightDirection; break;
          case 2: speedDirection = !speedDirection; break;
          case 3: scaleDirection = !scaleDirection; break;
        }
        setDirectionTimeout = false;
      }
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("changeDirection %d, %d, %d\n"), brightDirection, speedDirection, scaleDirection);
#endif
    }

  void debugPrint(){
      LOG.printf_P(PSTR("lampMode: %d numHold: %d currentMode: %d brightness: %d speed: %d scale: %d\n"), mode, numHold, effects.getEn(), getLampBrightness(), effects.getSpeed(), effects.getScale());
  }
#endif

#ifdef VERTGAUGE
    void GaugeShow() {
      byte ind;
      if(!startButtonHolding) return;
      
      switch (numHold) {    // индикатор уровня яркости/скорости/масштаба
#if defined(VERTGAUGE)
        case 1:
          //if(currentMode==EFF_WHITE_COLOR)
          //  ind = sqrt((255.0/(BRIGHTNESS-MIN_WHITE_COLOR_BRGHT))*(modes[currentMode].Brightness-MIN_WHITE_COLOR_BRGHT) + 1); // привести к полной шкале
          //else
            ind = sqrt((255.0/BRIGHTNESS)*getLampBrightness() + 1); // привести к полной шкале (TODO: проверить и исправить)
          for (byte x = 0; x <= xCol*(xStep-1) ; x+=xStep) {
            for (byte y = 0; y < HEIGHT ; y++) {
              if (ind > y)
                drawPixelXY(x, y, CHSV(10, 255, 255));
              else
                drawPixelXY(x, y,  0);
            }
          }
          break;
        case 2:
          ind = sqrt(effects.getSpeed() - 1);
          for (byte x = 0; x <= xCol*(xStep-1) ; x+=xStep) {
            for (byte y = 0; y <= HEIGHT ; y++) {
              if (ind <= y)
                drawPixelXY(x, HEIGHT-1-y, CHSV(100, 255, 255));
              else
                drawPixelXY(x, HEIGHT-1-y,  0);
            }
          }
          break;
        case 3:
          ind = sqrt(effects.getScale() + 1);
          for (byte x = 0; x <= xCol*(xStep-1) ; x+=xStep) {
            for (byte y = 0; y < HEIGHT ; y++) {
              if (ind > y)
                drawPixelXY(x, y, CHSV(150, 255, 255));
              else
                drawPixelXY(x, y,  0);
            }
          }
          break;
#else
        case 1:
          //if(currentMode==EFF_WHITE_COLOR)
          //  ind = sqrt((255.0/(BRIGHTNESS-MIN_WHITE_COLOR_BRGHT))*(modes[currentMode].Brightness-MIN_WHITE_COLOR_BRGHT) + 1); // привести к полной шкале
          //else
            ind = sqrt((255.0/BRIGHTNESS)*getLampBrightness() + 1); // привести к полной шкале (TODO: проверить и исправить)
          for (byte y = 0; y <= yCol*(yStep-1) ; y+=yStep) {
            for (byte x = 0; x < WIDTH ; x++) {
              if (ind > x)
                drawPixelXY((x+y)%WIDTH, y, CHSV(10, 255, 255));
              else
                drawPixelXY((x+y)%WIDTH, y,  0);
            }
          }
          break;
        case 2:
          ind = sqrt(effects.getSpeed() - 1);
          for (byte y = 0; y <= yCol*(yStep-1) ; y+=yStep) {
            for (byte x = 0; x <= WIDTH ; x++) {
              if (ind < x)
                drawPixelXY((WIDTH-x+y)%WIDTH, y, CHSV(100, 255, 255));
              else
                drawPixelXY((WIDTH-x+y)%WIDTH, y,  0);
            }
          }
          break;
        case 3:
          ind = sqrt(effects.getScale() + 1);
          for (byte y = 0; y <= yCol*(yStep-1) ; y+=yStep) {
            for (byte x = 0; x < WIDTH ; x++) {
              if (ind > x)
                drawPixelXY((x+y)%WIDTH, y, CHSV(150, 255, 255));
              else
                drawPixelXY((x+y)%WIDTH, y,  0);
            }
          }
          break;
#endif
      }
    }
#endif
    void ConfigSaveCheck(){ if(tmConfigSaveTime.isReady()) {if(effects.autoSaveConfig()) tmConfigSaveTime.setInterval(0); } }

    bool faderTick(){
      static byte faderStep = 1;

      if(!tmFaderTimeout.isReady()){
        if(isFaderOn && tmFaderStepTime.isReady()) {
#ifdef LAMP_DEBUG
//LOG.printf_P(PSTR("leds[1]=%d %d %d\n"),leds[1].red,leds[1].green,leds[1].blue);
#endif
          faderStep++;
          float chVal = ((float)globalBrightness*FADERSTEPTIME)/FADERTIMEOUT;
          for(int led = 0 ; led < NUM_LEDS ; led++ ) {
            //leds[led]/=((faderStep>5)?2:1);
            leds[led].subtractFromRGB((uint8_t)(chVal*faderStep*0.33));
          }
        }
      } else {
        tmFaderTimeout.setInterval(0); // отключить до следующего раза, также переключаем эффект на новый, заодно запоминаем яркость текущего эффекта
        //storeEffBrightness = modes[lamp mode].Brightness;
        loadingFlag = true; // некоторые эффекты требуют начальной иницализации, поэтому делаем так...
        isFaderOn = false;
        faderStep = 1;

        if(!manualFader){
          if(mode==MODE_DEMO){
            if(RANDOM_DEMO)
              effects.moveBy(random(0, effects.getModeAmount()));
            else
              effects.moveNext();

#ifdef LAMP_DEBUG
          LOG.printf_P(PSTR("%s Demo mode: %d, storedEffect: %d\n"),(RANDOM_DEMO?F("Random"):F("Seq")) , mode, storedEffect);
#endif
          }
        }
        EFFECT *currentEffect = effects.getCurrent();
        if(currentEffect->func!=nullptr)
          currentEffect->func(getUnsafeLedsArray(), currentEffect->param); // отрисовать текущий эффект
        manualFader = false;
      }

      return isFaderOn;
    }


public:
    EffectWorker effects; // объект реализующий доступ к эффектам
    
    bool isLoading() {if(!loadingFlag) return loadingFlag; else {loadingFlag=false; return true;}}
    void setLoading(bool flag=true) {loadingFlag = flag;}
    byte getLampBrightness() { return (mode==MODE_DEMO)?globalBrightness:effects.getBrightness();}
    void setLampBrightness(byte brg) { if(mode==MODE_DEMO) globalBrightness = brg; else effects.setBrightness(brg);}
    void restartDemoTimer() {tmDemoTimer.reset();}

    LAMP() : tmFaderTimeout(0), tmFaderStepTime(FADERSTEPTIME), tmDemoTimer(DEMO_TIMEOUT*1000), tmConfigSaveTime(0), tmNumHoldTimer(NUMHOLD_TIME)
    {

      init(); // инициализация и настройка лампы

#ifdef ESP_USE_BUTTON
    tmChangeDirectionTimer.setInterval(NUMHOLD_TIME);     // таймаут смены направления увеличение-уменьшение при удержании кнопки
#endif

#ifdef VERTGAUGE
      if(VERTGAUGE){
        xStep = WIDTH / 4;
        xCol = 4;
        if(xStep<2) {
          xStep = WIDTH / 3;
          xCol = 3;
        } else if(xStep<2) {
          xStep = WIDTH / 2;
          xCol = 2;
        } else if(xStep<2) {
          xStep = 1;
          xCol = 1;
        }
      
        yStep = HEIGHT / 4;
        yCol = 4;
        if(yStep<2) {
          yStep = HEIGHT / 3;
          yCol = 3;
        } else if(yStep<2) {
          yStep = HEIGHT / 2;
          yCol = 2;
        } else if(yStep<2) {
          yStep = 1;
          yCol = 1;
        }
    }
#endif
    }
    void handle(); // главная функция обработки эффектов
    void init();

    void ConfigSaveSetup(int in){ tmConfigSaveTime.setInterval(in); tmConfigSaveTime.reset(); }
    void setOnOff(bool flag) {ONflag = flag; changePower();}
    void setMIRR_V(bool flag) {if (flag!=MIRR_V) { MIRR_V = flag; FastLED.clear();}}
    void setMIRR_H(bool flag) {if (flag!=MIRR_H) { MIRR_H = flag; FastLED.clear();}}

    void startFader(bool isManual=false)
    {
        tmFaderTimeout.setInterval(FADERTIMEOUT); // взводим таймер фейдера
        tmFaderTimeout.reset();
        isFaderOn = true;
        manualFader = isManual;
    }

    // ---------- служебные функции -------------
    uint32_t getEffDelay() {return effDelay;}
    uint8_t getmaxDim() {return maxDim;}
    void setEffDelay(uint32_t dl) {effDelay=dl;}

    void changePower() // плавное включение/выключение
    {
      if (ONflag){
        for (uint8_t i = 0U; i < getLampBrightness(); i = constrain(i + 8, 0, getLampBrightness()))
        {
          FastLED.setBrightness(i);
          delay(1);
          FastLED.show();
        }

        FastLED.setBrightness(getLampBrightness());
        delay(2);
        FastLED.show();
      }
      else
      {
        for (uint8_t i = getLampBrightness(); i > 0; i = constrain(i - 8, 0, getLampBrightness()))
        {
          FastLED.setBrightness(i);
          delay(1);
          FastLED.show();
        }
        FastLED.clear();
        delay(2);
        FastLED.show();
      }

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
      digitalWrite(MOSFET_PIN, (ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
#endif
    }

    // получить номер пикселя в ленте по координатам
    uint32_t getPixelNumber(uint16_t x, uint16_t y)
    {
      if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
      {
        return ((uint32_t)THIS_Y * _WIDTH + THIS_X);
      }
      else                                                      // если нечётная строка
      {
        return ((uint32_t)THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
      }
    }

    // функция получения цвета пикселя по его номеру
    uint32_t getPixColor(uint32_t thisSegm)
    {
      uint32_t thisPixel = thisSegm * SEGMENTS;
      if (thisPixel > NUM_LEDS - 1) return 0;
      return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
    }

    // функция получения цвета пикселя в матрице по его координатам
    uint32_t getPixColorXY(uint16_t x, uint16_t y)
    {
      return getPixColor(getPixelNumber(x, y));
    }

    // залить все
    void fillAll(CRGB color)
    {
      for (int32_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = color;
      }
    }

    // функция отрисовки точки по координатам X Y
    void drawPixelXY(int16_t x, int16_t y, CRGB color)
    {
      if (x < 0 || x > (int16_t)(WIDTH - 1) || y < 0 || y > (int16_t)(HEIGHT - 1)) return;
      uint32_t thisPixel = getPixelNumber((uint16_t)x, (uint16_t)y) * SEGMENTS;
      for (uint16_t i = 0; i < SEGMENTS; i++)
      {
        leds[thisPixel + i] = color;
      }
    }

    CRGB *getUnsafeLedsArray(){return leds;}

    void setLeds(uint16_t idx, CHSV val)
    {
      leds[idx] = val;
    }

    void setLeds(uint16_t idx, CRGB val)
    {
      leds[idx] = val;
    }

    void setLedsfadeToBlackBy(uint16_t idx, uint8_t val)
    {
      leds[idx].fadeToBlackBy(val);
    }

    void setLedsNscale8(uint16_t idx, uint8_t val)
    {
      leds[idx].nscale8(val);
    }

    void dimAll(uint8_t value) {
      for (uint16_t i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(value); //fadeToBlackBy
      }
    }

    CRGB getLeds(uint16_t idx)
    {
      return leds[idx];
    }

    ~LAMP() {}
};

