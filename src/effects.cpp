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

void EffectCalc::init(EFF_ENUM _eff, LList<UIControl*>* controls, LAMPSTATE *_lampstate){
  effect=_eff;
  for(int i=0; i<controls->size(); i++){
    switch(i){
      case 0:
        brightness = (*controls)[i]->getVal().toInt();
        break;
      case 1:
        speed = (*controls)[i]->getVal().toInt();
        break;
      case 2:
        scale = (*controls)[i]->getVal().toInt();
        break;
      default:
        setDynCtrl((*controls)[i]);
        break;
    }
  }

  if(!_lampstate->isMicOn){ // при отключенном глобальном дернем выключение
    UIControl *ctrl = new UIControl(7,(CONTROL_TYPE)18,String(FPSTR(TINTF_020)), FPSTR(TCONST_FFFE), String(""), String(""), String(""));
    setDynCtrl(ctrl);
    delete ctrl;
  }

  active=true;
  load();
}

/*
 *  первоначальная загрузка эффекта, автозапускается из init()
 */
void EffectCalc::load(){}

bool EffectCalc::run(CRGB* ledarr, EffectWorker *opt){
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
  //LOG(printf_P, PSTR("Worker brt: %d\n"), brightness);
}

/**
 * setSpd - установка скорости для воркера
 */
void EffectCalc::setspd(const byte _spd){
  speed = _spd;
  //LOG(printf_P, PSTR("Worker speed: %d\n"), speed);
}

/**
 * setBrt - установка шкалы для воркера
 */
void EffectCalc::setscl(byte _scl){
  scale = _scl;
  //LOG(printf_P, PSTR("Worker scale: %d\n"), scale);

  if (usepalettes && (ctrls->size()<4 || (ctrls->size()>=4 && !isCtrlPallete))){   // менять палитру в соответствие со шкалой, если только 3 контрола или если нет контрола палитры
    palettemap(palettes, _scl);
    paletteIdx = _scl;
  }
}

/**
 * setDynCtrl - была смена динамического контрола, idx=3+
 * вызывается в UI, для реализации особого поведения (палитра и т.д.)...
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/page-112#post-48848
 */
void EffectCalc::setDynCtrl(UIControl*_val){
  if(!_val) return;

  if (usepalettes && _val->getName().startsWith(FPSTR(TINTF_084))==1){ // Начинается с Палитра
    paletteIdx = _val->getVal().toInt();
    palettemap(palettes, paletteIdx);
    isCtrlPallete = true;
  }

  if(_val->getName().startsWith(FPSTR(TINTF_020)) && _val->getId()==7){ // Начинается с микрофон и имеет 7 id
    isMicActive = _val->getVal()=="true" ? true : false;
#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(isMicActive);
#endif
  }
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
  //palettes.push_back(&WhiteBlackColors_p);

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

  for(int i=3;i<ctrls->size();i++){
    setDynCtrl((*ctrls)[i]);
  }

  if(!isCtrlPallete){
    palettemap(palettes, (*ctrls)[2]->getVal().toInt());
  }
}

// В виду "творческой" переработки управлени эффетом, пришлось создать спец.метод выбора палитры
void EffectWaves::WavesPaletteMap(std::vector<PGMPalette*> &_pals, const uint8_t _val){
  if (!_pals.size()) {
    LOG(println,F("No palettes loaded or wrong value!"));
    return;
  }

  palettepos = _val;
  curPalette = _pals.at(palettepos);
 
  //LOG(printf_P,PSTR("Mapping value to pallete: Psize=%d, POS=%d, ptPallete=%d, palettescale=%d, szof=%d\n"), _pals.size(), palettepos, ptPallete, palettescale, sizeof(TProgmemRGBPalette16 *));
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
      int8_t delta = ((uint16_t)fabs(amount) >> 8) ;
      int8_t fraction = ((uint16_t)fabs(amount) & 255);
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

/* kostyamat добавил
функция увеличения яркости */
CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

/* kostyamat добавил
 функция уменьшения яркости */
CRGB makeDarker( const CRGB& color, fract8 howMuchDarker)
{
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

// ============= ЭФФЕКТЫ ===============

// ------------- конфетти --------------
bool EffectSparcles::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  return sparklesRoutine(*&ledarr, &*opt);
}

bool EffectSparcles::sparklesRoutine(CRGB *leds, EffectWorker *param)
{
#ifdef MIC_EFFECTS
  uint8_t mic = myLamp.getMicMapMaxPeak();
  uint8_t mic_f = map(myLamp.getMicMapFreq(), LOW_FREQ_MAP_VAL, HI_FREQ_MAP_VAL, 0, 255);
  if (isMicActive and getCtrlVal(3).toInt() > 5) 
    fadeToBlackBy(leds, NUM_LEDS, 255 - mic);
  EffectMath::fader(isMicActive ? map(getCtrlVal(2).toInt(), 1, 255, 100, 1) : map(getCtrlVal(2).toInt(), 1, 255, 50, 1));
#else
  EffectMath::fader(map(scale, 1, 255, 1, 50));
#endif

  CHSV currentHSV;

  for (uint8_t i = 0; i < (uint8_t)round(2.5 * (getCtrlVal(1).toInt() / 255.0) + 1); i++) {
    uint8_t x = random8(0U, WIDTH);
    uint8_t y = random8(0U, HEIGHT);
#ifdef MIC_EFFECTS
    switch (getCtrlVal(3).toInt())
  {
  case 1 :
  case 6 :
    mic_f = myLamp.getMicMapFreq();
    break;
  case 2 :
  case 7 :
    mic_f = map(myLamp.getMicMapFreq(), LOW_FREQ_MAP_VAL, HI_FREQ_MAP_VAL, 0, 255);
    break;
  case 3 :
  case 8 :
    mic_f = sin8(myLamp.getMicMapFreq());
    break;
  case 4 :
  case 9 :
    mic_f = cos8(myLamp.getMicMapFreq());
    break;
  case 5 :
  case 10 :
    mic_f = random8(255U);
    break;
  }

#endif
    if (myLamp.getPixColorXY(x, y) == 0U) {
#ifdef MIC_EFFECTS
      if (isMicActive) {
        currentHSV = CHSV(mic_f, 255U - myLamp.getMicMapMaxPeak()/3, constrain(mic * 1.25f, 48, 255));
      }
      else
        currentHSV = CHSV(random8(1U, 255U), random8(192U, 255U), random8(192U, 255U));
#else
        currentHSV = CHSV(random8(1U, 255U), random8(192U, 255U), random8(192U, 255U));
#endif
      myLamp.setLeds(myLamp.getPixelNumber(x, y), currentHSV);
    }
  }
  return true;
}

// ------ Эффект "Белая Лампа"
// ------------- белый свет (светится горизонтальная полоса по центру лампы; масштаб - высота центральной горизонтальной полосы; скорость - регулировка от холодного к тёплому; яркость - общая яркость) -------------
bool EffectWhiteColorStripe::run(CRGB *ledarr, EffectWorker *opt){

  return whiteColorStripeRoutine(*&ledarr, &*opt);
}

bool EffectWhiteColorStripe::whiteColorStripeRoutine(CRGB *leds, EffectWorker *param)
{
#ifdef MIC_EFFECTS
  byte _scale = isMicActive ? myLamp.getMicMapMaxPeak() : scale;
#else
  byte _scale = scale;
#endif
    if(_scale <= 127){
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
              : ((_scale) / 143.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? BRIGHTNESS : br);  // для остальных горизонтальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

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
              45U,                                                        // определяем тон
              map(speed, 0U, 255U, 0U, 170U),                            // определяем насыщенность
              x == centerX                                                                                                    // определяем яркость
                ? BRIGHTNESS                                                                                                  // для центральной вертикальной полосы (или двух) яркость всегда равна BRIGHTNESS
                : ((255-_scale) / 143.0F) > ((centerX + 1.0F) - (x + 1.0F)) / (centerX + 1.0F) ? BRIGHTNESS : br);  // для остальных вертикальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

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
bool EffectEverythingFall::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  return fire2012WithPalette(*&ledarr, &*opt);
}

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80U // 50 // 30 // 120 // 90 // 60
bool EffectEverythingFall::fire2012WithPalette(CRGB*leds, EffectWorker *param) {
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
bool EffectPulse::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  return pulseRoutine(*&ledarr, &*opt);
}

bool EffectPulse::pulseRoutine(CRGB *leds, EffectWorker *param) {

  CRGBPalette16 palette;
  CRGB _pulse_color;
  uint8_t _pulse_delta = 0;

  palette = RainbowColors_p;
  uint8_t _scale = scale;
#ifdef MIC_EFFECTS
  #define FADE 255U - (isMicActive ? myLamp.getMicMapMaxPeak()*2 : 248U) // (isMicActive ? 300U - myLamp.getMicMapMaxPeak() : 5U)
  #define BLUR (isMicActive ? myLamp.getMicMapMaxPeak()/3 : 10U) //(isMicActive ? map(myLamp.getMicMapMaxPeak(), 1, 255, 1, 30) : 10U)
#else
  #define FADE 7U
  #define BLUR 10U
#endif

  //myLamp.dimAll(248U); // если эффект устанавливается с другими эффектами от Stefan Petrick, тогда  процедура должна называться dimAll (без двоечки)
  fadeToBlackBy(leds, NUM_LEDS, FADE);
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
      myLamp.drawCircle(centerX, centerY, i, _pulse_color);
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
  myLamp.blur2d(BLUR);
  return true;
}

// радуги 2D
// ------------- радуга вертикальная/горизонтальная ----------------
//uint8_t hue;
bool EffectRainbow::rainbowHorVertRoutine(bool isVertical)
{
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
#ifdef MIC_EFFECTS
    uint8_t micPeak = myLamp.getMicMapMaxPeak();
    CHSV thisColor = CHSV((uint8_t)(hue + i * (micPeak > map(speed, 1, 255, 100, 10) and isMicActive ? (scale - micPeak) : scale) % 170), 255, 255); // 1/3 без центральной между 1...255, т.е.: 1...84, 170...255
#else
    CHSV thisColor = CHSV((uint8_t)(hue + i * scale%170), 255, 255);
#endif
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      myLamp.drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
  return true;
}

// ------------- радуга диагональная -------------
bool EffectRainbow::run(CRGB *ledarr, EffectWorker *opt){
  return rainbowDiagonalRoutine(*&ledarr, &*opt);
}

bool EffectRainbow::rainbowDiagonalRoutine(CRGB *leds, EffectWorker *param)
{
  // коэф. влияния замаплен на скорость, 4 ползунок нафиг не нужен
  hue += (6.0 * (speed / 255.0) + 0.05 ); // скорость смещения цвета зависит от кривизны наклна линии, коэф. 6.0 и 0.05

  if(scale<85){
    rainbowHorVertRoutine(false);
    return true;
  } else if (scale>170){
    rainbowHorVertRoutine(true);
    return true;
  }

  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      float twirlFactor = EffectMath::fmap((float)scale, 85, 170, 8.3, 24);      // на сколько оборотов будет закручена матрица, [0..3]
#ifdef MIC_EFFECTS
      twirlFactor *= myLamp.getMicMapMaxPeak() > map(speed, 1, 255, 80, 10) and isMicActive ? 1.5f * ((float)myLamp.getMicMapFreq() / 255.0f) : 1.0f;
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

bool EffectColors::run(CRGB *ledarr, EffectWorker *opt){
  // if (dryrun())
  //   return false;
  return colorsRoutine(*&ledarr, &*opt);
}

bool EffectColors::colorsRoutine(CRGB *leds, EffectWorker *param)
{
  static unsigned int step = 0; // доп. задержка
  unsigned int delay = (speed==1)?4294967294:255-speed+1; // на скорости 1 будет очень долгое ожидание)))

  ihue = (speed==1)?scale:ihue;
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
  if(isMicActive){
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
bool EffectMatrix::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  return matrixRoutine(*&ledarr, &*opt);
}

bool EffectMatrix::matrixRoutine(CRGB *leds, EffectWorker *param)
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
bool EffectSnow::run(CRGB *ledarr, EffectWorker *opt){
  return snowRoutine(*&ledarr, &*opt);
}

#define SNOW_SCALE (1.25) //0.25...5.0
bool EffectSnow::snowRoutine(CRGB *leds, EffectWorker *param)
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
        CRGB curentColor = leds[myLamp.getPixelNumber(x, y + SNOW_SCALE*snowShift)];
        myLamp.drawPixelXY(x, y, (unsigned int)curentColor > 0 ? curentColor : CRGB::Black);
      }
    }

    for (uint8_t x = 0U; x < WIDTH && SNOW_SCALE*snowShift>1.0; x++)
    {
      // заполняем случайно верхнюю строку
      // а также не даём двум блокам по вертикали вместе быть
      if (myLamp.getPixColorXY(x, HEIGHT - 2U) == 0U && (random(0, 260 - scale) <= 5)) {
        CHSV color = CHSV(0, 0, random8(48U, 255U));
        myLamp.drawPixelXY(x, HEIGHT - 1U, color/*0xE0FFFF - 0x101010 * random(0, 4)*/);
      }
      else
        myLamp.drawPixelXY(x, HEIGHT - 1U, CRGB::Black/*0x000000*/);
    }
  }
  // т.к. не храним позицию, то смещаем все синхронно, но в идеале - хранить позиции
  snowShift = (SNOW_SCALE*snowShift > 1.0 ? (SNOW_SCALE*snowShift - (int)(snowShift*SNOW_SCALE)) : (snowShift));

  return true;
}

