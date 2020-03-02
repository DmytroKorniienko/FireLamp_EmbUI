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
#endif
}

void LAMP::handle()
{
#ifdef ESP_USE_BUTTON
  if (buttonEnabled)
  {
    buttonTick(); // обработчик кнопки
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

  touch.tick();

  if (!ONflag) { // Обработка из выключенного состояния
    if (touch.isDouble()) { // Демо-режим, с переключением каждые 30 секунд для двойного клика в выключенном состоянии
      mode = MODE_DEMO;
      randomSeed(millis());
      effects.moveBy(random(0, MODE_AMOUNT));
      //globalBrightness = EEPROM.read(EEPROM_GLOBAL_BRIGHTNESS);
      delay(5);
      FastLED.setBrightness(getLampBrightness());
      ONflag = true;
      tmDemoTimer.reset(); // момент включения для таймаута в DEMOTIME
      changePower();
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("Demo mode: %d, storeEffect: %d\n"), mode, storedEffect);
#endif
    }
    
    if (touch.isHolded()) {
#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("Удержание кнопки из выключенного состояния\n"));
#endif
      // Включаем белую лампу в полную яркость
      numHold = 1;
      mode = MODE_WHITELAMP;
      effects.moveBy(EFF_WHITE_COLOR);
      effects.setBrightness(BRIGHTNESS);
      FastLED.setBrightness(getLampBrightness());

#ifdef LAMP_DEBUG
      LOG.printf_P(PSTR("currentMode: %d, storeEffect: %d\n"), mode, storedEffect);
#endif
      
      ONflag = true;
      startButtonHolding = true;
      setDirectionTimeout = false;
      tmNumHoldTimer.reset();
      tmChangeDirectionTimer.reset();
      brightDirection = 1;
      changePower();
    }
  } 

  uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;

  // однократное нажатие
  if (clickCount == 1U)
  {
    // #ifdef ONETOUCH_CLOCK     
    // if (!osd_printCurrentTime(CRGB::White)){}          // попытка напечатать время. Если не получается или текст уже на экране, то переходим ко включению
    // #endif  

    // #ifdef ONETOUCH_CLOCK
    // printTime(thisTime, true, ONflag, false, false); // оригинальный вариант вывода времени
    // #endif
    
    if(!ONflag){
      numHold = 0;
      mode = MODE_NORMAL;
      effects.moveBy(storedEffect);
    } else {
      storedEffect = ((mode == EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
    }
    
    #ifdef LAMP_DEBUG
    if(ONflag)
      LOG.printf_P(PSTR("Лампа выключена, currentMode: %d, storeEffect: %d\n"), mode, storedEffect);
    else
      LOG.printf_P(PSTR("Лампа включена, currentMode: %d, storeEffect: %d\n"), mode, storedEffect);
    #endif
    
    if (dawnFlag)
    {
      manualOff = true;
      dawnFlag = false;
      FastLED.setBrightness(getLampBrightness());
      changePower();
    }
    else
    {
      ONflag = !ONflag;
      changePower();
    }
    loadingFlag = true;

    #ifdef USE_MQTT
    if (espMode == 1U)
    {
      MqttManager::needToPublish = true;
    }
    #endif
  }


  // двухкратное нажатие
  if (ONflag && clickCount == 2U)
  {
    effects.moveNext();
    FastLED.setBrightness(getLampBrightness());
    loadingFlag = true;
    FastLED.clear();
    delay(1);
  }


  // трёхкратное нажатие
  if (ONflag && clickCount == 3U)
  {
    effects.movePrev();
    FastLED.setBrightness(getLampBrightness());
    loadingFlag = true;
    FastLED.clear();
    delay(1);
  }


  // четырёхкратное нажатие
  if (clickCount == 4U)
  {
    // #ifdef OTA
    // if (otaManager.RequestOtaUpdate())
    // {
    //   ONflag = true;
    //   currentMode = EFF_MATRIX;                             // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
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

  // кнопка только начала удерживаться
  if (ONflag && touch.isHolded()){ //  && lampMode != MODE_WHITELAMP
    startButtonHolding = true;
    setDirectionTimeout = false;
    isFirstHoldingPress = true;
    switch (touch.getHoldClicks()){
      case 0U: {
        if(!numHold){
          numHold = 1;
          // if(mode==MODE_DEMO) {
          //   storeEffBrightness = modes[currentMode].Brightness; // запоминаем яркость эффекта, которая была изначально
          //   modes[currentMode].Brightness = GlobalBrightness; // перенакрываем глобальной
          // }
        }
        break;
      }
      case 1U: {
        if(!numHold)
          numHold = 2;
        break;
      }
      case 2U: {
        if(!numHold)
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
      //loadingFlag = true;
    }
    
    switch (numHold) {
      case 1:
         setLampBrightness(constrain(getLampBrightness() + (getLampBrightness() / 25 + 1) * (brightDirection * 2 - 1), 1 , BRIGHTNESS));
         break;

      case 2:
        effects.setSpeed(constrain(effects.getSpeed() + (effects.getSpeed() / 25 + 1) * (speedDirection * 2 - 1), 1 , 255));
        break;

      case 3:
        effects.setScale(constrain(effects.getScale() + (effects.getScale() / 25 + 1) * (scaleDirection * 2 - 1), 1 , 255));
        break;
    }

    if(numHold==1){
        FastLED.setBrightness(getLampBrightness());
    }
  }

  // кнопка отпущена после удерживания
  if (ONflag && !touch.isHold() && startButtonHolding)      // кнопка отпущена после удерживания, нужно отправить MQTT сообщение об изменении яркости лампы
  {
    startButtonHolding = false;
    setDirectionTimeout = false;
    loadingFlag = true;

    changeDirection(numHold);

    #ifdef LAMP_DEBUG
    switch (numHold) {
      case 1:
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), getLampBrightness());
        //if(lampMode==MODE_DEMO) {GlobalBrightness = modes[currentMode].Brightness; EepromManager::SaveGlobalBrightness(&GlobalBrightness); modes[currentMode].Brightness = storeEffBrightness;}
        break;
      case 2:
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), effects.getSpeed());
        break;
      case 3:
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), effects.getScale());
        break;
    }
    #endif

    #ifdef USE_MQTT
    if (espMode == 1U)
    {
      MqttManager::needToPublish = true;
    }
    #endif

    // if(lampMode==MODE_DEMO && GlobalBrightness>0)
    //   FastLED.setBrightness(GlobalBrightness);
    // else
    //   FastLED.setBrightness(modes[currentMode].Brightness);

    // // возвращаем сохраненную яркость, после регулировки глобальной яркости для демо
    // if(lampMode == MODE_DEMO)
    //   modes[currentMode].Brightness = storeEffBrightness;
  }

  if (tmNumHoldTimer.isReadyManual() && !startButtonHolding) { // сброс текущей комбинации в обычном режиме, если уже не нажата
      numHold = 0;
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
  static byte faderStep = 1;
  EFFECT *currentEffect = effects.getCurrent();

  if (!dawnFlag) // флаг устанавливается будильником рассвет
  {
    if (ONflag)
    {
      if(millis() - effTimer >= EFFECTS_RUN_TIMER && !isFaderOn){ // effects.getSpeed() - теперь эта обработка будет внутри эффектов
        if(currentEffect->func!=nullptr){
          //try{
            currentEffect->func(getUnsafeLedsArray(), currentEffect->param); // отрисовать текущий эффект
          //} catch(...)
          //{
          //  //ONflag = false;
          //  effects.moveBy(EFF_ENUM::EFF_NONE);
          //}
        }
        effTimer = millis();
      }

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
      } else { // время на фейдер вышло
        tmFaderTimeout.setInterval(0); // отключить до следующего раза, также переключаем эффект на новый, заодно запоминаем яркость текущего эффекта
        if(RANDOM_DEMO)
          effects.moveBy(random(0, effects.getModeAmount()));
        else
          effects.moveNext();
        //storeEffBrightness = modes[currentMode].Brightness;
        loadingFlag = true; // некоторые эффекты требуют начальной иницализации, поэтому делаем так...
        isFaderOn = false;
        faderStep = 1;
#ifdef LAMP_DEBUG
        LOG.printf_P(PSTR("%s Demo mode: %d, storeEffect: %d\n"),(RANDOM_DEMO?F("Random"):F("Seq")) , mode, storedEffect);
#endif
        currentEffect = effects.getCurrent();
        if(currentEffect->func!=nullptr)
          currentEffect->func(getUnsafeLedsArray(), currentEffect->param); // отрисовать текущий эффект
      }

#ifdef USELEDBUF
      memcpy(ledsbuff, leds, sizeof(CRGB)* NUM_LEDS);                             // сохранение сформированной картинки эффекта в буфер (для медленных или зависящих от предыдущей)
#endif

      if(tmDemoTimer.isReady() && (mode == MODE_DEMO)){
        tmFaderTimeout.setInterval(FADERTIMEOUT); // взводим таймер фейдера
        tmFaderTimeout.reset();
        isFaderOn = true;
        faderStep = 1;
      }
      
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
