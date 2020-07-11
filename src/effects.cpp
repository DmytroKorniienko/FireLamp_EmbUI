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

void EffectCalc::init(EFF_ENUM _eff, byte _brt, byte _spd, byte _scl){
  effect=_eff;
  brightness=_brt;
  speed=_spd;
  scale=_scl;
  active=true;
  load();
}

/*
 *  первоначальная загрузка эффекта, автозапускается из init()
 */
void EffectCalc::load(){}

bool EffectCalc::run(CRGB* ledarr, const char *opt){
  return false;
}

/**
 * проверка на холостой вызов для эффектов с доп. задержкой
 */
bool EffectCalc::dryrun(){
  if((millis() - lastrun - EFFECTS_RUN_TIMER) < (unsigned)(255-speed)/3){
    active=false;
  } else {
    lastrun = millis();
    active=true;
  }

  return !active;
}

/**
 * status - статус воркера, если работает и загружен эффект, отдает true
 */
bool EffectCalc::status(){return active;}

/**
 * setBrt - установка яркости для воркера
 */
void EffectCalc::setbrt(const byte _brt){
  brightness = _brt;
  //LOG(printf, "Worker brt: %d\n", brightness);
}

/**
 * setSpd - установка скорости для воркера
 */
void EffectCalc::setspd(const byte _spd){
  speed = _spd;
  //LOG(printf, "Worker speed: %d\n", speed);
}

/**
 * setBrt - установка шкалы для воркера
 */
void EffectCalc::setscl(byte _scl){
  scale = _scl;
  //LOG(printf, "Worker scale: %d\n", scale);

  if (usepalettes && !rval)    // менять палитру в соответствие со шкалой, если выставлен флаг
    palettemap(palettes, _scl);
}

/**
 * setrval - установка переменной Rval
 * должна вызываться внешним методом при смене рвал в гуе или где-там...
 */
void EffectCalc::setrval(const uint8_t _rval){
  if(!_rval) return;
  rval = _rval;
  // при смене Rval меняем палитру
  if (usepalettes)
    palettemap(palettes, rval);
}

// Load palletes into array
void EffectCalc::palettesload(){
  palettes.reserve(FASTLED_PALETTS_COUNT);
  palettes.push_back(&CloudColors_p);
  palettes.push_back(&ForestColors_p);
  palettes.push_back(&HeatColors_p);
  palettes.push_back(&LavaColors_p);
  palettes.push_back(&OceanColors_p);
  palettes.push_back(&PartyColors_p);
  palettes.push_back(&RainbowColors_p);
  palettes.push_back(&RainbowStripeColors_p);

  usepalettes = true; // активируем "авто-переключатель" палитр при изменении scale/R
  scale2pallete();    // выставляем текущую палитру
}

/**
 * palletemap - меняет указатель на текущую палитру из набора в соответствие с "ползунком"
 * @param _val - байт "ползунка"
 * @param _pals - набор с палитрами
 */
void EffectCalc::palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val){
  if (!_pals.size()) {
    LOG(println,F("No palettes loaded or wrong value!"));
    return;
  }
  ptPallete = (MAX_RANGE+0.1)/_pals.size();     // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
  palettepos = (uint8_t)((float)_val/ptPallete);
  curPalette = _pals.at(palettepos);
  palettescale = _val-ptPallete*(palettepos); // разбиваю на поддиапазоны внутри диапазона, будет уходить в 0 на крайней позиции поддиапазона, ну и хрен с ним :), хотя нужно помнить!

  LOG(printf_P,PSTR("Mapping value to pallete: Psize=%d, POS=%d, ptPallete=%4.2f, palettescale=%d, szof=%d\n"), _pals.size(), palettepos, ptPallete, palettescale, sizeof(TProgmemRGBPalette16 *));
}

/**
 * метод выбирает текущую палитру '*curPalette' из набора дотупных палитр 'palettes'
 * в соответствии со значением "бегунка" шкалы. В случае если задана паременная rval -
 * метод использует значение R,  иначе используется значение scale
 * (палитры меняются автоматом при изменении значения шкалы/R, метод оставлен для совместимости
 * и для первоначальной загрузки эффекта)
 */
void EffectCalc::scale2pallete(){
  if (!usepalettes)
    return;

  String var = myLamp.effects.getValue(myLamp.effects.getCurrent()->param, F("R"));
  if(!var.isEmpty()){
    rval = var.toInt();
    palettemap(palettes, rval);
  } else {
    palettemap(palettes, scale);
  }
}

// непустой дефолтный деструктор (если понадобится)
// EffectCalc::~EffectCalc(){LOG(println, "Effect object destroyed");}

// ******** общие мат. функции переиспользуются в другом эффекте
uint8_t EffectMath::mapsincos8(bool map, uint8_t theta, uint8_t lowest, uint8_t highest) {
  uint8_t beat = map ? sin8(theta) : cos8(theta);
  return lowest + scale8(beat, highest - lowest);
}

void EffectMath::MoveFractionalNoise(bool _scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift) {
  uint8_t zD;
  uint8_t zF;
  CRGB *leds = myLamp.getUnsafeLedsArray(); // unsafe
  CRGB ledsbuff[NUM_LEDS];
  uint16_t _side_a = _scale ? HEIGHT : WIDTH;
  uint16_t _side_b = _scale ? WIDTH : HEIGHT;

  for(uint8_t i=0; i<NUM_LAYERS; i++)
    for (uint16_t a = 0; a < _side_a; a++) {
      uint8_t _pixel = _scale ? noise3d[i][0][a] : noise3d[i][a][0];
      int16_t amount = ((int16_t)(_pixel - 128) * 2 * amplitude + shift * 256);
      int8_t delta = ((uint16_t)abs(amount) >> 8) ;
      int8_t fraction = ((uint16_t)abs(amount) & 255);
      for (uint8_t b = 0 ; b < _side_b; b++) {
        if (amount < 0) {
          zD = b - delta; zF = zD - 1;
        } else {
          zD = b + delta; zF = zD + 1;
        }
        CRGB PixelA = CRGB::Black  ;
        if ((zD >= 0) && (zD < _side_b))
          PixelA = _scale ? myLamp.getLeds(myLamp.getPixelNumber(zD%WIDTH, a%HEIGHT)) : myLamp.getLeds(myLamp.getPixelNumber(a%WIDTH, zD%HEIGHT));

        CRGB PixelB = CRGB::Black ;
        if ((zF >= 0) && (zF < _side_b))
          PixelB = _scale ? myLamp.getLeds(myLamp.getPixelNumber(zF%WIDTH, a%HEIGHT)) : myLamp.getLeds(myLamp.getPixelNumber(a%WIDTH, zF%HEIGHT));
        uint16_t x = _scale ? b : a;
        uint16_t y = _scale ? a : b;
        ledsbuff[myLamp.getPixelNumber(x%WIDTH, y%HEIGHT)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
      }
    }
  memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);
}

/**
 * Возвращает частное от а,б округленное до большего целого
 */
uint8_t EffectMath::ceil8(const uint8_t a, const uint8_t b){
  return a/b + !!(a%b);
}



// новый фейдер
void EffectMath::fadePixel(uint8_t i, uint8_t j, uint8_t step)
{
    int32_t pixelNum = myLamp.getPixelNumber(i, j);
    if (myLamp.getPixColor(pixelNum) == 0U) return;

    CRGB led = myLamp.getLeds(pixelNum);
    if (led.r >= 30U ||
        led.g >= 30U ||
        led.b >= 30U){
        myLamp.setLedsfadeToBlackBy(pixelNum,step);
    }
    else{
        myLamp.setLeds(pixelNum, 0U);
    }
}

// функция плавного угасания цвета для всех пикселей
void EffectMath::fader(uint8_t step)
{
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      fadePixel(i, j, step);
    }
  }
}


// ============= ЭФФЕКТЫ ===============

// ------------- конфетти --------------
bool EffectSparcles::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  return sparklesRoutine(*&ledarr, &*opt);
}

#define EFF_FADE_OUT_SPEED        (15U)                         // скорость затухания
bool EffectSparcles::sparklesRoutine(CRGB *leds, const char *param)
{

#ifndef MIC_EFFECTS
  #define _SPEED speed
  #define _SCALE scale
#else
  #define _SPEED _speed
  #define _SCALE _scale
  uint8_t _scale = scale, _speed = speed;

  if (myLamp.isMicOnOff()){
    uint8_t mmf = myLamp.getMicMapFreq();
    uint8_t mmp = myLamp.getMicMapMaxPeak();
    _scale = constrain(scale*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?3.0:(mmp?0.012*mmp:1.0)),1,255);
    _speed = constrain(speed*(mmf>0?(1.5*(mmf/255.0)+0.33):1),1,255);
    EffectMath::fader((uint8_t)(EFF_FADE_OUT_SPEED*((float)_SCALE)/255)+1);
  } else
#endif
    EffectMath::fader(map(_SCALE, 1, 255, 50, 0));

  for (uint8_t i = 0; i < (uint8_t)round(2.5*(_SPEED/255.0)+1); i++){
      uint8_t x = random(0U, WIDTH);
      uint8_t y = random(0U, HEIGHT);
      if (myLamp.getPixColorXY(x, y) == 0U){
        myLamp.setLeds(myLamp.getPixelNumber(x, y),CHSV(random(1U, 255U), random(192U, 255U), random(192U, 255U)));
      }
  }
  return true;
}

// ------------- белый свет (светится горизонтальная полоса по центру лампы; масштаб - высота центральной горизонтальной полосы; скорость - регулировка от холодного к тёплому; яркость - общая яркость) -------------
bool EffectWhiteColorStripe::run(CRGB *ledarr, const char *opt){
  // if (dryrun()) // для этого эффекта задержка не нужна в общем-то...
  //   return false;
  return whiteColorStripeRoutine(*&ledarr, &*opt);
}

bool EffectWhiteColorStripe::whiteColorStripeRoutine(CRGB *leds, const char *param)
{
    if(scale<127){
        uint8_t centerY = max((uint8_t)round(HEIGHT / 2.0F) - 1, 0);
        uint8_t bottomOffset = (uint8_t)(!(HEIGHT & 1) && (HEIGHT > 1));                      // если высота матрицы чётная, линий с максимальной яркостью две, а линии с минимальной яркостью снизу будут смещены на один ряд
        //Serial.printf_P(PSTR("%d %d\n"), centerY, bottomOffset);
        for (int16_t y = centerY; y >= 0; y--)
        {
          int br = BRIGHTNESS-(13*(WIDTH-y)); if((br-(255-myLamp.getLampBrightness()))<0) br=0;

          CRGB color = CHSV(
            45U,                                                                              // определяем тон
            map(speed, 0U, 255U, 0U, 170U),                            // определяем насыщенность
            y == centerY                                                                                                    // определяем яркость
              ? BRIGHTNESS                                                                                                  // для центральной горизонтальной полосы (или двух) яркость всегда равна BRIGHTNESS
              : ((scale) / 143.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? BRIGHTNESS : br);  // для остальных горизонтальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

          for (int16_t x = 0U; x < (int16_t)WIDTH; x++)
          {
            myLamp.drawPixelXY(x, y, color);                                                         // при чётной высоте матрицы максимально яркими отрисуются 2 центральных горизонтальных полосы
            myLamp.drawPixelXY(x, max((uint8_t)(HEIGHT - 1U) - (y + 1U) + bottomOffset, 0U), color); // при нечётной - одна, но дважды
          }
        }
    } else {
        uint8_t centerX = max((uint8_t)round(WIDTH / 2.0F) - 1, 0);
        uint8_t leftOffset = (uint8_t)(!(WIDTH & 1) && (WIDTH > 1));                      // если ширина матрицы чётная, линий с максимальной яркостью две, а линии с минимальной яркостью слева будут смещены на один ряд
        //Serial.printf_P(PSTR("%d %d\n"), centerX, leftOffset);
        for (int16_t y = 0U; y < (int16_t)HEIGHT; y++)
        {
          for (int16_t x = centerX; x >= 0; x--)
          {
            int br = BRIGHTNESS-(13*(WIDTH-x)); if((br-(255-myLamp.getLampBrightness()))<0) br=0;

            CRGB color = CHSV(
              45U,                                                                              // определяем тон
              map(speed, 0U, 255U, 0U, 170U),                            // определяем насыщенность
              x == centerX                                                                                                    // определяем яркость
                ? BRIGHTNESS                                                                                                  // для центральной вертикальной полосы (или двух) яркость всегда равна BRIGHTNESS
                : ((255-scale) / 143.0F) > ((centerX + 1.0F) - (x + 1.0F)) / (centerX + 1.0F) ? BRIGHTNESS : br);  // для остальных вертикальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

            myLamp.drawPixelXY(x, y, color);                                                      // при чётной ширине матрицы максимально яркими отрисуются 2 центральных вертикальных полосы
            myLamp.drawPixelXY(max((uint8_t)(WIDTH - 1U) - (x + 1U) + leftOffset, 0U), y, color); // при нечётной - одна, но дважды
          }
        }
    }
  return true;
}

// ============= водо/огне/лава/радуга/хренопад ===============
void EffectEverythingFall::load(){
    palettesload();    // подгружаем дефолтные палитры
}
bool EffectEverythingFall::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  return fire2012WithPalette(*&ledarr, &*opt);
}

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80U // 50 // 30 // 120 // 90 // 60
bool EffectEverythingFall::fire2012WithPalette(CRGB*leds, const char *param) {
  uint8_t coolingnew = constrain((uint16_t)scale * palettes.size() / HEIGHT + 7, 1, 255) ;

  myLamp.blur2d(20);
  myLamp.dimAll(254U - scale * (palettes.size() -1));

  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Cool down every cell a little
    for (unsigned int i = 0; i < HEIGHT; i++) {
      //heat[x][i] = qsub8(heat[x][i], random8(0, ((COOLINGNEW * 10) / HEIGHT) + 2));
      heat[x][i] = qsub8(heat[x][i], random8(0, coolingnew));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = HEIGHT - 1; k >= 2; k--) {
      heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKINGNEW) {
      int y = random8(2);
      heat[x][y] = qadd8(heat[x][y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (unsigned int j = 0; j < HEIGHT; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8(heat[x][j], 240);
      myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(*curPalette, colorindex));
    }
  }
  return true;
}

// --------------------------- эффект пульс ----------------------
// Stefan Petrick's PULSE Effect mod by PalPalych for GyverLamp
bool EffectPulse::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  return pulseRoutine(*&ledarr, &*opt);
}

void drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CRGB & color){
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    myLamp.drawPixelXY(x0, y0, color);
    return;
  }

  while (a >= b)  {
    myLamp.drawPixelXY(a + x0, b + y0, color);
    myLamp.drawPixelXY(b + x0, a + y0, color);
    myLamp.drawPixelXY(-a + x0, b + y0, color);
    myLamp.drawPixelXY(-b + x0, a + y0, color);
    myLamp.drawPixelXY(-a + x0, -b + y0, color);
    myLamp.drawPixelXY(-b + x0, -a + y0, color);
    myLamp.drawPixelXY(a + x0, -b + y0, color);
    myLamp.drawPixelXY(b + x0, -a + y0, color);
    b++;
    if (radiusError < 0)
      radiusError += 2 * b + 1;
    else
    {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
}

bool EffectPulse::pulseRoutine(CRGB *leds, const char *param) {

  CRGBPalette16 palette;
  CRGB _pulse_color;
  uint8_t currentRadius = 4;
  uint8_t centerX = random8(WIDTH - 5U) + 3U;
  uint8_t centerY = random8(HEIGHT - 5U) + 3U;
  //uint16_t _rc;
  uint8_t _pulse_hue = 0;
  uint8_t _pulse_hueall = 0;
  uint8_t _pulse_delta = 0;

  palette = RainbowColors_p;
  uint8_t _scale = scale;

  myLamp.dimAll(248U); // если эффект устанавливается с другими эффектами от Stefan Petrick, тогда  процедура должна называться dimAll (без двоечки)
  if (pulse_step <= currentRadius) {
    for (uint8_t i = 0; i < pulse_step; i++ ) {
      uint8_t _dark = qmul8( 2U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
      if (_scale == 1) {            // 1 - случайные диски
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 17) {    // 2...17 - перелив цвета дисков
        _pulse_delta = (17U - _scale) ;
        _pulse_color = CHSV(_pulse_hueall, 255U, _dark);

      } else if (_scale <= 33) {    // 18...33 - выбор цвета дисков
        _pulse_hue = (_scale - 18U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 50) {    // 34...50 - дискоцветы
        _pulse_hue += (_scale - 33U) * 5U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 67) {    // 51...67 - пузыри цветы
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
         _pulse_hue += (68U - _scale) * 7U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);

      } else if (_scale < 83) {     // 68...83 - выбор цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = (_scale - 68U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);

      } else if (_scale < 100) {    // 84...99 - перелив цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_delta = (_scale - 85U)  ;
        _pulse_color = CHSV(_pulse_hueall, _sat, _dark);

      } else { // 100 - случайные пузыри
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      }
      drawCircle(centerX, centerY, i, _pulse_color  );
    }
  } else {
    centerX = random8(WIDTH - 5U) + 3U;
    centerY = random8(HEIGHT - 5U) + 3U;
    _pulse_hueall += _pulse_delta;
    pulse_hue = random8(0U, 255U);
    currentRadius = random8(3U, 9U);
    pulse_step = 0;
  }
  pulse_step++;
  return true;
}

// радуги 2D
// ------------- радуга вертикальная/горизонтальная ----------------
//uint8_t hue;
bool EffectRainbow::rainbowHorVertRoutine(bool isVertical)
{
#ifdef MIC_EFFECTS
  hue += (4 * (myLamp.getMicMapMaxPeak()>map(rval, 1, 255, 80, 10)?5*(myLamp.getMicMapFreq()/255.0):1));
#else
  hue += 4;
#endif
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
    CHSV thisColor = CHSV((uint8_t)(hue + i * scale%86), 255, 255); // 1/3 без центральной между 1...255, т.е.: 1...84, 170...255
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      myLamp.drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
  return true;
}

// ------------- радуга диагональная -------------
bool EffectRainbow::run(CRGB *ledarr, const char *opt){
  // if (dryrun())
  //   return false;
  return rainbowDiagonalRoutine(*&ledarr, &*opt);
}

bool EffectRainbow::rainbowDiagonalRoutine(CRGB *leds, const char *param)
{
  if((millis() - lastrun - EFFECTS_RUN_TIMER)
#ifdef MIC_EFFECTS
    < (unsigned)(255-speed)*((myLamp.getMicMapMaxPeak()>scale%86+50)?(2.0*(50.0/myLamp.getMicMapMaxPeak())):1) ){
#else
    < (unsigned)(255-speed)){
#endif
    return false;
  } else {
    lastrun=millis();
  }

  if(scale<85){
    rainbowHorVertRoutine(false);
    return true;
  } else if (scale>170){
    rainbowHorVertRoutine(true);
    return true;
  }

  hue += 4;
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      float twirlFactor = EffectMath::fmap((float)scale, 85, 170, 8.3, 24);      // на сколько оборотов будет закручена матрица, [0..3]
#ifdef MIC_EFFECTS
      twirlFactor *= myLamp.getMicMapMaxPeak() > map(rval, 1, 255, 80, 10) ? 1.5 * (myLamp.getMicMapFreq() / 255.0) : 1;
#endif
      CRGB thisColor = CHSV((uint8_t)(hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)myLamp.getmaxDim())), 255, 255);
      myLamp.drawPixelXY(i, j, thisColor);
    }
  }
  return true;
}

// ------------- цвета -----------------
void EffectColors::load(){
    myLamp.fillAll(CHSV(scale, 255U, 55U)); // еще не наступила смена цвета, поэтому выводим текущий
}

bool EffectColors::run(CRGB *ledarr, const char *opt){
  // if (dryrun())
  //   return false;
  return colorsRoutine(*&ledarr, &*opt);
}

bool EffectColors::colorsRoutine(CRGB *leds, const char *param)
{
  static unsigned int step = 0; // доп. задержка
  unsigned int delay = (speed==1)?4294967294:255-speed+1; // на скорости 1 будет очень долгое ожидание)))

  step=(step+1)%(delay+1);
  if(step!=delay) {

#ifdef MIC_EFFECTS
  uint16_t mmf = myLamp.getMicMapFreq();
  uint16_t mmp = myLamp.getMicMapMaxPeak();

#if defined(LAMP_DEBUG) && defined(MIC_EFFECTS)
EVERY_N_SECONDS(1){
  LOG(printf_P,PSTR("MF: %5.2f MMF: %d MMP: %d scale %d speed: %d\n"), myLamp.getMicFreq(), mmf, mmp, scale, speed);
}
#endif
  if(myLamp.isMicOnOff()){
    // включен микрофон
    if(scale>=127){
      uint8_t pos = (round(3.0*(mmf+(25.0*speed/255.0))/255.0))*HEIGHT/8; // двигаем частоты по диапазону в зависимости от скорости и делим на 4 части 0...3
      for(uint8_t y=pos;y<pos+HEIGHT/8;y++){
        for(uint8_t x=0; x<WIDTH; x++){
          myLamp.setLeds(myLamp.getPixelNumber(x, y), CHSV(mmf / 1.5, 255U, constrain(mmp * (2.0 * (scale >> 1) / 127.0 + 0.33), 1, 255)));
          myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), CHSV(mmf / 1.5, 255U, constrain(mmp * (2.0 * (scale >> 1) / 127.0 + 0.33), 1, 255)));
        }
      }
      myLamp.dimAll(254); // плавно гасим
    } else {
      if(mmp>scale) // если амплитуда превышает масштаб
        myLamp.fillAll(CHSV(constrain(mmf*(2.0*speed/255.0),1,255), 255U, constrain(mmp*(2.0*scale/127.0+1.5),1,255))); // превышает минимаьный уровень громкости, значит выводим текущую частоту
      else
        myLamp.dimAll(252); // плавно гасим
    }
  } else {
    // выключен микрофон
    myLamp.fillAll(CHSV(ihue, 255U, 255U)); // еще не наступила смена цвета, поэтому выводим текущий
  }
#else
  myLamp.fillAll(CHSV(ihue, 255U, 255U)); // еще не наступила смена цвета, поэтому выводим текущий
#endif
  } else {
    ihue += scale; // смещаемся на следущий
  }
  return true;
}

// ------------- матрица ---------------
bool EffectMatrix::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  return matrixRoutine(*&ledarr, &*opt);
}

bool EffectMatrix::matrixRoutine(CRGB *leds, const char *param)
{
  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    // обрабатываем нашу матрицу снизу вверх до второй сверху строчки
    for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
    {
      uint32_t thisColor = myLamp.getPixColorXY(x, y);                                              // берём цвет нашего пикселя
      uint32_t upperColor = myLamp.getPixColorXY(x, y + 1U);                                        // берём цвет пикселя над нашим
      if (upperColor >= 0x900000 && random(7 * HEIGHT) != 0U)                  // если выше нас максимальная яркость, игнорим этот факт с некой вероятностью или опускаем цепочку ниже
        myLamp.drawPixelXY(x, y, upperColor);
      else if (thisColor == 0U && random((255 - scale) * HEIGHT) == 0U)  // если наш пиксель ещё не горит, иногда зажигаем новые цепочки
      //else if (thisColor == 0U && random((255 - scale) * HEIGHT*3) == 0U)  // для длинных хвостов
        myLamp.drawPixelXY(x, y, 0x9bf800);
      else if (thisColor <= 0x050800)                                                        // если наш пиксель почти погас, стараемся сделать затухание медленней
      {
        if (thisColor >= 0x030000)
          myLamp.drawPixelXY(x, y, 0x020300);
        else if (thisColor != 0U)
          myLamp.drawPixelXY(x, y, 0U);
      }
      else if (thisColor >= 0x900000)                                                        // если наш пиксель максимальной яркости, резко снижаем яркость
        myLamp.drawPixelXY(x, y, 0x558800);
      else
        myLamp.drawPixelXY(x, y, thisColor - 0x0a1000);                                             // в остальных случаях снижаем яркость на 1 уровень
        //myLamp.drawPixelXY(x, y, thisColor - 0x050800);                                             // для длинных хвостов
    }
    // аналогично обрабатываем верхний ряд пикселей матрицы
    uint32_t thisColor = myLamp.getPixColorXY(x, HEIGHT - 1U);
    if (thisColor == 0U)                                                                     // если наш верхний пиксель не горит, заполняем его с вероятностью .Scale
    {
      if (random(255 - scale) == 0U)
        myLamp.drawPixelXY(x, HEIGHT - 1U, 0x9bf800);
    }
    else if (thisColor <= 0x050800)                                                          // если наш верхний пиксель почти погас, стараемся сделать затухание медленней
    {
      if (thisColor >= 0x030000)
        myLamp.drawPixelXY(x, HEIGHT - 1U, 0x020300);
      else
        myLamp.drawPixelXY(x, HEIGHT - 1U, 0U);
    }
    else if (thisColor >= 0x900000)                                                          // если наш верхний пиксель максимальной яркости, резко снижаем яркость
      myLamp.drawPixelXY(x, HEIGHT - 1U, 0x558800);
    else
      myLamp.drawPixelXY(x, HEIGHT - 1U, thisColor - 0x0a1000);                                     // в остальных случаях снижаем яркость на 1 уровень
      //myLamp.drawPixelXY(x, HEIGHT - 1U, thisColor - 0x050800);                                     // для длинных хвостов
  }

  return true;
}

// ------------- снегопад ----------
bool EffectSnow::run(CRGB *ledarr, const char *opt){
  return snowRoutine(*&ledarr, &*opt);
}

#define SNOW_SCALE (1.25) //0.25...5.0
bool EffectSnow::snowRoutine(CRGB *leds, const char *param)
{

  snowShift = snowShift + speed/255.0;

  if(SNOW_SCALE*snowShift>1.0){ // будет смещение

  EVERY_N_SECONDS(1){
    LOG(printf_P, PSTR("%5.2f : %5.2f\n"),snowShift, SNOW_SCALE*snowShift );
  }

    // сдвигаем всё вниз
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
      for (uint8_t y = 0U; y < HEIGHT - 1; y++)
      {
        myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y + SNOW_SCALE*snowShift));
      }
    }

    for (uint8_t x = 0U; x < WIDTH && SNOW_SCALE*snowShift>1.0; x++)
    {
      // заполняем случайно верхнюю строку
      // а также не даём двум блокам по вертикали вместе быть
      if (myLamp.getPixColorXY(x, HEIGHT - 2U) == 0U && (random(0, 255 - scale) == 0U))
        myLamp.drawPixelXY(x, HEIGHT - 1U, 0xE0FFFF - 0x101010 * random(0, 4));
      else
        myLamp.drawPixelXY(x, HEIGHT - 1U, 0x000000);
    }
  }
  // т.к. не храним позицию, то смещаем все синхронно, но в идеале - хранить позиции
  snowShift = (SNOW_SCALE*snowShift > 1.0 ? (SNOW_SCALE*snowShift - (int)(snowShift*SNOW_SCALE)) : (snowShift));
  return true;
}

// ------------- метель -------------

// ------------- звездопад/метель -------------
bool EffectStarFall::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  return snowStormStarfallRoutine(*&ledarr, &*opt);
}

bool EffectStarFall::snowStormStarfallRoutine(CRGB *leds, const char *param)
{
  // заполняем головами комет левую и верхнюю линию
  for (uint8_t i = HEIGHT / 2U; i < HEIGHT; i++)
  {
    if (myLamp.getPixColorXY(0U, i) == 0U &&
       (random(0, (scale<127?SNOW_DENSE:STAR_DENSE)) == 0) &&
        myLamp.getPixColorXY(0U, i + 1U) == 0U &&
        myLamp.getPixColorXY(0U, i - 1U) == 0U)
    {
      myLamp.setLeds(myLamp.getPixelNumber(0U, i), CHSV(random(0, 200), (scale<127?SNOW_SATURATION:STAR_SATURATION), 255U));
    }
  }

  for (uint8_t i = 0U; i < WIDTH / 2U; i++)
  {
    if (myLamp.getPixColorXY(i, HEIGHT - 1U) == 0U &&
       (random(0, map((scale%128)*2, 0U, 255U, 10U, 120U)) == 0U) &&
        myLamp.getPixColorXY(i + 1U, HEIGHT - 1U) == 0U &&
        myLamp.getPixColorXY(i - 1U, HEIGHT - 1U) == 0U)
    {
      myLamp.setLeds(myLamp.getPixelNumber(i, HEIGHT - 1U), CHSV(random(0, 200),  (scale<127?SNOW_SATURATION:STAR_SATURATION), 255U));
    }
  }

  // сдвигаем по диагонали
  for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
  {
    for (uint8_t x = WIDTH - 1U; x > 0U; x--)
    {
      myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x - 1U, y + 1U));
    }
  }

  // уменьшаем яркость левой и верхней линии, формируем "хвосты"
  for (uint8_t i = HEIGHT / 2U; i < HEIGHT; i++)
  {
    EffectMath::fadePixel(0U, i,  (scale<127?SNOW_TAIL_STEP:STAR_TAIL_STEP));
  }
  for (uint8_t i = 0U; i < WIDTH / 2U; i++)
  {
    EffectMath::fadePixel(i, HEIGHT - 1U, (scale<127?SNOW_TAIL_STEP:STAR_TAIL_STEP));
  }
  return true;
}

// ------------- светлячки --------------
//#define LIGHTERS_AM           (100U)
void EffectLighters::load(){
  randomSeed(millis());
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++)
  {
    lightersIdx=0;
    lightersPos[0U][i] = random(0, WIDTH);
    lightersPos[1U][i] = random(0, HEIGHT);
    lightersSpeed[0U][i] = random(-20, 20);
    lightersSpeed[1U][i] = random(-20, 20);
    lightersColor[i] = random(0U, 255U);
  }

}

bool EffectLighters::run(CRGB *ledarr, const char *opt){
  return lightersRoutine(*&ledarr, &*opt);
}

bool EffectLighters::lightersRoutine(CRGB *leds, const char *param)
{

  float speedfactor = speed/4096.0+0.001;

  myLamp.blur2d(speed/10);
  myLamp.dimAll(50+speed/10);

  for (uint8_t i = 0U; i < (uint8_t)((LIGHTERS_AM/255.0)*scale)+1; i++) // масштабируем на LIGHTERS_AM, чтобы не было выхода за диапазон
  {
    // EVERY_N_SECONDS(1)
    // {
    //   LOG.printf_P("S0:%d S1:%d P0:%3.2f P1:%3.2f, scale:%3.2f\n", lightersSpeed[0U][i], lightersSpeed[1U][i],lightersPos[0U][i],lightersPos[1U][i],speedfactor);
    // }

    EVERY_N_MILLIS(1000)
    {
      lightersIdx = (lightersIdx+1)%(uint8_t)(((LIGHTERS_AM/255.0)*scale)+1);
      lightersSpeed[0U][lightersIdx] += random(-10, 10);
      lightersSpeed[1U][lightersIdx] += random(-10, 10);
      lightersSpeed[0U][lightersIdx] %= 21;
      lightersSpeed[1U][lightersIdx] %= 21;
    }

    lightersPos[0U][i] += lightersSpeed[0U][i]*speedfactor;
    lightersPos[1U][i] += lightersSpeed[1U][i]*speedfactor;

    if (lightersPos[0U][i] < 0) lightersPos[0U][i] = (WIDTH - 1);
    if (lightersPos[0U][i] >= (int32_t)WIDTH) lightersPos[0U][i] = 0;

    if (lightersPos[1U][i] <= 0)
    {
      lightersPos[1U][i] = 0;
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    if (lightersPos[1U][i] >= (int32_t)(HEIGHT - 1))
    {
      lightersPos[1U][i] = (HEIGHT - 1U);
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    myLamp.drawPixelXY(lightersPos[0U][i], lightersPos[1U][i], CHSV(lightersColor[i], 255U, 255U));
  }
  return true;
}

// ------------- светлячки со шлейфом -------------
void EffectLighterTracers::load(){
  for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
  {
    int8_t sign;
    // забиваем случайными данными
    coord[j][0U] = WIDTH / 2;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][0U] = random(4, 15) * sign;
    coord[j][1U] = HEIGHT / 2;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][1U] = random(4, 15) * sign;
    //ballColors[j] = random(0, 9) * 28;
  }
}