// ------------- метель -------------

// ------------- звездопад/метель -------------
bool EffectStarFall::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  return snowStormStarfallRoutine(*&ledarr, &*opt);
}

bool EffectStarFall::snowStormStarfallRoutine(CRGB *leds, EffectWorker *param)
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
    lightersSpeed[0U][i] = (float)random(-200, 200) / 10.0f;
    lightersSpeed[1U][i] = (float)random(-200, 200) / 10.0f;
    lightersColor[i] = random(0U, 255U);
    light[i] = 127;
  }

}

bool EffectLighters::run(CRGB *ledarr, EffectWorker *opt){
  return lightersRoutine(*&ledarr, &*opt);
}

bool EffectLighters::lightersRoutine(CRGB *leds, EffectWorker *param)
{
  bool subPix = false;
  if (getCtrlVal(3) == F("true"))
  { // переключатель в true
    subPix = true;
  }
  float speedfactor = (float)speed / 4096.0f + 0.001f;

 // myLamp.blur2d(speed/10);
  //myLamp.dimAll(50 + speed/10);
  memset8( leds, 0, NUM_LEDS * 3); 

  for (uint8_t i = 0U; i < (uint8_t)((LIGHTERS_AM/255.0)*scale)+1; i++) // масштабируем на LIGHTERS_AM, чтобы не было выхода за диапазон
  {
    // EVERY_N_SECONDS(1)
    // {
    //   LOG.printf_P("S0:%d S1:%d P0:%3.2f P1:%3.2f, scale:%3.2f\n", lightersSpeed[0U][i], lightersSpeed[1U][i],lightersPos[0U][i],lightersPos[1U][i],speedfactor);
    // }

    EVERY_N_MILLIS(random16(1024))
    {
      lightersIdx = (lightersIdx+1)%(uint8_t)(((LIGHTERS_AM/255.0)*scale)+1);
      lightersSpeed[0U][lightersIdx] += random(-10, 10);
      lightersSpeed[1U][lightersIdx] += random(-10, 10);
      lightersSpeed[0U][lightersIdx] = fmod(lightersSpeed[0U][lightersIdx], 21);
      lightersSpeed[1U][lightersIdx] = fmod(lightersSpeed[1U][lightersIdx], 21);
    }

    lightersPos[0U][i] += lightersSpeed[0U][i]*speedfactor;
    lightersPos[1U][i] += lightersSpeed[1U][i]*speedfactor;

    if (lightersPos[0U][i] < 0) lightersPos[0U][i] = (float)(WIDTH - 1);
    if (lightersPos[0U][i] >= (float)WIDTH) lightersPos[0U][i] = 0.0f;

    if (lightersPos[1U][i] <= 0.0f)
    {
      lightersPos[1U][i] = 0.0f;
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    if (lightersPos[1U][i] >= (int32_t)(HEIGHT - 1))
    {
      lightersPos[1U][i] = (HEIGHT - 1U);
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }

    EVERY_N_MILLIS(random16(512, 2048)) {
      if (light[i] == 127) 
        light[i] = 255;
      else light[i] = 127;
    }
    if (subPix)
      myLamp.drawPixelXYF(lightersPos[0U][i], lightersPos[1U][i], CHSV(lightersColor[i], 200U, light[i]));
    else 
      myLamp.drawPixelXY((uint8_t)lightersPos[0U][i], (uint8_t)lightersPos[1U][i], CHSV(lightersColor[i], 200U, light[i]));
  }
  return true;
}

// ------------- светлячки со шлейфом -------------
void EffectLighterTracers::load(){
  for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
  {
    int8_t sign;
    // забиваем случайными данными
    coord[j][0U] = (float)WIDTH / 2.0f;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][0U] = ((float)random(40, 150) / 10.0f) * sign;
    coord[j][1U] = (float)HEIGHT / 2;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][1U] = ((float)random(40, 150) / 10.0f) * sign;
    light[j] = 127;
    //ballColors[j] = random(0, 9) * 28;
  }
}


bool EffectLighterTracers::run(CRGB *ledarr, EffectWorker *opt){
  // if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
  //   return;
  // } else {
  //   myLamp.setEffDelay(millis());
  // }

  return lighterTracersRoutine(*&ledarr, &*opt);
}

bool EffectLighterTracers::lighterTracersRoutine(CRGB *leds, EffectWorker *param)
{
  //float speedfactor = speed/2048.0+0.01;
  float speedfactor = (float)speed / 4096.0f + 0.001f;

  if (!BALL_TRACK)                                          // режим без следов шариков
  {
    FastLED.clear();
  }
  else                                                      // режим со следами
  {
    //myLamp.blur2d(speed/10); // точно нужен прямой доступ??? 
    //EffectMath::fader(TRACK_STEP);
    fadeToBlackBy(leds, NUM_LEDS, 255 - (uint8_t)(10 * ((float)speed) /255) + 40); // выводим кубик со шлейфом, длинна которого зависит от скорости.

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
      coord[j][i] += vector[j][i] * speedfactor;
      if (coord[j][i] < 0)
      {
        coord[j][i] = 0.0f;
        vector[j][i] = -vector[j][i];
      }
    }

    if ((uint16_t)coord[j][0U] > (WIDTH - 1))
    {
      coord[j][0U] = (WIDTH - 1);
      vector[j][0U] = -vector[j][0U];
    }
    if ((uint16_t)coord[j][1U] > (HEIGHT - 1))
    {
      coord[j][1U] = (HEIGHT - 1);
      vector[j][1U] = -vector[j][1U];
    }
    EVERY_N_MILLIS(random16(1024)) {
      if (light[j] == 127) 
        light[j] = 255;
      else light[j] = 127;
    }
    myLamp.drawPixelXYF(coord[j][0U], coord[j][1U], CHSV(ballColors[j], 200U, 255U));
  }
  return true;
}

// ------------- пейнтбол -------------
bool EffectLightBalls::run(CRGB *ledarr, EffectWorker *opt){
  return lightBallsRoutine(*&ledarr, &*opt);
}

#define BORDERTHICKNESS       (1U)                          // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)
bool EffectLightBalls::lightBallsRoutine(CRGB *leds, EffectWorker *param)
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
  byte _scale = getCtrlVal(2).toInt();
  //LOG(printf_P, PSTR("getCtrlVal(5)=%s, %d\n"), getCtrlVal(5).c_str(), getCtrlVal(5).toInt()); /// провека чтения отсутствующего контрола

  if (_scale <= 85) 
    ballSize = map(_scale, 1, 85, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else if (_scale > 85 and _scale <= 170)
    ballSize = map(_scale, 170, 86, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else
    ballSize = map(_scale, 171, 255, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));

  for (uint8_t i = 0U; i < 2U; i++)
  {
    coordB[i] = i? float(WIDTH - ballSize) / 2 : float(HEIGHT - ballSize) / 2;
    vectorB[i] = (float)random(0, 240) / 10.0f - 12.0f;
    ballColor = CHSV(random8(1, 255), 220, random8(100, 255));
  }
}

bool EffectBall::run(CRGB *ledarr, EffectWorker *opt){
  return ballRoutine(*&ledarr, &*opt);
}

