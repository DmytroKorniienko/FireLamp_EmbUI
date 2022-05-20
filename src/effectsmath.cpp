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

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

#include "lamp.h"
#include "effectmath.h"
//#include "main.h"
#ifdef MATRIXx4
  #include "matrix4.h"
#endif
extern LAMP myLamp; // Объект лампы

// Общий набор мат. функций и примитивов для обсчета эффектов

namespace EffectMath_PRIVATE {
    MATRIXFLAGS matrixflags;
    CRGB leds[NUM_LEDS]; // основной буфер вывода изображения
    CRGB overrun;
    
    CRGB *getUnsafeLedsArray(){return leds;}

    // ключевая функция с подстройкой под тип матрицы, использует MIRR_V и MIRR_H
    uint32_t getPixelNumber(int16_t x, int16_t y) // получить номер пикселя в ленте по координатам
    {
    #if defined(XY_EXTERN)
        uint16_t i = (y * WIDTH) + x;
        uint16_t j = pgm_read_dword(&XYTable[i]);
        return j;
    #elif defined(MATRIXx4)
      return matrix4_XY(x, y);
    #else
        // хак с макроподстановкой, пусть живет пока
        #define MIRR_H matrixflags.MIRR_H
        #define MIRR_V matrixflags.MIRR_V
        
        if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
        {
            return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + THIS_X);
        }
        else                                                      // если нечётная строка
        {
            return ((uint32_t)THIS_Y * SEGMENTS * _WIDTH + _WIDTH - THIS_X - 1);
        }
    
        #undef MIRR_H
        #undef MIRR_V
    #endif
    }
}

using namespace EffectMath_PRIVATE;

// используется встроенный блер, так что необходимости в данной функции более нет, отключено
uint16_t XY(uint8_t x, uint8_t y) {return 0;}
// // для работы FastLed (blur2d)
// uint16_t XY(uint8_t x, uint8_t y)
// {
// #ifdef ROTATED_MATRIX
//   return getPixelNumber(y,x); // повернутое на 90 градусов
// #else
//   return getPixelNumber(x,y); // обычное подключение
// #endif
// }

//--------------------------------------
// blur1d: one-dimensional blur filter. Spreads light to 2 line neighbors.
// blur2d: two-dimensional blur filter. Spreads light to 8 XY neighbors.
//
//           0 = no spread at all
//          64 = moderate spreading
//         172 = maximum smooth, even spreading
//
//         173..255 = wider spreading, but increasing flicker
//
//         Total light is NOT entirely conserved, so many repeated
//         calls to 'blur' will also result in the light fading,
//         eventually all the way to black; this is by design so that
//         it can be used to (slowly) clear the LEDs to black.
void EffectMath::blur1d( CRGB* leds, uint16_t numLeds, fract8 blur_amount)
{
    uint8_t keep = 255 - blur_amount;
    uint8_t seep = blur_amount >> 1;
    CRGB carryover = CRGB::Black;
    for( uint16_t i = 0; i < numLeds; ++i) {
        CRGB cur = leds[i];
        CRGB part = cur;
        part.nscale8( seep);
        cur.nscale8( keep);
        cur += carryover;
        if( i) leds[i-1] += part;
        leds[i] = cur;
        carryover = part;
    }
}

void EffectMath::blur2d( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
    blurRows(leds, width, height, blur_amount);
    blurColumns(leds, width, height, blur_amount);
}

// blurRows: perform a blur1d on every row of a rectangular matrix
void EffectMath::blurRows( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
/*    for( uint8_t row = 0; row < height; ++row) {
        CRGB* rowbase = leds + (row * width);
        blur1d( rowbase, width, blur_amount);
    }
*/
    // blur rows same as columns, for irregular matrix
    uint8_t keep = 255 - blur_amount;
    uint8_t seep = blur_amount >> 1;
    for( uint8_t row = 0; row < height; row++) {
        CRGB carryover = CRGB::Black;
        for( uint8_t i = 0; i < width; i++) {
            CRGB cur = leds[getPixelNumber(i,row)];
            CRGB part = cur;
            part.nscale8( seep);
            cur.nscale8( keep);
            cur += carryover;
            if( i) leds[getPixelNumber(i-1,row)] += part;
            leds[getPixelNumber(i,row)] = cur;
            carryover = part;
        }
    }
}