bool EffectLighterTracers::run(CRGB *ledarr, const char *opt){
  // if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
  //   return;
  // } else {
  //   myLamp.setEffDelay(millis());
  // }

  return lighterTracersRoutine(*&ledarr, &*opt);
}

bool EffectLighterTracers::lighterTracersRoutine(CRGB *leds, const char *param)
{
  float speedfactor = speed/2048.0+0.01;

  if (!BALL_TRACK)                                          // режим без следов шариков
  {
    FastLED.clear();
  }
  else                                                      // режим со следами
  {
    myLamp.blur2d(myLamp.effects.getSpeed()/10);
    EffectMath::fader(TRACK_STEP);
  }

  // движение шариков
  int maxBalls = (uint8_t)((BALLS_AMOUNT/255.0)*scale+0.99);
  for (uint8_t j = 0U; j < maxBalls; j++)
  {
    // цвет зависит от масштаба
    ballColors[j] = scale * (maxBalls-j) * BALLS_AMOUNT + j;

    // движение шариков
    for (uint8_t i = 0U; i < 2U; i++)
    {
      coord[j][i] += vector[j][i]*speedfactor;
      if (coord[j][i] < 0)
      {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }

    if (coord[j][0U] > (int16_t)((WIDTH - 1)))
    {
      coord[j][0U] = (WIDTH - 1);
      vector[j][0U] = -vector[j][0U];
    }
    if (coord[j][1U] > (int16_t)((HEIGHT - 1)))
    {
      coord[j][1U] = (HEIGHT - 1);
      vector[j][1U] = -vector[j][1U];
    }
    myLamp.setLeds(myLamp.getPixelNumber(coord[j][0U], coord[j][1U]), CHSV(ballColors[j], 255U, 255U));
  }
  return true;
}

// ------------- пейнтбол -------------
bool EffectLightBalls::run(CRGB *ledarr, const char *opt){
  return lightBallsRoutine(*&ledarr, &*opt);
}

#define BORDERTHICKNESS       (1U)                          // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)
bool EffectLightBalls::lightBallsRoutine(CRGB *leds, const char *param)
{
  const uint8_t paintWidth = WIDTH - BORDERTHICKNESS * 2;
  const uint8_t paintHeight = HEIGHT - BORDERTHICKNESS * 2;

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  uint8_t blurAmount = dim8_raw(beatsin8(3,64,100));
  blur2d(leds, WIDTH, HEIGHT, blurAmount);

  float speedScale = (((float)speed)/255)+(5.0/255.0);

  // Use two out-of-sync sine waves
  uint16_t  i = beatsin16( 79*speedScale, 0, 255); //91
  uint16_t  j = beatsin16( 67*speedScale, 0, 255); //109
  uint16_t  k = beatsin16( 53*speedScale, 0, 255); //73
  uint16_t  m = beatsin16( 97*speedScale, 0, 255); //123

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis() / (scale/16 + 1);
  leds[myLamp.getPixelNumber( highByte(i * paintWidth) + BORDERTHICKNESS, highByte(j * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 29, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(j * paintWidth) + BORDERTHICKNESS, highByte(k * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 41, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(k * paintWidth) + BORDERTHICKNESS, highByte(m * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 37, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(m * paintWidth) + BORDERTHICKNESS, highByte(i * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 53, 200U, 255U);

  return true;
}

// ------------- эффект "блуждающий кубик" -------------
void EffectBall::load(){
  ballSize = map(scale, 0U, 255U, 2U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 2));
  for (uint8_t i = 0U; i < 2U; i++)
  {
    coordB[i] = i? (WIDTH - ballSize) / 2 : (HEIGHT - ballSize) / 2;
    vectorB[i] = random(8, 24) - 12;
    ballColor = random(1, 255) * scale;
  }
}

bool EffectBall::run(CRGB *ledarr, const char *opt){
  return ballRoutine(*&ledarr, &*opt);
}

bool EffectBall::ballRoutine(CRGB *leds, const char *param)
{
  ballSize = map(scale, 0U, 255U, 2U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 2));

// каждые 10 секунд коррекция направления
  EVERY_N_SECONDS(10){
    for (uint8_t i = 0U; i < 2U; i++)
    {
      if(abs(vectorB[i])<12)
        vectorB[i] += (random(0, 8) - 4);
      else if (vectorB[i]>12)
        vectorB[i] -= random(1, 6);
      else
        vectorB[i] += random(1, 6);
      if(!vectorB[i]) vectorB[i]++;
      ballColor = random(1, 255) * scale;
    }
  }

  for (uint8_t i = 0U; i < 2U; i++)
  {
    coordB[i] += vectorB[i]*((0.1*speed)/255.0);
    if ((int8_t)coordB[i] < 0)
    {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR) ballColor = random(1, 255) * scale;
    }
  }
  if ((int8_t)coordB[0U] > (int16_t)(WIDTH - ballSize))
  {
    coordB[0U] = (WIDTH - ballSize);
    vectorB[0U] = -vectorB[0U];
    if (RANDOM_COLOR) ballColor = random(1, 255) * scale;
  }
  if ((int8_t)coordB[1U] > (int16_t)(HEIGHT - ballSize))
  {
    coordB[1U] = (HEIGHT - ballSize);
    vectorB[1U] = -vectorB[1U];
    if (RANDOM_COLOR) ballColor = random(1, 255) * scale;
  }

  //FastLED.clear();
  EffectMath::fader((uint8_t)(10*((float)speed)/255)+50); // фейдер, 10 - коэф. масштабирование (вся шкала 0...10), 50 - смещение (мин. уровень фейдера)
  for (uint8_t i = 0U; i < ballSize; i++)
  {
    for (uint8_t j = 0U; j < ballSize; j++)
    {
      myLamp.setLeds(myLamp.getPixelNumber((int8_t)coordB[0U] + i, (int8_t)coordB[1U] + j), CHSV(ballColor, 255U, 255U));
    }
  }
  return true;
}

//-- 3D Noise эффектцы --------------

// для работы FastLed (blur2d)
uint16_t XY(uint8_t x, uint8_t y)
{
#ifdef ROTATED_MATRIX
  return myLamp.getPixelNumber(y,x); // повернутое на 90 градусов
#else
  return myLamp.getPixelNumber(x,y); // обычное подключение
#endif
}

void Effect3DNoise::fillNoiseLED()
{
  uint8_t dataSmoothing = 0;
  if (_speed < 50)
  {
    dataSmoothing = 200 - (_speed * 4);
  }
  for (uint8_t i = 0; i < myLamp.getminDim()*2; i++)
  {
    int32_t ioffset = _scale * i;
    for (uint8_t j = 0; j < myLamp.getmaxDim(); j++)
    {
      int32_t joffset = _scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing)
      {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }
  z += _speed;

  // apply slow drift to X and Y, just for visual variation.
  x += _speed / 8;
  y -= _speed / 16;

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint8_t index = noise[j%(myLamp.getminDim()*2)][i];
      uint8_t bri =   noise[i%(myLamp.getminDim()*2)][j];
      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop)
      {
        index += ihue;
      }
      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 )
      {
        bri = 255;
      }
      else
      {
        bri = dim8_raw( bri * 2);
      }
      CRGB color = ColorFromPalette( *curPalette, index, bri);

      myLamp.drawPixelXY(i, j, color);                             //leds[getPixelNumber(i, j)] = color;
    }
  }
  ihue += 1;
}

void Effect3DNoise::fillnoise8()
{
  for (uint8_t i = 0; i < myLamp.getminDim()*2; i++)
  {
    int32_t ioffset = _scale * i;
    for (uint8_t j = 0; j < myLamp.getmaxDim(); j++)
    {
      int32_t joffset = _scale * j;
      noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    }
  }
  z += _speed;
}

void Effect3DNoise::load(){
  switch (effect)
  {
  case EFF_ENUM::EFF_RAINBOW :
    curPalette = &RainbowColors_p;
    colorLoop = 1;
    break;
  case EFF_ENUM::EFF_RAINBOW_STRIPE :
    curPalette = &RainbowStripeColors_p;
    colorLoop = 1;
    break;
  case EFF_ENUM::EFF_ZEBRA :
    curPalette = &ZeebraColors_p;
    colorLoop = 1;
    break;
  case EFF_ENUM::EFF_FOREST :
    curPalette = &ForestColors_p;
    colorLoop = 0;
    break;
  case EFF_ENUM::EFF_OCEAN :
    curPalette = &OceanColors_p;
    colorLoop = 0;
     break;
  case EFF_ENUM::EFF_PLASMA :
    curPalette = &PartyColors_p;
    colorLoop = 1;
    break;
  case EFF_ENUM::EFF_CLOUDS :
    curPalette = &CloudColors_p;
    colorLoop = 0;
    break;
  case EFF_ENUM::EFF_LAVA :
    curPalette = &LavaColors_p;
    colorLoop = 0;
    break;
  default:
    return;
  }

}

bool Effect3DNoise::run(CRGB *ledarr, const char *opt){
  #ifdef MIC_EFFECTS
    uint8_t mmf = myLamp.getMicMapFreq();
    uint8_t mmp = myLamp.getMicMapMaxPeak();
    _scale = (NOISE_SCALE_AMP*scale/255+NOISE_SCALE_ADD)*(mmf>0?(1.5*mmf/255.0):1);
    _speed = NOISE_SCALE_AMP*speed/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
    _scale = NOISE_SCALE_AMP*scale/255+NOISE_SCALE_ADD;
    _speed = NOISE_SCALE_AMP*speed/255;
  #endif

  switch (effect)
  {
  case EFF_ENUM::EFF_MADNESS :
    fillnoise8();
    for (uint8_t i = 0; i < WIDTH; i++)
    {
      for (uint8_t j = 0; j < HEIGHT; j++)
      {
        CRGB thisColor = CHSV(noise[j][i], 255, noise[i][j]);
        myLamp.drawPixelXY(i, j, thisColor);                         //leds[getPixelNumber(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);
      }
    }
    ihue += 1;
    break;
  default:
    fillNoiseLED();
  }
  return true;
}


// --------------------------- эффект мячики ----------------------
//  BouncingBalls2014 is a program that lets you animate an LED strip
//  to look like a group of bouncing balls
//  Daniel Wilson, 2014
//  https://github.com/githubcdr/Arduino/blob/master/bouncingballs/bouncingballs.ino
//  With BIG thanks to the FastLED community!
//  адаптация от SottNick
void EffectBBalls::load(){
    FastLED.clear();
    bballsNUM_BALLS =  map(scale, 0, 255, 1, bballsMaxNUM_BALLS);

    for (int i = 0 ; i < bballsNUM_BALLS ; i++) {          // Initialize variables
      bballsCOLOR[i] = random8();
      bballsX[i] = random8(1U, WIDTH);
      bballsTLast[i] = millis();
      bballsPos[i] = 0;                                    // Balls start on the ground
      bballsVImpact[i] = bballsVImpact0;                   // And "pop" up at vImpact0
      bballsCOR[i] = 0.90 - float(i) / pow(bballsNUM_BALLS, 2);
      bballsShift[i] = false;
    }
}

bool EffectBBalls::run(CRGB *ledarr, const char *opt){
  return bBallsRoutine(*&ledarr, &*opt);
}

bool EffectBBalls::bBallsRoutine(CRGB *leds, const char *param)
{
  bballsNUM_BALLS =  map(scale, 0, 255, 1, bballsMaxNUM_BALLS);

  myLamp.dimAll(50);
  for (int i = 0 ; i < bballsNUM_BALLS ; i++) {
    //myLamp.setLeds(myLamp.getPixelNumber(bballsX[i], bballsPos[i]), CRGB::Black); // off for the next loop around

    bballsTCycle =  millis() - bballsTLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    bballsHi = 0.5 * bballsGRAVITY * pow( bballsTCycle/(1150 - speed * 3) , 2.0 ) + bballsVImpact[i] * bballsTCycle/(1150 - speed * 3);

    if ( bballsHi < 0 ) {
      bballsTLast[i] = millis();
      bballsHi = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      bballsVImpact[i] = bballsCOR[i] * bballsVImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR


      //if ( bballsVImpact[i] < 0.01 ) bballsVImpact[i] = bballsVImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
      if ( bballsVImpact[i] < 0.1 ) // сделал, чтобы мячики меняли свою прыгучесть и положение каждый цикл
      {
        bballsCOR[i] = 0.90 - float(random(0U, 9U)) / pow(random(4U, 9U), 2); // сделал, чтобы мячики меняли свою прыгучесть каждый цикл
        bballsShift[i] = bballsCOR[i] >= 0.89;                             // если мячик максимальной прыгучести, то разрешаем ему сдвинуться
        bballsVImpact[i] = bballsVImpact0;
      }
    }
    bballsPos[i] = round( bballsHi * (HEIGHT - 1) / bballsH0);       // Map "h" to a "pos" integer index position on the LED strip
    if (bballsShift[i] && (bballsPos[i] == HEIGHT - 1)) {                  // если мячик получил право, то пускай сдвинется на максимальной высоте 1 раз
      bballsShift[i] = false;
      if (bballsCOLOR[i] % 2 == 0) {                                       // чётные налево, нечётные направо
        if (bballsX[i] == 0U) bballsX[i] = WIDTH - 1U;
        else --bballsX[i];
      } else {
        if (bballsX[i] == WIDTH - 1U) bballsX[i] = 0U;
        else ++bballsX[i];
      }
    }
    myLamp.setLeds(myLamp.getPixelNumber(bballsX[i], bballsPos[i]), CHSV(bballsCOLOR[i], 255, 255));
  }
  return true;
}

// ***** SINUSOID3 / СИНУСОИД3 *****
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
/*
  Sinusoid3 by Stefan Petrick (mod by Palpalych for GyverLamp 27/02/2020)
  read more about the concept: https://www.youtube.com/watch?v=mubH-w_gwdA
*/
bool EffectSinusoid3::run(CRGB *ledarr, const char *opt){
  return sinusoid3Routine(*&ledarr, &*opt);
}

bool EffectSinusoid3::sinusoid3Routine(CRGB *leds, const char *param)
{
  const uint8_t semiHeightMajor =  HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t semiWidthMajor =  WIDTH / 2  + (WIDTH % 2) ;
  float e_s3_speed = 0.004 * speed + 0.015; // speed of the movement along the Lissajous curves
  float e_s3_size = 3 * (float)scale/255.0 + 2;    // amplitude of the curves

  float time_shift = millis(); //float(millis()%(uint32_t)(30000*(1.0/((float)myLamp.effects.getSpeed()/255))));

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      CRGB color;
      uint8_t _scale = map8(255-scale,50,150);

      float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 98.301 * time_shift)))/32767.0;  // the 8 centers the middle on a 16x16
      float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 72.0874 * time_shift)))/32767.0;
      int8_t v = 127 * (1 + sin16 ( 127*_scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.r = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 68.8107 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 65.534 * time_shift)))/32767.0;
      v = 127 * (1 + sin16 ( 127*_scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.g = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 134.3447 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 170.3884 * time_shift)))/32767.0;
      v = 127 * (1 + sin16 ( 127*_scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.b = ~v;

      myLamp.drawPixelXY(x, y, color);
    }
  }
  return true;
}

/*
  Metaballs proof of concept by Stefan Petrick (mod by Palpalych for GyverLamp 27/02/2020)
  ...very rough 8bit math here...
  read more about the concept of isosurfaces and metaballs:
  https://www.gamedev.net/articles/programming/graphics/exploring-metaballs-and-isosurfaces-in-2d-r2556
*/

