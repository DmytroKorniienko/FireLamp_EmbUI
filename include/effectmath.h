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

#ifndef __EFFECTMATH_H
#define __EFFECTMATH_H

// Общий набор мат. функций и примитивов для обсчета эффектов

#define M_PI_2	1.57079632679489661923

#define MAP_SIN 1
#define MAP_COS 0
#define MOVE_X 1
#define MOVE_Y 0

static const PROGMEM float LUT[102] = {
     0,           0.0099996664, 0.019997334, 0.029991005, 0.039978687,
     0.049958397, 0.059928156,  0.069885999, 0.079829983, 0.089758173,
     0.099668652, 0.10955953,   0.11942893,  0.12927501,  0.13909595,
     0.14888994,  0.15865526,   0.16839015,  0.17809294,  0.18776195,
     0.19739556,  0.20699219,   0.21655031,  0.22606839,  0.23554498,
     0.24497867,  0.25436807,   0.26371184,  0.27300870,  0.28225741,
     0.29145679,  0.30060568,   0.30970293,  0.31874755,  0.32773849,
     0.33667481,  0.34555557,   0.35437992,  0.36314702,  0.37185606,
     0.38050637,  0.38909724,   0.39762798,  0.40609807,  0.41450688,
     0.42285392,  0.43113875,   0.43936089,  0.44751999,  0.45561564,
     0.46364760,  0.47161558,   0.47951928,  0.48735857,  0.49513325,
     0.50284320,  0.51048833,   0.51806855,  0.52558380,  0.53303409,
     0.54041952,  0.54774004,   0.55499572,  0.56218672,  0.56931317,
     0.57637525,  0.58337301,   0.59030676,  0.59717667,  0.60398299,
     0.61072594,  0.61740589,   0.62402308,  0.63057774,  0.63707036,
     0.64350110,  0.64987046,   0.65617871,  0.66242629,  0.66861355,
     0.67474097,  0.68080884,   0.68681765,  0.69276786,  0.69865984,
     0.70449406,  0.71027100,   0.71599114,  0.72165483,  0.72726268,
     0.73281509,  0.73831260,   0.74375558,  0.74914461,  0.75448018,
     0.75976276,  0.76499283,   0.77017093,  0.77529752,  0.78037310,
     0.78539819,  0.79037325};


/**
 * Класс с набором статических мат. функций, которые используются в нескольких разных эффектах
 */

namespace EffectMath_PRIVATE {
    typedef union {
    struct {
        bool MIRR_V:1; // отзрекаливание по V
        bool MIRR_H:1; // отзрекаливание по H
    };
    uint32_t flags; // набор битов для конфига
    } MATRIXFLAGS;

    extern MATRIXFLAGS matrixflags;
    extern CRGB leds[NUM_LEDS]; // основной буфер вывода изображения
    extern CRGB overrun;

    CRGB *getUnsafeLedsArray();
    uint32_t getPixelNumber(int16_t x, int16_t y);
}

using namespace EffectMath_PRIVATE;

class EffectMath {
private:
    friend class LAMP;
    friend uint16_t XY(uint8_t x, uint8_t y); // Friend function 
public:
  static const uint16_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);
  static const uint16_t minDim = ((WIDTH<HEIGHT)?WIDTH:HEIGHT);
  static uint16_t getmaxDim() {return maxDim;}
  static uint16_t getminDim() {return minDim;}
  static int16_t getmaxWidthIndex() {return (int16_t)WIDTH-1;}
  static int16_t getmaxHeightIndex() {return (int16_t)HEIGHT-1;}

    // для работы с буфером
    static uint32_t getPixelNumberBuff(uint16_t x, uint16_t y, uint8_t W , uint8_t H) // получить номер пикселя в буфере по координатам
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

    static void setPixel(int16_t x, int16_t y, const CRGB &pixel){
        // Все, что не попадает в диапазон WIDTH x HEIGHT отправляем в "невидимый" светодиод.
        if (y < 0 || y > getmaxHeightIndex() || x < 0 || x > getmaxWidthIndex()) return;
        leds[getPixelNumber(x,y)] = pixel;
    }

    static CRGB &getPixel(int16_t x, int16_t y){
        // Все, что не попадает в диапазон WIDTH x HEIGHT отправляем в "невидимый" светодиод.
        if (y < 0 || y > getmaxHeightIndex() || x < 0 || x > getmaxWidthIndex())
            return overrun;
        return leds[getPixelNumber(x,y)];
    }

  static uint8_t mapsincos8(bool map, uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255);
  static void MoveFractionalNoise(bool scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift = 0);
  static void fadePixel(uint8_t i, uint8_t j, uint8_t step);
  static void fader(uint8_t step);
  static uint8_t ceil8(const uint8_t a, const uint8_t b);
  static CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter = 5);
  static CRGB makeDarker( const CRGB& color, fract8 howMuchDarker = 5);
  static float randomf(float min, float max);
  static bool isInteger(float val);
  static void addGlitter(uint8_t chanceOfGlitter = 127);
  static void nightMode(CRGB *leds);

