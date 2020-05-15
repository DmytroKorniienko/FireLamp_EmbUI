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

extern const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {
  CRGB::Black,
  CRGB::DarkSlateGray,
  CRGB::DimGray,
  CRGB::LightSlateGray,

  CRGB::DimGray,
  CRGB::DarkSlateGray,
  CRGB::Silver,
  CRGB::DarkCyan,

  CRGB::Lavender,
  CRGB::Silver,
  CRGB::Azure,
  CRGB::LightGrey,

  CRGB::GhostWhite,
  CRGB::Silver,
  CRGB::White,
  CRGB::RoyalBlue
};

extern const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000,
    0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
    0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,
    0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};

extern const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::OrangeRed, CRGB::Orange, CRGB::Gold};             //* Orange
extern const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, CRGB::Red, 0xff3c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
extern const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, 0xff3c00, 0xff7800};                      // пытаюсь сделать что-то более приличное
extern const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Orange, CRGB::Gold, CRGB::Goldenrod};           //* Yellow
extern const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Green, CRGB::GreenYellow, CRGB::LimeGreen};     //* Green
extern const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Blue, CRGB::DeepSkyBlue, CRGB::LightSkyBlue};  //* Blue
extern const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::Indigo, CRGB::DarkBlue};        //* Indigo
extern const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::Indigo, CRGB::MediumPurple, CRGB::DeepPink}; //* Violet
extern const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, CRGB::FireBrick, CRGB::Pink, CRGB::DeepPink};        //* Red

//----------------------------------------------------
void fadePixel(uint8_t i, uint8_t j, uint8_t step)          // новый фейдер
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
void fader(uint8_t step)
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
#define EFF_FADE_OUT_SPEED        (15U)                         // скорость затухания
void sparklesRoutine(CRGB *leds, const char *param)
{

#ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = constrain(myLamp.effects.getScale()*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?3.0:(mmp?0.012*mmp:1.0)),1,255);
  GSHMEM.speed = constrain(myLamp.effects.getSpeed()*(mmf>0?(1.5*(mmf/255.0)+0.33):1),1,255);
// #if defined(LAMP_DEBUG) && defined(MIC_EFFECTS)
// EVERY_N_SECONDS(1){
//   LOG.printf_P(PSTR("MF: %5.2f MMF: %d MMP: %d GSHMEM.scale %d GSHMEM.speed: %d\n"), myLamp.getMicFreq(), mmf, mmp, GSHMEM.scale, GSHMEM.speed);
// }
// #endif
#else
  GSHMEM.scale = myLamp.effects.getScale();
  GSHMEM.speed = myLamp.effects.getSpeed();
#endif

  fader((uint8_t)(EFF_FADE_OUT_SPEED*((float)GSHMEM.scale)/255)+1);

  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-GSHMEM.speed)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  //EVERY_N_MILLIS(500){
  for (uint8_t i = 0; i < (uint8_t)round(2.5*(GSHMEM.speed/255.0)+1); i++){
      uint8_t x = random(0U, WIDTH);
      uint8_t y = random(0U, HEIGHT);
      if (myLamp.getPixColorXY(x, y) == 0U){
        myLamp.setLeds(myLamp.getPixelNumber(x, y),CHSV(random(1U, 255U), random(192U, 255U), random(192U, 255U)));
      }
  }
}

// ------------- белый свет -------------
void whiteColorRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    FastLED.clear();

    for (uint16_t i = 0U; i < NUM_LEDS; i++)
    {
      myLamp.setLeds(i, CHSV(0U, 0U, 255U));
    }
  }
}

// ------------- белый свет (светится горизонтальная полоса по центру лампы; масштаб - высота центральной горизонтальной полосы; скорость - регулировка от холодного к тёплому; яркость - общая яркость) -------------
void whiteColorStripeRoutine(CRGB *leds, const char *param)
{
    // //if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    // if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < 200){
    //   FastLED.show(); // борьба с мерцанием на белом цвете...
    //   return;
    // } else {
    //   myLamp.setEffDelay(millis());
    // }

    if(myLamp.effects.getScale()<127){
        uint8_t centerY = max((uint8_t)round(HEIGHT / 2.0F) - 1, 0);
        uint8_t bottomOffset = (uint8_t)(!(HEIGHT & 1) && (HEIGHT > 1));                      // если высота матрицы чётная, линий с максимальной яркостью две, а линии с минимальной яркостью снизу будут смещены на один ряд
        //Serial.printf_P(PSTR("%d %d\n"), centerY, bottomOffset);  
        for (int16_t y = centerY; y >= 0; y--)
        {
          int br = BRIGHTNESS-(13*(WIDTH-y)); if((br-(255-myLamp.getLampBrightness()))<0) br=0;
          
          CRGB color = CHSV(
            45U,                                                                              // определяем тон
            map(myLamp.effects.getSpeed(), 0U, 255U, 0U, 170U),                            // определяем насыщенность
            y == centerY                                                                                                    // определяем яркость
              ? BRIGHTNESS                                                                                                  // для центральной горизонтальной полосы (или двух) яркость всегда равна BRIGHTNESS
              : ((myLamp.effects.getScale()) / 143.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? BRIGHTNESS : br);  // для остальных горизонтальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

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
              map(myLamp.effects.getSpeed(), 0U, 255U, 0U, 170U),                            // определяем насыщенность
              x == centerX                                                                                                    // определяем яркость
                ? BRIGHTNESS                                                                                                  // для центральной вертикальной полосы (или двух) яркость всегда равна BRIGHTNESS
                : ((255-myLamp.effects.getScale()) / 143.0F) > ((centerX + 1.0F) - (x + 1.0F)) / (centerX + 1.0F) ? BRIGHTNESS : br);  // для остальных вертикальных полос яркость равна либо BRIGHTNESS, либо вычисляется по br

            myLamp.drawPixelXY(x, y, color);                                                      // при чётной ширине матрицы максимально яркими отрисуются 2 центральных вертикальных полосы
            myLamp.drawPixelXY(max((uint8_t)(WIDTH - 1U) - (x + 1U) + leftOffset, 0U), y, color); // при нечётной - одна, но дважды
          }
        }
    }
}

// ============= водо/огне/лава/радуга/хренопад ===============
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80U // 50 // 30 // 120 // 90 // 60
void fire2012WithPalette(CRGB*leds, const char *param) {
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

  uint8_t scale = myLamp.effects.getScale();
  uint8_t COOLINGNEW = constrain((uint16_t)(scale % 32) * 8 / HEIGHT + 7, 1, 255) ;
  // Array of temperature readings at each simulation cell
  // static byte GSHMEM.heat[WIDTH][HEIGHT];

  myLamp.blur2d(20);
  myLamp.dimAll(254U - ((myLamp.effects.getScale()%32))*8);

  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Cool down every cell a little
    for (unsigned int i = 0; i < HEIGHT; i++) {
      //GSHMEM.heat[x][i] = qsub8(GSHMEM.heat[x][i], random8(0, ((COOLINGNEW * 10) / HEIGHT) + 2));
      GSHMEM.heat[x][i] = qsub8(GSHMEM.heat[x][i], random8(0, COOLINGNEW));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = HEIGHT - 1; k >= 2; k--) {
      GSHMEM.heat[x][k] = (GSHMEM.heat[x][k - 1] + GSHMEM.heat[x][k - 2] + GSHMEM.heat[x][k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of GSHMEM.heat near the bottom
    if (random8() < SPARKINGNEW) {
      int y = random8(2);
      GSHMEM.heat[x][y] = qadd8(GSHMEM.heat[x][y], random8(160, 255));
    }

    // Step 4.  Map from GSHMEM.heat cells to LED colors
    for (unsigned int j = 0; j < HEIGHT; j++) {
      // Scale the GSHMEM.heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8(GSHMEM.heat[x][j], 240);
      myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(*curPalette, colorindex));
    }
  }
}

// --------------------------- эффект пульс ----------------------
// Stefan Petrick's PULSE Effect mod by PalPalych for GyverLamp 

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

// uint8_t GSHMEM.pulse_hue;
// uint8_t GSHMEM.pulse_step = 0;
void pulseRoutine(CRGB *leds, const char *param) {
    if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
      return;
    } else {
      myLamp.setEffDelay(millis());
    }

  if(myLamp.isLoading()){
	GSHMEM.pulse_step = 0;
  }
  
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
  uint8_t _scale = myLamp.effects.getScale();
  //const uint8_t limitsteps = 6U;
  //static const float fadeRate = 0.8;

  myLamp.dimAll(248U); // если эффект устанавливается с другими эффектами от Stefan Petrick, тогда  процедура должна называться dimAll (без двоечки)
  if (GSHMEM.pulse_step <= currentRadius) {
    for (uint8_t i = 0; i < GSHMEM.pulse_step; i++ ) {
      uint8_t _dark = qmul8( 2U, cos8 (128U / (GSHMEM.pulse_step + 1U) * (i + 1U))) ;
      if (_scale == 1) {            // 1 - случайные диски
        _pulse_hue = GSHMEM.pulse_hue;
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
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (GSHMEM.pulse_step + 1U) * (i + 1U))) ;
         _pulse_hue += (68U - _scale) * 7U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      
      } else if (_scale < 83) {     // 68...83 - выбор цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (GSHMEM.pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = (_scale - 68U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      
      } else if (_scale < 100) {    // 84...99 - перелив цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (GSHMEM.pulse_step + 1U) * (i + 1U))) ;
        _pulse_delta = (_scale - 85U)  ;
        _pulse_color = CHSV(_pulse_hueall, _sat, _dark);
      
      } else { // 100 - случайные пузыри
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (GSHMEM.pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = GSHMEM.pulse_hue;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      }
      drawCircle(centerX, centerY, i, _pulse_color  );
    }
  } else {
    centerX = random8(WIDTH - 5U) + 3U;
    centerY = random8(HEIGHT - 5U) + 3U;
    _pulse_hueall += _pulse_delta;
    GSHMEM.pulse_hue = random8(0U, 255U);
    currentRadius = random8(3U, 9U);
    GSHMEM.pulse_step = 0;
  }
  GSHMEM.pulse_step++;
}

// радуги 2D
// ------------- радуга вертикальная/горизонтальная ----------------
//uint8_t GSHMEM.hue;
void rainbowHorVertRoutine(bool isVertical)
{
#ifdef MIC_EFFECTS
  GSHMEM.hue += (4 * (myLamp.getMicMapMaxPeak()>50?5*(myLamp.getMicMapFreq()/255.0):1));
#else
  GSHMEM.hue += 4;
#endif
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
    CHSV thisColor = CHSV((uint8_t)(GSHMEM.hue + i * myLamp.effects.getScale()%86), 255, 255); // 1/3 без центральной между 1...255, т.е.: 1...84, 170...255
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      myLamp.drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
}

// ------------- радуга диагональная -------------
void rainbowDiagonalRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER)
#ifdef MIC_EFFECTS
    < (unsigned)(255-myLamp.effects.getSpeed())*((myLamp.getMicMapMaxPeak()>myLamp.effects.getScale()%86+50)?(2.0*(50.0/myLamp.getMicMapMaxPeak())):1) ){
#else
    < (unsigned)(255-myLamp.effects.getSpeed())){
#endif
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  if(myLamp.effects.getScale()<85){
    rainbowHorVertRoutine(false);
    return;
  } else if (myLamp.effects.getScale()>170){
    rainbowHorVertRoutine(true);
    return;
  }

  GSHMEM.hue += 4;
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      float twirlFactor = 3.0F * (myLamp.effects.getScale() / 30.0F);      // на сколько оборотов будет закручена матрица, [0..3]
#ifdef MIC_EFFECTS
      twirlFactor *= myLamp.getMicMapMaxPeak()>50?1.5*(myLamp.getMicMapFreq()/255.0):1;
#endif
      CRGB thisColor = CHSV((uint8_t)(GSHMEM.hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)myLamp.getmaxDim())), 255, 255);
      myLamp.drawPixelXY(i, j, thisColor);
    }
  }
}

// ------------- цвета -----------------
void colorsRoutine(CRGB *leds, const char *param)
{
  static unsigned int step = 0; // доп. задержка
  unsigned int delay = (myLamp.effects.getSpeed()==1)?4294967294:255-myLamp.effects.getSpeed()+1; // на скорости 1 будет очень долгое ожидание)))
  
  if (myLamp.isLoading()){ // начальная установка цвета
    GSHMEM.ihue = myLamp.effects.getScale();
    myLamp.fillAll(CHSV(GSHMEM.ihue, 255U, 55U)); // еще не наступила смена цвета, поэтому выводим текущий
  } else {
    step=(step+1)%(delay+1);
    if(step!=delay) {

#ifdef MIC_EFFECTS
  uint16_t mmf = myLamp.getMicMapFreq();
  uint16_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = myLamp.effects.getScale();
  GSHMEM.speed = myLamp.effects.getSpeed();

#if defined(LAMP_DEBUG) && defined(MIC_EFFECTS)
EVERY_N_SECONDS(1){
  LOG.printf_P(PSTR("MF: %5.2f MMF: %d MMP: %d GSHMEM.scale %d GSHMEM.speed: %d\n"), myLamp.getMicFreq(), mmf, mmp, GSHMEM.scale, GSHMEM.speed);
}
#endif
      if(myLamp.isMicOnOff()){
        // включен микрофон
        if(GSHMEM.scale>127){
          uint8_t pos = (round(3.0*(mmf+(25.0*GSHMEM.speed/255.0))/255.0))*HEIGHT/8; // двигаем частоты по диапазону в зависимости от скорости и делим на 4 части 0...3
          for(uint8_t y=pos;y<pos+HEIGHT/8;y++){
            for(uint8_t x=0; x<WIDTH; x++){
              //if(mmp>MIN_PEAK_LEVEL/2 || pos==3){ // в половину минимальной амплитуды уже пропускаем :)
                myLamp.setLeds(myLamp.getPixelNumber(x,y),CHSV(mmf/1.5, 255U, constrain(mmp*(2.0*(GSHMEM.scale>>1)/127.0+0.33),1,255)));
                myLamp.setLeds(myLamp.getPixelNumber(x,HEIGHT-1-y),CHSV(mmf/1.5, 255U, constrain(mmp*(2.0*(GSHMEM.scale>>1)/127.0+0.33),1,255)));
                //myLamp.SetLeds(myLamp.getPixelXY(x,y+HIGHT/4-1),CHSV(mmf, 255U, 255));
              //}
            }
          }
          myLamp.dimAll(254); // плавно гасим 
        } else {
          if(mmp>GSHMEM.scale) // если амплитуда превышает масштаб
            myLamp.fillAll(CHSV(constrain(mmf*(2.0*GSHMEM.speed/255.0),1,255), 255U, constrain(mmp*(2.0*GSHMEM.scale/127.0+1.5),1,255))); // превышает минимаьный уровень громкости, значит выводим текущую частоту
          else
            myLamp.dimAll(252); // плавно гасим
        }
      } else {
        // выключен микрофон
        myLamp.fillAll(CHSV(GSHMEM.ihue, 255U, 255U)); // еще не наступила смена цвета, поэтому выводим текущий
      }
#else
      myLamp.fillAll(CHSV(GSHMEM.ihue, 255U, 255U)); // еще не наступила смена цвета, поэтому выводим текущий
#endif  
    }
    else {  
      GSHMEM.ihue += myLamp.effects.getScale(); // смещаемся на следущий
    }
  }
}