bool EffectBall::ballRoutine(CRGB *leds, EffectWorker *param)
{
  byte _speed = getCtrlVal(1).toInt();
  byte _scale = getCtrlVal(2).toInt();

  if (_scale <= 85) 
    ballSize = map(_scale, 1, 85, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else if (_scale > 85 and _scale <= 170)
    ballSize = map(_scale, 170, 86, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else
    ballSize = map(_scale, 171, 255, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  

// каждые 5 секунд коррекция направления
  EVERY_N_SECONDS(5){
    //LOG(println,ballSize);
    for (uint8_t i = 0U; i < 2U; i++)
    {
      if(fabs(vectorB[i]) < 12)
        vectorB[i] += (float)random8(0U, 80U) / 10.0f - 4.0f;
      else if (vectorB[i] > 12)
        vectorB[i] -= (float)random8(10, 60) / 10.0f;
      else
        vectorB[i] += (float)random8(10, 60) / 10.0f;
      if(!(uint8_t)vectorB[i])
        vectorB[i] += vectorB[i] > 0 ? 0.25f : -0.25f;

     // ballColor = CHSV(random8(1, 255), 220, random8(100, 255));
    }
  }

  for (uint8_t i = 0U; i < 2U; i++)
  {
    coordB[i] += vectorB[i] * ((0.1f * (float)_speed) /255.0f);
    if ((int8_t)coordB[i] < 0)
    {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR) ballColor = CHSV(random8(1, 255), 220, random8(100, 255));
    }
  }
  if ((int8_t)coordB[0U] > (int16_t)(WIDTH - ballSize))
  {
    coordB[0U] = (WIDTH - ballSize);
    vectorB[0U] = -vectorB[0U];
    if (RANDOM_COLOR) ballColor = CHSV(random8(1, 255), 220, random8(100, 255));
  }
  if ((int8_t)coordB[1U] > (int16_t)(HEIGHT - ballSize))
  {
    coordB[1U] = (HEIGHT - ballSize);
    vectorB[1U] = -vectorB[1U];
    if (RANDOM_COLOR) ballColor = CHSV(random8(1, 255), 220, random8(100, 255));
  }

  if (_scale <= 85)  // при масштабе до 85 выводим кубик без шлейфа
    memset8( leds, 0, NUM_LEDS * 3);
  else if (_scale > 85 and _scale <= 170)
    fadeToBlackBy(leds, NUM_LEDS, 255 - (uint8_t)(10 * ((float)_speed) /255) + 30); // выводим кубик со шлейфом, длинна которого зависит от скорости.
  else
    fadeToBlackBy(leds, NUM_LEDS, 255 - (uint8_t)(10 * ((float)_speed) /255) + 15); // выводим кубик с длинным шлейфом, длинна которого зависит от скорости.

  for (float i = 0.0f; i < (float)ballSize; i+= 0.25f)
  {
    for (float j = 0.0f; j < (float)ballSize; j+=0.25f)
    {
      myLamp.drawPixelXYF(coordB[0U] + i, coordB[1U] + j, ballColor);
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

bool Effect3DNoise::run(CRGB *ledarr, EffectWorker *opt){
  #ifdef MIC_EFFECTS
    uint8_t mmf = isMicActive ? myLamp.getMicMapFreq() : 0;
    uint8_t mmp = isMicActive ? myLamp.getMicMapMaxPeak() : 0;
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


//----- Эффект "Прыгающие Мячики"
//  BouncingBalls2014 is a program that lets you animate an LED strip
//  to look like a group of bouncing balls
//  Daniel Wilson, 2014
//  https://github.com/githubcdr/Arduino/blob/master/bouncingballs/bouncingballs.ino
//  With BIG thanks to the FastLED community!
//  адаптация от SottNick
// перевод на субпиксельную графику kostyamat
bool EffectBBalls::run(CRGB *ledarr, EffectWorker *opt){
  if (csum != (scale^getCtrlVal(3).toInt())) {
    csum = scale^getCtrlVal(3).toInt();
    regen = true;
  }

  return bBallsRoutine(*&ledarr, &*opt);
}

bool EffectBBalls::bBallsRoutine(CRGB *leds, EffectWorker *param)
{
  if (regen) {
    FastLED.clear();
    if (scale <= 16) {
      bballsNUM_BALLS =  map(scale, 1, 16, 1, bballsMaxNUM_BALLS);
    } else {
      bballsNUM_BALLS =  map(scale, 32, 17, 1, bballsMaxNUM_BALLS);
    }
    for (int i = 0 ; i < bballsNUM_BALLS ; i++) {          // Initialize variables
      bballsCOLOR[i] = random8();
      bballsX[i] = (float)random8(10U, WIDTH*10) / 10.0f;
      bballsTLast[i] = millis();
      bballsPos[i] = 0.0f;                                 // Balls start on the ground
      bballsVImpact[i] = bballsVImpact0;                   // And "pop" up at vImpact0
      bballsCOR[i] = 0.90f - float(i) / pow(bballsNUM_BALLS, 2);
      bballsShift[i] = false;
    }
    regen = false;
  }

  if (scale <= 16) 
    FastLED.clear();
  else 
    fadeToBlackBy(leds, NUM_LEDS, 50);

  for (int i = 0 ; i < bballsNUM_BALLS ; i++) {
    bballsTCycle =  millis() - bballsTLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    bballsHi = 0.5f * bballsGRAVITY * pow( bballsTCycle / (float)(1550 - speed * 3) , 2.0 ) + bballsVImpact[i] * bballsTCycle / (float)(1525 - speed * 3);

    if ( bballsHi < 0 ) {
      bballsTLast[i] = millis();
      bballsHi = 0.0f;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      bballsVImpact[i] = bballsCOR[i] * bballsVImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR


      //if ( bballsVImpact[i] < 0.01 ) bballsVImpact[i] = bballsVImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
      if ( bballsVImpact[i] < 0.1 ) // сделал, чтобы мячики меняли свою прыгучесть и положение каждый цикл
      {
        bballsCOR[i] = 0.90 - ((float)random8(0U, 90U) /10.0f) / pow(random8(40U, 90U) / 10.0f, 2); // сделал, чтобы мячики меняли свою прыгучесть каждый цикл
        bballsShift[i] = bballsCOR[i] >= 0.89;                             // если мячик максимальной прыгучести, то разрешаем ему сдвинуться
        bballsVImpact[i] = bballsVImpact0;
      }
    }
    bballsPos[i] = bballsHi * (float)(HEIGHT - 1) / bballsH0;       // Map "h" to a "pos" integer index position on the LED strip
    if (bballsShift[i] > 0.0f && bballsPos[i] >= (float)HEIGHT - 1.6f) {                  // если мячик получил право, то пускай сдвинется на максимальной высоте 1 раз
      bballsShift[i] = 0.0f;
      if (bballsCOLOR[i] % 2 == 0) {                                       // чётные налево, нечётные направо
        if (bballsX[i] <= 0.0f) bballsX[i] = (float)(WIDTH - 1U);
        else bballsX[i] -= 0.5f;
      } else {
        if (bballsX[i] >= float(WIDTH - 1U)) bballsX[i] = 0.0f;
        else bballsX[i] += 0.5f;
      }
    }
    myLamp.drawPixelXYF(bballsX[i], bballsPos[i], CHSV(bballsCOLOR[i], 255, 255));
  }
  return true;
}

// ***** SINUSOID3 / СИНУСОИД3 *****
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
/*
  Sinusoid3 by Stefan Petrick (mod by Palpalych for GyverLamp 27/02/2020)
  read more about the concept: https://www.youtube.com/watch?v=mubH-w_gwdA
*/
bool EffectSinusoid3::run(CRGB *ledarr, EffectWorker *opt){
  return sinusoid3Routine(*&ledarr, &*opt);
}

bool EffectSinusoid3::sinusoid3Routine(CRGB *leds, EffectWorker *param)
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
bool EffectMetaBalls::run(CRGB *ledarr, EffectWorker *opt){
  return metaBallsRoutine(*&ledarr, &*opt);
}

bool EffectMetaBalls::metaBallsRoutine(CRGB *leds, EffectWorker *param)
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
      uint8_t  dx =  fabs(x - x1);
      uint8_t  dy =  fabs(y - y1);
      uint8_t dist = 2 * sqrt((dx * dx) + (dy * dy));

      dx =  fabs(x - x2);
      dy =  fabs(y - y2);
      dist += sqrt((dx * dx) + (dy * dy));

      dx =  fabs(x - x3);
      dy =  fabs(y - y3);
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

bool EffectSpiro::run(CRGB *ledarr, EffectWorker *opt){
  return spiroRoutine(*&ledarr, &*opt);
}

// ***** Эффект "Спираль"     ****
bool EffectSpiro::spiroRoutine(CRGB *leds, EffectWorker *param)
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

bool EffectComet::run(CRGB *ledarr, EffectWorker *opt){
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

bool EffectComet::rainbowCometRoutine(CRGB *leds, EffectWorker *param)
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

bool EffectComet::rainbowComet3Routine(CRGB *leds, EffectWorker *param)
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

bool EffectPrismata::run(CRGB *ledarr, EffectWorker *opt){
  return prismataRoutine(*&ledarr, &*opt);
}

bool EffectPrismata::prismataRoutine(CRGB *leds, EffectWorker *param)
{
  if (curPalette == nullptr) {
    return false;
  }

  EVERY_N_MILLIS(100) {
    spirohueoffset += 1;
  }

  fadeToBlackBy(leds, NUM_LEDS, 256U - getCtrlVal(3).toInt()); // делаем шлейф
  
  for (float x = 0.0f; x <= (float)WIDTH - 1; x += 0.25f) {
      float y = (float)beatsin8((uint8_t)x + 1 * speed/5, 0, (HEIGHT-1)* 4) / 4.0f;
      myLamp.drawPixelXYF(x, y, ColorFromPalette(*curPalette, ((uint8_t)x + spirohueoffset) * 4));
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

bool EffectFlock::run(CRGB *ledarr, EffectWorker *opt){
  return flockRoutine(*&ledarr, &*opt);
}

bool EffectFlock::flockRoutine(CRGB *leds, EffectWorker *param) {
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

bool EffectSwirl::run(CRGB *ledarr, EffectWorker *opt){
  return swirlRoutine(*&ledarr, &*opt);
}

#define e_swi_BORDER (1U)  // размытие экрана за активный кадр
bool EffectSwirl::swirlRoutine(CRGB *leds, EffectWorker *param)
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

bool EffectDrift::run(CRGB *ledarr, EffectWorker *opt){
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

bool EffectDrift::incrementalDriftRoutine(CRGB *leds, EffectWorker *param)
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
bool EffectDrift::incrementalDriftRoutine2(CRGB *leds, EffectWorker *param)
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

bool EffectFreq::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  myLamp.setMicAnalyseDivider(0); // отключить авто-работу микрофона, т.к. тут все анализируется отдельно, т.е. не нужно выполнять одну и ту же работу дважды
  return freqAnalyseRoutine(*&ledarr, &*opt);
}

bool EffectFreq::freqAnalyseRoutine(CRGB *leds, EffectWorker *param)
{
  float samp_freq;
  double last_freq = 0;
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
  if(paletteIdx){
    _ptPallete = ptPallete;
    _pos = palettepos;
    _curVal = paletteIdx;
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

bool EffectTwinkles::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  return twinklesRoutine(*&ledarr, &*opt);
}

bool EffectTwinkles::twinklesRoutine(CRGB *leds, EffectWorker *param)
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
void EffectRadar::load(){
  palettesload();    // подгружаем дефолтные палитры
}

bool EffectRadar::run(CRGB *ledarr, EffectWorker *opt){
  return radarRoutine(*&ledarr, &*opt);
}

bool EffectRadar::radarRoutine(CRGB *leds, EffectWorker *param)
{
  bool subPix = false;
  if (getCtrlVal(3) == F("true"))
  { // переключатель в true
    subPix = true;
  }
  if (curPalette == nullptr)
  {
    return false;
  }
  if (subPix)
  {
    fadeToBlackBy(leds, NUM_LEDS, 5 + 20 * (float)speed / 255);
    for (float offset = 0.0f; offset <= (float)(WIDTH - 1) / 2U; offset += 0.5f)
    {
      float x = (float)EffectMath::mapsincos8(MAP_COS, eff_theta, offset * 10, (WIDTH - 1U) * 10 - offset * 10) / 10.0f;
      float y = (float)EffectMath::mapsincos8(MAP_SIN, eff_theta, offset * 10, (WIDTH - 1U) * 10 - offset * 10) / 10.0f;
      CRGB color = ColorFromPalette(*curPalette, hue);
      myLamp.drawPixelXYF(x, y, color);
      //hue = beatsin8(100);
    }
  }
  else
  {
    uint8_t scale = palettescale; // диапазоны внутри палитры, влияют на степень размытия хвоста
    myLamp.blur2d(beatsin8(5U, 3U, 10U));
    myLamp.dimAll(255U - (0 + scale * 3));

    for (uint8_t offset = 0U; offset < WIDTH / 2U - 1U; offset++)
    {
      myLamp.setLeds(myLamp.getPixelNumber(EffectMath::mapsincos8(MAP_COS, eff_theta, offset, (WIDTH - 1U) - offset),
                                           EffectMath::mapsincos8(MAP_SIN, eff_theta, offset, (WIDTH - 1U) - offset)),
                                           ColorFromPalette(*curPalette, 255U - (offset * 16U + eff_offset)));
    }
  }
  EVERY_N_MILLIS(24)
  {
    eff_theta += 5.5 * (speed / 255.0) + 1;
    eff_offset += 3.5 * ((255 - speed) / 255.0) + 1;
    if (subPix) {
      hue = random8();
      hue++;
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

bool EffectWaves::run(CRGB *ledarr, EffectWorker *opt){

  waveCount = (scale <= 127 ? (scale <= 63 ? 0 : 1) : (scale >= 191 ? 1 : 0));
  waveRotation = (scale <= 127 ? 0 : 1);  /* сильно перебрал управление эффнктом
  не стал мапить на 4-й ползунок, потому как в этом случае "масштаб" превращается в переключатель на 4-ре позиции. 
*/
   
  return wavesRoutine(*&ledarr, &*opt);
}

bool EffectWaves::wavesRoutine(CRGB *leds, EffectWorker *param)
{

  WavesPaletteMap(palettes,  (scale <= 127 ? (scale <= 63 ? map(scale, 1, 63, 0, 7) : map(scale, 64, 127, 0, 7)) : (scale >= 191 ? map(scale, 191, 255, 0, 7) : map(scale, 128, 190, 0, 7))));

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
  palettes.push_back(&PotassiumFireColors_p);
  palettes.push_back(&SodiumFireColors_p);
  palettes.push_back(&LithiumFireColors_p);
  palettes.push_back(&RubidiumFireColors_p);
  palettes.push_back(&NormalFire_p);
  palettes.push_back(&HeatColors2_p);
  palettes.push_back(&WoodFireColors_p);
  palettes.push_back(&CopperFireColors_p);
  palettes.push_back(&AlcoholFireColors_p);
  palettes.push_back(&WhiteBlackColors_p);

  usepalettes = true; // активируем "переключатель" палитр
  scale2pallete();    // выбираем палитру согласно "шкале"

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(256));
}

bool EffectFire2012::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  return fire2012Routine(*&ledarr, &*opt);
}

bool EffectFire2012::fire2012Routine(CRGB *ledarr, EffectWorker *opt)
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
bool EffectRain::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  /*EVERY_N_SECONDS(3){
    LOG(printf_P, PSTR("speed: %d, scale: %d\n"), speed, scale);
  }*/

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

bool EffectRain::coloredRainRoutine(CRGB *leds, EffectWorker *param) // внимание! этот эффект заточен на работу бегунка Масштаб в диапазоне от 0 до 255. пока что единственный.
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

bool EffectRain::simpleRainRoutine(CRGB *leds, EffectWorker *param)
{
  CRGB solidRainColor = CRGB(60, 80, 90);
  //uint8_t Scale = scale;
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,2,60), 10, solidRainColor, true, true, false);
  rain(60, 180, scale, 30, solidRainColor, true, true, false);
  return true;
}

bool EffectRain::stormyRainRoutine(CRGB *leds, EffectWorker *param)
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
bool EffectFire2018::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  return fire2018Routine(*&ledarr, &*opt);
}

bool EffectFire2018::fire2018Routine(CRGB *leds, EffectWorker *param)
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
      //dim = dim / 1.7 * constrain(0.05*myLamp.effects.getBrightness()+0.01,0.01,1.0); //точно нужен прямой доступ?
      dim = dim / 1.7 * constrain(0.05*brightness+0.01,0.01,1.0);
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
bool EffectRingsLock::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  if (csum != (scale^getCtrlVal(3).toInt()))
    ringsSet();
  return ringsRoutine(*&ledarr, &*opt);
}

void EffectRingsLock::load(){
  palettesload();
  csum = scale^getCtrlVal(3).toInt();
  ringsSet();
}

// Установка параметров колец
void EffectRingsLock::ringsSet(){
  if (curPalette == nullptr) {
    return;
  }
  csum = scale^getCtrlVal(3).toInt();
  ringWidth = map(scale, 1, 255, 1, 8); // толщина кольца от 1 до 8 для каждой из палитр
  ringNb = (float)HEIGHT / ringWidth + ((HEIGHT % ringWidth == 0U) ? 0U : 1U)%HEIGHT; // количество колец
  upRingHue = ringWidth - (ringWidth * ringNb - HEIGHT) / 2U; // толщина верхнего кольца. может быть меньше нижнего
  downRingHue = HEIGHT - upRingHue - (ringNb - 2U) * ringWidth; // толщина нижнего кольца = всё оставшееся

  
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

bool EffectRingsLock::ringsRoutine(CRGB *leds, EffectWorker *param)
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

bool EffectCube2d::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  if (csum != (scale^getCtrlVal(3).toInt()))
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
          color = ColorFromPalette(*curPalette, random8(17, 255)); // Не хотелось бы получать черные кубики, поэтому выбор цвета от выше 16-ти
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
  csum = scale^getCtrlVal(3).toInt();
  //end
}

bool EffectCube2d::cube2dRoutine(CRGB *leds, EffectWorker *param)
{
  if (curPalette == nullptr) {
    return false;
  }
  if (!pauseSteps){
    pauseSteps--;
    return false; // пропускаем кадры после прокрутки кубика (делаем паузу)
  }

  if (!shiftSteps) {  // если цикл вращения завершён
    // ====== определяем направление прокруток на будущий цикл
    pauseSteps = CUBE2D_PAUSE_FRAMES;
    direction = random8()%2;  // сдвиг 0 - строки, 1 - столбцы
    moveItems.resize(direction ? cntX : cntY, 0);

    for (uint8_t i=0; i<(direction ? cntX : cntY); i++){
      moveItems.at(i)= random8()%3; // 1 - fwd, 0 - bkw, 2 - none 
    }

    shiftSteps = ((direction ? sizeY : sizeX)+1) * random8(direction ? cntY : cntX);  // такой рандом добавляет случайную задержку в паузу, попадая на "0"
    return false;
  }


  //двигаем, что получилось...
  shiftSteps--;

  for (uint8_t i=0; i<(direction ? cntX : cntY); i++){
    if (moveItems.at(i)==2) // skip some items
      continue;

    direction ? cube2dmoveCols(i, moveItems.at(i)) : cube2dmoveRows(i, moveItems.at(i));
  }

  return true;
}

// идём по горизонтали, крутим по вертикали (столбцы двигаются)
void EffectCube2d::cube2dmoveCols(uint8_t moveItem, bool movedirection){
      uint16_t x, anim0;
  CRGB color, color2;

      x = moveItem * (sizeX + 1U);
      anim0 = 0;

      // если крутим столбец вниз
      if (!movedirection)
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
          return;
        }

      // крутим столбец вверх
      color = myLamp.getPixColorXY(x,anim0+fieldY-1);                            // берём цвет от верхней строчки
      for (uint8_t k = anim0+fieldY-1; k > anim0 ; k--)
      {
        color2 = myLamp.getPixColorXY(x,k-1);                                   // берём цвет от строчки под нашей
        for (uint8_t m = x; m < x + sizeX; m++)
          myLamp.setLeds(myLamp.getPixelNumber(m,k), color2);                           // копируем его на всю нашу строку
      }
      for   (uint8_t m = x; m < x + sizeX; m++)
        myLamp.setLeds(myLamp.getPixelNumber(m, anim0), color);                         // цвет верхней строчки копируем на всю нижнюю
}

// идём по вертикали, крутим по горизонтали (строки двигаются)
void EffectCube2d::cube2dmoveRows(uint8_t moveItem, bool movedirection){
  uint16_t y, anim0;
  CRGB color, color2;

  y = moveItem * (sizeY + 1U);
  anim0 = 0;

  // крутим строку влево
  if (!movedirection)
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
    return;
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

}

//--------------
bool EffectTime::run(CRGB *ledarr, EffectWorker *opt){
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

bool EffectTime::timePrintRoutine(CRGB *leds, EffectWorker *param)
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
bool EffectMStreamSmoke::run(CRGB *ledarr, EffectWorker *opt){
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
bool EffectMStreamSmoke::multipleStreamSmokeRoutine(CRGB *leds, EffectWorker *param)
{
  CRGB color;

  myLamp.dimAll(254);

  //String var = myLamp.effects.getValue(myLamp.effects.getCurrent()->param, F("R"));

  int val;
  int ctrl3 = getCtrlVal(3).toInt();

  if(ctrl3){
    val = ((int)(6*ctrl3/255.1))%6;
    xSmokePos=xSmokePos+(ctrl3%43)/42.0+0.01;
    xSmokePos2=xSmokePos2+(ctrl3%43)/84.0+0.01;
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


// ============= Эффект "Огненная лампа" =================
void EffectFire::load(){
  pcnt = 0U;
  generateLine();
}

bool EffectFire::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;

  return fireRoutine(*&ledarr, &*opt);
}

bool EffectFire::fireRoutine(CRGB *leds, EffectWorker *param)
{
  if (pcnt >= 100) {                                  // внутренний делитель кадров для поднимающегося пламени
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
  uint8_t baseHue = (scale - 1U) * 2.6; // может так хватит?
#else
  uint8_t baseHue = isColored ? 255U : 0U;
#endif
  uint8_t baseSat = ( scale < 255) ? 255U : 0U;  // вычисление базового оттенка // может так хватит?
  
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

// ----------- Эффекты "Пикассо" (c) obliterator
void EffectPicasso::generate(bool reset){
  double minSpeed = 0.2, maxSpeed = 0.8;
  unsigned num = map(scale, 0U, 255U, 6U, sizeof(particles) / sizeof(*particles));

  for (unsigned i = numParticles; i < num; i++) {
    Particle *curr = (Particle *)&particles[i];
    curr->position_x = random8(0, WIDTH);
    curr->position_y = random8(0, HEIGHT);

    curr->speed_x = +((-maxSpeed / 3) + (maxSpeed * (float)random(1, 100) / 100));
    curr->speed_x += curr->speed_x > 0 ? minSpeed : -minSpeed;

    curr->speed_y = +((-maxSpeed / 2) + (maxSpeed * (float)random(1, 100) / 100));
    curr->speed_y += curr->speed_y > 0 ? minSpeed : -minSpeed;

    curr->color = CHSV(random8(1U, 255U), 255U, 255U);
    curr->hue_next = curr->color.h;
  };

  for (unsigned i = 0; i < num; i++) {
    if (reset) {
      particles[i].hue_next = random8(1U, 255U);
      particles[i].hue_step = (particles[i].hue_next - particles[i].color.h) / 25;
    }
    if (particles[i].hue_next != particles[i].color.h && particles[i].hue_step) {
      particles[i].color.h += particles[i].hue_step;
    }
  }
  numParticles = num;
}

void EffectPicasso::position(){
  float speedfactor = speed/255.0+0.1;
  for (unsigned i = 0; i < numParticles; i++) {
    Particle *curr = (Particle *)&particles[i];
    if (curr->position_x + curr->speed_x > WIDTH || curr->position_x + curr->speed_x < 0) {
      curr->speed_x = -curr->speed_x;
    }

    if (curr->position_y + curr->speed_y > HEIGHT || curr->position_y + curr->speed_y < 0) {
      curr->speed_y = -curr->speed_y;
    }

    curr->position_x += curr->speed_x*speedfactor;
    curr->position_y += curr->speed_y*speedfactor;
  };
}

bool EffectPicasso::picassoRoutine(CRGB *leds, EffectWorker *param){
  generate();
  position();

  for (unsigned i = 0; i < numParticles - 2; i+=2) {
    Particle *p1 = (Particle *)&particles[i];
    Particle *p2 = (Particle *)&particles[i + 1];
    myLamp.drawLine(p1->position_x, p1->position_y, p2->position_x, p2->position_y, p1->color);
  }

  EVERY_N_MILLIS(20000){
    generate(true);
  }

  myLamp.blur2d(80);
  return true;
}

bool EffectPicasso::picassoRoutine2(CRGB *leds, EffectWorker *param){
  generate();
  position();
  myLamp.dimAll(180);

  for (unsigned i = 0; i < numParticles - 1; i++) {
    Particle *p1 = (Particle *)&particles[i];
    Particle *p2 = (Particle *)&particles[i + 1];
    myLamp.drawLineF(p1->position_x, p1->position_y, p2->position_x, p2->position_y, p1->color);
  }

  EVERY_N_MILLIS(20000){
    generate(true);
  }

  myLamp.blur2d(80);

  return true;
}

bool EffectPicasso::picassoRoutine3(CRGB *leds, EffectWorker *param){
  generate();
  position();
  myLamp.dimAll(180);

  for (unsigned i = 0; i < numParticles - 2; i+=2) {
    Particle *p1 = (Particle *)&particles[i];
    Particle *p2 = (Particle *)&particles[i + 1];
    myLamp.drawCircleF(std::fabs(p1->position_x - p2->position_x), std::fabs(p1->position_y - p2->position_y), std::fabs(p1->position_x - p1->position_y), p1->color);
  }

  EVERY_N_MILLIS(20000){
    generate(true);
  }

  myLamp.blur2d(80);

  return true;
}

bool EffectPicasso::run(CRGB *ledarr, EffectWorker *opt){
  // if (dryrun())
  //   return false;
  if (effect == EFF_PICASSO) {
    return picassoRoutine(*&ledarr, &*opt);
  }
  if (effect == EFF_PICASSO2) {
    return picassoRoutine2(*&ledarr, &*opt);
  }
  return picassoRoutine3(*&ledarr, &*opt);
}

// -------- Эффект "Прыгуны" (c) obliterator
void EffectLeapers::restart_leaper(EffectLeapers::Leaper * l) {
  // leap up and to the side with some random component
#ifdef MIC_EFFECTS
  uint8_t mic = myLamp.getMicMaxPeak();
  float rand = random8(1, 50 + _rv * 4);
  l->xd = (1 * (float)(isMicActive ? 25 + mic : rand) / 100.0);
  l->yd = (2 * (float)(isMicActive ? 25 + mic : rand) / 100.0);
#else
  l->xd = (1 * (float)random8(1, 50 + _rv * 4) / 100);
  l->yd = (2 * (float)random8(1, 100 + _rv * 3) / 100);
#endif

  // for variety, sometimes go 50% faster
  if (random8() < 12) {
    l->xd += l->xd * 0.5;
    l->yd += l->yd * 0.5;
  }

  // leap towards the centre of the screen
  if (l->x > (WIDTH / 2)) {
    l->xd = -l->xd;
  }
}

void EffectLeapers::move_leaper(EffectLeapers::Leaper * l) {
#define GRAVITY            0.06
//float GRAVITY  = EffectMath::fmap(speed, 1, 255, 0.01f, 0.6f);
#define SETTLED_THRESHOLD  0.1
#define WALL_FRICTION      0.95
#define WIND               0.95    // wind resistance

  float speedfactor = (speed/512.0);
  float speedfactor2 = speedfactor + 0.33;
  
  l->x += l->xd*speedfactor2;
  l->y += l->yd*speedfactor2;

  // bounce off the floor and ceiling?
  if (l->y < 0 || l->y > HEIGHT - 1) {
    l->yd = (-l->yd * WALL_FRICTION);
    l->xd = ( l->xd * WALL_FRICTION);
    l->y += l->yd*speedfactor;
    if (l->y < 0) l->y = 0;
    // settled on the floor?
    if (l->y <= SETTLED_THRESHOLD && abs(l->yd) <= SETTLED_THRESHOLD) {
      restart_leaper(l);
    }
  }

  // bounce off the sides of the screen?
  if (l->x <= 0 || l->x >= WIDTH - 1) {
    l->xd = (-l->xd * WALL_FRICTION);
    if (l->x <= 0) {
      l->x = l->xd*speedfactor;
    } else {
      l->x = WIDTH - 1 - l->xd;
    }
  }

  l->yd -= GRAVITY;
  l->xd *= WIND;
  l->yd *= WIND;
}

void EffectLeapers::generate(bool reset){
  unsigned num = map(scale, 0U, 255U, 6U, sizeof(leapers) / sizeof(*leapers));

  for (unsigned i = numParticles; i < num; i++) {
    Leaper *curr = (Leaper *)&leapers[i];
    curr->x = random8(0, WIDTH);
    curr->y = random8(0, HEIGHT);

    curr->color = CHSV(random(1U, 255U), 255U, 255U);
  };
  numParticles = num;
}

bool EffectLeapers::leapersRoutine(CRGB *leds, EffectWorker *param){
  _rv = getCtrlVal(3).toInt() ? getCtrlVal(3).toInt() : 13;
  generate();

  myLamp.dimAll(0);

  for (unsigned i = 0; i < numParticles; i++) {
    move_leaper(&leapers[i]);
    myLamp.drawPixelXYF(leapers[i].x, leapers[i].y, leapers[i].color);
  };

  myLamp.blur2d(20);
  return true;
}

bool EffectLeapers::run(CRGB *ledarr, EffectWorker *opt){
  return leapersRoutine(*&ledarr, &*opt);
}

// ------- Эффект "Вихри"
bool EffectWhirl::run(CRGB *ledarr, EffectWorker *opt){
  if (dryrun())
    return false;
  return whirlRoutine(*&ledarr, &*opt);
}

void EffectWhirl::load(){
  palettesload();    // подгружаем дефолтные палитры
  ff_x = random16();
  ff_y = random16();
  ff_z = random16();
  for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
      boids[i] = Boid(random(WIDTH), 0);
  }
}

bool EffectWhirl::whirlRoutine(CRGB *leds, EffectWorker *param) {
#ifdef MIC_EFFECTS
  micPick = isMicActive ? myLamp.getMicMaxPeak() : 0;
  myLamp.dimAll(isMicActive ? /*(100 + micPick*2)*/ 250U : 250U);
#else
  myLamp.dimAll(250U);
#endif

  for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
    Boid * boid = &boids[i];
    
    int ioffset = ff_scale * boid->location.x;
    int joffset = ff_scale * boid->location.y;

    byte angle = inoise8(ff_x + ioffset, ff_y + joffset, ff_z);

    boid->velocity.x = (float) sin8(angle) * 0.0078125 - 1.0;
    boid->velocity.y = -((float)cos8(angle) * 0.0078125 - 1.0);
    boid->update();
#ifdef MIC_EFFECTS    
    myLamp.drawPixelXY(boid->location.x, boid->location.y, isMicActive ? CHSV(myLamp.getMicMapFreq(), 255-micPick, constrain(micPick * EffectMath::fmap(scale, 1.0f, 255.0f, 1.25f, 5.0f), 48, 255)) : ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу
#else
    myLamp.drawPixelXY(boid->location.x, boid->location.y, ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу
#endif
    if (boid->location.x < 0 || boid->location.x >= WIDTH || boid->location.y < 0 || boid->location.y >= HEIGHT) {
      boid->location.x = random(WIDTH);
      boid->location.y = 0;
    }
  }
#ifdef MIC_EFFECTS
  myLamp.blur2d(isMicActive ? /*constrain(micPick * 2, 10, 100)*/ 30U : 30U);
#else
  myLamp.blur2d(30U);
#endif
  EVERY_N_MILLIS(200) {
    hue++;
  }

  ff_x += ff_speed;
  ff_y += ff_speed;
  ff_z += ff_speed;
  return true;
}

// ------------- цвет + вода в бассейне ------------------
// (с) SottNick. 03.2020
bool EffectAquarium::run(CRGB *ledarr, EffectWorker *opt) {
  if (dryrun())
    return false;

  return aquariumRoutine(*&ledarr, &*opt);
}

bool EffectAquarium::aquariumRoutine(CRGB *leds, EffectWorker *param) {
  bool glare = false;
  if(getCtrlVal(3)==F("true")) { // переключатель в true
    glare = true;
  }

    for (int16_t i = 0U; i < NUM_LEDS; i++)
    {
#ifdef MIC_EFFECTS
      if (isMicActive) {
        hue = myLamp.getMicMapFreq();
        leds[i] = CHSV(hue, 
          255U, 
          constrain(myLamp.getMicMaxPeak() * EffectMath::fmap(scale, 1.0f, 255.0f, 1.25f, 5.0f), 
          48, 
          255)
          ); 
      }
      else 
        leds[i] = CHSV(hue, 255U, 255U);
#else
      leds[i] = CHSV(hue, 255U, 255U);
#endif
    }
    deltaHue = 0U;
    deltaHue2 = 0U;

  if (glare) // если регулятор масштаб на минимуме, то будет работать старый эффект "цвет" (без анимации бликов воды)
  {
    myLamp.dimAll(200);
    if (step > 24U) // количество кадров в анимации -1 (отсчёт с нуля)
      step = 0U;
    if (step > 0U && step < 3U) // пару раз за цикл анимации двигаем текстуру по радиусу лампы. а может и не двигаем. как повезёт
    {
      if (random(2U) == 0U)
      {
        deltaHue++;
        if (deltaHue > 31U) deltaHue = 0U;
      }
    }
    if (step > 11U && step < 14U) // пару раз за цикл анимации двигаем текстуру по вертикали. а может и не двигаем. как повезёт
    {
      if (random(2U) == 0U)
      {
        deltaHue2++;
        if (deltaHue2 > 31U) deltaHue2 = 0U;
      }
    }
#ifdef MIC_EFFECTS
    byte _video = isMicActive ? constrain(myLamp.getMicMaxPeak() * EffectMath::fmap(scale, 1.0f, 255.0f, 1.25f, 5.0f), 48U, 255U) : 255;
#else
    byte _video = 255;
#endif
    for (byte x = 0; x < WIDTH ; x++) {
      for (byte y = 0; y < HEIGHT; y++) {
        // y%32, x%32 - это для масштабирования эффекта на лампы размером большим, чем размер анимации 32х32, а также для произвольного сдвига текстуры
        /*leds[XY(x, y)] = */ 
        myLamp.drawPixelXY(x, y, CHSV(hue, 255U - pgm_read_byte(&aquariumGIF[step][(y + deltaHue2) % 32U][(x + deltaHue) % 32U]) * CAUSTICS_BR / 100U, _video));
        // чтобы регулятор Масштаб начал вместо цвета регулировать яркость бликов, нужно закомментировать предыдущую строчку и раскоментировать следующую
        //        leds[XY(x, y)] = CHSV(158U, 255U - pgm_read_byte(&aquariumGIF[step][(y+deltaHue2)%32U][(x+deltaHue)%32U]) * modes[currentMode].Scale / 100U, 255U);
      }
    }
    step++;
  } 
 
  if (speed == 1) { 
    hue = scale;
  }
  else {
    hue +=1;
  }
  return true;
}             

// ------- Эффект "Звезды"
bool EffectStar::run(CRGB *ledarr, EffectWorker *opt){

  return starRoutine(*&ledarr, &*opt);
}

void EffectStar::load(){
  palettesload();    // подгружаем дефолтные палитры
}

void EffectStar::drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler)// random multipoint star
{
  radius2 = 255.0 / points;
  for (int i = 0; i < points; i++)
  {
#ifdef MIC_EFFECTS
    myLamp.drawLine(xlocl + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), isMicActive ? CHSV(koler+myLamp.getMicMapFreq(), 255-micPick, constrain(micPick * EffectMath::fmap(scale, 1.0f, 255.0f, 1.25f, 5.0f), 48, 255)) : ColorFromPalette(*curPalette, koler));
    myLamp.drawLine(xlocl + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), isMicActive ? CHSV(koler+myLamp.getMicMapFreq(), 255-micPick, constrain(micPick * EffectMath::fmap(scale, 1.0f, 255.0f, 1.25f, 5.0f), 48, 255)) : ColorFromPalette(*curPalette, koler));

#else
    myLamp.drawLine(xlocl + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));
    myLamp.drawLine(xlocl + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));
#endif
    }
}

bool EffectStar::starRoutine(CRGB *leds, EffectWorker *param) {
  
if (setup) { // однократная настройка при старте эффекта
  float _speed = speed;
  counter = 0.0;
  // driftx = random8(4, WIDTH - 4);//set an initial location for the animation center
  // drifty = random8(4, HEIGHT - 4);// set an initial location for the animation center
  
  // стартуем с центра, раз это единственная причина перезапуска по масштабу :)
  driftx = (float)WIDTH/2.0;
  drifty = (float)HEIGHT/2.0;

  cangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f;//angle of movement for the center of animation gives a float value between -1 and 1
  sangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f;//angle of movement for the center of animation in the y direction gives a float value between -1 and 1
  //shifty = random (3, 12);//how often the drifter moves будет CENTER_DRIFT_SPEED = 6

  stars_count = WIDTH / 2U;
  if (stars_count > STARS_NUM) stars_count = STARS_NUM;
  for (uint8_t num = 0; num < stars_count; num++) {
    points[num] = random8(3, 9); // количество углов в звезде
    delay[num] = _speed / 5 + (num << 2) + 1U; // задержка следующего пуска звезды
    color[num] = random8();
  }
  setup = false;
}

#ifdef MIC_EFFECTS
  micPick = myLamp.getMicMaxPeak();
  //myLamp.dimAll(isMicActive ? micPick*2 : 90);
  fadeToBlackBy(leds, NUM_LEDS, 255U - (isMicActive ? micPick*2 : 90)); // работает быстрее чем dimAll
#else
  fadeToBlackBy(leds, NUM_LEDS, 165);
  //myLamp.dimAll(90);
#endif

  float _scalefactor = ((float)speed/380.0+0.05);

  counter+=_scalefactor; // определяет то, с какой скоростью будет приближаться звезда

  if (driftx > (WIDTH - spirocenterX / 2U))//change directin of drift if you get near the right 1/4 of the screen
    cangle = 0 - fabs(cangle);
  if (driftx < spirocenterX / 2U)//change directin of drift if you get near the right 1/4 of the screen
    cangle = fabs(cangle);
  if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0)
    driftx = driftx + (cangle * _scalefactor);//move the x center every so often

  if (drifty > ( HEIGHT - spirocenterY / 2U))// if y gets too big, reverse
    sangle = 0 - fabs(sangle);
  if (drifty < spirocenterY / 2U) // if y gets too small reverse
    sangle = fabs(sangle);
  //if ((counter + CENTER_DRIFT_SPEED / 2U) % CENTER_DRIFT_SPEED == 0)
  if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0)
    drifty =  drifty + (sangle * _scalefactor);//move the y center every so often

  for (uint8_t num = 0; num < stars_count; num++) {
    if (counter >= delay[num])//(counter >= ringdelay)
    {
      if (counter - delay[num] <= WIDTH + 5) { 
        EffectStar::drawStar(driftx, drifty, 2 * (counter - delay[num]), (counter - delay[num]), points[num], STAR_BLENDER + color[num], color[num]);
        color[num]+=_scalefactor; // в зависимости от знака - направление вращения
      }
      else
        delay[num] = counter + (stars_count << 1) + 1U;//random8(50, 99);//modes[currentMode].Scale;//random8(50, 99); // задержка следующего пуска звезды
    }
  }
#ifdef MIC_EFFECTS
  myLamp.blur2d(isMicActive ? micPick/2 : 30U); //fadeToBlackBy() сам блурит, уменьшил блур под микрофон
#else
  myLamp.blur2d(30U);
#endif
  return true;
}
                                                                                                                                    
//---------- Эффект "Фейерверк" адаптация kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373

CRGB &piXY(CRGB *leds, byte x, byte y) {
  x -= PIXEL_X_OFFSET;
  y -= PIXEL_Y_OFFSET;
  if( x < WIDTH && y < HEIGHT) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (WIDTH - 1) - x;
      return leds[(y * WIDTH) + reverseX];
    } else {
      // Even rows run forwards
      return leds[ (y * HEIGHT) + x ] ;
    }
  } else
    return leds[0];
}

void screenscale( accum88 a, byte N, byte& screen, byte& screenerr)
{
  byte ia = a >> 8;
  screen = scale8( ia, N);
  byte m = screen * (256 / N);
  screenerr = (ia - m) * scale8(255,N);
  return;
}

int16_t scale15by8_local( int16_t i, fract8 _scale )
{
    int16_t result;
    result = (int32_t)((int32_t)i * _scale) / 256;
    return result;
}

void Dot::Skyburst( accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor)
  {
    yv = (int16_t)0 + (int16_t)random16(1500) - (int16_t)500;
    xv = basedv + (int16_t)random16(2000) - (int16_t)1000;
    y = basey;
    x = basex;
    color = basecolor;
    color *= 20;
    theType = 2;
    show = 1;
  }

void Dot::GroundLaunch(DOTS_STORE &store)
  {
    yv = 600 + random16(300 + (25 * HEIGHT));
    if(yv > 1200) yv = 1200;
    xv = (int16_t)random16(600) - (int16_t)300;
    y = 0;
    x = 0x8000;
    hsv2rgb_rainbow( CHSV( random8(), 240, 200), color);
    //color = CHSV(random8(), 240, 200);
    show = 1;
  }

  void Dot::Move(DOTS_STORE &store)
  {
    if( !show) return;
    yv -= gGravity;
    xv = scale15by8_local( xv, gDrag);    
    yv = scale15by8_local( yv, gDrag);

    if( theType == 2) {
      xv = scale15by8_local( xv, gDrag);    
      yv = scale15by8_local( yv, gDrag);
      color.nscale8( 255);
      if( !color) {
        show = 0;
      }
    }
    // if we'd hit the ground, bounce
    if( yv < 0 && (y < (-yv)) ) {
      if( theType == 2 ) {
        show = 0;
      } else {
        yv = -yv;
        yv = scale15by8_local( yv, gBounce);
        if( yv < 500 ) {
          show = 0;
        }
      }
    }   
    if( (yv < -300) /* && (!(oyv < 0))*/ ) {
      // pinnacle
      if( theType == 1 ) {

        if( (y > (uint16_t)(0x8000)) && (random8() < 64)) {
          // boom
          LEDS.showColor( CRGB::WhiteSmoke);
          LEDS.showColor( CRGB::Black);
        }

        show = 0;

        store.gSkyburst = true;
        store.gBurstx = x;
        store.gBursty = y;
        store.gBurstxv = xv;
        store.gBurstyv = yv;
        store.gBurstcolor = color;        
      }
    }
    if( theType == 2) {
      if( ((xv >  0) && (x > xv)) ||
          ((xv < 0 ) && (x < (0xFFFF + xv))) )  {
        x += xv;
      } else {
        show = 0;
      }
    } else {
      x += xv;
    }
    y += yv;
    
  }

void Dot::Draw(CRGB *leds)
  {
    if( !show) return;
    byte ix, xe, xc;
    byte iy, ye, yc;
    screenscale( x, MODEL_WIDTH, ix, xe);
    screenscale( y, MODEL_HEIGHT, iy, ye);
    yc = 255 - ye;
    xc = 255 - xe;
    
    CRGB c00 = CRGB( dim8_video( scale8( scale8( color.r, yc), xc)), 
                     dim8_video( scale8( scale8( color.g, yc), xc)), 
                     dim8_video( scale8( scale8( color.b, yc), xc))
                     );
    CRGB c01 = CRGB( dim8_video( scale8( scale8( color.r, ye), xc)), 
                     dim8_video( scale8( scale8( color.g, ye), xc)), 
                     dim8_video( scale8( scale8( color.b, ye), xc))
                     );

    CRGB c10 = CRGB( dim8_video( scale8( scale8( color.r, yc), xe)), 
                     dim8_video( scale8( scale8( color.g, yc), xe)), 
                     dim8_video( scale8( scale8( color.b, yc), xe))
                     );
    CRGB c11 = CRGB( dim8_video( scale8( scale8( color.r, ye), xe)), 
                     dim8_video( scale8( scale8( color.g, ye), xe)), 
                     dim8_video( scale8( scale8( color.b, ye), xe))
                     );

    piXY(leds, ix, iy) += c00;
    piXY(leds, ix, iy + 1) += c01;
    piXY(leds, ix + 1, iy) += c10;
    piXY(leds, ix + 1, iy + 1) += c11;
  }

bool EffectFireworks::run(CRGB *ledarr, EffectWorker *opt) 
{
  EVERY_N_MILLIS(15) {
    random16_add_entropy(analogRead(A0));
    return fireworksRoutine(*&ledarr, &*opt);
  }
    return false;
}

void EffectFireworks::sparkGen() {
  for (byte c = 0; c < map(scale, 1, 255, 1, SPARK); c++) {
    if( gDot[c].show == 0 ) {
      if( launchcountdown[c] == 0) {
        gDot[c].GroundLaunch(this->store);
        gDot[c].theType = 1;
        launchcountdown[c] = random16(865 - speed*3) + 1;
      } else {
        launchcountdown[c] --;
      }
    }
  }
  
  if( store.gSkyburst) {
    byte nsparks = random8( NUM_SPARKS / 2, NUM_SPARKS + 1);
    for( byte b = 0; b < nsparks; b++) {
      gSparks[b].Skyburst( store.gBurstx, store.gBursty, store.gBurstyv, store.gBurstcolor);
      store.gSkyburst = false;
    }
  }
  //myLamp.blur2d(20);
}

bool EffectFireworks::fireworksRoutine(CRGB *leds, EffectWorker *param) 
{
  sparkGen();
  memset8( leds, 0, NUM_LEDS * 3);

#if 1
  CRGB sky1(0,0,2);
  CRGB sky2(2,0,2);
   for( uint16_t v = 0; v < NUM_LEDS; v++) {
     leds[v] = sky1;
   }
    for( byte u = 0; u < 1; u++) {
      leds[random16(NUM_LEDS)] = sky2;
    }
#endif
  
  for (byte a = 0; a < map(scale, 1, 255, 1, SPARK); a++) {
    gDot[a].Move(store);
    gDot[a].Draw(leds);
  }
  for( byte b = 0; b < NUM_SPARKS; b++) {
    gSparks[b].Move(store);
    gSparks[b].Draw(leds);
  }
    return true;
}

// ------------ Эффект "Тихий Океан"
//  "Pacifica" перенос кода kostyamat
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
 bool EffectPacific::run(CRGB *ledarr, EffectWorker *opt)
{
    return pacificRoutine(*&ledarr, &*opt);
  return false;
}

// Add one layer of waves into the led array
void pacifica_one_layer(CRGB *leds, const TProgmemRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps(CRGB *leds)
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors(CRGB *leds)
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

bool EffectPacific::pacificRoutine(CRGB *leds, EffectWorker *param)
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / map(speed, 1, 255, 620, 60);
  uint32_t deltams2 = (deltams * speedfactor2) / map(speed, 1, 255, 620, 60);
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(&*leds, pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer(&*leds, pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer(&*leds, pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer(&*leds, pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps(&*leds);

  // Deepen the blues and greens a bit
  pacifica_deepen_colors(&*leds);
  myLamp.blur2d(20);
  return true;
}

#ifdef MIC_EFFECTS
//----- Эффект "Осциллограф" (c) kostyamat
void EffectOsc::load() {
  pointer = myLamp.getMicScale()/ _scaler;
}

 bool EffectOsc::run(CRGB *ledarr, EffectWorker *opt)
{
  if (speed <= 127) {
    spd = 127;
    OSC_HV = WIDTH;
  } else{
    spd = 0;
    OSC_HV = HEIGHT;
  }
  if((millis() - lastrun ) <= (isMicActive ? 15U : map(speed, 128 - spd, 255 - spd, 15U, 60U))) { 
    _rv = getCtrlVal(3).toInt();
    micA = isMicActive;
    return false;
  } else {
    lastrun = millis();
  }
  
  div = EffectMath::fmap(speed, 128.0f - spd, 255.0f - spd, 0.50f, 4.0f);
  random16_set_seed(micros());
  return oscRoutine(*&ledarr, &*opt);
}

bool EffectOsc::oscRoutine(CRGB *leds, EffectWorker *param) {  
  static float y[2] = {0.0f, 0.0f};
    //memset8( leds, 0, NUM_LEDS * 3);
  fadeToBlackBy(leds, NUM_LEDS, 200);

  for (float x = 0.0f; x < ((float)OSC_HV - div); x += div) {
    byte micPick = (micA ? myLamp.getMicMaxPeak() : random8(255));
    color = CHSV(micA ? myLamp.getMicMapFreq() : random8(255), 200, scale == 1 ? 96 : constrain(micPick * EffectMath::fmap(scale, 1.0f, 255.0f, 1.0f, 5.0f), 51, 255));
if (spd == 127) {

    myLamp.drawLineF(y[0], x, y[1], (x + div) >= OSC_HV ? OSC_HV - 1 : (x + div), color);
    //myLamp.drawLineF(x, y[0], (x + div) >= OSC_HV ? OSC_HV - 1 : (x + div), y[1], color);
} else 
    myLamp.drawLineF(x, y[0], (x + div) >= OSC_HV ? OSC_HV - 1 : (x + div), y[1], color);

    y[0] = y[1];
    y[1] = EffectMath::fmap((float)(micA ? analogRead(MIC_PIN) : random16(1023U)), 0.0f + (float)(_rv - 1), (pointer * 2U) - (float)(_rv - 1), 0.0f, float(OSC_HV - 1U)); 
    delayMicroseconds((uint16_t)((micA ? 1024U : 1568U) * div));
  
  }  

return true;
}
#endif

// ------ Эффект "Вышиванка" (с) проект Aurora "Munch"
void EffectMunch::load() {
  palettesload();
}

 bool EffectMunch::run(CRGB *ledarr, EffectWorker *opt)
{
  if (dryrun())
    return false;
  return munchRoutine(*&ledarr, &*opt);
}

bool EffectMunch::munchRoutine(CRGB *leds, EffectWorker *param) {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  byte rand = 4;
  if (scale%31 == 0)  rand = random8(4, 8); // Хрень, конечно, но хоть какое-то разнообразие.
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
      leds[myLamp.getPixelNumber(x, y)] = (x ^ y ^ flip) < count ? ColorFromPalette(*curPalette, ((x ^ y) << rand) + generation) : CRGB::Black;
    }
  }

  count += dir;

  if (count <= 0 || count >= mic[0]) {
    dir = -dir;
    if (count <= 0) {
      mic[0] = mic[1];
      if (flip == 0)
        flip = mic[1] - 1;
      else
        flip = 0;
    }
  }

  generation++;
#ifdef MIC_EFFECTS
  mic[1] = isMicActive ? map(myLamp.getMicMapMaxPeak(), 0, 255, 0, WIDTH) : WIDTH;
#else
  mic[1] = WIDTH;
#endif
  return true;
}

// ------ Эффект "Цветной шум" (с) https://gist.github.com/StefanPetrick/c856b6d681ec3122e5551403aabfcc68
DEFINE_GRADIENT_PALETTE( pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};
bool EffectNoise::noiseRoutine(CRGB *leds, EffectWorker *param) {
  myLamp.dimAll(200U);

  //CRGBPalette16 Pal( pit );

  //modulate the position so that it increases/decreases x
  //(here based on the top left pixel - it could be any position else)
  //the factor "2" defines the max speed of the x movement
  //the "-255" defines the median moving direction
  x[0] = x[0] + (map(speed, 1U, 255U, 2U, 16U ) * noise[0][0][0]) - 255U;
  //modulate the position so that it increases/decreases y
  //(here based on the top right pixel - it could be any position else)
  y[0] = y[0] + (map(speed, 1U, 255U, 2U, 16U ) * noise[0][WIDTH - 1][0]) - 255U;
  //z just in one direction but with the additional "1" to make sure to never get stuck
  //in case the movement is stopped by a crazy parameter (noise data) combination
  //(here based on the down left pixel - it could be any position else)
  z[0] += 1 + ((noise[0][0][HEIGHT - 1]) / 4);
  //set the scaling based on left and right pixel of the middle line
  //here you can set the range of the zoom in both dimensions
  scale_x[0] = 8000 + (noise[0][0][CentreY] * 16);
  scale_y[0] = 8000 + (noise[0][WIDTH - 1][CentreY] * 16);

  //calculate the noise data
  uint8_t layer = 0;
  for (uint8_t i = 0; i < WIDTH; i++) {
    uint32_t ioffset = scale_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      uint32_t joffset = scale_y[layer] * (j - CentreY);
      uint16_t data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]);
      // limit the 16 bit results to the interesting range
      if (data < 11000) data = 11000;
      if (data > 51000) data = 51000;
      // normalize
      data = data - 11000;
      // scale down that the result fits into a byte
      data = data / 161;
      // store the result in the array
      noise[layer][i][j] = data;
    }
  }

  //map the colors
  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      //I will add this overlay CRGB later for more colors
      //it´s basically a rainbow mapping with an inverted brightness mask
      CRGB overlay = CHSV(noise[0][y][x], 255, noise[0][x][y]);
      //here the actual colormapping happens - note the additional colorshift caused by the down right pixel noise[0][15][15]
      leds[XY(x, y)] = ColorFromPalette( *curPalette, noise[0][WIDTH - 1][HEIGHT - 1] + noise[0][x][y]) + overlay;
    }
  }

  //make it looking nice
  adjust_gamma(*&leds);

  //and show it!
 return true;
}

