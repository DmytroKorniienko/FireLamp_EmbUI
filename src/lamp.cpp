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

#include "main.h"
#include "effectmath.h"

extern LAMP myLamp; // Объект лампы

void LAMP::lamp_init(const uint16_t curlimit)
{
  setcurLimit(curlimit);
  // Такую коррекцию стоит оставить, с ней можно получить хотя бы более менее жёлтый цвет. Иначе он всегда зеленит (коррекцию нашел на просторах, люди рекомендуют)
  //FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalPixelString);
  FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(getUnsafeLedsArray(), NUM_LEDS)/*.setCorrection(0xFFE08C)*/; // цветокоррекция нафиг не нужна, проверяется на минимальной яркости в белой лампе

  brightness(0, false);                          // начинаем с полностью потушеной матрицы 1-й яркости
  if (curlimit > 0){
    FastLED.setMaxPowerInVoltsAndMilliamps(5, curlimit); // установка максимального тока БП
  }
  FastLED.clearData();
  //FastLED.clear();                                            // очистка матрицы
  //FastLED.show(); // для ESP32 вызывает перезагрузку циклическую!!! Убираю, т.к. при 160Мгц вызывает бросок тока и яркости!!! Не включать и оставить как напоминание!
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
#ifdef MIC_EFFECTS
  static unsigned long mic_check = 0; // = 40000; // пропускаю первые 40 секунд
  if(effects.worker && flags.isMicOn && (flags.ONflag || isMicCalibration()) && !isAlarm() && mic_check + MIC_POLLRATE < millis()){
    if(effects.worker->isMicOn() || isMicCalibration())
      micHandler();
    mic_check = millis();
  }
#endif

// Deprecated
// #if defined(LAMP_DEBUG) && DEBUG_TELNET_OUTPUT
//   handleTelnetClient();
// #endif

  // все что ниже, будет выполняться раз в 0.999 секундy
  static unsigned long wait_handlers;
  if (wait_handlers + 999U > millis())
      return;
  wait_handlers = millis();
#ifdef LAMP_DEBUG
  EVERY_N_SECONDS(15){
    // fps counter
    LOG(printf_P, PSTR("Eff:%d FPS: %u\n"), effects.getEn(), avgfps);

#ifdef ESP8266
    LOG(printf_P, PSTR("MEM stat: %d, HF: %d, Time: %s\n"), ESP.getFreeHeap(), ESP.getHeapFragmentation(), embui.timeProcessor.getFormattedShortTime().c_str());
#endif

#ifdef ESP32
    LOG(printf_P, PSTR("MEM stat: %d, Time: %s\n"), ESP.getFreeHeap(), embui.timeProcessor.getFormattedShortTime().c_str());
#endif

  }
  avgfps = (avgfps+fps) / 2;
  fps = 0; // сброс FPS раз в секунду
#endif

  // будильник обрабатываем раз в секунду
  alarmWorker();

  if(iflags.isEffectsDisabledUntilText && !iflags.isStringPrinting) {
    setBrightness(0,false,false); // напечатали, можно гасить матрицу :)
    iflags.isEffectsDisabledUntilText = false;
  }

  // отложенное включение/выключение
  if(iflags.isOffAfterText && !iflags.isStringPrinting) {
    iflags.isOffAfterText = false;
    changePower(false);
  }

  newYearMessageHandle();
  ConfigSaveCheck(); // для взведенного таймера автосохранения настроек

#ifdef OTA
  otaManager.HandleOtaUpdate();                       // ожидание и обработка команды на обновление прошивки по воздуху
#endif

  // обработчик событий (пока не выкину в планировщик)
  if (flags.isEventsHandled) {
    events.events_handle();
  }

}

// обработчик будильника "рассвет"
void LAMP::alarmWorker(){
    // временно статикой, дальше нужно будет переписать
    static CHSV dawnColorMinus[6];                                            // цвет "рассвета"
    static uint8_t dawnCounter = 0;                                           // счётчик первых шагов будильника
    static time_t startmillis;
    
    if (mode != LAMPMODE::MODE_ALARMCLOCK){
      iflags.dawnFlag = false;
      return;
    }

    // проверка рассвета, первый вход в функцию
    if (!iflags.dawnFlag){
      startmillis = millis();
      memset(dawnColorMinus,0,sizeof(dawnColorMinus));
      dawnCounter = 0;
      FastLED.clear();
      brightness(BRIGHTNESS, false);   // не помню, почему тут стояло 255... надо будет проверить работу рассвета :), ниже есть доп. ограничение - DAWN_BRIGHT
      // величина рассвета 0-255
      int16_t dawnPosition = map((millis()-startmillis)/1000,0,curAlarm.alarmP*60,0,255); // 0...curAlarm.alarmP*60 секунд приведенные к 0...255
      dawnPosition = constrain(dawnPosition, 0, 255);
      dawnColorMinus[0] = CHSV(map(dawnPosition, 0, 255, 10, 35),
        map(dawnPosition, 0, 255, 255, 170),
        map(dawnPosition, 0, 255, 10, DAWN_BRIGHT)
      );
    }

    if (((millis() - startmillis) / 1000 > (((uint32_t)(curAlarm.alarmP) + curAlarm.alarmT) * 60UL+30U))) {
      // рассвет закончился
      stopAlarm();
      return;
    }

    // проверка рассвета
    EVERY_N_SECONDS(10){
      // величина рассвета 0-255
      int16_t dawnPosition = map((millis()-startmillis)/1000,0,curAlarm.alarmP*60,0,255); // 0...300 секунд приведенные к 0...255
      dawnPosition = constrain(dawnPosition, 0, 255);

#ifdef MP3PLAYER
      mp3->setTempVolume(map(dawnPosition,0,255,1,(curAlarm.isLimitVol ? mp3->getVolume() : 30))); // запуск звука будильника
      if(dawnPosition==255 && !curAlarm.isStartSnd && !mp3->alarm){
        mp3->setAlarm(true);
        mp3->StartAlarmSound(curAlarm.type); // запуск звука будильника
      }
#endif
      
      dawnColorMinus[0] = CHSV(map(dawnPosition, 0, 255, 10, 35),
        map(dawnPosition, 0, 255, 255, 170),
        map(dawnPosition, 0, 255, 10, DAWN_BRIGHT)
      );
      dawnCounter++; //=dawnCounter%(sizeof(dawnColorMinus)/sizeof(CHSV))+1;

      for (uint8_t i = sizeof(dawnColorMinus) / sizeof(CHSV) - 1; i > 0U; i--){
          dawnColorMinus[i]=((dawnCounter > i)?dawnColorMinus[i-1]:dawnColorMinus[i]);
      }
    }

    EVERY_N_SECONDS(1){
      if (embui.timeProcessor.seconds00()) {
        CRGB letterColor;
        hsv2rgb_rainbow(dawnColorMinus[0], letterColor); // конвертация цвета времени, с учетом текущей точки рассвета
        if(!curAlarm.msg.isEmpty()) {
            sendStringToLamp(curAlarm.msg.c_str(), letterColor, true);
        } else {
#ifdef PRINT_ALARM_TIME
#ifdef MP3PLAYER
          if(mp3->alarm) // если отложенный звук будильника, то время тоже не выводим, т.к. может быть включено озвучивание
#endif
            sendStringToLamp(String(F("%TM")).c_str(), letterColor, true);
#endif
        }
      }
    }

    for (uint16_t i = 0U; i < NUM_LEDS; i++) {
        getUnsafeLedsArray()[i] = dawnColorMinus[i%(sizeof(dawnColorMinus)/sizeof(CHSV))];
    }
    iflags.dawnFlag = true;
}

