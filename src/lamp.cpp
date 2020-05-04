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

void LAMP::lamp_init()
{
  FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS)  /*.setCorrection(TypicalLEDStrip)*/;
  brightness(0, false);                          // начинаем с полностью потушеной матрицы 0-й яркости
  if (CURRENT_LIMIT > 0){
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); // установка максимального тока БП
  }
  FastLED.clear();                                            // очистка матрицы
  //FastLED.show(); // для ESP32 вызывает перезагрузку циклическую!!!
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
// EVERY_N_SECONDS(10){
//     FastLED.setMaxPowerInVoltsAndMilliamps(5, 100); // установка максимального тока БП
// }
  effectsTick(); // обработчик эффектов

#ifdef ESP_USE_BUTTON
  static unsigned long button_check;
  if (buttonEnabled && button_check + 30U < millis()) // раз в 30 мс проверяем кнопку и будильник
  {
    buttonTick(); // обработчик кнопки
    alarmWorker();
#ifdef MIC_EFFECTS
    if(isMicOn && ONflag)
      micHandler();
#endif
    button_check = millis();
  }
#endif

#if defined(LAMP_DEBUG) && DEBUG_TELNET_OUTPUT
  handleTelnetClient();
#endif

  // все что ниже, будет выполняться раз в 0.999 секундy
  static unsigned long wait_handlers;
  if (wait_handlers + 999U > millis())
      return;
  wait_handlers = millis();

  // отложенное включение/выключение
  if(isOffAfterText && !isStringPrinting) {
    isOffAfterText = false;
    setOnOff(false);
  }

  if(isEffectsDisabledUntilText && !isStringPrinting) {
    isEffectsDisabledUntilText = false;
  }

  newYearMessageHandle();
  periodicTimeHandle();
  ConfigSaveCheck(); // для взведенного таймера автосохранения настроек

#ifdef OTA
  otaManager.HandleOtaUpdate();                       // ожидание и обработка команды на обновление прошивки по воздуху