bool EffectNoise::run(CRGB *ledarr, EffectWorker *opt) {

  return noiseRoutine(*&ledarr, &*opt);
}

void EffectNoise::load() {
  palettesload();
}

// ---- Эффект "Мотыльки" 
// (с) Сотнег, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49262
bool EffectButterfly::run(CRGB *ledarr, EffectWorker *opt) {

  return butterflyRoutine(*&ledarr, &*opt);
}

void EffectButterfly::setDynCtrl(UIControl*_val)
{
  EffectCalc::setDynCtrl(_val); // сначала дергаем базовый, чтобы была обработка палитр/микрофона (если такая обработка точно не нужна, то можно не вызывать базовый метод)
  // теперь проверяем уже поведение для этого эффекта
  LOG(printf_P,PSTR("_val->getType():%d, _val->getId():%d, _val->getVal():%s\n"),_val->getType(),_val->getId(),_val->getVal().c_str());

  if(_val->getType()==2 && _val->getId()==3)
    wings = _val->getVal()==F("true") ? true : false;
  if(_val->getType()==2 && _val->getId()==4)
    isColored = _val->getVal()==F("true") ? false : true;
}

bool EffectButterfly::butterflyRoutine(CRGB *leds, EffectWorker *param)
{
  // Эти же контролы отбражены на _scale и _speed... т.е. зачем прямое обращение?
  // byte _scale = getCtrlVal(2).toInt();
  // byte _speed = getCtrlVal(1).toInt();
  
  byte _scale = scale;
  byte _speed = speed;

  if (csum != (isColored^(127U^_scale))) // проверяем не пора ли
  {
    csum = (isColored^(127U^_scale));
    if (isColored) // для режима смены цвета фона фиксируем количество мотыльков
      deltaValue = (_scale > BUTTERFLY_MAX_COUNT) ? BUTTERFLY_MAX_COUNT : _scale;
    else
      deltaValue = BUTTERFLY_FIX_COUNT;
    for (uint8_t i = 0U; i < BUTTERFLY_MAX_COUNT; i++)
    {
      butterflysPosX[i] = random8(WIDTH);
      butterflysPosY[i] = random8(HEIGHT);
      butterflysSpeedX[i] = 0;
      butterflysSpeedY[i] = 0;
      butterflysTurn[i] = 0;
      butterflysColor[i] = (isColored) ? random8() : 0U;
      butterflysBrightness[i] = 255U;
    }
    //для инверсии, чтобы сто раз не пересчитывать
    if (_scale != 1U and !isColored)
      hue = map(_scale, 2, BUTTERFLY_MAX_COUNT + 1U, 0, 255);
    hue2 = (_scale == 1U) ? 100U : 190U;  // вычисление базового оттенка

  }
  if (wings && isColored)
    //myLamp.dimAll(35U); // для крылышков
    fadeToBlackBy(leds, NUM_LEDS, 200);
  else
    FastLED.clear();

  float maxspeed;
  uint8_t tmp;
  float speedfactor = (float)_speed / 2048.0f + 0.001f;
  if (++step >= deltaValue)
    step = 0U;
  for (uint8_t i = 0U; i < deltaValue; i++)
  {
    butterflysPosX[i] += butterflysSpeedX[i]*speedfactor;
    butterflysPosY[i] += butterflysSpeedY[i]*speedfactor;

    if (butterflysPosX[i] < 0)
      butterflysPosX[i] = (float)(WIDTH - 1) + butterflysPosX[i];
    if (butterflysPosX[i] > WIDTH - 1)
      butterflysPosX[i] = butterflysPosX[i] + 1 - WIDTH;

    if (butterflysPosY[i] < 0)
    {
      butterflysPosY[i] = -butterflysPosY[i];
      butterflysSpeedY[i] = -butterflysSpeedY[i];
      //butterflysSpeedX[i] = -butterflysSpeedX[i];
    }
    if (butterflysPosY[i] > HEIGHT - 1U)
    {
      butterflysPosY[i] = (HEIGHT << 1U) - 2U - butterflysPosY[i];
      butterflysSpeedY[i] = -butterflysSpeedY[i];
      //butterflysSpeedX[i] = -butterflysSpeedX[i];
    }

    //проворот траектории
    maxspeed = fabs(butterflysSpeedX[i])+fabs(butterflysSpeedY[i]); // максимальная суммарная скорость
    if (maxspeed == fabs(butterflysSpeedX[i] + butterflysSpeedY[i]))
      {
          if (butterflysSpeedX[i] > 0) // правый верхний сектор вектора
          {
            butterflysSpeedX[i] += butterflysTurn[i];
            if (butterflysSpeedX[i] > maxspeed) // если вектор переехал вниз
              {
                butterflysSpeedX[i] = maxspeed + maxspeed - butterflysSpeedX[i];
                butterflysSpeedY[i] = butterflysSpeedX[i] - maxspeed;
              }
            else
              butterflysSpeedY[i] = maxspeed - fabs(butterflysSpeedX[i]);
          }
          else                           // левый нижний сектор
          {
            butterflysSpeedX[i] -= butterflysTurn[i];
            if (butterflysSpeedX[i] + maxspeed < 0) // если вектор переехал вверх
              {
                butterflysSpeedX[i] = 0 - butterflysSpeedX[i] - maxspeed - maxspeed;
                butterflysSpeedY[i] = maxspeed - fabs(butterflysSpeedX[i]);
              }
            else
              butterflysSpeedY[i] = fabs(butterflysSpeedX[i]) - maxspeed;
          }
      }
    else //левый верхний и правый нижний секторы вектора
      {
          if (butterflysSpeedX[i] > 0) // правый нижний сектор
          {
            butterflysSpeedX[i] -= butterflysTurn[i];
            if (butterflysSpeedX[i] > maxspeed) // если вектор переехал наверх
              {
                butterflysSpeedX[i] = maxspeed + maxspeed - butterflysSpeedX[i];
                butterflysSpeedY[i] = maxspeed - butterflysSpeedX[i];
              }
            else
              butterflysSpeedY[i] = fabs(butterflysSpeedX[i]) - maxspeed;
          }
          else                           // левый верхний сектор
          {
            butterflysSpeedX[i] += butterflysTurn[i];
            if (butterflysSpeedX[i] + maxspeed < 0) // если вектор переехал вниз
              {
                butterflysSpeedX[i] = 0 - butterflysSpeedX[i] - maxspeed - maxspeed;
                butterflysSpeedY[i] = 0 - butterflysSpeedX[i] - maxspeed;
              }
            else
              butterflysSpeedY[i] = maxspeed - fabs(butterflysSpeedX[i]);
          }
      }
   
    if (butterflysBrightness[i] == 255U)
    {
      if (step == i && random8(2U) == 0U)//(step == 0U && ((pcnt + i) & 0x01))
      {
        butterflysBrightness[i] = random8(220U,244U);
        butterflysSpeedX[i] = (float)random8(101U) / 20.0f + 1.0f;
        if (random8(2U) == 0U) butterflysSpeedX[i] = -butterflysSpeedX[i];
        butterflysSpeedY[i] = (float)random8(101U) / 20.0f + 1.0f;
        if (random8(2U) == 0U) butterflysSpeedY[i] = -butterflysSpeedY[i];
        // проворот траектории
        //butterflysTurn[i] = (float)random8((fabs(butterflysSpeedX[i])+fabs(butterflysSpeedY[i]))*2.0+2.0) / 40.0f;
        butterflysTurn[i] = (float)random8((fabs(butterflysSpeedX[i])+fabs(butterflysSpeedY[i]))*20.0f+2.0f) / 200.0f;
        if (random8(2U) == 0U) butterflysTurn[i] = -butterflysTurn[i];
      }
    }
    else
    {
      if (step == i)
        butterflysBrightness[i]++;
      tmp = 255U - butterflysBrightness[i];
      if (tmp == 0U || ((uint16_t)(butterflysPosX[i] * tmp) % tmp == 0U && (uint16_t)(butterflysPosY[i] * tmp) % tmp == 0U))
      {
        butterflysPosX[i] = round(butterflysPosX[i]);
        butterflysPosY[i] = round(butterflysPosY[i]);
        butterflysSpeedX[i] = 0;
        butterflysSpeedY[i] = 0;
        butterflysTurn[i] = 0;
        butterflysBrightness[i] = 255U;
      }
    }

    if (wings)
      myLamp.drawPixelXYF(butterflysPosX[i], butterflysPosY[i], CHSV(butterflysColor[i], 255U, (butterflysBrightness[i] == 255U) ? 255U : 128U + random8(2U) * 111U)); // это процедура рисования с нецелочисленными координатами. ищите её в прошивке
    else
      myLamp.drawPixelXYF(butterflysPosX[i], butterflysPosY[i], CHSV(butterflysColor[i], 255U, butterflysBrightness[i])); // это процедура рисования с нецелочисленными координатами. ищите её в прошивке
  }

  // постобработка кадра
  if (isColored){
    for (uint8_t i = 0U; i < deltaValue; i++) // ещё раз рисуем всех Мотыльков, которые "сидят на стекле"
      if (butterflysBrightness[i] == 255U)
        myLamp.drawPixelXY(butterflysPosX[i], butterflysPosY[i], CHSV(butterflysColor[i], 255U, butterflysBrightness[i]));
  }
  else {
    //теперь инверсия всей матрицы
    if (_scale == 1U)
      if (++deltaHue == 0U) hue++;
    for (uint16_t i = 0U; i < NUM_LEDS; i++)
      leds[i] = CHSV(hue, hue2, 255U - leds[i].r);
  }
  return true;
}