// ------------- матрица ---------------
void matrixRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    // обрабатываем нашу матрицу снизу вверх до второй сверху строчки
    for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
    {
      uint32_t thisColor = myLamp.getPixColorXY(x, y);                                              // берём цвет нашего пикселя
      uint32_t upperColor = myLamp.getPixColorXY(x, y + 1U);                                        // берём цвет пикселя над нашим
      if (upperColor >= 0x900000 && random(7 * HEIGHT) != 0U)                  // если выше нас максимальная яркость, игнорим этот факт с некой вероятностью или опускаем цепочку ниже
        myLamp.drawPixelXY(x, y, upperColor);
      else if (thisColor == 0U && random((255 - myLamp.effects.getScale()) * HEIGHT) == 0U)  // если наш пиксель ещё не горит, иногда зажигаем новые цепочки
      //else if (thisColor == 0U && random((255 - myLamp.effects.getScale()) * HEIGHT*3) == 0U)  // для длинных хвостов
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
      if (random(255 - myLamp.effects.getScale()) == 0U)
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
}

// ------------- снегопад ----------
void snowRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  // сдвигаем всё вниз
  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    for (uint8_t y = 0U; y < HEIGHT - 1; y++)
    {
      myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y + 1U));
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    // заполняем случайно верхнюю строку
    // а также не даём двум блокам по вертикали вместе быть
    if (myLamp.getPixColorXY(x, HEIGHT - 2U) == 0U && (random(0, 255 - myLamp.effects.getScale()) == 0U))
      myLamp.drawPixelXY(x, HEIGHT - 1U, 0xE0FFFF - 0x101010 * random(0, 4));
    else
      myLamp.drawPixelXY(x, HEIGHT - 1U, 0x000000);
  }
}

// ------------- метель -------------
#define SNOW_DENSE            (60U)                         // плотность снега
#define SNOW_TAIL_STEP        (100U)                        // длина хвоста
#define SNOW_SATURATION       (0U)                          // насыщенность (от 0 до 255)
// ------------- звездопад -------------
#define STAR_DENSE            (60U)                         // плотность комет
#define STAR_TAIL_STEP        (100U)                        // длина хвоста кометы
#define STAR_SATURATION       (150U)                        // насыщенность кометы (от 0 до 255)

// ------------- звездопад/метель -------------
void snowStormStarfallRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading()){
    FastLED.clear();
  }
  
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  // заполняем головами комет левую и верхнюю линию
  for (uint8_t i = HEIGHT / 2U; i < HEIGHT; i++)
  {
    if (myLamp.getPixColorXY(0U, i) == 0U &&
       (random(0, (myLamp.effects.getScale()<127?SNOW_DENSE:STAR_DENSE)) == 0) &&
        myLamp.getPixColorXY(0U, i + 1U) == 0U &&
        myLamp.getPixColorXY(0U, i - 1U) == 0U)
    {
      myLamp.setLeds(myLamp.getPixelNumber(0U, i), CHSV(random(0, 200), (myLamp.effects.getScale()<127?SNOW_SATURATION:STAR_SATURATION), 255U));
    }
  }
  
  for (uint8_t i = 0U; i < WIDTH / 2U; i++)
  {
    if (myLamp.getPixColorXY(i, HEIGHT - 1U) == 0U &&
       (random(0, map((myLamp.effects.getScale()%128)*2, 0U, 255U, 10U, 120U)) == 0U) &&
        myLamp.getPixColorXY(i + 1U, HEIGHT - 1U) == 0U &&
        myLamp.getPixColorXY(i - 1U, HEIGHT - 1U) == 0U)
    {
      myLamp.setLeds(myLamp.getPixelNumber(i, HEIGHT - 1U), CHSV(random(0, 200),  (myLamp.effects.getScale()<127?SNOW_SATURATION:STAR_SATURATION), 255U));
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
    fadePixel(0U, i,  (myLamp.effects.getScale()<127?SNOW_TAIL_STEP:STAR_TAIL_STEP));
  }
  for (uint8_t i = 0U; i < WIDTH / 2U; i++)
  {
    fadePixel(i, HEIGHT - 1U, (myLamp.effects.getScale()<127?SNOW_TAIL_STEP:STAR_TAIL_STEP));
  }
}

// ------------- светлячки --------------
//#define LIGHTERS_AM           (100U)
void lightersRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    randomSeed(millis());
    for (uint8_t i = 0U; i < LIGHTERS_AM; i++)
    {
      GSHMEM.lightersIdx=0;
      GSHMEM.lightersPos[0U][i] = random(0, WIDTH);
      GSHMEM.lightersPos[1U][i] = random(0, HEIGHT);
      GSHMEM.lightersSpeed[0U][i] = random(-20, 20);
      GSHMEM.lightersSpeed[1U][i] = random(-20, 20);
      GSHMEM.lightersColor[i] = random(0U, 255U);
    }
  }

  float speedfactor = myLamp.effects.getSpeed()/4096.0+0.001;

  myLamp.blur2d(myLamp.effects.getSpeed()/10);
  myLamp.dimAll(50+myLamp.effects.getSpeed()/10);

  for (uint8_t i = 0U; i < (uint8_t)((LIGHTERS_AM/255.0)*myLamp.effects.getScale())+1; i++) // масштабируем на LIGHTERS_AM, чтобы не было выхода за диапазон
  {
    // EVERY_N_SECONDS(1)
    // {
    //   LOG.printf_P("S0:%d S1:%d P0:%3.2f P1:%3.2f, scale:%3.2f\n", GSHMEM.lightersSpeed[0U][i], GSHMEM.lightersSpeed[1U][i],GSHMEM.lightersPos[0U][i],GSHMEM.lightersPos[1U][i],speedfactor);
    // }

    EVERY_N_MILLIS(1000)
    {
      GSHMEM.lightersIdx = (GSHMEM.lightersIdx+1)%(uint8_t)(((LIGHTERS_AM/255.0)*myLamp.effects.getScale())+1);
      GSHMEM.lightersSpeed[0U][GSHMEM.lightersIdx] += random(-10, 10);
      GSHMEM.lightersSpeed[1U][GSHMEM.lightersIdx] += random(-10, 10);
      GSHMEM.lightersSpeed[0U][GSHMEM.lightersIdx] %= 21;
      GSHMEM.lightersSpeed[1U][GSHMEM.lightersIdx] %= 21;
    }

    GSHMEM.lightersPos[0U][i] += GSHMEM.lightersSpeed[0U][i]*speedfactor;
    GSHMEM.lightersPos[1U][i] += GSHMEM.lightersSpeed[1U][i]*speedfactor;

    if (GSHMEM.lightersPos[0U][i] < 0) GSHMEM.lightersPos[0U][i] = (WIDTH - 1);
    if (GSHMEM.lightersPos[0U][i] >= (int32_t)WIDTH) GSHMEM.lightersPos[0U][i] = 0;

    if (GSHMEM.lightersPos[1U][i] <= 0)
    {
      GSHMEM.lightersPos[1U][i] = 0;
      GSHMEM.lightersSpeed[1U][i] = -GSHMEM.lightersSpeed[1U][i];
      GSHMEM.lightersSpeed[0U][i] = -GSHMEM.lightersSpeed[0U][i];
    }
    if (GSHMEM.lightersPos[1U][i] >= (int32_t)(HEIGHT - 1))
    {
      GSHMEM.lightersPos[1U][i] = (HEIGHT - 1U);
      GSHMEM.lightersSpeed[1U][i] = -GSHMEM.lightersSpeed[1U][i];
      GSHMEM.lightersSpeed[0U][i] = -GSHMEM.lightersSpeed[0U][i];
    }
    myLamp.drawPixelXY(GSHMEM.lightersPos[0U][i], GSHMEM.lightersPos[1U][i], CHSV(GSHMEM.lightersColor[i], 255U, 255U));
  }
}

// ------------- светлячки со шлейфом -------------
//#define BALLS_AMOUNT          (7U)                          // максимальное количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define BALL_TRACK            (1U)                          // (0 / 1) - вкл/выкл следы шариков
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)
void ballsRoutine(CRGB *leds, const char *param)
{
  // static int16_t GSHMEM.coord[BALLS_AMOUNT][2U];
  // static int8_t GSHMEM.vector[BALLS_AMOUNT][2U];
  int16_t ballColors[BALLS_AMOUNT];

  if (myLamp.isLoading())
  {
    for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
    {
      int8_t sign;
      // забиваем случайными данными
      GSHMEM.coord[j][0U] = WIDTH / 2;
      random(0, 2) ? sign = 1 : sign = -1;
      GSHMEM.vector[j][0U] = random(4, 15) * sign;
      GSHMEM.coord[j][1U] = HEIGHT / 2;
      random(0, 2) ? sign = 1 : sign = -1;
      GSHMEM.vector[j][1U] = random(4, 15) * sign;
      //ballColors[j] = random(0, 9) * 28;
    }
  }

  // if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
  //   return;
  // } else {
  //   myLamp.setEffDelay(millis());
  // }

  float speedfactor = myLamp.effects.getSpeed()/2048.0+0.01;

  if (!BALL_TRACK)                                          // режим без следов шариков
  {
    FastLED.clear();
  }
  else                                                      // режим со следами
  {
    myLamp.blur2d(myLamp.effects.getSpeed()/10);
    fader(TRACK_STEP);
  }

  // движение шариков
  int maxBalls = (uint8_t)((BALLS_AMOUNT/255.0)*myLamp.effects.getScale()+0.99);
  for (uint8_t j = 0U; j < maxBalls; j++)
  {
    // цвет зависит от масштаба
    ballColors[j] = myLamp.effects.getScale() * (maxBalls-j) * BALLS_AMOUNT + j;
          
    // движение шариков
    for (uint8_t i = 0U; i < 2U; i++)
    {
      GSHMEM.coord[j][i] += GSHMEM.vector[j][i]*speedfactor;
      if (GSHMEM.coord[j][i] < 0)
      {
        GSHMEM.coord[j][i] = 0;
        GSHMEM.vector[j][i] = -GSHMEM.vector[j][i];
      }
    }

    if (GSHMEM.coord[j][0U] > (int16_t)((WIDTH - 1)))
    {
      GSHMEM.coord[j][0U] = (WIDTH - 1);
      GSHMEM.vector[j][0U] = -GSHMEM.vector[j][0U];
    }
    if (GSHMEM.coord[j][1U] > (int16_t)((HEIGHT - 1)))
    {
      GSHMEM.coord[j][1U] = (HEIGHT - 1);
      GSHMEM.vector[j][1U] = -GSHMEM.vector[j][1U];
    }
    myLamp.setLeds(myLamp.getPixelNumber(GSHMEM.coord[j][0U], GSHMEM.coord[j][1U]), CHSV(ballColors[j], 255U, 255U));
  }
}

