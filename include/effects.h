/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

#ifndef _EFFECTS_H
#define _EFFECTS_H

#include "color_palette.h"
#include "effectworker.h"
#include "effectmath.h"


const uint8_t maxDim = (WIDTH > HEIGHT)? WIDTH : HEIGHT;
const uint8_t minDim = (WIDTH < HEIGHT)? WIDTH : HEIGHT;
const uint8_t width_adj = (WIDTH < HEIGHT ? (HEIGHT - WIDTH) /2 : 0);
const uint8_t height_adj = (HEIGHT < WIDTH ? (WIDTH - HEIGHT) /2: 0);

//===== Спеціально навчений пустий ефект :) ====//
class EffectNone : public EffectCalc {
private:
    void load() override { FastLED.clear(); };
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override {return true;};
};

//===== Ефект Годинник =========================//
class EffectTime : public EffectCalc {
private:
    bool timeShiftDir; // Напрямок зсуву
    float curTimePos;  // Позиція вводу
    float color_idx;   // Індекс кольору
    CRGB hColor[1];    // Колір годин
    CRGB mColor[1];    // Колір хвилин
    uint32_t lastrun=0;// Лічильник часу для ефектів з "затримкою"
    bool isMinute=false;

    bool timePrintRoutine(CRGB *leds, EffectWorker *param);
    bool palleteTest(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Метасфери ========================//
/*
 Metaballs proof of concept by Stefan Petrick 
 https://gist.github.com/StefanPetrick/170fbf141390fafb9c0c76b8a0d34e54
*/
class EffectMetaBalls : public EffectCalc {
private:
	float speedFactor;
	const float hormap = (256 / WIDTH);
    const float vermap = (256 / HEIGHT);
	String setDynCtrl(UIControl*_val) override;
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Синусоїд =========================//
/*
 Sinusoid3 by Stefan Petrick
 read more about the concept: https://www.youtube.com/watch?v=mubH-w_gwdA
*/
//Sinusoid I,II,IV recreation by Stepko
class EffectSinusoid3 : public EffectCalc {
private:
	const uint8_t semiHeightMajor =  HEIGHT / 2 + (HEIGHT % 2);
	const uint8_t semiWidthMajor =  WIDTH / 2  + (WIDTH % 2);
	float e_s3_speed;
	float e_s3_size;
	uint8_t _scale;
	uint8_t type;
	
	String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Стрибаючі м'ячики ================//
//  BouncingBalls2014 is a program that lets you animate an LED strip
//  to look like a group of bouncing balls
//  Daniel Wilson, 2014
//  https://github.com/githubcdr/Arduino/blob/master/bouncingballs/bouncingballs.ino
//  With BIG thanks to the FastLED community!
//  переведено на субпіксельну графіку kostyamat
class EffectBBalls : public EffectCalc {
private:
    // можно переписать на динамческую память
    uint8_t bballsNUM_BALLS;                            // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way) ... количество мячиков теперь задаётся бегунком, а не константой
    byte bballsCOLOR[bballsMaxNUM_BALLS] ;              // прикручено при адаптации для разноцветных мячиков
    byte bballsBri[bballsMaxNUM_BALLS];                 // --- // ---
    int8_t bballsX[bballsMaxNUM_BALLS] ;                // прикручено при адаптации для распределения мячиков по радиусу лампы
    float bballsPos[bballsMaxNUM_BALLS] ;               // The integer position of the dot on the strip (LED index)
    float bballsHi = 0.0;                               // An array of heights
    float bballsVImpact[bballsMaxNUM_BALLS] ;           // As time goes on the impact velocity will change, so make an array to store those values
    uint32_t bballsTCycle = 0;                        // The time since the last time the ball struck the ground
    float bballsCOR[bballsMaxNUM_BALLS] ;               // Coefficient of Restitution (bounce damping)
    long  bballsTLast[bballsMaxNUM_BALLS] ;             // The clock time of the last ground strike
    float bballsShift[bballsMaxNUM_BALLS];
    float hue;
    bool halo = false;                                  // ореол
    uint8_t _scale=1;
    uint16_t _speed;
    bool bBallsRoutine(CRGB *leds, EffectWorker *param);
    void regen();
    void load() override;
	String setDynCtrl(UIControl*_val) override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Восьминіг =========================//
//Stepko and Sutaburosu
//Idea from https://www.youtube.com/watch?v=HsA-6KIbgto&ab_channel=GreatScott%21
//22/05/22
class EffectLightBalls : public EffectCalc {
private:
    const int8_t C_X = WIDTH / 2;
    const int8_t C_Y = HEIGHT / 2;
    uint8_t m_angle[WIDTH][HEIGHT];
    uint8_t m_radius[WIDTH][HEIGHT];
    float t = 0;
    float speedFactor;
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Пульс ============================//
// Stefan Petrick's PULSE Effect
class EffectPulse : public EffectCalc {
private:
    uint8_t pulse_hue;
    float pulse_step = 0;
    uint8_t centerX = random8(WIDTH - 5U) + 3U;
    uint8_t centerY = random8(HEIGHT - 5U) + 3U;
    uint8_t currentRadius = 4;
    float _pulse_hue = 0;
    uint8_t _pulse_hueall = 0;
    float speedFactor;
    String setDynCtrl(UIControl*_val) override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Блукаючий кубик ==================//
class EffectBall : public EffectCalc {
private:
    uint8_t ballSize;
    CRGB ballColor;
    float vectorB[2U];
    float coordB[2U];
	bool flag[2] = {true, true};
	float speedFactor;

	String setDynCtrl(UIControl*_val);
	
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Світлячки зі шлейфом =============//
#define _AMOUNT 16U
class EffectLighterTracers : public EffectCalc {
private:
    uint8_t cnt = 1;
    float vector[_AMOUNT][2U];
    float coord[_AMOUNT][2U];
    uint8_t ballColors[_AMOUNT];
    byte light[_AMOUNT];
    float speedFactor;
    bool lighterTracersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Веселка ==========================//
class EffectRainbow : public EffectCalc {
private:
    float hue; // вещественное для малых скоростей, нужно приведение к uint8_t по месту
    float twirlFactor;
    float micCoef;

    bool rainbowHorVertRoutine(bool isVertical);
    bool rainbowDiagonalRoutine();

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Кольори ==========================//
class EffectColors : public EffectCalc {
private:
    uint8_t ihue;
    uint8_t mode;
    uint8_t modeColor;

    bool colorsRoutine(CRGB *leds, EffectWorker *param);
    //void setscl(const byte _scl) override;
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Біла лампа =======================//
class EffectWhiteColorStripe : public EffectCalc {
private:
    uint8_t shift=0;
    bool whiteColorStripeRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Конфеті ==========================//
class EffectSparcles : public EffectCalc {
private:
    uint8_t eff = 1;
    bool sparklesRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Ефектопад ========================//
class EffectEverythingFall : public EffectCalc {
private:
    byte heat[WIDTH][HEIGHT];

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Вогонь 2012 ======================//
// based on FastLED example Fire2012WithPalette
// https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
// Updated by SottNick 17.04.2020
class EffectFire2012 : public EffectCalc {
private:
#define NUMPALETTES 10

  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
    uint8_t cooling = 80U; // 70
  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
     uint8_t sparking = 90U; // 130
  // SMOOTHING; How much blending should be done between frames
  // Lower = more blending and smoother flames. Higher = less blending and flickery flames
    uint8_t _scale = 1;
    const uint8_t fireSmoothing = 60U; // 90
    uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];
    bool fire2012Routine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Клас "Світлячків" ======================//
// потрібний для деяких ефектів
class EffectLighters : public EffectCalc {
protected:
    uint8_t cnt = 1;
    bool subPix = false;
    uint16_t lightersIdx;
    float lightersSpeed[2U][LIGHTERS_AM];
    uint8_t lightersColor[LIGHTERS_AM];
    float lightersPos[2U][LIGHTERS_AM];
    byte light[LIGHTERS_AM];
    float speedFactor;
private:
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Матриця ==========================//
// (c)Kostyamat
class EffectMatrix : public EffectLighters {
private:
    bool matrixRoutine(CRGB *leds, EffectWorker *param);
    uint8_t _scale = 1;
    byte gluk = 1;
    uint8_t hue, _hue;
    bool randColor = false;
    bool white = false;
    float count;
    float speedFactor;
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Хурделиця, Зіркопад, Метеори =====//
// (c)Kostyamat
class EffectStarFall : public EffectLighters {
private:
    uint8_t _scale = 1;
    uint8_t effId = 1;
    bool isNew = true;
    float fade;
    float speedFactor;
    bool snowStormStarfallRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Шум 3D ===========================//
class Effect3DNoise : public EffectCalc {
private:
    void fillNoiseLED();
    void fillnoise8();

    uint8_t ihue;
    bool colorLoop;
	bool blurIm;
    float _speed;             // speed is set dynamically once we've started up
    float _scale;             // scale is set dynamically once we've started up
    float x;
    float y;
    float z;
    uint8_t noise[maxDim][maxDim];

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Спіраль ==========================//
// https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
// Copyright (c) 2014 Jason Coon
// Перевів на субпіксель Stepko
class EffectSpiro : public EffectCalc {
private:
  const float spirocenterX = WIDTH /2-0.5; //(!(WIDTH & 1) ? (WIDTH -1) : WIDTH) / 2;
  const float spirocenterY = HEIGHT /2-0.5; //(!(HEIGHT & 1) ? (HEIGHT-1) : HEIGHT) / 2;

  byte AM = 1;
  bool incenter;
float Angle;
bool change = false;
  float speedFactor;

  String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Призмата =========================//
// https://github.com/pixelmatix/aurora/blob/master/PatternPendulumWave.h
// Copyright (c) 2014 Jason Coon
// Перевів на субпіксель Kostyamat
class EffectPrismata : public EffectCalc {
private:
    byte spirohueoffset = 0;
    uint8_t fadelvl=1;
	float speedFactor;
    
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Стадо та хижак ===================//
// https://github.com/pixelmatix/aurora/blob/master/PatternFlock.h
// Copyright (c) 2014 Jason Coon
// Адаптація (c)SottNick
class EffectFlock : public EffectCalc {
private:
  Boid boids[AVAILABLE_BOID_COUNT];
  Boid predator;
  PVector wind;
  float speedFactor;

  bool predatorPresent;
  float hueoffset;

  bool flockRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
  //void setspd(const byte _spd) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Комети, Витаючі Вогні ++ =========//
// Фундамент (c) Stefan Petrick
class EffectComet : public EffectCalc {
private:
    byte hue;
    byte hue2;
    uint8_t eNs_noisesmooth;
    uint32_t e_x[NUM_LAYERS];
    uint32_t e_y[NUM_LAYERS];
    uint32_t e_z[NUM_LAYERS];
    uint32_t e_scaleX[NUM_LAYERS];
    uint32_t e_scaleY[NUM_LAYERS];
    uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];
    uint8_t count;
    uint8_t speedy;
    float spiral;
    float spiral2;
    float speedFactor;
    uint8_t _scale = 1;
    uint8_t effId = 1;      // 2, 1-6
    uint8_t colorId;        // 3, 1-255
    uint8_t smooth = 1;     // 4, 1-12
    uint8_t blur;           // 5, 1-64

    const uint8_t e_centerX =  (WIDTH / 2) -  ((WIDTH - 1) & 0x01);
    const uint8_t e_centerY = (HEIGHT / 2) - ((HEIGHT - 1) & 0x01);

    void drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color);
    void FillNoise(int8_t layer);

    bool rainbowCometRoutine(CRGB *leds, EffectWorker *param);
    bool rainbowComet3Routine(CRGB *leds, EffectWorker *param);
    bool firelineRoutine(CRGB *leds, EffectWorker *param);
    bool fractfireRoutine(CRGB *leds, EffectWorker *param);
    bool flsnakeRoutine(CRGB *leds, EffectWorker *param);
    bool smokeRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Водоверть ========================//
// https://gist.github.com/kriegsman/5adca44e14ad025e6d3b
// Copyright (c) 2014 Mark Kriegsman
class EffectSwirl : public EffectCalc {
private:
    bool swirlRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Дрифт ============================//
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h
// Copyright(c) 2014 Jason Coon
// Subpixel ver by Stepko
class EffectDrift : public EffectCalc {
private:
	const byte maxDim_steps = 256 / maxDim;
	uint8_t dri_phase;
	float _dri_speed;
	uint8_t _dri_delta;
	byte driftType = 0;
    bool flag = false;

	String setDynCtrl(UIControl*_val) override;
	bool incrementalDriftRoutine(CRGB *leds, EffectWorker *param);
	bool incrementalDriftRoutine2(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Мерехтіння =======================//
// Ідея від SottNick
class EffectTwinkles : public EffectCalc {
private:
  typedef enum _DIR {FADEHI,FADELOW} DIR;
  typedef struct _TTWINKLE {
    union {
        struct {
            uint16_t pos:12; // позиція може співпадати, не перевіряємо щоб працювало швидко, 4096 элементів ліміт ефекта
            uint16_t direction:1;
            uint16_t speed:3; // 0...7
        };
        uint16_t data;
    };
    uint8_t color; // колір в палітрі
    uint8_t brightness; // яскравість
  } TTWINKLE;

  static const uint16_t storagesize = NUM_LEDS/2;
  TTWINKLE twinkles[storagesize];
  uint16_t tnum;
  uint16_t curtnum = 0;

  float speedFactor;
  bool twinklesRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    void setup();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    virtual ~EffectTwinkles();
};

//===== Ефект Хвилі ============================//
// https://github.com/pixelmatix/aurora/blob/master/PatternWave.h
// Copyright(c) 2014 Jason Coon
// Адаптовано (c)SottNick
class EffectWaves : public EffectCalc {
private:
  float whue;
  float waveTheta;
  uint8_t _scale=1;
  float speedFactor;
  bool wavesRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Радар ============================//
// https://github.com/pixelmatix/aurora/blob/master/PatternRadar.h
// Copyright(c) 2014 Jason Coon
class EffectRadar : public EffectCalc {
private:
    float eff_offset; 
    float eff_theta;  // глобальная переменная угла для работы эффектов
    bool subPix = false;
    byte hue;
    const float width_adj_f = (float)(WIDTH < HEIGHT ? (HEIGHT - WIDTH) / 2. : 0);
    const float height_adj_f= (float)(HEIGHT < WIDTH ? (WIDTH - HEIGHT) / 2. : 0);
    String setDynCtrl(UIControl *_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Вогонь 2018 ======================//
// https://gist.github.com/StefanPetrick/819e873492f344ebebac5bcd2fdd8aa8
// https://gist.github.com/StefanPetrick/1ba4584e534ba99ca259c1103754e4c5
// Updating by SottNick 17.04.2020
class EffectFire2018 : public EffectCalc {
private:
  const uint8_t CentreY = HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t CentreX = WIDTH / 2 + (WIDTH % 2);
  bool isLinSpeed = true;

  uint32_t noise32_x[NUM_LAYERS2];
  uint32_t noise32_y[NUM_LAYERS2];
  uint32_t noise32_z[NUM_LAYERS2];
  uint32_t scale32_x[NUM_LAYERS2];
  uint32_t scale32_y[NUM_LAYERS2];
  uint8_t fire18heat[NUM_LEDS];
  uint8_t noise3dx[NUM_LAYERS2][WIDTH][HEIGHT];

  String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Кодовий замок ====================//
// (c) SottNick
class EffectRingsLock : public EffectCalc {
private:
  uint8_t ringWidth; // максимальне количество пикселей в кольце (толщина кольца) от 1 до height / 2 + 1
  uint8_t ringNb; // количество колец от 2 до height
  uint8_t downRingHue, upRingHue; // количество пикселей в нижнем (downRingHue) и верхнем (upRingHue) кольцах

  uint8_t ringColor[HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
  uint8_t huePos[HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
  uint8_t shiftHueDir[HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
  ////ringHueShift[ringsCount]; // шаг градиета оттенка внутри кольца -8 - +8 случайное число
  ////ringHueShift2[ringsCount]; // обычная скорость переливания оттенка всего кольца -8 - +8 случайное число
  uint8_t currentRing; // кольцо, которое в настоящий момент нужно провернуть
  uint8_t stepCount; // оставшееся количество шагов, на которое нужно провернуть активное кольцо - случайное от WIDTH/5 до WIDTH-3
  void ringsSet();
  bool ringsRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Куб 2D ===========================//
// (c) SottNick
// refactored by Vortigont
class EffectCube2d : public EffectCalc {
private:
  bool classic = false;
  uint8_t sizeX, sizeY; // размеры ячеек по горизонтали / вертикали
  uint8_t cntX, cntY; // количество ячеек по горизонтали / вертикали
  uint8_t fieldX, fieldY; // размер всего поля блоков по горизонтали / вертикали (в том числе 1 дополнительная пустая дорожка-разделитель с какой-то из сторон)
  uint8_t currentStep;
  uint8_t pauseSteps; // осталось шагов паузы
  uint8_t shiftSteps; // всего шагов сдвига
  std::vector<int8_t> moveItems;     // индекс перемещаемого элемента
  //bool movedirection;   // направление смещения
  bool direction; // направление вращения в текущем цикле (вертикаль/горизонталь)
  uint8_t storage[WIDTH][HEIGHT];
  int8_t globalShiftX, globalShiftY;
  uint8_t gX, gY;
  bool seamlessX = true;

  std::vector<CRGB> ledbuff;

  void swapBuff();
  void cubesize();
  bool cube2dRoutine(CRGB *leds, EffectWorker *param);
  bool cube2dClassicRoutine(CRGB *leds, EffectWorker *param);
  void cube2dmoveCols(uint8_t moveItem, bool movedirection);
  void cube2dmoveRows(uint8_t moveItem, bool movedirection);
  String setDynCtrl(UIControl*_val) override;
  //void setscl(const byte _scl) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Пікассо ==========================//
// (c) obliterator
class EffectPicasso : public EffectCalc {
    typedef struct Particle{
        float position_x = 0;
        float position_y = 0;
        float speed_x = 0;
        float speed_y = 0;
        CHSV color;
        uint8_t hue_next = 0;
        int8_t hue_step = 0;
    } Particle;
private:
    uint8_t effId=0;
    uint8_t pidx = 0;
    Particle particles[20];
    unsigned numParticles = 0;
	float speedFactor;
    void generate(bool reset = false);
    void position();
    bool picassoRoutine(CRGB *leds, EffectWorker *param);
    bool metaBallsRoutine(CRGB *leds, EffectWorker *param);
    GradientPaletteList *palettes;
public:
    EffectPicasso() {
        palettes = new GradientPaletteList();
        palettes->add(MBVioletColors_gp, 0, 16); // будет заменен генератором
        palettes->add(MBVioletColors_gp, 0, 16);

        palettes->add(ib_jul01_gp, 60, 16, 200);

        palettes->add(es_pinksplash_08_gp, 125, 16);

        palettes->add(departure_gp, 0);
        palettes->add(departure_gp, 140, 16, 220);

        palettes->add(es_landscape_64_gp, 25, 16, 250);
        palettes->add(es_landscape_64_gp, 125);
        palettes->add(es_landscape_64_gp, 175, 50, 220);

        palettes->add(es_ocean_breeze_036_gp, 0);

        palettes->add(es_landscape_33_gp, 0);
        palettes->add(es_landscape_33_gp, 50);
        palettes->add(es_landscape_33_gp, 50, 50);

        palettes->add(GMT_drywet_gp, 0);
        palettes->add(GMT_drywet_gp, 75);
        palettes->add(GMT_drywet_gp, 150, 0, 200);

        palettes->add(fire_gp, 175);

        palettes->add(Pink_Purple_gp, 25);
        palettes->add(Pink_Purple_gp, 175, 0, 220);

        palettes->add(Sunset_Real_gp, 25, 0, 200);
        palettes->add(Sunset_Real_gp, 50, 0, 220);

        palettes->add(BlacK_Magenta_Red_gp, 25);
    }
    ~EffectPicasso() { delete palettes; }
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Стрибуни =========================//
class EffectLeapers : public EffectCalc {
    typedef struct Leaper{
        float x, y;
        float xd, yd;
        byte color;
    } Leaper;
private:
    Leaper leapers[20];
    unsigned numParticles = 0;
    uint8_t _rv;
	uint8_t num;
	float speedFactor;
    void generate(bool reset = false);
    void restart_leaper(Leaper * l);
    void move_leaper(Leaper * l);
	String setDynCtrl(UIControl*_val) override;
public:
	void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;

};

//===== Ефект Лавова лампа =====================//
// (c) obliterator
class EffectLiquidLamp : public EffectCalc {
    typedef struct Particle{
        float position_x = 0;
        float position_y = 0;
        float speed_x = 0;
        float speed_y = 0;
        float rad = 0;
        float hot = 0;
        float spf = 0;
        int mass = 0;
        unsigned mx = 0;
        unsigned sc = 0;
        unsigned tr = 0;
    } Particle;
private:
    unsigned MASS_MIN = 10;
    unsigned MASS_MAX = 50;
    Particle particles[20];
    uint8_t buff[WIDTH][HEIGHT];
    uint8_t pidx = 0;
    unsigned numParticles = 0;
    unsigned physic_on = 1;
    unsigned filter = 0;
	float speedFactor;
    void generate(bool reset = false);
    void position();
    void physic();
    bool Routine(CRGB *leds, EffectWorker *param);

    GradientPaletteList *palettes;
public:
    EffectLiquidLamp() {
        palettes = new GradientPaletteList();
        palettes->add(MBVioletColors_gp, 0, 16);
        // эта политра создана под эффект
        palettes->add(MBVioletColors_gp, 0, 16);
        // палитры частично подогнаные под эффект
        palettes->add(ib_jul01_gp, 60, 16, 200);
        palettes->add(Sunset_Real_gp, 25, 0, 200);

        palettes->add(es_landscape_33_gp, 50, 50);

        palettes->add(es_pinksplash_08_gp, 125, 16);

        palettes->add(es_landscape_64_gp, 175, 50, 220);
        palettes->add(es_landscape_64_gp, 25, 16, 250);

        palettes->add(es_ocean_breeze_036_gp, 0);

        palettes->add(es_landscape_33_gp, 0);

        palettes->add(GMT_drywet_gp, 0);
        palettes->add(GMT_drywet_gp, 75);
        palettes->add(GMT_drywet_gp, 150, 0, 200);

        palettes->add(fire_gp, 175);

        palettes->add(Pink_Purple_gp, 25);
        palettes->add(Pink_Purple_gp, 175, 0, 220);

        palettes->add(Sunset_Real_gp, 50, 0, 220);

        palettes->add(BlacK_Magenta_Red_gp, 25);
    }
    ~EffectLiquidLamp() { delete palettes; }
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Вихори ===========================//
// https://github.com/pixelmatix/aurora/blob/master/PatternFlowField.h
// Copyright(c) 2014 Jason Coon
// адаптація SottNick
class EffectWhirl : public EffectCalc {
private:
    float ff_x;
    float ff_y;
    float ff_z;
    float hue;
    Boid boids[AVAILABLE_BOID_COUNT];
    uint8_t micPick = 0;
	float speedFactor;

    const uint8_t ff_speed = 1; // чем выше этот параметр, тем короче переходы (градиенты) между цветами. 1 - это самое красивое
    const uint8_t ff_scale = 26; // чем больше этот параметр, тем больше "языков пламени" или как-то так. 26 - это норм

    bool whirlRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Відблиски на воді ================//
// Ідея SottNick
// переписані на програмні відблиски - (c)Kostyamat
// Генератор відблисків - (c)Stepko
class EffectAquarium : public EffectCalc
{
private:
    CRGBPalette16 currentPalette;
    const uint8_t _scale = 24;
    const uint8_t _speed = 3;

    float hue = 0.;
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint8_t noise[2][WIDTH + 1][HEIGHT + 1];
#define amountDrops ((HEIGHT + WIDTH) / 6)
    const uint8_t maxRadius = WIDTH + HEIGHT;
    uint8_t posX[amountDrops];
    uint8_t posY[amountDrops];
    float radius[amountDrops];
    uint8_t satur;
    uint8_t glare = 0;
    uint8_t iconIdx = 0;
    float speedFactor;

    void nGlare(uint8_t bri);
    void nDrops(uint8_t bri);
    void nTest(uint8_t bri);
    void wu(int16_t x, int16_t y);
    void fillNoise();

public:
    void load() override;
    String setDynCtrl(UIControl *_val) override;
    bool run(CRGB *ledarr, EffectWorker *opt = nullptr) override;
};

//===== Ефект Зірки ============================//
// https://github.com/marcmerlin/NeoMatrix-FastLED-IR/blob/master/Table_Mark_Estes_Impl.h
// https://github.com/marcmerlin/FastLED_NeoMatrix_SmartMatrix_LEDMatrix_GFX_Demos/blob/master/LEDMatrix/Table_Mark_Estes/Table_Mark_Estes.ino
// (c)Marc Merlin
// Адаптація (c)SottNick
#define STARS_NUM (16)
class EffectStar : public EffectCalc {
private:
    float driftx;
    float drifty;
    float cangle;
    float sangle;
    float radius2;
    uint8_t stars_count;
    float color[STARS_NUM] ;                        // цвет звезды
    uint8_t points[STARS_NUM] ;                       // количество углов в звезде
    unsigned int delay[STARS_NUM] ;                   // задержка пуска звезды относительно счётчика
    float counter = 0;                                // счетчик для реализации смещений, наростания и т.д.
	float speedFactor;
	float _speed;
    bool setup = true;
    uint8_t micPick = 0;
    const uint8_t spirocenterX = WIDTH / 2;
    const uint8_t spirocenterY = HEIGHT / 2;
    void drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler);
	String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Феєрверк =========================//
// https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373
// адаптований і переписаний Kostyamat
class EffectFireworks : public EffectCalc {
private:
    DOTS_STORE store;
    uint16_t launchcountdown[SPARK];
    byte dim;
    uint8_t valDim;
    uint8_t cnt;
    bool flashing = false;
    bool fireworksRoutine(CRGB *leds, EffectWorker *param);
    void sparkGen();
    Dot gDot[SPARK];
    Dot gSparks[NUM_SPARKS];
    String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Тихий Океан ======================//
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//  "Pacifica" перенос коду kostyamat
class EffectPacific : public EffectCalc {
private:
	uint32_t speedFactor;
	void pacifica_one_layer(const TProgmemRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
	void pacifica_deepen_colors();
	void pacifica_add_whitecaps();
	String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

#ifdef MIC_EFFECTS

//===== Ефект Осцилограф =======================// 
// (c) kostyamat
class EffectOsc : public EffectCalc {
private:
    byte oscHV;
    byte oscilLimit;
    float pointer;
    CRGB color;
    float div;
    byte gain;
    float y[2] = {0., 0.};
    String setDynCtrl(UIControl*_val) override;
    
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};
#endif

//===== Ефект Вишиванка ========================// 
// https://github.com/pixelmatix/aurora/blob/master/PatternMunch.h
// Copyright(c) 2014 Jason Coon
class EffectMunch : public EffectCalc {
private:
    byte count = 0;
    int8_t dir = 1;
    byte flip = 0;
    byte generation = 0;
    byte mic[2];
    byte rand;
    bool flag = false;
    uint8_t minDimLocal = maxDim > 32 ? 32 : 16;

    String setDynCtrl(UIControl*_val) override;
    bool munchRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Кольоровий шум ===================//
// (с) https://gist.github.com/StefanPetrick/c856b6d681ec3122e5551403aabfcc68
class EffectNoise : public EffectCalc {
private:

    uint8_t CentreX =  (WIDTH / 2) - 1;
    uint8_t CentreY = (HEIGHT / 2) - 1;
    uint32_t x[NUM_LAYERS];
    uint32_t y[NUM_LAYERS];
    uint32_t z[NUM_LAYERS];
    uint32_t scale_x[NUM_LAYERS];
    uint32_t scale_y[NUM_LAYERS];
    uint8_t  noise[NUM_LAYERS][WIDTH][HEIGHT];
	uint8_t speedFactor;
    bool type = false;

	String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Метелики =========================//
// (с) SottNick
class EffectButterfly : public EffectCalc {
private:
    float butterflysPosX[BUTTERFLY_MAX_COUNT];
    float butterflysPosY[BUTTERFLY_MAX_COUNT];
    float butterflysSpeedX[BUTTERFLY_MAX_COUNT];
    float butterflysSpeedY[BUTTERFLY_MAX_COUNT];
    float butterflysTurn[BUTTERFLY_MAX_COUNT];
    uint8_t butterflysColor[BUTTERFLY_MAX_COUNT];
    uint8_t butterflysBrightness[BUTTERFLY_MAX_COUNT];
    uint8_t deltaValue;
    uint8_t deltaHue;
    uint8_t hue;
    uint8_t hue2;
    byte step = 0;
    byte csum = 0;
    uint8_t cnt;
    bool wings = false;
    bool isColored = true;
	float speedFactor;
	String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Тіні =============================//
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/effects.ino
class EffectShadows : public EffectCalc {
private:
    uint16_t sPseudotime = 0;
    uint16_t sLastMillis = 0;
    uint16_t sHue16 = 0;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Візерунки ========================//
// Ідея https://github.com/vvip-68/GyverPanelWiFi/
// (c) kostyamat (Kostyantyn Matviyevskyy) 2020
// перероблено kDn
class EffectPatterns : public EffectCalc {
private:
    int8_t patternIdx = -1;
    int8_t lineIdx = 0;
    bool _subpixel = false;
    bool _sinMove = false;
    int8_t _speed = 1, _scale = 1;
    bool dir = false;
    byte csum = 0;
    byte _bri = 255U;
    byte buff[20 * 20];
    float xsin, ysin;
    unsigned long lastrun2;
    byte _sc = 0;
    float _speedX, _speedY;

    CHSV colorMR[12] = {
        CHSV(0, 0, 0),              // 0 - Black
        CHSV(HUE_RED, 255, 255),    // 1 - Red
        CHSV(HUE_GREEN , 255, 255),  // 2 - Green
        CHSV(HUE_BLUE, 255, 255),   // 3 - Blue
        CHSV(HUE_YELLOW, 255, 255), // 4 - Yellow
        CHSV(0, 0, 220),            // 5 - White
        CHSV(0, 255, 255),              // 6 - плавно меняеться в цикле (фон)
        CHSV(0, 255, 255),              // 7 - цвет равен 6 но +64
        CHSV(HUE_ORANGE, 255, 255),
        CHSV(HUE_PINK, 255, 255),
        CHSV(HUE_PURPLE, 255, 255),
        CHSV(HUE_AQUA, 255, 255),
    };

    String setDynCtrl(UIControl*_val) override;
    void drawPicture_XY();
    bool patternsRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Стрілки ==========================//
// https://github.com/vvip-68/GyverPanelWiFi/
class EffectArrows : public EffectCalc {
private:
    float arrow_x[4], arrow_y[4], stop_x[4], stop_y[4];
    byte arrow_direction;             // 0x01 - слева направо; 0x02 - снизу вверх; 0х04 - справа налево; 0х08 - сверху вниз
    byte arrow_mode, arrow_mode_orig; // 0 - по очереди все варианты
                                      // 1 - по очереди от края до края экрана;
                                      // 2 - одновременно по горизонтали навстречу к ентру, затем одновременно по вертикали навстречу к центру
                                      // 3 - одновременно все к центру
                                      // 4 - по два (горизонталь / вертикаль) все от своего края к противоположному, стрелки смещены от центра на 1/3
                                      // 5 - одновременно все от своего края к противоположному, стрелки смещены от центра на 1/3
    bool arrow_complete, arrow_change_mode;
    byte arrow_hue[4];
    byte arrow_play_mode_count[6];      // Сколько раз проигрывать полностью каждый режим если вариант 0 - текущий счетчик
    byte arrow_play_mode_count_orig[6]; // Сколько раз проигрывать полностью каждый режим если вариант 0 - исходные настройки
    uint8_t _scale;
    float speedFactor;
    void arrowSetupForMode(byte mode, bool change);
    void arrowSetup_mode1();
    void arrowSetup_mode2();
    //void arrowSetup_mode3(;)
    void arrowSetup_mode4();

    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Дикі кульки ======================//
// https://gist.github.com/bonjurroughs/9c107fa5f428fb01d484#file-noise-balls
class EffectNBals : public EffectCalc {
private:
    uint8_t lastSecond = 99;
    uint16_t speedy;// speed is set dynamically once we've started up
    uint16_t _scale;
    byte beat1, beat2 = 0;
    byte balls = 4;
    void balls_timer();
    void blur(CRGB *leds);
    bool nballsRoutine(CRGB *leds, EffectWorker *param);

    String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Притягування =====================//
// https://github.com/pixelmatix/aurora/blob/master/PatternAttract.h
// причесав kostyamat
class EffectAttract : public EffectCalc {
private:
    const uint8_t spirocenterX = WIDTH / 2;
    const uint8_t spirocenterY = HEIGHT / 2;
    float speedFactor;
    float mass;    // Mass, tied to size
    float G;       // Gravitational Constant
    uint8_t _mass = 127;
    uint8_t _energy = 127;
    static const uint8_t count = HEIGHT *2 - WIDTH /2;
    bool loadingFlag = true;
    byte csum = 0;
    //Boid boids[AVAILABLE_BOID_COUNT];
    Boid boids[count];
    PVector location;   // Location
    String setDynCtrl(UIControl*_val) override;
    void setup();

    PVector attract(Boid m) {
        PVector force = location - m.location;   // Calculate direction of force
        float d = force.mag();                              // Distance between objects
        d = constrain(d, 5.0f, 32.0f);                        // Limiting the distance to eliminate "extreme" results for very close or very far objects
        force.normalize();                                  // Normalize vector (distance doesn't matter here, we just want this vector for direction)
        float strength = (G * mass * m.mass) / (d * d);      // Calculate gravitional force magnitude
        force *= strength;                                  // Get force vector --> magnitude * direction
        return force;
    }

public:
    EffectAttract() {
        location = PVector(spirocenterX, spirocenterY);
        mass = 10;
        G = .5;
    }
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Змійки ===========================//
// варіант субпікселя і поведінка від kDn
class EffectSnake : public EffectCalc {
private:
    float hue;
    float speedFactor;
    int snakeCount;
    bool subPix = false;
    bool onecolor = false;
    enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Pixel
{
    float x;
    float y;
};

CRGB colors[SNAKE_LENGTH];
struct Snake
{
  float internal_counter = 0.0;
  float internal_speedf = 1.0;
  Pixel pixels[SNAKE_LENGTH];

  Direction direction;

  void newDirection()
  {
    switch (direction)
    {
    case UP:
    case DOWN:
      direction = random(0, 2) == 1 ? RIGHT : LEFT;
      break;

    case LEFT:
    case RIGHT:
      direction = random(0, 2) == 1 ? DOWN : UP;

    default:
      break;
    }
  };

  void shuffleDown(float speedy, bool subpix)
  {
    internal_counter+=speedy*internal_speedf;

    if(internal_counter>1.0){
        for (byte i = (byte)SNAKE_LENGTH - 1; i > 0; i--)
        {
            if(subpix)
                pixels[i] = pixels[i - 1];
            else {
                pixels[i].x = (uint8_t)pixels[i - 1].x;
                pixels[i].y = (uint8_t)pixels[i - 1].y;
            }
        }
        double f;
        internal_counter=modf(internal_counter, &f);
    }
  }

  void reset()
  {
    direction = UP;
    for (int i = 0; i < (int)SNAKE_LENGTH; i++)
    {
      pixels[i].x = 0;
      pixels[i].y = 0;
    }
  }

  void move(float speedy)
  {
    float inc = speedy*internal_speedf;

    switch (direction)
    {
    case UP:
      pixels[0].y = pixels[0].y >= HEIGHT ? inc : (pixels[0].y + inc);
      break;
    case LEFT:
      pixels[0].x = pixels[0].x >= WIDTH ? inc : (pixels[0].x + inc);
      break;
    case DOWN:
      pixels[0].y = pixels[0].y <= 0 ? HEIGHT - inc : pixels[0].y - inc;
      break;
    case RIGHT:
      pixels[0].x = pixels[0].x <= 0 ? WIDTH - inc : pixels[0].x - inc;
      break;
    }
  }

  void draw(CRGB colors[SNAKE_LENGTH], int snakenb, bool subpix, bool isDebug=false);
};

    Snake snakes[MAX_SNAKES];
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Nexus ============================//
// База паттерн "Змейка" з проекту Аврора, 
// Переписано kostyamat
#define NEXUS (WIDTH)

class EffectNexus: public EffectCalc {
  private:
    float dotPosX[NEXUS];
    float dotPosY[NEXUS];
    int8_t dotDirect[NEXUS];       // направление точки 
    CRGB dotColor[NEXUS];          // цвет точки
    float dotAccel[NEXUS];         // персональное ускорение каждой точки
    bool white = false;
    byte type = 1;
    uint8_t _scale = 1;
    bool randColor = false;
    float windProgress;
	float speedFactor;

    void reload();
    void resetDot(uint8_t idx);
    String setDynCtrl(UIControl*_val) override;

  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};

//===== Ефект Зміїний острів ===================//
// (c)SottNick
// адаптація і допороблення kostyamat
class EffectTest : public EffectCalc {
private:
//#define MAX_SNAKES    (WIDTH * 2)          // максимальное количество червяков
    uint8_t SnakeNum;                        // выбранное количество червяков
    long  snakeLast[MAX_SNAKES] ;            // тут будет траектория тела червяка
    float snakePosX[MAX_SNAKES];             // тут будет позиция головы
    float snakePosY[MAX_SNAKES];             // тут будет позиция головы
    float snakeSpeedX[MAX_SNAKES];           // тут будет скорость червяка
    float snakeSpeedY[MAX_SNAKES];           // тут будет дробная часть позиции головы
    //float snakeTurn[MAX_SNAKES];           //не пригодилось пока что
    uint8_t snakeColor[MAX_SNAKES];          // тут будет начальный цвет червяка
    uint8_t snakeDirect[MAX_SNAKES];         //тут будет направление червяка
	float speedFactor;

    String setDynCtrl(UIControl*_val) override;
    void regen();

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Попкорн ==========================//
// (C) Aaron Gotwalt (Soulmate)
// адаптація і дороблення kostyamat
class EffectPopcorn : public EffectCalc {
private:
    uint8_t numRockets = 10;
    bool blurred = false;
    bool revCol = false;
    //bool tiltDirec;
    float speedFactor;
    float center = (float)WIDTH / 2.;

    typedef struct
    {
        float x, y, xd, yd;
        byte hue;
    } Rocket;

    std::vector<Rocket> rockets;

    void restart_rocket(uint8_t r);
    void reload();

    String setDynCtrl(UIControl*_val) override;
    //void setscl(const byte _scl) override; // перегрузка для масштаба

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Полумняні мрії ===================//
// https://editor.soulmatelights.com/gallery/505
// (с)Stepko
#define WAVES_AMOUNT WIDTH
class EffectSmokeballs: public EffectCalc {
  private:
    uint8_t _scale = 1;
    uint16_t reg[WAVES_AMOUNT];
    uint16_t pos[WAVES_AMOUNT];
    float sSpeed[WAVES_AMOUNT];
    uint8_t maxMin[WAVES_AMOUNT];
    float speedFactor;
    uint8_t waveColors[WAVES_AMOUNT];
    void shiftUp();
    void regen();
    String setDynCtrl(UIControl*_val) override;
  public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Клітинки-ялинки ==================//
// Cell (C)Elliott Kember from Soulmate-IDE examples
// Spider, Spruce, Lines (c)stepko
// Flowering (c)Taras Yuzov
class EffectCell: public EffectCalc {
  private:
    const uint8_t Lines = 5;
	const bool glitch = abs((int)WIDTH-(int)HEIGHT) >= minDim/4;
	const byte density = 50;
    uint8_t Scale = 6;
    uint8_t _scale = 1;
    int16_t offsetX = 0;
    int16_t offsetY = 0;
    float x;
    uint8_t effId = 1;
    uint8_t hue;
    int16_t a;

	float speedFactor;
    void cell(CRGB *leds);
    void spider(CRGB *leds);
    void spruce(CRGB *leds);
    void vals(CRGB *leds);
    void flower(CRGB *leds);
    void lightBalls(CRGB *leds);
  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

//===== Ефект Тіксі Ленд =======================//
// https://github.com/owenmcateer/tixy.land-display
// (c)Martin Kleppe @aemkei
class EffectTLand: public EffectCalc {
  private:
    bool isSeq = false;
    byte animation = 0;
    bool ishue;
    bool ishue2;
    byte hue = 0;
    byte hue2 = 128;
    byte shift = 0;
    byte fine = 1;
    double t;
    void processFrame(CRGB *leds, double t, double x, double y);
    float code(double i, double x, double y);
    String setDynCtrl(UIControl*_val);
  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Осцилятор ========================//
// (c)SottNick
class EffectOscilator: public EffectCalc {
  private:
    uint8_t hue, hue2;                                 // постепенный сдвиг оттенка или какой-нибудь другой цикличный счётчик
    uint8_t deltaHue, deltaHue2;                       // ещё пара таких же, когда нужно много
    uint8_t step;                                      // какой-нибудь счётчик кадров или последовательностей операций
    uint8_t deltaValue;  
    unsigned long timer;

    class oscillatingCell {
        public:
        byte red;
        byte blue;
        byte green;
        byte color;
    };

    oscillatingCell oscillatingWorld[WIDTH][HEIGHT];

    void drawPixelXYFseamless(float x, float y, CRGB color);
    int redNeighbours(uint8_t x, uint8_t y);
    int blueNeighbours(uint8_t x, uint8_t y);
    int greenNeighbours(uint8_t x, uint8_t y);
    void setCellColors(uint8_t x, uint8_t y);
    //String setDynCtrl(UIControl*_val) override;
  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};

//===== Ефект Шторм ============================// 
// (с)kostyamat 1.12.2020
#define counts  (WIDTH*3)

class EffectWrain: public EffectCalc {
  private:
    static const uint8_t cloudHeight = HEIGHT / 5 + 1;
    float dotPosX[counts];
    float dotPosY[counts];
    float dotChaos;         // сила ветра
    int8_t dotDirect;       // направление ветра 
    byte dotColor[counts];  // цвет капли
    float dotAccel[counts]; // персональное ускорение каждой капли
    byte dotBri[counts];    // яркость капли
    bool clouds = false;
    bool storm = false;
    bool white = false;
    uint8_t _scale=1;
    byte type = 1;
    bool _flash;
    bool randColor = false;
    float windProgress;
	float speedFactor;
    uint8_t *_noise = (uint8_t *)malloc(WIDTH * cloudHeight);
    uint8_t *lightning = (uint8_t *)malloc(WIDTH * HEIGHT);  
    uint32_t timer = 0;
    

    void reload();
    String setDynCtrl(UIControl*_val) override;
    bool Lightning(uint16_t chanse);
    void Clouds(bool flash);

  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};

//===== За мотивами ефектів Particle System ====//
// https://github.com/fuse314/arduino-particle-sys
// https://github.com/giladaya/arduino-particle-sys
// https://www.youtube.com/watch?v=S6novCRlHV8&t=51s

//===== Ефекти Фея і Джерело ====================//
// (c) SottNick

#define trackingOBJECT_MAX_COUNT    (WIDTH * 3)  // максимальное количество отслеживаемых объектов (очень влияет на расход памяти)
#define enlargedOBJECT_MAX_COUNT    (WIDTH * 3) // максимальное количество сложных отслеживаемых объектов (меньше, чем trackingOBJECT_MAX_COUNT)

class EffectFairy : public EffectCalc {
private:
    float   trackingObjectPosX[trackingOBJECT_MAX_COUNT];
    float   trackingObjectPosY[trackingOBJECT_MAX_COUNT];
    float   trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
    float   trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
    float   trackingObjectShift[trackingOBJECT_MAX_COUNT];
    uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];
    float   trackingObjectState[trackingOBJECT_MAX_COUNT];
    bool    trackingObjectIsShift[trackingOBJECT_MAX_COUNT];
    uint8_t enlargedObjectNUM;                                       // используемое в эффекте количество объектов

    Boid boids[2];

    uint8_t hue;
    uint8_t hue2;
    uint8_t step;
    uint8_t deltaValue;
    uint8_t deltaHue;
    uint8_t deltaHue2;
    float speedFactor;
    byte type = false;
    byte blur;
    uint8_t _video = 255;
    uint8_t gain;

    void particlesUpdate(uint8_t i);
    void fairyEmit(uint8_t i);
    void fountEmit(uint8_t i);
    bool fairy(CRGB *leds);
    void fount(CRGB *leds);
    //void setscl(const byte _scl) override; // перегрузка для масштаба
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Бульбулятор ======================//
// https://editor.soulmatelights.com/gallery/11
// (C)Elliott Kember 
// перероблено і адаптовано kostyamat
#define NUMBER_OF_CIRCLES (NUM_LEDS / 16U)
class EffectCircles : public EffectCalc {
private:
    byte color;
    byte count;
    float speedFactor;
    byte _video = 255;
    byte gain;
    class Circle
    {
    public:
        //uint16_t offset;
        int16_t centerX;
        int16_t centerY;
        byte hue;
        float bpm = random(0, 255);

        void move() {
            centerX = random(0, WIDTH-1);
            centerY = random(0, HEIGHT-1);
        }
        
        void reset() {
            centerX = random(0, WIDTH-1);
            centerY = random(0, HEIGHT-1);
            hue = random(0, 255);
        }

        float radius() {
            float radius = EffectMath::fmap(triwave8(bpm), 0, 254, 0, 5); //beatsin16(bpm, 0, 500, 0, offset) / 100.0;
            return radius;
        }
    };

    Circle circles[NUMBER_OF_CIRCLES] = {};

    void drawCircle(CRGB *leds, Circle circle);
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;

}; 

//===== Ефект Бенгальські вогні ================//
// https://wokwi.com/arduino/projects/289797125785520649
// (c)Stepko
// 06.02.2021
class EffectBengalL : public EffectCalc {
private:
    #define sparksNum  WIDTH*4

    float sparksPos[2][sparksNum];
    float sparksSpeed[2][sparksNum];
    byte sparksColor[sparksNum];
    float sparksSat[sparksNum];
    float sparksFade[sparksNum];
    uint8_t gPos[2];

    bool centerRun = true;
    byte period = 10;
    byte _x = WIDTH/2;
    byte _y = HEIGHT/2;
    float speedFactor;

    void regen(byte id);
    void phisics(byte id);
    String setDynCtrl(UIControl*_val) override;


public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект кульки ===========================//
// https://wokwi.com/arduino/projects/289839434049782281
// (c) stepko and kostyamat
// 07.02.2021
class EffectBalls : public EffectCalc {
private:
#if WIDTH >= HEIGHT
    #define ballsAmount WIDTH
#else
    #define ballsAmount HEIGHT
#endif
    float ball[ballsAmount][4]; //0-PosY 1-PosX 2-SpeedY 3-SpeedX
    float radius[ballsAmount];
    bool rrad[ballsAmount];
    byte color[ballsAmount];
    const float radiusMax = (float)ballsAmount /5;

    float speedFactor;
    //void setspd(const byte _spd) override;
    String setDynCtrl(UIControl*_val) override;


public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Лабіринт =========================//
// solving algorithm by Stepko
class EffectMaze : public EffectCalc {
private:
    bool start = true;
	bool checkFlag;
	uint8_t posX, posY;
	uint8_t color;
	uint8_t Lookdir;
	uint8_t _speed;
	uint16_t SubPos;
	bool maze[M_WIDTH][M_HEIGHT];

	void digMaze(int x, int y);
	void generateMaze();

    String setDynCtrl(UIControl*_val) override;
    //void setspd(const byte _spd) override; // перегрузка для скорости
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;

}; 

//===== Ефект В'юнкі кольори ===================//
// https://wokwi.com/arduino/projects/283705656027906572
// (c) Stepko
class EffectFrizzles : public EffectCalc {
private:
    float _speed;
    float _scale;
    //String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Північне Сяйво ===================//
// (c) kostyamat 05.02.2021
// Велике дякую @Stepko за створені палітри спеціально для цього ефекту
static const TProgmemRGBPalette16 GreenAuroraColors_p FL_PROGMEM ={0x000000, 0x003300, 0x006600, 0x009900, 0x00cc00,0x00ff00, 0x33ff00, 0x66ff00, 0x99ff00,0xccff00, 0xffff00, 0xffcc00, 0xff9900, 0xff6600, 0xff3300, 0xff0000};
static const TProgmemRGBPalette16 BlueAuroraColors_p FL_PROGMEM ={0x000000, 0x000033, 0x000066, 0x000099, 0x0000cc,0x0000ff, 0x3300ff, 0x6600ff, 0x9900ff,0xcc00ff, 0xff00ff, 0xff33ff, 0xff66ff, 0xff99ff, 0xffccff, 0xffffff};
static const TProgmemRGBPalette16 NeonAuroraColors_p FL_PROGMEM ={0x000000, 0x003333, 0x006666, 0x009999, 0x00cccc,0x00ffff, 0x33ffff, 0x66ffff, 0x99ffff,0xccffff, 0xffffff, 0xffccff, 0xff99ff, 0xff66ff, 0xff33ff, 0xff00ff};

class EffectPolarL : public EffectCalc {
private:
    const byte numpalettes = 14;
    unsigned long timer;
    float adjastHeight;
    uint16_t adjScale;
    byte pal;
    uint16_t _scale;
    byte flag;
	byte _speed;

    //void setscl(const byte _scl) override;
    String setDynCtrl(UIControl*_val) override;
    void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
    void palettesload() override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Космо-Рейсер =====================//
// https://editor.soulmatelights.com/gallery/655
// (c)Stepko + Kostyamat
class EffectRacer: public EffectCalc {
private:
    uint16_t posX = random(0, WIDTH-1)*10;
    uint16_t posY = random(0, HEIGHT-1)*10;
    uint16_t aimX = random(0, WIDTH-1)*10;
    uint16_t aimY = random(0, HEIGHT-1)*10;
	int16_t deltaX, deltaY, signX, signY, error;
    float radius = 0;
    byte hue = millis()>>1; //random(0, 255);
    CRGB color;
    float speedFactor;
    float addRadius;
    float angle;
    byte starPoints = random(3, 7);

    const float _speed = (float)NUM_LEDS / 256; // Нормализация скорости для разных размеров матриц
    const float _addRadius = (float)NUM_LEDS / 4000;   // Нормализация скорости увеличения радиуса круга для разных матриц


    void aimChange();
    void drawStarF(float x, float y, float biggy, float little, int16_t points, float dangle, CRGB color);
    //void setspd(const byte _spd) override;
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Магма ============================//
// (c)SottNick 2021
// адаптація і доведення до розуму kostyamat
class EffectMagma: public EffectCalc {
private:
    //uint16_t ff_x;
    float ff_y, ff_z;                         // большие счётчики
    //control magma bursts
    const byte deltaValue = 6U; // 2-12 
    const byte deltaHue = 8U; // высота языков пламени должна уменьшаться не так быстро, как ширина
    const float Gravity = 0.1;
    uint8_t step, ObjectNUM = WIDTH; 
    uint8_t shiftHue[HEIGHT];
    float trackingObjectPosX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectPosY[enlargedOBJECT_MAX_COUNT];
    uint8_t trackingObjectHue[enlargedOBJECT_MAX_COUNT];
    float trackingObjectSpeedX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectShift[enlargedOBJECT_MAX_COUNT];
    float speedFactor;

    void palettesload();
    void regen();
    void LeapersMove_leaper(uint8_t l);
    void LeapersRestart_leaper(uint8_t l);
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Прапори ==========================//
// https://editor.soulmatelights.com/gallery/739-flags
// (c) Stepko + kostyamat
// 17.03.21
class EffectFlags: public EffectCalc {
private:
    const float DEVIATOR = 512. / WIDTH;
    float counter;
    uint8_t flag = 0;
    uint8_t _flag;
    uint8_t count;
    uint8_t _speed; // 1 - 16
    const uint8_t CHANGE_FLAG = 30; // >= 10 Autochange

    uint8_t thisVal;
    uint8_t thisMax;
	
	uint8_t mix(uint8_t a1, uint8_t a2, uint8_t l){
	return ((a1*l)+(a2*(255-l)))/255;}

    //Germany
    void germany(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(68, 255, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 255, thisVal)
            : CHSV(0, 0, thisVal / 2.5);
    }

    //Ukraine
    void ukraine(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax) ? CHSV(50, 255, thisVal) : CHSV(150, 255, thisVal);
    }

    //Belarus
    void belarus(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(0, 0, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 224, thisVal)
            : CHSV(0, 0, thisVal);
    }

    //Poland
    void poland(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax + 1) ? CHSV(248, 214, (float)thisVal * 0.83) : CHSV(25, 3, (float)thisVal * 0.91);
    }

    //The USA
    void usa(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) +=
            ((i <= WIDTH / 2) && (j + thisMax > HEIGHT - 1 + HEIGHT / 16)) ? 
            ((i % 2 && ((int)j - HEIGHT / 16 + thisMax) % 2) ? 
            CHSV(160, 0, thisVal) : CHSV(160, 255, thisVal)) 
            : ((j + 1 + thisMax) % 6 < 3 ? CHSV(0, 0, thisVal) : CHSV(0, 255, thisVal));
    }

    //Italy
    void italy(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(90, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
    }

    //France
    void france(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(160, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
    }

    //UK
    void uk(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (
                (
                    (i > WIDTH / 2 + 1 || i < WIDTH / 2 - 2) && ((i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -2) && (i - (j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 2))
                )
                    ||
                (
                    (i > WIDTH / 2 + 1 || i < WIDTH / 2 - 2) && ( (((int)WIDTH - 1 - i - ((int)j + thisMax - (int)(HEIGHT * 2 - WIDTH) / 2) > -2) && (WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 2)) )
                )
            || 
            (WIDTH / 2 - i == 0) || (WIDTH / 2 - 1 - i == 0) 
            || 
            ((HEIGHT - (j + thisMax)) == 0) || ((HEIGHT - 1 - (j + thisMax)) == 0)) ? 
            CHSV(0, 255, thisVal) 
            : 
            (((i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -4) 
            && (i - (j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 4)) 
            || 
            (((int)WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -4) 
            && (WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 4)) 
            || (WIDTH / 2 + 1 - i == 0) || (WIDTH / 2 - 2 - i == 0) 
            || (HEIGHT + 1 - (j + thisMax) == 0) || (HEIGHT - 2 - (int)(j + thisMax) == 0)) ? 
            CHSV(0, 0, thisVal) : CHSV(150, 255, thisVal);
    }

    //Spain
    void spain(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 3) ? 
            CHSV(250, 224, (float)thisVal * 0.68) : (j < thisMax + HEIGHT / 3) ? CHSV(64, 255, (float)thisVal * 0.98)
            : CHSV(250, 224, (float)thisVal * 0.68);
    }


    void changeFlags();
    String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Зоряний десант ===================//
// Starship Troopers https://editor.soulmatelights.com/gallery/839-starship-troopers
// Based on (c) stepko`s codes https://editor.soulmatelights.com/gallery/639-space-ships
// reworked (c) kostyamat (subpixel, shift speed control, etc) 08.04.2021
class EffectStarShips: public EffectCalc {
private:
    byte _scale = 8;
    const byte DIR_CHARGE = 2; // Chance to change direction 1-5
    uint16_t chance = 4096;
	
    byte _dir;
    uint8_t _fade;
	int8_t zF;int8_t zD;
	float speedFactor;
	
	void MoveX(uint8_t am, int8_t amplitude, float shift);
	void MoveY(uint8_t am, int8_t amplitude, float shift);
    void draw(float x, float y, CRGB color);
    String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};
#ifdef MIC_EFFECTS

//===== Ефект Частотний аналізатор =============//
/* (c) G6EJD, https://www.youtube.com/watch?v=OStljy_sUVg&t=0s
   reworked by s-marley https://github.com/s-marley/ESP32_FFT_VU
   adopted for FireLamp_EmbUI by kostyamat, kDn
   reworked and updated (c) kostyamat 24.04.2021 */
class EffectVU: public EffectCalc {
private:
    CRGBPalette16 gradPal[5] = { 
    purple_gp,    rainbowsherbet_gp, 
    redyellow_gp, Colorfull_gp, es_ocean_breeze_068_gp
    };
    uint8_t NUM_BANDS = WIDTH;
    uint8_t BAR_WIDTH =  (WIDTH  / (NUM_BANDS - 1));
    uint8_t calcArray = 1; // уменьшение частоты пересчета массива
    uint8_t colorTimer = 0;
    const uint8_t colorDev = 256/TOP;
    // Sampling and FFT stuff
    float peak[WIDTH];              // The length of these arrays must be >= NUM_BANDS
    float oldBarHeights[WIDTH];
    float bandValues[WIDTH];

    float samp_freq;
    double last_freq = 0;
    uint8_t last_min_peak, last_max_peak;
    float maxVal;
    float threshold;
    byte tickCounter;
    byte colorType;

    float amplitude = 1.0;
    int effId = 0;
    bool type = false;
    bool colorShifting = false;
    const float speedFactorVertical = (float)HEIGHT / 16;
    bool averaging = true;

    String setDynCtrl(UIControl*_val) override;
    void horizontalColoredBars(uint8_t band, float barHeight, uint8_t type = 0, uint8_t colorShift = 0);
    void paletteBars(uint8_t band, float barHeight, CRGBPalette16& palette, uint8_t colorShift = 0);
    void verticalColoredBars(uint8_t band, float barHeight, uint8_t type = 0, uint8_t colorShift = 0);
    void centerBars(uint8_t band, float barHeight, CRGBPalette16& palette, uint8_t colorShift = 0);
    void whitePeak(uint8_t band);
    void outrunPeak(uint8_t band, CRGBPalette16& palette, uint8_t colorShift = 0);
    void waterfall(uint8_t band, uint8_t barHeight);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};
#endif

//===== Ефект Вогняна лампа ====================//
// https://editor.soulmatelights.com/gallery/546-fire
// (c) Stepko 17.06.21
// sparks (c) kostyamat 10.01.22 https://editor.soulmatelights.com/gallery/1619-fire-with-sparks
class EffectFire2021 : public EffectCalc {
private:
    byte _pal = 8;
    byte _scale = 32;
	byte speedFactor;
    uint32_t t;
    bool withSparks = false;

    const uint8_t sparksCount = WIDTH / 4;
    const uint8_t spacer = HEIGHT/4;

    class Spark {
    private:
        CRGB color;
        uint8_t Bri;
        uint8_t Hue;
        float x, y, speedy = 1;
    
    public:
        void addXY(float nx, float ny) {
            EffectMath::drawPixelXYF(x, y, 0);
            x += nx;
            y += ny * speedy;
        }

        float getY() {
        return y;
        }

        void reset() {
            uint32_t peak = 0;
            speedy = (float)random(5, 30) / 10;
            y = random((HEIGHT/4) * 5, (HEIGHT /2) * 5) / 5;
            for (uint8_t i=0; i < WIDTH; i++) {
                uint32_t temp = EffectMath::RGBweight(leds, getPixelNumber(i, y));
                if (temp > peak) {
                    x = i;
                    peak = temp;
                }
            }
            color = EffectMath::getPixel(x, y);
        }

        void draw() {
            color.fadeLightBy(256 / (HEIGHT));
            EffectMath::drawPixelXYF(x, y, color);
        }
    }; 

    std::vector<Spark> sparks;

    String setDynCtrl(UIControl*_val) override;
    void palettesload() override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект П'ятнашки ========================//
// https://editor.soulmatelights.com/gallery/1471-puzzles-subpixel
// (c) Stepko 10.12.21
class EffectPuzzles : public EffectCalc {
private:
    byte PSizeX = 4;
    byte PSizeY = 4;
    float Ecols = (WIDTH / PSizeX);
    float Erows = (HEIGHT / PSizeY);
    bool Ca = (WIDTH % PSizeX)? 1 : 0;
    bool Ra = (HEIGHT % PSizeY)? 1 : 0;
    uint8_t PCols = round(Ecols) + Ca;
    uint8_t PRows = round(Erows) + Ra;
    float speedFactor = 0.1;
    std::vector<std::vector<uint8_t>> puzzle;
    byte color;
    byte z_dot[2];
    byte step;
    int8_t move[2];
    float shift[2] = { 0, 0 };
    bool XorY;

    void draw_square(byte x1, byte y1, byte x2, byte y2, byte col);
    void draw_squareF(float x1, float y1, float x2, float y2, byte col);
    void regen();

    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Кольорові драже ==================//
// (c)stepko																									   
class EffectPile : public EffectCalc {
private:
    uint8_t pcnt = 0U, _scale;
    String setDynCtrl(UIControl*_val) override;
    void changeFrame();
    byte F[2][WIDTH][HEIGHT];
    int shift;
public:
    void load() override;
    
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект ДНК ==============================//
// (c) Stepko
// https://editor.soulmatelights.com/gallery/1520-dna
class EffectDNA : public EffectCalc {
private:
    float a = (256.0 / (float)WIDTH);
    float t = 0.0;
    float speedFactor = 0.5;
    bool flag = true; 
    bool bals = false;
    uint8_t type = 0, _type = 1, _scale = 16;
    
    String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Дим ==============================//
// based on code by @Stepko (c) 23/12/2021
class EffectSmoker : public EffectCalc {
private:
    byte color, saturation;
    byte _scale = 30, glitch;
	float speedFactor;
    float t;

    String setDynCtrl(UIControl*_val) override;

public:

    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Міраж ============================//
// based on cod by @Stepko (c) 23/12/2021
class EffectMirage : public EffectCalc {
private:
    const float div = 10.;
    const uint16_t width = (WIDTH - 1) * div, height = HEIGHT * div;
    uint16_t _speed;
    byte color;
    bool colorShift = false;
    byte buff[WIDTH + 2][HEIGHT + 2];
    bool a = true;
	float speedFactor;

    String setDynCtrl(UIControl*_val) override;
    void drawDot(float x, float y, byte a);
    void blur();

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Акварель =========================//
// https://editor.soulmatelights.com/gallery/1587-oil
// (c) kostyamat 26.12.2021
#define BLOT_SIZE WIDTH/2
class EffectWcolor : public EffectCalc {
private:
    float speedFactor;
    uint8_t bCounts = 1;
    uint8_t blur;
    bool mode = false;
    float t;

    class Blot {
    private:
        byte hue, sat;
        float bri;
        int x0, y0;
        float x[BLOT_SIZE]; 
        float y[BLOT_SIZE];

    public:
        void appendXY(float nx, float ny) {
            for (byte i = 0; i < BLOT_SIZE; i++) {
                x[i] += nx;
                y[i] += ny; 
            }
        }
        
        void reset(byte num, byte Counts) {  
            x0 = random(-5, WIDTH - 5);
            float y0 = EffectMath::randomf(-1, HEIGHT+1);
            uint8_t dy;
            for (uint8_t i = 0; i < BLOT_SIZE; i++) {
                bool f = random(0,2);
                dy = random(0, 2); 
                x[i] = x0 + i;
                if (f)
                y[i] = float((i ? y[i-1] : y0) + dy);
                else 
                y[i] = float((i ? y[i-1] : y0) - dy);
            }
            hue = random(0, 256);
            sat = random(160, 256);
            bri = random(128, 256);    
        }

        double getY() {
            double result = y[0];
            for (uint8_t i = 1; i < BLOT_SIZE; i++) {
                if (y[i] > result) result = y[i];
            }
            return result;
        }

        void drawing() {
            for (uint8_t i = 0; i < BLOT_SIZE; i++) {
                byte bright = constrain((float)bri / (float)HEIGHT * (y[i] + HEIGHT - y0), 32, 255);
                if (y[i] > -0.1)
                    EffectMath::drawPixelXYF(x[i], y[i], CHSV(hue, sat, bright), 0);
            }
        }
    };

    std::vector<Blot> blots;

    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Неопалима купина =================//
// https://editor.soulmatelights.com/gallery/1570-radialfire
// (c) Stepko and Sutaburosu
// 23.12.21
class EffectRadialFire : public EffectCalc {
private:
    const int8_t C_X = (WIDTH / 2);
	const int8_t C_Y = (HEIGHT / 2);
    uint16_t XY_angle[maxDim][maxDim];
    uint8_t XY_radius[maxDim][maxDim];
    float t;
    float speedFactor;
    uint8_t _scale;
    bool mode = false;

    String setDynCtrl(UIControl*_val) override;
    // void palettesload();

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Нікола Тесла =====================//
// (c)Stepko
class EffectSplashBals : public EffectCalc {
private:
    uint8_t count = 3;
    uint8_t hue;
    const uint8_t DEV = 5;
    const float R = (float)NUM_LEDS/128;

    float x[6];
    float y[6];
    uint8_t iniX[6];
    uint8_t iniY[6];

    float speedFactor;

    float dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
        float dist;
        int a = y2 - y1;
        int b = x2 - x1;
        a *= a;
        b *= b;
        a += b;
        dist = EffectMath::sqrt(a);
        return dist;
    };
    
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//===== Ефект Мильні розводи ===================//
// https://editor.soulmatelights.com/gallery/1626-soap
// (c)Stepko
class EffectFlower : public EffectCalc {
	private:
        uint16_t mov;
        int8_t zD;
        int8_t zF;
        uint8_t noise3d[WIDTH][HEIGHT];
        uint32_t noise32_x;
        uint32_t noise32_y;
        uint32_t noise32_z;
        uint32_t scale32_x;
        uint32_t scale32_y;
        uint8_t noisesmooth;

        void NoiseFill();
        void MoveFractionalNoiseX(int8_t amplitude = 1, float shift = 0);
        void MoveFractionalNoiseY(int8_t amplitude = 1, float shift = 0);
	public:
        void load() override;
        bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

#ifdef RGB_PLAYER

//===== Програвач 332/556 файлів ===============//
// https://editor.soulmatelights.com/gallery/1684-pgm-player-with-resize
// (c) Kostyantyn Matviyevskyy aka kostyamat, file format and decoder\encoder (c) Stepko and Sutaburosu
// 27.01.2022
// License GPL v.3 as a part of the FireLamp_EmbUI project
#define MULTIPLIC 256
class EffectPlayer : public EffectCalc {
    private:
        uint8_t frameWidth, frameHeight, frames, frame = 0; 
        uint16_t maxSize; 
        int16_t corrX, corrY;
        uint16_t resizeX, resizeY;
        uint8_t* frameBuf = nullptr;
        uint8_t frameDelay;
        bool codec332 = true;
        File rgbFile;
        uint8_t bufSize = 0;
        bool blur;

        void calc();
        void getFromFile_332(uint8_t frame);
        void getFromFile_565(uint8_t frame);
        void drawFrame();
        bool loadFile(String filename);
        String setDynCtrl(UIControl*_val) override;
    
    public:
        void load() override;
        bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
        ~EffectPlayer() {
            delete [] frameBuf;
            frameBuf = nullptr;
        }
};
#endif

#endif