// ---- Эффект "Тени" 
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/effects.ino
bool EffectShadows::run(CRGB *ledarr, EffectWorker *opt) {
  //if (dryrun())
  //  return false;
  return shadowsRoutine(*&ledarr, &*opt);
}

bool EffectShadows::shadowsRoutine(CRGB *leds, EffectWorker *param) {
  
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
#ifdef MIC_EFFECTS
  uint8_t msmultiplier = isMicActive ? myLamp.getMicMapMaxPeak() :  speed; //beatsin88(147, 23, 60);
  byte effectBrightness = isMicActive ? myLamp.getMicMapMaxPeak() * 1.5f : scale;
#else
  uint8_t msmultiplier = speed; //beatsin88(147, 23, 60);
  byte effectBrightness = scale;
#endif
  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;

  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;

  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768U;

    uint32_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536U;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536U;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, map8(bri8, map(effectBrightness, 1, 255, 32, 125), map(effectBrightness, 1, 255, 125, 250))); 
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
  return true;
}

// ---- Эффект "Узоры" 
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/patterns.ino
// Сдвиг всей матрицы вверх
void shiftDown() {
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
      myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y + 1));
    }
  }
}

// Сдвиг всей матрицы вверх
void shiftUp() {
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = HEIGHT; y > 0; y--) {
      myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y - 1));
    }
  }
}