// ------------- пейнтбол -------------
#define BORDERTHICKNESS       (1U)                          // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)
void lightBallsRoutine(CRGB *leds, const char *param)
{
  const uint8_t paintWidth = WIDTH - BORDERTHICKNESS * 2;
  const uint8_t paintHeight = HEIGHT - BORDERTHICKNESS * 2;

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  uint8_t blurAmount = dim8_raw(beatsin8(3,64,100));
  blur2d(leds, WIDTH, HEIGHT, blurAmount);

  float speedScale = (((float)myLamp.effects.getSpeed())/255)+(5.0/255.0);

  // Use two out-of-sync sine waves
  uint16_t  i = beatsin16( 79*speedScale, 0, 255); //91
  uint16_t  j = beatsin16( 67*speedScale, 0, 255); //109
  uint16_t  k = beatsin16( 53*speedScale, 0, 255); //73
  uint16_t  m = beatsin16( 97*speedScale, 0, 255); //123

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis() / (myLamp.effects.getScale()/16 + 1);
  leds[myLamp.getPixelNumber( highByte(i * paintWidth) + BORDERTHICKNESS, highByte(j * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 29, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(j * paintWidth) + BORDERTHICKNESS, highByte(k * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 41, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(k * paintWidth) + BORDERTHICKNESS, highByte(m * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 37, 200U, 255U);
  leds[myLamp.getPixelNumber( highByte(m * paintWidth) + BORDERTHICKNESS, highByte(i * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 53, 200U, 255U);
}

// ------------- блуждающий кубик -------------
#define RANDOM_COLOR          (1U)                          // случайный цвет при отскоке
void ballRoutine(CRGB *leds, const char *param)
{
  // static float GSHMEM.coordB[2U];
  // static int8_t GSHMEM.vectorB[2U];
  // static int16_t GSHMEM.ballColor;
  int8_t ballSize;

  ballSize = map(myLamp.effects.getScale(), 0U, 255U, 2U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 2));

  if (myLamp.isLoading())
  {
    //FastLED.clear();
    myLamp.setEffDelay(millis());
    for (uint8_t i = 0U; i < 2U; i++)
    {
      GSHMEM.coordB[i] = i? (WIDTH - ballSize) / 2 : (HEIGHT - ballSize) / 2;
      GSHMEM.vectorB[i] = random(8, 24) - 12;
      GSHMEM.ballColor = random(1, 255) * myLamp.effects.getScale();
    }
  }

  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) > 10000){ // каждые 10 секунд коррекция направления
    myLamp.setEffDelay(millis());
    for (uint8_t i = 0U; i < 2U; i++)
    {
      if(abs(GSHMEM.vectorB[i])<12)
        GSHMEM.vectorB[i] += (random(0, 8) - 4);
      else if (GSHMEM.vectorB[i]>12)
        GSHMEM.vectorB[i] -= random(1, 6);
      else
        GSHMEM.vectorB[i] += random(1, 6);
      if(!GSHMEM.vectorB[i]) GSHMEM.vectorB[i]++;
      GSHMEM.ballColor = random(1, 255) * myLamp.effects.getScale();
    }
  }

  for (uint8_t i = 0U; i < 2U; i++)
  {
    GSHMEM.coordB[i] += GSHMEM.vectorB[i]*((0.1*myLamp.effects.getSpeed())/255.0);
    if ((int8_t)GSHMEM.coordB[i] < 0)
    {
      GSHMEM.coordB[i] = 0;
      GSHMEM.vectorB[i] = -GSHMEM.vectorB[i];
      if (RANDOM_COLOR) GSHMEM.ballColor = random(1, 255) * myLamp.effects.getScale();
    }
  }
  if ((int8_t)GSHMEM.coordB[0U] > (int16_t)(WIDTH - ballSize))
  {
    GSHMEM.coordB[0U] = (WIDTH - ballSize);
    GSHMEM.vectorB[0U] = -GSHMEM.vectorB[0U];
    if (RANDOM_COLOR) GSHMEM.ballColor = random(1, 255) * myLamp.effects.getScale();
  }
  if ((int8_t)GSHMEM.coordB[1U] > (int16_t)(HEIGHT - ballSize))
  {
    GSHMEM.coordB[1U] = (HEIGHT - ballSize);
    GSHMEM.vectorB[1U] = -GSHMEM.vectorB[1U];
    if (RANDOM_COLOR) GSHMEM.ballColor = random(1, 255) * myLamp.effects.getScale();
  }

  //FastLED.clear();
  fader((uint8_t)(10*((float)myLamp.effects.getSpeed())/255)+50); // фейдер, 10 - коэф. масштабирование (вся шкала 0...10), 50 - смещение (мин. уровень фейдера)
  for (uint8_t i = 0U; i < ballSize; i++)
  {
    for (uint8_t j = 0U; j < ballSize; j++)
    {
      myLamp.setLeds(myLamp.getPixelNumber((int8_t)GSHMEM.coordB[0U] + i, (int8_t)GSHMEM.coordB[1U] + j), CHSV(GSHMEM.ballColor, 255U, 255U));
    }
  }
}

//-- 3D Noise эффектцы --------------
// uint16_t GSHMEM.speed = 20;                                        // speed is set dynamically once we've started up
// uint16_t GSHMEM.scale = 30;                                        // scale is set dynamically once we've started up
// uint8_t GSHMEM.ihue = 0;
// static uint16_t GSHMEM.x;
// static uint16_t GSHMEM.y;
// static uint16_t GSHMEM.z;
// #if (WIDTH > HEIGHT)
// uint8_t GSHMEM.noise[WIDTH][WIDTH];
// #else
// uint8_t GSHMEM.noise[HEIGHT][HEIGHT];
// #endif
// CRGBPalette16 GSHMEM.currentPalette(PartyColors_p);
// uint8_t GSHMEM.colorLoop = 1;

// ************* СЛУЖЕБНЫЕ *************

// для работы FastLed (blur2d)
uint16_t XY(uint8_t x, uint8_t y)
{
#ifdef ROTATED_MATRIX  
  return myLamp.getPixelNumber(y,x); // повернутое на 90 градусов
#else
  return myLamp.getPixelNumber(x,y); // обычное подключение
#endif
}

void fillNoiseLED()
{
  uint8_t dataSmoothing = 0;
  if (GSHMEM.speed < 50)
  {
    dataSmoothing = 200 - (GSHMEM.speed * 4);
  }
  for (uint8_t i = 0; i < myLamp.getminDim()*2; i++)
  {
    int32_t ioffset = GSHMEM.scale * i;
    for (uint8_t j = 0; j < myLamp.getmaxDim(); j++)
    {
      int32_t joffset = GSHMEM.scale * j;

      uint8_t data = inoise8(GSHMEM.x + ioffset, GSHMEM.y + joffset, GSHMEM.z);

      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing)
      {
        uint8_t olddata = GSHMEM.noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      GSHMEM.noise[i][j] = data;
    }
  }
  GSHMEM.z += GSHMEM.speed;

  // apply slow drift to X and Y, just for visual variation.
  GSHMEM.x += GSHMEM.speed / 8;
  GSHMEM.y -= GSHMEM.speed / 16;

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint8_t index = GSHMEM.noise[j%(myLamp.getminDim()*2)][i];
      uint8_t bri =   GSHMEM.noise[i%(myLamp.getminDim()*2)][j];
      // if this palette is a 'loop', add a slowly-changing base value
      if ( GSHMEM.colorLoop)
      {
        index += GSHMEM.ihue;
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
      CRGB color = ColorFromPalette( *(CRGBPalette16 *)(GSHMEM.currentPalette), index, bri);      
      myLamp.drawPixelXY(i, j, color);                             //leds[getPixelNumber(i, j)] = color;
    }
  }
  GSHMEM.ihue += 1;

// #if defined(LAMP_DEBUG) && defined(MIC_EFFECTS)
// EVERY_N_SECONDS(1){
//   LOG.printf_P(PSTR("MF: %5.2f MMF: %d MP:%d MMP: %d\n"), myLamp.getMicFreq(), myLamp.getMicMapFreq(), myLamp.getMicMaxPeak(), myLamp.getMicMapMaxPeak());
// }
// #endif

}

void fillnoise8()
{
  for (uint8_t i = 0; i < myLamp.getminDim()*2; i++)
  {
    int32_t ioffset = GSHMEM.scale * i;
    for (uint8_t j = 0; j < myLamp.getmaxDim(); j++)
    {
      int32_t joffset = GSHMEM.scale * j;
      GSHMEM.noise[i][j] = inoise8(GSHMEM.x + ioffset, GSHMEM.y + joffset, GSHMEM.z);
    }
  }
  GSHMEM.z += GSHMEM.speed;
}

void madnessNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillnoise8();
  for (uint8_t i = 0; i < WIDTH; i++)
  {
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      CRGB thisColor = CHSV(GSHMEM.noise[j][i], 255, GSHMEM.noise[i][j]);
      myLamp.drawPixelXY(i, j, thisColor);                         //leds[getPixelNumber(i, j)] = CHSV(GSHMEM.noise[j][i], 255, GSHMEM.noise[i][j]);
    }
  }
  GSHMEM.ihue += 1;
}

void rainbowNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = RainbowColors_p;
    GSHMEM.colorLoop = 1;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void rainbowStripeNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = RainbowStripeColors_p;
    GSHMEM.colorLoop = 1;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (32UL*myLamp.effects.getScale()/255+8UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 32UL*myLamp.effects.getScale()/255+8UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void zebraNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    // 'black out' all 16 palette entries...
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = PartyColors_p;
    CRGBPalette16 *cp = (CRGBPalette16 *)(GSHMEM.currentPalette);

    fill_solid(*cp, 16, CRGB::Black);
    // and set every fourth one to white.
    (*cp)[0] = CRGB::White;
    (*cp)[4] = CRGB::White;
    (*cp)[8] = CRGB::White;
    (*cp)[12] = CRGB::White;
    GSHMEM.colorLoop = 1;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+8UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>35?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+8L;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void forestNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = ForestColors_p;
    GSHMEM.colorLoop = 0;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void oceanNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = OceanColors_p;
    GSHMEM.colorLoop = 0;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (32UL*myLamp.effects.getScale()/255+8UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 32UL*myLamp.effects.getScale()/255+8UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void plasmaNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = PartyColors_p;
    GSHMEM.colorLoop = 1;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void cloudsNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = CloudColors_p;
    GSHMEM.colorLoop = 0;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

void lavaNoiseRoutine(CRGB *leds, const char *param)
{
  if (myLamp.isLoading())
  {
    *(CRGBPalette16 *)(GSHMEM.currentPalette) = LavaColors_p;
    GSHMEM.colorLoop = 0;
  }
  #ifdef MIC_EFFECTS
  uint8_t mmf = myLamp.getMicMapFreq();
  uint8_t mmp = myLamp.getMicMapMaxPeak();
  GSHMEM.scale = (64UL*myLamp.effects.getScale()/255+32UL)*(mmf>0?(1.5*mmf/255.0):1);
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
  GSHMEM.scale = 64UL*myLamp.effects.getScale()/255+32UL;
  GSHMEM.speed = 64UL*myLamp.effects.getSpeed()/255;
  #endif
  fillNoiseLED();
}

// --------------------------- эффект мячики ----------------------
//  BouncingBalls2014 is a program that lets you animate an LED strip
//  to look like a group of bouncing balls
//  Daniel Wilson, 2014
//  https://github.com/githubcdr/Arduino/blob/master/bouncingballs/bouncingballs.ino
//  With BIG thanks to the FastLED community!
//  адаптация от SottNick
#define bballsGRAVITY           (-9.81)              // Downward (negative) acceleration of gravity in m/s^2
#define bballsH0                (1)                  // Starting height, in meters, of the ball (strip length)
//#define bballsMaxNUM_BALLS      (16U)                // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
void BBallsRoutine(CRGB *leds, const char *param)
{
  uint8_t bballsNUM_BALLS;                             // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way) ... количество мячиков теперь задаётся бегунком, а не константой

  // static uint8_t GSHMEM.bballsCOLOR[bballsMaxNUM_BALLS] ;                   // прикручено при адаптации для разноцветных мячиков
  // static uint8_t GSHMEM.bballsX[bballsMaxNUM_BALLS] ;                       // прикручено при адаптации для распределения мячиков по радиусу лампы
  // static int   GSHMEM.bballsPos[bballsMaxNUM_BALLS] ;                       // The integer position of the dot on the strip (LED index)
  // static float GSHMEM.bballsH[bballsMaxNUM_BALLS] ;                         // An array of heights
  // static float GSHMEM.bballsVImpact[bballsMaxNUM_BALLS] ;                   // As time goes on the impact velocity will change, so make an array to store those values
  // static float GSHMEM.bballsTCycle[bballsMaxNUM_BALLS] ;                    // The time since the last time the ball struck the ground
  // static float GSHMEM.bballsCOR[bballsMaxNUM_BALLS] ;                       // Coefficient of Restitution (bounce damping)
  // static long  GSHMEM.bballsTLast[bballsMaxNUM_BALLS] ;                     // The clock time of the last ground strike
  float bballsVImpact0 = sqrt( -2 * bballsGRAVITY * bballsH0 );      // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip

  bballsNUM_BALLS = (uint8_t)((bballsMaxNUM_BALLS * myLamp.effects.getScale())/256+1);
  for (int i = 0 ; i < bballsNUM_BALLS ; i++)
    GSHMEM.bballsCOR[i] = 0.90 - float(i)/(255-myLamp.effects.getSpeed()); // это, видимо, прыгучесть. для каждого мячика уникальная изначально

  if (myLamp.isLoading()){
    FastLED.clear();

    for (int i = 0 ; i < bballsNUM_BALLS ; i++) {          // Initialize variables
      GSHMEM.bballsCOLOR[i] = random(0U, 256U);
      GSHMEM.bballsX[i] = random(0U, WIDTH);
      GSHMEM.bballsTLast[i] = millis();
      GSHMEM.bballsH[i] = bballsH0;
      GSHMEM.bballsPos[i] = 0;                                    // Balls start on the ground
      GSHMEM.bballsVImpact[i] = bballsVImpact0;                   // And "pop" up at vImpact0
      GSHMEM.bballsTCycle[i] = 0;
    }
  }
  

  for (int i = 0 ; i < bballsNUM_BALLS ; i++) {
    myLamp.setLeds(myLamp.getPixelNumber(GSHMEM.bballsX[i], GSHMEM.bballsPos[i]), CRGB::Black); // off for the next loop around

    GSHMEM.bballsTCycle[i] =  millis() - GSHMEM.bballsTLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    GSHMEM.bballsH[i] = 0.5 * bballsGRAVITY * pow( GSHMEM.bballsTCycle[i]/1000 , 2.0 ) + GSHMEM.bballsVImpact[i] * GSHMEM.bballsTCycle[i]/1000;

    if ( GSHMEM.bballsH[i] < 0 ) {                      
      GSHMEM.bballsH[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      GSHMEM.bballsVImpact[i] = GSHMEM.bballsCOR[i] * GSHMEM.bballsVImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
      GSHMEM.bballsTLast[i] = millis();

//      if ( GSHMEM.bballsVImpact[i] < 0.01 ) GSHMEM.bballsVImpact[i] = GSHMEM.bballsVImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
      if ( GSHMEM.bballsVImpact[i] < 0.01 ) // сделал, чтобы мячики меняли свою прыгучесть и положение каждый цикл
        {
          switch (random(3U)) // этот свитч двигает мячики влево-вправо иногда
            {
              case 0U:
              {
                if (GSHMEM.bballsX[i] == 0U) GSHMEM.bballsX[i] = WIDTH - 1U;
                  else --GSHMEM.bballsX[i];
                break;
              }
              case 2U:
              {
                if (GSHMEM.bballsX[i] == WIDTH - 1U) GSHMEM.bballsX[i] = 0U;
                  else ++GSHMEM.bballsX[i];
                break;
              }
            }
          GSHMEM.bballsCOR[i] = 0.90 - float(random(0U,5U))/pow(random(1U,6U),2); // а это прыгучесть меняется. кажется, не очень удачно сделано
          GSHMEM.bballsVImpact[i] = bballsVImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
        }
    }
    GSHMEM.bballsPos[i] = round( GSHMEM.bballsH[i] * (HEIGHT - 1) / bballsH0);       // Map "h" to a "pos" integer index position on the LED strip

    myLamp.setLeds(myLamp.getPixelNumber(GSHMEM.bballsX[i], GSHMEM.bballsPos[i]), CHSV(GSHMEM.bballsCOLOR[i], 255, 255));
  }
}

// ***** SINUSOID3 / СИНУСОИД3 *****
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
/*
  Sinusoid3 by Stefan Petrick (mod by Palpalych for GyverLamp 27/02/2020)
  read more about the concept: https://www.youtube.com/watch?v=mubH-w_gwdA
*/
void Sinusoid3Routine (CRGB *leds, const char *param)
{
  const uint8_t semiHeightMajor =  HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t semiWidthMajor =  WIDTH / 2  + (WIDTH % 2) ;
  float e_s3_speed = 0.004 * myLamp.effects.getSpeed() + 0.015; // speed of the movement along the Lissajous curves
  float e_s3_size = 3 * (float)myLamp.effects.getScale()/255.0 + 2;    // amplitude of the curves

  float time_shift = millis(); //float(millis()%(uint32_t)(30000*(1.0/((float)myLamp.effects.getSpeed()/255))));

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      CRGB color;
      uint8_t scale = map8(255-myLamp.effects.getScale(),50,150);

      float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 98.301 * time_shift)))/32767.0;  // the 8 centers the middle on a 16x16
      float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 72.0874 * time_shift)))/32767.0;
      //int8_t v = 127 * (1 + sinf ( sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) ));
      //int8_t v = 127 * (1 + sin16 ( 32767.0/3*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0); // 32767.0/3 или 32767.0/2 как оригинальный эффект
      int8_t v = 127 * (1 + sin16 ( 127*scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.r = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 68.8107 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 65.534 * time_shift)))/32767.0;
      //v = 127 * (1 + sinf ( sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) ));
      //v = 127 * (1 + sin16 ( 32767.0/3*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      v = 127 * (1 + sin16 ( 127*scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.g = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 134.3447 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 170.3884 * time_shift)))/32767.0;
      //v = 127 * (1 + sinf ( sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) ));
      //v = 127 * (1 + sin16 ( 32767.0/3*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      v = 127 * (1 + sin16 ( 127*scale*sqrtf ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.b = ~v;

      // EVERY_N_SECONDS(1){
      //   LOG.printf_P("%f %f %f\n", cx,cy,v);
      // }

      myLamp.drawPixelXY(x, y, color);
    }
  }
}

