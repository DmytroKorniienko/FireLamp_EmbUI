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

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
#include "LittleFS.h"
#include "effects_types.h"

#define DEFAULT_SLIDER 127
#define PARAM_BUFSIZE 128

static const char _R255[] PROGMEM = "[{'R':'127'}]";

typedef enum _EFF_ENUM {
EFF_NONE = (0U),                              // Специальный служебный эффект, не комментировать и индекс не менять константу!
EFF_WHITE_COLOR,                              // Белый свет
EFF_COLORS,                                   // Смена цвета
EFF_RAINBOW_2D,                               // Радуга универсальная
EFF_SPARKLES,                                 // Конфетти
EFF_SNOW,                                     // Снегопад
EFF_SNOWSTORMSTARFALL,                        // Метель + Звездопад
EFF_MATRIX,                                   // Матрица
EFF_LIGHTERS,                                 // Светлячки
EFF_LIGHTER_TRACES,                           // Светлячки со шлейфом
EFF_CUBE,                                     // Блуждающий кубик
EFF_PULSE,                                    // Пульс
EFF_EVERYTHINGFALL,                           // Водо/огне/лава/радуга/хренопад
EFF_FIRE,                                     // Огонь
EFF_PAINTBALL,                                // Пейнтбол
EFF_MADNESS,                                  // Безумие 3D
EFF_CLOUDS,                                   // Облака 3D
EFF_LAVA,                                     // Лава 3D
EFF_PLASMA,                                   // Плазма 3D
EFF_RAINBOW,                                  // Радуга 3D
EFF_RAINBOW_STRIPE,                           // Павлин 3D
EFF_ZEBRA,                                    // Зебра 3D
EFF_FOREST,                                   // Лес 3D
EFF_OCEAN,                                    // Океан 3D
EFF_BBALS,                                    // Прыгающие мячики
EFF_SINUSOID3,                                // Синусоид 3
EFF_METABALLS,                                // Метасферы
EFF_SPIRO,                                    // Спираль
EFF_RAINBOWCOMET,                             // Радужная комета
EFF_RAINBOWCOMET3,                            // 3 кометы
EFF_PRIZMATA,                                 // Призмата
EFF_FLOCK,                                    // Стая
EFF_SWIRL,                                    // Водоворот
EFF_DRIFT,                                    // Дрифт
EFF_DRIFT2,                                   // Дрифт2
EFF_TWINKLES,                                 // Мерцание
EFF_RADAR,                                    // Радар
EFF_WAVES,                                    // Волны
EFF_FIRE2012,                                 // Огонь 2012
EFF_RAIN,                                     // Дождь
EFF_COLORRAIN,                                // Цветной дождь
EFF_STORMYRAIN,                               // Тучка в банке
EFF_FIRE2018,                                 // Огонь 2018
EFF_RINGS,                                    // Кодовый замок
EFF_CUBE2,                                    // Куб 2D
EFF_SMOKE,                                    // Дым
EFF_TIME = (98)                               // Часы (служебный, смещаем в конец)
#ifdef MIC_EFFECTS
,EFF_FREQ = (99U)                             // Частотный анализатор (служебный, смещаем в конец)
#endif
} EFF_ENUM;

/*
 * заглушка для "старых" эффектов
 */
void stubRoutine(CRGB *, const char *);
//-------------------------------------------------

#pragma pack(push,1)
typedef struct _EFFECT {
    bool canBeSelected:1;
    bool isFavorite:1;
    byte brightness;
    byte speed;
    byte scale;
    EFF_ENUM eff_nb;
    const char *eff_name;
    void (*func)(CRGB*,const char*);
    char *param;
    void setNone(){ eff_nb=EFF_NONE; eff_name=nullptr; brightness=127; speed=127; scale=127; canBeSelected=false; isFavorite=false; func=nullptr; param=nullptr; }
} EFFECT;
#pragma pack(pop)