#endif

  timeProcessor.handleTime();                         // Обновление времени

  if(!second(timeProcessor.getUnixTime()) && isEventsHandled) // только на 0 секунду, т.е. 1 раз в минуту и если обработка разрешена
    events.events_handle(timeProcessor.getUnixTime(),timeProcessor.getOffset());
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
      startDemoMode();
      return;
    }
    
    if (touch.isHolded()) {
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("Удержание кнопки из выключенного состояния\n"));
#endif
      numHold = 1;
      int clicks = touch.getHoldClicks();
      if(!clicks) {
        // Включаем белую лампу в полную яркость
        brightDirection = 1;
        mode = MODE_WHITELAMP;
        effects.moveBy(EFF_WHITE_COLOR);
        effects.setBrightness(255);
      }
      if(clicks==1){
        // Включаем белую лампу в минимальную яркость
        brightDirection = 0;
        mode = MODE_WHITELAMP;
        effects.moveBy(EFF_WHITE_COLOR);
        effects.setBrightness(1);
      }
      //FastLED.setBrightness(getNormalizedLampBrightness());   // реальную яркость меняем в методе changePower()
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("lamp mode: %d, storedEffect: %d, LampBrightness=%d\n"), mode, storedEffect, getNormalizedLampBrightness());
#endif
      ONflag = true;

      startButtonHolding = true;
      setDirectionTimeout = false;

      tmNumHoldTimer.reset();
      tmChangeDirectionTimer.reset();

      changePower();
      if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
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
         setBrightness(getNormalizedLampBrightness());
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
        //FastLED.setBrightness(getNormalizedLampBrightness());   // LED яркость меняется в changePower()
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
      setBrightness(getNormalizedLampBrightness());
      loadingFlag = true;
    }

    // трёхкратное нажатие
    if (ONflag && clickCount == 3U)
    {
      effects.movePrev();
      setBrightness(getNormalizedLampBrightness());
      loadingFlag = true;
    }

    // четырёхкратное нажатие
    if (clickCount == 4U)
    {
      #ifdef OTA
      if (otaManager.RequestOtaUpdate())
      {
        startOTAUpdate();
      }
      #endif
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

        sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::White);
    }


    // шестикратное нажатие
    if (clickCount == 6U)                                     // вывод текущего времени бегущей строкой
    {
      myLamp.sendStringToLamp(myLamp.timeProcessor.getFormattedShortTime().c_str(), CRGB::Green); // вывести время на лампу
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

void LAMP::alarmWorker() // обработчик будильника "рассвет"
{
    // static CHSV GSHMEM.dawnColorMinus[6];                                            // цвет "рассвета"
    // static uint8_t GSHMEM.dawnCounter = 0;                                           // счётчик первых шагов будильника
    // static time_t GSHMEM.startmillis;

    if (mode != LAMPMODE::MODE_ALARMCLOCK){
      dawnFlag = false;
      manualOff = false;
      return;
    }

    // проверка рассвета, первый вход в функцию
    if (mode == LAMPMODE::MODE_ALARMCLOCK && !dawnFlag){
      GSHMEM.startmillis = millis();
      manualOff = false;
      memset(GSHMEM.dawnColorMinus,0,sizeof(GSHMEM.dawnColorMinus));
      GSHMEM.dawnCounter = 0;
      FastLED.clear();
      brightness(255, false);   // зачем на яркость больше чем hardcoded BRIGHTNESS? :)
      // величина рассвета 0-255
      int16_t dawnPosition = map((millis()-GSHMEM.startmillis)/1000,0,300,0,255); // 0...300 секунд приведенные к 0...255
      dawnPosition = constrain(dawnPosition, 0, 255);
      GSHMEM.dawnColorMinus[0] = CHSV(map(dawnPosition, 0, 255, 10, 35),
                        map(dawnPosition, 0, 255, 255, 170),
                        map(dawnPosition, 0, 255, 10, DAWN_BRIGHT));
    }

    if(LAMPMODE::MODE_ALARMCLOCK && ((millis()-GSHMEM.startmillis)/1000>(5+DAWN_TIMEOUT)*60+30 || manualOff)){ // рассвет закончился
      mode = storedMode;
      // не время будильника (ещё не начался или закончился по времени)
      if (dawnFlag)
      {
        dawnFlag = false;
        manualOff = false;
        FastLED.clear();
        changePower();                                                  // выключение матрицы или установка яркости текущего эффекта в засисимости от того, была ли включена лампа до срабатывания будильника
      }
      // #if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
      // digitalWrite(ALARM_PIN, !ALARM_LEVEL);
      // #endif

      // #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
      // digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
      // #endif
    }

    //blur2d(25);

    // проверка рассвета
    if (mode == LAMPMODE::MODE_ALARMCLOCK)
    {
      if (!manualOff)                                                   // будильник не был выключен вручную (из приложения или кнопкой)
      {
        EVERY_N_SECONDS(10){
          // величина рассвета 0-255
          int16_t dawnPosition = map((millis()-GSHMEM.startmillis)/1000,0,300,0,255); // 0...300 секунд приведенные к 0...255
          dawnPosition = constrain(dawnPosition, 0, 255);
          GSHMEM.dawnColorMinus[0] = CHSV(map(dawnPosition, 0, 255, 10, 35),
                          map(dawnPosition, 0, 255, 255, 170),
                          map(dawnPosition, 0, 255, 10, DAWN_BRIGHT));
          GSHMEM.dawnCounter++; //=GSHMEM.dawnCounter%(sizeof(GSHMEM.dawnColorMinus)/sizeof(CHSV))+1;

          for(uint8_t i=sizeof(GSHMEM.dawnColorMinus)/sizeof(CHSV)-1; i>0U; i--){
              GSHMEM.dawnColorMinus[i]=((GSHMEM.dawnCounter > i)?GSHMEM.dawnColorMinus[i-1]:GSHMEM.dawnColorMinus[i]);
          }
        }

#ifdef PRINT_ALARM_TIME        
        EVERY_N_SECONDS(1){
          if(!second(timeProcessor.getUnixTime())){
            CRGB letterColor;
            hsv2rgb_rainbow(GSHMEM.dawnColorMinus[0], letterColor); // конвертация цвета времени, с учетом текущей точки рассвета
            myLamp.sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), letterColor, true);
          }
        }
#endif

        // fill_solid(leds, NUM_LEDS, dawnColor);
        for (uint16_t i = 0U; i < NUM_LEDS; i++)
            leds[i] = GSHMEM.dawnColorMinus[i%(sizeof(GSHMEM.dawnColorMinus)/sizeof(CHSV))];
        dawnFlag = true;
      }

      // #if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
      // if (thisTime == alarms[thisDay].Time)                             // установка, только в минуту, на которую заведён будильник
      // {
      //   digitalWrite(ALARM_PIN, manualOff ? !ALARM_LEVEL : ALARM_LEVEL);// установка сигнала в зависимости от того, был ли отключен будильник вручную
      // }
      // #endif

      // #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, матрица должна быть включена на время работы будильника
      // digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
      // #endif
    }
}

