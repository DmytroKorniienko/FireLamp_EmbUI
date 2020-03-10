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

#include "lamp.h"
#include "main.h"
#include "misc.h"

void LAMP::init()
{
  FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS)  /*.setCorrection(TypicalLEDStrip)*/;
  FastLED.setBrightness(BRIGHTNESS);                          // установка яркости
  if (CURRENT_LIMIT > 0){
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); // установка максимального тока БП
  }
  FastLED.clear();                                            // очистка матрицы

  // ПИНЫ
#ifdef MOSFET_PIN                                         // инициализация пина, управляющего MOSFET транзистором в состояние "выключен"
  pinMode(MOSFET_PIN, OUTPUT);
#ifdef MOSFET_LEVEL
  digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
#endif
#endif

#ifdef ALARM_PIN                                          // инициализация пина, управляющего будильником в состояние "выключен"
  pinMode(ALARM_PIN, OUTPUT);
#ifdef ALARM_LEVEL
  digitalWrite(ALARM_PIN, !ALARM_LEVEL);
#endif
#endif

  // TELNET
#if defined(LAMP_DEBUG) && DEBUG_TELNET_OUTPUT
  telnetServer.begin();
  for (uint8_t i = 0; i < 100; i++)                         // пауза 10 секунд в отладочном режиме, чтобы успеть подключиться по протоколу telnet до вывода первых сообщений
  {
    handleTelnetClient();
    delay(100);
    ESP.wdtFeed();
  }
#endif

#ifdef ESP_USE_BUTTON
  touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch.setTimeout(BUTTON_TIMEOUT);
  //touch.setDebounce(250);
  //touch.setTickMode(true);
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

void LAMP::handle()
{
#ifdef ESP_USE_BUTTON
  static unsigned long button_check;
  if (buttonEnabled && button_check + 30 < millis()) // раз в 30 мс проверяем кнопку
  {
    buttonTick(); // обработчик кнопки
    button_check = millis();
  }
#endif

  effectsTick(); // обработчик эффектов

#ifdef USE_NTP
  timeTick(); // обработчик будильника "рассвет"
#endif

#if defined(LAMP_DEBUG) && DEBUG_TELNET_OUTPUT
  handleTelnetClient();
#endif

}