bool EffectPatterns::run(CRGB *ledarr, EffectWorker *opt) {
  EVERY_N_MILLIS((1005000U / speed)){
    if (scale == 1) 
      dir = !dir;
    loadingFlag = true;
  }
  if (dryrun()) {
    return false;
  }
  if (csum != (127U^scale)) {
    loadingFlag = true;
    csum = (127U^scale);
  }
  if (scale != 1)
    dir = getCtrlVal(3) == "true";

  return patternsRoutine(*&ledarr, &*opt);
}

void EffectPatterns::load() {
  FastLED.clear(true);
}

// Заполнение матрицы указанным паттерном
// ptrn - индекс узора в массив узоров patterns[] в patterns.h
// X   - позиция X вывода узора
// Y   - позиция Y вывода узора
// W   - ширина паттерна
// H   - высота паттерна
// dir - рисовать 'u' снизу, сдвигая вверх; 'd' - сверху, сдвигая вниз
void EffectPatterns::drawPattern(uint8_t ptrn, uint8_t X, uint8_t Y, uint8_t W, uint8_t H, bool dir) {
  
  if (dir) 
    shiftDown();
  else 
    shiftUp();

  uint8_t y = dir ? (HEIGHT - 1) : 0;

  // Если ширина паттерна не кратна ширине матрицы - отрисовывать со сдвигом? чтобы рисунок был максимально по центру
  int8_t offset_x = -((WIDTH % W) / 2) + 1;
  //_bri = random8(96U, 255U);
  
  for (uint8_t x = 0; x < WIDTH + W; x++) {
    int8_t xx = offset_x + x;
    if (xx >= 0 && xx < (int8_t)WIDTH) {
      uint8_t in = (uint8_t)pgm_read_byte(&(patterns[ptrn][lineIdx][x % 10])); 
      CHSV color = colorMR[in];
      CHSV color2 = color.v != 0 ? CHSV(color.h, color.s, _bri) : color;
      myLamp.drawPixelXY(xx, y, color2); 
    }
  }

  if (dir) {
    lineIdx = (lineIdx > 0) ? (lineIdx - 1) : (H - 1);  
  } else {
    lineIdx = (lineIdx < H - 1) ? (lineIdx + 1) : 0;
  }
}