/*
  Metaballs proof of concept by Stefan Petrick (mod by Palpalych for GyverLamp 27/02/2020)
  ...very rough 8bit math here...
  read more about the concept of isosurfaces and metaballs:
  https://www.gamedev.net/articles/programming/graphics/exploring-metaballs-and-isosurfaces-in-2d-r2556
*/

// ***** METABALLS / МЕТАШАРИКИ *****
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
void metaBallsRoutine(CRGB *leds, const char *param)
{
  float speed = myLamp.effects.getSpeed()/127.0;

  // get some 2 random moving points
  uint8_t x2 = inoise8(millis() * speed, 25355, 685 ) / WIDTH;
  uint8_t y2 = inoise8(millis() * speed, 355, 11685 ) / HEIGHT;

  uint8_t x3 = inoise8(millis() * speed, 55355, 6685 ) / WIDTH;
  uint8_t y3 = inoise8(millis() * speed, 25355, 22685 ) / HEIGHT;

  // and one Lissajou function
  uint8_t x1 = beatsin8(23 * speed, 0, 15);
  uint8_t y1 = beatsin8(28 * speed, 0, 15);

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
      byte color = myLamp.effects.getScale()*4 / dist;

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
}

// --------------------------- эффект спирали ----------------------
/*
 * Aurora: https://github.com/pixelmatix/aurora
 * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
 * Copyright (c) 2014 Jason Coon
 * Неполная адаптация SottNick
 */
uint8_t mapsin8(uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255) {
  uint8_t beatsin = sin8(theta);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatsin, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

uint8_t mapcos8(uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255) {
  uint8_t beatcos = cos8(theta);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatcos, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

void spiroRoutine(CRGB *leds, const char *param)
{
    //static float GSHMEM.spirotheta1 = 0;
    //static float GSHMEM.spirotheta2 = 0;
    //static byte GSHMEM.spirohueoffset = 0;
    //static uint8_t GSHMEM.spirocount = 1;
    //static boolean GSHMEM.spiroincrement = false;
    //static boolean GSHMEM.spirohandledChange = false;
    if(myLamp.isLoading()){
      GSHMEM.spirotheta1 = 0;
      GSHMEM.spirotheta2 = 0;
      GSHMEM.spirohueoffset = 0;
      GSHMEM.spirocount = 1;
      GSHMEM.spiroincrement = false;
      GSHMEM.spirohandledChange = false;
    }

    const uint8_t spiroradiusx = WIDTH / 4;
    const uint8_t spiroradiusy = HEIGHT / 4;
    
    const uint8_t spirocenterX = WIDTH / 2;
    const uint8_t spirocenterY = HEIGHT / 2;
    
    const uint8_t spirominx = spirocenterX - spiroradiusx;
    const uint8_t spiromaxx = spirocenterX + spiroradiusx + 1;
    const uint8_t spirominy = spirocenterY - spiroradiusy;
    const uint8_t spiromaxy = spirocenterY + spiroradiusy + 1;
  
    const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
    const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];
    const float speed_factor = (float)myLamp.effects.getSpeed()/127+1;
    uint8_t spirooffset = 256 / GSHMEM.spirocount;
    boolean change = false;

    myLamp.blur2d(15);//45/(speed_factor*3));
    myLamp.dimAll(254U - (32-(myLamp.effects.getScale()%32)));
    //myLamp.dimAll(250-speed_factor*7);

    
    for (int i = 0; i < GSHMEM.spirocount; i++) {
      uint8_t x = mapsin8(GSHMEM.spirotheta1 + i * spirooffset, spirominx, spiromaxx);
      uint8_t y = mapcos8(GSHMEM.spirotheta1 + i * spirooffset, spirominy, spiromaxy);

      uint8_t x2 = mapsin8(GSHMEM.spirotheta2 + i * spirooffset, x - spiroradiusx, x + spiroradiusx);
      uint8_t y2 = mapcos8(GSHMEM.spirotheta2 + i * spirooffset, y - spiroradiusy, y + spiroradiusy);

      CRGB color = ColorFromPalette(*curPalette, (GSHMEM.spirohueoffset + i * spirooffset), 128U);

      if (x2<WIDTH && y2<HEIGHT){ // добавил проверки. не знаю, почему эффект подвисает без них
        CRGB tmpColor = myLamp.getPixColorXY(x2, y2);
        tmpColor += color;
        myLamp.setLeds(myLamp.getPixelNumber(x2, y2), tmpColor); // += color
      }
      
      if(x2 == spirocenterX && y2 == spirocenterY) change = true;
    }

    GSHMEM.spirotheta2 += 2*speed_factor;

    EVERY_N_MILLIS(EFFECTS_RUN_TIMER/2) {
      GSHMEM.spirotheta1 += 1*speed_factor;
    }

    EVERY_N_MILLIS(50) {
      if (change && !GSHMEM.spirohandledChange) {
        GSHMEM.spirohandledChange = true;
        
        if (GSHMEM.spirocount >= WIDTH || GSHMEM.spirocount == 1) GSHMEM.spiroincrement = !GSHMEM.spiroincrement;

        if (GSHMEM.spiroincrement) {
          if(GSHMEM.spirocount >= 4)
            GSHMEM.spirocount *= 2;
          else
            GSHMEM.spirocount += 1;
        }
        else {
          if(GSHMEM.spirocount > 4)
            GSHMEM.spirocount /= 2;
          else
            GSHMEM.spirocount -= 1;
        }

        spirooffset = 256 / GSHMEM.spirocount;
      }
      
      if(!change) GSHMEM.spirohandledChange = false;
    }

    EVERY_N_MILLIS(33) {
      GSHMEM.spirohueoffset += 1;
    }
}

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
// v1.5.0 - Follow the Rainbow Comet / first release by PalPalych 29.02.2020
// v1.7.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
// v1.7.1 - Scale & Speed correction, dither & blur 23.03.2020
// v1.7.2 - 0 Scale for Random 3d color 24.03.2020

#define e_com_TAILSPEED             (500)         // скорость смещения хвоста 
#define e_com_BLUR                  (24U)         // размытие хвоста 
#define e_com_3DCOLORSPEED          (5U)          // скорость случайного изменения цвета (0й - режим)

// // The coordinates for 3 16-bit noise spaces.
// #define NUM_LAYERS 1
// uint32_t GSHMEM.e_x[NUM_LAYERS];
// uint32_t GSHMEM.e_y[NUM_LAYERS];
// uint32_t GSHMEM.e_z[NUM_LAYERS];
// uint32_t GSHMEM.e_scaleX[NUM_LAYERS];
// uint32_t GSHMEM.e_scaleY[NUM_LAYERS];
// uint8_t GSHMEM.noise3d[NUM_LAYERS][WIDTH][HEIGHT];
// uint8_t GSHMEM.eNs_noisesmooth;

void FillNoise(int8_t layer) {
  const uint8_t e_centerX =  (WIDTH / 2) - 1;
  const uint8_t e_centerY = (HEIGHT / 2) - 1;

  for (uint8_t i = 0; i < WIDTH; i++) {
    int32_t ioffset = GSHMEM.e_scaleX[layer] * (i - e_centerX);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      int32_t joffset = GSHMEM.e_scaleY[layer] * (j - e_centerY);
      int8_t data = inoise16(GSHMEM.e_x[layer] + ioffset, GSHMEM.e_y[layer] + joffset, GSHMEM.e_z[layer]) >> 8;
      int8_t olddata = GSHMEM.noise3d[layer][i][j];
      int8_t newdata = scale8( olddata, GSHMEM.eNs_noisesmooth ) + scale8( data, 255 - GSHMEM.eNs_noisesmooth );
      data = newdata;
      GSHMEM.noise3d[layer][i][j] = data;
    }
  }
}

void MoveFractionalNoiseX(int8_t amplitude = 1, float shift = 0) {
  uint8_t zD;
  uint8_t zF;
  CRGB *leds = myLamp.getLeds(); // unsafe
  CRGB ledsbuff[NUM_LEDS];

  for(uint8_t i=0; i<NUM_LAYERS; i++)
    for (uint8_t y = 0; y < HEIGHT; y++) {
      int16_t amount = ((int16_t)(GSHMEM.noise3d[i][0][y] - 128) * 2 * amplitude + shift * 256);
      int8_t delta = ((uint16_t)abs(amount) >> 8) ;
      int8_t fraction = ((uint16_t)abs(amount) & 255);
      for (uint8_t x = 0 ; x < WIDTH; x++) {
        if (amount < 0) {
          zD = x - delta; zF = zD - 1;
        } else {
          zD = x + delta; zF = zD + 1;
        }
        CRGB PixelA = CRGB::Black  ;
        if ((zD >= 0) && (zD < WIDTH)) PixelA = myLamp.getLeds(myLamp.getPixelNumber(zD%WIDTH, y%HEIGHT));
        CRGB PixelB = CRGB::Black ;
        if ((zF >= 0) && (zF < WIDTH)) PixelB = myLamp.getLeds(myLamp.getPixelNumber(zF%WIDTH, y%HEIGHT));
        //myLamp.setLeds(myLamp.getPixelNumber(x, y), (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction))));   // lerp8by8(PixelA, PixelB, fraction );
        //((CRGB *)GSHMEM.ledsbuff)[myLamp.getPixelNumber(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
        ledsbuff[myLamp.getPixelNumber(x%WIDTH, y%HEIGHT)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
      }
    }
  // memcpy(leds, GSHMEM.ledsbuff, sizeof(CRGB)* NUM_LEDS);
  memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);
}

void MoveFractionalNoiseY(int8_t amplitude = 1, float shift = 0) {
  uint8_t zD;
  uint8_t zF;
  CRGB *leds = myLamp.getLeds(); // unsafe
  CRGB ledsbuff[NUM_LEDS];

  for(uint8_t i=0; i<NUM_LAYERS; i++)
    for (uint8_t x = 0; x < WIDTH; x++) {
      int16_t amount = ((int16_t)(GSHMEM.noise3d[i][x][0] - 128) * 2 * amplitude + shift * 256);
      int8_t delta = (uint16_t)abs(amount) >> 8 ;
      int8_t fraction = (uint16_t)abs(amount) & 255;
      for (uint8_t y = 0 ; y < HEIGHT; y++) {
        if (amount < 0) {
          zD = y - delta; zF = zD - 1;
        } else {
          zD = y + delta; zF = zD + 1;
        }
        CRGB PixelA = CRGB::Black ;
        if ((zD >= 0) && (zD < HEIGHT)) PixelA = myLamp.getLeds(myLamp.getPixelNumber(x%WIDTH, zD%HEIGHT));
        CRGB PixelB = CRGB::Black ;
        if ((zF >= 0) && (zF < HEIGHT)) PixelB = myLamp.getLeds(myLamp.getPixelNumber(x%WIDTH, zF%HEIGHT));
        //myLamp.setLeds(myLamp.getPixelNumber(x, y), (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction))));
        //((CRGB *)GSHMEM.ledsbuff)[myLamp.getPixelNumber(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
        ledsbuff[myLamp.getPixelNumber(x%WIDTH, y%HEIGHT)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
      }
    }
  // memcpy(leds, GSHMEM.ledsbuff, sizeof(CRGB)* NUM_LEDS);
  memcpy(leds, ledsbuff, sizeof(CRGB)* NUM_LEDS);
}

void drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color)
{ // Fine if: 0<x1<x2<WIDTH && 0<y1<y2<HEIGHT
  for (int8_t xP = x1; xP <= x2; xP++)
  {
    for (int8_t yP = y1; yP <= y2; yP++)
    {
      myLamp.setLeds(myLamp.getPixelNumber(xP, yP), color);
    }
  }
}

