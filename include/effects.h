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

#ifndef _EFFECTS_H
#define _EFFECTS_H

#include "color_palette.h"
#include "effectworker.h"
#include "effectmath.h"


const byte maxDim = max(WIDTH, HEIGHT);
const byte minDim = min(WIDTH, HEIGHT);
const byte width_adj = (WIDTH < HEIGHT ? (HEIGHT - WIDTH) /2 : 0);
const byte height_adj = (HEIGHT < WIDTH ? (WIDTH - HEIGHT) /2: 0);


//-------------- Специально обученный пустой эффект :)
class EffectNone : public EffectCalc {
private:
    void load() override { FastLED.clear(); };
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override {return true;};
};

//-------------- Эффект "Часы"
class EffectTime : public EffectCalc {
private:
    bool timeShiftDir; // направление сдвига
    float curTimePos; // текущая позиция вывода
    float color_idx; // индекс цвета
    CRGB hColor[1]; // цвет часов и минут
    CRGB mColor[1]; // цвет часов и минут
    uint32_t lastrun=0;     /**< счетчик времени для эффектов с "задержкой" */
    bool isMinute=false;

    bool timePrintRoutine(CRGB *leds, EffectWorker *param);
    bool palleteTest(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

/*
 ***** METABALLS / МЕТАСФЕРЫ *****
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

// ***** SINUSOID3 / СИНУСОИД3 *****
/*
  Sinusoid3 by Stefan Petrick
  read more about the concept: https://www.youtube.com/watch?v=mubH-w_gwdA
*/
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



//----- Эффект "Прыгающие Мячики"
// перевод на субпиксельную графику kostyamat
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

// ------------- Эффект "Пейнтбол" -------------
class EffectLightBalls : public EffectCalc {
private:
	#define BORDERTHICKNESS       (1U)   // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)
	const uint8_t paintWidth = WIDTH - BORDERTHICKNESS * 2;
	const uint8_t paintHeight = HEIGHT - BORDERTHICKNESS * 2;
	float speedFactor;
	
	String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------- Эффект "Пульс"
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

// ------------- эффект "Блуждающий кубик" -------------
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
    void load();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// -------- Эффект "Светлячки со шлейфом"
#define _AMOUNT 16U
class EffectLighterTracers : public EffectCalc {
private:
    uint8_t cnt = 1;
    float vector[_AMOUNT][2U];
    float coord[_AMOUNT][2U];
    int16_t ballColors[_AMOUNT];
    byte light[_AMOUNT];
    float speedFactor;
    bool lighterTracersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

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

// ------ Эффект "Белая Лампа"
class EffectWhiteColorStripe : public EffectCalc {
private:
    uint8_t shift=0;
    bool whiteColorStripeRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Конфетти"
class EffectSparcles : public EffectCalc {
private:
    uint8_t eff = 1;
    bool sparklesRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

// ========== Эффект "Эффектопад"
// совместное творчество юзеров форума community.alexgyver.ru
class EffectEverythingFall : public EffectCalc {
private:
    byte heat[WIDTH][HEIGHT];

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= FIRE 2012 /  ОГОНЬ 2012 ===============
// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
/*
 * Эффект "Огонь 2012"
 */
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

// ------------- класс Светлячки -------------
// нужен для некоторых эффектов
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

// ------------- Эффект "New Матрица" ---------------
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

// ------------- звездопад/метель -------------
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

// ----------- Эффекты "Лава, Зебра, etc"
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
    #if (WIDTH > HEIGHT)
    uint8_t noise[2*HEIGHT][WIDTH];
    #else
    uint8_t noise[2*WIDTH][HEIGHT];
    #endif

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

// ***** Эффект "Спираль"     ****
/*
 * Aurora: https://github.com/pixelmatix/aurora
 * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
 * Copyright (c) 2014 Jason Coon
 * Неполная адаптация SottNick
 */
class EffectSpiro : public EffectCalc {
private:
  const uint8_t spiroradiusx = WIDTH /4; //((!WIDTH & 1) ? (WIDTH -1) : WIDTH) / 4;
  const uint8_t spiroradiusy = HEIGHT /4;//(!(HEIGHT & 1) ? (HEIGHT-1) : HEIGHT) / 4;

  const uint8_t spirocenterX = WIDTH /2; //(!(WIDTH & 1) ? (WIDTH -1) : WIDTH) / 2;
  const uint8_t spirocenterY = HEIGHT /2; //(!(HEIGHT & 1) ? (HEIGHT-1) : HEIGHT) / 2;

  const uint8_t spirominx = spirocenterX - spiroradiusx;
  const uint8_t spiromaxx = spirocenterX + spiroradiusx - (WIDTH%2 == 0 ? 1:0);// + 1;
  const uint8_t spirominy = spirocenterY - spiroradiusy;
  const uint8_t spiromaxy = spirocenterY + spiroradiusy - (HEIGHT%2 == 0 ? 1:0); // + 1;

  bool spiroincrement = false;
  bool spirohandledChange = false;
  float spirohueoffset = 0;
  uint8_t spirocount = 1;
  float spirotheta1 = 0;
  float spirotheta2 = 0;
  uint8_t internalCnt = 0;
  float speedFactor;

  String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= ЭФФЕКТ ПРИЗМАТА ===============
// Prismata Loading Animation
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

// ============= ЭФФЕКТ СТАЯ ===============
// Адаптация от (c) SottNick
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

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
// ***** Парящий огонь, Кровавые Небеса, Радужный Змей и т.п.
// базис (c) Stefan Petrick
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

// ============= SWIRL /  ВОДОВОРОТ ===============
// https://gist.github.com/kriegsman/5adca44e14ad025e6d3b
// Copyright (c) 2014 Mark Kriegsman
class EffectSwirl : public EffectCalc {
private:
    bool swirlRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= DRIFT / ДРИФТ ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h
class EffectDrift : public EffectCalc {
private:
	const byte maxDim_steps = 256 / max(WIDTH, HEIGHT);
	uint8_t dri_phase;
	float _dri_speed;
	uint8_t _dri_delta;
	byte driftType = 0;

	String setDynCtrl(UIControl*_val) override;
	bool incrementalDriftRoutine(CRGB *leds, EffectWorker *param);
	bool incrementalDriftRoutine2(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------------------------ ЭФФЕКТ МЕРЦАНИЕ ----------------------
// (c) SottNick
class EffectTwinkles : public EffectCalc {
private:
  uint8_t thue = 0U;
  uint8_t tnum;
  CRGB ledsbuff[NUM_LEDS];
  float speedFactor;
  bool twinklesRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
  //void setscl(const byte _scl) override;
public:
    void load() override;
    void setup();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

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

// ============= RADAR / РАДАР ===============
// Aurora : https://github.com/pixelmatix/aurora/blob/master/PatternRadar.h
// Copyright(c) 2014 Jason Coon
class EffectRadar : public EffectCalc {
private:
    float eff_offset; 
    float eff_theta;  // глобальная переменная угла для работы эффектов
    bool subPix = false;
    byte hue;
    const float width_adj_f = (float)(WIDTH < HEIGHT ? (HEIGHT - WIDTH) / 2. : 0);
    const float height_adj_f= (float)(HEIGHT < WIDTH ? (WIDTH - HEIGHT) / 2. : 0);
    bool radarRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl *_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

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

// -------------- Эффект "Кодовый замок"
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

// ------------------------------ ЭФФЕКТ КУБИК 2D ----------------------
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

// ------ Эффекты "Пикассо"
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
    /*
    bool picassoRoutine2(CRGB *leds, EffectWorker *param);
    bool picassoRoutine3(CRGB *leds, EffectWorker *param);
    */
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

// ------ Эффект "Прыгуны" (c) obliterator
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

// ------ Эффект "Лавовая Лампа"
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
    float buff2[WIDTH][HEIGHT];
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

// ------- Эффект "Вихри"
// Based on Aurora : https://github.com/pixelmatix/aurora/blob/master/PatternFlowField.h
// Copyright(c) 2014 Jason Coon
//адаптация SottNick
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

// ------------- Эффект "Блики на воде Цвета"
// Идея SottNick
// переписал на программные блики + паттерны - (c) kostyamat
// Генератор бликов (c) stepko
class EffectAquarium : public EffectCalc {
private:
#define MAX_DIMENSION max(WIDTH, HEIGHT)
    CRGBPalette16 currentPalette;
    const uint8_t _scale = 25;
    const uint8_t _speed = 3;

    float hue = 0.;
    uint16_t x;
    uint16_t y;
    uint16_t z;
    byte ledbuff[WIDTH*2 * HEIGHT*2];
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
    void fillNoiseLED(CRGB *leds);

public:
    void load() override;
    String setDynCtrl(UIControl*_val) override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};


// ----------- Эффект "Звезды"
// (c) SottNick
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

//---------- Эффект "Фейерверк"
//адаптация и переписал - kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373
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

// ------------ Эффект "Тихий Океан"
//  "Pacifica" перенос кода kostyamat
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
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
//----- Эффект "Осциллограф" 
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
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};
#endif

// ------ Эффект "Вышиванка" 
// (с) проект Aurora "Munch"
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

// ------ Эффект "Цветной шум" 
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

// ---- Эффект "Мотыльки"
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

// ---- Эффект "Тени"
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/effects.ino
class EffectShadows : public EffectCalc {
private:
    uint16_t sPseudotime = 0;
    uint16_t sLastMillis = 0;
    uint16_t sHue16 = 0;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Узоры"
// (c) kostyamat (Kostyantyn Matviyevskyy) 2020
// переделано kDn
// идея отсюда https://github.com/vvip-68/GyverPanelWiFi/
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


// ***************************** "Стрелки" *****************************
// взято отсюда https://github.com/vvip-68/GyverPanelWiFi/
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

// ------ Эффект "Дикие шарики"
// (с) https://gist.github.com/bonjurroughs/9c107fa5f428fb01d484#file-noise-balls
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

// ------ Эффект "Притяжение"
// project Aurora
// доведено до ума - kostyamat
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

//------------ Эффект "Змейки"
// вариант субпикселя и поведения от kDn
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

//------------ Эффект "Nexus"
// База паттерн "Змейка" из проекта Аврора, 
// перенос и переписан - kostyamat
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

// ----------- Эфеект "Змеиный Остров"
// (c) Сотнег
// адаптация и доработки kostyamat
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

// ----------- Эфеект "Попкорн"
// (C) Aaron Gotwalt (Soulmate)
// адаптация и доработки kostyamat
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

//-------- Эффект "Детские сны"
// (c) Stepko https://editor.soulmatelights.com/gallery/505
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

// ----------- Эффект "Ёлки-Палки"
// "Cell" (C) Elliott Kember из примеров программы Soulmate
// Spaider и Spruce (c) stepko
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

  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

// ---------- Эффект "Тикси Ленд"
// (c)  Martin Kleppe @aemkei, https://github.com/owenmcateer/tixy.land-display
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

// ----------- Эффект "Осцилятор"
// (c) Сотнег (SottNick)
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

//------------ Эффект "Шторм" 
// (с) kostyamat 1.12.2020
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

//-------- по мотивам Эффектов Particle System -------------------------
// https://github.com/fuse314/arduino-particle-sys
// https://github.com/giladaya/arduino-particle-sys
// https://www.youtube.com/watch?v=S6novCRlHV8&t=51s
//при попытке вытащить из этой библиотеки только минимально необходимое выяснилось, что там очередной (третий) вариант реализации субпиксельной графики.
//ну его нафиг. лучше будет повторить визуал имеющимися в прошивке средствами.

// ============= ЭФФЕКТ Фея/Источник ===============
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

// ---------- Эффект "Бульбулятор"
// "Circles" (C) Elliott Kember https://editor.soulmatelights.com/gallery/11
// адаптация и переделка - kostyamat
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

// ----------- Эффект "Бенгальские Огни"
// (c) stepko https://wokwi.com/arduino/projects/289797125785520649
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

// ----------- Эффект "Шары"
// (c) stepko and kostyamat https://wokwi.com/arduino/projects/289839434049782281
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

// ---------- Эффект-игра "Лабиринт"
class EffectMaze : public EffectCalc {
private:
    const uint16_t maxSolves = MAZE_WIDTH * MAZE_WIDTH * 5;
    char *maze = (char*)malloc(MAZE_WIDTH * MAZE_HEIGHT * sizeof(char));
    int8_t playerPos[2];
    uint32_t labTimer;
    bool mazeMode = false;
    bool mazeStarted = false;
    uint8_t hue;
    CRGB color;
    CRGB playerColor = CRGB::White;

    bool loadingFlag = true;
    bool gameOverFlag = false;
    bool gameDemo = true;
    bool gamePaused = false;
    bool track = random8(0,2);  // будет ли трек игрока
    uint8_t buttons;

    unsigned long timer = millis(), gameTimer = 200;         // Таймер скорости игр

    void newGameMaze();
    void buttonsTickMaze();
    void movePlayer(int8_t nowX, int8_t nowY, int8_t prevX, int8_t prevY);
    void demoMaze();
    bool checkPath(int8_t x, int8_t y);
    void CarveMaze(char *maze, int width, int height, int x, int y);
    void GenerateMaze(char *maze, int width, int height);
    void SolveMaze(char *maze, int width, int height);

    bool checkButtons()
    {
        if (buttons != 4)
            return true;
        return false;
    }

    String setDynCtrl(UIControl*_val) override;
    //void setspd(const byte _spd) override; // перегрузка для скорости
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;

}; 

// --------- Эффект "Вьющийся Цвет"
// (c) Stepko https://wokwi.com/arduino/projects/283705656027906572
class EffectFrizzles : public EffectCalc {
private:
    float _speed;
    float _scale;
    //String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// --------- Эффект "Северное Сияние"
// (c) kostyamat 05.02.2021
// идеи подсмотрены тут https://www.reddit.com/r/FastLED/comments/jyly1e/challenge_fastled_sketch_that_fits_entirely_in_a/

// Палитры, специально созданные под этот эффект, огромная благодарность @Stepko
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

// --------- Эффект "Космо-Гонщик"
// (c) Stepko + kostyamat https://editor.soulmatelights.com/my-patterns/655
class EffectRacer: public EffectCalc {
private:
    float posX = random(0, WIDTH-1);
    float posY = random(0, HEIGHT-1);
    uint8_t aimX = random(0, WIDTH)-1;
    uint8_t aimY = random(0, HEIGHT-1);
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

// ----------------- Эффект "Магма"
// (c) Сотнег (SottNick) 2021
// адаптация и доводка до ума - kostyamat
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

// ------------- Эффект "Флаги"
// (c) Stepko + kostyamat
// 17.03.21
// https://editor.soulmatelights.com/gallery/739-flags
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

    //Germany
    void germany(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(68, 255, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 255, thisVal)
            : CHSV(0, 0, thisVal / 2.5);
        }
    }

    //Ukraine
    void ukraine(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (j < thisMax) ? CHSV(50, 255, thisVal) : CHSV(150, 255, thisVal);
        }
    }

    //Belarus
    void belarus(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(0, 224, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 0, thisVal)
            : CHSV(0, 224, thisVal);
        }
    }

    //Poland
    void poland(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (j < thisMax + 1) ? CHSV(248, 214, (float)thisVal * 0.83) : CHSV(25, 3, (float)thisVal * 0.91);
        }
    }

    //The USA
    void usa(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) +=
            ((i <= WIDTH / 2) && (j + thisMax > HEIGHT - 1 + HEIGHT / 16)) ? 
            ((i % 2 && ((int)j - HEIGHT / 16 + thisMax) % 2) ? 
            CHSV(160, 0, thisVal) : CHSV(160, 255, thisVal)) 
            : ((j + 1 + thisMax) % 6 < 3 ? CHSV(0, 0, thisVal) : CHSV(0, 255, thisVal));
        }
    }

    //Italy
    void italy(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(90, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
        }
    }

    //France
    void france(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(160, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
        }
    }

    //UK
    void uk(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
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
            CHSV(0, 0, thisVal)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  : CHSV(150, 255, thisVal);
        }
    }

    //Spain
    void spain(uint8_t i)
    {
        for (uint8_t j = 0; j < HEIGHT; j++)
        {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 3) ? 
            CHSV(250, 224, (float)thisVal * 0.68) : (j < thisMax + HEIGHT / 3) ? CHSV(64, 255, (float)thisVal * 0.98)
            : CHSV(250, 224, (float)thisVal * 0.68);
        }
    }


    void changeFlags();
    String setDynCtrl(UIControl*_val) override;

public:
    //void load () override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// --------------------- Эффект "Звездный Десант"
// Starship Troopers https://editor.soulmatelights.com/gallery/839-starship-troopers
// Based on (c) stepko`s codes https://editor.soulmatelights.com/gallery/639-space-ships
// reworked (c) kostyamat (subpixel, shift speed control, etc) 08.04.2021
class EffectStarShips: public EffectCalc {
private:
    byte _scale = 8;
    const byte DIR_CHARGE = 2; // Chance to change direction 1-5
    uint16_t chance = 4096;

    byte dir = 3;
    byte _dir;
    byte count = 0;
    uint8_t _fade;

	float speedFactor;

    void draw(float x, float y, CRGB color);
    String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};
#ifdef MIC_EFFECTS
/* -------------- эффект "VU-meter"
    (c) G6EJD, https://www.youtube.com/watch?v=OStljy_sUVg&t=0s
    reworked by s-marley https://github.com/s-marley/ESP32_FFT_VU
    adopted for FireLamp_jeeUI by kostyamat, kDn
    reworked and updated (c) kostyamat 24.04.2021
*/
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

// ----------- Эффект "Огненная лампа"
// https://editor.soulmatelights.com/gallery/546-fire
// (c) Stepko 17.06.21
// sparks (c) kostyamat 10.01.2022 https://editor.soulmatelights.com/gallery/1619-fire-with-sparks
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

// ----------- Эффект "Пятнашки"
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

// ============= Эффект Цветные драже ===============
// (c) SottNick
//по мотивам визуала эффекта by Yaroslaw Turbin 14.12.2020
//https://vk.com/ldirko программный код которого он запретил брать
class EffectPile : public EffectCalc {
private:
    uint8_t pcnt = 0U, _scale;
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= Эффект ДНК ===============
// (c) Stepko
// https://editor.soulmatelights.com/gallery/1520-dna
//по мотивам визуала эффекта by Yaroslaw Turbin
//https://vk.com/ldirko программный код которого он запретил брать
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

// ----------- Эффект "Дым"
// based on cod by @Stepko (c) 23/12/2021
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

// ----------- Эффект "Мираж"
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

// -------------------- Эффект "Акварель"
// (c) kostyamat 26.12.2021
// https://editor.soulmatelights.com/gallery/1587-oil
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

// ----------- Эффект "Неопалимая купина"
//RadialFire
// (c) Stepko and Sutaburosu https://editor.soulmatelights.com/gallery/1570-radialfire
//23/12/21
class EffectRadialFire : public EffectCalc {
private:
    const int8_t MIN_MAX = max(WIDTH, HEIGHT);
    const int8_t C_X = (WIDTH / 2);
	const int8_t C_Y = (HEIGHT / 2);
    std::vector<std::vector<int>> XY_angle;
    std::vector<std::vector<byte>> XY_radius;
    float t;
    float speedFactor;
    uint8_t _scale;
    bool mode = false;


    String setDynCtrl(UIControl*_val) override;
    void palettesload();

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

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

class EffectFlower : public EffectCalc {
	private:
        uint8_t effTimer;
        float ZVoffset = 0;
        const float COLS_HALF = WIDTH * .5;
        const float ROWS_HALF = HEIGHT * .5;
        int16_t ZVcalcRadius(int16_t x, int16_t y);
        int16_t ZVcalcDist(uint8_t x, uint8_t y, float center_x, float center_y);
	public:
        bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

#ifdef RGB_PLAYER
// (c) Kostyantyn Matviyevskyy aka kostyamat, file format and decoder\encoder (c) Stepko
// 27.01.2022
// https://editor.soulmatelights.com/gallery/1684-pgm-player-with-resize
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
        bool loadFile(String &filename);
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