// ***** METABALLS / МЕТАШАРИКИ *****
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
bool EffectMetaBalls::run(CRGB *ledarr, const char *opt){
  return metaBallsRoutine(*&ledarr, &*opt);
}

bool EffectMetaBalls::metaBallsRoutine(CRGB *leds, const char *param)
{
  float _speed = speed/127.0;

  // get some 2 random moving points
  uint8_t x2 = inoise8(millis() * _speed, 25355, 685 ) / WIDTH;
  uint8_t y2 = inoise8(millis() * _speed, 355, 11685 ) / HEIGHT;

  uint8_t x3 = inoise8(millis() * _speed, 55355, 6685 ) / WIDTH;
  uint8_t y3 = inoise8(millis() * _speed, 25355, 22685 ) / HEIGHT;

  // and one Lissajou function
  uint8_t x1 = beatsin8(23 * _speed, 0, 15);
  uint8_t y1 = beatsin8(28 * _speed, 0, 15);

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {

      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      uint8_t  dx =  abs(x - x1);
      uint8_t  dy =  abs(y - y1);
      uint8_t dist = 2 * sqrt((dx * dx) + (dy * dy));

      dx =  abs(x - x2);
      dy =  abs(y - y2);
      dist += sqrt((dx * dx) + (dy * dy));

      dx =  abs(x - x3);
      dy =  abs(y - y3);
      dist += sqrt((dx * dx) + (dy * dy));

      // inverse result
      byte color = scale*4 / (dist==0?1:dist);

      // map color between thresholds
      if (color > 0 and color < 60) {
        myLamp.drawPixelXY(x, y, CHSV(color * 9, 255, 255));
      } else {
        myLamp.drawPixelXY(x, y, CHSV(0, 255, 255));
      }
      // show the 3 points, too
      myLamp.drawPixelXY(x1, y1, CRGB(255, 255, 255));
      myLamp.drawPixelXY(x2, y2, CRGB(255, 255, 255));
      myLamp.drawPixelXY(x3, y3, CRGB(255, 255, 255));
    }
  }
  return true;
}

// --------------------------- эффект спирали ----------------------
/*
 * Aurora: https://github.com/pixelmatix/aurora
 * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
 * Copyright (c) 2014 Jason Coon
 * Неполная адаптация SottNick
 */

void EffectSpiro::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectSpiro::run(CRGB *ledarr, const char *opt){
  return spiroRoutine(*&ledarr, &*opt);
}

// ***** Эффект "Спираль"     ****
bool EffectSpiro::spiroRoutine(CRGB *leds, const char *param)
{

  // страхуемся от креша
  if (curPalette == nullptr) {
    return false;
  }

  const float speed_factor = (float)speed/127+1;
  uint8_t spirooffset = 256 / spirocount;
  boolean change = false;
  myLamp.blur2d(15);//45/(speed_factor*3));
  myLamp.dimAll(254U - palettescale);
  //myLamp.dimAll(250-speed_factor*7);

  for (int i = 0; i < spirocount; i++) {
    uint8_t  x = EffectMath::mapsincos8(MAP_SIN, spirotheta1 + i * spirooffset, spirominx, spiromaxx);
    uint8_t  y = EffectMath::mapsincos8(MAP_COS, spirotheta1 + i * spirooffset, spirominy, spiromaxy);
    uint8_t x2 = EffectMath::mapsincos8(MAP_SIN, spirotheta2 + i * spirooffset, x - spiroradiusx, x + spiroradiusx);
    uint8_t y2 = EffectMath::mapsincos8(MAP_COS, spirotheta2 + i * spirooffset, y - spiroradiusy, y + spiroradiusy);
    CRGB color = ColorFromPalette(*curPalette, (spirohueoffset + i * spirooffset), 128U);
    if (x2<WIDTH && y2<HEIGHT){ // добавил проверки. не знаю, почему эффект подвисает без них
      CRGB tmpColor = myLamp.getPixColorXY(x2, y2);
      tmpColor += color;
      myLamp.setLeds(myLamp.getPixelNumber(x2, y2), tmpColor); // += color
    }

    if(x2 == spirocenterX && y2 == spirocenterY) change = true;
  }

  spirotheta2 += 2*speed_factor;

  EVERY_N_MILLIS(EFFECTS_RUN_TIMER/2) {
    spirotheta1 += 1*speed_factor;
  }

  EVERY_N_MILLIS(50) {
    if (change && !spirohandledChange) {
      spirohandledChange = true;

      if (spirocount >= WIDTH || spirocount == 1) spiroincrement = !spiroincrement;
      if (spiroincrement) {
        if(spirocount >= 4)
          spirocount *= 2;
        else
          spirocount += 1;
      } else {
        if(spirocount > 4)
          spirocount /= 2;
        else
            spirocount -= 1;
      }

      spirooffset = 256 / spirocount;
    }

    if(!change) spirohandledChange = false;
  }

  EVERY_N_MILLIS(33) {
      spirohueoffset += 1;
  }

  return true;
}

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
void EffectComet::drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color)
{ // Fine if: 0<x1<x2<WIDTH && 0<y1<y2<HEIGHT
  for (int8_t xP = x1; xP <= x2; xP++)
  {
    for (int8_t yP = y1; yP <= y2; yP++)
    {
      myLamp.setLeds(myLamp.getPixelNumber(xP, yP), color);
    }
  }
}

void EffectComet::FillNoise(int8_t layer) {
  const uint8_t e_centerX =  (WIDTH / 2) - 1;
  const uint8_t e_centerY = (HEIGHT / 2) - 1;

  for (uint8_t i = 0; i < WIDTH; i++) {
    int32_t ioffset = e_scaleX[layer] * (i - e_centerX);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      int32_t joffset = e_scaleY[layer] * (j - e_centerY);
      int8_t data = inoise16(e_x[layer] + ioffset, e_y[layer] + joffset, e_z[layer]) >> 8;
      int8_t olddata = noise3d[layer][i][j];
      int8_t newdata = scale8( olddata, eNs_noisesmooth ) + scale8( data, 255 - eNs_noisesmooth );
      data = newdata;
      noise3d[layer][i][j] = data;
    }
  }
}

void EffectComet::load(){
    eNs_noisesmooth = random(0, 200*(uint_fast16_t)speed/255); // степень сглаженности шума 0...200
}

bool EffectComet::run(CRGB *ledarr, const char *opt){
  switch (effect)
  {
  case EFF_ENUM::EFF_RAINBOWCOMET :
    return rainbowCometRoutine(*&ledarr, &*opt);
    break;
  case EFF_ENUM::EFF_RAINBOWCOMET3 :
    return rainbowComet3Routine(*&ledarr, &*opt);
    break;
  default:
    return false;
  }
}

bool EffectComet::rainbowCometRoutine(CRGB *leds, const char *param)
{ // Rainbow Comet by PalPalych
/*
  Follow the Rainbow Comet Efect by PalPalych
  Speed = tail dispersing
  Scale = 0 - Random 3d color
          1...127 - time depending color
          128...254 - selected color
          255 - white
*/

  myLamp.blur2d(e_com_BLUR);    // < -- размытие хвоста
  myLamp.dimAll(254);            // < -- затухание эффекта для последующего кадра
  CRGB _eNs_color = CRGB::White;
  if (scale <= 1) {
    _eNs_color = CHSV(noise3d[0][0][0] * e_com_3DCOLORSPEED , 255, 255);
  } else if (scale < 128) {
    _eNs_color = CHSV(millis() / ((uint16_t)scale + 1U) * 4 + 10, 255, 255);
  } else {
    _eNs_color = CHSV((scale - 128) * 2, 255, 255);
  }
  drawFillRect2_fast(e_centerX - 1, e_centerY - 1, e_centerX + 1, e_centerY + 1, _eNs_color);
  // Noise
  uint16_t sc = (uint16_t)speed * 30 + 500; //64 + 1000;
  uint16_t sc2 = (float)speed/127.0+1.5; //1.5...3.5;
  for(uint8_t i=0; i<NUM_LAYERS; i++){
    e_x[i] += e_com_TAILSPEED*sc2;
    e_y[i] += e_com_TAILSPEED*sc2;
    e_z[i] += e_com_TAILSPEED*sc2;
    e_scaleX[i] = sc; // 8000;
    e_scaleY[i] = sc; // 8000;
    FillNoise(i);
  }
  EffectMath::MoveFractionalNoise(MOVE_X, noise3d, WIDTH / 2U - 1U);
  EffectMath::MoveFractionalNoise(MOVE_Y, noise3d, HEIGHT / 2U - 1U);
  return true;
}

bool EffectComet::rainbowComet3Routine(CRGB *leds, const char *param)
{ // Rainbow Comet by PalPalych
/*
  Follow the Rainbow Comet Efect by PalPalych
  Speed = tail dispersing
  Scale = 0 - Random 3d color
          1...127 - time depending color
          128...254 - selected color
          255 - white
*/

  myLamp.blur2d(scale/5+1);    // < -- размытие хвоста
  myLamp.dimAll(255-scale/66);            // < -- затухание эффекта для последующего кадра
  byte xx = 2 + sin8( millis() / 10) / 22;
  byte yy = 2 + cos8( millis() / 9) / 22;
  myLamp.setLeds(myLamp.getPixelNumber( xx%WIDTH, yy%HEIGHT), 0x0000FF);

  xx = 4 + sin8( millis() / 10) / 32;
  yy = 4 + cos8( millis() / 7) / 32;
  myLamp.setLeds(myLamp.getPixelNumber( xx%WIDTH, yy%HEIGHT), 0xFF0000);
  myLamp.setLeds(myLamp.getPixelNumber( e_centerX, e_centerY), 0x00FF00);

  uint16_t sc = (uint16_t)speed * 30 + 500; //64 + 1000;
  uint16_t sc2 = (float)speed/127.0+1.5; //1.5...3.5;
  for(uint8_t i=0; i<NUM_LAYERS; i++){
    e_x[i] += 1500*sc2;
    e_y[i] += 1500*sc2;
    e_z[i] += 1500*sc2;
    e_scaleX[i] = sc; // 8000;
    e_scaleY[i] = sc; // 8000;
    FillNoise(i);
  }
  EffectMath::MoveFractionalNoise(MOVE_X, noise3d, 2);
  EffectMath::MoveFractionalNoise(MOVE_Y, noise3d, 2, 0.33);
  return true;
}

// ============= ЭФФЕКТ ПРИЗМАТА ===============
// Prismata Loading Animation
void EffectPrismata::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectPrismata::run(CRGB *ledarr, const char *opt){
  return prismataRoutine(*&ledarr, &*opt);
}

bool EffectPrismata::prismataRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  EVERY_N_MILLIS(100) {
    spirohueoffset += 1;
  }

  myLamp.blur2d(15);
  myLamp.dimAll(254U - scale);
  for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t y = beatsin8(x + 1 * speed/5, 0, HEIGHT-1);
      myLamp.drawPixelXY(x, y, ColorFromPalette(*curPalette, (x+spirohueoffset) * 4));
    }
  return true;
}

// ============= ЭФФЕКТ СТАЯ ===============
// Адаптация от (c) SottNick
void EffectFlock::load(){
  palettesload();    // подгружаем дефолтные палитры

  FastLED.clear();
  for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
    boids[i] = Boid(15, 15);
    boids[i].maxspeed = 0.380*speed/127.0+0.380/2;
    boids[i].maxforce = 0.015*speed/127.0+0.015/2;
  }
  predatorPresent = random(0, 2) >= 1;
  if (predatorPresent) {
    predator = Boid(31, 31);
    predatorPresent = true;
    predator.maxspeed = 0.385*speed/127.0+0.385/2;
    predator.maxforce = 0.020*speed/127.0+0.020/2;
    predator.neighbordist = 8.0;
    predator.desiredseparation = 0.0;
  }

}

bool EffectFlock::run(CRGB *ledarr, const char *opt){
  return flockRoutine(*&ledarr, &*opt);
}

bool EffectFlock::flockRoutine(CRGB *leds, const char *param) {
  if (curPalette == nullptr) {
    return false;
  }

  myLamp.blur2d(15);
  myLamp.dimAll(254U - scale);
  bool applyWind = random(0, 255) > 240;
  if (applyWind) {
    wind.x = Boid::randomf() * .015 * speed/127.0 + .015/2;
    wind.y = Boid::randomf() * .015 * speed/127.0 + .015/2;
  }
  CRGB color = ColorFromPalette(*curPalette, hueoffset);

  for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
    Boid * boid = &boids[i];
    if (predatorPresent) {
          // flee from predator
          boid->repelForce(predator.location, 8);
        }
    boid->run(boids, AVAILABLE_BOID_COUNT);
    boid->wrapAroundBorders();
    PVector location = boid->location;
    // PVector velocity = boid->velocity;
        // backgroundLayer.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);
        // effects.leds[XY(location.x, location.y)] += color;
    myLamp.drawPixelXY(location.x, location.y, color);
    if (applyWind) {
          boid->applyForce(wind);
          applyWind = false;
        }
  }
  if (predatorPresent) {
    predator.run(boids, AVAILABLE_BOID_COUNT);
    predator.wrapAroundBorders();
    color = ColorFromPalette(*curPalette, hueoffset + 128);
    PVector location = predator.location;
    // PVector velocity = predator.velocity;
        // backgroundLayer.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);
        // effects.leds[XY(location.x, location.y)] += color;
    myLamp.drawPixelXY(location.x, location.y, color);
  }
  EVERY_N_MILLIS(333) {
        hueoffset += 1;
  }

  EVERY_N_SECONDS(30) {
        predatorPresent = !predatorPresent;
      }
  return true;
}

// ============= SWIRL /  ВОДОВОРОТ ===============
// Prismata Loading Animation
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither by PalPalych 12.04.2020
// Aurora: https://github.com/pixelmatix/aurora
// Copyright (c) 2014 Jason Coon
void EffectSwirl::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectSwirl::run(CRGB *ledarr, const char *opt){
  return swirlRoutine(*&ledarr, &*opt);
}

#define e_swi_BORDER (1U)  // размытие экрана за активный кадр
bool EffectSwirl::swirlRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
#if (WIDTH < 25)
  myLamp.blur2d(beatsin8(2, 10, 128 + scale*3));
#else
  // Never mind, on my 64x96 array, the dots are just too small
   myLamp.blur2d(172);
#endif

  // Use two out-of-sync sine waves
  uint8_t i = beatsin8(27*(speed/100.0)+5, e_swi_BORDER, HEIGHT - e_swi_BORDER); // borderWidth
  uint8_t j = beatsin8(41*(speed/100.0)+5, e_swi_BORDER, WIDTH - e_swi_BORDER);
  // Also calculate some reflections
  uint8_t ni = (WIDTH - 1U)-i;
  uint8_t nj = (WIDTH - 1U)-j;

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();
  myLamp.setLeds(myLamp.getPixelNumber(i, j),CRGB(myLamp.getPixColorXY(i, j)) + ColorFromPalette(*curPalette, ms / 11));
  myLamp.setLeds(myLamp.getPixelNumber(j, i),CRGB(myLamp.getPixColorXY(j, i)) + ColorFromPalette(*curPalette, ms / 13));
  myLamp.setLeds(myLamp.getPixelNumber(ni, nj),CRGB(myLamp.getPixColorXY(ni, nj)) + ColorFromPalette(*curPalette, ms / 17));
  myLamp.setLeds(myLamp.getPixelNumber(nj, ni),CRGB(myLamp.getPixColorXY(nj, ni)) + ColorFromPalette(*curPalette, ms / 29));
  myLamp.setLeds(myLamp.getPixelNumber(i, nj),CRGB(myLamp.getPixColorXY(i, nj)) + ColorFromPalette(*curPalette, ms / 37));
  myLamp.setLeds(myLamp.getPixelNumber(ni, j),CRGB(myLamp.getPixColorXY(ni, j)) + ColorFromPalette(*curPalette, ms / 41));

  return true;
}

// ============= DRIFT / ДРИФТ ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h

