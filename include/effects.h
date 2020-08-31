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
#include "text_res.h"
#include "effects_types.h"
#include "../../include/LList.h"

// #define DEFAULT_SLIDER 127
// #define PARAM_BUFSIZE 128
#define IDX_ITEMBUFFSIZE    25      // буфер для джейсон элемента индекса
//#define FILEIO_BUFFSIZE    256      // буфер IO для работы с файлами (делаем равный странице littlefs)

//-------------------------------------------------
class EffectWorker;

class UIControl{
private:
    uint8_t id;
    CONTROL_TYPE ctype;
    String control_name;
    String val;
    String min;
    String max;
    String step;
public:
    UIControl(const uint8_t id, const CONTROL_TYPE ctype, const String &control_name, const String &val, const String &min, const String &max, const String &step)
    {
        this->id=id;
        this->ctype = ctype;
        this->control_name = String(control_name);

        this->val = val;
        this->min = min;
        this->max = max;
        this->step = step;
    }
    UIControl() : id(0), ctype(CONTROL_TYPE::RANGE), control_name(), val(), min(), max(), step() {}
    const uint8_t getId() {return id;}
    const CONTROL_TYPE getType() {return ctype;}
    const String &getName() {return control_name;}
    const String &getVal() {return val;}
    const String &getMin() {return min;}
    const String &getMax() {return max;}
    const String &getStep() {return step;}

    void setVal(const String &_val) {val=_val;}
};

typedef union {
    uint8_t mask;
    struct {
        bool canBeSelected:1;
        bool isFavorite:1;
    };
} EFFFLAGS;

class EffectListElem{
    private:
    unsigned long ms = micros(); // момент создания элемента, для сортировки в порядке следования
    public:
    uint16_t eff_nb; // номер эффекта, для копий наращиваем старший байт
    EFFFLAGS flags; // флаги эффекта

    EffectListElem(uint16_t nb, uint8_t mask){
        eff_nb = nb;
        flags.mask = mask;
    }

    EffectListElem(const EffectListElem *base){
        eff_nb = ((((base->eff_nb >> 8) + 1 ) << 8 ) | (base->eff_nb&0xFF)); // в старшем байте увеличиваем значение на 1
        flags = base->flags;
    }
    bool canBeSelected(){ return flags.canBeSelected; }
    void canBeSelected(bool val){ flags.canBeSelected = val; }
    bool isFavorite(){ return flags.isFavorite; }
    void isFavorite(bool val){ flags.isFavorite = val; }
    unsigned long  getMS(){ return ms; }
};

//! Basic Effect Calc class
/**
 * Базовый класс эффекта с основными переменными и методами общими для всех эффектов
 * методы переопределяются каждым эффектом по необходимости
*/
class EffectCalc {
private:
    EffectWorker *pworker = nullptr; // указатель на воркер
    LList<UIControl *> *ctrls;
    String dummy; // дефолтная затычка для отсутствующего контролла, в случае приведения к целому получится "0"
protected:
    bool active=0;          /**< работает ли воркер и был ли обсчет кадров с момента последнего вызова, пока нужно чтобы пропускать холостые кадры */
    bool isCtrlPallete = false; // признак наличия контрола палитры

    uint32_t lastrun=0;     /**< счетчик времени для эффектов с "задержкой" */
    EFF_ENUM effect;        /**< энумератор эффекта */

    byte brightness;
    byte speed;
    byte scale;

    uint8_t palettescale;       /**< странная переменная шкалы внутри палитры */
    float ptPallete;            // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
    uint8_t palettepos;         // позиция в массиве указателей паллитр
    uint8_t paletteIdx;         // индекс палитры переданный с UI

    /** флаг, включает использование палитр в эффекте.
     *  влияет на:
     *  - подгрузку дефолтовых палитр при init()
     *  - переключение палитры при изменении ползунка "шкалы"
     *  -  проверку R?
     */
    bool usepalettes=false;
    std::vector<PGMPalette*> palettes;          /**< набор используемых палитр (пустой)*/
    TProgmemRGBPalette16 const *curPalette = nullptr;     /**< указатель на текущую палитру */

    const String &getCtrlVal(int idx) {
        return (idx<ctrls->size() && idx>=0) ? (*ctrls)[idx]->getVal() : dummy;
    }

public:

    /** полезные обертки **/
    uint8_t wrapX(int8_t x){ return (x + WIDTH) % WIDTH; }
    uint8_t wrapY(int8_t y){ return (y + HEIGHT) % HEIGHT; }

    EffectCalc(){}

    /**
     * pre_init метод, вызывается отдельно после создания экземпляра эффекта до каких либо иных инициализаций
     * это нужно чтобы объект понимал кто он и возможно было вычитать конфиг для мультиэфектов, никаких иных действий здесь не предполагается
    */
    void pre_init(EFF_ENUM _eff, EffectWorker *_pworker, LList<UIControl *> *_ctrls) {effect = _eff; pworker = _pworker; ctrls = _ctrls;}

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
    virtual bool run(CRGB* ledarr, EffectWorker *opt=nullptr);

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
     * setDynCtrl - обработка для динамических контролов idx=3+
     * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/page-112#post-48848
     */
    virtual void setDynCtrl(UIControl*_val);

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
    //virtual ~EffectCalc(){ LOG(println,PSTR("DEGUG: Effect was destroyed\n")); } // отладка, можно будет затем закомментировать
};

/**
 * Класс с набором статических мат. функций, которые используются в
 * нескольких разных эффектах. Функции требующие большую статическую память
 * сюда не переносим
 *
 */
class EffectMath {
public:
  static CRGB overrun;
  static uint8_t mapsincos8(bool map, uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255);
  static void MoveFractionalNoise(bool scale, const uint8_t noise3d[][WIDTH][HEIGHT], int8_t amplitude, float shift = 0);
  static void fadePixel(uint8_t i, uint8_t j, uint8_t step);
  static void fader(uint8_t step);
  static uint8_t ceil8(const uint8_t a, const uint8_t b);
  /*
  static CRGB& piXY(CRGB *leds, byte x, byte y);
  static void screenscale( accum88 a, byte N, byte& screen, byte& screenerr);
  static void plot88(CRGB *leds, byte x, byte y, CRGB& color);
  static int16_t scale15by8_local( int16_t i, fract8 scale );
*/
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

    //const String getName() override {return String(FPSTR(T_FREQ));}
    bool freqAnalyseRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};
#endif

// Думаю пример все поглядели, возвертаю часы в режим с 3 ползунками, а то масштаб вообще не работает :)
//static const char EFF_TIME_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":\"@ver@\",\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Палитра\"}]}";
class EffectTime : public EffectCalc {
private:
    bool timeShiftDir; // направление сдвига
    float curTimePos; // текущая позиция вывода
    CRGB hColor[1]; // цвет часов и минут
    CRGB mColor[1]; // цвет часов и минут