const char T_SPARKLES[] PROGMEM = "Конфетти";
const char T_FIRE[] PROGMEM = "Огненная лампа";
const char T_EVERYTHINGFALL[] PROGMEM = "Эффектопад";
const char T_RAINBOW_2D[] PROGMEM = "Радуга 2D";
const char T_COLORS[] PROGMEM = "Цвета";
const char T_PULSE[] PROGMEM = "Пульс";
const char T_MATRIX[] PROGMEM = "Матрица";
const char T_SNOW[] PROGMEM = "Снегопад";
const char T_SNOWSTORMSTARFALL[] PROGMEM = "Метель + Звездопад";
const char T_LIGHTERS[] PROGMEM = "Светлячки";
const char T_LIGHTER_TRACES[] PROGMEM = "Светлячки со шлейфом";
const char T_PAINTBALL[] PROGMEM = "Пейнтбол";
const char T_CUBE[] PROGMEM = "Блуждающий кубик";
const char T_BBALS[] PROGMEM = "Прыгающие мячики";
const char T_MADNESS[] PROGMEM = "Безумие 3D";
const char T_RAINBOW[] PROGMEM = "Радуга 3D";
const char T_RAINBOW_STRIPE[] PROGMEM = "Павлин 3D";
const char T_ZEBRA[] PROGMEM = "Зебра 3D";
const char T_FOREST[] PROGMEM = "Лес 3D";
const char T_OCEAN[] PROGMEM = "Океан 3D";
const char T_PLASMA[] PROGMEM = "Плазма 3D";
const char T_CLOUDS[] PROGMEM = "Облака 3D";
const char T_LAVA[] PROGMEM = "Лава 3D";
const char T_SINUSOID3[] PROGMEM = "Синусоид 3";
const char T_METABALLS[] PROGMEM = "Метасферы";
const char T_SPIRO[] PROGMEM = "Спираль";
const char T_RAINBOWCOMET[] PROGMEM = "Радужная комета";
const char T_RAINBOWCOMET3[] PROGMEM = "Три кометы";
const char T_WHITE_COLOR[] PROGMEM = "Белая лампа";
const char T_PRIZMATA[] PROGMEM = "Призмата";
const char T_FLOCK[] PROGMEM = "Стая";
const char T_SWIRL[] PROGMEM = "Водоворот";
const char T_DRIFT[] PROGMEM = "Дрифт";
const char T_DRIFT2[] PROGMEM = "Дрифт 2";
const char T_TWINKLES[] PROGMEM = "Мерцание";
const char T_RADAR[] PROGMEM = "Радар";
const char T_WAVES[] PROGMEM = "Волны";
const char T_FIRE2012[] PROGMEM = "Огонь 2012";
const char T_RAIN[] PROGMEM = "Дождь";
const char T_COLORRAIN[] PROGMEM = "Цветной дождь";
const char T_STORMYRAIN[] PROGMEM = "Тучка в банке";
const char T_FIRE2018[] PROGMEM = "Огонь 2018";
const char T_RINGS[] PROGMEM = "Кодовый замок";
const char T_CUBE2[] PROGMEM = "Куб 2D";
const char T_TIME[] PROGMEM = "Часы";
const char T_SMOKE[] PROGMEM = "Дым";

#ifdef MIC_EFFECTS
const char T_FREQ[] PROGMEM = "Частотный анализатор";
#endif

static EFFECT _EFFECTS_ARR[] = {
    {false, false, 127, 127, 127, EFF_NONE, nullptr, nullptr, nullptr},
    {true, true, 127, 127, 127, EFF_WHITE_COLOR, T_WHITE_COLOR, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_COLORS, T_COLORS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW_2D, T_RAINBOW_2D, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SPARKLES, T_SPARKLES, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SNOW, T_SNOW, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SNOWSTORMSTARFALL, T_SNOWSTORMSTARFALL, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_MATRIX, T_MATRIX, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LIGHTERS, T_LIGHTERS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LIGHTER_TRACES, T_LIGHTER_TRACES, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CUBE, T_CUBE, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PULSE, T_PULSE, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_EVERYTHINGFALL, T_EVERYTHINGFALL, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE, T_FIRE, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PAINTBALL, T_PAINTBALL, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_MADNESS, T_MADNESS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CLOUDS, T_CLOUDS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LAVA, T_LAVA, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PLASMA, T_PLASMA, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW, T_RAINBOW, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW_STRIPE, T_RAINBOW_STRIPE, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_ZEBRA, T_ZEBRA, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FOREST, T_FOREST, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_OCEAN, T_OCEAN, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_BBALS, T_BBALS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SINUSOID3, T_SINUSOID3, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_METABALLS, T_METABALLS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SPIRO, T_SPIRO, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOWCOMET, T_RAINBOWCOMET, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOWCOMET3, T_RAINBOWCOMET3, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PRIZMATA, T_PRIZMATA, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FLOCK, T_FLOCK, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SWIRL, T_SWIRL, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_DRIFT, T_DRIFT, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_DRIFT2, T_DRIFT2, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_TWINKLES, T_TWINKLES, stubRoutine, ((char *)_R255)}, // очень хреновое приведение типов, но дальше это разрулим :)
    {true, true, 127, 127, 127, EFF_RADAR, T_RADAR, stubRoutine, ((char *)_R255)},
    {true, true, 127, 127, 127, EFF_WAVES, T_WAVES, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE2012, T_FIRE2012, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAIN, T_RAIN, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_COLORRAIN, T_COLORRAIN, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_STORMYRAIN, T_STORMYRAIN, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE2018, T_FIRE2018, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RINGS, T_RINGS, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CUBE2, T_CUBE2, stubRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SMOKE, T_SMOKE, stubRoutine, ((char *)_R255)},  // очень хреновое приведение типов, но дальше это разрулим :)
    {true, true, 127, 127, 127, EFF_TIME, T_TIME, stubRoutine, nullptr}
#ifdef MIC_EFFECTS
     ,{true, true, 127, 127, 127, EFF_FREQ, T_FREQ, stubRoutine, ((char *)_R255)} // очень хреновое приведение типов, но дальше это разрулим :)
 #endif
};