void EffectDrift::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectDrift::run(CRGB *ledarr, const char *opt){
  myLamp.blur2d(beatsin8(3U, 5, 10 + scale*3));
  myLamp.dimAll(beatsin8(2U, 246, 252));
  _dri_speed = map8(speed, 1U, 20U);
  _dri_delta = beatsin8(1U);
  //EVERY_N_MILLIS(13){
    dri_phase++;    // 13 ms это примерно каждый кадр и есть
  //}

  switch (effect)
  {
  case EFF_ENUM::EFF_DRIFT :
    return incrementalDriftRoutine(*&ledarr, &*opt);
    break;
  case EFF_ENUM::EFF_DRIFT2 :
    return incrementalDriftRoutine2(*&ledarr, &*opt);
    break;
  default:
    return false;
  }
}

bool EffectDrift::incrementalDriftRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  for (uint8_t i = 1; i < WIDTH / 2U; i++) // возможно, стоит здесь использовать const MINLENGTH
  {
    int8_t x = beatsin8((CENTER_max - i) * _dri_speed, WIDTH / 2U - 1 - i, WIDTH / 2U - 1 + 1U + i, 0, 64U + dri_phase); // используем константы центра матрицы из эффекта Кометы
    int8_t y = beatsin8((CENTER_max - i) * _dri_speed, WIDTH / 2U - 1 - i, WIDTH / 2U - 1 + 1U + i, 0, dri_phase);       // используем константы центра матрицы из эффекта Кометы
    myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, (i - 1U) * WIDTH_steps * 2U + _dri_delta) ); // используем массив палитр из других эффектов выше
  }
  return true;
}

// ============= DRIFT 2 / ДРИФТ 2 ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift2.h
bool EffectDrift::incrementalDriftRoutine2(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    int8_t x = 0;
    int8_t y = 0;
    CRGB color;
    if (i < WIDTH / 2U)
    {
      x = beatsin8((i + 1) * _dri_speed, i + 1U, WIDTH - 1 - i, 0, 64U + dri_phase);
      y = beatsin8((i + 1) * _dri_speed, i + 1U, HEIGHT - 1 - i, 0, dri_phase);
      color = ColorFromPalette(*curPalette, i * WIDTH_steps * 2U + _dri_delta);
    }
    else
    {
      x = beatsin8((WIDTH - i) * _dri_speed, WIDTH - 1 - i, i + 1U, 0, dri_phase);
      y = beatsin8((HEIGHT - i) * _dri_speed, HEIGHT - 1 - i, i + 1U, 0, 64U + dri_phase);
      color = ColorFromPalette(*curPalette, ~(i * WIDTH_steps * 2U + _dri_delta));
    }
    myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
  }
  return true;
}

// Частотный (спектральный) анализатор
#ifdef MIC_EFFECTS
void EffectFreq::load()
{
  palettesload();    // подгружаем дефолтные палитры
  memset(peakX,0,sizeof(peakX));
}

bool EffectFreq::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  myLamp.setMicAnalyseDivider(0); // отключить авто-работу микрофона, т.к. тут все анализируется отдельно, т.е. не нужно выполнять одну и ту же работу дважды
  return freqAnalyseRoutine(*&ledarr, &*opt);
}

bool EffectFreq::freqAnalyseRoutine(CRGB *leds, const char *param)
{
  float samp_freq;
  double last_freq;
  uint8_t last_min_peak, last_max_peak;
  float x[WIDTH+1]; memset(x,0,sizeof(x));
  float maxVal;
  uint8_t freqDiv = 2U-scale/128; //1...2

  MICWORKER *mw = new MICWORKER(myLamp.getMicScale(),myLamp.getMicNoise());

  samp_freq = mw->process(myLamp.getMicNoiseRdcLevel()); // частота семплирования
  last_min_peak = mw->getMinPeak();
  last_max_peak = mw->getMaxPeak()*2;

  maxVal=mw->fillSizeScaledArray(x,WIDTH/freqDiv);

  float _scale = (maxVal==0? 0 : last_max_peak/maxVal);
  _scale = _scale * ((scale%128)/127.0);

// #ifdef LAMP_DEBUG
// EVERY_N_SECONDS(1){
//   for(uint8_t i=0; i<WIDTH/freqDiv; i++)
//     LOG(printf_P,PSTR("%5.2f "),x[i]);
//   LOG(printf_P,PSTR("F: %8.2f SC: %5.2f\n"),x[WIDTH/freqDiv], scale);
// }
// #endif

  TProgmemRGBPalette16 const *_curPalette;
  float _ptPallete; // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
  uint8_t _pos; // позиция в массиве указателей паллитр
  uint8_t _curVal; // curVal == либо var как есть, либо getScale
  if(rval){
    _ptPallete = ptPallete;
    _pos = palettepos;
    _curVal = rval;
  } else {
    _ptPallete = ptPallete/2.0;
    _pos = (uint8_t)((float)(scale%128)/_ptPallete);
    _curVal = scale%128;
  }

  // EVERY_N_SECONDS(1){
  //   LOG(printf_P,PSTR("palettepos: %d\n"),_pos);
  // }

  _curPalette = palettes.at(_pos); // выбираем из доп. регулятора

  for(uint8_t xpos=0; xpos<WIDTH/freqDiv; xpos++){
    for(uint8_t ypos=0; ypos<HEIGHT; ypos++){
      uint32_t color = (x[xpos]*_scale*(1.0/(ypos+1)))>5?255:0;
      if(color==255 && peakX[1][xpos] < ypos){
        peakX[1][xpos]=ypos;
        peakX[0][xpos]=10;
      }
      if(ypos>(1.5*HEIGHT/2.0)){
        color=color<<16;
      } else if(ypos>(HEIGHT/2.0)){
        color=(color<<8)|(color<<16);
      } else {
        //color=color<<8;
        if(color){
          CRGB tColor;
          if(!(_curVal%(uint8_t)(_ptPallete*(_pos+1)))) // для крайней точки рандом, иначе возьмем по индексу/2
            tColor = ColorFromPalette(*_curPalette,random8(15)); // sizeof(TProgmemRGBPalette16)/sizeof(uint32_t)
          else
            tColor = ColorFromPalette(*_curPalette,constrain(ypos,0,15)); // sizeof(TProgmemRGBPalette16)/sizeof(uint32_t)
          color=((unsigned long)tColor.r<<16)+((unsigned long)tColor.g<<8)+(unsigned long)tColor.b; // извлекаем и конвертируем цвет :)
        }
      }
      myLamp.setLeds(myLamp.getPixelNumber(freqDiv*xpos,ypos), color);
      if(freqDiv>1)
        myLamp.setLeds(myLamp.getPixelNumber(freqDiv*xpos+1,ypos), color);
    }
  }

  bool isfall=false;
  EVERY_N_MILLISECONDS(70){
    isfall = true;
  }

  for (size_t i = 0; i < WIDTH/freqDiv; i++)
  {
    uint32_t color = 255;
    int8_t &ypos=peakX[1][i];
    if(peakX[0][i])
      peakX[0][i]--;
    if(isfall && ypos>0 && !peakX[0][i]) ypos--;

    if(ypos>(1.5*HEIGHT/2.0)){
      color=color<<16;
    } else if(ypos>(HEIGHT/2.0)){
      color=(color<<8)|(color<<16);
    } else {
      //color=color<<8;
      if(color){
          CRGB tColor;
          if(!(_curVal%(uint8_t)(_ptPallete*(_pos+1)))) // для крайней точки рандом, иначе возьмем по индексу/2
            tColor = ColorFromPalette(*_curPalette,random8(15)); // sizeof(TProgmemRGBPalette16)/sizeof(uint32_t)
          else
            tColor = ColorFromPalette(*_curPalette,constrain(ypos,0,15)); // sizeof(TProgmemRGBPalette16)/sizeof(uint32_t)
          color=((unsigned long)tColor.r<<16)+((unsigned long)tColor.g<<8)+(unsigned long)tColor.b; // извлекаем и конвертируем цвет :)
      }
    }
    myLamp.setLeds(myLamp.getPixelNumber(freqDiv*i,ypos), color);
    if(freqDiv>1)
      myLamp.setLeds(myLamp.getPixelNumber(freqDiv*i+1,ypos), color);
  }

  samp_freq = samp_freq; last_min_peak=last_min_peak; last_freq=last_freq; // давим варнинги
  delete mw;
  return true;
}
#endif

// ------------------------------ ЭФФЕКТ МЕРЦАНИЕ ----------------------
// (c) SottNick

#define TWINKLES_SPEEDS 4     // всего 4 варианта скоростей мерцания
#define TWINKLES_MULTIPLIER 24 // слишком медленно, если на самой медленной просто по единичке добавлять

void EffectTwinkles::load(){
  palettesload();    // подгружаем дефолтные палитры

  tnum = palettescale;

  for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
    if (random8(tnum) == 0){                // я не понял что это значит
      ledsbuff[idx].r = random8();                          // оттенок пикселя
      ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS * 2 +1); // скорость и направление (нарастает 1-4 или угасает 5-8)
      ledsbuff[idx].b = random8();                          // яркость
    }
    else
      ledsbuff[idx] = 0;                                    // всё выкл
  }

}

bool EffectTwinkles::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  return twinklesRoutine(*&ledarr, &*opt);
}

bool EffectTwinkles::twinklesRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  scale = TWINKLES_MULTIPLIER*speed/255.0;  // почему масштаб зависит от скорости? и какая величина будет перекрывать другую...

/*
  // тоже не понял что это... если была переменная R, то нужно что-то обсчитать относительно бегунка шкалы?
  if(!var.isEmpty()){
    tnum = 50-49*(scale/255.0);
  }
*/

    for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
      if (ledsbuff[idx].b == 0){
        if (random8(tnum) == 0 && thue > 0){  // если пиксель ещё не горит, зажигаем каждый ХЗй
          ledsbuff[idx].r = random8();                          // оттенок пикселя
          ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS +1);     // скорость и направление (нарастает 1-4, но не угасает 5-8)
          ledsbuff[idx].b = ledsbuff[idx].g;                    // яркость
          thue--; // уменьшаем количество погасших пикселей
        }
      }
      else if (ledsbuff[idx].g <= TWINKLES_SPEEDS){             // если нарастание яркости
        if (ledsbuff[idx].b > 255U - ledsbuff[idx].g - scale){            // если досигнут максимум
          ledsbuff[idx].b = 255U;
          ledsbuff[idx].g = ledsbuff[idx].g + TWINKLES_SPEEDS;
        }
        else
          ledsbuff[idx].b = ledsbuff[idx].b + ledsbuff[idx].g + scale;
      }
      else {                                                    // если угасание яркости
        if (ledsbuff[idx].b <= ledsbuff[idx].g - TWINKLES_SPEEDS + scale){// если досигнут минимум
          ledsbuff[idx].b = 0;                                  // всё выкл
          thue++; // считаем количество погасших пикселей
        }
        else
          ledsbuff[idx].b = ledsbuff[idx].b - ledsbuff[idx].g + TWINKLES_SPEEDS - scale;
      }
      if (ledsbuff[idx].b == 0)
        myLamp.setLeds(idx, 0U);
      else
        myLamp.setLeds(idx, ColorFromPalette(*curPalette, ledsbuff[idx].r, ledsbuff[idx].b));
    }

  return true;
}

// ============= RADAR / РАДАР ===============
// Aurora : https://github.com/pixelmatix/aurora/blob/master/PatternRadar.h
// Copyright(c) 2014 Jason Coon
// v1.0 - Updating for GuverLamp v1.7 by Palpalych 14.04.2020
// v1.1 - +dither, +smoothing
void EffectRadar::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectRadar::run(CRGB *ledarr, const char *opt){
  return radarRoutine(*&ledarr, &*opt);
}

bool EffectRadar::radarRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  myLamp.blur2d(beatsin8(5U, 3U, 10U));
  myLamp.dimAll(map(scale, 1, 255, 170, 255));

  for (uint8_t offset = 0U; offset < WIDTH / 2U - 1U; offset++)
  {
    myLamp.setLeds(myLamp.getPixelNumber(EffectMath::mapsincos8(MAP_COS, eff_theta, offset, (WIDTH - 1U)-offset),
                   EffectMath::mapsincos8(MAP_SIN, eff_theta, offset, (WIDTH - 1U)-offset)),
                   ColorFromPalette(*curPalette, 255U - (offset * 16U + eff_offset)));

    EVERY_N_MILLIS(24)
    {
      eff_theta += 5.5*((speed)/255.0)+1;
      eff_offset += 3.5*((255-speed)/255.0)+1;
    }
  }
  return true;
}

// ============= WAVES /  ВОЛНЫ ===============
// Prismata Loading Animation
// v1.0 - Updating for GuverLamp v1.7 by SottNick 11.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternWave.h
// Адаптация от (c) SottNick
void EffectWaves::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectWaves::run(CRGB *ledarr, const char *opt){

  waveCount = speed % 2;
  waveRotation = palettescale/8;  // тут ерунда какая-то...
                                  //  waveRotation нужно привести к 0...3 (из них 2 повторяются, чет/нечет?) почему делитель - 8?
                                  //  верно ли тут выбирается не весь бегунок шкалы, а приведенная шкала в рамках палитры?
  return wavesRoutine(*&ledarr, &*opt);
}

bool EffectWaves::wavesRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  myLamp.blur2d(20); // @Palpalych советует делать размытие. вот в этом эффекте его явно не хватает...
  myLamp.dimAll(254);

  int n = 0;
  switch (waveRotation)
  {
  case 0:
  case 2:
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      n = quadwave8(x * 2 + waveTheta) / waveScale;
      myLamp.drawPixelXY(x, n, ColorFromPalette(*curPalette, whue + x));
      if (waveCount != 1)
        myLamp.drawPixelXY(x, HEIGHT - 1 - n, ColorFromPalette(*curPalette, whue + x));
    }
    break;

  case 1:
  case 3:
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
      n = quadwave8(y * 2 + waveTheta) / waveScale;
      myLamp.drawPixelXY(n, y, ColorFromPalette(*curPalette, whue + y));
      if (waveCount != 1)
        myLamp.drawPixelXY(WIDTH - 1 - n, y, ColorFromPalette(*curPalette, whue + y));
    }
    break;
  }

  waveTheta+=5*(speed/255.0)+1.0;
  whue+=speed/10.0+1;
  return true;
}

// ============= FIRE 2012 /  ОГОНЬ 2012 ===============
// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
/*
 * Эффект "Огонь 2012"
 */
void EffectFire2012::load(){
  // собираем свой набор палитр для эффекта
  palettes.reserve(NUMPALETTES);
  palettes.push_back(&HeatColors2_p);
  palettes.push_back(&WoodFireColors_p);
  palettes.push_back(&NormalFire_p);
  palettes.push_back(&NormalFire2_p);
  palettes.push_back(&LithiumFireColors_p);
  palettes.push_back(&SodiumFireColors_p);
  palettes.push_back(&CopperFireColors_p);
  palettes.push_back(&AlcoholFireColors_p);
  palettes.push_back(&RubidiumFireColors_p);
  palettes.push_back(&PotassiumFireColors_p);

  usepalettes = true; // активируем "переключатель" палитр
  scale2pallete();    // выбираем палитру согласно "шкале"

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(256));
}

bool EffectFire2012::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  return fire2012Routine(*&ledarr, &*opt);
}