void LAMP::effectsTick()
{
  //bool showMustGoON = false;
  bool storeEffect = false;
  
  if (!dawnFlag) // флаг устанавливается будильником рассвет
  {
    if (ONflag || _fadeTicker.active())   // временный костыль, продолжаем обрабаьывать эффект пока работает фейдер
    {
      /*
        if(isFaderOn){
          faderTick(); // фейдер
          showMustGoON = true;    // для чего это?
        }
      */
        if(millis() - effTimer >= EFFECTS_RUN_TIMER){ // effects.getSpeed() - теперь эта обработка будет внутри эффектов
          if(tmDemoTimer.isReady() && (mode == MODE_DEMO)){
            fadeeffect();
          }
          if(!isEffectsDisabledUntilText){
            if(effects.getCurrent()->func!=nullptr){
                effects.getCurrent()->func(getUnsafeLedsArray(), effects.getCurrent()->param); // отрисовать текущий эффект
                //showMustGoON = true;
                storeEffect = true;
            }
#ifdef USELEDBUF
            memcpy(ledsbuff, leds, sizeof(CRGB)* NUM_LEDS);                             // сохранение сформированной картинки эффекта в буфер (для медленных или зависящих от предыдущей)
#endif
          }
          doPrintStringToLamp(); // обработчик печати строки
#ifdef VERTGAUGE
          GaugeShow();
#endif
          effTimer = millis();
        }
    }

    if(ONflag || _fadeTicker.active()){
      FastLED.show();
      if(storeEffect){
#ifdef USELEDBUF
        memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);                             // восстановление кадра с прорисованным эффектом из буфера (без текста и индикаторов) 
#endif
      }
    }
    //showMustGoON = false;
    storeEffect = false;
  } else {
    if(!(millis()%11)){
      doPrintStringToLamp(); // обработчик печати строки
      FastLED.show();
    }
  }
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

/*
 * менялка эффектов через фейдер
 * первый раз вызывается методами на смену эффекта,
 * второй раз вызывает саму себя через планировщик
 * TODO: переделать второй вызов на коллбэк из самого федера
 */
void LAMP::fadeeffect(bool stage){

  // первая стадия - запускаем фейдер яркости в ноль, перезапускаем себя к моменту окончания затухания
  // тут есть некритичная бага - время затухания может быть короче дефолтового значения
  if (stage) {
    fadelight(FADE_MINCHANGEBRT);
    _fadeeffectTicker.once_ms_scheduled(FADE_TIME, std::bind(&LAMP::fadeeffect, this, false));
  } else {  // вторая стадия - меняем эффект, запускаем плавное разгорание лампы

    loadingFlag = true; // некоторые эффекты требуют начальной иницализации, поэтому делаем так...
    if(mode==MODE_DEMO){
      if(RANDOM_DEMO)
        effects.moveBy(random(0, effects.getModeAmount()));
      else
        effects.moveNext();
#ifdef LAMP_DEBUG
        LOG.printf_P(PSTR("%s Demo mode: %d, storedEffect: %d\n"),(RANDOM_DEMO?PSTR("Random"):PSTR("Seq")) , effects.getEn(), storedEffect);
#endif
        if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
          setLoading();
    }

    EFFECT *currentEffect = effects.getCurrent();
    if(currentEffect->func!=nullptr)
      currentEffect->func(getUnsafeLedsArray(), currentEffect->param); // отрисовать текущий эффект

    fadelight(getNormalizedLampBrightness()); 
  }
}