void LAMP::effectsTick(){
  /*
   * Здесь имеет место странная специфика тикера,
   * если где-то в коде сделали детач, но таймер уже успел к тому времени "выстрелить"
   * функция все равно будет запущена в loop(), она просто ждет своей очереди
   */
  _begin = millis();

  if (_effectsTicker.active() && !isAlarm()) { // && !isWarning()
    //if(millis()<5000) return; // затычка до выяснения
    if(!iflags.isEffectsDisabledUntilText){
      if (!ledsbuff.empty()) {
        std::copy( ledsbuff.begin(), ledsbuff.end(), getUnsafeLedsArray() );
        if(!iflags.isStringPrinting){ // чистить буфер только если не выводится строка, иначе держать его
          ledsbuff.resize(0);
          ledsbuff.shrink_to_fit();
        }
      }
      // посчитать текущий эффект (сохранить кадр в буфер, если ОК)
      if(effects.worker ? effects.worker->run(getUnsafeLedsArray(), &effects) : 1) {
        ledsbuff.resize(NUM_LEDS);
        std::copy(getUnsafeLedsArray(), getUnsafeLedsArray() + NUM_LEDS, ledsbuff.begin());
      }
    }
  }

  if(!drawbuff.empty()){
    uint8_t mi;
    for(uint16_t i=0; i<drawbuff.size() && i<NUM_LEDS; i++){
      mi = drawbuff[i].r > drawbuff[i].g ? drawbuff[i].r : drawbuff[i].g;
      mi = mi > drawbuff[i].b ? mi : drawbuff[i].b;
      if(mi>=5) {
        getUnsafeLedsArray()[i] = drawbuff[i];
      } else if(mi && mi<5) {
        EffectMath::setLedsNscale8(i, map(mi,1,4,128,10)); // 5 градаций прозрачности, где 0 - полностью прозрачный
      }
    }
  }

  if(isWarning()) {
    warning2Helper(); // вывод предупреждения
  }

  if (isAlarm() || iflags.isStringPrinting) { // isWarning() || 
    doPrintStringToLamp(); // обработчик печати строки
  }

#ifdef VERTGAUGE
  GaugeMix();
#endif

  if (isWarning() || isAlarm() || iflags.isEffectsDisabledUntilText || (effects.worker ? effects.worker->status() : 1) || iflags.isStringPrinting) {
    // выводим кадр только если есть текст или эффект
#ifdef ESP8266
    _effectsTicker.once_ms_scheduled(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
#elif defined ESP32
    _effectsTicker.once_ms(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
#endif
  } else if(isLampOn()) {
    // иначе возвращаемся к началу обсчета следующего кадра
#ifdef ESP8266
    _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
#elif defined ESP32
    _effectsTicker.once_ms(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
#endif
  }
}

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

  FastLED.show();
  if (!_effectsTicker.active() || (!_brt && !isLampOn() && !isAlarm()) ) return;

  // откладываем пересчет эффекта на время для желаемого FPS, либо
  // на минимальный интервал в следующем loop()
  int32_t delay = (ticktime + EFFECTS_RUN_TIMER) - millis();
  if (delay < LED_SHOW_DELAY || !(effects.worker ? effects.worker->status() : 1)) delay = LED_SHOW_DELAY;

#ifdef ESP8266
  _effectsTicker.once_ms_scheduled(delay, std::bind(&LAMP::effectsTick, this));
#elif defined ESP32
  _effectsTicker.once_ms(delay, std::bind(&LAMP::effectsTick, this));
#endif

  ++fps;
}

#ifdef VERTGAUGE
    void LAMP::GaugeShow(unsigned val, unsigned max, byte hue) {
      gauge_time = millis();
      gauge_val = val;
      gauge_max = max;
      gauge_hue = hue;
    }

    void LAMP::GaugeMix() {
      if (gauge_time + 3000 < millis() || millis()<5000) return; // в первые 5 секунд после перезагрузки не показываем :)

#if (VERTGAUGE==1)
      byte ind = (byte)((gauge_val + 1) * HEIGHT / (float)gauge_max + 1);
      for (byte x = 0; x <= xCol * (xStep - 1); x += xStep) {
        for (byte y = 0; y < HEIGHT ; y++) {
          if (ind > y)
            EffectMath::drawPixelXY(x, y, CHSV(gauge_hue, 255, 255));
          else
            EffectMath::drawPixelXY(x, y,  0);
        }
      }
#else
      byte ind = (byte)((gauge_val + 1) * WIDTH / (float)gauge_max + 1);
      for (byte y = 0; y <= yCol * (yStep - 1) ; y += yStep) {
        for (byte x = 0; x < WIDTH ; x++) {
          if (ind > x)
            EffectMath::drawPixelXY((x + y) % WIDTH, y, CHSV(gauge_hue, 255, 255));
          else
            EffectMath::drawPixelXY((x + y) % WIDTH, y,  0);
        }
      }
#endif
    }
#endif

LAMP::LAMP() : docArrMessages(512), tmConfigSaveTime(0), tmStringStepTime(DEFAULT_TEXT_SPEED), tmNewYearMessage(0), _fadeTicker(), _reservedTicker()
#ifdef OTA
    , otaManager((void (*)(CRGB, uint32_t, uint16_t))(&showWarning))
#endif
    , effects(&lampState)
    {
      iflags.isStringPrinting = false; // печатается ли прямо сейчас строка?
      iflags.isEffectsDisabledUntilText = false;
      iflags.isOffAfterText = false;
      iflags.dawnFlag = false; // флаг устанавливается будильником "рассвет"
//#ifdef MIC_EFFECTS
      iflags.isCalibrationRequest = false; // находимся ли в режиме калибровки микрофона
      iflags.micAnalyseDivider = 1; // анализ каждый раз
//#endif

      flags.MIRR_V = false; // отзрекаливание по V
      flags.MIRR_H = false; // отзрекаливание по H
      flags.ONflag = false; // флаг включения/выключения
      flags.isDebug = false; // флаг отладки
      flags.isFaderON = true; // признак того, что используется фейдер для смены эффектов
      flags.isEffClearing = false; // нужно ли очищать эффекты при переходах с одного на другой
      flags.isGlobalBrightness = false; // признак использования глобальной яркости для всех режимов
      flags.isEventsHandled = true;
      flags.isMicOn = true; // глобальное испльзование микрофона
      flags.numInList = false;
      flags.effHasMic = false;
      flags.dRand = false;
      flags.isShowSysMenu = false;
      flags.isOnMP3 = false;
      flags.showName = false;
      flags.playTime = TIME_SOUND_TYPE::TS_NONE; // воспроизводить время?
      flags.playName = false; // воспроизводить имя?
      flags.playEffect = false; // воспроизводить эффект?
      flags.alarmSound = ALARM_SOUND_TYPE::AT_NONE;
      flags.MP3eq = 0;
      flags.playMP3 = false;
      flags.limitAlarmVolume = false;
      flags.isDraw = false;

      _brt =0;
      _steps = 0;
      _brtincrement = 0;

#ifdef VERTGAUGE
      gauge_time = millis();
#endif
      lampState.flags = 0; // сборосить все флаги состояния
      lampState.speedfactor = 1.0; // дефолтное значение
      //lamp_init(); // инициализация и настройка лампы (убрано, будет настройка снаружи)
    }

void LAMP::changePower() {changePower(!flags.ONflag);}

void LAMP::changePower(bool flag) // флаг включения/выключения меняем через один метод
{
  stopAlarm();            // любая активность в интерфейсе - отключаем будильник
  if (flag == flags.ONflag) return;  // пропускаем холостые вызовы
  LOG(print, F("Lamp powering ")); LOG(println, flag ? F("On"): F("Off"));
  flags.ONflag = flag;

  if(mode == LAMPMODE::MODE_OTA)
    mode = LAMPMODE::MODE_NORMAL;

  if (flag){
    effectsTimer(T_ENABLE);
    if(mode == LAMPMODE::MODE_DEMO)
      demoTimer(T_ENABLE);
  } else  {
    fadelight(0, FADE_TIME, std::bind(&LAMP::effectsTimer, this, SCHEDULER::T_DISABLE));  // гасим эффект-процессор
    demoTimer(T_DISABLE);     // гасим Демо-таймер
  }

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, (flags.ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
#endif

  if (curLimit > 0){
    FastLED.setMaxPowerInVoltsAndMilliamps(5, curLimit); // установка максимального тока БП, более чем актуально))). Проверил, без этого куска - ограничение по току не работает :)
  }
}

void LAMP::startAlarm(char *value){
  DynamicJsonDocument doc(1024);
  String buf = value;
  buf.replace("'","\"");
  deserializeJson(doc,buf);
  curAlarm.alarmP = doc.containsKey(FPSTR(TCONST_00BB)) ? doc[FPSTR(TCONST_00BB)] : getAlarmP();
  curAlarm.alarmT = doc.containsKey(FPSTR(TCONST_00BC)) ? doc[FPSTR(TCONST_00BC)] : getAlarmT();
  curAlarm.msg = doc.containsKey(FPSTR(TCONST_0035)) ? doc[FPSTR(TCONST_0035)] : String("");
  curAlarm.isLimitVol = doc.containsKey(FPSTR(TCONST_00D2)) ? doc[FPSTR(TCONST_00D2)].as<String>()=="1" : getLampSettings().limitAlarmVolume;
  curAlarm.isStartSnd = doc.containsKey(FPSTR(TCONST_00D1)) ? doc[FPSTR(TCONST_00D1)].as<String>()=="1" : true;
  curAlarm.type = (ALARM_SOUND_TYPE)(doc.containsKey(FPSTR(TCONST_00D3)) ? doc[FPSTR(TCONST_00D3)].as<uint8_t>() : getLampSettings().alarmSound);

  storedMode = ((mode == LAMPMODE::MODE_ALARMCLOCK) ? storedMode: mode);
  mode = LAMPMODE::MODE_ALARMCLOCK;
  demoTimer(T_DISABLE);     // гасим Демо-таймер
  effectsTimer(T_ENABLE);
#ifdef MP3PLAYER
  if(curAlarm.isStartSnd){
    mp3->setAlarm(true);
    mp3->StartAlarmSound(curAlarm.type); // запуск звука будильника
  }
#endif

#if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
  digitalWrite(ALARM_PIN, ALARM_LEVEL);
#endif

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, MOSFET_LEVEL);
#endif
}

void LAMP::stopAlarm(){
  iflags.dawnFlag = false;
  if (mode != LAMPMODE::MODE_ALARMCLOCK) return;

  myLamp.setBrightness(myLamp.getNormalizedLampBrightness(), false, false);
  mode = (storedMode != LAMPMODE::MODE_ALARMCLOCK ? storedMode : LAMPMODE::MODE_NORMAL); // возвращаем предыдущий режим
#ifdef MP3PLAYER
  mp3->StopAndRestoreVolume(); // восстановить уровень громкости
  mp3->setAlarm(false);
  curAlarm.clear(); // очистить сообщение выводимое на лампу в будильнике
#endif

#if defined(ALARM_PIN) && defined(ALARM_LEVEL)                    // установка сигнала в пин, управляющий будильником
  digitalWrite(ALARM_PIN, !ALARM_LEVEL);
#endif

#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)                  // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, flags.ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
#endif

  LOG(printf_P, PSTR("Отключение будильника рассвет, ONflag=%d\n"), flags.ONflag);
  brightness(getNormalizedLampBrightness());
  if (!flags.ONflag) {
      effectsTimer(T_DISABLE);
      FastLED.clear();
      FastLED.show();
  } else if(mode==LAMPMODE::MODE_DEMO)
    demoTimer(T_ENABLE);     // вернуть демо-таймер
}

/*
 * запускаем режим "ДЕМО"
 */
void LAMP::startDemoMode(byte tmout)
{
  LOG(println,F("Demo mode"));
  demoTimer(T_ENABLE, tmout);
  if(mode == LAMPMODE::MODE_DEMO) return;
  
  storedEffect = ((static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_ENUM::EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  mode = LAMPMODE::MODE_DEMO;
  randomSeed(millis());
  remote_action(RA::RA_DEMO_NEXT, NULL);
  myLamp.sendStringToLamp(String(PSTR("- Demo ON -")).c_str(), CRGB::Green);
}

void LAMP::storeEffect()
{
  storedEffect = ((static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_ENUM::EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  storedBright = getLampBrightness();
  LOG(printf_P, PSTR("Store: %d,%d\n"),storedEffect,storedBright);
}

void LAMP::restoreStored()
{
  LOG(printf_P, PSTR("Restore: %d,%d\n"),storedEffect,storedBright);
  if(storedBright)
    setLampBrightness(storedBright);
  if (static_cast<EFF_ENUM>(storedEffect) != EFF_NONE) {    // ничего не должно происходить, включаемся на текущем :), текущий всегда определен...
    _reservedTicker.once(3,std::bind([this]{ // отсрочка возврата на 3 секунды, чтобы фейдер завершил работу
      remote_action(RA::RA_EFFECT, String(storedEffect).c_str(), NULL);
      //effects.directMoveBy(storedEffect);
    }));
  } else if(static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_NONE) { // если по каким-то причинам текущий пустой, то выбираем рандомный
    _reservedTicker.once(3,std::bind([this]{ // отсрочка возврата на 3 секунды, чтобы фейдер завершил работу
      remote_action(RA::RA_EFF_RAND, NULL);
    }));
  }
}

void LAMP::startNormalMode()
{
  LOG(println,F("Normal mode"));
  mode = LAMPMODE::MODE_NORMAL;
  demoTimer(T_DISABLE);
  restoreStored();
}
#ifdef OTA
void LAMP::startOTAUpdate()
{
  if (mode == LAMPMODE::MODE_OTA) return;
  storedMode = mode;
  mode = LAMPMODE::MODE_OTA;

  effects.directMoveBy(EFF_MATRIX); // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
  FastLED.clear();
  changePower(true);
  sendStringToLamp(String(PSTR("- OTA UPDATE ON -")).c_str(), CRGB::Green);
  otaManager.startOtaUpdate();
}
#endif

typedef enum {FIRSTSYMB=1,LASTSYMB=2} SYMBPOS;

bool LAMP::fillStringManual(const char* text,  const CRGB &letterColor, bool stopText, bool isInverse, int32_t pos, int8_t letSpace, int8_t txtOffset, int8_t letWidth, int8_t letHeight)
{
  static int32_t offset = (flags.MIRR_V ? 0 : WIDTH);
  uint8_t bcount = 0;

  if(pos)
    offset = (flags.MIRR_V ? 0 + pos : WIDTH - pos);

  if (!text || !strlen(text))
  {
    return true;
  }

  uint16_t i = 0, j = 0;
  uint8_t flSymb = SYMBPOS::FIRSTSYMB; // маркер первого символа строки
  while (text[i] != '\0')
  {
    if(text[i+1] == '\0')
      flSymb|=SYMBPOS::LASTSYMB; // маркер последнего символа строки
    if ((uint8_t)text[i] > 191)  // работаем с UTF8 после префикса
    {
      bcount = (uint8_t)text[i]; // кол-во октетов для UTF-8
      i++;
    }
    else
    {
      if(!flags.MIRR_V)
        drawLetter(bcount, text[i], offset + (int16_t)j * (letWidth + letSpace), letterColor, letSpace, txtOffset, isInverse, letWidth, letHeight, flSymb);
      else
        drawLetter(bcount, text[i], offset - (int16_t)j * (letWidth + letSpace), letterColor, letSpace, txtOffset, isInverse, letWidth, letHeight, flSymb);
      i++;
      j++;
      bcount = 0;
      flSymb &= (0xFF^SYMBPOS::FIRSTSYMB); // сбросить маркер первого символа строки
    }
  }

  if(!stopText)
    (flags.MIRR_V ? offset++ : offset--);
  if ((!flags.MIRR_V && offset < (int32_t)(-j * (letWidth + letSpace))) || (flags.MIRR_V && offset > (int32_t)(j * (letWidth + letSpace))+(signed)WIDTH))       // строка убежала
  {
    offset = (flags.MIRR_V ? 0 : WIDTH);
    return true;
  }
  if(pos) // если задана позиция, то считаем что уже отобразили
  {
    offset = (flags.MIRR_V ? 0 : WIDTH);
    return true;
  }

  return false;
}

void LAMP::drawLetter(uint8_t bcount, uint16_t letter, int16_t offset,  const CRGB &letterColor, uint8_t letSpace, int8_t txtOffset, bool isInverse, int8_t letWidth, int8_t letHeight, uint8_t flSymb)
{
  int16_t start_pos = 0, finish_pos = letWidth + letSpace;

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

  if(flSymb){
      if(flSymb&SYMBPOS::FIRSTSYMB){ // битовое &
        start_pos--; // c 0 для самого первого символа
      }
      if(flSymb&SYMBPOS::LASTSYMB && !letSpace){ // битовое &
        finish_pos++; // доп. ряд погашенных символов для последнего символа
      }
  }

  //LOG(printf_P, PSTR("%d %d\n"), start_pos, finish_pos);

  for (int16_t i = start_pos; i < finish_pos; i++)
  {
    uint8_t thisByte;

    if((i<0) || (finish_pos - i <= letSpace) || ((letWidth - 1 - i)<0))
      thisByte = 0x00;
    else
    {
      thisByte = getFont(bcount, letter, i);
    }

    for (uint16_t j = 0; j < letHeight + 1; j++) // +1 доп. пиксель сверху
    {
      bool thisBit = thisByte & (1 << (letHeight - 1 - j));

      // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
      if (offset + i >= 0 && offset + i < (int)WIDTH && txtOffset + j >= 0 && txtOffset + j < (int)HEIGHT) {
        if (thisBit) {
          if(!isInverse)
            EffectMath::drawPixelXY(offset + i, txtOffset + j, letterColor);
          else
            //EffectMath::setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), (isWarning() && iflags.warnType==2) ? 0 : (isWarning() && iflags.warnType==1) ? 255 : getBFade());
            EffectMath::getPixel(offset + i, txtOffset + j).fadeToBlackBy((isWarning() && iflags.warnType==2) ? 0 : (isWarning() && iflags.warnType==1) ? 255 : getBFade());
        } else {
          if(isInverse)
            EffectMath::drawPixelXY(offset + i, txtOffset + j, letterColor);
          else
            //EffectMath::setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), (isWarning() && iflags.warnType==2) ? 0 : (isWarning() && iflags.warnType==1) ? 255 : getBFade());
            EffectMath::getPixel(offset + i, txtOffset + j).fadeToBlackBy((isWarning() && iflags.warnType==2) ? 0 : (isWarning() && iflags.warnType==1) ? 255 : getBFade());
        }
      }
    }
  }
}

uint8_t LAMP::getFont(uint8_t bcount, uint8_t asciiCode, uint8_t row)       // интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
{
  asciiCode = asciiCode - '0' + 16;                         // перевод код символа из таблицы ASCII в номер согласно нумерации массива

  // if (asciiCode <= 90)                                      // печатаемые символы и английские буквы
  // {
  //   return pgm_read_byte(&fontHEX[asciiCode][row]);
  // }
  // else if (asciiCode >= 112 && asciiCode <= 159)
  // {
  //   return pgm_read_byte(&fontHEX[asciiCode - 17][row]);
  // }
  // else if (asciiCode >= 96 && asciiCode <= 111)
  // {
  //   return pgm_read_byte(&fontHEX[asciiCode + 47][row]);
  // }

  if (asciiCode <= 94) {
    return pgm_read_byte(&(fontHEX[asciiCode][row]));     // для английских букв и символов
  } else if ((bcount == 209) && asciiCode == 116) {         // є
    return pgm_read_byte(&(fontHEX[162][row])); 
  } else if ((bcount == 209) && asciiCode == 118) {        // і
    return pgm_read_byte(&(fontHEX[73][row])); 
  } else if ((bcount == 209) && asciiCode == 119) {         // ї
    return pgm_read_byte(&(fontHEX[163][row])); 
  } else if ((bcount == 208) && asciiCode == 100) {        // Є
    return pgm_read_byte(&(fontHEX[160][row])); 
  } else if ((bcount == 208) && asciiCode == 102) {         // І
    return pgm_read_byte(&(fontHEX[41][row])); 
  } else if ((bcount == 208) && asciiCode == 103) {        // Ї
    return pgm_read_byte(&(fontHEX[161][row])); 
  } else if ((bcount == 208) && asciiCode == 97) {         // Ё
    return pgm_read_byte(&(fontHEX[100][row])); 
  } else if ((bcount == 209) && asciiCode == 113) {        // ё
    return pgm_read_byte(&(fontHEX[132][row])); 
  } else if ((bcount == 208 || bcount == 209) && asciiCode >= 112 && asciiCode <= 159) {      // русские символы
    return pgm_read_byte(&(fontHEX[asciiCode - 17][row]));
  } else if ((bcount == 208 || bcount == 209) && asciiCode >= 96 && asciiCode <= 111) {
    return pgm_read_byte(&(fontHEX[asciiCode + 47][row]));
  } else if ((bcount == 194) && asciiCode == 144) {                                          // Знак градуса '°'
    return pgm_read_byte(&(fontHEX[159][row]));
  }

  return 0;
}

void LAMP::sendString(const char* text, const CRGB &letterColor){
  if (!isLampOn()){
      disableEffectsUntilText(); // будем выводить текст, при выкюченной матрице
      setOffAfterText();
      changePower(true);
      setBrightness(2, false, false); // выводить будем минимальной яркостью в 2 пункта
      sendStringToLamp(text, letterColor, true);
  } else {
      sendStringToLamp(text, letterColor);
  }
}

String &LAMP::prepareText(String &source){
  source.replace(F("%TM"), embui.timeProcessor.getFormattedShortTime());
  source.replace(F("%IP"), WiFi.localIP().toString());
  source.replace(F("%EN"), effects.getEffectName());
  const tm *tm = localtime(embui.timeProcessor.now());
  char buffer[11]; //"xx.xx.xxxx"
  sprintf_P(buffer,PSTR("%02d.%02d.%04d"),tm->tm_mday,tm->tm_mon+1,tm->tm_year+TM_BASE_YEAR);
  source.replace(F("%DT"), buffer);
#ifdef LAMP_DEBUG  
  if(!source.isEmpty() && effects.getCurrent()!=EFF_ENUM::EFF_TIME) // спам эффекта часы убираем костыльным способом :)
    LOG(println, source.c_str()); // вывести в лог строку, которая после преобразований получилась
#endif
  return source;  
}

void LAMP::sendStringToLamp(const char* text, const CRGB &letterColor, bool forcePrint, int8_t textOffset, int16_t fixedPos)
{
  if((!flags.ONflag && !forcePrint) || (isAlarm() && !forcePrint)) return; // если выключена, или если будильник, но не задан принудительный вывод - то на выход
  if(textOffset==-128) textOffset=this->txtOffset;

  if(text==nullptr){ // текст пустой
    if(!iflags.isStringPrinting){ // ничего сейчас не печатается
      if(docArrMessages.isNull()){ // массив пустой
        return; // на выход
      }
      else { // есть что печатать
        JsonArray arr = docArrMessages.as<JsonArray>(); // используем имеющийся
        JsonObject var=arr[0]; // извлекаем очередной
        if(!var.isNull()){
          String storage = var[F("s")];
          prepareText(storage);
          doPrintStringToLamp(storage.c_str(), (var[F("c")].as<unsigned long>()), (var[F("o")].as<int>()), (var[F("f")].as<int>())); // отправляем
#ifdef MP3PLAYER
          String tmpStr = var[F("s")];
          if(mp3!=nullptr && ((mp3->isOn() && isLampOn()) || isAlarm()) && flags.playTime && tmpStr.indexOf(String(F("%TM")))>=0)
            mp3->playTime(embui.timeProcessor.getHours(), embui.timeProcessor.getMinutes(), (TIME_SOUND_TYPE)flags.playTime);
#endif
        }
        if(arr.size()>0)
          arr.remove(0); // удаляем отправленный
      }
    } else {
        // текст на входе пустой, идет печать
        return; // на выход
    }
  } else { // текст не пустой
    if(!iflags.isStringPrinting){ // ничего сейчас не печатается
      String storage = text;
      prepareText(storage);
      doPrintStringToLamp(storage.c_str(), letterColor, textOffset, fixedPos); // отправляем
#ifdef MP3PLAYER
      String tmpStr = text;
      if(mp3!=nullptr && ((mp3->isOn() && isLampOn()) || isAlarm()) && flags.playTime && tmpStr.indexOf(String(F("%TM")))>=0)
        mp3->playTime(embui.timeProcessor.getHours(), embui.timeProcessor.getMinutes(), (TIME_SOUND_TYPE)flags.playTime);
#endif
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

  iflags.isStringPrinting = true;
  int8_t offs=(textOffset==-128?txtOffset:textOffset);

  if(text!=nullptr && text[0]!='\0'){
    toPrint.concat(text);
    _letterColor = letterColor;
  }

  if(toPrint.length()==0) {
    iflags.isStringPrinting = false;
    return; // нечего печатать
  } else {
    iflags.isStringPrinting = true;
  }

  if(tmStringStepTime.isReadyManual()){
    if(!fillStringManual(toPrint.c_str(), _letterColor, false, isAlarm() || (isWarning() && iflags.warnType<2), fixedPos, (fixedPos? 0 : LET_SPACE), offs) && (!isWarning() || (isWarning() && fixedPos))){ // смещаем
      tmStringStepTime.reset();
    }
    else {
      iflags.isStringPrinting = false;
      toPrint.clear(); // все напечатали
      sendStringToLamp(); // получаем новую порцию
    }
  } else {
    if((!isWarning() || (isWarning() && fixedPos)))
      fillStringManual(toPrint.c_str(), _letterColor, true, isAlarm() || (isWarning() && iflags.warnType<2), fixedPos, (fixedPos? 0 : LET_SPACE), offs);
  }
}

void LAMP::newYearMessageHandle()
{
  if(!tmNewYearMessage.isReady() || embui.timeProcessor.isDirtyTime())
    return;

  {
    char strMessage[256]; // буфер
    time_t calc = NEWYEAR_UNIXDATETIME - embui.timeProcessor.getUnixTime();

    if(calc<0) {
      sprintf_P(strMessage, NY_MDG_STRING2, localtime(embui.timeProcessor.now())->tm_year+TM_BASE_YEAR);
    } else if(calc<300){
      sprintf_P(strMessage, NY_MDG_STRING1, (int)calc, String(FPSTR(TINTF_0C1)).c_str());
    } else if(calc/60<60){
      uint16_t calcT=calc/(60*60); // минуты
      byte calcN=calcT%10; // остаток от деления на 10
      String str;
      if(calcN>=2 && calcN<=4) {
        str = FPSTR(TINTF_0CC); // минуты
      } else if(calcN==1) {
        str = FPSTR(TINTF_0CD); // минута
      } else {
        str = FPSTR(TINTF_0C2); // минут
      }
      sprintf_P(strMessage, NY_MDG_STRING1, calcT, str.c_str());
    } else if(calc/(60*60)<60){
	    uint16_t calcT=calc/(60*60); // часы
      byte calcN=calcT%10; // остаток от деления на 10
      String str;
      if(calcN>=2 && calcN<=4) {
        str = FPSTR(TINTF_0C7); // часа
      } else if(calcN==1) {
        str = FPSTR(TINTF_0C8); // час
      } else {
        str = FPSTR(TINTF_0C3); // часов
      }
      sprintf_P(strMessage, NY_MDG_STRING1, calcT, str.c_str());
    } else {
      uint16_t calcT=calc/(60*60*24); // дни
      byte calcN=calcT%10; // остаток от деления на 10
      String str;
      if(calcT>=11 && calcT<=20)
        str = FPSTR(TINTF_0C4);
      else if(calcN>=2 && calcN<=4)
        str = FPSTR(TINTF_0C5);
      else if(calc!=11 && calcN==1)
        str = FPSTR(TINTF_0C6);
      else
        str = FPSTR(TINTF_0C4);
      sprintf_P(strMessage, NY_MDG_STRING1, calcT, str.c_str());
    }

    LOG(printf_P, PSTR("Prepared message: %s\n"), strMessage);
    sendStringToLamp(strMessage, LETTER_COLOR);
  }
}


void LAMP::periodicTimeHandle()
{
  const tm* t = localtime(embui.timeProcessor.now());
  if(t->tm_sec)
    return;

  LOG(printf_P, PSTR("periodicTimeHandle: %02d:%02d:%02d\n"), t->tm_hour,t->tm_min,t->tm_sec);

  time_t tm = t->tm_hour * 60 + t->tm_min;
  String time = String(F("%TM"));

  CRGB color;
  if(!(tm%60)){
    color = CRGB::Red;
  } else if(!(tm%30)){
    color = CRGB::Green;
  } else {
    color =  CRGB::Blue;
  }
  sendStringToLamp(time.c_str(), color);
}

#ifdef MIC_EFFECTS
void LAMP::micHandler()
{
  static uint8_t counter=0;
  if(effects.getEn()==EFF_ENUM::EFF_NONE)
    return;
  if(mw==nullptr && !iflags.isCalibrationRequest){ // обычный режим
    mw = new MICWORKER(mic_scale,mic_noise);
    if(!mw) {
      mw=nullptr;
      return; // не удалось выделить память, на выход
    }
    //delete mw; mw = nullptr; return;
    
    samp_freq = mw->process(noise_reduce); // возвращаемое значение - частота семплирования
    last_min_peak = mw->getMinPeak();
    last_max_peak = mw->getMaxPeak();

    if(!counter) // раз на N измерений берем частоту, т.к. это требует обсчетов
      last_freq = mw->analyse(); // возвращаемое значение - частота главной гармоники
    if(iflags.micAnalyseDivider)
      counter = (counter+1)%(0x01<<(iflags.micAnalyseDivider-1)); // как часто выполнять анализ
    else
      counter = 1; // при micAnalyseDivider == 0 - отключено

    // EVERY_N_SECONDS(1){
    //   LOG(println, counter);
    // }

    //LOG(println, last_freq);
    //mw->debug();
    delete mw;
    mw = nullptr;
  } else if(iflags.isCalibrationRequest) {
    if(mw==nullptr){ // калибровка начало
      mw = new MICWORKER();
      mw->calibrate();
    } else { // калибровка продолжение
      mw->calibrate();
    }
    if(!mw->isCaliblation()){ // калибровка конец
      mic_noise = mw->getNoise();
      mic_scale = mw->getScale();
      iflags.isCalibrationRequest = false; // завершили
      delete mw;
      mw = nullptr;

      remote_action(RA::RA_MIC, NULL);
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
#ifdef ESP8266
        if (callback != nullptr) _fadeTicker.once_ms_scheduled(0, callback);
#elif defined ESP32
        if (callback != nullptr) _fadeTicker.once_ms(0, callback);
#endif
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
#ifdef ESP8266
        _fadeTicker.once_ms_scheduled(0, callback);
#elif defined ESP32
        _fadeTicker.once_ms(0, callback);
#endif
      } else { _fadeTicker.detach(); }
      _brt = _tgtbrt;
  } else {
      _brt += _brtincrement;
  }

  brightness(_brt);
}

/*
 * переключатель эффектов для других методов,
 * может использовать фейдер, выбирать случайный эффект для демо
 * @param EFFSWITCH action - вид переключения (пред, след, случ.)
 * @param fade - переключаться через фейдер или сразу
 */
void LAMP::switcheffect(EFFSWITCH action, bool fade, uint16_t effnb, bool skip) {
  LOG(printf_P, PSTR("EFFSWITCH=%d, fade=%d, effnb=%d\n"), action, fade, effnb);

#ifdef MIC_EFFECTS
    setMicAnalyseDivider(1); // восстановить делитель, при любой активности (поскольку эффекты могут его перенастраивать под себя)
#endif

  if (!skip) {
    switch (action) {
    case EFFSWITCH::SW_NEXT :
        effects.setSelected(effects.getNext());
        break;
    case EFFSWITCH::SW_NEXT_DEMO :
        effects.setSelected(effects.getByCnt(1));
        break;
    case EFFSWITCH::SW_PREV :
        effects.setSelected(effects.getPrev());
        break;
    case EFFSWITCH::SW_SPECIFIC :
        effects.setSelected(effects.getBy(effnb));
        break;
    case EFFSWITCH::SW_RND :
        effects.setSelected(effects.getByCnt(random(0, effects.getModeAmount())));
        break;
    case EFFSWITCH::SW_WHITE_HI:
        storeEffect();
        effects.setSelected(effects.getBy(EFF_WHITE_COLOR));
        myLamp.setMode(LAMPMODE::MODE_WHITELAMP);
        break;
    case EFFSWITCH::SW_WHITE_LO:
        storeEffect();
        effects.setSelected(effects.getBy(EFF_WHITE_COLOR));
        myLamp.setMode(LAMPMODE::MODE_WHITELAMP);
        break;
    default:
        return;
    }
    // тухнем "вниз" только на включенной лампе
    if (fade && flags.ONflag) {
      fadelight(FADE_MINCHANGEBRT, FADE_TIME, std::bind(&LAMP::switcheffect, this, action, fade, effnb, true));
      return;
    }
  }

  //LOG(printf_P,PSTR(">>>>>>>>>>>isEffClearing==%d\n"),isEffClearing);
  if(flags.isEffClearing || !effects.getEn()){ // для EFF_NONE или для случая когда включена опция - чистим матрицу
    FastLED.clear();
    FastLED.show();
  }

  effects.moveSelected();

  bool isShowName = (mode==LAMPMODE::MODE_DEMO && flags.showName);
#ifdef MP3PLAYER
  bool isPlayName = (isShowName && flags.playName && !flags.playMP3 && effects.getEn()>0);
#endif
  if(isShowName){
    myLamp.sendStringToLamp(String(F("%EN")).c_str(), CRGB::Green);
#ifdef MP3PLAYER
    if(isPlayName && mp3!=nullptr && mp3->isOn()) // воспроизведение 
      mp3->playName(effects.getEn());
#endif
  }

#ifdef MP3PLAYER
  if(mp3!=nullptr && mp3->isOn() && effects.getEn()>0 && (flags.playEffect || ((isLampOn() || millis()>5000) && flags.playMP3 && action!=EFFSWITCH::SW_NEXT_DEMO && action!=EFFSWITCH::SW_RND))){
    LOG(printf_P, PSTR("playEffect soundfile:%s, effect:%d, delayed:%d\n"), effects.getSoundfile().c_str(), effects.getEn(), (flags.playName && !flags.playMP3));
    if(!flags.playMP3 || (flags.playEffect && action!=EFFSWITCH::SW_NEXT_DEMO && action!=EFFSWITCH::SW_RND)) // для mp3-плеера есть отдельное управление
      mp3->playEffect(effects.getEn(), effects.getSoundfile(), (isPlayName && mp3!=nullptr && mp3->isOn() && !flags.playMP3)); // влияние на отложенное воспроизведение, но не для MP3-плеера
  } else {
    mp3->setCurEffect(effects.getEn());
  }
#endif

  bool natural = true;
  switch (action) {
  case EFFSWITCH::SW_WHITE_HI:
      setLampBrightness(255); // здесь яркость ползунка в UI, т.е. ставим 255 в самое крайнее положение, а дальше уже будет браться приведенная к BRIGHTNESS яркость
      fade = natural = false;
      changePower(true);  // принудительно включаем лампу
      break;
  case EFFSWITCH::SW_WHITE_LO:
      setLampBrightness(1); // здесь яркость ползунка в UI, т.е. ставим 1 в самое крайнее положение, а дальше уже будет браться приведенная к BRIGHTNESS яркость
      fade = natural = false;
      changePower(true);  // принудительно включаем лампу
      break;
  default:;
  }

  // отрисовать текущий эффект (только если лампа включена, иначе бессмысленно)
  if(effects.worker && flags.ONflag && !iflags.isEffectsDisabledUntilText){
    effects.worker->run(getUnsafeLedsArray(), &effects);
    ledsbuff.resize(NUM_LEDS);
    std::copy(getUnsafeLedsArray(), getUnsafeLedsArray() + NUM_LEDS, ledsbuff.begin());
  }
  setBrightness(getNormalizedLampBrightness(), fade, natural);
}

/*
 * включает/выключает режим "демо"
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
void LAMP::demoTimer(SCHEDULER action, byte tmout){
//  LOG.printf_P(PSTR("demoTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    _demoTicker.detach();
    break;
  case SCHEDULER::T_ENABLE :
#ifdef ESP8266
    _demoTicker.attach_scheduled(tmout, std::bind(&remote_action, RA::RA_DEMO_NEXT, NULL));
#elif defined ESP32
    _demoTicker.attach(tmout, std::bind(&remote_action, RA::RA_DEMO_NEXT, NULL));
#endif
    break;
  case SCHEDULER::T_RESET :
    if (isAlarm()) stopAlarm(); // тут же сбросим и будильник
    if (_demoTicker.active() ) demoTimer(T_ENABLE);
    break;
  default:
    return;
  }
}

/*
 * включает/выключает таймер обработки эффектов
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
void LAMP::effectsTimer(SCHEDULER action) {
//  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    _effectsTicker.detach();
    break;
  case SCHEDULER::T_ENABLE :
#ifdef ESP8266
    _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
#elif defined ESP32
    _effectsTicker.once_ms(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
#endif
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
  const CRGB &color,                                        /* цвет вспышки                                                 */
  uint32_t duration,                                        /* продолжительность отображения предупреждения (общее время)   */
  uint16_t blinkHalfPeriod)                                 /* продолжительность одной вспышки в миллисекундах (полупериод) */
{
  uint32_t blinkTimer = millis();
  enum BlinkState { OFF = 0, ON = 1 } blinkState = BlinkState::OFF;
  myLamp.fadelight(myLamp.getLampBrightness());    // установка яркости для предупреждения
  FastLED.clear();
  delay(2);
  FastLED.show();

  EffectMath::fillAll(color);                               // установка цвета всех диодов в WARNING_COLOR

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
// ------------- мигающий цвет (не эффект! используется для отображения краткосрочного предупреждения; неблокирующий код, рисует поверх эффекта!) -------------

void LAMP::warning2Helper(){
  if(iflags.isWarning) {
    switch(iflags.warnType){
      case 0: EffectMath::fillAll(warn_color); break;
      case 1: {
        uint16_t cnt = warn_duration/(warn_blinkHalfPeriod*2);
        uint8_t xPos = (WIDTH+LET_WIDTH*(cnt>99?3:cnt>9?2:1))/2;
        EffectMath::fillAll(warn_color);
        if (!myLamp.isPrintingNow())
          myLamp.sendStringToLamp(String(cnt).c_str(), warn_color, true, -128, xPos);
        break;
      }
      case 2: {
        uint16_t cnt = warn_duration/(warn_blinkHalfPeriod*2);
        uint8_t xPos = (WIDTH+LET_WIDTH*(cnt>99?3:cnt>9?2:1))/2;
        EffectMath::fillAll(warn_color);
        if (!myLamp.isPrintingNow())
          myLamp.sendStringToLamp(String(cnt).c_str(), -warn_color, true, -128, xPos);
        break;
      }
      case 3: {
        uint16_t cnt = warn_duration/(warn_blinkHalfPeriod*2);
        uint8_t xPos = (WIDTH+LET_WIDTH*(cnt>99?3:cnt>9?2:1))/2;
        if (!myLamp.isPrintingNow())
          myLamp.sendStringToLamp(String(cnt).c_str(), warn_color, true, -128, xPos);
        break;
      }
      default: break;
    }
  }
}

void LAMP::showWarning2(
  const CRGB &color,                                        /* цвет вспышки                                                 */
  uint32_t duration,                                        /* продолжительность отображения предупреждения (общее время)   */
  uint16_t blinkHalfPeriod,                                 /* продолжительность одной вспышки в миллисекундах (полупериод) */
  uint8_t warnType,                                         /* тип предупреждения 0...3                                     */
  bool forcerestart)                                        /* перезапускать, если пришло повторное событие предупреждения  */
{
  if(forcerestart || !_warningTicker.active()){
    warn_color = color;
    warn_duration = duration;
    warn_blinkHalfPeriod = blinkHalfPeriod;
    iflags.isWarning = true;
    iflags.warnType = warnType;
  }

  if(!forcerestart && warnType!=3)
    iflags.isWarning=!iflags.isWarning;
  if(warn_duration>warn_blinkHalfPeriod)
    warn_duration-=warn_blinkHalfPeriod;
  else
    warn_duration=0;
  if(warn_duration){
    if(_warningTicker.active())
      _warningTicker.detach();
#ifdef ESP8266
    _warningTicker.once_ms_scheduled(blinkHalfPeriod, std::bind(&LAMP::showWarning2, this, warn_color, warn_duration, warn_blinkHalfPeriod, (uint8_t)iflags.warnType, !iflags.isWarning));
#elif defined ESP32
    _warningTicker.once_ms(blinkHalfPeriod, std::bind(&LAMP::showWarning2, this, warn_color, warn_duration, warn_blinkHalfPeriod, (uint8_t)iflags.warnType, !iflags.isWarning));
#endif

  }
  else {
    iflags.isWarning = false;
    _warningTicker.detach();
  }
}