// Отрисовка указанной картинки с размерами WxH в позиции XY
void EffectPatterns::drawPicture_XY(uint8_t iconIdx, uint8_t X, uint8_t Y, uint8_t W, uint8_t H) {
  if (loadingFlag) {
    loadingFlag = false;
    _bri = random8(96U, 255U);
  }

  for (byte x = 0; x < W; x++) {
    for (byte y = 0; y < H; y++) {
      uint8_t in = (uint8_t)pgm_read_byte(&(patterns[iconIdx][y][x])); 
      if (in != 0) {
        CHSV color = colorMR[in];        
        CHSV color2 = color.v != 0 ? CHSV(color.h, color.s, _bri) : color;
        myLamp.drawPixelXY(X+x,Y+H-y, color2); 
      }
    }
  }
}

bool EffectPatterns::patternsRoutine(CRGB *leds, EffectWorker *param) {

  if (loadingFlag) {
    loadingFlag = false;
    patternIdx = map(scale, 1U, MAX_PATTERN + 1, -1, MAX_PATTERN);  // мапим к ползунку масштаба
    if (patternIdx < 0) {
      patternIdx = random8(0U, MAX_PATTERN); 
    }
    //fadeToBlackBy(leds, NUM_LEDS, 25);
    if (dir) 
      lineIdx = 9;         // Картинка спускается сверху вниз - отрисовка с нижней строки паттерна (паттерн 10x10)
    else 
      lineIdx = 0;         // Картинка поднимается сверху вниз - отрисовка с верхней строки паттерна
    // Цвета с индексом 6 и 7 - случайные, определяются в момент настройки эффекта
    colorMR[6] = CHSV(random8(), 255U, 255U);
    if (random8() % 10 == 0) {
      colorMR[7] = CHSV(0U, 0U, 255U);
    } else {
      colorMR[7] = CHSV(random8(), 255U, 255U);
      while (fabs(colorMR[7].h - colorMR[6].h) < 32) {
        colorMR[7] = CHSV(random8(), 255U, 255U);
      }
    }
  }  

  drawPattern(patternIdx, 0, 0, 10, 10, dir);  
  return true;
}

// ***************************** "Стрелки" *****************************
bool EffectArrows::run(CRGB *ledarr, EffectWorker *opt) {
  if (scale == 1) {
    EVERY_N_SECONDS((3000U / speed))
    {
      loadingFlag = true;
    }
  }
  speedfactor = (float)speed / 768.0 + 0.15;
  subpixel = getCtrlVal(3) == "true" ? true : false;
  //if (dryrun())
  //  return false;

  if (csum != (127U ^ scale))
  {
    csum = (127U ^ scale);
    loadingFlag = true;
  }

  return arrowsRoutine(*&ledarr, &*opt);
}

