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

// Общий набор мат. функций и примитивов для обсчета эффектов


// для работы FastLed (blur2d)
uint16_t XY(uint8_t x, uint8_t y)
{
#ifdef ROTATED_MATRIX
  return myLamp.getPixelNumber(y,x); // повернутое на 90 градусов
#else
  return myLamp.getPixelNumber(x,y); // обычное подключение
#endif
}

//--------------------------------------
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
          PixelA = _scale ? EffectMath::getLed(myLamp.getPixelNumber(zD%WIDTH, a%HEIGHT)) : EffectMath::getLed(myLamp.getPixelNumber(a%WIDTH, zD%HEIGHT));

        CRGB PixelB = CRGB::Black ;
        if ((zF >= 0) && (zF < _side_b))
          PixelB = _scale ? EffectMath::getLed(myLamp.getPixelNumber(zF%WIDTH, a%HEIGHT)) : EffectMath::getLed(myLamp.getPixelNumber(a%WIDTH, zF%HEIGHT));
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
    if (EffectMath::getPixColor(pixelNum) == 0U) return;

    CRGB led = EffectMath::getLed(pixelNum);
    if (led.r >= 30U ||
        led.g >= 30U ||
        led.b >= 30U){
        EffectMath::setLedsfadeToBlackBy(pixelNum,step);
    }
    else{
        EffectMath::setLed(pixelNum, 0U);
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
 с шагом 1/4095 */
float EffectMath::randomf(float min, float max)
{
  return fmap((float)random16(4095), 0.0, 4095.0, min, max);
}

/* kostyamat добавил
 функция возвращает true, если float
 ~ целое (первая цифра после запятой == 0) */
bool EffectMath::isInteger(float val) {
    float val1;
    val1 = val - (int)val;
    if ((int)(val1 * 10) != 0)
        return false;
    else
        return true;
}

// Функция создает вспышки в разных местах матрицы, параметр 0-255. Чем меньше, тем чаще.
void EffectMath::addGlitter(uint8_t chanceOfGlitter){
  if ( random8() < chanceOfGlitter) myLamp.getUnsafeLedsArray()[random16(NUM_LEDS)] += CRGB::Gray;
}

uint32_t EffectMath::getPixColor(uint32_t thisSegm) // функция получения цвета пикселя по его номеру
{
  uint32_t thisPixel = thisSegm * SEGMENTS;
  if (thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)myLamp.getUnsafeLedsArray()[thisPixel].r << 16) | ((uint32_t)myLamp.getUnsafeLedsArray()[thisPixel].g << 8 ) | (uint32_t)myLamp.getUnsafeLedsArray()[thisPixel].b);
}

// Заливает матрицу выбраным цветом
void EffectMath::fillAll(const CRGB &color) 
{
  for (int32_t i = 0; i < NUM_LEDS; i++)
  {
    myLamp.getUnsafeLedsArray()[i] = color;
  }
}

void EffectMath::drawPixelXY(int16_t x, int16_t y, const CRGB &color) // функция отрисовки точки по координатам X Y
{
  if (x < 0 || x > (int16_t)(WIDTH - 1) || y < 0 || y > (int16_t)(HEIGHT - 1)) return;
  uint32_t thisPixel = myLamp.getPixelNumber((uint16_t)x, (uint16_t)y) * SEGMENTS;
  for (uint16_t i = 0; i < SEGMENTS; i++)
  {
    myLamp.getUnsafeLedsArray()[thisPixel + i] = color;
  }
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
    CRGB clr = EffectMath::getPixColorXY(xn, yn);
    clr.r = qadd8(clr.r, (col.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (col.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (col.b * wu[i]) >> 8);

    EffectMath::drawPixelXY(xn, yn, clr);
  }
  #undef WU_WEIGHT
}

void EffectMath::drawPixelXYF(float x, float y, const CRGB &color, uint8_t darklevel)
{
  if (x<0 || y<0 || x>((float)WIDTH) || y>((float)HEIGHT)) return;

  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    CRGB clr = EffectMath::getPixColorXY(xn, yn);
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);

    // if(xn<(int)WIDTH-1 && yn<(int)HEIGHT-1 && yn>0 && xn>0){
    //   clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    //   clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    //   clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    // } else if((yn==0 || yn==HEIGHT-1 || xn==0) && xx<127) {
    //   clr.r = qadd8(clr.r, (color.r * 64) >> 8);
    //   clr.g = qadd8(clr.g, (color.g * 64) >> 8);
    //   clr.b = qadd8(clr.b, (color.b * 64) >> 8);
    // }
    if (darklevel > 0) EffectMath::drawPixelXY(xn, yn, EffectMath::makeDarker(clr, darklevel));
    else EffectMath::drawPixelXY(xn, yn, clr);
  }
  #undef WU_WEIGHT
}