class SHARED_MEM {
public:
    union {
        struct { // будильник "рассвет"
            uint8_t dawnCounter;                                           // счётчик первых шагов будильника
            time_t startmillis;
            CHSV dawnColorMinus[6];
		};
	};
};

//! Basic Effect Calc class
/**
 * Базовый класс эффекта с основными переменными и методами общими для всех эффектов
 * методы переопределяются каждым эффектом по необходимости
*/
class EffectCalc {
private:

public:
    bool active=0;          /**< работает ли воркер и был ли обсчет кадров с момента последнего вызова, пока нужно чтобы пропускать холостые кадры */
    uint32_t lastrun=0;     /**< счетчик времени для эффектов с "задержкой" */
    EFF_ENUM effect;        /**< энумератор эффекта */
    byte brightness;
    byte speed;
    byte scale;
    uint8_t rval=0;             /**< загадочная R, если 0 - то не используется */
    uint8_t palettescale;       /**< странная переменная шкалы внутри палитры */
    float ptPallete;            // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
    uint8_t palettepos;         // позиция в массиве указателей паллитр


    /** флаг, включает использование палитр в эффекте.
     *  влияет на:
     *  - подгрузку дефолтовых палитр при init()
     *  - переключение палитры при изменении ползунка "шкалы"
     *  -  проверку R?
     */
    bool usepalettes=false;
    std::vector<PGMPalette*> palettes;          /**< набор используемых палитр (пустой)*/
    TProgmemRGBPalette16 const *curPalette = nullptr;     /**< указатель на текущую палитру */

    /** полезные обертки **/
    uint8_t wrapX(int8_t x){ return (x + WIDTH) % WIDTH; }
    uint8_t wrapY(int8_t y){ return (y + HEIGHT) % HEIGHT; }

    EffectCalc(){}

    /**
     * intit метод, вызывается отдельно после создания экземпляра эффекта для установки базовых переменных
     * в конце выполнения вызывает метод load() который может быть переопределен в дочернем классе
     * @param _eff - энумератор эффекта, может пригодится для мультиэффектов типа 3DNoise если эффект
     * может использовать разные функции для различных версий эффекта
     * @param _brt - яркость, прилетающая из "настроек" эффекта, эффект может менять свою яркость позже независимо от указок "сверху"
     * @param _spd - скорость, прилетающая из "настроек" эффекта, эффект может менять свою скорость позже независимо от указок "сверху"
     * @param _scl - шкала, прилетающая из "настроек" эффекта, эффект может менять свою шкалу позже независимо от указок "сверху"
     *
    */
    void init(EFF_ENUM _eff, byte _brt, byte _spd, byte _scl);

    /**
     * load метод, по умолчанию пустой. Вызывается автоматом из init(), в дочернем классе можно заменять на процедуру первой загрузки эффекта (вместо того что выполняется под флагом load)
     *
    */
    virtual void load();

    /**
     * run метод, Вызывается для прохода одного цикла эффекта, можно переопределять либо фунцией обсчета смого эффекта,
     * либо вспомогательной оберткой, вызывающей приватный метод.
     * Метод должет вернуть true если обсчет эффекта был завершен успешно или false если обсчет был пропущен и кадр не менялся
     * @param ledarr - указатель на массив, пока не используется
     * @param opt - опция, пока не используется, вероятно нужно заменить на какую-нибудь расширяемую структуру
    */
    virtual bool run(CRGB* ledarr, const char *opt=nullptr);

    /**
     * drynrun метод, всеми любимая затычка-проверка на "пустой" вызов
     * возвращает false если еще не прошло достаточно времени с EFFECTS_RUN_TIMER
     */
    bool dryrun();

    /**
     * status - статус воркера, если работает и загружен эффект, отдает true
     */
    virtual bool status();

    ///
    /// следующие методы дублируют устранку "яркости", "скорости", "шкалы" для эффекта.
    /// Сейчас это не используется, но соображения "за" следующие:
    ///  - эффекты можно программить со своими локальными переменными, не дергая конкретный
    ///    экземпляр myLamp.effects.getXXX
    ///  - эффекты могут по необходимости масштабировать параметры из байта в свою размерность, или можно расширить базовый класс
    ///  - эфекты могут переопределять методы установки параметров и корректировать их с учетом микрофона, например
    ///


    /**
     * setBrt - установка яркости для воркера
     */
    virtual void setbrt(const byte _brt);

    /**
     * setSpd - установка скорости для воркера
     */
    virtual void setspd(const byte _spd);

    /**
     * setBrt - установка шкалы для воркера
     */
    virtual void setscl(const byte _scl);

    /**
     * setrval - установка переменной R
     */
    virtual void setrval(const byte _R);

    /**
     * загрузка дефолтных палитр в массив и установка текущей палитры
     * в соответствие в "бегунком" шкала/R
     */
    virtual void palettesload();