LAMP::LAMP() : docArrMessages(512), tmDemoTimer(DEMO_TIMEOUT*1000)
    , tmConfigSaveTime(0), tmNumHoldTimer(NUMHOLD_TIME), tmStringStepTime(DEFAULT_TEXT_SPEED), tmNewYearMessage(0), _fadeTicker(), _fadeeffectTicker()
#ifdef ESP_USE_BUTTON    
    , touch(BTN_PIN, PULL_MODE, NORM_OPEN)
    , tmChangeDirectionTimer(NUMHOLD_TIME)     // таймаут смены направления увеличение-уменьшение при удержании кнопки
#endif
#ifdef OTA
    , otaManager((void (*)(CRGB, uint32_t, uint16_t))(&showWarning))
#endif
    {
      MIRR_V = false; // отзрекаливание по V
      MIRR_H = false; // отзрекаливание по H
      dawnFlag = false; // флаг устанавливается будильником "рассвет"
      ONflag = false; // флаг включения/выключения
      manualOff = false;
      loadingFlag = true; // флаг для начальной инициализации эффекта
      //isFaderOn = false; // признак того, что выполняется фейдер текущего эффекта
      manualFader = false; // ручной или автоматический фейдер
      isGlobalBrightness = false; // признак использования глобальной яркости для всех режимов
      isFirstHoldingPress = false; // флаг: только начали удерживать?
      startButtonHolding = false; // кнопка удерживается
      buttonEnabled = true; // кнопка обрабатывается если true
      brightDirection = false;
      speedDirection = false;
      scaleDirection = false;
      setDirectionTimeout = false; // флаг: начало отсчета таймаута на смену направления регулировки
      isStringPrinting = false; // печатается ли прямо сейчас строка?
      isEffectsDisabledUntilText = false;
      isOffAfterText = false;
      isEventsHandled = true;
      _brt, _steps = 0;
      _brtincrement = 0;
#ifdef MIC_EFFECTS
      isCalibrationRequest = false; // находимся ли в режиме калибровки микрофона
      isMicOn = true; // глобальное испльзование микрофона
      micAnalyseDivider = 1; // анализ каждый раз
#endif


      lamp_init(); // инициализация и настройка лампы
    }

//    фейдер теперь запускается сам при вызове функции смены яркости
/*
    void LAMP::startFader(bool isManual=false)
    {
        tmFaderTimeout.setInterval(FADERTIMEOUT); // взводим таймер фейдера
        tmFaderTimeout.reset();
        isFaderOn = true;
        manualFader = isManual;
    }
*/

void LAMP::changePower() {changePower(ONflag);}