#ifdef ESP_USE_BUTTON
void LAMP::buttonTick()
{
  if (!buttonEnabled)                                       // события кнопки не обрабатываются, если она заблокирована
  {
    return;
  }

  //ONflag = true; // Отладка

  touch.tick();

  if (tmNumHoldTimer.isReady() && !startButtonHolding) { // сброс текущей комбинации в обычном режиме, если уже не нажата
      numHold = 0;
  }

  if (!ONflag) { // Обработка из выключенного состояния
    if (touch.isDouble()) { // Демо-режим, с переключением каждые 30 секунд для двойного клика в выключенном состоянии
      mode = MODE_DEMO;
      randomSeed(millis());
      effects.moveBy(random(0, MODE_AMOUNT));
      FastLED.setBrightness(getNormalizedLampBrightness());
      ONflag = true;
      tmDemoTimer.reset(); // момент включения для таймаута в DEMOTIME
      changePower();
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("Demo mode: %d, storedEffect: %d\n"), mode, storedEffect);
#endif
      return;
    }
    
    if (touch.isHolded()) {
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("Удержание кнопки из выключенного состояния\n"));
#endif
      // Включаем белую лампу в полную яркость
      numHold = 1;
      mode = MODE_WHITELAMP;
      effects.moveBy(EFF_WHITE_COLOR);
      effects.setBrightness(255);
      FastLED.setBrightness(getNormalizedLampBrightness());
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("lamp mode: %d, storedEffect: %d, LampBrightness=%d\n"), mode, storedEffect, getNormalizedLampBrightness());
#endif
      ONflag = true;
      startButtonHolding = true;
      setDirectionTimeout = false;
      tmNumHoldTimer.reset();
      tmChangeDirectionTimer.reset();
      brightDirection = 1;
      changePower();
      return;
    }
  } 

  // кнопка только начала удерживаться
  if (ONflag && (touch.isHolded())){
    int clicks = touch.getHoldClicks();
#ifdef LAMP_DEBUG
    LOG.printf_P(PSTR("touch.getHoldClicks()=%d\n"), clicks);
#endif
    startButtonHolding = true;
    setDirectionTimeout = false;
    isFirstHoldingPress = true;
    switch (clicks){
      case 0U: {
        if(!numHold){
          numHold = 1;
        }
        break;
      }
      case 1U: {
        //if(!numHold)
          numHold = 2;
        break;
      }
      case 2U: {
        //if(!numHold)
          numHold = 3;
        break;
      }
    }
  }

  // кнопка нажата и удерживается
  if (ONflag && touch.isStep())
  {
    if(!isFirstHoldingPress && (((getLampBrightness() == 255 || getLampBrightness() <= 1) && numHold == 1)
    || ((effects.getSpeed() == 255 || effects.getSpeed() <= 1) && numHold == 2)
    || ((effects.getScale() == 255 || effects.getScale() <= 1) && numHold == 3))){
      if(!setDirectionTimeout){
#ifdef LAMP_DEBUG
        LOG.printf_P(PSTR("Граничное значение! numHold: %d brightness: %d speed: %d scale: %d\n"), numHold,getLampBrightness(), effects.getSpeed(), effects.getScale());
#endif
        tmChangeDirectionTimer.reset(); // пауза на смену направления
        setDirectionTimeout = true;
      }
      else {
        changeDirection(numHold);
      }
    }

    // Для второго входа, сбрасываем флаг
    isFirstHoldingPress = false;
    
    #ifdef LAMP_DEBUG
    debugPrint(); // отладка
    #endif
    if (numHold != 0) {
      tmNumHoldTimer.reset();
      tmDemoTimer.reset(); // сбрасываем таймер переключения, если регулируем яркость/скорость/масштаб
    }
    
    switch (numHold) {
      case 1:
         setLampBrightness(constrain(getLampBrightness() + (getLampBrightness() / 25 + 1) * (brightDirection * 2 - 1), 1 , 255));
         FastLED.setBrightness(getNormalizedLampBrightness());
         break;

      case 2:
        effects.setSpeed(constrain(effects.getSpeed() + (effects.getSpeed() / 25 + 1) * (speedDirection * 2 - 1), 1 , 255));
        break;

      case 3:
        effects.setScale(constrain(effects.getScale() + (effects.getScale() / 25 + 1) * (scaleDirection * 2 - 1), 1 , 255));
        break;
    }
    return;
  }

  if (ONflag && !touch.isHold() && startButtonHolding)      // кнопка отпущена после удерживания
  {
    startButtonHolding = false;
    setDirectionTimeout = false;

    changeDirection(numHold);

#ifdef LAMP_DEBUG
    switch (numHold) {
      case 1:
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), getLampBrightness());
        break;
      case 2:
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), effects.getSpeed());
        break;
      case 3:
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), effects.getScale());
        break;
    }
#endif
    if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI

    return;
  }

// ---------------------- обработка одиночных нажатий без удержания ----------------