bool EffectFire2012::fire2012Routine(CRGB *ledarr, const char *opt)
{
  if (curPalette == nullptr) {
    return false;
  }

#if HEIGHT / 6 > 6
  #define FIRE_BASE 6
#else
  #define FIRE_BASE HEIGHT / 6 + 1
#endif

  // Loop for each column individually
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    // Step 1.  Cool down every cell a little
    for (uint8_t i = 0; i < HEIGHT; i++)
    {
      noise3d[0][x][i] = qsub8(noise3d[0][x][i], random(0, ((cooling * 10) / HEIGHT) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint8_t k = HEIGHT; k > 1; k--)
    {
      noise3d[0][x][wrapY(k)] = (noise3d[0][x][k - 1] + noise3d[0][x][wrapY(k - 2)] + noise3d[0][x][wrapY(k - 2)]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < sparking)
    {
      int j = random(FIRE_BASE);
      noise3d[0][x][j] = qadd8(noise3d[0][x][j], random(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
      // Blend new data with previous frame. Average data between neighbouring pixels
      if(curPalette!=palettes.at(0)){
        nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, y)], ColorFromPalette(HeatColors2_p, ((noise3d[0][x][y] * 0.7) + (noise3d[0][wrapX(x + 1)][y] * 0.3))), fireSmoothing);
      }
      nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, y)], ColorFromPalette(*curPalette, ((noise3d[0][x][y] * 0.7) + (noise3d[0][wrapX(x + 1)][y] * 0.3))), fireSmoothing);
    }
  }
  return true;
}

// ============= RAINS/LEDS/THUNDERS IN THE CAN /  ОСАДКИ/ТУЧКА/ГРОЗА В БАНКЕ ===============
// https://github.com/marcmerlin/FastLED_NeoMatrix_SmartMatrix_LEDMatrix_GFX_Demos/blob/master/FastLED/Sublime_Demos/Sublime_Demos.ino
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
// там по ссылке ещё остались эффекты с 3 по 9 (в SimplePatternList перечислены)

//прикольная процедура добавляет блеск почти к любому эффекту после его отрисовки https://www.youtube.com/watch?v=aobtR1gIyIo
//void addGlitter( uint8_t chanceOfGlitter){
//  if ( random8() < chanceOfGlitter) leds[ random16(NUM_LEDS) ] += CRGB::White;
//static uint8_t intensity = 42;  // будет бегунок масштаба
// Array of temp cells (used by fire, theMatrix, coloredRain, stormyRain)
// uint8_t **tempMatrix; = noise3d[0][WIDTH][HEIGHT]
// uint8_t *splashArray; = line[WIDTH] из эффекта Огонь
bool EffectRain::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  EVERY_N_SECONDS(3){
    LOG(printf, "speed: %d, scale: %d\n", speed, scale);
  }

  switch (effect)
  {
  case EFF_ENUM::EFF_RAIN :
    return simpleRainRoutine(*&ledarr, &*opt);
    break;
  case EFF_ENUM::EFF_COLORRAIN :
    return coloredRainRoutine(*&ledarr, &*opt);
    break;
  case EFF_ENUM::EFF_STORMYRAIN :
    return stormyRainRoutine(*&ledarr, &*opt);
    break;
  default:
    return false;
  }
}

void EffectRain::rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm, bool fixRC)
{
  // static uint16_t noiseX = random16();
  // static uint16_t noiseY = random16();
  // static uint16_t noiseZ = random16();
  // CRGB solidRainColor = CRGB(60, 80, 90);

  CRGB lightningColor = CRGB(72, 72, 80);
  CRGBPalette16 rain_p(CRGB::Black, rainColor);
#ifdef SMARTMATRIX
  CRGBPalette16 rainClouds_p(CRGB::Black, CRGB(75, 84, 84), CRGB(49, 75, 75), CRGB::Black);
#else
  CRGBPalette16 rainClouds_p(CRGB::Black, CRGB(15, 24, 24), CRGB(9, 15, 15), CRGB::Black);
#endif


  //fadeToBlackBy(leds, NUM_LEDS, 255 - tailLength);
  nscale8(myLamp.getUnsafeLedsArray(), NUM_LEDS, tailLength);

  // Loop for each column individually
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    // Step 1.  Move each dot down one cell
    for (uint8_t i = 0; i < HEIGHT; i++)
    {
      if (noise3d[0][x][i] >= backgroundDepth)
      { // Don't move empty cells
        if (i > 0)
          noise3d[0][x][wrapY(i - 1)] = noise3d[0][x][i];
        noise3d[0][x][i] = 0;
      }
    }

    // Step 2.  Randomly spawn new dots at top
    if (random(255) < spawnFreq)
    {
      noise3d[0][x][HEIGHT - 1] = random(backgroundDepth, maxBrightness);
    }

    // Step 3. Map from tempMatrix cells to LED colors
    //uint32_t color = CRGB::Black;
    for (int16_t y = HEIGHT-1; y >= 0; y--)
    {
      // if(color == CRGB::Black && myLamp.getPixColor(myLamp.getPixelNumber(x, y)) && y!=(HEIGHT-1))
      //   color = myLamp.getPixColor(myLamp.getPixelNumber(x, y));
      // else if(!myLamp.getPixColor(myLamp.getPixelNumber(x, y)) && y!=(HEIGHT-1))
      //    color = CRGB::Black;

      if (noise3d[0][x][y] >= backgroundDepth)
      { // Don't write out empty cells
        // if(fixRC && color!=CRGB::Black){
        //   myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
        // }
        // else if(fixRC && y==(HEIGHT-1) && color==CRGB::Black)
        //   myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(rain_p, noise3d[0][x][y]));
        // else if(!fixRC)
          myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(rain_p, noise3d[0][x][y]));
      }
    }
    //color = CRGB::Black;

    // Step 4. Add splash if called for
    if (splashes)
    {
      // FIXME, this is broken
      byte j = nline[x];
      byte v = noise3d[0][x][0];

      if (j >= backgroundDepth)
      {
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x - 2), 0), ColorFromPalette(rain_p, j / 3));
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x + 2), 0), ColorFromPalette(rain_p, j / 3));
        nline[x] = 0; // Reset splash
      }

      if (v >= backgroundDepth)
      {
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x - 1), 1), ColorFromPalette(rain_p, v / 2));
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x + 1), 1), ColorFromPalette(rain_p, v / 2));
        nline[x] = v; // Prep splash for next frame
      }
    }

    // Step 5. Add lightning if called for
    if (storm)
    {
      //uint8_t lightning[WIDTH][HEIGHT];
      // ESP32 does not like static arrays  https://github.com/espressif/arduino-esp32/issues/2567
      uint8_t *lightning = (uint8_t *)malloc(WIDTH * HEIGHT);
      while (lightning == NULL)
      {
        Serial.println("lightning malloc failed");
      }

      if (random16() < 72)
      {                                                                       // Odds of a lightning bolt
        lightning[scale8(random8(), WIDTH - 1) + (HEIGHT - 1) * WIDTH] = 255; // Random starting location
        for (uint8_t ly = HEIGHT - 1; ly > 1; ly--)
        {
          for (uint8_t lx = 1; lx < WIDTH - 1; lx++)
          {
            if (lightning[lx + ly * WIDTH] == 255)
            {
              lightning[lx + ly * WIDTH] = 0;
              uint8_t dir = random8(4);
              switch (dir)
              {
              case 0:
                myLamp.setLeds(myLamp.getPixelNumber(lx + 1, ly - 1), lightningColor);
                lightning[(lx + 1) + (ly - 1) * WIDTH] = 255; // move down and right
                break;
              case 1:
                myLamp.setLeds(myLamp.getPixelNumber(lx, ly - 1), CRGB(128, 128, 128)); // я без понятия, почему у верхней молнии один оттенок, а у остальных - другой
                lightning[lx + (ly - 1) * WIDTH] = 255;     // move down
                break;
              case 2:
                myLamp.setLeds(myLamp.getPixelNumber(lx - 1, ly - 1), CRGB(128, 128, 128));
                lightning[(lx - 1) + (ly - 1) * WIDTH] = 255; // move down and left
                break;
              case 3:
                myLamp.setLeds(myLamp.getPixelNumber(lx - 1, ly - 1), CRGB(128, 128, 128));
                lightning[(lx - 1) + (ly - 1) * WIDTH] = 255; // fork down and left
                myLamp.setLeds(myLamp.getPixelNumber(lx - 1, ly - 1), CRGB(128, 128, 128));
                lightning[(lx + 1) + (ly - 1) * WIDTH] = 255; // fork down and right
                break;
              }
            }
          }
        }
      }
      free(lightning);
    }

    // Step 6. Add clouds if called for
    if (clouds)
    {
      uint16_t noiseScale = 250; // A value of 1 will be so zoomed in, you'll mostly see solid colors. A value of 4011 will be very zoomed out and shimmery
      const uint16_t cloudHeight = (HEIGHT * 0.2) + 1;

      // This is the array that we keep our computed noise values in
      //static uint8_t noise[WIDTH][cloudHeight];
      static uint8_t *noise = (uint8_t *)malloc(WIDTH * cloudHeight);
      while (noise == NULL)
      {
        Serial.println("noise malloc failed");
      }
      int xoffset = noiseScale * x + rhue;

      for (int z = 0; z < cloudHeight; z++)
      {
        int yoffset = noiseScale * z - rhue;
        uint8_t dataSmoothing = 192;
        uint8_t noiseData = qsub8(inoise8(noiseX + xoffset, noiseY + yoffset, noiseZ), 16);
        noiseData = qadd8(noiseData, scale8(noiseData, 39));
        noise[x * cloudHeight + z] = scale8(noise[x * cloudHeight + z], dataSmoothing) + scale8(noiseData, 256 - dataSmoothing);
        nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, HEIGHT - z - 1)], ColorFromPalette(rainClouds_p, noise[x * cloudHeight + z]), (cloudHeight - z) * (250 / cloudHeight));
      }
      noiseZ++;
    }
  }
}

uint8_t EffectRain::myScale8(uint8_t x)
{ // даёт масштабировать каждые 8 градаций (от 0 до 7) бегунка Масштаб в значения от 0 до 255 по типа синусоиде
  uint8_t x8 = x % 8U;
  uint8_t x4 = x8 % 4U;
  if (x4 == 0U)
    if (x8 == 0U)
      return 0U;
    else
      return 255U;
  else if (x8 < 4U)
    return (1U + x4 * 72U); // всего 7шт по 36U + 3U лишних = 255U (чтобы восхождение по синусоиде не было зеркально спуску)
                            //else
  return (253U - x4 * 72U); // 253U = 255U - 2U
}

bool EffectRain::coloredRainRoutine(CRGB *leds, const char *param) // внимание! этот эффект заточен на работу бегунка Масштаб в диапазоне от 0 до 255. пока что единственный.
{
  CRGB solidRainColor = CRGB(60, 80, 90);
  CRGB randomRainColor = CHSV(random(1,255), 255U, 255U);
  // я хз, как прикрутить а 1 регулятор и длину хвостов и цвет капель
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,5,100), 195, CRGB::Green, false, false, false); // было CRGB::Green

  if (scale > 255U-8U)
    rain(60, 200, map8(42, 5, 100), (31*(scale%8)), solidRainColor, false, false, false);
  else if (scale > 255U-16U)
    rain(60, 200, map8(42, 5, 100), (31*(scale%8)), randomRainColor, false, false, false, true);
  else
    rain(60, 200, map8(42, 5, 100), (31*(scale%8)), CHSV(scale, 255U, 255U), false, false, false);

  return true;
}

bool EffectRain::simpleRainRoutine(CRGB *leds, const char *param)
{
  CRGB solidRainColor = CRGB(60, 80, 90);
  //uint8_t Scale = scale;
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,2,60), 10, solidRainColor, true, true, false);
  rain(60, 180, scale, 30, solidRainColor, true, true, false);
  return true;
}

bool EffectRain::stormyRainRoutine(CRGB *leds, const char *param)
{
  CRGB solidRainColor = CRGB(60, 80, 90);
  //uint8_t Scale = scale;
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(0, 90, map8(intensity,0,150)+60, 10, solidRainColor, true, true, true);
  rain(60, 160, scale, 30, solidRainColor, true, true, true);
  return true;
}


// ============= FIRE 2018 /  ОГОНЬ 2018 ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
// https://gist.github.com/StefanPetrick/819e873492f344ebebac5bcd2fdd8aa8
// https://gist.github.com/StefanPetrick/1ba4584e534ba99ca259c1103754e4c5
bool EffectFire2018::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;
  return fire2018Routine(*&ledarr, &*opt);
}

bool EffectFire2018::fire2018Routine(CRGB *leds, const char *param)
{
  // some changing values
  uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
  uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
  uint16_t ctrl = ((ctrl1 + ctrl2) / 2);

  // parameters for the heatmap
  uint16_t _speed = 25;     // speed пересекается с переменной в родительском классе
  noise32_x[0] = 3 * ctrl * _speed;
  noise32_y[0] = 20 * millis() * _speed;
  noise32_z[0] = 5 * millis() * _speed;
  scale32_x[0] = ctrl1 / 2;
  scale32_y[0] = ctrl2 / 2;

  //calculate the noise data
  uint8_t layer = 0;

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    uint32_t ioffset = scale32_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint32_t joffset = scale32_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(noise32_x[layer] + ioffset, noise32_y[layer] + joffset, noise32_z[layer])) + 1);
      noise3dx[layer][i][j] = data >> 8;
    }
  }

  // parameters for te brightness mask
  _speed = 20;
  noise32_x[1] = 3 * ctrl * _speed;
  noise32_y[1] = 20 * millis() * _speed;
  noise32_z[1] = 5 * millis() * _speed;
  scale32_x[1] = ctrl1 / 2;
  scale32_y[1] = ctrl2 / 2;

  //calculate the noise data
  layer = 1;
  for (uint8_t i = 0; i < WIDTH; i++)
  {
    uint32_t ioffset = scale32_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint32_t joffset = scale32_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(noise32_x[layer] + ioffset, noise32_y[layer] + joffset, noise32_z[layer])) + 1);
      noise3dx[layer][i][j] = data >> 8;
    }
  }

  // draw lowest line - seed the fire
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    fire18heat[myLamp.getPixelNumber(x, HEIGHT - 1)] = noise3dx[0][WIDTH - 1 - x][CentreY - 1]; // хз, почему взято с середины. вожможно, нужно просто с 7 строки вне зависимости от высоты матрицы
  }

  //copy everything one line up
  for (uint8_t y = 0; y < HEIGHT - 1; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      fire18heat[myLamp.getPixelNumber(x, y)] = fire18heat[myLamp.getPixelNumber(x, y + 1)];
    }
  }

  //dim
  for (uint8_t y = 0; y < HEIGHT - 1; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      uint8_t dim = noise3dx[0][x][y];
      // high value = high flames
      dim = dim / 1.7 * constrain(0.05*myLamp.effects.getBrightness()+0.01,0.01,1.0);
      dim = 255 - dim;
      fire18heat[myLamp.getPixelNumber(x, y)] = scale8(fire18heat[myLamp.getPixelNumber(x, y)], dim);
    }
  }

  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      // map the colors based on heatmap
      //myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), CRGB( fire18heat[myLamp.getPixelNumber(x, y)], 1 , 0));
      //myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), CRGB( fire18heat[myLamp.getPixelNumber(x, y)], fire18heat[myLamp.getPixelNumber(x, y)] * 0.153, 0));
      //myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), CHSV(Scale, 255-fire18heat[myLamp.getPixelNumber(x, y)], constrain(fire18heat[myLamp.getPixelNumber(x, y)]*10,1,255)));
      //nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, HEIGHT - 1 - y)], CRGB( fire18heat[myLamp.getPixelNumber(x, y)], fire18heat[myLamp.getPixelNumber(x, y)] * 0.153, 0), 200);
      CRGB color = CRGB(fire18heat[myLamp.getPixelNumber(x, y)], (float)fire18heat[myLamp.getPixelNumber(x, y)] * (scale/5.0) * 0.01, 0); color*=2.5;
      myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), color);

      // dim the result based on 2nd noise layer
      myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, HEIGHT - 1 - y)].nscale8(noise3dx[1][x][y]);
    }
  }
  return true;
}

// ------------------------------ ЭФФЕКТ КОЛЬЦА / КОДОВЫЙ ЗАМОК ----------------------
// (c) SottNick
// из-за повторного использоваия переменных от других эффектов теперь в этом коде невозможно что-то понять.
// поэтому для понимания придётся сперва заменить названия переменных на человеческие. но всё равно это песец, конечно.

