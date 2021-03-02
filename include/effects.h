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

#ifndef _EFFECTS_H
#define _EFFECTS_H

#include "color_palette.h"
#include "effectworker.h"
#include "effectmath.h"


#ifdef MIC_EFFECTS
class EffectFreq : public EffectCalc {
private:
    uint8_t type=1;
    int8_t peakX[2][WIDTH];
    float samp_freq;
    double last_freq = 0;
    uint8_t last_min_peak, last_max_peak;
    float x[WIDTH+1];
    float maxVal;
    uint8_t freqDiv = 2U-scale/128; //1...2

    bool freqAnalyseRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};
#endif

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

class EffectMetaBalls : public EffectCalc {
private:
    bool metaBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectSinusoid3 : public EffectCalc {
private:
    bool sinusoid3Routine(CRGB *leds, EffectWorker *param);

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
    float bballsTCycle = 0.0;                           // The time since the last time the ball struck the ground
    float bballsCOR[bballsMaxNUM_BALLS] ;               // Coefficient of Restitution (bounce damping)
    long  bballsTLast[bballsMaxNUM_BALLS] ;             // The clock time of the last ground strike
    float bballsShift[bballsMaxNUM_BALLS];
    bool halo = false;                                  // ореол
    uint8_t _scale=1;
    bool bBallsRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
    // void setscl(const byte _scl) override; // перегрузка для масштаба
    //void setspd(const byte _spd) override; // перегрузка для скорости
    void regen();
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- Эффект "Пейнтбол" -------------
class EffectLightBalls : public EffectCalc {
private:
    bool lightBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------- Эффект "Пульс"
class EffectPulse : public EffectCalc {
private:
    uint8_t pulse_hue;
    uint8_t pulse_step = 0;
    uint8_t centerX = random8(WIDTH - 5U) + 3U;
    uint8_t centerY = random8(HEIGHT - 5U) + 3U;
    uint8_t currentRadius = 4;
    uint8_t _pulse_hue = 0;
    uint8_t _pulse_hueall = 0;


    bool pulseRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- эффект "Блуждающий кубик" -------------
class EffectBall : public EffectCalc {
private:
    uint8_t ballSize;
    CHSV ballColor;
    float vectorB[2U];
    float coordB[2U];

    bool ballRoutine(CRGB *leds, EffectWorker *param);

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
    bool lighterTracersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

class EffectRainbow : public EffectCalc {
private:
    float hue; // вещественное для малых скоростей, нужно приведение к uint8_t по месту

    bool rainbowHorVertRoutine(bool isVertical);
    bool rainbowDiagonalRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectColors : public EffectCalc {
private:
    uint8_t ihue;
    uint8_t mode;
    uint8_t modeColor;

    bool colorsRoutine(CRGB *leds, EffectWorker *param);
    void setscl(const byte _scl) override;
    String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------ Эффект "Белая Лампа"
class EffectWhiteColorStripe : public EffectCalc {
private:
    bool whiteColorStripeRoutine(CRGB *leds, EffectWorker *param);

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
// совместное творчество юзеров форума https://community.alexgyver.ru/
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

// ------------- светлячки -------------
class EffectLighters : public EffectCalc {
protected:
    uint8_t cnt = 1;
    bool subPix = false;
    uint16_t lightersIdx;
    float lightersSpeed[2U][LIGHTERS_AM];
    uint8_t lightersColor[LIGHTERS_AM];
    float lightersPos[2U][LIGHTERS_AM];
    byte light[LIGHTERS_AM];
private:
    bool lightersRoutine(CRGB *leds, EffectWorker *param);
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
    uint8_t hue;
    bool randColor = false;
    bool white = false;
    float count;
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

class EffectSpiro : public EffectCalc {
private:
  const uint8_t spiroradiusx = WIDTH / 4;
  const uint8_t spiroradiusy = HEIGHT / 4;

  const uint8_t spirocenterX = WIDTH / 2;
  const uint8_t spirocenterY = HEIGHT / 2;

  const uint8_t spirominx = spirocenterX - spiroradiusx;
  const uint8_t spiromaxx = spirocenterX + spiroradiusx;// + 1;
  const uint8_t spirominy = spirocenterY - spiroradiusy;
  const uint8_t spiromaxy = spirocenterY + spiroradiusy; // + 1;

  bool spiroincrement = false;
  bool spirohandledChange = false;
  float spirohueoffset = 0;
  uint8_t spirocount = 1;
  float spirotheta1 = 0;
  float spirotheta2 = 0;
  uint8_t internalCnt = 0;

  bool spiroRoutine(CRGB *leds, EffectWorker *param);

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
  float speedfactor;

  bool predatorPresent;
  float hueoffset;

  bool flockRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
  void setspd(const byte _spd) override;
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
  bool twinklesRoutine(CRGB *leds, EffectWorker *param);
  void setscl(const byte _scl) override;
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
  bool wavesRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= RADAR / РАДАР ===============
// Aurora : https://github.com/pixelmatix/aurora/blob/master/PatternRadar.h
// Copyright(c) 2014 Jason Coon
// v1.0 - Updating for GuverLamp v1.7 by Palpalych 14.04.2020
// v1.1 - +dither, +smoothing
class EffectRadar : public EffectCalc {
private:
  float eff_offset;        // глобальная переменная для работы эффектов (обычно применяется для циклического пересчета hue, количества кадров и др...)
  float eff_theta;         // глобальная переменная угла для работы эффектов
  bool subPix = false;
  byte hue;
  bool radarRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------------------------ ЭФФЕКТ ДЫМ ----------------------
class EffectMStreamSmoke : public EffectCalc {
private:
  uint8_t smokeHue = 0U;
  int fillType;
  float xSmokePos;
  float xSmokePos2;
  uint8_t eNs_noisesmooth;
  uint8_t rhue;
  uint32_t e_x[NUM_LAYERS];
  uint32_t e_y[NUM_LAYERS];
  uint32_t e_z[NUM_LAYERS];
  uint32_t e_scaleX[NUM_LAYERS];
  uint32_t e_scaleY[NUM_LAYERS];
  uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];

  void FillNoise(int8_t layer);     // TODO: join with Comet's
  bool multipleStreamSmokeRoutine(CRGB *leds, EffectWorker *param);
  String setDynCtrl(UIControl*_val) override;

public:
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

  bool fire2018Routine(CRGB *leds, EffectWorker *param);
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
  void setscl(const byte _scl) override;

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
    uint8_t pidx = 0;
    Particle particles[20];
    unsigned numParticles = 0;
    void generate(bool reset = false);
    void position();
    bool picassoRoutine(CRGB *leds, EffectWorker *param);
    bool picassoRoutine2(CRGB *leds, EffectWorker *param);
    bool picassoRoutine3(CRGB *leds, EffectWorker *param);
    bool picassoRoutine4(CRGB *leds, EffectWorker *param);
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
        CHSV color;
    } Leaper;
private:
    Leaper leapers[20];
    unsigned numParticles = 0;
    uint8_t _rv;
    void generate(bool reset = false);
    void restart_leaper(Leaper * l);
    void move_leaper(Leaper * l);
    //void wu_pixel(uint32_t x, uint32_t y, CRGB col);
    bool leapersRoutine(CRGB *leds, EffectWorker *param);
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
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

// ----------- Эффект "Вихри" адаптация SottNick
class EffectWhirl : public EffectCalc {
private:
    float ff_x;
    float ff_y;
    float ff_z;
    float hue;
    Boid boids[AVAILABLE_BOID_COUNT];
    uint8_t micPick = 0;

    const uint8_t ff_speed = 1; // чем выше этот параметр, тем короче переходы (градиенты) между цветами. 1 - это самое красивое
    const uint8_t ff_scale = 26; // чем больше этот параметр, тем больше "языков пламени" или как-то так. 26 - это норм

    bool whirlRoutine(CRGB *leds, EffectWorker *param);

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
    uint16_t xsin;
    uint16_t ysin;
    uint8_t satur;
    uint8_t glare = 0;
    uint8_t iconIdx = 0;

    void nGlare(CRGB *leds);
    void nPatterns();
    bool aquariumRoutine(CRGB *leds, EffectWorker *param);
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
    uint8_t csum = 0;
    bool setup = true;
    uint8_t micPick = 0;
    const uint8_t spirocenterX = WIDTH / 2;
    const uint8_t spirocenterY = HEIGHT / 2;
    bool starRoutine(CRGB *leds, EffectWorker *param);
    void drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler);

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
    bool pacificRoutine(CRGB *leds, EffectWorker *param);

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
    const float _scaler = 3.3 / 1024;
    CRGB color;
    bool oscRoutine(CRGB *leds, EffectWorker *param);
    float div;
    byte gain;
    double y[2] = {0., 0.};
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
    byte dir = 1;
    byte flip = 0;
    byte generation = 0;
    byte mic[2];
    byte rand;
    bool flag = false;

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

    bool noiseRoutine(CRGB *leds, EffectWorker *param);
    void adjust_gamma(CRGB *leds)
    {
        for (uint16_t i = 0; i < NUM_LEDS; i++)
        {
            leds[i].r = dim8_video(leds[i].r);
            leds[i].g = dim8_video(leds[i].g);
            leds[i].b = dim8_video(leds[i].b);
        }
    }

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Мотыльки"
// (с) SottNick, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49262
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

    bool butterflyRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    //void load() override;
    virtual String setDynCtrl(UIControl*_val) override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Тени"
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/effects.ino
class EffectShadows : public EffectCalc {
private:
    uint16_t sPseudotime = 0;
    uint16_t sLastMillis = 0;
    uint16_t sHue16 = 0;

    bool shadowsRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load() override;
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
    uint8_t _speed = 1, _scale = 1;
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
    float speedfactor;
    bool subpixel;
    float prevVal[2];
    void arrowSetupForMode(byte mode, bool change);
    void arrowSetup_mode1();
    void arrowSetup_mode2();
    //void arrowSetup_mode3(;)
    void arrowSetup_mode4();

    bool arrowsRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
    String setDynCtrl(UIControl*_val) override;
public:
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
    void setscl(const byte _scl) override;
    void setspd(const byte _spd) override;
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
    bool attractRoutine(CRGB *leds, EffectWorker *param);

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

    void load() override;
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

  void shuffleDown(float speedFactor, bool subpix)
  {
    internal_counter+=speedFactor*internal_speedf;

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

  void move(float speedfactor)
  {
    float inc = speedfactor*internal_speedf;

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

  void draw(CRGB colors[SNAKE_LENGTH], float speedfactor, int snakenb, bool subpix, bool isDebug=false);
};

    Snake snakes[MAX_SNAKES];
    bool snakeRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
public:
    //void load();
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
    

    void reload();
    void resetDot(uint8_t idx);
    String setDynCtrl(UIControl*_val) override;

  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
};


// --------------  Эффект "Цветение"
//Yaroslaw Turbin
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
class EffectFlower : public EffectCalc {
private:
    float c = 0.5;   //diameter
    float angle = 1.;
    float  counter = 0.;
    CHSV color;

    bool flowerRoutine(CRGB *leds, EffectWorker *param);
    //String setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эфеект "ДНК"
// Yaroslaw Turbin 04.09.2020
// https://vk.com/ldirko
// https://pastebin.com/jwvC1sNF
// адаптация и доработки kostyamat
class EffectDNA : public EffectCalc {
private:
    double freq = 3000;
    float mn =255.0/13.8;
    uint8_t speeds = 30;
    bool rotate = false;

    bool DNARoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эффект "Огненная Лампа"
// Yaroslaw Turbin, 22.06.2020 
// https://vk.com/ldirko
// https://pastebin.com/eKqe4zzA
// переделака на субпиксель и доработки - kostyamat
class EffectFire2020 : public EffectCalc {
private:
    #define NOISE_HEIGHT  (LED_ROWS * 3U)
    uint16_t noises[LED_COLS * NOISE_HEIGHT];   //precalculated noise table
    byte colorfade[LED_ROWS];                   //simple colorfade table for speedup
    float a = 0;
    byte _pal = 8;
    byte _scale = 60;
    bool fire2020Routine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
    //void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
    void palettesload() override;
    void regenNoise();

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эфеект "Змеиный Остров"
// (c) Сотнег
// База https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-53132
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
    byte csum = 0;

    bool testRoutine(CRGB *leds, EffectWorker *param);
    String setDynCtrl(UIControl*_val) override;
    void setscl(const byte _scl) override; // перегрузка для масштаба
    //void setspd(const byte _spd) override; // перегрузка для скорости
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
    float speedfactor;
    float center = (float)WIDTH / 2.;

    typedef struct
    {
        float x, y, xd, yd;
        byte hue;
    } Rocket;

    std::vector<Rocket> rockets;

    void restart_rocket(uint8_t r);
    void reload();
    bool popcornRoutine(CRGB *leds, EffectWorker *param);
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
    float speedfactor;
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
    uint8_t Scale = 6;
    uint8_t _scale = 1;
    int16_t offsetX = 0;
    int16_t offsetY = 0;
    float x[WIDTH];
    uint8_t effId = 1;
    uint8_t hue;
    void cell(CRGB *leds);
    void spider(CRGB *leds);
    void spruce(CRGB *leds);

  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    String setDynCtrl(UIControl*_val) override;
};

// ----------- Эффект "Геометрический Вальс"
//F_lying 
//Fastled 16x16 rgb led matrix demo
//Yaroslaw Turbin, 27.10.2020 
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/

//https://www.reddit.com/r/FastLED/comments/jj4oc9/new_fastled_matrix_example_f_lying_code_in
//code for arduino: https://wokwi.com/arduino/projects/280541577702539789
//                  https://wokwi.com/arduino/projects/280607115091902988
class EffectF_lying: public EffectCalc {
  private:
    byte hue = 0;
    uint8_t _blur = 1;
    void mydrawLine(CRGB *leds, float x, float y, float x1, float y1, byte hueLamda);
    String setDynCtrl(UIControl*_val);
#ifdef BIGMATRIX
    const float deviator = 2.;
#else
    const float deviator = 4;
#endif
    bool type = false;
  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
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
    void load() override;
};

// -------- "LDIRKO Ленд"
//simple playground for fasled
//best use with 16x16 matrix and bigger
//like tixy.land ))
//...ldir... Yaroslaw Turbin, 18.11.2020 
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
class EffectLLand: public EffectCalc {
  private:
    const byte effects = 10; //how many effects
    byte effnumber = 0; //start effect
    uint8_t _scale = 1;
    float t;
    uint16_t code(byte x, byte y, uint16_t i, float t);
    String setDynCtrl(UIControl*_val) override;
    bool select = false;
    bool randColor = false;
    float hue = 0;
  public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    void load() override;
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

//------------ Эффект "Дождь с ветром" 
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

// ------------- Эффект "Цветные драже"
#define MAXDOTS 16
class EffectPile : public EffectCalc {
private:
  struct dot {float x; float y; uint8_t hue;};
  struct dot dots[MAXDOTS];
  float widthPos[WIDTH];
  bool done = true;
  float speedfactor = 0.0f;
  float internal_counter = 0.0f;
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
    bool clearrows(bool clear=true);
};

//-------- по мотивам Эффектов Particle System -------------------------
// https://github.com/fuse314/arduino-particle-sys
// https://github.com/giladaya/arduino-particle-sys
// https://www.youtube.com/watch?v=S6novCRlHV8&t=51s
//при попытке вытащить из этой библиотеки только минимально необходимое выяснилось, что там очередной (третий) вариант реализации субпиксельной графики.
//ну его нафиг. лучше будет повторить визуал имеющимися в прошивке средствами.

// ============= ЭФФЕКТ ФЕЯ ===============
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

    void particlesUpdate2(uint8_t i);
    void fairyEmit(uint8_t i);
    void fountEmit(uint8_t i);
    bool fairy(CRGB *leds);
    void fount(CRGB *leds);
    void setscl(const byte _scl) override; // перегрузка для масштаба
    String setDynCtrl(UIControl*_val) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---------- Эффект "Бульбулятор"
// "Circles" (C) Elliott Kember https://editor.soulmatelights.com/gallery/11
// адаптация и переделка - kostyamat
#define NUMBER_OF_CIRCLES (NUM_LEDS / 32U)
class EffectCircles : public EffectCalc {
private:
    byte color;
    class Circle
    {
    public:
        //uint16_t offset;
        int16_t centerX;
        int16_t centerY;
        byte hue;
        float bpm = random(0, 255);

        void move() {
            centerX = random(0, LED_COLS-1);
            centerY = random(0, LED_ROWS-1);
        }
        
        void reset() {
            centerX = random(0, LED_COLS-1);
            centerY = random(0, LED_ROWS-1);
            hue = random(0, 255);
        }

        float radius() {
            float radius = EffectMath::fmap(triwave8(bpm), 0, 254, 0, 5); //beatsin16(bpm, 0, 500, 0, offset) / 100.0;
            return radius;
        }
    };

    Circle circles[NUMBER_OF_CIRCLES] = {};

    void drawCircle(CRGB *leds, Circle circle);

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

    bool centerRun = false;
    byte period = 10;
    byte _x = WIDTH/2;
    byte _y = HEIGHT/2;
    float speedFactor;

    void regen(byte id);
    void phisics(byte id);
    void setspd(const byte _spd) override;
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
    void fill_circle(float cx, float cy, float radius, CRGB col);
    void setspd(const byte _spd) override;
    //String setDynCtrl(UIControl*_val) override;


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

    timerMinim gameTimer = 200;         // Таймер скорости игр

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

    void setspd(const byte _spd) override; // перегрузка для скорости
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
// особая благодарность https://www.reddit.com/user/ldirko/ Yaroslaw Turbin aka ldirko

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

    void setscl(const byte _scl) override;
    //void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
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
    void setspd(const byte _spd) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectSmoker: public EffectCalc {
private:
    float timer;
    byte bump[NUM_LEDS];
    CRGB chsv[256];
    byte sat;

    void generatebump ();
    void regen();
    void Bumpmap(CRGB *leds, int8_t lightx, int8_t lighty);
    String setDynCtrl(UIControl*_val) override;
    void setscl(const byte _scl) override; // перегрузка для масштаба
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// --------- конец секции эффектов

#endif