if(touch.isHold() || !touch.isHolded())
{
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;

    // однократное нажатие
    if (clickCount == 1U)
    {
  #ifdef LAMP_DEBUG
        LOG.printf_P(PSTR("Одиночное нажатие, lamp mode: %d, storedEffect: %d\n"), mode, storedEffect);
  #endif
      // #ifdef ONETOUCH_CLOCK     
      // if (!osd_printCurrentTime(CRGB::White)){}          // попытка напечатать время. Если не получается или текст уже на экране, то переходим ко включению
      // #endif  

      // #ifdef ONETOUCH_CLOCK
      // printTime(thisTime, true, ONflag, false, false); // оригинальный вариант вывода времени
      // #endif
      
      if(!ONflag){
        numHold = 0;
        mode = MODE_NORMAL;
        if(storedEffect!=EFF_NONE)
          effects.moveBy(storedEffect);
      } else {
        storedEffect = ((effects.getEn() == EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
      }
      
  #ifdef LAMP_DEBUG
      if(ONflag)
        LOG.printf_P(PSTR("Лампа выключена, lamp mode: %d, storedEffect: %d\n"), mode, storedEffect);
      else
        LOG.printf_P(PSTR("Лампа включена, lamp mode: %d, storedEffect: %d\n"), mode, storedEffect);
  #endif
      
      if (dawnFlag)
      {
        manualOff = true;
        dawnFlag = false;
        FastLED.setBrightness(getNormalizedLampBrightness());
        changePower();
      }
      else
      {
        ONflag = !ONflag;
        changePower();
      }
      loadingFlag = true;
    }

    // двухкратное нажатие
    if (ONflag && clickCount == 2U)
    {
  #ifdef LAMP_DEBUG
        LOG.printf_P(PSTR("Даблклик, lamp mode: %d, storedEffect: %d\n"), mode, storedEffect);
  #endif
      effects.moveNext();
      FastLED.setBrightness(getNormalizedLampBrightness());
      loadingFlag = true;
    }

    // трёхкратное нажатие
    if (ONflag && clickCount == 3U)
    {
      effects.movePrev();
      FastLED.setBrightness(getNormalizedLampBrightness());
      loadingFlag = true;
    }

    // четырёхкратное нажатие
    if (clickCount == 4U)
    {
      // #ifdef OTA
      // if (otaManager.RequestOtaUpdate())
      // {
      //   ONflag = true;
      //   lamp mode = EFF_MATRIX;                             // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
      //   FastLED.clear();
      //   delay(1);
      //   changePower();
      // }
      // #endif
    }


    // пятикратное нажатие
    if (clickCount == 5U)                                     // вывод IP на лампу
    {
        // loadingFlag = true;
        
        // #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время вывода текста
        // digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
        // #endif

        // while(!fillString(WiFi.localIP().toString().c_str(), CRGB::White)) { delay(1); ESP.wdtFeed(); }

        // #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)      // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы или будильника
        // digitalWrite(MOSFET_PIN, ONflag || (dawnFlag && !manualOff) ? MOSFET_LEVEL : !MOSFET_LEVEL);
        // #endif
    }


    // шестикратное нажатие
    if (clickCount == 6U)                                     // вывод текущего времени бегущей строкой
    {
      // printTime(thisTime, true, ONflag, false, false);
    }


    // семикратное нажатие
    if (ONflag && clickCount == 7U)                           // смена рабочего режима лампы: с WiFi точки доступа на WiFi клиент или наоборот
    {
      // espMode = (espMode == 0U) ? 1U : 0U;
      // EepromManager::SaveEspMode(&espMode);

      // #ifdef LAMP_DEBUG
      // LOG.printf_P(PSTR("Рабочий режим лампы изменён и сохранён в энергонезависимую память\nНовый рабочий режим: ESP_MODE = %d, %s\nРестарт...\n"),
      //   espMode, espMode == 0U ? F("WiFi точка доступа") : F("WiFi клиент (подключение к роутеру)"));
      // delay(1000);
      // #endif

      // showWarning(CRGB::Red, 3000U, 500U);                    // мигание красным цветом 3 секунды - смена рабочего режима лампы, перезагрузка
      // ESP.restart();
    }

    if(clickCount>0){
      if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
    }
  }
}
#endif

#ifdef USE_NTP
void LAMP::timeTick() // обработчик будильника "рассвет"
{

}
#endif

void LAMP::effectsTick()
{
  EFFECT *currentEffect = effects.getCurrent();

  if (!dawnFlag) // флаг устанавливается будильником рассвет
  {
    if (ONflag)
    {
      if(tmDemoTimer.isReady() && (mode == MODE_DEMO)){
        startFader(false);
      }

      faderTick(); // фейдер
      
      if(millis() - effTimer >= EFFECTS_RUN_TIMER && !isFaderOn){ // effects.getSpeed() - теперь эта обработка будет внутри эффектов
        if(currentEffect->func!=nullptr){
            currentEffect->func(getUnsafeLedsArray(), currentEffect->param); // отрисовать текущий эффект
        }
        effTimer = millis();
      }

#ifdef USELEDBUF
      memcpy(ledsbuff, leds, sizeof(CRGB)* NUM_LEDS);                             // сохранение сформированной картинки эффекта в буфер (для медленных или зависящих от предыдущей)
#endif
      
#ifdef NEWYEAR_MESSAGE
      NewYearMessagePrint(); // отрабатывает только во включенном состоянии
#endif
    }

    //onOffTimePrint();
    //osd_Tick(); // вывод сообщений по методу Palpalych https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proshivki-ot-gunner47.2418/page-26#post-26788

#ifdef VERTGAUGE
      if(ONflag)
        GaugeShow();
#endif
    
    if(ONflag){
      FastLED.show();
#ifdef USELEDBUF
      memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);                             // восстановление кадра с прорисованным эффектом из буфера (без текста и индикаторов) 
#endif
    }
  }

  ConfigSaveCheck(); // для взведенного таймера автосохранения настроек
}

