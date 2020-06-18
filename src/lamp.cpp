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

extern LAMP myLamp; // Объект лампы

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
  #if (PULL_MODE == LOW_PULL)
    pinMode(BTN_PIN, INPUT);
  #else
    pinMode(BTN_PIN, INPUT_PULLUP);
  #endif
  touch.setTickMode(MANUAL);    // мы сами говорим когда опрашивать пин
  touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
  touch.setTimeout(BUTTON_TIMEOUT);
  touch.setDebounce(BUTTON_DEBOUNCE);   // т.к. работаем с прерываниями, может пригодиться для железной кнопки
  _buttonTicker.attach_scheduled(1, std::bind(&LAMP::buttonTick, this));   // "ленивый" опрос 1 раз в сек
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
  //effectsTick(); // уехало в тикер

  static unsigned long mic_check;

#ifdef MIC_EFFECTS
  if(isMicOn && ONflag && (!dawnFlag) && mic_check + MIC_POLLRATE < millis()){
    micHandler();
    mic_check = millis();
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

#ifdef LAMP_DEBUG
EVERY_N_SECONDS(15){
  // fps counter
  LOG(printf, "Eff:%d FPS: %u\n", effects.getEn(), fps);
}
  fps = 0;
#endif

  // будильник обрабатываем раз в секунду
  alarmWorker();

  if(isEffectsDisabledUntilText && !isStringPrinting) {
    setBrightness(0,false,false); // напечатали, можно гасить матрицу :)
    isEffectsDisabledUntilText = false;
  }

  // отложенное включение/выключение
  if(isOffAfterText && !isStringPrinting) {
    isOffAfterText = false;
    setOnOff(false);
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
  //LOG.printf("Key: %u\n", millis());

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
      LOG(printf_P, PSTR("Удержание кнопки из выключенного состояния\n"));
      numHold = 1;
      int clicks = touch.getHoldClicks();
      if(!clicks) {
        // Включаем белую лампу в полную яркость
        brightDirection = 1;
        mode = MODE_WHITELAMP;
        effects.moveBy(EFF_WHITE_COLOR);
        setLampBrightness(255); // здесь яркость ползунка в UI, т.е. ставим 255 в самое крайнее положение, а дальше уже будет браться приведенная к BRIGHTNESS яркость
        setBrightness(getNormalizedLampBrightness(), isFaderON, false);
      }
      if(clicks==1){
        // Включаем белую лампу в минимальную яркость
        brightDirection = 0;
        mode = MODE_WHITELAMP;
        effects.moveBy(EFF_WHITE_COLOR);
        setLampBrightness(1); // здесь яркость ползунка в UI, т.е. ставим 1 в самое крайнее положение, а дальше уже будет браться приведенная к BRIGHTNESS яркость
        setBrightness(getNormalizedLampBrightness(), false, false);   // оставляем для включения с кнопки c минимальной яркости, тут так задумано, в обход фейдера :)
      }
      LOG(printf_P, PSTR("lamp mode: %d, storedEffect: %d, LampBrightness=%d\n"), mode, storedEffect, getNormalizedLampBrightness());

      startButtonHolding = true;
      setDirectionTimeout = false;

      tmNumHoldTimer.reset();
      tmChangeDirectionTimer.reset();

      changePower(true);
      if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
      return;
    }
  } 

  // кнопка только начала удерживаться
  if (ONflag && (touch.isHolded())){
    int clicks = touch.getHoldClicks();
    LOG( printf_P, PSTR("touch.getHoldClicks()=%d\n"), clicks);
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
    if(!isFirstHoldingPress && (((getLampBrightness() == BRIGHTNESS || getLampBrightness() <= 1) && numHold == 1)
    || ((effects.getSpeed() == 255 || effects.getSpeed() <= 1) && numHold == 2)
    || ((effects.getScale() == 255 || effects.getScale() <= 1) && numHold == 3))){
      if(!setDirectionTimeout){
        LOG(printf_P, PSTR("Граничное значение! numHold: %d brightness: %d speed: %d scale: %d\n"), numHold,getLampBrightness(), effects.getSpeed(), effects.getScale());
        tmChangeDirectionTimer.reset(); // пауза на смену направления
        setDirectionTimeout = true;
      }
      else {
        changeDirection(numHold);
      }
    }

    // Для второго входа, сбрасываем флаг
    isFirstHoldingPress = false;
    
    debugPrint(); // отладка
    if (numHold != 0) {
      tmNumHoldTimer.reset();
      demoTimer(T_RESET); // сбрасываем таймер переключения, если регулируем яркость/скорость/масштаб
    }

    uint8_t newval;
    switch (numHold) {
      case 1:
         newval = constrain(getLampBrightness() + (getLampBrightness() / 25 + 1) * (brightDirection * 2 - 1), 1 , 255);
         // не мелькаем яркостью там где не надо
         if (getNormalizedLampBrightness() != newval) {
           setLampBrightness(newval);
           setBrightness(getNormalizedLampBrightness(), false); // используем общий метод, но без эффекта фейда
         }
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
        LOG(printf_P, PSTR("Новое значение яркости: %d\n"), getLampBrightness());
        break;
      case 2:
        LOG(printf_P, PSTR("Новое значение скорости: %d\n"), effects.getSpeed());
        break;
      case 3:
        LOG(printf_P, PSTR("Новое значение масштаба: %d\n"), effects.getScale());
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
      LOG(printf_P, PSTR("Одиночное нажатие, current: %d, storedEffect: %d\n"), effects.getEn(), storedEffect);

      if (dawnFlag) // нажатие во время будильника
      {
        manualOff = true;
        dawnFlag = false;
        setBrightness(getNormalizedLampBrightness(),false, false); // восстановить яркость
        mode = (storedMode!=LAMPMODE::MODE_ALARMCLOCK?storedMode:LAMPMODE::MODE_NORMAL); // возвращаем предыдущий режим
        if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
        return;
      }

      if(!ONflag){    // лампа была выключена
        numHold = 0;
        mode = MODE_NORMAL;
        if(storedEffect!=EFF_NONE) {    // переключение на ПРЕДЫДУЩИЙ эффект только если он был запомнен, иначе используется ТЕКУЩИЙ из конфига
          switcheffect(SW_SPECIFIC, isFaderON, storedEffect); // ПРЕДЫДУЩИЙ будет запоминаться для случая включения белой лампы
        } else {
          //changePower(true);
          setOnOff(true);
          switcheffect(SW_SPECIFIC, getFaderFlag(), effects.getEn());
        }
      } else {        // лампа была включена
        storedEffect = ((effects.getEn() == EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
        changePower(false);
      }

      LOG(printf_P, PSTR("Лампа %s, lamp mode: %d, current: %d, storedEffect: %d\n"), ONflag ? F("включена") : F("выключена") , mode, effects.getEn(), storedEffect);
    }

    // двухкратное нажатие  - следующий эффект
    if (ONflag && clickCount == 2U)
    {
        LOG(printf_P, PSTR("Даблклик, lamp mode: %d, current: %d, storedEffect: %d\n"), mode, effects.getEn(), storedEffect);
      switcheffect(SW_NEXT, isFaderON);
    }

    // трёхкратное нажатие - предыдущий эффект
    if (ONflag && clickCount == 3U)
    {
      switcheffect(SW_PREV, isFaderON);
    }

    // четырёхкратное нажатие - запуск сервиса ОТА
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
        if(!isLampOn()){
            disableEffectsUntilText(); // будем выводить текст, при выкюченной матрице
            setOffAfterText();
            setOnOff(true);
            setBrightness(1,false,false); // выводить будем минимальной яркостью getNormalizedLampBrightness()
            sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::White);
        } else {
            sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::White);
        }
    }

    // шестикратное нажатие
    if (clickCount == 6U)                                     // вывод текущего времени бегущей строкой
    {
        if(!isLampOn()){
            disableEffectsUntilText(); // будем выводить текст, при выкюченной матрице
            setOffAfterText();
            setOnOff(true);
            setBrightness(1,false,false); // выводить будем минимальной яркостью getNormalizedLampBrightness()
            sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Green); // вывести время на лампу
        } else {
            sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), CRGB::Green); // вывести время на лампу
        }
    }

    if(clickCount>0 && !isOffAfterText){ // для случая ВРЕМЕННО включенной лампы не дергаем обновления параметров (вывод IP/времени при выключенной)
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
      brightness(BRIGHTNESS, false);   // не помню, почему тут стояло 255... надо будет проверить работу рассвета :), ниже есть доп. ограничение - DAWN_BRIGHT
      // величина рассвета 0-255
      int16_t dawnPosition = map((millis()-GSHMEM.startmillis)/1000,0,300,0,255); // 0...300 секунд приведенные к 0...255
      dawnPosition = constrain(dawnPosition, 0, 255);
      GSHMEM.dawnColorMinus[0] = CHSV(map(dawnPosition, 0, 255, 10, 35),
                        map(dawnPosition, 0, 255, 255, 170),
                        map(dawnPosition, 0, 255, 10, DAWN_BRIGHT));
    }

    if(LAMPMODE::MODE_ALARMCLOCK && ((millis()-GSHMEM.startmillis)/1000>(5+DAWN_TIMEOUT)*60+30 || manualOff)){ // рассвет закончился
      mode = (storedMode!=LAMPMODE::MODE_ALARMCLOCK?storedMode:LAMPMODE::MODE_NORMAL);
      // не время будильника (ещё не начался или закончился по времени)
      if (dawnFlag)
      {
        dawnFlag = false;
        manualOff = false;
        if(!ONflag){
          FastLED.clear();
          FastLED.show();
        }
      }
      // #if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
      // digitalWrite(ALARM_PIN, !ALARM_LEVEL);
      // #endif

      // #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
      // digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
      // #endif

      LOG(println, F("Отключение будильника рассвет."));
      brightness(getNormalizedLampBrightness());
      return; // на выход
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
            sendStringToLamp(timeProcessor.getFormattedShortTime().c_str(), letterColor, true);
          }
        }