bool EffectArrows::arrowsRoutine(CRGB *leds, EffectWorker *param) {
  if (loadingFlag) {
    loadingFlag = false;
    FastLED.clear();
    arrow_complete = false;
    arrow_mode_orig = scale-1;
    arrow_mode = arrow_mode_orig == 0 ? random8(1,5) : arrow_mode_orig;
    arrow_play_mode_count_orig[0] = 0;
    arrow_play_mode_count_orig[1] = 4;  // 4 фазы - все стрелки показаны по кругу один раз - переходить к следующему ->
    arrow_play_mode_count_orig[2] = 4;  // 2 фазы - гориз к центру (1), затем верт к центру (2) - обе фазы повторить по 2 раза -> 4
    arrow_play_mode_count_orig[3] = 4;  // 1 фаза - все к центру (1) повторить по 4 раза -> 4
    arrow_play_mode_count_orig[4] = 4;  // 2 фазы - гориз к центру (1), затем верт к центру (2) - обе фазы повторить по 2 раза -> 4
    arrow_play_mode_count_orig[5] = 4;  // 1 фаза - все сразу (1) повторить по 4 раза -> 4
    for (byte i=0; i<6; i++) {
      arrow_play_mode_count[i] = arrow_play_mode_count_orig[i];
    }
    arrowSetupForMode(arrow_mode, true);
  }
  
    prevVal[0] += speedfactor;
    if((byte)prevVal[0] == (byte)prevVal[1]) {
      prevVal[1] = (byte)prevVal[1]+ 1U;
      EffectMath::fader(65);
      //fadeToBlackBy(leds, NUM_LEDS, 50);
    } 

  CHSV color;
  
  // движение стрелки - cлева направо
  if ((arrow_direction & 0x01) > 0) {
    color = CHSV(arrow_hue[0], 255, 255);
    for (float x = 0.0f; x <= 4; x+= speedfactor) {
      for (float y = 0.0f; y <= x; y+= speedfactor) {    
        if (subpixel ? arrow_x[0] - x >= 0 && arrow_x[0] - x <= stop_x[0] : (byte)arrow_x[0] - (byte)x >= 0 && (byte)arrow_x[0] - (byte)x <= (byte)stop_x[0]) { 
          CHSV clr = ((byte)x < 4 || ((byte)x == 4 && (byte)y < 2)) ? color : CHSV(0,0,0);
          if (subpixel)
          {
            myLamp.drawPixelXYF(arrow_x[0] - x, arrow_y[0] - y, clr);
            myLamp.drawPixelXYF(arrow_x[0] - x, arrow_y[0] + y, clr);
          }
          else
          {
            myLamp.drawPixelXY((byte)(arrow_x[0] - x), (byte)(arrow_y[0] - y), clr);
            myLamp.drawPixelXY((byte)(arrow_x[0] - x), (byte)(arrow_y[0] + y), clr);
          }
        }
      }    
    }
    arrow_x[0]+= speedfactor;
  }

  // движение стрелки - cнизу вверх
  if ((arrow_direction & 0x02) > 0) {
    color = CHSV(arrow_hue[1], 255, 255);
    for (float y = 0.0f; y <= 4; y+= speedfactor) {
      for (float x = 0.0f; x <= y; x+= speedfactor) {    
        if (subpixel ? arrow_y[1] - y >= 0 && arrow_y[1] - y <= stop_y[1] : (byte)arrow_y[1] - (byte)y >= 0 && (byte)arrow_y[1] - (byte)y <= (byte)stop_y[1]) { 
          CHSV clr = ((byte)y < 4 || ((byte)y == 4 && (byte)x < 2)) ? color : CHSV(0,0,0);
          if (subpixel)
          {
            myLamp.drawPixelXYF(arrow_x[1] - x, arrow_y[1] - y, clr);
            myLamp.drawPixelXYF(arrow_x[1] + x, arrow_y[1] - y, clr);
          }
          else
          {
            myLamp.drawPixelXY((byte)(arrow_x[1] - x), (byte)(arrow_y[1] - y), clr);
            myLamp.drawPixelXY((byte)(arrow_x[1] + x), (byte)(arrow_y[1] - y), clr);
          }
        }
      }    
    }
    arrow_y[1]+= speedfactor;
  }

  // движение стрелки - cправа налево
  if ((arrow_direction & 0x04) > 0) {
    color = CHSV(arrow_hue[2], 255, 255);
    for (float x = 0.0f; x <= 4; x+= speedfactor) {
      for (float y = 0.0f; y <= x; y+= speedfactor) {    
        if (subpixel ? arrow_x[2] + x >= stop_x[2] && arrow_x[2] + x < WIDTH : (byte)arrow_x[2] + (byte)x >= (byte)stop_x[2] && (byte)arrow_x[2] + (byte)x < WIDTH) { 
          CHSV clr = ((byte)x < 4 || ((byte)x == 4 && (byte)y < 2)) ? color : CHSV(0,0,0);
          if (subpixel)
          {
            myLamp.drawPixelXYF(arrow_x[2] + x, arrow_y[2] - y, clr);
            myLamp.drawPixelXYF(arrow_x[2] + x, arrow_y[2] + y, clr);
          }
          else
          {
            myLamp.drawPixelXY((byte)(arrow_x[2] + x), (byte)(arrow_y[2] - y), clr);
            myLamp.drawPixelXY((byte)(arrow_x[2] + x), (byte)(arrow_y[2] + y), clr);
          }
        }
      }    
    }
    arrow_x[2]-= speedfactor;
  }

  // движение стрелки - cверху вниз
  if ((arrow_direction & 0x08) > 0) {
    color = CHSV(arrow_hue[3], 255, 255);
    for (float y = 0.0f; y <= 4; y+= speedfactor) {
      for (float x = 0.0f; x <= y; x+= speedfactor) {    
        if (subpixel ? arrow_y[3] + y >= stop_y[3] && arrow_y[3] + y < HEIGHT : (byte)arrow_y[3] + (byte)y >= (byte)stop_y[3] && (byte)arrow_y[3] + (byte)y < HEIGHT) { 
          CHSV clr = ((byte)y < 4 || ((byte)y == 4 && (byte)x < 2)) ? color : CHSV(0,0,0);
          if (subpixel)
          {
            myLamp.drawPixelXYF(arrow_x[3] - x, arrow_y[3] + y, clr);
            myLamp.drawPixelXYF(arrow_x[3] + x, arrow_y[3] + y, clr);
          }
          else
          {
            myLamp.drawPixelXY((byte)(arrow_x[3] - x), (byte)(arrow_y[3] + y), clr);
            myLamp.drawPixelXY((byte)(arrow_x[3] + x), (byte)(arrow_y[3] + y), clr);
          }
        }
      }    
    }
    arrow_y[3]-= speedfactor;
  }

  // Проверка завершения движения стрелки, переход к следующей фазе или режиму
  
  switch (arrow_mode) {

    case 1:
      // Последовательно - слева-направо -> снизу вверх -> справа налево -> сверху вниз и далее по циклу
      // В каждый сомент времени сктивна только одна стрелка, если она дошла до края - переключиться на следующую и задать ее начальные координаты
      arrow_complete = false;
      switch (arrow_direction) {
        case 1: arrow_complete = arrow_x[0] > stop_x[0]; break;
        case 2: arrow_complete = arrow_y[1] > stop_y[1]; break;
        case 4: arrow_complete = arrow_x[2] < stop_x[2]; break;
        case 8: arrow_complete = arrow_y[3] < stop_y[3]; break;
      }

      arrow_change_mode = false;
      if (arrow_complete) {
        arrow_direction = (arrow_direction << 1) & 0x0F;
        if (arrow_direction == 0) arrow_direction = 1;
        if (arrow_mode_orig == 0) {
          arrow_play_mode_count[1]--;
          if (arrow_play_mode_count[1] == 0) {
            arrow_play_mode_count[1] = arrow_play_mode_count_orig[1];
            arrow_mode = random8(1,5);
            arrow_change_mode = true;
          }
        }

        arrowSetupForMode(arrow_mode, arrow_change_mode);
      }
      break;

    case 2:
      // Одновременно горизонтальные навстречу до половины экрана
      // Затем одновременно вертикальные до половины экрана. Далее - повторять
      arrow_complete = false;
      switch (arrow_direction) {
        case  5: arrow_complete = arrow_x[0] > stop_x[0]; break;   // Стрелка слева и справа встречаются в центре одновременно - проверять только стрелку слева
        case 10: arrow_complete = arrow_y[1] > stop_y[1]; break;   // Стрелка снизу и сверху встречаются в центре одновременно - проверять только стрелку снизу
      }

      arrow_change_mode = false;
      if (arrow_complete) {
        arrow_direction = arrow_direction == 5 ? 10 : 5;
        if (arrow_mode_orig == 0) {
          arrow_play_mode_count[2]--;
          if (arrow_play_mode_count[2] == 0) {
            arrow_play_mode_count[2] = arrow_play_mode_count_orig[2];
            arrow_mode = random8(1,5);
            arrow_change_mode = true;
          }
        }
        
        arrowSetupForMode(arrow_mode, arrow_change_mode);
      }
      break;

    case 3:
      // Одновременно со всех сторон к центру
      // Завершение кадра режима - когда все стрелки собрались в центре.
      // Проверять стрелки по самой длинной стороне
      if (WIDTH >= HEIGHT)
        arrow_complete = arrow_x[0] > stop_x[0];
      else 
        arrow_complete = arrow_y[1] > stop_y[1];
        
      arrow_change_mode = false;
      if (arrow_complete) {
        if (arrow_mode_orig == 0) {
          arrow_play_mode_count[3]--;
          if (arrow_play_mode_count[3] == 0) {
            arrow_play_mode_count[3] = arrow_play_mode_count_orig[3];
            arrow_mode = random8(1,5);
            arrow_change_mode = true;
          }
        }
        
        arrowSetupForMode(arrow_mode, arrow_change_mode);
      }
      break;

    case 4:
      // Одновременно слева/справа от края до края со смещением горизонтальной оси на 1/3 высоты, далее
      // одновременно снизу/сверху от края до края со смещением вертикальной оси на 1/3 ширины
      // Завершение кадра режима - когда все стрелки собрались в центре.
      // Проверять стрелки по самой длинной стороне
      switch (arrow_direction) {
        case  5: arrow_complete = arrow_x[0] > stop_x[0]; break;   // Стрелка слева и справа движутся и достигают края одновременно - проверять только стрелку слева
        case 10: arrow_complete = arrow_y[1] > stop_y[1]; break;   // Стрелка снизу и сверху движутся и достигают края одновременно - проверять только стрелку снизу
      }

      arrow_change_mode = false;
      if (arrow_complete) {
        arrow_direction = arrow_direction == 5 ? 10 : 5;
        if (arrow_mode_orig == 0) {
          arrow_play_mode_count[4]--;
          if (arrow_play_mode_count[4] == 0) {
            arrow_play_mode_count[4] = arrow_play_mode_count_orig[4];
            arrow_mode = random8(1,5);
            arrow_change_mode = true;
          }
        }
        
        arrowSetupForMode(arrow_mode, arrow_change_mode);
      }
      break;

    case 5:
      // Одновременно со всех сторон от края до края со смещением горизонтальной оси на 1/3 высоты, далее
      // Проверять стрелки по самой длинной стороне
      if (WIDTH >= HEIGHT)
        arrow_complete = arrow_x[0] > stop_x[0];
      else 
        arrow_complete = arrow_y[1] > stop_y[1];

      arrow_change_mode = false;
      if (arrow_complete) {
        if (arrow_mode_orig == 0) {
          arrow_play_mode_count[5]--;
          if (arrow_play_mode_count[5] == 0) {
            arrow_play_mode_count[5] = arrow_play_mode_count_orig[5];
            arrow_mode = random8(1,5);
            arrow_change_mode = true;
          }
        }
        
        arrowSetupForMode(arrow_mode, arrow_change_mode);
      }
      break;
  }
  return true;
}

void EffectArrows::arrowSetupForMode(byte mode, bool change) {
    switch (mode) {
      case 1:
        if (change) arrow_direction = 1;
        arrowSetup_mode1();    // От края матрицы к краю, по центру гориз и верт
        break;
      case 2:
        if (change) arrow_direction = 5;
        arrowSetup_mode2();    // По центру матрицы (гориз / верт) - ограничение - центр матрицы
        break;
      case 3:
        if (change) arrow_direction = 15;
        arrowSetup_mode2();    // как и в режиме 2 - по центру матрицы (гориз / верт) - ограничение - центр матрицы
        break;
      case 4:
        if (change) arrow_direction = 5;
        arrowSetup_mode4();    // От края матрицы к краю, верт / гориз
        break;
      case 5:
        if (change) arrow_direction = 15;
        arrowSetup_mode4();    // как и в режиме 4 от края матрицы к краю, на 1/3
        break;
    }
}
void EffectArrows::arrowSetup_mode1() {
  // Слева направо
  if ((arrow_direction & 0x01) > 0) {
    arrow_hue[0] = random8();
    arrow_x[0] = 0;
    arrow_y[0] = HEIGHT / 2;
    stop_x [0] = WIDTH + 7;      // скрывается за экраном на 7 пикселей
    stop_y [0] = 0;              // неприменимо 
  }    
  // снизу вверх
  if ((arrow_direction & 0x02) > 0) {
    arrow_hue[1] = random8();
    arrow_y[1] = 0;
    arrow_x[1] = WIDTH / 2;
    stop_y [1] = HEIGHT + 7;     // скрывается за экраном на 7 пикселей
    stop_x [1] = 0;              // неприменимо 
  }    
  // справа налево
  if ((arrow_direction & 0x04) > 0) {
    arrow_hue[2] = random8();
    arrow_x[2] = WIDTH - 1;
    arrow_y[2] = HEIGHT / 2;
    stop_x [2] = -7;             // скрывается за экраном на 7 пикселей
    stop_y [2] = 0;              // неприменимо 
  }
  // сверху вниз
  if ((arrow_direction & 0x08) > 0) {
    arrow_hue[3] = random8();
    arrow_y[3] = HEIGHT - 1;
    arrow_x[3] = WIDTH / 2;
    stop_y [3] = -7;             // скрывается за экраном на 7 пикселей
    stop_x [3] = 0;              // неприменимо 
  }
}

void EffectArrows::arrowSetup_mode2() {
  // Слева направо до половины экрана
  if ((arrow_direction & 0x01) > 0) {
    arrow_hue[0] = random8();
    arrow_x[0] = 0;
    arrow_y[0] = HEIGHT / 2;
    stop_x [0] = WIDTH / 2 - 1;  // до центра экрана
    stop_y [0] = 0;              // неприменимо 
  }    
  // снизу вверх до половины экрана
  if ((arrow_direction & 0x02) > 0) {
    arrow_hue[1] = random8();
    arrow_y[1] = 0;
    arrow_x[1] = WIDTH / 2;
    stop_y [1] = HEIGHT / 2 - 1; // до центра экрана
    stop_x [1] = 0;              // неприменимо 
  }    
  // справа налево до половины экрана
  if ((arrow_direction & 0x04) > 0) {
    arrow_hue[2] = random8();
    arrow_x[2] = WIDTH - 1;
    arrow_y[2] = HEIGHT / 2;
    stop_x [2] = WIDTH / 2;      // до центра экрана
    stop_y [2] = 0;              // неприменимо 
  }
  // сверху вниз до половины экрана
  if ((arrow_direction & 0x08) > 0) {
    arrow_hue[3] = random8();
    arrow_y[3] = HEIGHT - 1;
    arrow_x[3] = WIDTH / 2;
    stop_y [3] = HEIGHT / 2;     // до центра экрана
    stop_x [3] = 0;              // неприменимо 
  }
}

void EffectArrows::arrowSetup_mode4() {
  // Слева направо
  if ((arrow_direction & 0x01) > 0) {
    arrow_hue[0] = random8();
    arrow_x[0] = 0;
    arrow_y[0] = (HEIGHT / 3) * 2;
    stop_x [0] = WIDTH + 7;      // скрывается за экраном на 7 пикселей
    stop_y [0] = 0;              // неприменимо 
  }    
  // снизу вверх
  if ((arrow_direction & 0x02) > 0) {
    arrow_hue[1] = random8();
    arrow_y[1] = 0;
    arrow_x[1] = (WIDTH / 3) * 2;
    stop_y [1] = HEIGHT + 7;     // скрывается за экраном на 7 пикселей
    stop_x [1] = 0;              // неприменимо 
  }    
  // справа налево
  if ((arrow_direction & 0x04) > 0) {
    arrow_hue[2] = random8();
    arrow_x[2] = WIDTH - 1;
    arrow_y[2] = HEIGHT / 3;
    stop_x [2] = -7;             // скрывается за экраном на 7 пикселей
    stop_y [2] = 0;              // неприменимо 
  }
  // сверху вниз
  if ((arrow_direction & 0x08) > 0) {
    arrow_hue[3] = random8();
    arrow_y[3] = HEIGHT - 1;
    arrow_x[3] = WIDTH / 3;
    stop_y [3] = -7;             // скрывается за экраном на 7 пикселей
    stop_x [3] = 0;              // неприменимо 
  }
}