    bool timePrintRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectMetaBalls : public EffectCalc {
private:
    //const String getName() override {return String(FPSTR(T_METABALLS));}
    bool metaBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectSinusoid3 : public EffectCalc {
private:
    //const String getName() override {return String(FPSTR(T_SINUSOID3));}
    bool sinusoid3Routine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//----- Эффект "Прыгающие Мячики"
// перевод на субпиксельную графику kostyamat
class EffectBBalls : public EffectCalc {
private:
    // можно переписать на динамческую память
    uint8_t bballsNUM_BALLS;                             // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way) ... количество мячиков теперь задаётся бегунком, а не константой
    uint8_t bballsCOLOR[bballsMaxNUM_BALLS] ;           // прикручено при адаптации для разноцветных мячиков
    float bballsX[bballsMaxNUM_BALLS] ;               // прикручено при адаптации для распределения мячиков по радиусу лампы
    float bballsPos[bballsMaxNUM_BALLS] ;               // The integer position of the dot on the strip (LED index)
    float bballsHi = 0.0;                                    // An array of heights
    float bballsVImpact[bballsMaxNUM_BALLS] ;           // As time goes on the impact velocity will change, so make an array to store those values
    float bballsTCycle = 0.0;                                // The time since the last time the ball struck the ground
    float bballsCOR[bballsMaxNUM_BALLS] ;               // Coefficient of Restitution (bounce damping)
    long  bballsTLast[bballsMaxNUM_BALLS] ;             // The clock time of the last ground strike
    float bballsShift[bballsMaxNUM_BALLS];
    byte csum = 0;
    bool regen = true;
    bool bBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectLightBalls : public EffectCalc {
private:
    //const String getName() override {return String(FPSTR(T_PAINTBALL));}
    bool lightBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= Эффект "Огненная лампа" =================
class EffectFire : public EffectCalc {
private:
  uint8_t pcnt;
  uint8_t shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
  uint8_t line[WIDTH];
  uint8_t shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)
  unsigned char matrixValue[8][16];

    void drawFrame(uint8_t pcnt, bool isColored);
    void generateLine();
    void shiftUp();
    bool fireRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectPulse : public EffectCalc {
private:
    uint8_t pulse_hue;
    uint8_t pulse_step = 0;
    uint8_t centerX = random8(WIDTH - 5U) + 3U;
    uint8_t centerY = random8(HEIGHT - 5U) + 3U;
    uint8_t currentRadius = 4;
    uint8_t _pulse_hue = 0;
    uint8_t _pulse_hueall = 0;

    //const String getName() override {return String(FPSTR(T_PULSE));}
    bool pulseRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- эффект "блуждающий кубик" -------------
class EffectBall : public EffectCalc {
private:
    int8_t ballSize;
    CHSV ballColor;
    float vectorB[2U];
    float coordB[2U];

    //const String getName() override {return String(FPSTR(T_CUBE));}
    bool ballRoutine(CRGB *leds, EffectWorker *param);

public:
    void load();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectLighterTracers : public EffectCalc {
private:
    float vector[BALLS_AMOUNT][2U];
    float coord[BALLS_AMOUNT][2U];
    int16_t ballColors[BALLS_AMOUNT];
    byte light[BALLS_AMOUNT];
    //const String getName() override {return String(FPSTR(T_LIGHTER_TRACES));}
    bool lighterTracersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectRainbow : public EffectCalc {
private:
    float hue; // вещественное для малых скоростей, нужно приведение к uint8_t по месту
    
    //const String getName() override {return String(FPSTR(T_RAINBOW_2D));}
    bool rainbowHorVertRoutine(bool isVertical);
    bool rainbowDiagonalRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectColors : public EffectCalc {
private:
    uint8_t ihue;

    //const String getName() override {return String(FPSTR(T_COLORS));}
    bool colorsRoutine(CRGB *leds, EffectWorker *param);

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

class EffectMatrix : public EffectCalc {
private:

    //const String getName() override {return String(FPSTR(T_MATRIX));}
    bool matrixRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectSnow : public EffectCalc {
private:
    //const String getName() override {return String(FPSTR(T_SNOW));}
    bool snowRoutine(CRGB *leds, EffectWorker *param);
    float snowShift = 0.0; // сдвиг снега
    
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Конффетти"
class EffectSparcles : public EffectCalc {
private:
    bool sparklesRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectEverythingFall : public EffectCalc {
private:
    byte heat[WIDTH][HEIGHT];
    
    //const String getName() override {return String(FPSTR(T_EVERYTHINGFALL));}
    bool fire2012WithPalette(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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
  
  //const String getName() override {return String(FPSTR(T_FIRE2012));}
  bool fire2012Routine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectStarFall : public EffectCalc {
private:
    //const String getName() override {return String(FPSTR(T_SNOWSTORMSTARFALL));}
    bool snowStormStarfallRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override {FastLED.clear();}
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectLighters : public EffectCalc {
private:
    uint16_t lightersIdx;
    float lightersSpeed[2U][LIGHTERS_AM];
    uint8_t lightersColor[LIGHTERS_AM];
    float lightersPos[2U][LIGHTERS_AM];
    byte light[LIGHTERS_AM];

    //const String getName() override {return String(FPSTR(T_LIGHTERS));}
    bool lightersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class Effect3DNoise : public EffectCalc {
private:
//    uint16_t XY(uint8_t x, uint8_t y);
    /*
    const String getName() override {
        switch (effect)
        {
        case EFF_ENUM::EFF_MADNESS :
            return String(FPSTR(T_MADNESS));
        case EFF_ENUM::EFF_RAINBOW :
            return String(FPSTR(T_RAINBOW));
        case EFF_ENUM::EFF_RAINBOW_STRIPE :
            return String(FPSTR(T_RAINBOW_STRIPE));
        case EFF_ENUM::EFF_ZEBRA :
            return String(FPSTR(T_ZEBRA));
        case EFF_ENUM::EFF_FOREST :
            return String(FPSTR(T_FOREST));
        case EFF_ENUM::EFF_OCEAN :
            return String(FPSTR(T_OCEAN));
        case EFF_ENUM::EFF_PLASMA :
            return String(FPSTR(T_PLASMA));
        case EFF_ENUM::EFF_CLOUDS :
            return String(FPSTR(T_CLOUDS));
        case EFF_ENUM::EFF_LAVA :
            return String(FPSTR(T_LAVA));
        default:
            return EffectCalc::getName();
        }
    }
    */
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
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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

  //const String getName() {return String(FPSTR(T_SPIRO));}
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
    bool prismataRoutine(CRGB *leds, EffectWorker *param);
public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectFlock : public EffectCalc {
private:
  Boid boids[AVAILABLE_BOID_COUNT];
  Boid predator;
  PVector wind;

  bool predatorPresent;
  uint8_t hueoffset;

  //const String getName() {return String(FPSTR(T_FLOCK));}
  bool flockRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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

    /*
    const String getName() override {
        switch (effect)
        {
        case EFF_ENUM::EFF_RAINBOWCOMET :
            return String(FPSTR(T_RAINBOWCOMET));
        case EFF_ENUM::EFF_RAINBOWCOMET3 :
            return String(FPSTR(T_RAINBOWCOMET3));
        default:
            return EffectCalc::getName();
        }
    }
    */
    void drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color);
    void FillNoise(int8_t layer);
    bool rainbowCometRoutine(CRGB *leds, EffectWorker *param);
    bool rainbowComet3Routine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectSwirl : public EffectCalc {
private:
    //const String getName() {return String(FPSTR(T_SWIRL));}
    bool swirlRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectDrift : public EffectCalc {
private:
  uint8_t dri_phase;
  uint8_t _dri_speed;
  uint8_t _dri_delta;
  /*
  const String getName() override {
        switch (effect)
        {
        case EFF_ENUM::EFF_DRIFT :
            return String(FPSTR(T_DRIFT));
        case EFF_ENUM::EFF_DRIFT2 :
            return String(FPSTR(T_DRIFT2));
        default:
            return EffectCalc::getName();
        }
    }
    */
  bool incrementalDriftRoutine(CRGB *leds, EffectWorker *param);
  bool incrementalDriftRoutine2(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectTwinkles : public EffectCalc {
private:
  uint8_t thue = 0U;
  uint8_t tnum;
  CRGB ledsbuff[NUM_LEDS];
  //const String getName() {return String(FPSTR(T_TWINKLES));}
  bool twinklesRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectWaves : public EffectCalc {
private:
  uint8_t waveCount = speed % 2;
  const uint8_t waveScale = 256 / WIDTH;
  uint8_t waveRotation;
  uint8_t whue;
  uint8_t waveTheta;
  //const String getName() {return String(FPSTR(T_WAVES));}
  bool wavesRoutine(CRGB *leds, EffectWorker *param);

  /**
    * Vpalletemap - меняет указатель на текущую палитру из набора в соответствие с N-ным значением 
    * @param _val - N-ное значение 0-7
    * @param _pals - набор с палитрами
*/
virtual void WavesPaletteMap(std::vector<PGMPalette*> &_pals, const uint8_t _val);

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
  byte hue;
  //const String getName() {return String(FPSTR(T_RADAR));}
  bool radarRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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

  //const String getName() {return String(FPSTR(T_SMOKE));}
  void FillNoise(int8_t layer);     // TODO: join with Comet's
  bool multipleStreamSmokeRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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

  //const String getName() {return String(FPSTR(T_FIRE2018));}
  bool fire2018Routine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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
  uint8_t csum;   // reload checksum

  //const String getName() {return String(FPSTR(T_RINGS));}
  void ringsSet();
  bool ringsRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectCube2d : public EffectCalc {
private:
  uint8_t sizeX, sizeY; // размеры ячеек по горизонтали / вертикали
  uint8_t cntX, cntY; // количество ячеек по горизонтали / вертикали
  uint8_t fieldX, fieldY; // размер всего поля блоков по горизонтали / вертикали (в том числе 1 дополнительная пустая дорожка-разделитель с какой-то из сторон)
  uint8_t csum;   // reload checksum

  uint8_t pauseSteps; // осталось шагов паузы
  uint8_t shiftSteps=0; // всего шагов сдвига
  std::vector<int8_t> moveItems;     // индекс перемещаемого элемента
  //bool movedirection;   // направление смещения
  bool direction; // направление вращения в текущем цикле (вертикаль/горизонталь)

  //const String getName() {return String(FPSTR(T_CUBE2));}
  void cubesize();
  bool cube2dRoutine(CRGB *leds, EffectWorker *param);
  void cube2dmoveCols(uint8_t moveItem, bool movedirection);
  void cube2dmoveRows(uint8_t moveItem, bool movedirection);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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
    /*
    const String getName() override {
        switch (effect)
        {
        case EFF_ENUM::EFF_RAIN :
            return String(FPSTR(T_RAIN));
        case EFF_ENUM::EFF_COLORRAIN :
            return String(FPSTR(T_COLORRAIN));
        case EFF_ENUM::EFF_STORMYRAIN :
            return String(FPSTR(T_STORMYRAIN));
        default:
            return EffectCalc::getName();
        }
    }
    */
  void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm, bool fixRC = false);
  bool coloredRainRoutine(CRGB *leds, EffectWorker *param);
  bool stormyRainRoutine(CRGB *leds, EffectWorker *param);
  bool simpleRainRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

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
    Particle particles[20];
    unsigned numParticles = 0;
    void generate(bool reset = false);
    void position();
    bool picassoRoutine(CRGB *leds, EffectWorker *param);
    bool picassoRoutine2(CRGB *leds, EffectWorker *param);
    bool picassoRoutine3(CRGB *leds, EffectWorker *param);
    /*
    const String getName() //override
    {
        switch (effect)
        {
        case EFF_ENUM::EFF_PICASSO:
            return String(FPSTR(T_PICASSO));
        case EFF_ENUM::EFF_PICASSO2:
            return String(FPSTR(T_PICASSO2));
        case EFF_ENUM::EFF_PICASSO3:
            return String(FPSTR(T_PICASSO3));
        default:
            return EffectCalc::getName();
        }
    }
    */

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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
    void wu_pixel(uint32_t x, uint32_t y, CRGB col);
    bool leapersRoutine(CRGB *leds, EffectWorker *param);
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эффект "Вихри" адаптация SottNick
class EffectWhirl : public EffectCalc {
private:
    uint16_t ff_x;
    uint16_t ff_y;
    uint16_t ff_z;
    uint8_t hue;
    Boid boids[AVAILABLE_BOID_COUNT];
    uint8_t micPick = 0;

    static const uint8_t ff_speed = 1; // чем выше этот параметр, тем короче переходы (градиенты) между цветами. 1 - это самое красивое
    static const uint8_t ff_scale = 26; // чем больше этот параметр, тем больше "языков пламени" или как-то так. 26 - это норм

    bool whirlRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- цвет + вода в бассейне ("Аквариум")------------------
// (с) SottNick. 03.2020
class EffectAquarium : public EffectCalc {
private:
    uint8_t hue = 0;
    uint8_t deltaHue = 0U;
    uint8_t deltaHue2 = 0U;
    uint8_t step = 0;
    bool aquariumRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

#define STARS_NUM (16)
// ----------- Эффект "Звезды" 
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

//---------- Эффект "Фейерверк" адаптация kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373
class EffectFireworks : public EffectCalc {
private:
    DOTS_STORE store;
    uint16_t launchcountdown[SPARK];
    //uint8_t micPick = 0;
    bool fireworksRoutine(CRGB *leds, EffectWorker *param);
    void sparkGen();
    Dot gDot[SPARK];
    Dot gSparks[NUM_SPARKS];

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
//----- Эффект "Осциллограф" (c) kostyamat
class EffectOsc : public EffectCalc {
private:
    byte OSC_HV;
    byte spd;
    float pointer;
    const float _scaler = 3.3f / 1024;
    CRGB color = CHSV(255, 200, 200);
    const float center = (float)HEIGHT / 2; 
    void wu_pixel(uint32_t x, uint32_t y, CRGB col);
    bool oscRoutine(CRGB *leds, EffectWorker *param);
    float div;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};
#endif

// ------ Эффект "Вышиванка" (с) проект Aurora "Munch"
class EffectMunch : public EffectCalc {
private:
    byte count = 0;
    byte dir = 1;
    byte flip = 0;
    byte generation = 0;
    byte mic[2];

    bool munchRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------ Эффект "Цветной шум" (с) https://gist.github.com/StefanPetrick/c856b6d681ec3122e5551403aabfcc68
class EffectNoise : public EffectCalc {
private:

    uint8_t CentreX =  (WIDTH / 2) - 1;
    uint8_t CentreY = (HEIGHT / 2) - 1;
    uint32_t x[NUM_LAYERS];
    uint32_t y[NUM_LAYERS];
    uint32_t z[NUM_LAYERS];
    uint32_t scale_x[NUM_LAYERS];
    uint32_t scale_y[NUM_LAYERS];
    uint8_t  noise[1][16][16];
    
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
// (с) Сотнег, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49189
class EffectButterfly : public EffectCalc {
private:
    float butterfly2Pos[2U][LIGHTERS_AM *2];
    float butterfly2Speed[2U][LIGHTERS_AM *2];
    float butterfly2Turn[LIGHTERS_AM *2];
    uint8_t butterfly2Color[LIGHTERS_AM *2];
    uint8_t light2[LIGHTERS_AM *2];
    byte step = 0;
    byte csum = 0;
    
    bool butterflyRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};
// --------- конец секции эффектов 

class EffectWorker {
private:
    SORT_TYPE effSort; // порядок сортировки в UI
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    EFFFLAGS flags; // подумать нужен ли он здесь...
    uint16_t curEff = (uint16_t)EFF_NONE;     ///< энумератор текущего эффекта
    uint16_t selEff = (uint16_t)EFF_NONE;     ///< энумератор выбранного эффекта (для отложенного перехода)

    String originalName;    // имя эффекта дефолтное
    String effectName;      // имя эффекта (предварительно заданное или из конфига)
    uint8_t version;        // версия эффекта

    LList<EffectListElem*> effects; // список эффектов с флагами из индекса
    LList<UIControl*> controls; // список контроллов текущего эффекта
    LList<UIControl*> selcontrols; // список контроллов выбранного эффекта (пока еще идет фейдер)

    /**
     * создает и инициализирует экземпляр класса выбранного эффекта
     *
    */
    void workerset(uint16_t effect, const bool isCfgProceed = true);

    EffectWorker(const EffectWorker&);  // noncopyable
    EffectWorker& operator=(const EffectWorker&);  // noncopyable

    void clearEffectList(); // очистка списка эффектов, вызываетсяч в initDefault
    void clearControlsList(); // очистка списка контроллов и освобождение памяти
    
    //void initDefault();
    void effectsReSort(SORT_TYPE st=(SORT_TYPE)(255));

    int loadeffconfig(const uint16_t nb, const char *folder=nullptr);

    // получение пути и имени файла конфига эффекта
    const String geteffectpathname(const uint16_t nb, const char *folder=nullptr);

    /**
     * проверка на существование "дефолтных" конфигов для всех статичных эффектов
     * 
     */
    void chckdefconfigs(const char *folder);

    void savedefaulteffconfig(uint16_t nb, String &filename);
    void saveeffconfig(uint16_t nb, char *folder=nullptr);
    void makeIndexFile(const char *folder = nullptr);
    // создать или обновить текущий индекс эффекта
    void updateIndexFile();
    // удалить эффект из индексного файла
    void deleteFromIndexFile(const uint16_t effect);

    /**
     * получить версию эффекта из "прошивки" по его ENUM
     */
    const uint8_t geteffcodeversion(const uint8_t id);

    /**
     *  метод загружает и пробует десериализовать джейсон из файла в предоставленный документ,
     *  возвращает true если загрузка и десериализация прошла успешно
     *  @param doc - DynamicJsonDocument куда будет загружен джейсон
     *  @param jsonfile - файл, для загрузки
     */
    bool deserializeFile(DynamicJsonDocument& doc, const char* filepath);

    /**
     * процедура открывает индекс-файл на запись в переданный хендл,
     * возвращает хендл 
     */
    File& openIndexFile(File& fhandle, const char *folder);


public:
    std::unique_ptr<EffectCalc> worker = nullptr;           ///< указатель-класс обработчик текущего эффекта
    void initDefault(); // пусть вызывается позже и явно
    ~EffectWorker() { clearEffectList(); clearControlsList(); }

    LList<UIControl*>&getControls() { return isSelected() ? controls : selcontrols; }

    // дефолтный конструктор
    EffectWorker() : effects(), controls(), selcontrols() {

      if (!LittleFS.begin()){
          //LOG(println, F("ERROR: Can't mount filesystem!"));
          return;
      }

      for(int8_t id=0;id<3;id++){
        controls.add(new UIControl(
            id,                                     // id
            CONTROL_TYPE::RANGE,                    // type
            id==0 ? F("Яркость") : id==1 ? F("Скорость") : F("Масштаб"),           // name
            String(127),                            // value
            String(1),                              // min
            String(255),                            // max
            String(1)                               // step
        ));
        // selcontrols.add(new UIControl(
        //     id,                                     // id
        //     CONTROL_TYPE::RANGE,                    // type
        //     id==0 ? F("Яркость") : id==1 ? F("Скорость") : F("Масштаб"),           // name
        //     String(127),                            // value
        //     String(1),                              // min
        //     String(255),                            // max
        //     String(1)                               // step
        // ));
      }
      //workerset(EFF_NONE);
      selcontrols = controls;
    } // initDefault(); убрал из конструктора, т.к. крайне неудобно становится отлаживать..

    // тип сортировки
    void setEffSortType(SORT_TYPE type) {effSort = type; effectsReSort();}

    // конструктор копий эффектов
    EffectWorker(const EffectListElem* base, const EffectListElem* copy);
    // Конструктор для отложенного эффекта
    EffectWorker(uint16_t delayeffnb);
    // конструктор текущего эффекта, для fast=true вычитываетсяч только имя
    EffectWorker(const EffectListElem* eff, bool fast=false);

    // отложенная запись конфига текущего эффекта
    bool autoSaveConfig(bool force=false, bool reset=false);

    // пересоздает индекс с текущего списка эффектов
    void makeIndexFileFromList(const char *folder = nullptr);
    // пересоздает индекс с конфигов в ФС
    void makeIndexFileFromFS(const char *fromfolder = nullptr, const char *tofolder = nullptr);

    byte getModeAmount() {return effects.size();}

    const String &getEffectName() {return effectName;}
    const String &getOriginalName() {return originalName;}

    /**
    * вычитать только имя эффекта из конфиг-файла и записать в предоставленную строку
    * в случае отсутствия/повреждения взять имя эффекта из флеш-таблицы, если есть
    * для работы метода не требуется экземпляра класса effectCalc'а
    * @param effectName - String куда записать результат
    * @param nb  - айди эффекта
    * @param folder - какой-то префикс для каталога
    */
    void loadeffname(String& effectName, const uint16_t nb, const char *folder=nullptr);

    // текущий эффект или его копия
    const uint16_t getEn() {return curEff;}

    // следующий эффект, кроме canBeSelected==false
    uint16_t getNext();
    // предыдущий эффект, кроме canBeSelected==false
    uint16_t getPrev();
    // получить указанный
    uint16_t getBy(uint16_t select){ return select;}
    // перейти по предворительно выбранному

    void moveSelected();
    // перейти на количество шагов, к ближйшему большему (для DEMO)
    
    void moveByCnt(byte cnt){ uint16_t eff = getByCnt(cnt); directMoveBy(eff); }
    // получить номер эффекта смещенного на количество шагов (для DEMO)
    uint16_t getByCnt(byte cnt);
    // перейти на указанный в обход нормального переключения, использовать только понимая что это (нужно для начальной инициализации и переключений выключенной лампы)
    void directMoveBy(uint16_t select);
    // вернуть первый элемент списка
    EffectListElem *getFirstEffect();
    // вернуть следующий эффект
    EffectListElem *getNextEffect(EffectListElem *current);
    // вернуть выбранный элемент списка
    EffectListElem *getEffect(uint16_t select);
    // вернуть текущий
    uint16_t getCurrent() {return curEff;}
    // вернуть текущий элемент списка
    EffectListElem *getCurrentListElement();
    // вернуть выбранный
    uint16_t getSelected() {return selEff;}
    // вернуть выбранный элемент списка
    EffectListElem *getSelectedListElement();
    void setSelected(const uint16_t effnb);
    bool isSelected(){ return (curEff == selEff); }
    // копирование эффекта
    void copyEffect(const EffectListElem *base);
    // удалить эффект
    void deleteEffect(const EffectListElem *eff);
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