void LAMP::changePower(bool flag) // плавное включение/выключение
    {
      if (flag){
        // включение
        fadelight(getNormalizedLampBrightness());
      }
      else
      {
        // Выключение
        fadelight(0);
      }

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
      digitalWrite(MOSFET_PIN, (ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
#endif

      if (CURRENT_LIMIT > 0){
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); // установка максимального тока БП
      }
    }


    uint32_t LAMP::getPixelNumber(uint16_t x, uint16_t y) // получить номер пикселя в ленте по координатам
    {
      if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
      {
        return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + THIS_X)%NUM_LEDS;
      }
      else                                                      // если нечётная строка
      {
        return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + _WIDTH - THIS_X - 1)%NUM_LEDS;
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

// ------------- мигающий цвет (не эффект! используется для отображения краткосрочного предупреждения; блокирующий код!) -------------
extern LAMP myLamp; // Объект лампы
void LAMP::showWarning(
  CRGB::HTMLColorCode color,                                               /* цвет вспышки                                                 */
  uint32_t duration,                                        /* продолжительность отображения предупреждения (общее время)   */
  uint16_t blinkHalfPeriod)                                 /* продолжительность одной вспышки в миллисекундах (полупериод) */
{
  uint32_t blinkTimer = millis();
  enum BlinkState { OFF = 0, ON = 1 } blinkState = BlinkState::OFF;
  myLamp.fadelight(myLamp.getLampBrightness());    // установка яркости для предупреждения
  FastLED.clear();
  delay(2);
  FastLED.show();

  for (uint16_t i = 0U; i < NUM_LEDS; i++)                  // установка цвета всех диодов в WARNING_COLOR
  {
    myLamp.setLeds(i, color);
  }

  uint32_t startTime = millis();
  while (millis() - startTime <= (duration + 5))            // блокировка дальнейшего выполнения циклом на время отображения предупреждения
  {
    if (millis() - blinkTimer >= blinkHalfPeriod)           // переключение вспышка/темнота
    {
      blinkTimer = millis();
      blinkState = (BlinkState)!blinkState;
      myLamp.brightness(blinkState == BlinkState::OFF ? 0 : myLamp.getLampBrightness());
      delay(1);
      FastLED.show();
    }
    delay(50);
  }

  FastLED.clear();
  myLamp.fadelight(myLamp.isLampOn() ? myLamp.getLampBrightness() : 0);  // установка яркости, которая была выставлена до вызова предупреждения
  delay(1);
  FastLED.show();
  myLamp.setLoading();                                       // принудительное отображение текущего эффекта (того, что был активен перед предупреждением)
}

void LAMP::startAlarm()
{
  storedMode = ((mode == LAMPMODE::MODE_ALARMCLOCK ) ? storedMode: mode);
  mode = LAMPMODE::MODE_ALARMCLOCK;
}

void LAMP::startDemoMode()
{
  storedEffect = ((effects.getEn() == EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  mode = LAMPMODE::MODE_DEMO;
  randomSeed(millis());
  effects.moveBy(random(0, MODE_AMOUNT));
  //FastLED.setBrightness(getNormalizedLampBrightness());   // уходим в changePower()
  ONflag = true;
  loadingFlag = true;
  tmDemoTimer.reset(); // момент включения для таймаута в DEMOTIME
  changePower();
  myLamp.sendStringToLamp(String(PSTR("- Demo ON -")).c_str(), CRGB::Green);
#ifdef LAMP_DEBUG
  LOG.printf_P(PSTR("Demo mode: %d, storedEffect: %d\n"), effects.getEn(), storedEffect);
#endif
  if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
}

void LAMP::startNormalMode()
{
  mode = LAMPMODE::MODE_NORMAL;
  if(storedEffect!=EFF_NONE)
    effects.moveBy(storedEffect);
  fadelight(getNormalizedLampBrightness());
  loadingFlag = true;
  if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
}
#ifdef OTA
void LAMP::startOTAUpdate()
{        
  mode = MODE_OTA;
  effects.moveBy(EFF_MATRIX); // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
  FastLED.clear();
  ONflag = true;
  changePower(true);
  if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
  myLamp.sendStringToLamp(String(PSTR("- OTA UPDATE ON -")).c_str(), CRGB::Green);
}
#endif
bool LAMP::fillStringManual(const char* text,  const CRGB &letterColor, bool stopText, bool isInverse, int8_t letSpace, int8_t txtOffset, int8_t letWidth, int8_t letHeight)
{
  static int32_t offset = (MIRR_V ? 0 : WIDTH);
  
  if (!text || !strlen(text))
  {
    return true;
  }

  uint16_t i = 0, j = 0;
  while (text[i] != '\0')
  {
    if ((uint8_t)text[i] > 191)                           // работаем с русскими буквами
    {
      i++;
    }
    else
    {
      if(!MIRR_V)
        drawLetter(text[i], offset + (int16_t)j * (letWidth + letSpace), letterColor, letSpace, txtOffset, isInverse, letWidth, letHeight);
      else
        drawLetter(text[i], offset - (int16_t)j * (letWidth + letSpace), letterColor, letSpace, txtOffset, isInverse, letWidth, letHeight);
      i++;
      j++;
    }
  }

  if(!stopText)
    (MIRR_V ? offset++ : offset--);
  if ((!MIRR_V && offset < (int32_t)(-j * (letWidth + letSpace))) || (MIRR_V && offset > (int32_t)(j * (letWidth + letSpace))+(signed)WIDTH))       // строка убежала
  {
    offset = (MIRR_V ? 0 : WIDTH);
    return true;
  }

  return false;
}

void LAMP::drawLetter(uint16_t letter, int16_t offset,  const CRGB &letterColor, int8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight)
{
  uint16_t start_pos = 0, finish_pos = letWidth + letSpace;

  if (offset < (int16_t)-letWidth || offset > (int16_t)WIDTH)
  {
    return;
  }
  if (offset < 0)
  {
    start_pos = (uint16_t)-offset;
  }
  if (offset > (int16_t)(WIDTH - letWidth))
  {
    finish_pos = (uint16_t)(WIDTH - offset);
  }

  for (uint16_t i = start_pos; i < finish_pos; i++)
  {
    uint8_t thisByte;

    if((finish_pos - i <= letSpace) || ((letWidth - 1 - i)<0))
      thisByte = 0x00;
    else
    {
      thisByte = getFont(letter, i);
    }

    for (uint16_t j = 0; j < letHeight; j++)
    {
      bool thisBit = thisByte & (1 << (letHeight - 1 - j));

      // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
      if (thisBit)
      {
        if(!isInverse)
          drawPixelXY(offset + i, txtOffset + j, letterColor);
        else
          setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), FADETOBLACKVALUE);
          //drawPixelXY(offset + i, txtOffset + j, (isInverse ? CRGB::Black : letterColor));
      }
      else
      {
        if(isInverse)
          drawPixelXY(offset + i, txtOffset + j, letterColor);
        else
          setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), FADETOBLACKVALUE);    
          //drawPixelXY(offset + i, txtOffset + j, (isInverse ? letterColor : CRGB::Black));
      }
    }
  }
}