//uint8_t ringColor[HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
//uint8_t huePos[HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
//uint8_t shiftHueDir[HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
////ringHueShift[ringsCount]; // шаг градиета оттенка внутри кольца -8 - +8 случайное число
////ringHueShift2[ringsCount]; // обычная скорость переливания оттенка всего кольца -8 - +8 случайное число
//uint8_t currentRing; // кольцо, которое в настоящий момент нужно провернуть
//uint8_t stepCount; // оставшееся количество шагов, на которое нужно провернуть активное кольцо - случайное от WIDTH/5 до WIDTH-3
bool EffectRingsLock::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  ringsSet();
  return ringsRoutine(*&ledarr, &*opt);
}

// Установка параметров колец
void EffectRingsLock::ringsSet(){
  ringWidth = scale%11U + 1U; // толщина кольца от 1 до 11 для каждой из палитр
  ringNb = (float)HEIGHT / ringWidth + ((HEIGHT % ringWidth == 0U) ? 0U : 1U)%HEIGHT; // количество колец
  upRingHue = ringWidth - (ringWidth * ringNb - HEIGHT) / 2U; // толщина верхнего кольца. может быть меньше нижнего
  downRingHue = HEIGHT - upRingHue - (ringNb - 2U) * ringWidth; // толщина нижнего кольца = всё оставшееся
}

void EffectRingsLock::load(){
  palettesload();    // подгружаем дефолтные палитры

  ringsSet();
  for (uint8_t i = 0; i < ringNb; i++)
  {
    ringColor[i] = random8(255U - WIDTH / 8U); // начальный оттенок кольца из палитры 0-255 за минусом длины кольца, делённой пополам
    shiftHueDir[i] = random8();
    huePos[i] = 0U; //random8(WIDTH); само прокрутится постепенно
    stepCount = 0U;
    //do { // песец конструкцию придумал бредовую
    //  stepCount = WIDTH - 3U - random8((WIDTH - 3U) * 2U); само присвоится при первом цикле
    //} while (stepCount < WIDTH / 5U || stepCount > 255U - WIDTH / 5U);
    currentRing = random8(ringNb);
  }
}

bool EffectRingsLock::ringsRoutine(CRGB *leds, const char *param)
{
  uint8_t h, x, y;

  for (uint8_t i = 0; i < ringNb; i++)
  {
    if (i != currentRing) // если это не активное кольцо
    {
       h = shiftHueDir[i] & 0x0F; // сдвигаем оттенок внутри кольца
       if (h > 8U)
         //ringColor[i] += (uint8_t)(7U - h); // с такой скоростью сдвиг оттенка от вращения кольца не отличается
         ringColor[i]--;
       else
         //ringColor[i] += h;
         ringColor[i]++;
    } else {
      if (stepCount == 0) { // если сдвиг активного кольца завершён, выбираем следующее
        currentRing = random8(ringNb);
        do {
          stepCount = WIDTH - 3U - random8((WIDTH - 3U) * 2U); // проворот кольца от хз до хз
        } while (stepCount < WIDTH / 5U || stepCount > 255U - WIDTH / 5U);
      } else {
        if (stepCount > 127U)
          {
            stepCount++;
            huePos[i] = (huePos[i] + 1U) % WIDTH;
          }
        else
          {
            stepCount--;
            huePos[i] = (huePos[i] - 1U + WIDTH) % WIDTH;
          }
      }
    }

    // отрисовываем кольца
    h = (shiftHueDir[i] >> 4) & 0x0F; // берём шаг для градиента вутри кольца
    if (h > 8U)
      h = 7U - h;
    for (uint8_t j = 0U; j < ((i == 0U) ? downRingHue : ((i == ringNb - 1U) ? upRingHue : ringWidth)); j++) // от 0 до (толщина кольца - 1)
    {
      y = i * ringWidth + j - ((i == 0U) ? 0U : ringWidth - downRingHue);
      for (uint8_t k = 0; k < WIDTH / 4U; k++) // Четверть кольца
        {
          x = (huePos[i] + k) % WIDTH; // первая половина кольца
          myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, ringColor[i] + k * h));
          x = (WIDTH - 1 + huePos[i] - k) % WIDTH; // вторая половина кольца (зеркальная первой)
          myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, ringColor[i] + k * h));
        }
      if (WIDTH & 0x01) //(WIDTH % 2U > 0U) // если число пикселей по ширине матрицы нечётное, тогда не забываем и про среднее значение
      {
        x = (huePos[i] + WIDTH / 2U) % WIDTH;
        myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, ringColor[i] + WIDTH / 2U * h));
      }
    }
  }
  return true;
}

// ------------------------------ ЭФФЕКТ КУБИК 2D ----------------------
// (c) SottNick
// refactored by Vortigont

bool EffectCube2d::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  if (csum != (scale^rval))
    cubesize();

  return cube2dRoutine(*&ledarr, &*opt);
}

void EffectCube2d::load(){
  palettesload();    // подгружаем дефолтные палитры
  cubesize();
}

// задаем размерность кубов
void EffectCube2d::cubesize(){

  if (curPalette == nullptr) {
    return;
  }

  FastLED.clear();
  uint8_t cubeScaleX = EffectMath::ceil8(MAX_RANGE, CUBE2D_MAX_SIZE);       // масштаб "шкалы" в макс. размерность прямоугольника по X
  sizeX = EffectMath::ceil8(scale, cubeScaleX);

  uint8_t cubeScaleY = EffectMath::ceil8(cubeScaleX, CUBE2D_MAX_SIZE);      // масштаб вторичной "шкалы" в макс. размерность прямоугольника по Y
  uint8_t scaleY = scale%cubeScaleX;
  ++scaleY;   // начинаем шкалу с "1"
  sizeY = EffectMath::ceil8(scaleY, cubeScaleY);

  cntY = HEIGHT / (sizeY + 1U);
	fieldY = (sizeY + 1U) * cntY;

  cntX = WIDTH / (sizeX + 1U);
	fieldX = (sizeX + 1U) * cntX;
  //LOG(printf_P, PSTR("CUBE2D Size: scX=%d, scY=%d, scaleY=%d, cntX=%d, cntY=%d\n"), cubeScaleX, cubeScaleY, scaleY, cntX, cntY);

  uint8_t x=0, y = 0;
  CRGB color;

  for (uint8_t j = 0U; j < cntY; j++)
  {
    y = j * (sizeY + 1U);
    for (uint8_t i = 0U; i < cntX; i++)
    {
      x = i * (sizeX + 1U);
      if (scale == 255U)
        color = CHSV(45U, 0U, 128U + random8(128U));
      else {
          color = ColorFromPalette(*curPalette, random8());
        for (uint8_t k = 0U; k < sizeY; k++){
          for (uint8_t m = 0U; m < sizeX; m++){
            myLamp.setLeds(myLamp.getPixelNumber(x+m, y+k), color);
          }
        }
      }
    }
  }

  pauseSteps = CUBE2D_PAUSE_FRAMES; // осталось шагов паузы
  shiftSteps = 0;
  csum = scale^rval;
  //end
}

bool EffectCube2d::cube2dRoutine(CRGB *leds, const char *param)
{
  if (curPalette == nullptr) {
    return false;
  }
  if (!pauseSteps){
    pauseSteps--;
    return false; // пропускаем кадры после прокрутки кубика (делаем паузу)
  }

  CRGB color, color2;
  uint16_t x, y, anim0;

  if (!shiftSteps) {  // если цикл вращения завершён
    // ====== определяем направление прокруток на будущий цикл
    pauseSteps = CUBE2D_PAUSE_FRAMES;
    direction = random8()%2;  // сдвиг 0 - строки, 1 - столбцы
    moveItem = random8()%(direction ? cntX : cntY);
    movedirection = random8()%2;  // 1 - fwd, 0 - bkwd
    shiftSteps = ((direction ? sizeY : sizeX)+1) * random8(direction ? cntY : cntX);  // такой рандом добавляет случайную задержку в паузу, попадая на "0"
    return false;
  }

  //LOG(printf_P, PSTR("Cube2D Move: dir=%d, steps=%d, movdir=%d\n"), direction, shiftSteps, movedirection);
  //двигаем, что получилось...
  shiftSteps--;
  if (direction)  // идём по горизонтали, крутим по вертикали (столбцы двигаются)
  {
      x = moveItem * (sizeX + 1U);
      anim0 = 0;
      if (!movedirection) // если крутим столбец вниз
        {
          color = myLamp.getPixColorXY(x, anim0);                                   // берём цвет от нижней строчки
          for (uint8_t k = anim0; k < anim0+fieldY-1; k++)
          {
            color2 = myLamp.getPixColorXY(x,k+1);                                   // берём цвет от строчки над нашей
            for (uint8_t m = x; m < x + sizeX; m++)
              myLamp.setLeds(myLamp.getPixelNumber(m,k), color2);                           // копируем его на всю нашу строку
          }
          for   (uint8_t m = x; m < x + sizeX; m++)
            myLamp.setLeds(myLamp.getPixelNumber(m, anim0+fieldY-1), color);                  // цвет нижней строчки копируем на всю верхнюю
          return true;
        }
      // shift > 0 крутим столбец вверх
      color = myLamp.getPixColorXY(x,anim0+fieldY-1);                            // берём цвет от верхней строчки
      for (uint8_t k = anim0+fieldY-1; k > anim0 ; k--)
      {
        color2 = myLamp.getPixColorXY(x,k-1);                                   // берём цвет от строчки под нашей
        for (uint8_t m = x; m < x + sizeX; m++)
          myLamp.setLeds(myLamp.getPixelNumber(m,k), color2);                           // копируем его на всю нашу строку
      }
      for   (uint8_t m = x; m < x + sizeX; m++)
        myLamp.setLeds(myLamp.getPixelNumber(m, anim0), color);                         // цвет верхней строчки копируем на всю нижнюю
      return true;
  }

  // идём по вертикали, крутим по горизонтали (строки двигаются)
  y = moveItem * (sizeY + 1U);
  anim0 = 0;
  if (!movedirection) // если крутим строку влево
  {
    color = myLamp.getPixColorXY(anim0, y);                            // берём цвет от левой колонки (левого пикселя)
    for (uint8_t k = anim0; k < anim0+fieldX-1; k++)
    {
      color2 = myLamp.getPixColorXY(k+1, y);                           // берём цвет от колонки (пикселя) правее
      for (uint8_t m = y; m < y + sizeY; m++)
        myLamp.setLeds(myLamp.getPixelNumber(k, m), color2);                           // копируем его на всю нашу колонку
    }
    for   (uint8_t m = y; m < y + sizeY; m++)
      myLamp.setLeds(myLamp.getPixelNumber(anim0+fieldX-1, m), color);                  // цвет левой колонки копируем на всю правую
    return true;
  }

  //  крутим строку вправо
  color = myLamp.getPixColorXY(anim0+fieldX-1, y);                    // берём цвет от правой колонки
  for (uint8_t k = anim0+fieldX-1; k > anim0 ; k--)
  {
    color2 = myLamp.getPixColorXY(k-1, y);                           // берём цвет от колонки левее
    for (uint8_t m = y; m < y + sizeY; m++)
      myLamp.setLeds(myLamp.getPixelNumber(k, m), color2);                           // копируем его на всю нашу колонку
  }
  for   (uint8_t m = y; m < y + sizeY; m++)
    myLamp.setLeds(myLamp.getPixelNumber(anim0, m), color);                          // цвет правой колонки копируем на всю левую

  return true;
}

//--------------
bool EffectTime::run(CRGB *ledarr, const char *opt){
  if((millis() - lastrun - EFFECTS_RUN_TIMER) < (unsigned)((255-speed)) && (speed==1 || speed==255)){
      myLamp.dimAll(254);
    return true;
  } else {
    lastrun = millis();
    if (myLamp.isPrintingNow()) // если выводится бегущая строка, то эффект приостанавливаем! Специально обученный костыль, т.к. вывод статического и динамического текста одноверенно не совместимы
      return true;
  }
  // if (dryrun())
  //   return false;
  return timePrintRoutine(*&ledarr, &*opt);
}

void EffectTime::load(){
  palettesload();    // подгружаем дефолтные палитры

  if(((curTimePos<=(signed)LET_WIDTH*2-(LET_WIDTH/2)) || (curTimePos>=(signed)WIDTH+(LET_WIDTH/2))) )
  {
    curTimePos = random(LET_WIDTH*2,WIDTH);
    hColor[0] = ColorFromPalette(*curPalette, random8());
    mColor[0] = ColorFromPalette(*curPalette, random8());
  }
}

bool EffectTime::timePrintRoutine(CRGB *leds, const char *param)
{
  if (speed==1 || speed==255){
    EVERY_N_SECONDS(5){
      FastLED.clear();
      uint8_t xPos = random(LET_WIDTH*2,WIDTH);
      String tmp = myLamp.timeProcessor.getFormattedShortTime();
      myLamp.sendStringToLamp(tmp.substring(0,2).c_str(), ColorFromPalette(*curPalette, random8()), false, HEIGHT-LET_HEIGHT, xPos);
      myLamp.sendStringToLamp(tmp.substring(3,5).c_str(), ColorFromPalette(*curPalette, random8()), false, HEIGHT-(LET_HEIGHT*2), xPos);
    }
  } else {
    //FastLED.clear();
    myLamp.dimAll(250-speed/3); // небольшой шлейф, чисто как визуальный эффект :)
    int16_t xPos = curTimePos;
    if((xPos<=(signed)LET_WIDTH*2-((signed)LET_WIDTH/2)) || (xPos>=(signed)WIDTH+((signed)LET_WIDTH/2))){
      if(xPos<=(signed)LET_WIDTH*2){
        timeShiftDir = false;
        xPos=LET_WIDTH*2-(LET_WIDTH/2); // будет на полсимвола выходить за пределы, так задумано :)
      } else {
        timeShiftDir = true;
        xPos=WIDTH+(LET_WIDTH/2); // будет на полсимвола выходить за пределы, так задумано :)
      }
      hColor[0] = ColorFromPalette(*curPalette, random8());
      mColor[0] = ColorFromPalette(*curPalette, random8());
    }
    String tmp = myLamp.timeProcessor.getFormattedShortTime();
    uint8_t shift = beatsin8(speed/5, -1, 1);
    myLamp.sendStringToLamp(tmp.substring(0,2).c_str(), hColor[0], false, HEIGHT-LET_HEIGHT+shift, xPos);
    myLamp.sendStringToLamp(tmp.substring(3,5).c_str(), mColor[0], false, HEIGHT-(LET_HEIGHT*2)+shift, xPos);
    curTimePos=curTimePos+(0.23*(speed/255.0))*(timeShiftDir?-1:1); // смещаем
  }
  return true;
}

// ------------------------------ ЭФФЕКТ ДЫМ ----------------------
bool EffectMStreamSmoke::run(CRGB *ledarr, const char *opt){
  return multipleStreamSmokeRoutine(*&ledarr, &*opt);
}

// TODO: объединить функцию вместе с "EffectComet", сейчас лень возиться с указателями на массивы:)
void EffectMStreamSmoke::FillNoise(int8_t layer) {
  const uint8_t e_centerX =  (WIDTH / 2) - 1;
  const uint8_t e_centerY = (HEIGHT / 2) - 1;

  for (uint8_t i = 0; i < WIDTH; i++) {
    int32_t ioffset = e_scaleX[layer] * (i - e_centerX);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      int32_t joffset = e_scaleY[layer] * (j - e_centerY);
      int8_t data = inoise16(e_x[layer] + ioffset, e_y[layer] + joffset, e_z[layer]) >> 8;
      int8_t olddata = noise3d[layer][i][j];
      int8_t newdata = scale8( olddata, eNs_noisesmooth ) + scale8( data, 255 - eNs_noisesmooth );
      data = newdata;
      noise3d[layer][i][j] = data;
    }
  }
}