    /**
     * palletemap - меняет указатель на текущую палитру из набора в соответствие с "ползунком"
     * @param _val - байт "ползунка"
     * @param _pals - набор с палитрами
     */
    virtual void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val);

    /**
     * метод выбирает текущую палитру '*curPalette' из набора дотупных палитр 'palettes'
     * в соответствии со значением "бегунка" шкалы. В случае если задана паременная rval -
     * метод использует значение R,  иначе используется значение scale
     * (палитры меняются автоматом при изменении значения шкалы/R, метод оставлен для совместимости
     * и для первоначальной загрузки эффекта)
     */
    void scale2pallete();


    /**
     * деструктор по-умолчанию пустой, может быть переопределен
     */
    virtual ~EffectCalc() = default;
};

/**
 * Класс с набором статических мат. функций, которые используются в
 * нескольких разных эффектах. Функции требующие большую статическую память
 * сюда не переносим
 *
 */
class EffectMath {
public:
  static uint8_t mapsincos8(bool map, uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255);
  static void MoveFractionalNoise(bool scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift = 0);
  static void fadePixel(uint8_t i, uint8_t j, uint8_t step);
  static void fader(uint8_t step);
  static uint8_t ceil8(const uint8_t a, const uint8_t b);

  /** аналог ардуино функции map(), но только для float
   */
  static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max){
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
};

#ifdef MIC_EFFECTS
class EffectFreq : public EffectCalc {
private:
    int8_t peakX[2][WIDTH];
    bool freqAnalyseRoutine(CRGB *leds, const char *param);
    void load() override;
public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};
#endif