void rainbowCometRoutine(CRGB *leds, const char *param)
{ // Rainbow Comet by PalPalych
/*
  Follow the Rainbow Comet Efect by PalPalych
  Speed = tail dispersing
  Scale = 0 - Random 3d color
          1...127 - time depending color
          128...254 - selected color
          255 - white
*/
  const uint8_t e_centerX =  (WIDTH / 2) - 1;
  const uint8_t e_centerY = (HEIGHT / 2) - 1;
  uint8_t Scale = myLamp.effects.getScale();


  if(myLamp.isLoading()){
    //randomSeed(millis());
    GSHMEM.eNs_noisesmooth = random(0, 200*(uint_fast16_t)myLamp.effects.getSpeed()/255); // степень сглаженности шума 0...200
    //for(uint8_t i=0; i<NUM_LAYERS;i++){
        // GSHMEM.e_x[i] = random16();
        // GSHMEM.e_y[i] = random16();
        // GSHMEM.e_z[i] = random16();
        // GSHMEM.e_scaleX[i] = 6000;
        // GSHMEM.e_scaleY[i] = 6000;
    //}
    // memset(GSHMEM.e_x,0,sizeof(GSHMEM.e_x));
    // memset(GSHMEM.e_y,0,sizeof(GSHMEM.e_y));
    // memset(GSHMEM.e_z,0,sizeof(GSHMEM.e_z));
    // memset(GSHMEM.e_scaleX,0,sizeof(GSHMEM.e_scaleX));
    // memset(GSHMEM.e_scaleY,0,sizeof(GSHMEM.e_scaleY));
    // memset(GSHMEM.noise3d,0,sizeof(GSHMEM.noise3d));
  }

  myLamp.blur2d(e_com_BLUR);    // < -- размытие хвоста
  myLamp.dimAll(254);            // < -- затухание эффекта для последующего кадра
  CRGB _eNs_color = CRGB::White;
  if (Scale <= 1) {
    _eNs_color = CHSV(GSHMEM.noise3d[0][0][0] * e_com_3DCOLORSPEED , 255, 255);
  } else if (Scale < 128) {
    _eNs_color = CHSV(millis() / ((uint16_t)Scale + 1U) * 4 + 10, 255, 255);
  } else if (Scale < 255) {
    _eNs_color = CHSV((Scale - 128) * 2, 255, 255);
  }
  drawFillRect2_fast(e_centerX - 1, e_centerY - 1, e_centerX + 1, e_centerY + 1, _eNs_color);
  // Noise
  uint16_t sc = (uint16_t)myLamp.effects.getSpeed() * 30 + 500; //64 + 1000;
  uint16_t sc2 = (float)myLamp.effects.getSpeed()/127.0+1.5; //1.5...3.5;
  for(uint8_t i=0; i<NUM_LAYERS; i++){
    GSHMEM.e_x[i] += e_com_TAILSPEED*sc2;
    GSHMEM.e_y[i] += e_com_TAILSPEED*sc2;
    GSHMEM.e_z[i] += e_com_TAILSPEED*sc2;
    GSHMEM.e_scaleX[i] = sc; // 8000;
    GSHMEM.e_scaleY[i] = sc; // 8000;
    FillNoise(i);
  }
  MoveFractionalNoiseX(WIDTH / 2U - 1U);
  MoveFractionalNoiseY(HEIGHT / 2U - 1U);
}

void rainbowComet3Routine(CRGB *leds, const char *param)
{ // Rainbow Comet by PalPalych
/*
  Follow the Rainbow Comet Efect by PalPalych
  Speed = tail dispersing
  Scale = 0 - Random 3d color
          1...127 - time depending color
          128...254 - selected color
          255 - white
*/
  const uint8_t e_centerX =  (WIDTH / 2) - 1;
  const uint8_t e_centerY = (HEIGHT / 2) - 1;
  uint8_t Scale = myLamp.effects.getScale();


  if(myLamp.isLoading()){
    //randomSeed(millis());
    GSHMEM.eNs_noisesmooth = random(0, 200*(uint_fast16_t)myLamp.effects.getSpeed()/255); // степень сглаженности шума 0...200
    //for(uint8_t i=0; i<NUM_LAYERS;i++){
        // GSHMEM.e_x[i] = random16();
        // GSHMEM.e_y[i] = random16();
        // GSHMEM.e_z[i] = random16();
        // GSHMEM.e_scaleX[i] = 6000;
        // GSHMEM.e_scaleY[i] = 6000;
    //}
    // memset(GSHMEM.e_x,0,sizeof(GSHMEM.e_x));
    // memset(GSHMEM.e_y,0,sizeof(GSHMEM.e_y));
    // memset(GSHMEM.e_z,0,sizeof(GSHMEM.e_z));
    // memset(GSHMEM.e_scaleX,0,sizeof(GSHMEM.e_scaleX));
    // memset(GSHMEM.e_scaleY,0,sizeof(GSHMEM.e_scaleY));
    // memset(GSHMEM.noise3d,0,sizeof(GSHMEM.noise3d));
  }

  myLamp.blur2d(Scale/5+1);    // < -- размытие хвоста
  myLamp.dimAll(255-Scale/66);            // < -- затухание эффекта для последующего кадра
  byte xx = 2 + sin8( millis() / 10) / 22;
  byte yy = 2 + cos8( millis() / 9) / 22;
  myLamp.setLeds(myLamp.getPixelNumber( xx%WIDTH, yy%HEIGHT), 0x0000FF);

  xx = 4 + sin8( millis() / 10) / 32;
  yy = 4 + cos8( millis() / 7) / 32;
  myLamp.setLeds(myLamp.getPixelNumber( xx%WIDTH, yy%HEIGHT), 0xFF0000);
  myLamp.setLeds(myLamp.getPixelNumber( e_centerX, e_centerY), 0x00FF00);

  uint16_t sc = (uint16_t)myLamp.effects.getSpeed() * 30 + 500; //64 + 1000;
  uint16_t sc2 = (float)myLamp.effects.getSpeed()/127.0+1.5; //1.5...3.5;
  for(uint8_t i=0; i<NUM_LAYERS; i++){
    GSHMEM.e_x[i] += 1500*sc2;
    GSHMEM.e_y[i] += 1500*sc2;
    GSHMEM.e_z[i] += 1500*sc2;
    GSHMEM.e_scaleX[i] = sc; // 8000;
    GSHMEM.e_scaleY[i] = sc; // 8000;
    FillNoise(i);
  }
  MoveFractionalNoiseX(2);
  MoveFractionalNoiseY(2, 0.33);
}

// ============= ЭФФЕКТ ПРИЗМАТА =============== 
// Prismata Loading Animation
void prismataRoutine(CRGB *leds, const char *param)
{ 
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

  EVERY_N_MILLIS(100) {
    GSHMEM.spirohueoffset += 1;
  }

  myLamp.blur2d(15);
  myLamp.dimAll(254U - (31-(myLamp.effects.getScale()%32))*8);
  for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t y = beatsin8(x + 1 * myLamp.effects.getSpeed()/5, 0, HEIGHT-1);
      myLamp.drawPixelXY(x, y, ColorFromPalette(*curPalette, (x+GSHMEM.spirohueoffset) * 4));
    }
}

// ============= ЭФФЕКТ СТАЯ ===============
// Адаптация от (c) SottNick
// Boid GSHMEM.boids[AVAILABLE_BOID_COUNT]; 
// Boid GSHMEM.predator;
// PVector GSHMEM.wind;
// bool GSHMEM.predatorPresent = true;

void flockRoutine(CRGB *leds, const char *param) {
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];
  Boid boids[AVAILABLE_BOID_COUNT];
  Boid predator;
  PVector wind;
  memcpy(boids,GSHMEM.boids,sizeof(Boid)*AVAILABLE_BOID_COUNT);
  memcpy(&predator,GSHMEM.predator,sizeof(Boid));
  memcpy(&wind,GSHMEM.wind,sizeof(PVector));

    if (myLamp.isLoading())
    {
      FastLED.clear();

      for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
        boids[i] = Boid(15, 15);
        boids[i].maxspeed = 0.380*myLamp.effects.getSpeed()/127.0+0.380/2;
        boids[i].maxforce = 0.015*myLamp.effects.getSpeed()/127.0+0.015/2;
      }
      GSHMEM.predatorPresent = random(0, 2) >= 1;
      if (GSHMEM.predatorPresent) {
        predator = Boid(31, 31);
        GSHMEM.predatorPresent = true;
        predator.maxspeed = 0.385*myLamp.effects.getSpeed()/127.0+0.385/2;
        predator.maxforce = 0.020*myLamp.effects.getSpeed()/127.0+0.020/2;
        predator.neighbordist = 8.0;
        predator.desiredseparation = 0.0;
      }
    }
    
      myLamp.blur2d(15);
      myLamp.dimAll(254U - (31-(myLamp.effects.getScale()%32))*8);

      bool applyWind = random(0, 255) > 240;
      if (applyWind) {
        wind.x = Boid::randomf() * .015 * myLamp.effects.getSpeed()/127.0 + .015/2;
        wind.y = Boid::randomf() * .015 * myLamp.effects.getSpeed()/127.0 + .015/2;
      }

      CRGB color = ColorFromPalette(*curPalette, GSHMEM.hueoffset);
      

      for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
        Boid * boid = &boids[i];

        if (GSHMEM.predatorPresent) {
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

      if (GSHMEM.predatorPresent) {
        predator.run(boids, AVAILABLE_BOID_COUNT);
        predator.wrapAroundBorders();
        color = ColorFromPalette(*curPalette, GSHMEM.hueoffset + 128);
        PVector location = predator.location;
        // PVector velocity = predator.velocity;
        // backgroundLayer.drawLine(location.x, location.y, location.x - velocity.x, location.y - velocity.y, color);
        // effects.leds[XY(location.x, location.y)] += color;        
        myLamp.drawPixelXY(location.x, location.y, color);        
      }

      EVERY_N_MILLIS(333) {
        GSHMEM.hueoffset += 1;
      }
      
      EVERY_N_SECONDS(30) {
        GSHMEM.predatorPresent = !GSHMEM.predatorPresent;
      }

      memcpy(GSHMEM.predator,&predator,sizeof(Boid));
      memcpy(GSHMEM.wind,&wind,sizeof(PVector));
      memcpy(GSHMEM.boids,boids,sizeof(Boid)*AVAILABLE_BOID_COUNT);
}

// ============= SWIRL /  ВОДОВОРОТ ===============
// Prismata Loading Animation
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither by PalPalych 12.04.2020
// Aurora: https://github.com/pixelmatix/aurora
// Copyright (c) 2014 Jason Coon

#define e_swi_BORDER (1U)  // размытие экрана за активный кадр
void swirlRoutine(CRGB *leds, const char *param)
{
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];
  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
#if (WIDTH < 25)
  myLamp.blur2d(beatsin8(2, 10, 128 + (31-(myLamp.effects.getScale()%32))*3));
#else
  // Never mind, on my 64x96 array, the dots are just too small
   myLamp.blur2d(172);
#endif

  // Use two out-of-sync sine waves
  uint8_t i = beatsin8(27*(myLamp.effects.getSpeed()/100.0)+5, e_swi_BORDER, HEIGHT - e_swi_BORDER); // borderWidth
  uint8_t j = beatsin8(41*(myLamp.effects.getSpeed()/100.0)+5, e_swi_BORDER, WIDTH - e_swi_BORDER);
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
}

#define CENTER_max  max(WIDTH / 2, HEIGHT / 2) // Наибольшее значение центра
#define WIDTH_steps  256U / WIDTH   // диапазон значений приходящихся на 1 пиксель ширины матрицы
#define HEIGHT_steps 256U / HEIGHT // диапазон значений приходящихся на 1 пиксель высоты матрицы
// ============= DRIFT / ДРИФТ ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h
void incrementalDriftRoutine(CRGB *leds, const char *param)
{
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

  myLamp.blur2d(beatsin8(3U, 5, 10 + (31-(myLamp.effects.getScale()%32))*3));
  myLamp.dimAll(beatsin8(2U, 246, 252));
  uint8_t _dri_speed = map8(myLamp.effects.getSpeed(), 1U, 20U);
  uint8_t _dri_delta = beatsin8(1U);
  EVERY_N_MILLIS(13){
  GSHMEM.dri_phase++;
  }
  for (uint8_t i = 1; i < WIDTH / 2U; i++) // возможно, стоит здесь использовать const MINLENGTH
  {
    int8_t x = beatsin8((CENTER_max - i) * _dri_speed, WIDTH / 2U - 1 - i, WIDTH / 2U - 1 + 1U + i, 0, 64U + GSHMEM.dri_phase); // используем константы центра матрицы из эффекта Кометы
    int8_t y = beatsin8((CENTER_max - i) * _dri_speed, WIDTH / 2U - 1 - i, WIDTH / 2U - 1 + 1U + i, 0, GSHMEM.dri_phase);       // используем константы центра матрицы из эффекта Кометы
    myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, (i - 1U) * WIDTH_steps * 2U + _dri_delta) ); // используем массив палитр из других эффектов выше
  }
}

// ============= DRIFT 2 / ДРИФТ 2 ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift2.h
void incrementalDriftRoutine2(CRGB *leds, const char *param)
{
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

  myLamp.blur2d(beatsin8(3U, 5, 10 + (31-(myLamp.effects.getScale()%32))*3));
  myLamp.dimAll(beatsin8(2U, 224, 252));
  uint8_t _dri_speed = map8(myLamp.effects.getSpeed(), 1U, 15U);
  uint8_t _dri_delta = beatsin8(1U);
  EVERY_N_MILLIS(13)
  {
    GSHMEM.dri_phase++;
  }
  for (uint8_t i = 0; i < WIDTH; i++)
  {
    int8_t x = 0;
    int8_t y = 0;
    CRGB color;
    if (i < WIDTH / 2U)
    {
      x = beatsin8((i + 1) * _dri_speed, i + 1U, WIDTH - 1 - i, 0, 64U + GSHMEM.dri_phase);
      y = beatsin8((i + 1) * _dri_speed, i + 1U, HEIGHT - 1 - i, 0, GSHMEM.dri_phase);
      color = ColorFromPalette(*curPalette, i * WIDTH_steps * 2U + _dri_delta);
    }
    else
    {
      x = beatsin8((WIDTH - i) * _dri_speed, WIDTH - 1 - i, i + 1U, 0, GSHMEM.dri_phase);
      y = beatsin8((HEIGHT - i) * _dri_speed, HEIGHT - 1 - i, i + 1U, 0, 64U + GSHMEM.dri_phase);
      color = ColorFromPalette(*curPalette, ~(i * WIDTH_steps * 2U + _dri_delta));
    }
    myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
  }
}