uint8_t LAMP::getFont(uint8_t asciiCode, uint8_t row)       // интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
{
  asciiCode = asciiCode - '0' + 16;                         // перевод код символа из таблицы ASCII в номер согласно нумерации массива

  if (asciiCode <= 90)                                      // печатаемые символы и английские буквы
  {
    return pgm_read_byte(&fontHEX[asciiCode][row]);
  }
  else if (asciiCode >= 112 && asciiCode <= 159)
  {
    return pgm_read_byte(&fontHEX[asciiCode - 17][row]);
  }
  else if (asciiCode >= 96 && asciiCode <= 111)
  {
    return pgm_read_byte(&fontHEX[asciiCode + 47][row]);
  }

  return 0;
}

void LAMP::sendStringToLamp(const char* text, const CRGB &letterColor, bool forcePrint)
{
  if((!ONflag && !forcePrint) || (dawnFlag && !forcePrint)) return; // если выключена, или если будильник, но не задан принудительный вывод - то на выход

  if(text==nullptr){ // текст пустой
    if(!isStringPrinting){ // ничего сейчас не печатается
      if(docArrMessages.isNull()){ // массив пустой
        return; // на выход
      }
      else { // есть что печатать
        JsonArray arr = docArrMessages.as<JsonArray>(); // используем имеющийся
        JsonObject var=arr[0]; // извлекаем очередной
        doPrintStringToLamp(var[F("s")], (var[F("c")].as<unsigned long>())); // отправляем
        arr.remove(0); // удаляем отправленный
#ifdef LAMP_DEBUG
        //LOG.println(docArrMessages.as<String>());
#endif
      }
    } else {
        // текст на входе пустой, идет печать
        return; // на выход
    }
  } else { // текст не пустой
    if(!isStringPrinting){ // ничего сейчас не печатается
      doPrintStringToLamp(text, letterColor); // отправляем
    } else { // идет печать, помещаем в очередь
      JsonArray arr; // добавляем в очередь
      
      if(!docArrMessages.isNull())
        arr = docArrMessages.as<JsonArray>(); // используем имеющийся
      else
        arr = docArrMessages.to<JsonArray>(); // создаем новый
      
      JsonObject var = arr.createNestedObject();
      var[F("s")]=text;
      var[F("c")]=(unsigned long)letterColor.r*65536+(unsigned long)letterColor.g*256+(unsigned long)letterColor.b;
#ifdef LAMP_DEBUG
      LOG.println(docArrMessages.as<String>());
#endif
      String tmp; // Тут шаманство, чтобы не ломало JSON
      serializeJson(docArrMessages, tmp);
      deserializeJson(docArrMessages, tmp);
    }
  }
}