// blurColumns: perform a blur1d on each column of a rectangular matrix
void EffectMath::blurColumns(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
    // blur columns
    uint8_t keep = 255 - blur_amount;
    uint8_t seep = blur_amount >> 1;
    for( uint8_t col = 0; col < width; ++col) {
        CRGB carryover = CRGB::Black;
        for( uint8_t i = 0; i < height; ++i) {
            CRGB cur = leds[getPixelNumber(col,i)];
            CRGB part = cur;
            part.nscale8( seep);
            cur.nscale8( keep);
            cur += carryover;
            if( i) leds[getPixelNumber(col,i-1)] += part;
            leds[getPixelNumber(col,i)] = cur;
            carryover = part;
        }
    }
} 

// ******** общие мат. функции переиспользуются в другом эффекте
uint8_t EffectMath::mapsincos8(bool map, uint8_t theta, uint8_t lowest, uint8_t highest) {
  uint8_t beat = map ? sin8(theta) : cos8(theta);
  return lowest + scale8(beat, highest - lowest);
}

void EffectMath::MoveFractionalNoise(bool _scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift) {
  uint8_t zD;
  uint8_t zF;
  CRGB *leds = getUnsafeLedsArray(); // unsafe
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
          PixelA = _scale ? EffectMath::getPixel(zD%WIDTH, a%HEIGHT) : EffectMath::getPixel(a%WIDTH, zD%HEIGHT);

        CRGB PixelB = CRGB::Black ;
        if ((zF >= 0) && (zF < _side_b))
          PixelB = _scale ? EffectMath::getPixel(zF%WIDTH, a%HEIGHT) : EffectMath::getPixel(a%WIDTH, zF%HEIGHT);
        uint16_t x = _scale ? b : a;
        uint16_t y = _scale ? a : b;
        ledsbuff[getPixelNumber(x%WIDTH, y%HEIGHT)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
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
    CRGB &led = EffectMath::getPixel(i,j);
    if (!led) return; // см. приведение к bool для CRGB, это как раз тест на 0
    
    if (led.r >= 30U || led.g >= 30U || led.b >= 30U){
        led.fadeToBlackBy(step);
    }
    else{
        EffectMath::drawPixelXY(i, j, 0U);
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
CRGB EffectMath::makeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

/* kostyamat добавил
 функция уменьшения яркости */
CRGB EffectMath::makeDarker( const CRGB& color, fract8 howMuchDarker )
{
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

/* kostyamat добавил
 функция возвращает рандомное значение float между min и max 
 с шагом 1/1024 */
float EffectMath::randomf(float min, float max)
{
  return fmap(random(1024), 0, 1023, min, max);
}

/* kostyamat добавил
 функция возвращает true, если float
 ~= целое (первая цифра после запятой == 0) */
bool EffectMath::isInteger(float val) {
    float val1;
    val1 = val - (int)val;
    if ((int)(val1 * 10) == 0)
        return true;
    else
        return false;
}

// Функция создает вспышки в разных местах матрицы, параметр 0-255. Чем меньше, тем чаще.
void EffectMath::addGlitter(uint8_t chanceOfGlitter){
  if ( random8() < chanceOfGlitter) leds[random16(NUM_LEDS)] += CRGB::Gray;
}

// Функция создает разноцветные конфетти в разных местах матрицы, параметр 0-255. Чем меньше, тем чаще.
void EffectMath::confetti(byte density) {
    uint16_t idx = random16(NUM_LEDS);
    for (byte i=0; i < NUM_LEDS/256; i++)
      if ( random8() < density)
        if (RGBweight(leds, idx) < 32) leds[idx] = random(32, 16777216);
}

void EffectMath::gammaCorrection()
{ //gamma correction function
  byte r, g, b;
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    r = leds[i].r;
    g = leds[i].g;
    b = leds[i].b;
    leds[i].r = pgm_read_byte(gamma_exp + r);
    leds[i].g = pgm_read_byte(gamma_exp + g);
    leds[i].b = pgm_read_byte(gamma_exp + b);
  }
}

uint32_t EffectMath::getPixColor(uint32_t thisSegm) // функция получения цвета пикселя по его номеру
{
  uint32_t thisPixel = thisSegm * SEGMENTS;
  if (thisPixel < NUM_LEDS ) 
    return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
  else return (((uint32_t)overrun.r << 16) | ((uint32_t)overrun.g << 8 ) | (uint32_t)overrun.b);
}

// Заливает матрицу выбраным цветом
void EffectMath::fillAll(const CRGB &color) 
{
  for (int32_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}

void EffectMath::drawPixelXY(int16_t x, int16_t y, const CRGB &color) // функция отрисовки точки по координатам X Y
{
  getPixel(x,y) = color;
}

void EffectMath::wu_pixel(uint32_t x, uint32_t y, CRGB col) {      //awesome wu_pixel procedure by reddit u/sutaburosu
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t xn = (x >> 8) + (i & 1); uint16_t yn = (y >> 8) + ((i >> 1) & 1);
    CRGB clr = getLed(getPixelNumber(xn, yn));
    clr.r = qadd8(clr.r, (col.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (col.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (col.b * wu[i]) >> 8);

    EffectMath::drawPixelXY(xn, yn, clr);
  }
  #undef WU_WEIGHT
}

CRGB colorsmear(const CRGB &col1, const CRGB &col2, byte l) {
  CRGB temp1 = col1;
  nblend(temp1, col2, l);
  return temp1;
}

void EffectMath::sDrawPixelXYF(float x, float y, const CRGB &color) {
  byte ax = byte(x);
  byte xsh = (x - byte(x)) * 255;
  byte ay = byte(y);
  byte ysh = (y - byte(y)) * 255;
  CRGB colP1 = colorsmear(color, CRGB(0, 0, 0), xsh);
  CRGB col1 = colorsmear(colP1, CRGB(0, 0, 0), ysh);
  CRGB col2 = colorsmear(CRGB(0, 0, 0), color, xsh);
  CRGB col3 = colorsmear(CRGB(0, 0, 0),colP1, ysh);
  CRGB col4 = colorsmear(CRGB(0, 0, 0),col2, ysh);

  getPixel(ax, ay) += col1;
  getPixel(ax+1, ay) += col2;
  getPixel(ax, ay+1) += col3;
  getPixel(ax+1, ay+1) += col4;
}

void EffectMath::sDrawPixelXYF_X(float x, int16_t y, const CRGB &color) {
  byte ax = byte(x);
  byte xsh = (x - byte(x)) * 255;
  CRGB col1 = colorsmear(color, CRGB(0, 0, 0), xsh);
  CRGB col2 = colorsmear(CRGB(0, 0, 0), color, xsh);
  getPixel(ax, y) += col1;
  getPixel(ax + 1, y) += col2;
}

void EffectMath::sDrawPixelXYF_Y(int16_t x, float y, const CRGB &color) {
  byte ay = byte(y);
  byte ysh = (y - byte(y)) * 255;
  CRGB col1 = colorsmear(color, CRGB(0, 0, 0), ysh);
  CRGB col2 = colorsmear(CRGB(0, 0, 0), color, ysh);
  getPixel(x, ay) += col1;
  getPixel(x, ay+1) += col2; 
}

void EffectMath::drawPixelXYF(float x, float y, const CRGB &color, uint8_t darklevel)
{
#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                  WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    // тут нам, ИМХО, незачем гонять через прокладки, и потом сдвигать регистры. А в случае сегмента подразумевается, 
    // что все ЛЕД в одном сегменте одинакового цвета, и достаточно получить цвет любого из них.
    CRGB clr = getPixel(xn, yn); 
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    if (darklevel > 0) getPixel(xn, yn) = EffectMath::makeDarker(clr, darklevel);
    else getPixel(xn, yn) = clr;
  }
  #undef WU_WEIGHT
}

void EffectMath::drawPixelXYF_X(float x, int16_t y, const CRGB &color, uint8_t darklevel)
{
  if (x<-1.0 || y<-1 || x>((float)WIDTH) || y>((float)HEIGHT)) return;

  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, ix = 255 - xx;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {ix, xx};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
    int16_t xn = x + (i & 1);
    CRGB clr = getPixel(xn, y);
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    if (darklevel > 0) getPixel(xn, y) = EffectMath::makeDarker(clr, darklevel);
    else getPixel(xn, y) = clr;
  }
}

void EffectMath::drawPixelXYF_Y(int16_t x, float y, const CRGB &color, uint8_t darklevel)
{
  if (x<-1 || y<-1.0 || x>((float)WIDTH) || y>((float)HEIGHT)) return;

  // extract the fractional parts and derive their inverses
  uint8_t yy = (y - (int)y) * 255, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {iy, yy};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
    int16_t yn = y + (i & 1);
    CRGB clr = getPixel(x, yn);
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    if (darklevel > 0) getPixel(x, yn) = EffectMath::makeDarker(clr, darklevel);
    else getPixel(x, yn) = clr;
  }
}

CRGB EffectMath::getPixColorXYF(float x, float y)
{
  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  CRGB clr=CRGB::Black;
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    if(!i){
      clr = getPixel(xn, yn);
    } else {
      CRGB tmpColor = getPixel(xn, yn);
      clr.r = qadd8(clr.r, (tmpColor.r * wu[i]) >> 8);
      clr.g = qadd8(clr.g, (tmpColor.g * wu[i]) >> 8);
      clr.b = qadd8(clr.b, (tmpColor.b * wu[i]) >> 8);
    }
  }
  return clr;
  #undef WU_WEIGHT
}

CRGB EffectMath::getPixColorXYF_X(float x, int16_t y)
{
  if (x<-1.0 || y<-1.0 || x>((float)WIDTH) || y>((float)HEIGHT)) return CRGB::Black;

  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, ix = 255 - xx;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {ix, xx};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  CRGB clr=CRGB::Black;
  for (int8_t i = 1; i >= 0; i--) {
      int16_t xn = x + (i & 1);
      if(i){
        clr = getPixel(xn, y);
      } else {
        CRGB tmpColor = getPixel(xn, y);
        clr.r = qadd8(clr.r, (tmpColor.r * wu[i]) >> 8);
        clr.g = qadd8(clr.g, (tmpColor.g * wu[i]) >> 8);
        clr.b = qadd8(clr.b, (tmpColor.b * wu[i]) >> 8);
      }
  }
  return clr;
}

CRGB EffectMath::getPixColorXYF_Y(int16_t x, float y)
{
  if (x<-1 || y<-1.0 || x>((float)WIDTH) || y>((float)HEIGHT)) return CRGB::Black;

  // extract the fractional parts and derive their inverses
  uint8_t yy = (y - (int)y) * 255, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {iy, yy};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  CRGB clr=CRGB::Black;
  for (int8_t i = 1; i >= 0; i--) {
      int16_t yn = y + (i & 1);
      if(i){
        clr = getPixel(x, yn);
      } else {
        CRGB tmpColor = getPixel(x, yn);
        clr.r = qadd8(clr.r, (tmpColor.r * wu[i]) >> 8);
        clr.g = qadd8(clr.g, (tmpColor.g * wu[i]) >> 8);
        clr.b = qadd8(clr.b, (tmpColor.b * wu[i]) >> 8);
      }
  }
  return clr;
}

void EffectMath::drawLine(int x1, int y1, int x2, int y2, const CRGB &color){
  int deltaX = abs(x2 - x1);
  int deltaY = abs(y2 - y1);
  int signX = x1 < x2 ? 1 : -1;
  int signY = y1 < y2 ? 1 : -1;
  int error = deltaX - deltaY;

  drawPixelXY(x2, y2, color);
  while (x1 != x2 || y1 != y2) {
      drawPixelXY(x1, y1, color);
      int error2 = error * 2;
      if (error2 > -deltaY) {
          error -= deltaY;
          x1 += signX;
      }
      if (error2 < deltaX) {
          error += deltaX;
          y1 += signY;
      }
  }
}

void EffectMath::drawLineF(float x1, float y1, float x2, float y2, const CRGB &color){
  float deltaX = fabs(x2 - x1);
  float deltaY = fabs(y2 - y1);
  float error = deltaX - deltaY;

  float signX = x1 < x2 ? 0.5 : -0.5;
  float signY = y1 < y2 ? 0.5 : -0.5;

  while (x1 != x2 || y1 != y2) { // (true) - а я то думаю - "почему функция часто вызывает вылет по вачдогу?" А оно вон оно чё, Михалычь!
    if ((signX > 0. && x1 > x2 + signX) || (signX < 0. && x1 < x2 + signX))
      break;
    if ((signY > 0. && y1 > y2 + signY) || (signY < 0. && y1 < y2 + signY))
      break;
    drawPixelXYF(x1, y1, color);
    float error2 = error;
    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
      }
      if (error2 < deltaX) {
          error += deltaX;
          y1 += signY;
      }
  }
}