// (c) SottNick
// Относительно стартовой версии - переписан 20200521
bool EffectMStreamSmoke::multipleStreamSmokeRoutine(CRGB *leds, const char *param)
{
  CRGB color;

  myLamp.dimAll(254);

  //String var = myLamp.effects.getValue(myLamp.effects.getCurrent()->param, F("R"));

  int val;
  if(rval){
    val = ((int)(6*rval/255.1))%6;
    xSmokePos=xSmokePos+(rval%43)/42.0+0.01;
    xSmokePos2=xSmokePos2+(rval%43)/84.0+0.01;
  } else {
    val = scale%6;
    xSmokePos=xSmokePos+speed/255.0+0.01;
    xSmokePos2=xSmokePos2+speed/512.0+0.01;
  }

  bool isColored = scale<250; // 250...255, т.е. 6 штук закладываю на заполнения
  if (isColored)
  {
    if (smokeHue == scale)
      {
        smokeHue = 0U;
        rhue = random8();
      }
    color = CHSV(rhue, 255U, 255U);
    if ((int)xSmokePos & 0x01)//((smokeDeltaHue >> 2U) == 0U) // какой-то умножитель охота подключить к задержке смены цвета, но хз какой...
      smokeHue++;
  }
  else
    color = CHSV((scale - 1U) * 2.6, !isColored ? 0U : 255U, 255U);

  switch(val){
    case 0:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(((int)xSmokePos-y)%WIDTH-(int)(xSmokePos2)%WIDTH, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-((int)xSmokePos-y)-1)%WIDTH-(int)(xSmokePos2)%WIDTH, y)] += color;
      }
      break;
    case 1:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((int)(xSmokePos-y)%WIDTH+(int)(xSmokePos2)%WIDTH, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-(int)(xSmokePos-y)-1)%WIDTH+(int)(xSmokePos2)%WIDTH, y)] += color;
      }
      break;
    case 2:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((int)((xSmokePos-y)*1.5)%WIDTH-(int)(xSmokePos2)%WIDTH, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-(int)((xSmokePos-y)*1.5)-1)%WIDTH-(int)(xSmokePos2)%WIDTH, y)] += color; // увеличим частоту в 1.5
      }
      break;
    case 3:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((int)((xSmokePos-y)*1.5)%WIDTH+(int)(xSmokePos2)%WIDTH, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-(int)((xSmokePos-y)*1.5)-1)%WIDTH+(int)(xSmokePos2)%WIDTH, y)] += color; // увеличим частоту в 1.5
      }
      break;
    case 4:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(((int)xSmokePos-y)%WIDTH*y/(HEIGHT-1)+WIDTH/2, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-((int)xSmokePos-y)-1)%WIDTH*y/(HEIGHT-1)+WIDTH/2, y)] += color;
      }
      break;
    case 5:
      for (uint8_t y = 0; y < HEIGHT; y++) {
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((xSmokePos-y)*y/(HEIGHT-1)+WIDTH/2, y)] += color; // на то что Х может оказаться отрицательным - ложим болт :)
        myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber((WIDTH-(xSmokePos-y)-1)*y/(HEIGHT-1)+WIDTH/2, y)] += color;
      }
      break;
    default:
      break;
  }

  // Noise
    uint16_t sc = (uint16_t)speed * 6 + 500;
    uint16_t sc2 = (float)speed/127.0 + 1.5;

    e_x[0] += sc;
    e_y[0] += sc;
    e_z[0] += sc;
    e_scaleX[0] = sc2;
    e_scaleY[0] = sc2;
    FillNoise(0);
    //MoveX(3);
    //MoveY(3);

    EffectMath::MoveFractionalNoise(MOVE_X, noise3d, 3);//4
    EffectMath::MoveFractionalNoise(MOVE_Y, noise3d, 3);//4

  myLamp.blur2d(25); // без размытия как-то пиксельно, наверное...
  return true;
}


// ============= Fire Effect =================
void EffectFire::load(){
  pcnt = 0U;
  generateLine();
}

bool EffectFire::run(CRGB *ledarr, const char *opt){
  if (dryrun())
    return false;

  return fireRoutine(*&ledarr, &*opt);
}

bool EffectFire::fireRoutine(CRGB *leds, const char *param)
{
  if (pcnt >= 30) {                                  // внутренний делитель кадров для поднимающегося пламени
    shiftUp();                                              // смещение кадра вверх
    generateLine();                                         // перерисовать новую нижнюю линию случайным образом
    pcnt = 0;
  }

  drawFrame(pcnt, true);                              // для прошивки где стоит логический параметр
  pcnt += 25;
  return true;
}

// Randomly generate the next line (matrix row)
void EffectFire::generateLine() {
  for (uint8_t x = 0U; x < WIDTH; x++) {
    line[x] = random8(127, 255);                             // заполнение случайным образом нижней линии (127, 255) - менее контрастное, (64, 255) - оригинал
  }
}

void EffectFire::shiftUp() {                                            //подъем кадра
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      uint8_t newX = x % 16U;                               // сократил формулу без доп. проверок
      if (y > 7U) continue;
      matrixValue[y][newX] = matrixValue[y - 1U][newX];     //смещение пламени (только для зоны очага)
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++) {                    // прорисовка новой нижней линии
    uint8_t newX = x % 16U;                                 // сократил формулу без доп. проверок
    matrixValue[0U][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void EffectFire::drawFrame(uint8_t pcnt, bool isColored) {                  // прорисовка нового кадра
  int32_t nextv;
#ifdef UNIVERSE_FIRE                                            // если определен универсальный огонь
  uint8_t baseHue = (myLamp.effects.getScale() - 1U) * 2.6;
#else
  uint8_t baseHue = isColored ? 255U : 0U;
#endif
  uint8_t baseSat = (myLamp.effects.getScale() < 255) ? 255U : 0U;  // вычисление базового оттенка

  uint8_t deltaValue = random8(0U, 3U) ? constrain (shiftValue[0] + random8(0U, 2U) - random8(0U, 2U), 15, 17) : shiftValue[0]; // random(0U, 3U)= скорость смещения очага чем больше 3U - тем медленнее                          // текущее смещение пламени (hueValue)

  //first row interpolates with the "next" line
  uint8_t deltaHue = random8(0U, 2U) ? constrain (shiftHue[0] + random8(0U, 2U) - random8(0U, 2U), 15, 17) : shiftHue[0]; // random(0U, 2U)= скорость смещения языков чем больше 2U - тем медленнее
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  // высчитываем плавную дорожку смещения всполохов для нижней строки
  // так как в последствии координаты точки будут исчисляться из остатка, то за базу можем принять кратную ширину матрицы hueMask
  // ширина матрицы hueMask = 16, поэтому нам нужно получить диапазон чисел от 15 до 17
  // далее к предыдущему значению прибавляем случайную 1 и отнимаем случайную 1 - это позволит плавным образом менять значение смещения
  shiftHue[0] = deltaHue;                                   // заносим это значение в стэк
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  shiftValue[0] = deltaValue;


  for (uint8_t x = 0U; x < WIDTH; x++) {                                          // прорисовка нижней строки (сначала делаем ее, так как потом будем пользоваться ее значением смещения)
    uint8_t newX = x % 16;                                                        // сократил формулу без доп. проверок
    nextv =                                                               // расчет значения яркости относительно valueMask и нижерасположенной строки.
      (((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0)
      - pgm_read_byte(&valueMask[0][(x + deltaValue) % 16U]);
    CRGB color = CHSV(                                                            // вычисление цвета и яркости пикселя
                   baseHue + pgm_read_byte(&hueMask[0][(x + deltaHue) % 16U]),    // H - смещение всполохов
                   baseSat,                                                       // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                   (uint8_t)max(0, nextv)                                         // V
                 );
    myLamp.setLeds(myLamp.getPixelNumber(x, 0), color);                                            // прорисовка цвета очага
  }

  //each row interpolates with the one before it
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {                                      // прорисовка остальных строк с учетом значения низлежащих
    deltaHue = shiftHue[y];                                                         // извлекаем положение
    shiftHue[y] = shiftHue[y - 1];                                                  // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
    deltaValue = shiftValue[y];                                                     // извлекаем положение
    shiftValue[y] = shiftValue[y - 1];                                              // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем


    if (y > 8U) {                                                                   // цикл стирания текущей строоки для искр
      for (uint8_t _x = 0U; _x < WIDTH; _x++) {                                     // стираем строчку с искрами (очень не оптимально)
        myLamp.drawPixelXY(_x, y, 0U);
      }
    }
    for (uint8_t x = 0U; x < WIDTH; x++) {                                          // пересчет координаты x для текущей строки
      uint8_t newX = x % 16U;                                                       // функция поиска позиции значения яркости для матрицы valueMask
      if (y < 8U) {                                                                 // если строка представляет очаг
        nextv =                                                                     // расчет значения яркости относительно valueMask и нижерасположенной строки.
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&valueMask[y][(x + deltaValue) % 16U]);

        CRGB color = CHSV(                                                                  // определение цвета пикселя
                       baseHue + pgm_read_byte(&hueMask[y][(x + deltaHue) % 16U ]),         // H - смещение всполохов
                       baseSat,                                                             // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                       (uint8_t)max(0, nextv)                                               // V
                     );
        myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
      }
      else if (y == 8U && SPARKLES) {                                               // если это самая нижняя строка искр - формитуем искорку из пламени
        if (random8(0, 20) == 0 && myLamp.getPixColorXY(x, y - 1U) != 0U) myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y - 2U));  // 20 = обратная величина количества искр
        else myLamp.drawPixelXY(x, y, 0U);
      }
      else if (SPARKLES) {                                                          // если это не самая нижняя строка искр - перемещаем искорку выше
        // старая версия для яркости
        newX = (random8(0, 4)) ? x : (x + WIDTH + random8(0U, 2U) - random8(0U, 2U)) % WIDTH ;   // с вероятностью 1/3 смещаем искорку влево или вправо
        if (myLamp.getPixColorXY(x, y - 1U) > 0U) myLamp.drawPixelXY(newX, y, myLamp.getPixColorXY(x, y - 1U));    // рисуем искорку на новой строчке
      }
    }
  }
}



/*
 * Создаем экземпляр класса калькулятора в зависимости от требуемого эффекта
 */
void EffectWorker::workerset(EFF_ENUM effect){
  switch (effect)
  {
  case EFF_ENUM::EFF_TIME :
    worker = std::unique_ptr<EffectTime>(new EffectTime());
    break;
  case EFF_ENUM::EFF_SWIRL :
    worker = std::unique_ptr<EffectSwirl>(new EffectSwirl());
    break;
  case EFF_ENUM::EFF_RAINBOWCOMET :
  case EFF_ENUM::EFF_RAINBOWCOMET3 :
    worker = std::unique_ptr<EffectComet>(new EffectComet());
    break;
  case EFF_ENUM::EFF_FLOCK :
    worker = std::unique_ptr<EffectFlock>(new EffectFlock());
    break;
  case EFF_ENUM::EFF_PRIZMATA :
    worker = std::unique_ptr<EffectPrismata>(new EffectPrismata());
    break;
  case EFF_ENUM::EFF_SPIRO :
    worker = std::unique_ptr<EffectSpiro>(new EffectSpiro());
    break;
  case EFF_ENUM::EFF_METABALLS :
    worker = std::unique_ptr<EffectMetaBalls>(new EffectMetaBalls());
    break;
  case EFF_ENUM::EFF_SINUSOID3 :
    worker = std::unique_ptr<EffectSinusoid3>(new EffectSinusoid3());
    break;
  case EFF_ENUM::EFF_BBALS :
    worker = std::unique_ptr<EffectBBalls>(new EffectBBalls());
    break;
  case EFF_ENUM::EFF_PAINTBALL :
    worker = std::unique_ptr<EffectLightBalls>(new EffectLightBalls());
    break;
  case EFF_ENUM::EFF_FIRE :
    worker = std::unique_ptr<EffectFire>(new EffectFire());
    break;
  case EFF_ENUM::EFF_PULSE :
    worker = std::unique_ptr<EffectPulse>(new EffectPulse());
    break;
  case EFF_ENUM::EFF_CUBE :
    worker = std::unique_ptr<EffectBall>(new EffectBall());
    break;
  case EFF_ENUM::EFF_LIGHTER_TRACES :
    worker = std::unique_ptr<EffectLighterTracers>(new EffectLighterTracers());
    break;
  case EFF_ENUM::EFF_RAINBOW_2D :
    worker = std::unique_ptr<EffectRainbow>(new EffectRainbow());
    break;
  case EFF_ENUM::EFF_COLORS :
    worker = std::unique_ptr<EffectColors>(new EffectColors());
    break;
  case EFF_ENUM::EFF_WHITE_COLOR :
    worker = std::unique_ptr<EffectWhiteColorStripe>(new EffectWhiteColorStripe());
    break;
  case EFF_ENUM::EFF_MATRIX :
    worker = std::unique_ptr<EffectMatrix>(new EffectMatrix());
    break;
  case EFF_ENUM::EFF_SNOW :
    worker = std::unique_ptr<EffectSnow>(new EffectSnow());
    break;
  case EFF_ENUM::EFF_SPARKLES :
    worker = std::unique_ptr<EffectSparcles>(new EffectSparcles());
    break;
  case EFF_ENUM::EFF_EVERYTHINGFALL :
    worker = std::unique_ptr<EffectEverythingFall>(new EffectEverythingFall());
    break;
  case EFF_ENUM::EFF_FIRE2012 :
    worker = std::unique_ptr<EffectFire2012>(new EffectFire2012());
    break;
  case EFF_ENUM::EFF_SNOWSTORMSTARFALL :
    worker = std::unique_ptr<EffectStarFall>(new EffectStarFall());
    break;
  case EFF_ENUM::EFF_LIGHTERS :
    worker = std::unique_ptr<EffectLighters>(new EffectLighters());
    break;
  case EFF_ENUM::EFF_MADNESS :
  case EFF_ENUM::EFF_CLOUDS :
  case EFF_ENUM::EFF_LAVA :
  case EFF_ENUM::EFF_PLASMA :
  case EFF_ENUM::EFF_RAINBOW :
  case EFF_ENUM::EFF_RAINBOW_STRIPE :
  case EFF_ENUM::EFF_ZEBRA :
  case EFF_ENUM::EFF_FOREST :
  case EFF_ENUM::EFF_OCEAN :
    worker = std::unique_ptr<Effect3DNoise>(new Effect3DNoise());
    break;
  case EFF_ENUM::EFF_DRIFT :
  case EFF_ENUM::EFF_DRIFT2 :
    worker = std::unique_ptr<EffectDrift>(new EffectDrift());
    break;
  case EFF_ENUM::EFF_TWINKLES :
    worker = std::unique_ptr<EffectTwinkles>(new EffectTwinkles());
    break;
  case EFF_ENUM::EFF_WAVES :
    worker = std::unique_ptr<EffectWaves>(new EffectWaves());
    break;
  case EFF_ENUM::EFF_RADAR :
    worker = std::unique_ptr<EffectRadar>(new EffectRadar());
    break;
  case EFF_ENUM::EFF_SMOKE :
    worker = std::unique_ptr<EffectMStreamSmoke>(new EffectMStreamSmoke());
    break;
  case EFF_ENUM::EFF_FIRE2018 :
    worker = std::unique_ptr<EffectFire2018>(new EffectFire2018());
    break;
  case EFF_ENUM::EFF_RINGS :
    worker = std::unique_ptr<EffectRingsLock>(new EffectRingsLock());
    break;
  case EFF_ENUM::EFF_CUBE2 :
    worker = std::unique_ptr<EffectCube2d>(new EffectCube2d());
    break;
  case EFF_ENUM::EFF_RAIN :
  case EFF_ENUM::EFF_COLORRAIN :
  case EFF_ENUM::EFF_STORMYRAIN :
    worker = std::unique_ptr<EffectRain>(new EffectRain());
    break;
#ifdef MIC_EFFECTS
  case EFF_ENUM::EFF_FREQ :
    worker = std::unique_ptr<EffectFreq>(new EffectFreq());
    break;
#endif

  default:
    worker = std::unique_ptr<EffectCalc>(new EffectCalc());
  }

  worker->init(effect, myLamp.effects.getBrightness(), myLamp.effects.getSpeed(), myLamp.effects.getScale());

}

void stubRoutine(CRGB *, const char *){}
