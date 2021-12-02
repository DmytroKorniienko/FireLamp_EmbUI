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
#include "fontHEX.h"

extern LAMP myLamp; // Объект лампы

void LAMP::lamp_init(const uint16_t curlimit)
{
  setcurLimit(curlimit);

  //FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalPixelString);
  FastLED.addLeds<WS2812B, LAMP_PIN, COLOR_ORDER>(getUnsafeLedsArray(), NUM_LEDS);

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

  // все что ниже, будет выполняться раз в 0.999 секундy
  static unsigned long wait_handlers;
  if (wait_handlers + 999U > millis())
      return;
  wait_handlers = millis();

  EVERY_N_SECONDS(15){
    lampState.freeHeap = ESP.getFreeHeap();
#ifdef ESP8266
    lampState.HeapFragmentation = ESP.getHeapFragmentation();
#else
    lampState.HeapFragmentation = 0;
#endif
    lampState.rssi = WiFi.RSSI();

#ifdef LAMP_DEBUG
    // fps counter
    LOG(printf_P, PSTR("Eff:%d, FPS: %u, FastLED FPS: %u\n"), effects.getEn(), avgfps, FastLED.getFPS());
#ifdef ESP8266
    LOG(printf_P, PSTR("MEM stat: %d, HF: %d, Time: %s\n"), lampState.freeHeap, lampState.HeapFragmentation, embui.timeProcessor.getFormattedShortTime().c_str());
#else
    LOG(printf_P, PSTR("MEM stat: %d, Time: %s\n"), lampState.freeHeap, embui.timeProcessor.getFormattedShortTime().c_str());
#endif
#endif
  }
#ifdef LAMP_DEBUG
  avgfps = (avgfps+fps) / 2;
#endif
  fps = 0; // сброс FPS раз в секунду


  // будильник обрабатываем раз в секунду
  alarmWorker();

  if(lampState.isEffectsDisabledUntilText && !lampState.isStringPrinting) {
    setBrightness(0,false,false); // напечатали, можно гасить матрицу :)
    lampState.isEffectsDisabledUntilText = false;
  }

  // отложенное включение/выключение
  if(lampState.isOffAfterText && !lampState.isStringPrinting) {
    changePower(false);
    remote_action(RA::RA_OFF, NULL);
  }

  newYearMessageHandle();
  //ConfigSaveCheck(); // для взведенного таймера автосохранения настроек

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
      lampState.dawnFlag = false;
      return;
    }

    // проверка рассвета, первый вход в функцию
    if (!lampState.dawnFlag){
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
      //LOG(println, dawnPosition);
      if(curAlarm.isStartSnd)
        mp3->setTempVolume(map(dawnPosition,0,255,1,(curAlarm.isLimitVol ? mp3->getVolume() : 30))); // наростание громкости
      else if(dawnPosition==255 && !curAlarm.isStartSnd && !mp3->isAlarm()){
        mp3->setAlarm(true);
        mp3->StartAlarmSoundAtVol(curAlarm.type, mp3->getVolume()); // запуск звука будильника
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
        if(!curAlarm.msg.isEmpty() && curAlarm.msg != "-") {
            sendStringToLamp(curAlarm.msg.c_str(), letterColor, true);
        } else {
#ifdef PRINT_ALARM_TIME
#ifdef MP3PLAYER
          if(mp3->isAlarm()) // если отложенный звук будильника, то время тоже не выводим, т.к. может быть включено озвучивание
#endif
            if(curAlarm.msg != "-") // отключение вывода по спец. символу "минус"
              sendStringToLamp(String(F("%TM")).c_str(), letterColor, true);
#endif
        }
      }
    }

    for (uint16_t i = 0U; i < NUM_LEDS; i++) {
        getUnsafeLedsArray()[i] = dawnColorMinus[i%(sizeof(dawnColorMinus)/sizeof(CHSV))];
    }
    lampState.dawnFlag = true;
}