#endif

        // fill_solid(leds, NUM_LEDS, dawnColor);
        for (uint16_t i = 0U; i < NUM_LEDS; i++)
            leds[i] = GSHMEM.dawnColorMinus[i%(sizeof(GSHMEM.dawnColorMinus)/sizeof(CHSV))];
        dawnFlag = true;
        if (!ONflag) effectsTimer(T_ENABLE);  // наверное нужен для печати какой-то строки, todo: вопрос где его потом выключить (не обязательно)
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
  /*
   * Здесь имеет место странная специфика тикера,
   * если где-то в коде сделали детач, но таймер уже успел к тому времени "выстрелить"
   * функция все равно будет запущена в loop(), она просто ждет своей очереди
   */
  if (!_effectsTicker.active() ) return;

  uint32_t _begin = millis();
  if(dawnFlag){
    doPrintStringToLamp(); // обработчик печати строки
    _effectsTicker.once_ms_scheduled(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
    return;
  }

  if(!isEffectsDisabledUntilText){
    // посчитать текущий эффект (сохранить кадр в буфер, если ОК) 
    if(effects.worker->run(getUnsafeLedsArray(), effects.getCurrent()->param)) {
#ifdef USELEDBUF
      ledsbuff.resize(NUM_LEDS);
      std::copy(leds, leds + NUM_LEDS, ledsbuff.begin());
#endif
    }
  }

  doPrintStringToLamp(); // обработчик печати строки
#ifdef VERTGAUGE
  GaugeShow();
#endif

  if (isEffectsDisabledUntilText || effects.worker->status()) {
    // выводим кадр только если есть текст или эффект
    _effectsTicker.once_ms_scheduled(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
  } else {
    // иначе возвращаемся к началу обсчета следующего кадра
    _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
  }

}
// end of void LAMP::effectsTick()

/*
 * вывод готового кадра на матрицу,
 * и перезапуск эффект-процессора
 */
void LAMP::frameShow(const uint32_t ticktime){
  /*
   * Здесь имеет место странная специфика тикера,
   * если где-то в коде сделали детач, но таймер уже успел к тому времени "выстрелить"
   * функция все равно будет запущена в loop(), она просто ждет своей очереди
   */
  if (!_effectsTicker.active() ) return;

  FastLED.show();
// восстановление кадра с прорисованным эффектом из буфера (без текста и индикаторов) 
#ifdef USELEDBUF
  if (!ledsbuff.empty()) {
    std::copy( ledsbuff.begin(), ledsbuff.end(), leds );
    ledsbuff.resize(0);
    ledsbuff.shrink_to_fit();
  }
#endif

  // откладываем пересчет эффекта на время для желаемого FPS, либо
  // на минимальный интервал в следующем loop()
  int32_t delay = EFFECTS_RUN_TIMER + ticktime - millis();
  if (delay < LED_SHOW_DELAY) delay = LED_SHOW_DELAY;
  _effectsTicker.once_ms_scheduled(delay, std::bind(&LAMP::effectsTick, this));
#ifdef LAMP_DEBUG
  ++fps;
#endif
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
      LOG(printf_P, PSTR("changeDirection %d, %d, %d\n"), brightDirection, speedDirection, scaleDirection);
    }

  void LAMP::debugPrint(){
      LOG(printf_P, PSTR("lampMode: %d numHold: %d currentMode: %d brightness: %d speed: %d scale: %d\n"), mode, numHold, effects.getEn(), getLampBrightness(), effects.getSpeed(), effects.getScale());
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


LAMP::LAMP() : docArrMessages(512), tmConfigSaveTime(0), tmNumHoldTimer(NUMHOLD_TIME), tmStringStepTime(DEFAULT_TEXT_SPEED), tmNewYearMessage(0), _fadeTicker(), _fadeeffectTicker()
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
      isFaderON = true; // признак того, что используется фейдер для смены эффектов
      isGlobalBrightness = false; // признак использования глобальной яркости для всех режимов
      isFirstHoldingPress = false; // флаг: только начали удерживать?
      startButtonHolding = false; // кнопка удерживается
      buttonEnabled = true; // кнопка обрабатывается если true, пока что обрабатывается всегда :)
      brightDirection = false;
      speedDirection = false;
      scaleDirection = false;
      setDirectionTimeout = false; // флаг: начало отсчета таймаута на смену направления регулировки
      isStringPrinting = false; // печатается ли прямо сейчас строка?
      isEffectsDisabledUntilText = false;
      isOffAfterText = false;
      isEventsHandled = true;
      _brt =0;
      _steps = 0;
      _brtincrement = 0;
#ifdef MIC_EFFECTS
      isCalibrationRequest = false; // находимся ли в режиме калибровки микрофона
      isMicOn = true; // глобальное испльзование микрофона
      micAnalyseDivider = 1; // анализ каждый раз
#endif

      lamp_init(); // инициализация и настройка лампы
    }

void LAMP::changePower() {changePower(!ONflag);}

void LAMP::changePower(bool flag) // флаг включения/выключения меняем через один метод
{
  if ( flag == ONflag) return;  // пропускаем холостые вызовы
  LOG(printf_P, PSTR("Lamp powering %s\n"), flag ? "ON": "Off");
  ONflag = flag;

  if (flag){
    effectsTimer(T_ENABLE);
  } else  {
    fadelight(0, FADE_TIME, std::bind(&LAMP::effectsTimer, this, SCHEDULER::T_DISABLE));  // гасим эффект-процессор
    demoTimer(T_DISABLE);     // гасим Демо-таймер
  }

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
      digitalWrite(MOSFET_PIN, (ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
#endif

      if (CURRENT_LIMIT > 0){
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); // установка максимального тока БП, более чем актуально))). Проверил, без этого куска - ограничение по току не работает :)
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

void LAMP::startAlarm()
{
  storedMode = ((mode == LAMPMODE::MODE_ALARMCLOCK ) ? storedMode: mode);
  mode = LAMPMODE::MODE_ALARMCLOCK;
}

/*
 * запускаем режим "ДЕМО"
 */
void LAMP::startDemoMode()
{
  storedEffect = ((effects.getEn() == EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  mode = LAMPMODE::MODE_DEMO;
  randomSeed(millis());
  demoNext();
  myLamp.sendStringToLamp(String(PSTR("- Demo ON -")).c_str(), CRGB::Green);
  demoTimer(T_ENABLE);
}

void LAMP::startNormalMode()
{
  mode = LAMPMODE::MODE_NORMAL;
  demoTimer(T_DISABLE);
  if(storedEffect!=EFF_NONE) {    // ничего не должно происходить, включаемся на текущем :), текущий всегда определен...
    switcheffect(SW_SPECIFIC, isFaderON, storedEffect);
  } else if(effects.getEn()==EFF_NONE){ // если по каким-то причинам текущий пустой, то выбираем рандомный
    switcheffect(SW_RND, isFaderON); 
  }
}
#ifdef OTA
void LAMP::startOTAUpdate()
{        
  mode = MODE_OTA;
  effects.moveBy(EFF_MATRIX); // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
  FastLED.clear();
  changePower(true);
  if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
  sendStringToLamp(String(PSTR("- OTA UPDATE ON -")).c_str(), CRGB::Green);
}
#endif
bool LAMP::fillStringManual(const char* text,  const CRGB &letterColor, bool stopText, bool isInverse, int32_t pos, int8_t letSpace, int8_t txtOffset, int8_t letWidth, int8_t letHeight)
{
  static int32_t offset = (MIRR_V ? 0 : WIDTH);

  if(pos)
    offset = (MIRR_V ? 0 + pos : WIDTH - pos);
  
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
  if(pos) // если задана позиция, то считаем что уже отобразили
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

void LAMP::sendStringToLamp(const char* text, const CRGB &letterColor, bool forcePrint, int8_t textOffset, int16_t fixedPos)
{
  if((!ONflag && !forcePrint) || (dawnFlag && !forcePrint)) return; // если выключена, или если будильник, но не задан принудительный вывод - то на выход
  if(textOffset==-128) textOffset=this->txtOffset;

  if(text==nullptr){ // текст пустой
    if(!isStringPrinting){ // ничего сейчас не печатается
      if(docArrMessages.isNull()){ // массив пустой
        return; // на выход
      }
      else { // есть что печатать
        JsonArray arr = docArrMessages.as<JsonArray>(); // используем имеющийся
        JsonObject var=arr[0]; // извлекаем очередной
        doPrintStringToLamp(var[F("s")], (var[F("c")].as<unsigned long>()), (var[F("o")].as<int>()), (var[F("f")].as<int>())); // отправляем
        arr.remove(0); // удаляем отправленный
      }
    } else {
        // текст на входе пустой, идет печать
        return; // на выход
    }
  } else { // текст не пустой
    if(!isStringPrinting){ // ничего сейчас не печатается
      doPrintStringToLamp(text, letterColor, textOffset, fixedPos); // отправляем
    } else { // идет печать, помещаем в очередь
      JsonArray arr; // добавляем в очередь
      
      if(!docArrMessages.isNull())
        arr = docArrMessages.as<JsonArray>(); // используем имеющийся
      else
        arr = docArrMessages.to<JsonArray>(); // создаем новый
      
      JsonObject var = arr.createNestedObject();
      var[F("s")]=text;
      var[F("c")]=((unsigned long)letterColor.r<<16)+((unsigned long)letterColor.g<<8)+(unsigned long)letterColor.b;
      var[F("o")]=textOffset;
      var[F("f")]=fixedPos;

      LOG(println, docArrMessages.as<String>());

      String tmp; // Тут шаманство, чтобы не ломало JSON
      serializeJson(docArrMessages, tmp);
      deserializeJson(docArrMessages, tmp);
    }
  }
}

void LAMP::doPrintStringToLamp(const char* text,  const CRGB &letterColor, const int8_t textOffset, const int16_t fixedPos)
{
  static String toPrint;
  static CRGB _letterColor;

  isStringPrinting = true;
  int8_t offs=(textOffset==-128?txtOffset:textOffset);

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
    if(!fillStringManual(toPrint.c_str(), _letterColor, false, dawnFlag, fixedPos, (fixedPos? 0 : LET_SPACE), offs)){ // смещаем
      tmStringStepTime.reset();
    }
    else {
      isStringPrinting = false;
      toPrint.clear(); // все напечатали
      sendStringToLamp(); // получаем новую порцию
    }
  } else {
    fillStringManual(toPrint.c_str(), _letterColor, true, dawnFlag, fixedPos, (fixedPos? 0 : LET_SPACE), offs);
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

    LOG(printf_P, PSTR("Prepared message: %s\n"), strMessage);
    sendStringToLamp(strMessage, LETTER_COLOR);
  }
}

void LAMP::periodicTimeHandle()
{
  static bool cancel = false;
  
  time_t tm = timeProcessor.getUnixTime();
  //LOG(println, tm);
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
    //   LOG(println, counter);
    // }

    //LOG(println, last_freq);
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

void LAMP::fadelight(const uint8_t _targetbrightness, const uint32_t _duration, std::function<void(void)> callback) {
    LOG(printf, PSTR("Fading to: %d\n"), _targetbrightness);
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
        if (callback != nullptr) _fadeTicker.once_ms_scheduled(0, callback);
        return;
    }

    _brtincrement = (_targetbrightness - _brt) / _steps;

    //_SPTO(Serial.printf_P, F_fadeinfo, _brt, _targetbrightness, _steps, _brtincrement)); _SPLN("");
    _fadeTicker.attach_ms(FADE_STEPTIME, std::bind(&LAMP::fader, this, _targetbrightness, callback));
}

/*
 * Change global brightness with or without fade effect
 * fade applied in non-blocking way
 * FastLED dim8 function applied internaly for natural brightness controll
 * @param uint8_t _brt - target brigtness level 0-255
 * @param bool fade - use fade effect on brightness change
 */
void LAMP::setBrightness(const uint8_t _brt, const bool fade, const bool natural){
    LOG(printf_P, PSTR("Set brightness: %u\n"), _brt);
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
    uint8_t _cur = natural ? brighten8_video(FastLED.getBrightness()) : FastLED.getBrightness();
    if ( _cur == _brt) return;

    if (_brt) {
      FastLED.setBrightness(natural ? dim8_video(_brt) : _brt);
    } else {
      FastLED.setBrightness(0); // полностью гасим лапу если нужна 0-я яркость
      FastLED.show();
    }
}

/*
 * Fade light callback
 * @param uint8_t _tgtbrt - last step brightness
 */
void LAMP::fader(const uint8_t _tgtbrt, std::function<void(void)> callback){
  --_steps;
  if (! _steps) {   // on last step
      if (callback != nullptr) {
        _fadeTicker.once_ms_scheduled(0, callback);
      } else { _fadeTicker.detach(); }
      _brt = _tgtbrt;
  } else {
      _brt += _brtincrement;
  }

  brightness(_brt);
}

/*
 * buttonPress - управление планировщиком опроса кнопки
 * оберка нужна т.к. touch.tick() нельзя положить в ICACHE_RAM
 * по наступлению прерывания "нажато" врубаем опрос событий кнопки не реже чем BUTTON_STEP_TIMEOUT/2 чтобы отловить "удержание"
 * 
 * т.к. гайвербаттон не умеет работать чисто по событиям, при "отпускании" продолжаем дергать обработчик раз в секунду,
 * чтобы не он забыл зачем живет :)
 */
#ifdef ESP_USE_BUTTON
void LAMP::buttonPress(bool state){

  if (!buttonEnabled)   // события кнопки не обрабатываются, если она заблокирована
  {
    _buttonTicker.detach();
    return;
  }

  LOG(printf_P, PSTR("Button %s: %lu\n"), state ? PSTR("press") : PSTR("release"), millis());

  _buttonTicker.attach_ms_scheduled(state ? BUTTON_STEP_TIMEOUT/2 : 1000, std::bind(&LAMP::buttonTick, this));

  buttonTick();   // обрабатываем текущее нажатие вне очереди
}
#endif

/*
 * переключатель эффектов для других методов,
 * может использовать фейдер, выбирать случайный эффект для демо
 * @param EFFSWITCH action - вид переключения (пред, след, случ.)
 * @param fade - переключаться через фейдер или сразу
 */
void LAMP::switcheffect(EFFSWITCH action, bool fade, EFF_ENUM effnb) {
  LOG(printf_P, PSTR("EFFSWITCH=%d, fade=%d, effnb=%d\n"), action, fade, effnb);
  if (action == SW_DELAY ) {
    action = _postponedSW;
    _postponedSW = EFFSWITCH::SW_NONE;  // сбрасываем отложенный эффект
  } else if (fade && ONflag ) {         // тухнем "вниз" только на включенной лампе
    _postponedSW = action;  // откладывает смену эффекта на следующий вызов через коллбек от фейдера
    fadelight(FADE_MINCHANGEBRT, FADE_TIME, std::bind(&LAMP::switcheffect, this, EFFSWITCH::SW_DELAY, fade, effnb));
    return;
  }

  changePower(true);  // любой запрос на смену эффекта автоматом включает лампу

  switch (action)
  {
  case EFFSWITCH::SW_NEXT :
      effects.moveNext();
      break;
  case EFFSWITCH::SW_NEXT_DEMO :
      effects.moveBy(1);
      break;
  case EFFSWITCH::SW_PREV :
      effects.movePrev();
      break;
  case EFFSWITCH::SW_SPECIFIC :
      effects.moveBy(effnb);
      break;
  case EFFSWITCH::SW_RND :
      effects.moveBy(random(0, effects.getModeAmount()));
      LOG(printf_P, PSTR("%s DEMO mode ON. Current: %d, storedEffect: %d\n"),(RANDOM_DEMO?PSTR("Random"):PSTR("Seq")) , effects.getEn(), storedEffect);
      break;
  default:
      return;
      break;
  }

  EFFECT *currentEffect = effects.getCurrent();

  // отрисовать текущий эффект
  effects.worker->run(getUnsafeLedsArray(), effects.getCurrent()->param);

  if (fade) {
    fadelight(getNormalizedLampBrightness());
  } else {
    setBrightness(getNormalizedLampBrightness());
  }

  if(updateParmFunc!=nullptr) updateParmFunc(); // обновить параметры UI
}

/*
 * включает/выключает режим "демо", возвращает установленный статус
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
void LAMP::demoTimer(SCHEDULER action){
//  LOG.printf_P(PSTR("demoTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    _demoTicker.detach();
    break;
  case SCHEDULER::T_ENABLE :
    _demoTicker.attach_scheduled(DEMO_TIMEOUT, std::bind(&LAMP::demoNext, this));
    break;
  case SCHEDULER::T_RESET :
    if(dawnFlag) { mode = (storedMode!=LAMPMODE::MODE_ALARMCLOCK?storedMode:LAMPMODE::MODE_NORMAL); manualOff = true; dawnFlag = false; FastLED.clear(); FastLED.show(); }// тут же сбросим и будильник
    if (_demoTicker.active() ) demoTimer(T_ENABLE);
    break;
  default:
    return;
  }
}

void LAMP::effectsTimer(SCHEDULER action) {
//  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    _effectsTicker.detach();
    break;
  case SCHEDULER::T_ENABLE :
    _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
    break;
  case SCHEDULER::T_RESET :
    if (_effectsTicker.active() ) effectsTimer(T_ENABLE);
    break;
  default:
    return;
  }
}

//-----------------------------
// ------------- мигающий цвет (не эффект! используется для отображения краткосрочного предупреждения; блокирующий код!) -------------
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
  // наверное это не актуально
  //myLamp.setLoading();                                       // принудительное отображение текущего эффекта (того, что был активен перед предупреждением)
}
//-----------------------------