#ifdef MIC_EFFECTS
void freqAnalyseRoutine(CRGB *leds, const char *param)
{
  myLamp.setMicAnalyseDivider(0); // отключить авто-работу микрофона, т.к. тут все анализируется отдельно, т.е. не нужно выполнять одну и ту же работу дважды

  if(myLamp.isLoading()){
    memset(GSHMEM.peakX,0,sizeof(GSHMEM.peakX));
  }
  
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  float samp_freq;
  double last_freq;
  uint8_t last_min_peak, last_max_peak;
  float x[WIDTH+1]; memset(x,0,sizeof(x));
  float maxVal;

  MICWORKER *mw = new MICWORKER(myLamp.getMicScale(),myLamp.getMicNoise());
  samp_freq = mw->process(myLamp.getMicNoiseRdcLevel()); // частота семплирования
  last_min_peak = mw->getMinPeak();
  last_max_peak = mw->getMaxPeak()*2;

  maxVal=mw->fillSizeScaledArray(x,WIDTH);

  float scale = (maxVal==0? 0 : last_max_peak/maxVal);
  scale = scale * (myLamp.effects.getScale()/255.0);

EVERY_N_SECONDS(1){
  for(uint8_t i=0; i<WIDTH; i++)
    LOG.printf_P(PSTR("%5.2f "),x[i]);
  LOG.printf_P(PSTR("F: %8.2f SC: %5.2f\n"),x[WIDTH], scale); 
}

  for(uint8_t xpos=0; xpos<WIDTH; xpos++){
    for(uint8_t ypos=0; ypos<HEIGHT; ypos++){
      uint32_t color = (x[xpos]*scale*(1.0/(ypos+1)))>5?255:0;
      if(color==255 && GSHMEM.peakX[1][xpos] < ypos){
        GSHMEM.peakX[1][xpos]=ypos;
        GSHMEM.peakX[0][xpos]=10;
      }
      if(ypos>(1.5*HEIGHT/2.0)){
        color=color<<16;
      } else if(ypos>(HEIGHT/2.0)){
        color=(color<<8)|(color<<16);
      } else {
        color=color<<8;
      }
      myLamp.setLeds(myLamp.getPixelNumber(xpos,ypos), color);
    }
  }

  bool isfall=false;
  EVERY_N_MILLISECONDS(70){
    isfall = true;
  }

  for (size_t i = 0; i < WIDTH; i++)
  {
    uint32_t color = 255;
    int8_t &ypos=GSHMEM.peakX[1][i];
    if(GSHMEM.peakX[0][i])
      GSHMEM.peakX[0][i]--;
    if(isfall && ypos>0 && !GSHMEM.peakX[0][i]) ypos--;

    if(ypos>(1.5*HEIGHT/2.0)){
      color=color<<16;
    } else if(ypos>(HEIGHT/2.0)){
      color=(color<<8)|(color<<16);
    } else {
      color=color<<8;
    }
    myLamp.setLeds(myLamp.getPixelNumber(i,ypos), color);
  }

  samp_freq = samp_freq; last_min_peak=last_min_peak; last_freq=last_freq; // давим варнинги
  delete mw;
}
#endif

// ------------------------------ ЭФФЕКТ МЕРЦАНИЕ ----------------------
// (c) SottNick

#define TWINKLES_SPEEDS 4     // всего 4 варианта скоростей мерцания
#define TWINKLES_MULTIPLIER 24 // слишком медленно, если на самой медленной просто по единичке добавлять

void twinklesRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  uint scale = TWINKLES_MULTIPLIER*myLamp.effects.getSpeed()/255.0;
  TProgmemRGBPalette16 const *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  TProgmemRGBPalette16 const *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];
  uint8_t tnum = 32-myLamp.effects.getScale() % 32U;

  String var = myLamp.effects.getCurrent()->getValue(myLamp.effects.getCurrent()->param, F("R"));
  if(!var.isEmpty()){
    curPalette = palette_arr[(int)((float)var.toInt()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))]; // выбираем из доп. регулятора
    tnum = 50-49*(myLamp.effects.getScale()/255.0);
  }  

    if (myLamp.isLoading())
    {
      GSHMEM.thue = 0U;
      for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
        if (random8(tnum) == 0){
          GSHMEM.ledsbuff[idx].r = random8();                          // оттенок пикселя
          GSHMEM.ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS * 2 +1); // скорость и направление (нарастает 1-4 или угасает 5-8)
          GSHMEM.ledsbuff[idx].b = random8();                          // яркость
        }
        else
          GSHMEM.ledsbuff[idx] = 0;                                    // всё выкл
      }
    }
    for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
      if (GSHMEM.ledsbuff[idx].b == 0){
        if (random8(tnum) == 0 && GSHMEM.thue > 0){  // если пиксель ещё не горит, зажигаем каждый ХЗй
          GSHMEM.ledsbuff[idx].r = random8();                          // оттенок пикселя
          GSHMEM.ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS +1);     // скорость и направление (нарастает 1-4, но не угасает 5-8)
          GSHMEM.ledsbuff[idx].b = GSHMEM.ledsbuff[idx].g;                    // яркость
          GSHMEM.thue--; // уменьшаем количество погасших пикселей
        }
      }
      else if (GSHMEM.ledsbuff[idx].g <= TWINKLES_SPEEDS){             // если нарастание яркости
        if (GSHMEM.ledsbuff[idx].b > 255U - GSHMEM.ledsbuff[idx].g - scale){            // если досигнут максимум
          GSHMEM.ledsbuff[idx].b = 255U;
          GSHMEM.ledsbuff[idx].g = GSHMEM.ledsbuff[idx].g + TWINKLES_SPEEDS;
        }
        else
          GSHMEM.ledsbuff[idx].b = GSHMEM.ledsbuff[idx].b + GSHMEM.ledsbuff[idx].g + scale;
      }
      else {                                                    // если угасание яркости
        if (GSHMEM.ledsbuff[idx].b <= GSHMEM.ledsbuff[idx].g - TWINKLES_SPEEDS + scale){// если досигнут минимум
          GSHMEM.ledsbuff[idx].b = 0;                                  // всё выкл
          GSHMEM.thue++; // считаем количество погасших пикселей
        }
        else
          GSHMEM.ledsbuff[idx].b = GSHMEM.ledsbuff[idx].b - GSHMEM.ledsbuff[idx].g + TWINKLES_SPEEDS - scale;
      }
      if (GSHMEM.ledsbuff[idx].b == 0)
        myLamp.setLeds(idx, 0U);
      else
        myLamp.setLeds(idx, ColorFromPalette(*curPalette, GSHMEM.ledsbuff[idx].r, GSHMEM.ledsbuff[idx].b));
    }
}

// ============= RADAR / РАДАР ===============
// Aurora : https://github.com/pixelmatix/aurora/blob/master/PatternRadar.h
// Copyright(c) 2014 Jason Coon
// v1.0 - Updating for GuverLamp v1.7 by Palpalych 14.04.2020
// v1.1 - +dither, +smoothing

void radarRoutine(CRGB *leds, const char *param)
{
  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

  myLamp.blur2d(beatsin8(5U, 3U, 10U));
  myLamp.dimAll(255U - (0 + (31-(myLamp.effects.getScale()%32))*3));

  for (uint8_t offset = 0U; offset < WIDTH / 2U - 1U; offset++)
  {
    myLamp.setLeds(myLamp.getPixelNumber(mapcos8(GSHMEM.eff_theta, offset, (WIDTH - 1U)-offset),
                   mapsin8(GSHMEM.eff_theta, offset, (WIDTH - 1U)-offset)),
                   ColorFromPalette(*curPalette, 255U - (offset * 16U + GSHMEM.eff_offset)));
    
    EVERY_N_MILLIS(24)
    {
      GSHMEM.eff_theta += 5.5*((myLamp.effects.getSpeed())/255.0)+1;
      GSHMEM.eff_offset += 3.5*((255-myLamp.effects.getSpeed())/255.0)+1;
    }
  }
}

// ============= WAVES /  ВОЛНЫ ===============
// Prismata Loading Animation
// v1.0 - Updating for GuverLamp v1.7 by SottNick 11.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternWave.h
// Адаптация от (c) SottNick
void wavesRoutine(CRGB *leds, const char *param)
{
  const uint8_t waveRotation = (31-(myLamp.effects.getScale()%32))/8;
  const uint8_t waveCount = myLamp.effects.getSpeed() % 2;
  const uint8_t waveScale = 256 / WIDTH;

  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];


  myLamp.blur2d(20); // @Palpalych советует делать размытие. вот в этом эффекте его явно не хватает...
  myLamp.dimAll(254);

  int n = 0;
  switch (waveRotation)
  {
  case 0:
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      n = quadwave8(x * 2 + GSHMEM.waveTheta) / waveScale;
      myLamp.drawPixelXY(x, n, ColorFromPalette(*curPalette, GSHMEM.whue + x));
      if (waveCount != 1)
        myLamp.drawPixelXY(x, HEIGHT - 1 - n, ColorFromPalette(*curPalette, GSHMEM.whue + x));
    }
    break;

  case 1:
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
      n = quadwave8(y * 2 + GSHMEM.waveTheta) / waveScale;
      myLamp.drawPixelXY(n, y, ColorFromPalette(*curPalette, GSHMEM.whue + y));
      if (waveCount != 1)
        myLamp.drawPixelXY(WIDTH - 1 - n, y, ColorFromPalette(*curPalette, GSHMEM.whue + y));
    }
    break;

  case 2:
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      n = quadwave8(x * 2 - GSHMEM.waveTheta) / waveScale;
      myLamp.drawPixelXY(x, n, ColorFromPalette(*curPalette, GSHMEM.whue + x));
      if (waveCount != 1)
        myLamp.drawPixelXY(x, HEIGHT - 1 - n, ColorFromPalette(*curPalette, GSHMEM.whue + x));
    }
    break;

  case 3:
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
      n = quadwave8(y * 2 - GSHMEM.waveTheta) / waveScale;
      myLamp.drawPixelXY(n, y, ColorFromPalette(*curPalette, GSHMEM.whue + y));
      if (waveCount != 1)
        myLamp.drawPixelXY(WIDTH - 1 - n, y, ColorFromPalette(*curPalette, GSHMEM.whue + y));
    }
    break;
  }

  GSHMEM.waveTheta+=5*(myLamp.effects.getSpeed()/255.0)+1.0;
  GSHMEM.whue+=myLamp.effects.getSpeed()/10.0+1;
}

// ============= FIRE 2012 /  ОГОНЬ 2012 ===============
// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
uint8_t wrapX(int8_t x)
{
  return (x + WIDTH) % WIDTH;
}
uint8_t wrapY(int8_t y)
{
  return (y + HEIGHT) % HEIGHT;
}

void fire2012Routine(CRGB *leds, const char *param)
{
const TProgmemRGBPalette16 *firePalettes[] = {
    &HeatColors2_p,
    &WoodFireColors_p,
    &NormalFire_p,
    &NormalFire2_p,
    &LithiumFireColors_p,
    &SodiumFireColors_p,
    &CopperFireColors_p,
    &AlcoholFireColors_p,
    &RubidiumFireColors_p,
    &PotassiumFireColors_p};

  const TProgmemRGBPalette16 *curPalette = firePalettes[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-1))];
  
#if HEIGHT / 6 > 6
#define FIRE_BASE 6
#else
#define FIRE_BASE HEIGHT / 6 + 1
#endif
  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
  uint8_t cooling = 70;
  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
  uint8_t sparking = 130;
  // SMOOTHING; How much blending should be done between frames
  // Lower = more blending and smoother flames. Higher = less blending and flickery flames
  const uint8_t fireSmoothing = 80*2.0*myLamp.effects.getSpeed()/255.0+10;
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(256));

  // Loop for each column individually
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    // Step 1.  Cool down every cell a little
    for (uint8_t i = 0; i < HEIGHT; i++)
    {
      GSHMEM.noise3d[0][x][i] = qsub8(GSHMEM.noise3d[0][x][i], random(0, ((cooling * 10) / HEIGHT) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint8_t k = HEIGHT; k > 1; k--)
    {
      GSHMEM.noise3d[0][x][wrapY(k)] = (GSHMEM.noise3d[0][x][k - 1] + GSHMEM.noise3d[0][x][wrapY(k - 2)] + GSHMEM.noise3d[0][x][wrapY(k - 2)]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random(255) < sparking)
    {
      int j = random(FIRE_BASE);
      GSHMEM.noise3d[0][x][j] = qadd8(GSHMEM.noise3d[0][x][j], random(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
      // Blend new data with previous frame. Average data between neighbouring pixels
      if(curPalette!=firePalettes[0]){
        nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, y)], ColorFromPalette(HeatColors2_p, ((GSHMEM.noise3d[0][x][y] * 0.7) + (GSHMEM.noise3d[0][wrapX(x + 1)][y] * 0.3))), fireSmoothing);
      }
      nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, y)], ColorFromPalette(*curPalette, ((GSHMEM.noise3d[0][x][y] * 0.7) + (GSHMEM.noise3d[0][wrapX(x + 1)][y] * 0.3))), fireSmoothing);
    }
  }
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