void EffectMath::drawPixelXYF_X(float x, uint16_t y, const CRGB &color, uint8_t darklevel)
{
  if (x<0 || y<0 || x>((float)WIDTH) || y>((float)HEIGHT)) return;

  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, ix = 255 - xx;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {ix, xx};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
      int16_t xn = x + (i & 1);
      CRGB clr = EffectMath::getPixColorXY(xn, y);
      if(xn>0 && xn<(int)WIDTH-1){
        clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
        clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
        clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
      } else if(xn==0 || xn==(int)WIDTH-1) {
        clr.r = qadd8(clr.r, (color.r * 85) >> 8);
        clr.g = qadd8(clr.g, (color.g * 85) >> 8);
        clr.b = qadd8(clr.b, (color.b * 85) >> 8);
      }
    if (darklevel > 0) EffectMath::drawPixelXY(xn, y, EffectMath::makeDarker(clr, darklevel));
    else EffectMath::drawPixelXY(xn, y, clr);
  }
}

void EffectMath::drawPixelXYF_Y(uint16_t x, float y, const CRGB &color, uint8_t darklevel)
{
  if (x<0 || y<0 || x>((float)WIDTH) || y>((float)HEIGHT)) return;

  // extract the fractional parts and derive their inverses
  uint8_t yy = (y - (int)y) * 255, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {iy, yy};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
      int16_t yn = y + (i & 1);
      CRGB clr = EffectMath::getPixColorXY(x, yn);
      if(yn>0 && yn<(int)HEIGHT-1){
        clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
        clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
        clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
      } else if(yn==0 || yn==(int)HEIGHT-1) {
        clr.r = qadd8(clr.r, (color.r * 85) >> 8);
        clr.g = qadd8(clr.g, (color.g * 85) >> 8);
        clr.b = qadd8(clr.b, (color.b * 85) >> 8);
      }
    if (darklevel > 0) EffectMath::drawPixelXY(x, yn, EffectMath::makeDarker(clr, darklevel));
    else EffectMath::drawPixelXY(x, yn, clr);
  }
}

CRGB EffectMath::getPixColorXYF(float x, float y)
{
  if (x<0 || y<0 || x>((float)WIDTH-1) || y>((float)HEIGHT-1)) return CRGB::Black;

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
      clr = EffectMath::getPixColorXY(xn, yn);
    } else {
      CRGB tmpColor=EffectMath::getPixColorXY(xn, yn);
      clr.r = qadd8(clr.r, (tmpColor.r * wu[i]) >> 8);
      clr.g = qadd8(clr.g, (tmpColor.g * wu[i]) >> 8);
      clr.b = qadd8(clr.b, (tmpColor.b * wu[i]) >> 8);
    }
  }
  return clr;
  #undef WU_WEIGHT
}