void LAMP::doPrintStringToLamp(const char* text,  const CRGB &letterColor)
{
  static String toPrint;
  static CRGB _letterColor;

  isStringPrinting = true;

  if(text!=nullptr && text[0]!='\0'){
    toPrint.concat(text);
    toPrint.replace(F("%TM"), timeProcessor.getFormattedShortTime());
    toPrint.replace(F("%IP"), WiFi.localIP().toString());
    _letterColor = letterColor;
  }

  if(toPrint.length()==0) {
    isStringPrinting = false;
    return; // нечего печатать
  } else {
    isStringPrinting = true;
  }

  if(tmStringStepTime.isReadyManual()){
    if(!fillStringManual(toPrint.c_str(), _letterColor, false, dawnFlag, 1, txtOffset)){ // смещаем
      tmStringStepTime.reset();
    }
    else {
      isStringPrinting = false;
      toPrint.clear(); // все напечатали
      sendStringToLamp(); // получаем новую порцию
    }
  } else {
    fillStringManual(toPrint.c_str(), _letterColor, true, dawnFlag, 1, txtOffset);
  }
}


static const char NY_MDG_STRING1[] PROGMEM = "До нового года осталось %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "C новым %d годом!";

void LAMP::newYearMessageHandle()
{
  if(!tmNewYearMessage.isReady() || timeProcessor.isDirtyTime())
    return;

  {
    char strMessage[256]; // буффер
    time_t calc = NEWYEAR_UNIXDATETIME - timeProcessor.getUTCUnixTime(); // unix_diff_time

    if(calc<0) {
      sprintf_P(strMessage, NY_MDG_STRING2, timeProcessor.getYear());
    } else if(calc<300){
      sprintf_P(strMessage, NY_MDG_STRING1, (int)calc, PSTR("секунд"));
    } else if(calc/60<60){
      sprintf_P(strMessage, NY_MDG_STRING1, (int)(calc/60), PSTR("минут"));
    } else if(calc/(60*60)<60){
      sprintf_P(strMessage, NY_MDG_STRING1, (int)(calc/(60*60)), PSTR("часов"));
    } else {
      byte calcN=(int)(calc/(60*60*24))%10; // остаток от деления на 10
      String str;
      if(calcN>=2 && calcN<=4)
        str = F("дня");
      else if(calc!=11 && calcN==1)
        str = F("день");
      else
        str = F("дней");
      sprintf_P(strMessage, NY_MDG_STRING1, (int)(calc/(60*60*24)), str.c_str());
    }

#ifdef LAMP_DEBUG
    LOG.printf_P(PSTR("Prepared message: %s\n"), strMessage);
#endif
    sendStringToLamp(strMessage, LETTER_COLOR);
  }
}

void LAMP::periodicTimeHandle()
{
  static bool cancel = false;
  
  time_t tm = timeProcessor.getUnixTime();
  //LOG.println(tm);
  if(second(tm)) {cancel=false; return;}
  if(cancel) return;

  cancel = true; // только раз в минуту срабатываем, на первую секунду
  tm = hour(tm) * 60 + minute(tm);

  switch (enPeriodicTimePrint)
  {
    case PERIODICTIME::PT_EVERY_1:
      if(tm%60)
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Blue);
      break;
    case PERIODICTIME::PT_EVERY_5:
      if(!(tm%5) && tm%60)
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Blue);
      break;
    case PERIODICTIME::PT_EVERY_10:
      if(!(tm%10) && tm%60)
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Blue);
      break;
    case PERIODICTIME::PT_EVERY_15:
      if(!(tm%15) && tm%60)
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Blue);
      break;
    case PERIODICTIME::PT_EVERY_30:
      if(!(tm%30) && tm%60)
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Blue);
      break;
    case PERIODICTIME::PT_EVERY_60:
      if(!(tm%60))
        sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Red);
      break;
    
    default:
      break;
  }
  if(enPeriodicTimePrint!=PERIODICTIME::PT_EVERY_60 && enPeriodicTimePrint!=PERIODICTIME::PT_NOT_SHOW && !(tm%60))
    sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Red);
}