class EffectTime : public EffectCalc {
private:
    bool timeShiftDir; // направление сдвига
    float curTimePos; // текущая позиция вывода
    CRGB hColor[1]; // цвет часов и минут
    CRGB mColor[1]; // цвет часов и минут
    bool timePrintRoutine(CRGB *leds, const char *param);
    void load() override;
public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectMetaBalls : public EffectCalc {
private:
    bool metaBallsRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectSinusoid3 : public EffectCalc {
private:
    bool sinusoid3Routine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectBBalls : public EffectCalc {
private:
    // можно переписать на динамческую память
    uint8_t bballsNUM_BALLS;                             // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way) ... количество мячиков теперь задаётся бегунком, а не константой
    uint8_t bballsCOLOR[bballsMaxNUM_BALLS] ;           // прикручено при адаптации для разноцветных мячиков
    uint8_t bballsX[bballsMaxNUM_BALLS] ;               // прикручено при адаптации для распределения мячиков по радиусу лампы
    int   bballsPos[bballsMaxNUM_BALLS] ;               // The integer position of the dot on the strip (LED index)
    float bballsHi = 0.0;                                    // An array of heights
    float bballsVImpact[bballsMaxNUM_BALLS] ;           // As time goes on the impact velocity will change, so make an array to store those values
    float bballsTCycle = 0.0;                                // The time since the last time the ball struck the ground
    float bballsCOR[bballsMaxNUM_BALLS] ;               // Coefficient of Restitution (bounce damping)
    long  bballsTLast[bballsMaxNUM_BALLS] ;             // The clock time of the last ground strike
    float bballsShift[bballsMaxNUM_BALLS];
    bool bBallsRoutine(CRGB *leds, const char *param);

public:
    void load();
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectLightBalls : public EffectCalc {
private:
    bool lightBallsRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectFire : public EffectCalc {

  uint8_t pcnt;
  uint8_t shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
  uint8_t line[WIDTH];
  uint8_t shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)
  unsigned char matrixValue[8][16];

private:
    void drawFrame(uint8_t pcnt, bool isColored);
    void generateLine();
    void shiftUp();
    bool fireRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectPulse : public EffectCalc {
private:
    uint8_t pulse_hue;
    uint8_t pulse_step = 0;
    bool pulseRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectBall : public EffectCalc {
private:
    int8_t ballSize;
    int16_t ballColor;
    int8_t vectorB[2U];
    float coordB[2U];
    bool ballRoutine(CRGB *leds, const char *param);

public:
    void load();
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectLighterTracers : public EffectCalc {
private:
    int8_t vector[BALLS_AMOUNT][2U];
    float coord[BALLS_AMOUNT][2U];
    int16_t ballColors[BALLS_AMOUNT];
    bool lighterTracersRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectRainbow : public EffectCalc {
private:
    uint8_t hue;
    bool rainbowHorVertRoutine(bool isVertical);
    bool rainbowDiagonalRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectColors : public EffectCalc {
private:
    uint8_t ihue;
    bool colorsRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectWhiteColorStripe : public EffectCalc {
private:
    bool whiteColorStripeRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectMatrix : public EffectCalc {
private:
    bool matrixRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectSnow : public EffectCalc {
private:
    bool snowRoutine(CRGB *leds, const char *param);
    float snowShift = 0.0; // сдвиг снега
public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectSparcles : public EffectCalc {
private:
    bool sparklesRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectEverythingFall : public EffectCalc {
private:
    byte heat[WIDTH][HEIGHT];
    bool fire2012WithPalette(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectFire2012 : public EffectCalc {
private:
#define NUMPALETTES 10

  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
  const uint8_t cooling = 70;
  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
  const uint8_t sparking = 130;
  // SMOOTHING; How much blending should be done between frames
  // Lower = more blending and smoother flames. Higher = less blending and flickery flames
  const uint8_t fireSmoothing = 90;


  uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];

  bool fire2012Routine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectStarFall : public EffectCalc {
private:
    bool snowStormStarfallRoutine(CRGB *leds, const char *param);

public:
    void load() override {FastLED.clear();}
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectLighters : public EffectCalc {
private:
    uint16_t lightersIdx;
    int8_t lightersSpeed[2U][LIGHTERS_AM];
    uint8_t lightersColor[LIGHTERS_AM];
    float lightersPos[2U][LIGHTERS_AM];

    bool lightersRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class Effect3DNoise : public EffectCalc {
private:
//    uint16_t XY(uint8_t x, uint8_t y);
    void fillNoiseLED();
    void fillnoise8();

    uint8_t ihue;
    uint8_t colorLoop;
    uint16_t _speed;             // speed is set dynamically once we've started up
    uint16_t _scale;             // scale is set dynamically once we've started up
    uint16_t x;
    uint16_t y;
    uint16_t z;
    #if (WIDTH > HEIGHT)
    uint8_t noise[2*HEIGHT][WIDTH];
    #else
    uint8_t noise[2*WIDTH][HEIGHT];
    #endif

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectSpiro : public EffectCalc {
private:
  const uint8_t spiroradiusx = WIDTH / 4;
  const uint8_t spiroradiusy = HEIGHT / 4;

  const uint8_t spirocenterX = WIDTH / 2;
  const uint8_t spirocenterY = HEIGHT / 2;

  const uint8_t spirominx = spirocenterX - spiroradiusx;
  const uint8_t spiromaxx = spirocenterX + spiroradiusx + 1;
  const uint8_t spirominy = spirocenterY - spiroradiusy;
  const uint8_t spiromaxy = spirocenterY + spiroradiusy + 1;

  boolean spiroincrement = false;
  boolean spirohandledChange = false;
  byte spirohueoffset = 0;
  uint8_t spirocount = 1;
  float spirotheta1 = 0;
  float spirotheta2 = 0;

  bool spiroRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectPrismata : public EffectCalc {
private:
  byte spirohueoffset = 0;

  bool prismataRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectFlock : public EffectCalc {
private:
  Boid boids[AVAILABLE_BOID_COUNT];
  Boid predator;
  PVector wind;

  bool predatorPresent;
  uint8_t hueoffset;

  bool flockRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};


class EffectComet : public EffectCalc {
private:
    uint8_t eNs_noisesmooth;
    uint8_t rhue;
    uint8_t smokeHue;
    float xSmokePos;
    float xSmokePos2;
    uint16_t noiseX;
    uint16_t noiseY;
    uint16_t noiseZ;
    uint8_t nline[WIDTH];
    uint32_t e_x[NUM_LAYERS];
    uint32_t e_y[NUM_LAYERS];
    uint32_t e_z[NUM_LAYERS];
    uint32_t e_scaleX[NUM_LAYERS];
    uint32_t e_scaleY[NUM_LAYERS];
    uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];

   const uint8_t e_centerX =  (WIDTH / 2) - 1;
   const uint8_t e_centerY = (HEIGHT / 2) - 1;


    void drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color);
    void FillNoise(int8_t layer);
    bool rainbowCometRoutine(CRGB *leds, const char *param);
    bool rainbowComet3Routine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectSwirl : public EffectCalc {
private:
    bool swirlRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectDrift : public EffectCalc {
private:
  uint8_t dri_phase;
  uint8_t _dri_speed;
  uint8_t _dri_delta;
  bool incrementalDriftRoutine(CRGB *leds, const char *param);
  bool incrementalDriftRoutine2(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectTwinkles : public EffectCalc {
private:
  uint8_t thue = 0U;
  uint8_t tnum;
  CRGB ledsbuff[NUM_LEDS];
  bool twinklesRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectWaves : public EffectCalc {
private:
  uint8_t waveCount = speed % 2;
  const uint8_t waveScale = 256 / WIDTH;
  uint8_t waveRotation;
  uint8_t whue;
  uint8_t waveTheta;
  bool wavesRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectRadar : public EffectCalc {
private:
  uint8_t eff_offset;        // глобальная переменная для работы эффектов (обычно применяется для циклического пересчета hue, количества кадров и др...)
  uint8_t eff_theta;         // глобальная переменная угла для работы эффектов
  bool radarRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectMStreamSmoke : public EffectCalc {
private:
  uint8_t smokeHue = 0U;
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
  bool multipleStreamSmokeRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectFire2018 : public EffectCalc {
private:
  const uint8_t CentreY = HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t CentreX = WIDTH / 2 + (WIDTH % 2);

  uint32_t noise32_x[NUM_LAYERS2];
  uint32_t noise32_y[NUM_LAYERS2];
  uint32_t noise32_z[NUM_LAYERS2];
  uint32_t scale32_x[NUM_LAYERS2];
  uint32_t scale32_y[NUM_LAYERS2];
  uint8_t fire18heat[NUM_LEDS];
  uint8_t noise3dx[NUM_LAYERS2][WIDTH][HEIGHT];

  bool fire2018Routine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

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
  bool ringsRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectCube2d : public EffectCalc {
private:
  uint8_t sizeX, sizeY; // размеры ячеек по горизонтали / вертикали
  uint8_t cntX, cntY; // количество ячеек по горизонтали / вертикали
  uint8_t fieldX, fieldY; // размер всего поля блоков по горизонтали / вертикали (в том числе 1 дополнительная пустая дорожка-разделитель с какой-то из сторон)
  uint8_t csum;   // reload checksum

  uint8_t pauseSteps; // осталось шагов паузы
  uint8_t shiftSteps; // всего шагов сдвига (от 3 до 4)
  uint8_t moveItem;     // индекс перемещаемого элемента
  bool movedirection;   // направление смещения
  bool direction; // направление вращения в текущем цикле (вертикаль/горизонталь)

  void cubesize();
  bool cube2dRoutine(CRGB *leds, const char *param);

public:
    void load() override;
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};

class EffectRain : public EffectCalc {
private:
  uint16_t noiseX;
  uint16_t noiseY;
  uint16_t noiseZ;
  uint8_t rhue;
  uint8_t nline[WIDTH];
  uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];

  uint8_t myScale8(uint8_t x);
  void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm, bool fixRC = false);
  bool coloredRainRoutine(CRGB *leds, const char *param);
  bool stormyRainRoutine(CRGB *leds, const char *param);
  bool simpleRainRoutine(CRGB *leds, const char *param);

public:
    bool run(CRGB *ledarr, const char *opt=nullptr) override;
};


class EffectWorker {
private:
    const unsigned int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    EFF_ENUM curEff = EFF_NONE;     ///< энумератор текущего эффекта
    unsigned int workIdx = 0;       ///< абсолютный номер эффекта по порядку (исполняемый)
    unsigned int selectIdx = 0;     ///< абсолютный номер эффекта по порядку (выбраный)
    EFFECT* effects = _EFFECTS_ARR; ///< массив настроек всех эффектов

    /**
     * создает и инициализирует экземпляр класса выбранного эффекта
     *
    */
    void workerset(EFF_ENUM effect);

    EffectWorker(const EffectWorker&);  // noncopyable
    EffectWorker& operator=(const EffectWorker&);  // noncopyable

public:
    EffectWorker() {
        workerset(EFF_NONE);
    }

    ~EffectWorker() {}

    std::unique_ptr<EffectCalc> worker;           ///< указатель-класс обработчик текущего эффекта

    void loadConfig(const char *cfg = nullptr) {
        if (LittleFS.begin()) {
            File configFile;
            if (cfg == nullptr) {
                LOG(println, F("Load default effects config file"));
                configFile = LittleFS.open(F("/eff_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
            } else {
                LOG(printf_P, PSTR("Load %s effects config file\n"), cfg);
                configFile = LittleFS.open(cfg, "r"); // PSTR("r") использовать нельзя, будет исключение!
            }
            String cfg_str = configFile.readString();
            configFile.close();

            if (cfg_str == F("")){
                LOG(println, F("Failed to open effects config file"));
                saveConfig();
                return;
            }

            LOG(println, F("\nStart desialization of effects\n\n"));

            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, cfg_str);
            if (error) {
                LOG(print, F("deserializeJson error: "));
                LOG(println, error.code());
                return;
            }

            JsonArray arr = doc.as<JsonArray>();
            EFFECT *eff;
            for (size_t i=0; i<arr.size(); i++) {
                JsonObject item = arr[i];

                EFF_ENUM nb = (EFF_ENUM)(item[F("nb")].as<int>());
                eff = getEffectBy(nb);
                if(eff->eff_nb!=EFF_NONE){
                    eff->brightness = item[F("br")].as<int>();
                    eff->speed = item[F("sp")].as<int>();
                    eff->scale = item[F("sc")].as<int>();
                    eff->isFavorite = (bool)(item[F("isF")].as<int>());
                    eff->canBeSelected = (bool)(item[F("cbS")].as<int>());
                    String tmp = item[F("prm")];
                    if(eff->param!=nullptr && cfg != nullptr) // так некрасиво, но сойдет пока что... (т.е. не освобождаем память, если читаем основной конфиг, чтобы не грохнуть PROGMEM указатели)
                        delete [] eff->param;
                    eff->param = new char[tmp.length()+1];
                    strcpy(eff->param, tmp.c_str());
                }
                LOG(printf_P, PSTR("(%d - %d - %d - %d - %d - %d - %s)\n"), nb, eff->brightness, eff->speed, eff->scale, eff->isFavorite, eff->canBeSelected, eff->param!=nullptr?FPSTR(eff->param):FPSTR(F("")));
            }
            doc.clear();
        }
    }

    void saveConfig(const char *cfg = nullptr) {
        if (LittleFS.begin()) {
            File configFile;
            if (cfg == nullptr) {
                LOG(println, F("Save default effects config file"));
                configFile = LittleFS.open(F("/eff_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
            } else {
                LOG(printf_P, PSTR("Save %s effects config file\n"), cfg);
                configFile = LittleFS.open(cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
            }
            EFFECT *cur_eff;

            configFile.print("[");
            for(unsigned int i=1; i<MODE_AMOUNT; i++){ // EFF_NONE не сохраняем
                cur_eff = &(effects[i]);
                configFile.printf_P( PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?FPSTR(cur_eff->param):FPSTR(F(""))));
                LOG(printf_P, PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?FPSTR(cur_eff->param):FPSTR(F(""))));
            }
            configFile.print("]");
            configFile.flush();
            configFile.close();
            LOG(println,"");
        }
    }

    bool autoSaveConfig() {
        static unsigned long i;
        if(i + (30 * 1000) > millis()){  // если не пришло время - выходим из функции и сбрасываем счетчик (ожидаем бездействия в 30 секунд относительно последней записи)
            i = millis();
            return false;
        }
        saveConfig();
        i = millis();
        return true; // сохранились
    }

    byte getModeAmount() {return MODE_AMOUNT;}

    void setBrightness(byte val) {effects[workIdx].brightness = val; if (worker) worker->setbrt(val);}
    void setSpeed(byte val) {effects[workIdx].speed = val; if (worker) worker->setspd(val);}
    void setScale(byte val) {effects[workIdx].scale = val; if (worker) worker->setscl(val);}
    byte getBrightness() { return effects[workIdx].brightness; }
    byte getSpeed() { return effects[workIdx].speed; }
    byte getScale() { return effects[workIdx].scale; }
    const char *getName() {return effects[workIdx].eff_name;}
    const EFF_ENUM getEn() {return effects[workIdx].eff_nb;}

    void setBrightnessS(byte val) {effects[selectIdx].brightness = val; if (worker && isSelected()) worker->setbrt(val);}
    void setSpeedS(byte val) {effects[selectIdx].speed = val; if (worker && isSelected()) worker->setspd(val);}
    void setScaleS(byte val) {effects[selectIdx].scale = val; if (worker && isSelected()) worker->setscl(val);}
    byte getBrightnessS() { return effects[selectIdx].brightness; }
    byte getSpeedS() { return effects[selectIdx].speed; }
    byte getScaleS() { return effects[selectIdx].scale; }
    const char *getNameS() {return effects[selectIdx].eff_name;}
    const EFF_ENUM getEnS() {return effects[selectIdx].eff_nb;}

    unsigned getNext() { // следующий эффект, кроме canBeSelected==false
        unsigned i;
        for (i = workIdx + 1; i < MODE_AMOUNT; i++) {
            if (effects[i].canBeSelected) return i;
        }
        for (i = 1; i < MODE_AMOUNT; i++) {
            if (effects[i].canBeSelected) return i;
        }
        return 0;
    }

    void moveNext() { // следующий эффект, кроме canBeSelected==false
        workIdx = selectIdx = getNext();
        curEff = effects[workIdx].eff_nb;
        workerset(curEff);
    }

    unsigned getPrev() { // предыдущий эффект, кроме canBeSelected==false
        unsigned i;
        for (i = selectIdx - 1; i > 0; i--) {
            if (effects[i].canBeSelected) return i;
        }
        for (i = MODE_AMOUNT - 1; i >= 0; i--){
            if (effects[i].canBeSelected) return i;
        }
        return 0;
    }

    void movePrev() { // предыдущий эффект, кроме canBeSelected==false
        workIdx = selectIdx = getPrev();
        curEff = effects[workIdx].eff_nb;
        workerset(curEff);
    }

    unsigned getBy(EFF_ENUM select){ // перейти по перечислению
        for (unsigned i = MODE_AMOUNT - 1; i >= 0; i--) {
            if (effects[i].eff_nb == select) return i;
        }
    }

    void moveBy(EFF_ENUM select){ // перейти по перечислению
        workIdx = selectIdx = getBy(select);
        curEff = effects[workIdx].eff_nb;
        workerset(curEff);
    }

    void moveSelected(){ // перейти по предворительно выбранному
        workIdx = selectIdx;
        curEff = effects[workIdx].eff_nb;
        workerset(curEff);
    }

    unsigned getBy(byte cnt){ // перейти на количество шагов, к ближайшему большему (для DEMO)
        unsigned i = (selectIdx + cnt) % MODE_AMOUNT; // смещаемся на нужное число шагов, но не больше лимита эффектов
        unsigned tmp = i; // запомним позицию

        while (!effects[i].isFavorite) { // если не избранный, то будем перебирать по +1
            if (++i == MODE_AMOUNT) i = 0;
            if (i == tmp) break; // круг прошли, но не нашли, на выход
        }
        return i;
    }

    void moveBy(byte cnt){ // перейти на количество шагов, к ближайшему большему (для DEMO)
        workIdx = selectIdx = getBy(cnt);
        curEff = effects[workIdx].eff_nb;
        workerset(curEff);
    }

    EFFECT *enumNextEffect(EFFECT *current){
        for(unsigned int i=0; i<MODE_AMOUNT; i++){
            if(effects[i].eff_nb == current->eff_nb){
                if((i+1)!=MODE_AMOUNT)
                    return &effects[i+1];
                else
                    return &effects[0]; // NONE
            }
        }
        return &effects[0]; // NONE
    }

    EFFECT *getEffectBy(EFF_ENUM select){
        for(int i=MODE_AMOUNT-1; i>=0; i--){
            if(effects[i].eff_nb == select){
                return &(effects[i]);
            }
        }
        return &(effects[0]); // NONE
    }

    EFFECT *getCurrent(){ // вернуть текущий
        return &(effects[workIdx]);
    }

    EFFECT *getSelected(){ // вернуть текущий
        return &(effects[selectIdx]);
    }

    void setSelected(unsigned idx){
        selectIdx = idx;
    }

    bool isSelected(){
        return (selectIdx == workIdx);
    }

    // ой как не нравятся мне джейсоны :()
    String getParam() {
        if (effects[selectIdx].param != nullptr) {
            size_t slen = strlen_P(effects[selectIdx].param);
            char buffer[slen + 4]; memset(buffer, 0, slen+4);
            strcpy_P(buffer, effects[selectIdx].param); // Обход Exeption 3, это шаманство из-за корявого использования указателя, он одновременно может быть и на PROGMEM, и на RAM
            String tmp = buffer;
            return tmp;
        }
        return String(); // empty
    }

    void updateParam(const char *str) {
        if (effects[selectIdx].param != nullptr && effects[workIdx].param != _R255) {
            // херовая проверка, надобно будет потом выяснить как безопасно разпознать указатель на PROGMEM или на RAM
            delete [] effects[selectIdx].param;
        }
        effects[selectIdx].param = new char[strlen(str)+1];
        strcpy(effects[selectIdx].param, str);
    }

    void updateParam(EFFECT *eff, const char *str) {
        if (eff->param != nullptr && eff->param != _R255) {
            // херовая проверка, надобно будет потом выяснить как безопасно разпознать указатель на PROGMEM или на RAM
            delete [] eff->param;
        }
        eff->param = new char[strlen(str)+1];
        strcpy(eff->param, str);
    }

    String getValue(const char *src, const _PTR type){
        if(src==nullptr)
            return String(); // empty
        String tmp(FPSTR(src)); // разве сюда в src прилетат указатель на чары из структуры конфига, не нa флеш??
        //String tmp(src);
        if (tmp.length()==0)
            return String(); // empty
        DynamicJsonDocument doc(PARAM_BUFSIZE);
        tmp.replace(F("'"),F("\"")); // так делать не красиво, но шопаделаешь...
        deserializeJson(doc,tmp);
        JsonArray arr = doc.as<JsonArray>();
        for (size_t i=0; i<arr.size(); i++) {
            JsonObject item = arr[i];
            if(item.containsKey(FPSTR(type))){
                return item[FPSTR(type)].as<String>();
            }
        }
        return String(); // empty
    }

    void setValue(const char *src, const __FlashStringHelper *type, const char *val){
        if (!strlen(val))
            return;
        DynamicJsonDocument doc(PARAM_BUFSIZE);
        deserializeJson(doc,String(FPSTR(src)));
        JsonArray arr = doc.as<JsonArray>();
        for (size_t i=0; i<arr.size(); i++) {
            JsonObject item = arr[i];
            if(item.containsKey(FPSTR(type))){
                item[FPSTR(type)]=FPSTR(val);
            }
        }
        String tmp;
        serializeJson(doc,tmp);
        tmp.replace(F("\""),F("'")); // так делать не красиво, но шопаделаешь...
        updateParam(tmp.c_str());

        // устанавливаем переменну 'rval' если задается ключ 'R'
        if (!strcmp_P("R", (PGM_P)type) && worker) {
            //LOG(printf_P, PSTR("TRY to set Rval=%s\n"), val);
            worker->setrval(atoi(val));
        }
    }

};

typedef enum _PERIODICTIME {
  PT_NOT_SHOW = 1,
  PT_EVERY_60,
  PT_EVERY_30,
  PT_EVERY_15,
  PT_EVERY_10,
  PT_EVERY_5,
  PT_EVERY_1,
} PERIODICTIME;



#endif