void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm, bool fixRC = false)
{
  // static uint16_t GSHMEM.noiseX = random16();
  // static uint16_t GSHMEM.noiseY = random16();
  // static uint16_t GSHMEM.noiseZ = random16();
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
      if (GSHMEM.noise3d[0][x][i] >= backgroundDepth)
      { // Don't move empty cells
        if (i > 0)
          GSHMEM.noise3d[0][x][wrapY(i - 1)] = GSHMEM.noise3d[0][x][i];
        GSHMEM.noise3d[0][x][i] = 0;
      }
    }

    // Step 2.  Randomly spawn new dots at top
    if (random(255) < spawnFreq)
    {
      GSHMEM.noise3d[0][x][HEIGHT - 1] = random(backgroundDepth, maxBrightness);
    }

    // Step 3. Map from tempMatrix cells to LED colors
    //uint32_t color = CRGB::Black;
    for (int16_t y = HEIGHT-1; y >= 0; y--)
    {
      // if(color == CRGB::Black && myLamp.getPixColor(myLamp.getPixelNumber(x, y)) && y!=(HEIGHT-1))
      //   color = myLamp.getPixColor(myLamp.getPixelNumber(x, y));
      // else if(!myLamp.getPixColor(myLamp.getPixelNumber(x, y)) && y!=(HEIGHT-1))
      //    color = CRGB::Black;

      if (GSHMEM.noise3d[0][x][y] >= backgroundDepth)
      { // Don't write out empty cells
        // if(fixRC && color!=CRGB::Black){
        //   myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
        // }
        // else if(fixRC && y==(HEIGHT-1) && color==CRGB::Black)
        //   myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(rain_p, GSHMEM.noise3d[0][x][y]));
        // else if(!fixRC)
          myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(rain_p, GSHMEM.noise3d[0][x][y]));
      }
    }
    //color = CRGB::Black;

    // Step 4. Add splash if called for
    if (splashes)
    {
      // FIXME, this is broken
      byte j = GSHMEM.nline[x];
      byte v = GSHMEM.noise3d[0][x][0];

      if (j >= backgroundDepth)
      {
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x - 2), 0), ColorFromPalette(rain_p, j / 3));
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x + 2), 0), ColorFromPalette(rain_p, j / 3));
        GSHMEM.nline[x] = 0; // Reset splash
      }

      if (v >= backgroundDepth)
      {
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x - 1), 1), ColorFromPalette(rain_p, v / 2));
        myLamp.setLeds(myLamp.getPixelNumber(wrapX(x + 1), 1), ColorFromPalette(rain_p, v / 2));
        GSHMEM.nline[x] = v; // Prep splash for next frame
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
      int xoffset = noiseScale * x + GSHMEM.rhue;

      for (int z = 0; z < cloudHeight; z++)
      {
        int yoffset = noiseScale * z - GSHMEM.rhue;
        uint8_t dataSmoothing = 192;
        uint8_t noiseData = qsub8(inoise8(GSHMEM.noiseX + xoffset, GSHMEM.noiseY + yoffset, GSHMEM.noiseZ), 16);
        noiseData = qadd8(noiseData, scale8(noiseData, 39));
        noise[x * cloudHeight + z] = scale8(noise[x * cloudHeight + z], dataSmoothing) + scale8(noiseData, 256 - dataSmoothing);
        nblend(myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, HEIGHT - z - 1)], ColorFromPalette(rainClouds_p, noise[x * cloudHeight + z]), (cloudHeight - z) * (250 / cloudHeight));
      }
      GSHMEM.noiseZ++;
    }
  }
}

uint8_t myScale8(uint8_t x)
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

void coloredRainRoutine(CRGB *leds, const char *param) // внимание! этот эффект заточен на работу бегунка Масштаб в диапазоне от 0 до 255. пока что единственный.
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  CRGB solidRainColor = CRGB(60, 80, 90);
  CRGB randomRainColor = CHSV(random(1,255), 255U, 255U);
  // я хз, как прикрутить а 1 регулятор и длину хвостов и цвет капель
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,5,100), 195, CRGB::Green, false, false, false); // было CRGB::Green
  uint8_t Scale = myLamp.effects.getScale();

  if (Scale > 255U-8U)
    rain(60, 200, map8(42, 5, 100), (31*(Scale%8)), solidRainColor, false, false, false);  
  else if (Scale > 255U-16U)
    rain(60, 200, map8(42, 5, 100), (31*(Scale%8)), randomRainColor, false, false, false, true);
  else
    rain(60, 200, map8(42, 5, 100), (31*(Scale%8)), CHSV(Scale, 255U, 255U), false, false, false);
}

void simpleRainRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }  
  
  CRGB solidRainColor = CRGB(60, 80, 90);
  uint8_t Scale = myLamp.effects.getScale();
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,2,60), 10, solidRainColor, true, true, false);
  rain(60, 180, Scale, 30, solidRainColor, true, true, false);
}

void stormyRainRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  CRGB solidRainColor = CRGB(60, 80, 90);
  uint8_t Scale = myLamp.effects.getScale();  
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(0, 90, map8(intensity,0,150)+60, 10, solidRainColor, true, true, true);
  rain(60, 160, Scale, 30, solidRainColor, true, true, true);
}

// ============= FIRE 2018 /  ОГОНЬ 201 ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
// https://gist.github.com/StefanPetrick/819e873492f344ebebac5bcd2fdd8aa8
// https://gist.github.com/StefanPetrick/1ba4584e534ba99ca259c1103754e4c5

void fire2018Routine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }  

  uint8_t Scale = myLamp.effects.getScale()/2.3;
  //uint8_t Speed = myLamp.effects.getSpeed();

  const uint8_t CentreY = HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t CentreX = WIDTH / 2 + (WIDTH % 2);

  // some changing values
  uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
  uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
  uint16_t ctrl = ((ctrl1 + ctrl2) / 2);

  // parameters for the heatmap
  uint16_t speed = 25;
  GSHMEM.noise32_x[0] = 3 * ctrl * speed;
  GSHMEM.noise32_y[0] = 20 * millis() * speed;
  GSHMEM.noise32_z[0] = 5 * millis() * speed;
  GSHMEM.scale32_x[0] = ctrl1 / 2;
  GSHMEM.scale32_y[0] = ctrl2 / 2;

  //calculate the noise data
  uint8_t layer = 0;

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    uint32_t ioffset = GSHMEM.scale32_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint32_t joffset = GSHMEM.scale32_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(GSHMEM.noise32_x[layer] + ioffset, GSHMEM.noise32_y[layer] + joffset, GSHMEM.noise32_z[layer])) + 1);
      GSHMEM.noise3dx[layer][i][j] = data >> 8;
    }
  }

  // parameters for te brightness mask
  speed = 20;
  GSHMEM.noise32_x[1] = 3 * ctrl * speed;
  GSHMEM.noise32_y[1] = 20 * millis() * speed;
  GSHMEM.noise32_z[1] = 5 * millis() * speed;
  GSHMEM.scale32_x[1] = ctrl1 / 2;
  GSHMEM.scale32_y[1] = ctrl2 / 2;

  //calculate the noise data
  layer = 1;
  for (uint8_t i = 0; i < WIDTH; i++)
  {
    uint32_t ioffset = GSHMEM.scale32_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint32_t joffset = GSHMEM.scale32_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(GSHMEM.noise32_x[layer] + ioffset, GSHMEM.noise32_y[layer] + joffset, GSHMEM.noise32_z[layer])) + 1);
      GSHMEM.noise3dx[layer][i][j] = data >> 8;
    }
  }

  // draw lowest line - seed the fire
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    GSHMEM.fire18heat[myLamp.getPixelNumber(x, HEIGHT - 1)] = GSHMEM.noise3dx[0][WIDTH - 1 - x][CentreY - 1]; // хз, почему взято с середины. вожможно, нужно просто с 7 строки вне зависимости от высоты матрицы
  }

  //copy everything one line up
  for (uint8_t y = 0; y < HEIGHT - 1; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      GSHMEM.fire18heat[myLamp.getPixelNumber(x, y)] = GSHMEM.fire18heat[myLamp.getPixelNumber(x, y + 1)];
    }
  }

  //dim
  for (uint8_t y = 0; y < HEIGHT - 1; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      uint8_t dim = GSHMEM.noise3dx[0][x][y];
      // high value = high flames
      dim = dim / 1.7;
      dim = 255 - dim;
      GSHMEM.fire18heat[myLamp.getPixelNumber(x, y)] = scale8(GSHMEM.fire18heat[myLamp.getPixelNumber(x, y)], dim);
    }
  }

  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      // map the colors based on heatmap
      //leds[XY(x, HEIGHT - 1 - y)] = CRGB( fire18heat[XY(x, y)], 1 , 0);
      //leds[XY(x, HEIGHT - 1 - y)] = CRGB( fire18heat[XY(x, y)], fire18heat[XY(x, y)] * 0.153, 0);// * 0.153 - лучший оттенок
      myLamp.setLeds(myLamp.getPixelNumber(x, HEIGHT - 1 - y), CRGB(GSHMEM.fire18heat[myLamp.getPixelNumber(x, y)], (float)GSHMEM.fire18heat[myLamp.getPixelNumber(x, y)] * Scale * 0.01, 0));

      //пытался понять, как регулировать оттенок пламени...
      //  if (modes[currentMode].Scale > 50)
      //    leds[XY(x, HEIGHT - 1 - y)] = CRGB( fire18heat[XY(x, y)], fire18heat[XY(x, y)] * (modes[currentMode].Scale % 50)  * 0.051, 0);
      //  else
      //    leds[XY(x, HEIGHT - 1 - y)] = CRGB( fire18heat[XY(x, y)], 1 , fire18heat[XY(x, y)] * modes[currentMode].Scale * 0.051);
      //примерно понял

      // dim the result based on 2nd noise layer
      myLamp.getUnsafeLedsArray()[myLamp.getPixelNumber(x, HEIGHT - 1 - y)].nscale8(GSHMEM.noise3dx[1][x][y]);
    }
  }
}

// ------------------------------ ЭФФЕКТ КОЛЬЦА / КОДОВЫЙ ЗАМОК ----------------------
// (c) SottNick
// из-за повторного использоваия переменных от других эффектов теперь в этом коде невозможно что-то понять.
// поэтому для понимания придётся сперва заменить названия переменных на человеческие. но всё равно это песец, конечно.

//uint8_t GSHMEM.ringColor[HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
//uint8_t GSHMEM.huePos[HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
//uint8_t GSHMEM.shiftHueDir[HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
////ringHueShift[ringsCount]; // шаг градиета оттенка внутри кольца -8 - +8 случайное число
////ringHueShift2[ringsCount]; // обычная скорость переливания оттенка всего кольца -8 - +8 случайное число
//uint8_t GSHMEM.currentRing; // кольцо, которое в настоящий момент нужно провернуть
//uint8_t GSHMEM.stepCount; // оставшееся количество шагов, на которое нужно провернуть активное кольцо - случайное от WIDTH/5 до WIDTH-3
void ringsRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }  

    uint8_t h, x, y;
    uint8_t ringWidth; // максимальне количество пикселей в кольце (толщина кольца) от 1 до height / 2 + 1
    uint8_t ringNb; // количество колец от 2 до height
    uint8_t downRingHue, upRingHue; // количество пикселей в нижнем (downRingHue) и верхнем (upRingHue) кольцах

    const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
    const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];

    ringWidth = myLamp.effects.getScale()%11U + 1U; // толщина кольца от 1 до 11 для каждой из палитр
    ringNb = (float)HEIGHT / ringWidth + ((HEIGHT % ringWidth == 0U) ? 0U : 1U)%HEIGHT; // количество колец
    upRingHue = ringWidth - (ringWidth * ringNb - HEIGHT) / 2U; // толщина верхнего кольца. может быть меньше нижнего
    downRingHue = HEIGHT - upRingHue - (ringNb - 2U) * ringWidth; // толщина нижнего кольца = всё оставшееся
	
    if (myLamp.isLoading())
    {
      for (uint8_t i = 0; i < ringNb; i++)
      {
        GSHMEM.ringColor[i] = random8(255U - WIDTH / 2U); // начальный оттенок кольца из палитры 0-255 за минусом длины кольца, делённой пополам
        GSHMEM.shiftHueDir[i] = random8();
        GSHMEM.huePos[i] = 0U; //random8(WIDTH); само прокрутится постепенно
        GSHMEM.stepCount = 0U;
        //do { // песец конструкцию придумал бредовую
        //  GSHMEM.stepCount = WIDTH - 3U - random8((WIDTH - 3U) * 2U); само присвоится при первом цикле
        //} while (GSHMEM.stepCount < WIDTH / 5U || GSHMEM.stepCount > 255U - WIDTH / 5U);
        GSHMEM.currentRing = random8(ringNb);
      }
    }
    for (uint8_t i = 0; i < ringNb; i++)
    {
      if (i != GSHMEM.currentRing) // если это не активное кольцо
        {
          h = GSHMEM.shiftHueDir[i] & 0x0F; // сдвигаем оттенок внутри кольца
          if (h > 8U)
            //GSHMEM.ringColor[i] += (uint8_t)(7U - h); // с такой скоростью сдвиг оттенка от вращения кольца не отличается
            GSHMEM.ringColor[i]--;
          else
            //GSHMEM.ringColor[i] += h;
            GSHMEM.ringColor[i]++;
        }
      else
        {
          if (GSHMEM.stepCount == 0) // если сдвиг активного кольца завершён, выбираем следующее
            {
              GSHMEM.currentRing = random8(ringNb);
              do {
                GSHMEM.stepCount = WIDTH - 3U - random8((WIDTH - 3U) * 2U); // проворот кольца от хз до хз
              } while (GSHMEM.stepCount < WIDTH / 5U || GSHMEM.stepCount > 255U - WIDTH / 5U);
            }
          else
            {
              if (GSHMEM.stepCount > 127U)
                {
                  GSHMEM.stepCount++;
                  GSHMEM.huePos[i] = (GSHMEM.huePos[i] + 1U) % WIDTH;
                }
              else
                {
                  GSHMEM.stepCount--;
                  GSHMEM.huePos[i] = (GSHMEM.huePos[i] - 1U + WIDTH) % WIDTH;
                }
            }
        }
        // отрисовываем кольца
        h = (GSHMEM.shiftHueDir[i] >> 4) & 0x0F; // берём шаг для градиента вутри кольца
        if (h > 8U)
          h = 7U - h;
        for (uint8_t j = 0U; j < ((i == 0U) ? downRingHue : ((i == ringNb - 1U) ? upRingHue : ringWidth)); j++) // от 0 до (толщина кольца - 1)
        {
          y = i * ringWidth + j - ((i == 0U) ? 0U : ringWidth - downRingHue);
          for (uint8_t k = 0; k < WIDTH / 2U; k++) // полукольцо
            {
              x = (GSHMEM.huePos[i] + k) % WIDTH; // первая половина кольца
              myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, GSHMEM.ringColor[i] + k * h));
              x = (WIDTH - 1 + GSHMEM.huePos[i] - k) % WIDTH; // вторая половина кольца (зеркальная первой)
              myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, GSHMEM.ringColor[i] + k * h));
            }
          if (WIDTH & 0x01) //(WIDTH % 2U > 0U) // если число пикселей по ширине матрицы нечётное, тогда не забываем и про среднее значение
          {
            x = (GSHMEM.huePos[i] + WIDTH / 2U) % WIDTH;
            myLamp.setLeds(myLamp.getPixelNumber(x, y), ColorFromPalette(*curPalette, GSHMEM.ringColor[i] + WIDTH / 2U * h));
          }
        }
    }
}