/*
  static CRGB& piXY(CRGB *leds, byte x, byte y);
  static void screenscale( accum88 a, byte N, byte& screen, byte& screenerr);
  static void plot88(CRGB *leds, byte x, byte y, CRGB& color);
  static int16_t scale15by8_local( int16_t i, fract8 scale );
*/
    // функция получения цвета пикселя по его номеру
    static uint32_t getPixColor(uint32_t thisSegm); 
    // функция получения цвета пикселя в матрице по его координатам
    static uint32_t getPixColorXY(uint16_t x, uint16_t y); 
    // залить все
    static void fillAll(const CRGB &color); 
    
    static void drawPixelXY(int16_t x, int16_t y, const CRGB &color); // функция отрисовки точки по координатам X Y
    /* 
    Функция отрисовки точки по координатам X, Y
    По умолчанию leds[idx] = color
    opt == 1: leds[idx] += color
    opt == 2: leds[idx] -= color
    opt == 3: leds[idx] *= color
    opt == 4: leds[idx] /= color
    */
    static void drawPixelXY(int16_t x, int16_t y, const CRGB &color, byte opt); 
    static void wu_pixel(uint32_t x, uint32_t y, CRGB col);
    static void drawPixelXYF(float x, float y, const CRGB &color, uint8_t darklevel=25); // darklevel - насколько затемнять картинку
    static void drawPixelXYF_Y(uint16_t x, float y, const CRGB &color, uint8_t darklevel=50);
    static void drawPixelXYF_X(float x, uint16_t y, const CRGB &color, uint8_t darklevel=50);

    static CRGB getPixColorXYF_X(float x, uint16_t y);
    static CRGB getPixColorXYF_Y(uint16_t x, float y);
    static CRGB getPixColorXYF(float x, float y);

    static void drawLine(int x1, int y1, int x2, int y2, const CRGB &color);
    static void drawLineF(float x1, float y1, float x2, float y2, const CRGB &color);
    static void drawCircle(int x0, int y0, int radius, const CRGB &color);
    static void drawCircleF(float x0, float y0, float radius, const CRGB &color, float step = 0.25);
    //static void setLedsfadeToBlackBy(uint16_t idx, uint8_t val);
    static void setLedsNscale8(uint16_t idx, uint8_t val);
    static void dimAll(uint8_t value);
    static CRGB &getLed(uint16_t idx);
    static void blur2d(uint8_t val);
    static CRGB *setLed(uint16_t idx, CHSV val);
    static CRGB *setLed(uint16_t idx, CRGB val);

    /** аналог ардуино функции map(), но только для float
   */
    static double fmap(const double x, const double in_min, const double in_max, const double out_min, const double out_max){
        return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
    }

    static float distance(float x1, float y1, float x2, float y2){
        float dx = x2 - x1, dy = y2 - y1;
        return EffectMath::sqrt((dx * dx) + (dy * dy));
    }
    // чуть менее точная, зато в 3 раза быстрее
    static float sqrt(float x){
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

    static float tan2pi_fast(float x) {
        float y = (1 - x*x);
        return x * (((-0.000221184 * y + 0.0024971104) * y - 0.02301937096) * y + 0.3182994604 + 1.2732402998 / y);
        //float y = (1 - x*x);
        //return x * (-0.0187108 * y + 0.31583526 + 1.27365776 / y);
    }

    static float atan2_fast(float y, float x)
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

    static float atan_fast(float x){
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

    // аналог fmap, но не линейная. (linear == fmap)
    static float mapcurve(const float x, const float in_min, const float in_max, const float out_min, const float out_max, float (*curve)(float,float,float,float)){
        if (x <= in_min) return out_min;
        if (x >= in_max) return out_max;
        return curve((x - in_min), out_min, (out_max - out_min), (in_max - in_min));
    }
    static float linear(float t, float b, float c, float d) { return c * t / d + b; }
    static float InQuad(float t, float b, float c, float d) { t /= d; return c * t * t + b; }
    static float OutQuad(float t, float b, float c, float d) { t /= d; return -c * t * (t - 2) + b; }
    static float InOutQuad(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t + b;
        --t;
        return -c / 2 * (t * (t - 2) - 1) + b;
    }
    static float InCubic(float t, float b, float c, float d) { t /= d; return c * t * t * t + b; }
    static float OutCubic(float t, float b, float c, float d) { t = t / d - 1; return c * (t * t * t + 1) + b; }
    static float InOutCubic(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t * t + b;
        t -= 2;
        return c / 2 * (t * t * t + 2) + b;
    }
    static float InQuart(float t, float b, float c, float d) { t /= d; return c * t * t * t * t + b; }
    static float OutQuart(float t, float b, float c, float d) { t = t / d - 1; return -c * (t * t * t * t - 1) + b; }
    static float InOutQuart(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return c / 2 * t * t * t * t + b;
        t -= 2;
        return -c / 2 * (t * t * t * t - 2) + b;
    }
    static float InQuint(float t, float b, float c, float d) { t /= d; return c * t * t * t * t * t + b; }

    static float fixed_to_float(int input){
        return ((float)input / (float)(1 << 16));
    }
    static int float_to_fixed(float input){
        return (int)(input * (1 << 16));
    }
    static float OutQuint(float t, float b, float c, float d) {
        t = t / d - 1;
        return c * (t * t * t * t * t + 1) + b;
    }
    static float InOutQuint(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return  c / 2 * t * t * t * t * t + b;
        t -= 2;
        return c / 2 * (t * t * t * t * t + 2) + b;
    }
    // static float InSine(float t, float b, float c, float d) { return -c * Math.cos(t/d * (Math.PI/2)) + c + b; }
    // static float OutSine(float t, float b, float c, float d) { return c * Math.sin(t/d * (Math.PI/2)) + b; }
    // static float InOutSine(float t, float b, float c, float d) { return -c/2 * (Math.cos(Math.PI*t/d) - 1) + b; }
    static float InExpo(float t, float b, float c, float d) { return (t==0) ? b : c * powf(2, 10 * (t/d - 1)) + b; }
    static float OutExpo(float t, float b, float c, float d) { return (t==d) ? b+c : c * (-powf(2, -10 * t/d) + 1) + b; }
    static float InOutExpo(float t, float b, float c, float d) {
        if (t==0) return b;
        if (t==d) return b + c;
        t /= d / 2;
        if (t < 1) return c/2 * powf(2, 10 * (t - 1)) + b;
        --t;
        return c/2 * (-powf(2, -10 * t) + 2) + b;
    }
    static float InCirc(float t, float b, float c, float d) { t /= d; return -c * (sqrt(1 - t * t) - 1) + b; }
    static float OutCirc(float t, float b, float c, float d) { t = t / d - 1; return c * sqrt(1 - t * t) + b; }
    static float InOutCirc(float t, float b, float c, float d) {
        t /= d / 2;
        if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
        t -= 2;
        return c/2 * (sqrt(1 - t*t) + 1) + b;
    }
};

#endif