#ifdef MIC_EFFECTS
void LAMP::micHandler()
{
  static uint8_t counter=0;
  
  if(mw==nullptr && !isCalibrationRequest){ // обычный режим
    //if(millis()%1000) return; // отладка
    mw = new MICWORKER(mic_scale,mic_noise);
    samp_freq = mw->process(noise_reduce); // возвращаемое значение - частота семплирования
    last_min_peak = mw->getMinPeak();
    last_max_peak = mw->getMaxPeak();

    if(!counter) // раз на N измерений берем частоту, т.к. это требует обсчетов
      last_freq = mw->analyse(); // возвращаемое значение - частота главной гармоники
    if(micAnalyseDivider)
      counter = (counter+1)%(0x01<<(micAnalyseDivider-1)); // как часто выполнять анализ
    else
      counter = 1; // при micAnalyseDivider == 0 - отключено

    // EVERY_N_SECONDS(1){
    //   LOG.println(counter);
    // }

    //LOG.println(last_freq);
    //mw->debug();
    delete mw;
    mw = nullptr;
  } else {
    if(mw==nullptr){ // калибровка начало
      mw = new MICWORKER();
      mw->calibrate();
    } else { // калибровка продолжение
      mw->calibrate();
    }
    if(!mw->isCaliblation()){ // калибровка конец
      mic_noise = mw->getNoise();
      mic_scale = mw->getScale();
      isCalibrationRequest = false; // завершили
      delete mw;
      mw = nullptr;
    }
  }
}
#endif

void LAMP::fadelight(const uint8_t _targetbrightness, const uint32_t _duration) {
    _fadeTicker.detach();

    uint8_t _maxsteps = _duration / FADE_STEPTIME;
    _brt = getBrightness();
    uint8_t _brtdiff = abs(_targetbrightness - _brt);

    if (_brtdiff > FADE_MININCREMENT * _maxsteps) {
        _steps = _maxsteps;
    } else {
        _steps = _brtdiff/FADE_MININCREMENT;
    }

    if (_steps < 3) {
        brightness(_targetbrightness);
        return;
    }

    _brtincrement = (_targetbrightness - _brt) / _steps;

    //_SPTO(Serial.printf_P(F_fadeinfo, _brt, _targetbrightness, _steps, _brtincrement)); _SPLN("");
    _fadeTicker.attach_ms(FADE_STEPTIME, std::bind(&LAMP::fader, this, _targetbrightness));
}

/*
 * Change global brightness with or without fade effect
 * fade applied in non-blocking way
 * FastLED dim8 function applied internaly for natural brightness controll
 * @param uint8_t _brt - target brigtness level 0-255
 * @param bool fade - use fade effect on brightness change
 */
void LAMP::setBrightness(const uint8_t _brt, const bool fade, const bool natural){
    //_SP(F("Set brightness: ")); _SPLN(_brt);
    if (fade) {
        fadelight(_brt);
    } else {
        brightness(_brt, natural);
    }
}

/*
 * Get current brightness
 * FastLED brighten8 function applied internaly for natural brightness compensation
 * @param bool natural - return compensated or absolute brightness
 */
uint8_t LAMP::getBrightness(const bool natural){
    return (natural ? brighten8_raw(FastLED.getBrightness()) : FastLED.getBrightness());
}


/*
 * Set global brightness
 * @param bool natural 
 */
void LAMP::brightness(const uint8_t _brt, bool natural){
    uint8_t _cur = natural ? brighten8_raw(FastLED.getBrightness()) : FastLED.getBrightness();
    if ( _cur == _brt) return;

    FastLED.setBrightness(natural ? dim8_raw(_brt) : _brt);
    FastLED.show();
}

/*
 * Fade light callback
 * @param bool natural 
 */
void LAMP::fader(const uint8_t _tgtbrt){
  --_steps;
  if (! _steps) {   // on last step
      _fadeTicker.detach();
      _brt = _tgtbrt;
  } else {
      _brt += _brtincrement;
  }

  brightness(_brt);
}