// ------------------------------ ЭФФЕКТ КУБИК 2D ----------------------
// (c) SottNick

#define PAUSE_MAX 7

//uint8_t GSHMEM.storage[WIDTH][HEIGHT];
//uint8_t GSHMEM.pauseSteps; // осталось шагов паузы
//uint8_t GSHMEM.currentStep; // текущий шаг сдвига (от 0 до GSHMEM.shiftSteps-1)
//uint8_t GSHMEM.shiftSteps; // всего шагов сдвига (от 3 до 4)
//uint8_t GSHMEM.gX, GSHMEM.gY; // глобальный X и глобальный Y нашего "кубика"
// int8_t GSHMEM.globalShiftX, GSHMEM.globalShiftY; // нужно ли сдвинуть всё поле по окончаии цикла и в каком из направлений (-1, 0, +1)
// bool GSHMEM.direction; // направление вращения в данный момент

void cube2dRoutine(CRGB *leds, const char *param)
{
  uint8_t x, y;
	uint8_t sizeX, sizeY; // размеры ячеек по горизонтали / вертикали
  uint8_t anim0; // будем считать тут начальный пиксель для анимации сдвига строки/колонки
  int8_t shift, shiftAll; // какое-то расчётное направление сдвига (-1, 0, +1)
	uint8_t cntX, cntY; // количество ячеек по горизонтали / вертикали
	uint8_t fieldX, fieldY; // размер всего поля по горизонтали / вертикали (в том числе 1 дополнительная пустая дорожка-разделитель с какой-то из сторон)
  CRGB color, color2;
	bool seamlessX; // получилось ли сделать поле по Х бесшовным

  const TProgmemRGBPalette16 *palette_arr[] = {&PartyColors_p, &OceanColors_p, &LavaColors_p, &HeatColors_p, &WaterfallColors_p, &CloudColors_p, &ForestColors_p, &RainbowColors_p, &RainbowStripeColors_p};
  const TProgmemRGBPalette16 *curPalette = palette_arr[(int)((float)myLamp.effects.getScale()/255.1*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1))];


  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }  

	sizeY = (myLamp.effects.getScale() - 1U) % 11U + 1U; // размер ячейки от 1 до 11 пикселей для каждой из 9 палитр
	sizeX = (myLamp.effects.getScale() - 1U) % 11U + 1U; // размер ячейки от 1 до 11 пикселей для каждой из 9 палитр
	if (myLamp.effects.getSpeed() & 0x01) // по идее, ячейки не обязательно должны быть квадратными, поэтому можно тут поизвращаться
		sizeY = (sizeY << 1U) + 1U;
    cntY = HEIGHT / (sizeY + 1U);
	if (cntY < 2U)
		cntY = 2U;
	y = HEIGHT / cntY - 1U;
	if (sizeY > y)
		sizeY = y;
	fieldY = (sizeY + 1U) * cntY;
	cntX = WIDTH / (sizeX + 1U);
	if (cntX < 2U)
		cntX = 2U;
	x = WIDTH / cntX - 1U;
	if (sizeX > x)
		sizeX = x;
	fieldX = (sizeX + 1U) * cntX;
	seamlessX = (fieldX == WIDTH);
	  
    if (myLamp.isLoading())
    {
      FastLED.clear();

      GSHMEM.gX = 0U;
      GSHMEM.gY = 0U;
      GSHMEM.globalShiftX = 0;
      GSHMEM.globalShiftY = 0;

      for (uint8_t j = 0U; j < cntY; j++)
      {
        y = j * (sizeY + 1U); // + GSHMEM.gY т.к. оно =0U
        for (uint8_t i = 0U; i < cntX; i++)
        {
          x = i * (sizeX + 1U); // + GSHMEM.gX т.к. оно =0U
          if (myLamp.effects.getScale() == 255U)
            color = CHSV(45U, 0U, 128U + random8(128U));
          else 
            color = ColorFromPalette(*curPalette, random8());
          for (uint8_t k = 0U; k < sizeY; k++)
            for (uint8_t m = 0U; m < sizeX; m++)
              myLamp.setLeds(myLamp.getPixelNumber(x+m, y+k), color);
        }
      }
      GSHMEM.currentStep = 4U; // текущий шаг сдвига первоначально с перебором (от 0 до GSHMEM.shiftSteps-1)
      GSHMEM.shiftSteps = 4U; // всего шагов сдвига (от 3 до 4)
      GSHMEM.pauseSteps = 0U; // осталось шагов паузы
    }

  //двигаем, что получилось...
  if (GSHMEM.pauseSteps == 0 && GSHMEM.currentStep < GSHMEM.shiftSteps) // если пауза закончилась, а цикл вращения ещё не завершён
  {
    GSHMEM.currentStep++;
    if (GSHMEM.direction)
    {
      for (uint8_t i = 0U; i < cntX; i++)
      {
        x = (GSHMEM.gX + i * (sizeX + 1U)) % WIDTH;
        if (GSHMEM.storage[i][0] > 0) // в нулевой ячейке храним оставшееся количество ходов прокрутки
        {
          GSHMEM.storage[i][0]--;
          shift = GSHMEM.storage[i][1] - 1; // в первой ячейке храним направление прокрутки

          if (GSHMEM.globalShiftY == 0)
            anim0 = (GSHMEM.gY == 0U) ? 0U : GSHMEM.gY - 1U;
          else if (GSHMEM.globalShiftY > 0)
            anim0 = GSHMEM.gY;
          else
            anim0 = GSHMEM.gY - 1U;
         
          if (shift < 0) // если крутим столбец вниз
          {
            color = myLamp.getPixColorXY(x, anim0);                                   // берём цвет от нижней строчки
            for (uint8_t k = anim0; k < anim0+fieldY-1; k++)
            {
              color2 = myLamp.getPixColorXY(x,k+1);                                   // берём цвет от строчки над нашей
              for (uint8_t m = x; m < x + sizeX; m++)
                myLamp.setLeds(myLamp.getPixelNumber(m % WIDTH,k), color2);                           // копируем его на всю нашу строку
            }
            for   (uint8_t m = x; m < x + sizeX; m++)
              myLamp.setLeds(myLamp.getPixelNumber(m % WIDTH,anim0+fieldY-1), color);                  // цвет нижней строчки копируем на всю верхнюю
          }
          else if (shift > 0) // если крутим столбец вверх
          {
            color = myLamp.getPixColorXY(x,anim0+fieldY-1);                            // берём цвет от верхней строчки
            for (uint8_t k = anim0+fieldY-1; k > anim0 ; k--)
            {
              color2 = myLamp.getPixColorXY(x,k-1);                                   // берём цвет от строчки под нашей
              for (uint8_t m = x; m < x + sizeX; m++)
                myLamp.setLeds(myLamp.getPixelNumber(m % WIDTH,k), color2);                           // копируем его на всю нашу строку
            }
            for   (uint8_t m = x; m < x + sizeX; m++)
              myLamp.setLeds(myLamp.getPixelNumber(m % WIDTH, anim0), color);                         // цвет верхней строчки копируем на всю нижнюю
          }
        }
      }
    }
    else
    {
      for (uint8_t j = 0U; j < cntY; j++)
      {
        y = GSHMEM.gY + j * (sizeY + 1U);
        if (GSHMEM.storage[0][j] > 0) // в нулевой ячейке храним оставшееся количество ходов прокрутки
        {
          GSHMEM.storage[0][j]--;
          shift = GSHMEM.storage[1][j] - 1; // в первой ячейке храним направление прокрутки
     
          if (seamlessX)
            anim0 = 0U;
          else if (GSHMEM.globalShiftX == 0)
            anim0 = (GSHMEM.gX == 0U) ? 0U : GSHMEM.gX - 1U;
          else if (GSHMEM.globalShiftX > 0)
            anim0 = GSHMEM.gX;
          else
            anim0 = GSHMEM.gX - 1U;
         
          if (shift < 0) // если крутим строку влево
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
          }
          else if (shift > 0) // если крутим столбец вверх
          {
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
        }
      }
    }
  
  }
  else if (GSHMEM.pauseSteps != 0U) // пропускаем кадры после прокрутки кубика (делаем паузу)
    GSHMEM.pauseSteps--;

  if (GSHMEM.currentStep >= GSHMEM.shiftSteps) // если цикл вращения завершён, меняем местами соотвествующие ячейки (цвет в них) и точку первой ячейки
    {
      GSHMEM.currentStep = 0U;
      GSHMEM.pauseSteps = PAUSE_MAX;
      //если часть ячеек двигалась на 1 пиксель, пододвигаем глобальные координаты начала
      GSHMEM.gY = GSHMEM.gY + GSHMEM.globalShiftY; //+= GSHMEM.globalShiftY;
      GSHMEM.globalShiftY = 0;
      //GSHMEM.gX += GSHMEM.globalShiftX; для бесшовной не годится
      GSHMEM.gX = (WIDTH + GSHMEM.gX + GSHMEM.globalShiftX) % WIDTH;
      GSHMEM.globalShiftX = 0;

      //пришла пора выбрать следующие параметры вращения
      shiftAll = 0;
      GSHMEM.direction = random8(2U);
      if (GSHMEM.direction) // идём по горизонтали, крутим по вертикали (столбцы двигаются)
      {
        for (uint8_t i = 0U; i < cntX; i++)
        {
          GSHMEM.storage[i][1] = random8(3);
          shift = GSHMEM.storage[i][1] - 1; // в первой ячейке храним направление прокрутки
          if (shiftAll == 0)
            shiftAll = shift;
          else if (shift != 0 && shiftAll != shift)
            shiftAll = 50;
        }
        GSHMEM.shiftSteps = sizeY + ((GSHMEM.gY - shiftAll >= 0 && GSHMEM.gY - shiftAll + fieldY < (int)HEIGHT) ? random8(2U) : 1U);

/*        if (shiftAll == 0) // пытался сделать, чтобы при совпадении "весь кубик стоит" сдвинуть его весь на пиксель, но заколебался
        {
          GSHMEM.shiftSteps = sizeY;
          shiftAll = (random8(2)) ? 1 : -1;
          if (GSHMEM.gY - shiftAll < 0 || GSHMEM.gY - shiftAll + fieldY >= (int)HEIGHT)
            shiftAll = 0 - shiftAll;
        }
*/
        if (GSHMEM.shiftSteps == sizeY) // значит полюбому shiftAll было = (-1, 0, +1) - и для нуля в том числе мы двигаем весь куб на 1 пиксель
        {
          GSHMEM.globalShiftY = 1 - shiftAll; //временно на единичку больше, чем надо
          for (uint8_t i = 0U; i < cntX; i++)
            if (GSHMEM.storage[i][1] == 1U) // если ячейка никуда не планировала двигаться
            {
              GSHMEM.storage[i][1] = GSHMEM.globalShiftY;
              GSHMEM.storage[i][0] = 1U; // в нулевой ячейке храним количество ходов сдвига
            }
            else
              GSHMEM.storage[i][0] = GSHMEM.shiftSteps; // в нулевой ячейке храним количество ходов сдвига
          GSHMEM.globalShiftY--;
        }
        else
          for (uint8_t i = 0U; i < cntX; i++)
            if (GSHMEM.storage[i][1] != 1U)
              GSHMEM.storage[i][0] = GSHMEM.shiftSteps; // в нулевой ячейке храним количество ходов сдвига
      }
      else // идём по вертикали, крутим по горизонтали (строки двигаются)
      {
        for (uint8_t j = 0U; j < cntY; j++)
        {
          GSHMEM.storage[1][j] = random8(3);
          shift = GSHMEM.storage[1][j] - 1; // в первой ячейке храним направление прокрутки
          if (shiftAll == 0)
            shiftAll = shift;
          else if (shift != 0 && shiftAll != shift)
            shiftAll = 50;
        }
        if (seamlessX)
          GSHMEM.shiftSteps = sizeX + ((shiftAll < 50) ? random8(2U) : 1U);
        else 
          GSHMEM.shiftSteps = sizeX + ((GSHMEM.gX - shiftAll >= 0 && GSHMEM.gX - shiftAll + fieldX < (int)WIDTH) ? random8(2U) : 1U);
       
/*        if (shiftAll == 0) // пытался сделать, чтобы при совпадении "весь кубик стоит" сдвинуть его весь на пиксель, но заколебался
        {
          GSHMEM.shiftSteps = sizeX;
          shiftAll = (random8(2)) ? 1 : -1;
          if (GSHMEM.gX - shiftAll < 0 || GSHMEM.gX - shiftAll + fieldX >= (int)WIDTH)
            shiftAll = 0 - shiftAll;
        }
*/         
        if (GSHMEM.shiftSteps == sizeX) // значит полюбому shiftAll было = (-1, 0, +1) - и для нуля в том числе мы двигаем весь куб на 1 пиксель
        {
          GSHMEM.globalShiftX = 1 - shiftAll; //временно на единичку больше, чем надо
          for (uint8_t j = 0U; j < cntY; j++)
            if (GSHMEM.storage[1][j] == 1U) // если ячейка никуда не планировала двигаться
            {
              GSHMEM.storage[1][j] = GSHMEM.globalShiftX;
              GSHMEM.storage[0][j] = 1U; // в нулевой ячейке храним количество ходов сдвига
            }
            else
              GSHMEM.storage[0][j] = GSHMEM.shiftSteps; // в нулевой ячейке храним количество ходов сдвига
          GSHMEM.globalShiftX--;
        }
        else
          for (uint8_t j = 0U; j < cntY; j++)
            if (GSHMEM.storage[1][j] != 1U)
              GSHMEM.storage[0][j] = GSHMEM.shiftSteps; // в нулевой ячейке храним количество ходов сдвига
      }
   }
}