#ifdef ESP_USE_BUTTON
    void LAMP::changeDirection(byte numHold){
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

  void LAMP::debugPrint(){
      LOG.printf_P(PSTR("lampMode: %d numHold: %d currentMode: %d brightness: %d speed: %d scale: %d\n"), mode, numHold, effects.getEn(), getLampBrightness(), effects.getSpeed(), effects.getScale());
  }
#endif

#ifdef VERTGAUGE
    void LAMP::GaugeShow() {
      byte ind;
      if(!startButtonHolding) return;
      
      switch (numHold) {    // индикатор уровня яркости/скорости/масштаба
#if (VERTGAUGE==1)
        case 1:
          ind = (byte)((getLampBrightness()+1)*HEIGHT/255.0+1);
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
          ind = (byte)((effects.getSpeed()+1)*HEIGHT/255.0+1);
          for (byte x = 0; x <= xCol*(xStep-1) ; x+=xStep) {
            for (byte y = 0; y < HEIGHT ; y++) {
              if (ind > y)
                drawPixelXY(x, y, CHSV(100, 255, 255));
              else
                drawPixelXY(x, y,  0);
            }
          }
          break;
        case 3:
          ind = (byte)((effects.getScale()+1)*HEIGHT/255.0+1);
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
          ind = (byte)((getLampBrightness()+1)*HEIGHT/255.0+1);
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
          ind = (byte)((effects.getSpeed()+1)*HEIGHT/255.0+1);
          for (byte y = 0; y <= yCol*(yStep-1) ; y+=yStep) {
            for (byte x = 0; x < WIDTH ; x++) {
              if (ind > x)
                drawPixelXY((x+y)%WIDTH, y, CHSV(100, 255, 255));
              else
                drawPixelXY((x+y)%WIDTH, y,  0);
            }
          }
          break;
        case 3:
          ind = (byte)((effects.getScale()+1)*HEIGHT/255.0+1);
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

bool LAMP::faderTick(){
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

LAMP::LAMP() : tmFaderTimeout(0), tmFaderStepTime(FADERSTEPTIME), tmDemoTimer(DEMO_TIMEOUT*1000), tmConfigSaveTime(0), tmNumHoldTimer(NUMHOLD_TIME)
#ifdef ESP_USE_BUTTON    
    , touch(BTN_PIN, PULL_MODE, NORM_OPEN)
    , tmChangeDirectionTimer(NUMHOLD_TIME)     // таймаут смены направления увеличение-уменьшение при удержании кнопки
#endif
    {
      init(); // инициализация и настройка лампы
    }

    void LAMP::startFader(bool isManual=false)
    {
        tmFaderTimeout.setInterval(FADERTIMEOUT); // взводим таймер фейдера
        tmFaderTimeout.reset();
        isFaderOn = true;
        manualFader = isManual;
    }

void LAMP::changePower() {changePower(ONflag);}

void LAMP::changePower(bool flag) // плавное включение/выключение
    {
      if (flag){
        for (uint8_t i = 0U; i < getNormalizedLampBrightness(); i = constrain(i + 8, 0, getNormalizedLampBrightness()))
        {
          FastLED.setBrightness(i);
          delay(1);
          FastLED.show();
        }

        FastLED.setBrightness(getNormalizedLampBrightness());
        delay(2);
        FastLED.show();
      }
      else
      {
        for (uint8_t i = getNormalizedLampBrightness(); i > 0; i = constrain(i - 8, 0, getNormalizedLampBrightness()))
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


    uint32_t LAMP::getPixelNumber(uint16_t x, uint16_t y) // получить номер пикселя в ленте по координатам
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

    uint32_t LAMP::getPixColor(uint32_t thisSegm) // функция получения цвета пикселя по его номеру
    {
      uint32_t thisPixel = thisSegm * SEGMENTS;
      if (thisPixel > NUM_LEDS - 1) return 0;
      return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
    }

    void LAMP::fillAll(CRGB color) // залить все
    {
      for (int32_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = color;
      }
    }

    void LAMP::drawPixelXY(int16_t x, int16_t y, CRGB color) // функция отрисовки точки по координатам X Y
    {
      if (x < 0 || x > (int16_t)(WIDTH - 1) || y < 0 || y > (int16_t)(HEIGHT - 1)) return;
      uint32_t thisPixel = getPixelNumber((uint16_t)x, (uint16_t)y) * SEGMENTS;
      for (uint16_t i = 0; i < SEGMENTS; i++)
      {
        leds[thisPixel + i] = color;
      }
    }