void LAMP::effectsTick(){
  uint32_t _begin = millis();

  if (effects.worker && (flags.ONflag || fader) && !isAlarm()) {
    if(!lampState.isEffectsDisabledUntilText){
      if (!ledsbuff.empty()) {
        std::copy( ledsbuff.begin(), ledsbuff.end(), getUnsafeLedsArray() );
      }
      // посчитать текущий эффект (сохранить кадр в буфер, если ОК)
      if(effects.worker ? effects.worker->run(getUnsafeLedsArray(), &effects) : 1) {
        ledsbuff.resize(NUM_LEDS);
        std::copy(getUnsafeLedsArray(), getUnsafeLedsArray() + NUM_LEDS, ledsbuff.begin());
      }
    }
  }
#if defined(USE_STREAMING) && defined(EXT_STREAM_BUFFER)
    if(!streambuff.empty()){
    uint8_t mi;
    for(uint16_t i=0; i<streambuff.size() && i<NUM_LEDS; i++){
      mi = streambuff[i].r > streambuff[i].g ? streambuff[i].r : streambuff[i].g;
      mi = mi > streambuff[i].b ? mi : streambuff[i].b;
      if(mi>=5) {
        getUnsafeLedsArray()[i] = streambuff[i];
      } else if(mi && mi<5) {
        EffectMath::setLedsNscale8(i, map(mi,1,4,128,10)); // 5 градаций прозрачности, где 0 - полностью прозрачный
      }
    }
  }
#endif
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
    warningHelper(); // вывод предупреждения
  }

  if (isAlarm() || lampState.isStringPrinting) { // isWarning() || 
    doPrintStringToLamp(); // обработчик печати строки
  }

#ifdef VERTGAUGE
  GaugeMix();
#endif

  if (isWarning() || isAlarm() || lampState.isEffectsDisabledUntilText || fader || (effects.worker ? effects.worker->status() : 1) || lampState.isStringPrinting) {
    // выводим кадр только если есть текст или эффект
    effectsTimer(T_FRAME_ENABLE, _begin);

  } else if(isLampOn()) {
    // иначе возвращаемся к началу обсчета следующего кадра
    effectsTimer(T_ENABLE);
  }
  
  if(!lampState.isStringPrinting && !flags.ONflag && !fader){ // чистить буфер только если не выводится строка, иначе держать его
    ledsbuff.resize(0);
    ledsbuff.shrink_to_fit();
  }
}

/*
 * вывод готового кадра на матрицу,
 * и перезапуск эффект-процессора
 */
void LAMP::frameShow(const uint32_t ticktime){
  if ( !fader && !isLampOn() && !isAlarm() ) return;

  FastLED.show();

  // откладываем пересчет эффекта на время для желаемого FPS, либо
  // на минимальный интервал в следующем loop()
  int32_t delay = (ticktime + EFFECTS_RUN_TIMER) - millis();
  if (delay < LED_SHOW_DELAY || !(effects.worker ? effects.worker->status() : 1)) delay = LED_SHOW_DELAY;

  effectsTimer(T_ENABLE, delay);
  ++fps;
}

#ifdef VERTGAUGE
    void LAMP::GaugeShow(unsigned val, unsigned max, uint8_t hue) {
      gauge_time = millis();
      gauge_val = val;
      gauge_max = max;
      gauge_hue = hue;
    }

    void LAMP::GaugeMix() {
      if (gauge_time + 3000 < millis() || millis()<5000) return; // в первые 5 секунд после перезагрузки не показываем :)

#if (VERTGAUGE==1)
/*      uint8_t ind = (uint8_t)((gauge_val + 1) * HEIGHT / (float)gauge_max + 1);
      for (uint8_t x = 0; x <= xCol * (xStep - 1); x += xStep) {
        for (uint8_t y = 0; y < HEIGHT ; y++) {
          if (ind > y)
            EffectMath::drawPixelXY(x, y, CHSV(gauge_hue, 255, 255));
          else
            EffectMath::drawPixelXY(x, y,  0);
        }
      }*/
      for (uint8_t x = 0; x <= xCol * (xStep - 1); x += xStep) {
        EffectMath::drawLine(x, 0, x, HEIGHT, 0);
        EffectMath::drawLineF(x, 0, x, EffectMath::fmap(gauge_val, 0, gauge_max, 0, HEIGHT), (gauge_hue ? CHSV(gauge_hue, 255, 255) : CRGB(gauge_color)));
      }
#else
      uint8_t ind = (uint8_t)((gauge_val + 1) * WIDTH / (float)gauge_max + 1);
      for (uint8_t y = 0; y <= yCol * (yStep - 1) ; y += yStep) {
        for (uint8_t x = 0; x < WIDTH ; x++) {
          if (ind > x)
            EffectMath::drawPixelXY((x + y) % WIDTH, y, CHSV(gauge_hue, 255, 255));
          else
            EffectMath::drawPixelXY((x + y) % WIDTH, y,  0);
        }
      }
#endif
    }
#endif