void EffectMath::drawSquareF(float x, float y, float leg, CRGB color) {
  EffectMath::drawLineF(x+leg,y+leg,x+leg,y-leg,color);
  EffectMath::drawLineF(x+leg,y-leg,x-leg,y-leg,color);
  EffectMath::drawLineF(x-leg,y-leg,x-leg,y+leg,color);
  EffectMath::drawLineF(x-leg,y+leg,x+leg,y+leg,color);
}

void EffectMath::drawCircle(int x0, int y0, int radius, const CRGB &color){
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    EffectMath::drawPixelXY(x0, y0, color);
    return;
  }

  while (a >= b)  {
    EffectMath::drawPixelXY(a + x0, b + y0, color);
    EffectMath::drawPixelXY(b + x0, a + y0, color);
    EffectMath::drawPixelXY(-a + x0, b + y0, color);
    EffectMath::drawPixelXY(-b + x0, a + y0, color);
    EffectMath::drawPixelXY(-a + x0, -b + y0, color);
    EffectMath::drawPixelXY(-b + x0, -a + y0, color);
    EffectMath::drawPixelXY(a + x0, -b + y0, color);
    EffectMath::drawPixelXY(b + x0, -a + y0, color);
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

void EffectMath::drawCircleF(float x0, float y0, float radius, const CRGB &color, float step){
  float a = radius, b = 0.;
  float radiusError = step - a;

  if (radius <= step*2) {
    EffectMath::drawPixelXYF(x0, y0, color);
    return;
  }

  while (a >= b)  {
      EffectMath::drawPixelXYF(a + x0, b + y0, color, 50);
      EffectMath::drawPixelXYF(b + x0, a + y0, color, 50);
      EffectMath::drawPixelXYF(-a + x0, b + y0, color, 50);
      EffectMath::drawPixelXYF(-b + x0, a + y0, color, 50);
      EffectMath::drawPixelXYF(-a + x0, -b + y0, color, 50);
      EffectMath::drawPixelXYF(-b + x0, -a + y0, color, 50);
      EffectMath::drawPixelXYF(a + x0, -b + y0, color, 50);
      EffectMath::drawPixelXYF(b + x0, -a + y0, color, 50);

    b+= step;
    if (radiusError < 0.)
      radiusError += 2. * b + step;
    else
    {
      a-= step;
      radiusError += 2 * (b - a + step);
    }
  }
}

void EffectMath::fill_circleF(float cx, float cy, float radius, CRGB col) {
  int8_t rad = radius;
  for (float y = -radius; y < radius; y += (fabs(y) < rad ? 1 : 0.2)) {
    for (float x = -radius; x < radius; x += (fabs(x) < rad ? 1 : 0.2)) {
      if (x * x + y * y < radius * radius)
        EffectMath::drawPixelXYF(cx + x, cy + y, col, 0);
    }
  }
}

void EffectMath::nightMode(CRGB *leds)
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i].r = dim8_lin(leds[i].r); //dim8_video
        leds[i].g = dim8_lin(leds[i].g);
        leds[i].b = dim8_lin(leds[i].b);
    }
}
uint32_t EffectMath::getPixColorXY(int16_t x, int16_t y) { return getPixColor( getPixelNumber(x, y)); } // функция получения цвета пикселя в матрице по его координатам
//void EffectMath::setLedsfadeToBlackBy(uint16_t idx, uint8_t val) { leds[idx].fadeToBlackBy(val); }
void EffectMath::setLedsNscale8(uint16_t idx, uint8_t val) { leds[idx].nscale8(val); }
void EffectMath::dimAll(uint8_t value) { for (uint16_t i = 0; i < NUM_LEDS; i++) {leds[i].nscale8(value); } }
void EffectMath::blur2d(uint8_t val) {EffectMath::blur2d(leds,WIDTH,HEIGHT,val);}