CRGB EffectMath::getPixColorXYF_X(float x, uint16_t y)
{
  if (x<0 || y<0 || x>((float)WIDTH-1) || y>((float)HEIGHT-1)) return CRGB::Black;

  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int)x) * 255, ix = 255 - xx;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {ix, xx};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  CRGB clr=CRGB::Black;
  for (int8_t i = 1; i >= 0; i--) {
      int16_t xn = x + (i & 1);
      if(i){
        clr = EffectMath::getPixColorXY(xn, y);
      } else {
        CRGB tmpColor=EffectMath::getPixColorXY(xn, y);
        clr.r = qadd8(clr.r, (tmpColor.r * wu[i]) >> 8);
        clr.g = qadd8(clr.g, (tmpColor.g * wu[i]) >> 8);
        clr.b = qadd8(clr.b, (tmpColor.b * wu[i]) >> 8);
      }
  }
  return clr;
}

CRGB EffectMath::getPixColorXYF_Y(uint16_t x, float y)
{
  if (x<0 || y<0 || x>((float)WIDTH-1) || y>((float)HEIGHT-1)) return CRGB::Black;

  // extract the fractional parts and derive their inverses
  uint8_t yy = (y - (int)y) * 255, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = {iy, yy};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  CRGB clr=CRGB::Black;
  for (int8_t i = 1; i >= 0; i--) {
      int16_t yn = y + (i & 1);
      if(i){
        clr = EffectMath::getPixColorXY(x, yn);
      } else {
        CRGB tmpColor=EffectMath::getPixColorXY(x, yn);
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

/*void EffectMath::drawCircleF(float x0, float y0, float radius, const CRGB &color){
  float x = 0., y = radius, error = 0.;
  float delta = .25 - 2. * radius;

  while (y >= 0) {
    drawPixelXYF(x0 + x, y0 + y, color);
    drawPixelXYF(x0 + x, y0 - y, color);
    drawPixelXYF(x0 - x, y0 + y, color);
    drawPixelXYF(x0 - x, y0 - y, color);
    error = 2. * (delta + y) - 1.;
    if (delta < 0. && error <= 0.) {
      ++x;
      delta += 2. * x + 1.;
      continue;
    }
    error = 2. * (delta - x) - 1.;
    if (delta > 0. && error > 0.) {
      --y;
      delta += 1. - 2. * y;
      continue;
    }
    ++x;
    delta += 2. * (x - y);
    --y;
  }
}*/

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

void EffectMath::nightMode(CRGB *leds)
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        myLamp.getUnsafeLedsArray()[i].r = dim8_video(myLamp.getUnsafeLedsArray()[i].r);
        myLamp.getUnsafeLedsArray()[i].g = dim8_video(myLamp.getUnsafeLedsArray()[i].g);
        myLamp.getUnsafeLedsArray()[i].b = dim8_video(myLamp.getUnsafeLedsArray()[i].b);
    }
}
uint32_t EffectMath::getPixColorXY(uint16_t x, uint16_t y) { return getPixColor( myLamp.getPixelNumber(x, y)); } // функция получения цвета пикселя в матрице по его координатам
void EffectMath::setLedsfadeToBlackBy(uint16_t idx, uint8_t val) { myLamp.getUnsafeLedsArray()[idx].fadeToBlackBy(val); }
void EffectMath::setLedsNscale8(uint16_t idx, uint8_t val) { myLamp.getUnsafeLedsArray()[idx].nscale8(val); }
void EffectMath::dimAll(uint8_t value) { for (uint16_t i = 0; i < NUM_LEDS; i++) { myLamp.getUnsafeLedsArray()[i].nscale8(value); } }
CRGB EffectMath::getLed(uint16_t idx) { return myLamp.getUnsafeLedsArray()[idx]; }
void EffectMath::blur2d(uint8_t val) {::blur2d(myLamp.getUnsafeLedsArray(),WIDTH,HEIGHT,val);}
CRGB *EffectMath::setLed(uint16_t idx, CHSV val) { myLamp.getUnsafeLedsArray()[idx] = val; return &myLamp.getUnsafeLedsArray()[idx]; }
CRGB *EffectMath::setLed(uint16_t idx, CRGB val) { myLamp.getUnsafeLedsArray()[idx] = val; return &myLamp.getUnsafeLedsArray()[idx]; }