LAMP::LAMP() : tmStringStepTime(DEFAULT_TEXT_SPEED), tmNewYearMessage(0)
#ifdef OTA
    , otaManager()
#endif
    , effects(&lampState)
    {
      lampState.isOptPass = false; // введен ли пароль для опций
      lampState.isInitCompleted = false; // завершилась ли инициализация лампы
      lampState.isStringPrinting = false; // печатается ли прямо сейчас строка?
      lampState.isEffectsDisabledUntilText = false;
      lampState.isOffAfterText = false;
      lampState.dawnFlag = false; // флаг устанавливается будильником "рассвет"
//#ifdef MIC_EFFECTS
      lampState.isCalibrationRequest = false; // находимся ли в режиме калибровки микрофона
      lampState.micAnalyseDivider = 1; // анализ каждый раз
//#endif

#ifdef VERTGAUGE
      gauge_time = millis();
#endif
      lampState.flags = 0; // сборосить все флаги состояния
      lampState.speedfactor = 1.0; // дефолтное значение
      lampState.brightness = 127;
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
#ifdef USE_STREAMING
    if (flags.isStream)
      Led_Stream::newStreamObj((STREAM_TYPE)embui.param(FPSTR(TCONST_0047)).toInt());
    if(!flags.isDirect || !flags.isStream)
#endif
    effectsTimer(T_ENABLE);
    if(mode == LAMPMODE::MODE_DEMO)
      demoTimer(T_ENABLE);
  } else  {
#ifdef USE_STREAMING
    Led_Stream::clearStreamObj();
#endif
    if(flags.isFaderON && !lampState.isOffAfterText)
      fadelight(this, 0, FADE_TIME, std::bind(&LAMP::effectsTimer, this, SCHEDULER::T_DISABLE, 0));  // гасим эффект-процессор
    else {
      brightness(0);
      effectsTimer(SCHEDULER::T_DISABLE);
    }
    lampState.isOffAfterText = false;
    lampState.isStringPrinting = false;
    demoTimer(T_DISABLE);     // гасим Демо-таймер
  }

  Task *_t = new Task(flags.isFaderON && !flags.ONflag ? 5*TASK_SECOND : 50, TASK_ONCE, // для выключения - отложенное переключение мосфета 5 секунд
    [this](){
#if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, (flags.ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
#endif
    TASK_RECYCLE; },
    &ts, false);
  _t->enableDelayed();

// #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
//   digitalWrite(MOSFET_PIN, (flags.ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL));
// #endif

#ifdef DS18B20
#ifdef SHOWSYSCONFIG
    // восстанавливаем значение тока после включения. Так как значение 0 не работает в ограничителе тока по перегреву, 
    // то если ограничение тока установлено в 0, устанвливаем вместо него рассчетный максимум в 15.36А на 256 диодов (бред конечно, но нужно же хоть какое-то значение больше 0).
    setcurLimit(embui.param(FPSTR(TCONST_0098)).toInt() == 0 ? (NUM_LEDS * 60) : embui.param(FPSTR(TCONST_0098)).toInt());
#else
    setcurLimit(CURRENT_LIMIT == 0U ? (NUM_LEDS * 60) : CURRENT_LIMIT);
#endif
#endif
    FastLED.setMaxPowerInVoltsAndMilliamps(5, curLimit); // установка максимального тока БП, более чем актуально))). Проверил, без этого куска - ограничение по току не работает :)
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
#ifdef USE_STREAMING
  if(!flags.isDirect || !flags.isStream)
#endif
  effectsTimer(T_ENABLE);