CRGB &EffectMath::getLed(uint16_t idx) { 
  if(idx<NUM_LEDS){
    return leds[idx];
  } else {
    return overrun;
  }
}


uint32_t EffectMath::getPixelNumberBuff(uint16_t x, uint16_t y, uint8_t W , uint8_t H) // получить номер пикселя в буфере по координатам
{

  uint16_t _THIS_Y = y;
  uint16_t _THIS_X = x;
  
  if ((_THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
  {
      return ((uint32_t)_THIS_Y * SEGMENTS * W + _THIS_X);
  }
  else                                                      // если нечётная строка
  {
      return ((uint32_t)_THIS_Y * SEGMENTS * W + W - _THIS_X - 1);
  }

}

CRGB &EffectMath::getPixel(uint16_t x, uint16_t y){
  // Все, что не попадает в диапазон WIDTH x HEIGHT отправляем в "невидимый" светодиод.
  if (y > getmaxHeightIndex() || x > getmaxWidthIndex())
      return overrun;
  return leds[getPixelNumber(x,y)];
}

float EffectMath::sqrt(float x){
  union{
      int i;
      float x;
  } u;

  u.x = x;
  // u.i = (1<<29) + (u.i >> 1) - (1<<22);
  // u.i = 0x20000000 + (u.i >> 1) - 0x400000;
  u.i = (u.i >> 1) + 0x1FC00000;
  return u.x;
}

float EffectMath::tan2pi_fast(float x) {
  float y = (1 - x*x);
  return x * (((-0.000221184 * y + 0.0024971104) * y - 0.02301937096) * y + 0.3182994604 + 1.2732402998 / y);
  //float y = (1 - x*x);
  //return x * (-0.0187108 * y + 0.31583526 + 1.27365776 / y);
}

float EffectMath::atan2_fast(float y, float x)
{
  //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
  //Volkan SALMA

  const float ONEQTR_PI = PI / 4.0;
  const float THRQTR_PI = 3.0 * PI / 4.0;
  float r, angle;
  float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
  if ( x < 0.0f )
  {
      r = (x + abs_y) / (abs_y - x);
      angle = THRQTR_PI;
  }
  else
  {
      r = (x - abs_y) / (x + abs_y);
      angle = ONEQTR_PI;
  }
  angle += (0.1963f * r * r - 0.9817f) * r;
  if ( y < 0.0f )
      return( -angle );     // negate if in quad III or IV
  else
      return( angle );
}

float EffectMath::atan_fast(float x){
  /*
  A fast look-up method with enough accuracy
  */
  if (x > 0) {
      if (x <= 1) {
      int index = round(x * 100);
      return LUT[index];
      } else {
      float re_x = 1 / x;
      int index = round(re_x * 100);
      return (M_PI_2 - LUT[index]);
      }
  } else {
      if (x >= -1) {
      float abs_x = -x;
      int index = round(abs_x * 100);
      return -(LUT[index]);
      } else {
      float re_x = 1 / (-x);
      int index = round(re_x * 100);
      return (LUT[index] - M_PI_2);
      }
  }
}

float EffectMath::mapcurve(const float x, const float in_min, const float in_max, const float out_min, const float out_max, float (*curve)(float,float,float,float)){
  if (x <= in_min) return out_min;
  if (x >= in_max) return out_max;
  return curve((x - in_min), out_min, (out_max - out_min), (in_max - in_min));
}

float EffectMath::InOutQuad(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return c / 2 * t * t + b;
  --t;
  return -c / 2 * (t * (t - 2) - 1) + b;
}

float EffectMath::InOutCubic(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return c / 2 * t * t * t + b;
  t -= 2;
  return c / 2 * (t * t * t + 2) + b;
}

float EffectMath::InOutQuart(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return c / 2 * t * t * t * t + b;
  t -= 2;
  return -c / 2 * (t * t * t * t - 2) + b;
}

float EffectMath::OutQuint(float t, float b, float c, float d) {
  t = t / d - 1;
  return c * (t * t * t * t * t + 1) + b;
}

float EffectMath::InOutQuint(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return  c / 2 * t * t * t * t * t + b;
  t -= 2;
  return c / 2 * (t * t * t * t * t + 2) + b;
}

float EffectMath::InOutExpo(float t, float b, float c, float d) {
  if (t==0) return b;
  if (t==d) return b + c;
  t /= d / 2;
  if (t < 1) return c/2 * powf(2, 10 * (t - 1)) + b;
  --t;
  return c/2 * (-powf(2, -10 * t) + 2) + b;
}

float EffectMath::InOutCirc(float t, float b, float c, float d) {
  t /= d / 2;
  if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
  t -= 2;
  return c/2 * (sqrt(1 - t*t) + 1) + b;
}

#ifdef RGB_PLAYER

// преобразовать цвет из 8 битного формата rgb332 в 24 битный
CRGB EffectMath::rgb332_To_CRGB(uint8_t value) {
    CRGB color;
    color.r = value & 0xe0; // mask out the 3 bits of red at the start of the byte
    color.r |= (color.r >> 3); // extend limited 0-224 range to 0-252
    color.r |= (color.r >> 3); // extend limited 0-252 range to 0-255
    color.g = value & 0x1c; // mask out the 3 bits of green in the middle of the byte
    color.g |= (color.g << 3) | (color.r >> 3); // extend limited 0-34 range to 0-255
    color.b = value & 0x03; // mask out the 2 bits of blue at the end of the byte
    color.b |= color.b << 2; // extend 0-3 range to 0-15
    color.b |= color.b << 4; // extend 0-15 range to 0-255
    return color;
}

// преобразовать цвет из 16 битного формата rgb565 в 24 битный
CRGB EffectMath::rgb565_To_CRGB(uint16_t value) {
  // gamma correction для expandColor
  static const uint8_t
  gamma5[] PROGMEM = {
    0x00, 0x01, 0x02, 0x03, 0x05, 0x07, 0x09, 0x0b,
    0x0e, 0x11, 0x14, 0x18, 0x1d, 0x22, 0x28, 0x2e,
    0x36, 0x3d, 0x46, 0x4f, 0x59, 0x64, 0x6f, 0x7c,
    0x89, 0x97, 0xa6, 0xb6, 0xc7, 0xd9, 0xeb, 0xff
  },
  gamma6[] PROGMEM = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08,
    0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x10, 0x12, 0x13,
    0x15, 0x17, 0x19, 0x1b, 0x1d, 0x20, 0x22, 0x25,
    0x27, 0x2a, 0x2d, 0x30, 0x33, 0x37, 0x3a, 0x3e,
    0x41, 0x45, 0x49, 0x4d, 0x52, 0x56, 0x5b, 0x5f,
    0x64, 0x69, 0x6e, 0x74, 0x79, 0x7f, 0x85, 0x8b,
    0x91, 0x97, 0x9d, 0xa4, 0xab, 0xb2, 0xb9, 0xc0,
    0xc7, 0xcf, 0xd6, 0xde, 0xe6, 0xee, 0xf7, 0xff
  };
  // CRGB color =  ((uint32_t)(gamma5[ value >> 11       ]) << 16) |
  //               ((uint32_t)(gamma6[(value >> 5) & 0x3F]) <<  8) |
  //               (gamma5[ value       & 0x1F]);
  CRGB color =  ((uint32_t)pgm_read_dword(&gamma5[ value >> 11       ]) << 16) |
                ((uint32_t)pgm_read_dword(&gamma6[(value >> 5) & 0x3F]) <<  8) |
                pgm_read_dword(&gamma5[ value       & 0x1F]);
  return color;
}

#endif