#ifdef MP3PLAYER
  if(curAlarm.isStartSnd){
    mp3->setAlarm(true);
    mp3->StartAlarmSoundAtVol(curAlarm.type, 1); // запуск звука будильника c минимальной громкости
  } else {
    mp3->setAlarm(false); // здесь будет стоп музыки
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
  lampState.dawnFlag = false;
  if (mode != LAMPMODE::MODE_ALARMCLOCK) return;

  setBrightness(getNormalizedLampBrightness(), false, false);
  mode = (storedMode != LAMPMODE::MODE_ALARMCLOCK ? storedMode : LAMPMODE::MODE_NORMAL); // возвращаем предыдущий режим
#ifdef MP3PLAYER
  mp3->setAlarm(false);
  Task *_t = new Task(300, TASK_ONCE, nullptr, &ts, false, nullptr, [this](){
    mp3->RestoreVolume(); // восстановить уровень громкости
    TASK_RECYCLE;
  });
  _t->enableDelayed();
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
void LAMP::startDemoMode(uint8_t tmout)
{
  LOG(println,F("Demo mode"));
  if(mode == LAMPMODE::MODE_DEMO) return;
  
  storedEffect = ((static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_ENUM::EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  mode = LAMPMODE::MODE_DEMO;
  if(isLampOn()){
    randomSeed(millis());
    remote_action(RA::RA_DEMO_NEXT, NULL);
    demoTimer(T_ENABLE, tmout);
  }
  sendString(String(PSTR("- Demo ON -")).c_str(), CRGB::Green, false);
}

void LAMP::storeEffect()
{
  storedEffect = ((static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_ENUM::EFF_WHITE_COLOR) ? storedEffect : effects.getEn()); // сохраняем предыдущий эффект, если только это не белая лампа
  storedBright = getLampBrightness();
  lampState.isMicOn = false;
  LOG(printf_P, PSTR("Store: %d,%d\n"),storedEffect,storedBright);
}

void LAMP::restoreStored()
{
  LOG(printf_P, PSTR("Restore: %d,%d\n"),storedEffect,storedBright);
  if(storedBright)
    setLampBrightness(storedBright);
  lampState.isMicOn = flags.isMicOn;
  if (static_cast<EFF_ENUM>(storedEffect) != EFF_NONE) {    // ничего не должно происходить, включаемся на текущем :), текущий всегда определен...
    Task *_t = new Task(3 * TASK_SECOND, TASK_ONCE, [this](){remote_action(RA::RA_EFFECT, String(storedEffect).c_str(), NULL); TASK_RECYCLE; }, &ts, false);
    _t->enableDelayed();
  } else if(static_cast<EFF_ENUM>(effects.getEn()%256) == EFF_NONE) { // если по каким-то причинам текущий пустой, то выбираем рандомный
    Task *_t = new Task(3 * TASK_SECOND, TASK_ONCE, [this](){remote_action(RA::RA_EFF_RAND, NULL); TASK_RECYCLE; }, &ts, false);
    _t->enableDelayed();
  }
}

void LAMP::startNormalMode(bool forceOff)
{
  LOG(println,F("Normal mode"));
  if(forceOff)
    flags.ONflag=false;
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
  sendString(String(PSTR("- OTA UPDATE ON -")).c_str(), CRGB::Green);
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
    offset = (flags.MIRR_V ? 0 : WIDTH);
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
            //EffectMath::setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), (isWarning() && lampState.warnType==2) ? 0 : (isWarning() && lampState.warnType==1) ? 255 : getBFade());
            EffectMath::getPixel(offset + i, txtOffset + j).fadeToBlackBy((isWarning() && lampState.warnType==2) ? 0 : (isWarning() && lampState.warnType==1) ? 255 : getBFade());
        } else {
          if(isInverse)
            EffectMath::drawPixelXY(offset + i, txtOffset + j, letterColor);
          else
            //EffectMath::setLedsfadeToBlackBy(getPixelNumber(offset + i, txtOffset + j), (isWarning() && lampState.warnType==2) ? 0 : (isWarning() && lampState.warnType==1) ? 255 : getBFade());
            EffectMath::getPixel(offset + i, txtOffset + j).fadeToBlackBy((isWarning() && lampState.warnType==2) ? 0 : (isWarning() && lampState.warnType==1) ? 255 : getBFade());
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

void LAMP::sendString(const char* text, const CRGB &letterColor, bool forcePrint, bool clearQueue){
  if (!isLampOn() && forcePrint){
      disableEffectsUntilText(); // будем выводить текст, при выкюченной матрице
      setOffAfterText();
      changePower(true);
      setBrightness(OFF_BRIGHTNESS, false, false); // выводить будем минимальной яркостью в OFF_BRIGHTNESS пункта
      sendStringToLamp(text, letterColor, forcePrint, clearQueue);
  } else {
      sendStringToLamp(text, letterColor, forcePrint, clearQueue);
  }
}

String &LAMP::prepareText(String &source){
  source.replace(F("%TM"), embui.timeProcessor.getFormattedShortTime());
  source.replace(F("%IP"), WiFi.localIP().toString());
  source.replace(F("%EN"), effects.getEffectName());
  const tm *tm = localtime(embui.timeProcessor.now());
  char buffer[11]; //"xx.xx.xxxx"
  sprintf_P(buffer,PSTR("%02d.%02d.%04d"),tm->tm_mday,tm->tm_mon+1,tm->tm_year+EMBUI_TM_BASE_YEAR);
  source.replace(F("%DT"), buffer);
#ifdef LAMP_DEBUG  
  if(!source.isEmpty() && effects.getCurrent()!=EFF_ENUM::EFF_TIME && !isWarning()) // спам эффекта часы и предупреждений убираем костыльным способом :)
    LOG(println, source.c_str()); // вывести в лог строку, которая после преобразований получилась
#endif
  return source;  
}

void LAMP::sendStringToLampDirect(const char* text, const CRGB &letterColor, bool forcePrint, bool clearQueue, int8_t textOffset, int16_t fixedPos)
{
    String storage = text;
    prepareText(storage);
    doPrintStringToLamp(storage.c_str(), letterColor, textOffset, fixedPos); // отправляем
}

void LAMP::sendStringToLamp(const char* text, const CRGB &letterColor, bool forcePrint, bool clearQueue, int8_t textOffset, int16_t fixedPos)
{
  if((!flags.ONflag && !forcePrint) || (isAlarm() && !forcePrint)) return; // если выключена, или если будильник, но не задан принудительный вывод - то на выход
  if(textOffset==-128) textOffset=this->txtOffset;

  if(text==nullptr){ // текст пустой
    if(!lampState.isStringPrinting){ // ничего сейчас не печатается
      if(!docArrMessages){ // массив пустой
        return; // на выход
      }
      else { // есть что печатать
        JsonArray arr = (*docArrMessages).as<JsonArray>(); // используем имеющийся
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
        if(!arr.size()){ // очередь опустела, освобождаем массив
          delete docArrMessages;
          docArrMessages = nullptr;
        }
      }
    } else {
        // текст на входе пустой, идет печать
        return; // на выход
    }
  } else { // текст не пустой
    if(clearQueue){
      LOG(println, F("Clear message queue"));
      if(docArrMessages){ // очистить очередь, освободить память
          delete docArrMessages;
          docArrMessages = nullptr;
      }
      lampState.isStringPrinting = false; // сбросить текущий вывод строки
    }

    if(!lampState.isStringPrinting){ // ничего сейчас не печатается
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

      if(docArrMessages){
        arr = (*docArrMessages).as<JsonArray>(); // используем имеющийся
      } else {
        docArrMessages = new DynamicJsonDocument(512);
        arr = (*docArrMessages).to<JsonArray>(); // создаем новый
      }

      for (size_t i = 0; i < arr.size(); i++)
      {
        if((arr[i])[F("s")]==text
          && (arr[i])[F("c")]==((unsigned long)letterColor.r<<16)+((unsigned long)letterColor.g<<8)+(unsigned long)letterColor.b
          && (arr[i])[F("o")]==textOffset
          && (arr[i])[F("f")]==fixedPos
        ){
          LOG(println, F("Duplicate string skipped"));
          //LOG(println, (*docArrMessages).as<String>());
          return;
        }
      }

      JsonObject var = arr.createNestedObject();
      var[F("s")]=text;
      var[F("c")]=((unsigned long)letterColor.r<<16)+((unsigned long)letterColor.g<<8)+(unsigned long)letterColor.b;
      var[F("o")]=textOffset;
      var[F("f")]=fixedPos;

      String tmp; // Тут шаманство, чтобы не ломало JSON
      serializeJson((*docArrMessages), tmp);
      deserializeJson((*docArrMessages), tmp);

      LOG(print, F("Array: "));
      LOG(println, (*docArrMessages).as<String>());
    }
  }
}

void LAMP::doPrintStringToLamp(const char* text,  const CRGB &letterColor, const int8_t textOffset, const int16_t fixedPos)
{
  static String toPrint;
  static CRGB _letterColor;

  if(!lampState.isStringPrinting){
    toPrint.clear();
    fillStringManual(nullptr, CRGB::Black);
  }

  lampState.isStringPrinting = true;
  int8_t offs=(textOffset==-128?txtOffset:textOffset);

  if(text!=nullptr && text[0]!='\0'){
    toPrint.concat(text);
    _letterColor = letterColor;
  }

  if(toPrint.length()==0) {
    lampState.isStringPrinting = false;
    return; // нечего печатать
  } else {
    lampState.isStringPrinting = true;
  }

  if(tmStringStepTime.isReadyManual()){
    if(!fillStringManual(toPrint.c_str(), _letterColor, false, isAlarm() || (isWarning() && lampState.warnType<2), fixedPos, (fixedPos? 0 : LET_SPACE), offs) && (!isWarning() || (isWarning() && fixedPos))){ // смещаем
      tmStringStepTime.reset();
    }
    else {
      lampState.isStringPrinting = false;
      toPrint.clear(); // все напечатали
      sendStringToLamp(); // получаем новую порцию
    }
  } else {
    if((!isWarning() || (isWarning() && fixedPos)))
      fillStringManual(toPrint.c_str(), _letterColor, true, isAlarm() || (isWarning() && lampState.warnType<2), fixedPos, (fixedPos? 0 : LET_SPACE), offs);
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
      sprintf_P(strMessage, NY_MDG_STRING2, localtime(embui.timeProcessor.now())->tm_year+EMBUI_TM_BASE_YEAR);
    } else if(calc<300){
      sprintf_P(strMessage, NY_MDG_STRING1, (int)calc, String(FPSTR(TINTF_0C1)).c_str());
    } else if(calc/60<60){
      uint16_t calcT=calc/(60*60); // минуты
      uint8_t calcN=calcT%10; // остаток от деления на 10
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
      uint8_t calcN=calcT%10; // остаток от деления на 10
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
      uint8_t calcN=calcT%10; // остаток от деления на 10
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

// при вызове - вывозит на лампу текущее время
void LAMP::periodicTimeHandle(bool force)
{
  const tm* t = localtime(embui.timeProcessor.now());
  if(t->tm_sec && !force)
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
  if(mw==nullptr && !lampState.isCalibrationRequest && lampState.micAnalyseDivider){ // обычный режим
    mw = new MICWORKER(lampState.mic_scale,lampState.mic_noise,!counter);
    if(!mw) {
      mw=nullptr;
      return; // не удалось выделить память, на выход
    }
    //delete mw; mw = nullptr; return;
    
    lampState.samp_freq = mw->process(lampState.noise_reduce); // возвращаемое значение - частота семплирования
    lampState.last_min_peak = mw->getMinPeak();
    lampState.last_max_peak = mw->getMaxPeak();
    lampState.cur_val = mw->getCurVal();

    if(!counter) // раз на N измерений берем частоту, т.к. это требует обсчетов
      lampState.last_freq = mw->analyse(); // возвращаемое значение - частота главной гармоники
    if(lampState.micAnalyseDivider)
      counter = (counter+1)%(0x01<<(lampState.micAnalyseDivider-1)); // как часто выполнять анализ
    else
      counter = 1; // при micAnalyseDivider == 0 - отключено

    // EVERY_N_SECONDS(1){
    //   LOG(println, counter);
    // }

    //LOG(println, last_freq);
    //mw->debug();
    delete mw;
    mw = nullptr;
  } else if(lampState.isCalibrationRequest) {
    if(mw==nullptr){ // калибровка начало
      mw = new MICWORKER();
      mw->calibrate();
    } else { // калибровка продолжение
      mw->calibrate();
    }
    if(!mw->isCaliblation()){ // калибровка конец
      lampState.mic_noise = mw->getNoise();
      lampState.mic_scale = mw->getScale();
      lampState.isCalibrationRequest = false; // завершили
      delete mw;
      mw = nullptr;

      remote_action(RA::RA_MIC, NULL);
    }
  }
}
#endif

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
        fadelight(this, _brt);
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
 * переключатель эффектов для других методов,
 * может использовать фейдер, выбирать случайный эффект для демо
 * @param EFFSWITCH action - вид переключения (пред, след, случ.)
 * @param fade - переключаться через фейдер или сразу
 */
void LAMP::switcheffect(EFFSWITCH action, bool fade, uint16_t effnb, bool skip) {
#ifdef MIC_EFFECTS
    lampState.setMicAnalyseDivider(1); // восстановить делитель, при любой активности (поскольку эффекты могут его перенастраивать под себя)
#endif

#ifdef ENCODER
 exitSettings();
#endif

  if (!skip) {
    switch (action) {
    case EFFSWITCH::SW_NEXT :
        fade = (!fader) && fade;
        effects.setSelected(effects.getNext());
        break;
    case EFFSWITCH::SW_NEXT_DEMO :
        effects.setSelected(effects.getByCnt(1));
        break;
    case EFFSWITCH::SW_PREV :
        fade = (!fader) && fade;
        effects.setSelected(effects.getPrev());
        break;
    case EFFSWITCH::SW_SPECIFIC :
        //fade = (!fader) && fade;
        effects.setSelected(effects.getBy(effnb));
        break;
    case EFFSWITCH::SW_RND :
        effects.setSelected(effects.getByCnt(random(0, effects.getModeAmount())));
        break;
    case EFFSWITCH::SW_WHITE_HI:
        storeEffect();
        effects.setSelected(effects.getBy(EFF_WHITE_COLOR));
        setMode(LAMPMODE::MODE_WHITELAMP);
        break;
    case EFFSWITCH::SW_WHITE_LO:
        storeEffect();
        effects.setSelected(effects.getBy(EFF_WHITE_COLOR));
        setMode(LAMPMODE::MODE_WHITELAMP);
        break;
    default:
        return;
    }
    LOG(printf_P, PSTR("EFFSWITCH=%d, fade=%d, effnb=%d\n"), action, fade, effects.getSelected());
    // тухнем "вниз" только на включенной лампе
    if (fade && flags.ONflag) {
      fadelight(this, min(FADE_MINCHANGEBRT, (unsigned int)myLamp.getLampBrightness()), FADE_TIME, std::bind(&LAMP::switcheffect, this, action, fade, effnb, true));
      return;
    }
  } else {
    LOG(printf_P, PSTR("EFFSWITCH=%d, fade=%d, effnb=%d\n"), action, fade, effnb ? effnb : effects.getSelected());
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
    sendStringToLamp(String(F("%EN")).c_str(), CRGB::Green);
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
  if(effects.worker && flags.ONflag && !lampState.isEffectsDisabledUntilText){
    effects.worker->run(getUnsafeLedsArray(), &effects);
    ledsbuff.resize(NUM_LEDS);
    std::copy(getUnsafeLedsArray(), getUnsafeLedsArray() + NUM_LEDS, ledsbuff.begin()); // сохранить кадр в буфер
  }
  setBrightness(getNormalizedLampBrightness(), fade, natural);
}

/*
 * включает/выключает режим "демо"
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
void LAMP::demoTimer(SCHEDULER action, uint8_t tmout){
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    if(demoTask){
      demoTask->cancel();
    }
    break;
  case SCHEDULER::T_ENABLE :
    if(tmout){
      if(demoTask){
        demoTask->cancel();
      }
      demoTask = new Task(tmout * TASK_SECOND, TASK_FOREVER, std::bind(&remote_action, RA::RA_DEMO_NEXT, NULL), &ts, false, nullptr, [this](){TASK_RECYCLE; demoTask = nullptr;});
      demoTask->enableDelayed();
    }
    break;
  case SCHEDULER::T_RESET :
    if (isAlarm())
      stopAlarm(); // тут же сбросим и будильник
    if (mode==MODE_DEMO && demoTask)
      demoTask->restartDelayed();
    break;
  default:
    return;
  }
}

/*
 * включает/выключает таймер обработки эффектов
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
void LAMP::effectsTimer(SCHEDULER action, uint32_t _begin) {
//  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    if(effectsTask){
      effectsTask->cancel();
      embui.taskRecycle(effectsTask);
      effectsTask = nullptr;
    }
    break;
  case SCHEDULER::T_ENABLE :
    if(!effectsTask){
      effectsTask = new Task(_begin?_begin:EFFECTS_RUN_TIMER, TASK_ONCE, [this](){effectsTick();}, &ts, false);
    } else {
      effectsTask->set(_begin?_begin:EFFECTS_RUN_TIMER, TASK_ONCE, [this](){effectsTick();});
    }
    effectsTask->restartDelayed();
    break;
  case SCHEDULER::T_FRAME_ENABLE :
    if(!effectsTask){
      effectsTask = new Task(LED_SHOW_DELAY, TASK_ONCE, [this, _begin](){frameShow(_begin);}, &ts, false);
    } else {
      effectsTask->set(LED_SHOW_DELAY, TASK_ONCE, [this, _begin](){frameShow(_begin);});
    }
    effectsTask->restartDelayed();
    break;
  case SCHEDULER::T_RESET :
    if (effectsTask)
      effectsTask->restartDelayed();
    break;
  default:
    return;
  }
}

//-----------------------------

// ------------- мигающий цвет (не эффект! используется для отображения краткосрочного предупреждения; неблокирующий код, рисует поверх эффекта!) -------------
void LAMP::warningHelper(){
  if(lampState.isWarning) {
    if(!warningTask)
      return;
    String msg = warningTask->getData();

    uint16_t cnt = warningTask->getWarn_duration()/(warningTask->getWarn_blinkHalfPeriod()*2);
    uint8_t xPos = (WIDTH+LET_WIDTH*(cnt>99?3:cnt>9?2:1))/2;    
    switch(lampState.warnType){
      case 0: EffectMath::fillAll(warningTask->getWarn_color());
        break;
      case 1: {
        EffectMath::fillAll(warningTask->getWarn_color());
        if (!isPrintingNow())
          sendStringToLamp(msg.isEmpty() ? String(cnt).c_str() : msg.c_str(), warningTask->getWarn_color(), true, false, -128, xPos);
        break;
      }
      case 2: {
        EffectMath::fillAll(warningTask->getWarn_color());
        if (!isPrintingNow())
          sendStringToLamp(msg.isEmpty() ? String(cnt).c_str() : msg.c_str(), -warningTask->getWarn_color(), true, false, -128, xPos);
        break;
      }
      case 3: {
        if (!isPrintingNow())
          //sendStringToLamp(String(cnt).c_str(), cnt%2?warn_color:-warn_color, true, false, -128, xPos);
          sendStringToLamp(msg.isEmpty() ? String(cnt).c_str() : msg.c_str(), warningTask->getWarn_color(), true, false, -128, xPos);
        break;
      }
      default: break;
    }
  }
}

void LAMP::showWarning(
  const CRGB &color,                                        /* цвет вспышки                                                 */
  uint32_t duration,                                        /* продолжительность отображения предупреждения (общее время)   */
  uint16_t blinkHalfPeriod,                                 /* продолжительность одной вспышки в миллисекундах (полупериод) */
  uint8_t warnType,                                         /* тип предупреждения 0...3                                     */
  bool forcerestart,                                        /* перезапускать, если пришло повторное событие предупреждения  */
  const char *msg)                                          /* сообщение для вывода на матрицу                              */
{
  CRGB warn_color = CRGB::Black;
  uint32_t warn_duration = 1000;
  uint16_t warn_blinkHalfPeriod = 500;

  if(warningTask && !forcerestart){ // вытянуть данные из предыдущего таска, если таск существует и не перезапуск
    warn_color = warningTask->getWarn_color();
    warn_duration = warningTask->getWarn_duration();
    warn_blinkHalfPeriod = warningTask->getWarn_blinkHalfPeriod();
  }

  if(forcerestart || !warningTask){ // перезапуск или таск не существует - инициализация из параметров
    warn_color = color;
    warn_duration = duration;
    warn_blinkHalfPeriod = blinkHalfPeriod;
    lampState.isWarning = true;
    lampState.warnType = warnType;
  }

  if(!forcerestart && warnType<2)
    lampState.isWarning=!lampState.isWarning;
  if(warn_duration>warn_blinkHalfPeriod)
    warn_duration-=warn_blinkHalfPeriod;
  else
    warn_duration=0;
  if(warn_duration){
    if(warningTask){
      warningTask->cancel();
    }

    warningTask = new WarningTask(warn_color, warn_duration, warn_blinkHalfPeriod, msg, blinkHalfPeriod, TASK_ONCE
    //, std::bind(&LAMP::showWarning, this, warn_color, warn_duration, warn_blinkHalfPeriod, (uint8_t)lampState.warnType, !lampState.isWarning, msg)
    ,[this](){
      WarningTask *cur = (WarningTask *)ts.getCurrentTask();
      showWarning(cur->getWarn_color(),cur->getWarn_duration(),cur->getWarn_blinkHalfPeriod(),(uint8_t)lampState.warnType, !lampState.isWarning, cur->getData());
    }
    , &ts, false, nullptr, [](){TASK_RECYCLE;});
    warningTask->enableDelayed();
  }
  else {
    lampState.isWarning = false;
    if(warningTask)
      warningTask->cancel();
    warningTask = nullptr;
  }
}

// Fader object
LEDFader *fader = nullptr;
/**
 * @brief - Non-blocking light fader, uses scheduler to globaly fade FastLED brighness within specified duration
 * @param LAMP *lamp - lamp instance
 * @param uint8_t _targetbrightness - end value for the brighness to fade to, FastLED dim8
 *                                   function applied internaly for natiral dimming
 * @param uint32_t _duration - fade effect duraion, ms
 * @param callback  -  callback-функция, которая будет выполнена после окончания затухания
 */
void fadelight(LAMP *lamp, const uint8_t _targetbrightness, const uint32_t _duration, std::function<void()> callback){
    while(fader){
      fader->skipBrightness(); // отмена предыдущего фейдера
    }
    fader = (new LEDFader(&ts, lamp,_targetbrightness, _duration, callback))->getInstance();
}