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
#include "misc.h"
#include "LList.h"
#include "patterns.h"
#include "color_palette.h"

// #define DEFAULT_SLIDER 127
// #define PARAM_BUFSIZE 128
#define IDX_ITEMBUFFSIZE    25      // буфер для джейсон элемента индекса
//#define FILEIO_BUFFSIZE    256      // буфер IO для работы с файлами (делаем равный странице littlefs)

//-------------------------------------------------
class EffectWorker;

typedef struct {
    union {
        uint8_t flags;
        struct {
            bool isMicOn:1;
            bool isDebug:1;
        };
    };
} LAMPSTATE;


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
    LAMPSTATE *lampstate = nullptr;
    LList<UIControl *> *ctrls;
    String dummy; // дефолтная затычка для отсутствующего контролла, в случае приведения к целому получится "0"
    bool active = false;          /**< работает ли воркер и был ли обсчет кадров с момента последнего вызова, пока нужно чтобы пропускать холостые кадры */
    bool isCtrlPallete = false; // признак наличия контрола палитры
    bool isMicActive = false; // признак включенного микрофона
protected:
    EFF_ENUM effect;        /**< энумератор эффекта */
    bool isDebug() {return lampstate!=nullptr ? lampstate->isDebug : false;}
    bool isActive() {return active;}
    void setActive(bool flag) {active=flag;}
    uint32_t lastrun=0;     /**< счетчик времени для эффектов с "задержкой" */
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
        //return (idx<ctrls->size() && idx>=0) ? (*ctrls)[idx]->getVal() : dummy;

        // Добавлена поддержка вариантов следования индексов контролов вида 0,1,2,5,7 т.е. с пропусками
        if(idx<ctrls->size() && idx>=0 && idx<=2 && (*ctrls)[idx]->getId()==idx){
            return (*ctrls)[idx]->getVal();
        } else {
            for(int i = 3; i<ctrls->size(); i++){
                if((*ctrls)[i]->getId()==idx) return (*ctrls)[i]->getVal();
            }
        }
        return dummy;
    }

public:

    bool isMicOn() {return isMicActive;}

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
     * @param _eff - энумератор эффекта
     * @param _controls - контролы эффекта
     * @param _state - текущее состояние лампы
     *
    */
    void init(EFF_ENUM _eff, LList<UIControl*>* _controls, LAMPSTATE* _state);

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
    bool dryrun(float n=1.0, uint8_t delay = EFFECTS_RUN_TIMER);

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
    virtual void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min=1,  const uint8_t _max=255);

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
  static CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter = 5);
  static CRGB makeDarker( const CRGB& color, fract8 howMuchDarker = 5);
  static float randomf(float min, float max);
  static bool isInteger(float val);
  static bool Lightning(CRGB lightningColor = CHSV(30,90,255) /*CRGB(72, 72, 80)*/, uint8_t chanse = 72U);
  static void Clouds(uint8_t rhue = 2, bool flash = false);
  static void addGlitter(uint8_t chanceOfGlitter = 127);
  static void nightMode(CRGB *leds);

  /*
  static CRGB& piXY(CRGB *leds, byte x, byte y);
  static void screenscale( accum88 a, byte N, byte& screen, byte& screenerr);
  static void plot88(CRGB *leds, byte x, byte y, CRGB& color);
  static int16_t scale15by8_local( int16_t i, fract8 scale );
*/

    static uint32_t getPixColor(uint32_t thisSegm); // функция получения цвета пикселя по его номеру
    static uint32_t getPixColorXY(uint16_t x, uint16_t y); // функция получения цвета пикселя в матрице по его координатам
    static void fillAll(const CRGB &color); // залить все
    static void drawPixelXY(int16_t x, int16_t y, const CRGB &color); // функция отрисовки точки по координатам X Y
    static void drawPixelXYF(float x, float y, const CRGB &color, uint8_t darklevel=25); // darklevel - насколько затемнять картинку
    static void drawPixelXYF_Y(uint16_t x, float y, const CRGB &color, uint8_t darklevel=50);
    static void drawPixelXYF_X(float x, uint16_t y, const CRGB &color, uint8_t darklevel=50);

    static CRGB getPixColorXYF_X(float x, uint16_t y);
    static CRGB getPixColorXYF_Y(uint16_t x, float y);
    static CRGB getPixColorXYF(float x, float y);

    static void drawLine(int x1, int y1, int x2, int y2, const CRGB &color);
    static void drawLineF(float x1, float y1, float x2, float y2, const CRGB &color);
    static void drawCircle(int x0, int y0, int radius, const CRGB &color);
    static void drawCircleF(float x0, float y0, float radius, const CRGB &color, bool fill = false, float step = 0.25);
    static void setLedsfadeToBlackBy(uint16_t idx, uint8_t val);
    static void setLedsNscale8(uint16_t idx, uint8_t val);
    static void dimAll(uint8_t value);
    static CRGB getLed(uint16_t idx);
    static void blur2d(uint8_t val);
    static CRGB *setLed(uint16_t idx, CHSV val);
    static CRGB *setLed(uint16_t idx, CRGB val);

    /** аналог ардуино функции map(), но только для float
   */
    static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max){
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

#ifdef MIC_EFFECTS
class EffectFreq : public EffectCalc {
private:
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
};
#endif

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
    bool bBallsRoutine(CRGB *leds, EffectWorker *param);
    void setDynCtrl(UIControl*_val) override;
    void setscl(const byte _scl) override; // перегрузка для масштаба
    //void setspd(const byte _spd) override; // перегрузка для скорости
    void regen();
    void load() override;
public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- пейнтбол -------------
class EffectLightBalls : public EffectCalc {
private:
    bool lightBallsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= Эффект "Огненная лампа" =================
class EffectFire : public EffectCalc {
private:
  uint8_t delaytype; // тип задержки (отладка)
  uint8_t pcnt;
  uint8_t shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
  uint8_t line[WIDTH];
  uint8_t shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)
  unsigned char matrixValue[8][16];


    void drawFrame(uint8_t pcnt, bool isColored);
    void generateLine();
    void shiftUp();
    bool fireRoutine(CRGB *leds, EffectWorker *param);
    void setDynCtrl(UIControl*_val);

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
    bool lighterTracersRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
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

    bool matrixRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- снегопад ----------
class EffectSnow : public EffectCalc {
private:
    float nextFrame;
    bool snowRoutine(CRGB *leds, EffectWorker *param);


public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Конфетти"
class EffectSparcles : public EffectCalc {
private:

    bool sparklesRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

class EffectEverythingFall : public EffectCalc {
private:
    byte heat[WIDTH][HEIGHT];

    bool fire2012WithPalette(CRGB *leds, EffectWorker *param);

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
    const uint8_t fireSmoothing = 60U; // 90
    uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];



  bool fire2012Routine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------------- звездопад/метель -------------
class EffectStarFall : public EffectCalc {
private:
    bool colored = false;
    bool isOld = false;
    bool snowStormStarfallRoutine(CRGB *leds, EffectWorker *param);
    void setDynCtrl(UIControl*_val) override;

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

    bool lightersRoutine(CRGB *leds, EffectWorker *param);

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
    bool prismataRoutine(CRGB *leds, EffectWorker *param);
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
  void setDynCtrl(UIControl*_val) override;
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
    uint8_t eNs_noisesmooth;
    uint32_t e_x[NUM_LAYERS];
    uint32_t e_y[NUM_LAYERS];
    uint32_t e_z[NUM_LAYERS];
    uint32_t e_scaleX[NUM_LAYERS];
    uint32_t e_scaleY[NUM_LAYERS];
    uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];


    const uint8_t e_centerX =  (WIDTH / 2) -  ((WIDTH - 1) & 0x01);
    const uint8_t e_centerY = (HEIGHT / 2) - ((HEIGHT - 1) & 0x01);

    void drawFillRect2_fast(int8_t x1, int8_t y1, int8_t x2, int8_t y2, CRGB color);
    void FillNoise(int8_t layer);

    bool rainbowCometRoutine(CRGB *leds, EffectWorker *param);
    bool rainbowComet3Routine(CRGB *leds, EffectWorker *param);
    bool firelineRoutine(CRGB *leds, EffectWorker *param);
    bool fractfireRoutine(CRGB *leds, EffectWorker *param);
    bool flsnakeRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ============= SWIRL /  ВОДОВОРОТ ===============
// Prismata Loading Animation
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither by PalPalych 12.04.2020
// Aurora: https://github.com/pixelmatix/aurora
// Copyright (c) 2014 Jason Coon
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
  uint8_t waveCount = 1;
  const uint8_t waveScale = 256 / WIDTH;
  uint8_t waveRotation;
  uint8_t whue;
  float waveTheta;
  bool wavesRoutine(CRGB *leds, EffectWorker *param);
  void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
  void setscl(const byte _scl) override;
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
  void setDynCtrl(UIControl*_val) override;

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
  void setDynCtrl(UIControl*_val) override;

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


  void ringsSet();
  bool ringsRoutine(CRGB *leds, EffectWorker *param);

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
  uint8_t shiftSteps=0; // всего шагов сдвига
  std::vector<int8_t> moveItems;     // индекс перемещаемого элемента
  //bool movedirection;   // направление смещения
  bool direction; // направление вращения в текущем цикле (вертикаль/горизонталь)
  uint8_t storage[WIDTH][HEIGHT];
  int8_t globalShiftX, globalShiftY;
  uint8_t gX, gY;
  bool seamlessX = false;

  //CRGB *ledbuff = new CRGB[NUM_LEDS];
  std::vector<CRGB> ledbuff;
  //CRGB ledbuff[(WIDTH + WIDTH/2) * (HEIGHT + HEIGHT/2)];


  void swapBuff();
  void cubesize();
  bool cube2dRoutine(CRGB *leds, EffectWorker *param);
  bool cube2dClassicRoutine(CRGB *leds, EffectWorker *param);
  void cube2dmoveCols(uint8_t moveItem, bool movedirection);
  void cube2dmoveRows(uint8_t moveItem, bool movedirection);
  void setDynCtrl(UIControl*_val) override;
  void setscl(const byte _scl) override;

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ===== Эффект "Шторм" адаптация и рефакторинг kostyamat
// https://github.com/marcmerlin/FastLED_NeoMatrix_SmartMatrix_LEDMatrix_GFX_Demos/blob/master/FastLED/Sublime_Demos/Sublime_Demos.ino
// v1.0 - Updating for GuverLamp v1.7 by SottNick 17.04.2020
// там по ссылке ещё остались эффекты с 3 по 9 (в SimplePatternList перечислены)
class EffectRain : public EffectCalc {
private:

  uint8_t rhue;
  uint8_t nline[WIDTH];
  uint8_t noise3d[NUM_LAYERS][WIDTH][HEIGHT];
  float speedfactor;
  uint8_t myScale8(uint8_t x);
  bool clouds = false;
  bool storm = false;
  bool splashes = true;

  void setDynCtrl(UIControl*_val) override;
  void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor);
  //bool coloredRainRoutine(CRGB *leds, EffectWorker *param);
  //bool stormyRainRoutine(CRGB *leds, EffectWorker *param);
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
    void setDynCtrl(UIControl*_val) override;
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
    void setDynCtrl(UIControl*_val) override;
};

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
    void setDynCtrl(UIControl*_val) override;
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

// ------------- цвет + вода в бассейне ------------------
// (с) SottNick. 03.2020
// переписал на программные субпиксельные блики - (c) kostyamat
class EffectAquarium : public EffectCalc {
private:
    float hue = 0.;
    //uint8_t deltaHue = 0U;
    //uint8_t deltaHue2 = 0U;
    //uint8_t step = 0;
    byte ledbuff[WIDTH*2 * HEIGHT*2];
    uint16_t xsin;
    uint16_t ysin;
    byte satur;
    byte glare = 0;
    byte iconIdx = 0;

    void nGlare();
    void nPatterns();
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

//---------- Эффект "Фейерверк"
//адаптация и переписал - kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373
class EffectFireworks : public EffectCalc {
private:
    DOTS_STORE store;
    uint16_t launchcountdown[SPARK];
    byte dim;
    uint8_t valDim;
    bool flashing = false;
    bool fireworksRoutine(CRGB *leds, EffectWorker *param);
    void sparkGen();
    Dot gDot[SPARK];
    Dot gSparks[NUM_SPARKS];
    void setDynCtrl(UIControl*_val) override;

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
    byte _rv;
    bool micA;
    float y[2] = {0.0f, 0.0f};
    uint32_t lastrun=0;     /**< счетчик времени для эффектов с "задержкой" */
    void setDynCtrl(UIControl*_val) override;
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
// (с) Сотнег, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49262
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
    bool wings = false;
    bool isColored = true;

    bool butterflyRoutine(CRGB *leds, EffectWorker *param);
    void load() override;
public:
    //void load() override;
    virtual void setDynCtrl(UIControl*_val) override;
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
// https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/patterns.ino
class EffectPatterns : public EffectCalc {
private:
    int8_t patternIdx = -1;
    int8_t lineIdx = 0;
    bool loadingFlag = true;
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

    void setDynCtrl(UIControl*_val);
    void drawPicture_XY(uint8_t iconIdx);
    bool patternsRoutine(CRGB *leds, EffectWorker *param);

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};


// ***************************** "Стрелки" *****************************
class EffectArrows : public EffectCalc {
private:
    bool loadingFlag = true;
    byte csum = 0;
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
    float speedfactor;
    bool subpixel;
    float prevVal[2];
    void arrowSetupForMode(byte mode, bool change);
    void arrowSetup_mode1();
    void arrowSetup_mode2();
    //void arrowSetup_mode3(;)
    void arrowSetup_mode4();

    bool arrowsRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ---- Эффект "Узоры"
// Идея https://github.com/vvip-68/GyverPanelWiFi/blob/master/firmware/GyverPanelWiFi_v1.02/patterns.ino
// (c) kostyamat
class EffectPatternsOrig : public EffectCalc {
private:
    int8_t patternIdx = -1;
    int8_t lineIdx = 0;
    bool loadingFlag = true;
    bool dir = false;
    byte csum = 0;
    byte _bri = 255U;
    byte buff[20 * 20];
    uint32_t xsin, ysin;
    unsigned long lastrun2;
    byte _sc = 0;
    int8_t _speedX, _speedY;

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

    void setDynCtrl(UIControl*_val);
    void drawPicture_XY(uint8_t iconIdx);
    bool patternsRoutine(CRGB *leds, EffectWorker *param);

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
    byte balls = 1;

    void balls_timer();
    void blur(CRGB *leds);
    bool nballsRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ------ Эффект "Притяжение"
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
    void setDynCtrl(UIControl*_val) override;
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
    void setDynCtrl(UIControl*_val) override;
public:
    //void load();
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//------------ Эффект "Змеиный Остров"
// База паттерн "Змейка" из проекта Аврора, перенос и субпиксель - kostyamat
class EffectSnake2 : public EffectCalc
{
private:
    float hue;
    float speedFactor;
    static const uint8_t snakeCount = WIDTH / 2U;
    bool subPix = false;
    bool disko;

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

        void shuffleDown(float speedFactor)
        {
            internal_counter += speedFactor * internal_speedf;

            if (internal_counter > 1.0)
            {
                for (byte i = (byte)SNAKE_LENGTH - 1; i > 0; i--)
                {
                    pixels[i] = pixels[i - 1];
                }
                double f;
                internal_counter = modf(internal_counter, &f);
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
            switch (direction)
            {
            case UP:
                pixels[0].y = pixels[0].y >= HEIGHT ? speedfactor : (pixels[0].y + speedfactor);
                break;
            case LEFT:
                pixels[0].x = pixels[0].x >= WIDTH ? speedfactor : (pixels[0].x + speedfactor);
                break;
            case DOWN:
                pixels[0].y = pixels[0].y <= 0 ? HEIGHT - speedfactor : pixels[0].y - speedfactor;
                break;
            case RIGHT:
                pixels[0].x = pixels[0].x <= 0 ? WIDTH - speedfactor : pixels[0].x - speedfactor;
                break;
            }
        }

        void draw(CRGB colors[SNAKE_LENGTH], float speedfactor, int snakenb, bool subpix);
    };

    Snake snakes[snakeCount];
    void setDynCtrl(UIControl *_val) override;
    bool snakeRoutine(CRGB *leds, EffectWorker *param);

public:
    //void load();
    bool run(CRGB *ledarr, EffectWorker *opt = nullptr) override;
};

// --------------  Эффект "Цветение"
// (c) Idir Idir (Soulmate)
class EffectFlower : public EffectCalc {
private:
    float c = 0.5;   //diameter
    float angle = 1.;
    float  counter = 0.;
    CHSV color;

    bool flowerRoutine(CRGB *leds, EffectWorker *param);
    //void setDynCtrl(UIControl*_val) override;

public:
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

//------------ Эффект "Дождь за окном..."
// (c) Idir Idir (Soulmate) переделан кардинально (с)kostyamat
class EffectCRain : public EffectCalc
{
private:
    byte rain[NUM_LEDS];

    float counter = 1.0;
    float _speed = 0;
    float hue;
    bool storm = false;
    bool clouds = false;
    void setDynCtrl(UIControl*_val) override;

    void changepattern();
    void raininit(byte rain[NUM_LEDS]);
    void updaterain(CRGB *leds, float speedFactor);
    bool crainRoutine(CRGB *leds, EffectWorker *param);

public:
    bool run(CRGB *ledarr, EffectWorker *opt = nullptr) override;
    void load() override;
};

// ----------- Эфеект "ДНК"
// База https://pastebin.com/jwvC1sNF адаптация и доработки kostyamat
class EffectDNA : public EffectCalc {
private:
    double freq = 3000;
    float mn =255.0/13.8;
    uint8_t speeds = 30;

    bool DNARoutine(CRGB *leds, EffectWorker *param);
    //void setDynCtrl(UIControl*_val) override;

public:
    //void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эффект "Огненная Лампа"
// База https://pastebin.com/eKqe4zzA переделака на субпиксель и доработки - kostyamat
class EffectFire2020 : public EffectCalc {
private:
 /*   int scale = 60; // scale of fire
    int speed = 20;   //speed of effect
*/
    #define NOISE_HEIGHT  (LED_COLS * 4U)
    uint16_t noises[LED_COLS * NOISE_HEIGHT];   //precalculated noise table
    byte colorfade[LED_ROWS];                   //simple colorfade table for speedup
    byte a = 0;
    byte _pal = 8;
    byte _scale = 60;
    byte csum = 0;
    void adjust_gamma(CRGB *leds)
    {
        for (uint16_t i = 0; i < NUM_LEDS; i++)
        {
            leds[i].r = dim8_video(leds[i].r);
            leds[i].g = dim8_video(leds[i].g);
            leds[i].b = dim8_video(leds[i].b);
        }
    }

    bool fire2020Routine(CRGB *leds, EffectWorker *param);
    void setDynCtrl(UIControl*_val) override;
    void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
    void palettesload() override;
    void regenNoise();

public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// ----------- Эфеект "Змейки"
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
    void setDynCtrl(UIControl*_val) override;
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
    uint8_t NUM_ROCKETS = 10;
    float gravity = 15;
    bool blurred = false;
    bool revCol = false;
    bool tiltDirec;
    float speedfactor;

    typedef struct
    {
        int32_t x, y, xd, yd;
    } Rocket;

    std::vector<Rocket> rockets;

    void restart_rocket(uint8_t r);
    void paint(CRGB *leds);
    void move();
    bool popcornRoutine(CRGB *leds, EffectWorker *param);
    void setDynCtrl(UIControl*_val) override;
    void setscl(const byte _scl) override; // перегрузка для масштаба
    //void setspd(const byte _spd) override; // перегрузка для скорости


public:
    void load() override;
    bool run(CRGB *ledarr, EffectWorker *opt=nullptr) override;
};

// --------- конец секции эффектов

class EffectWorker {
private:
    LAMPSTATE *lampstate; // ссылка на состояние лампы
    SORT_TYPE effSort; // порядок сортировки в UI
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    EFFFLAGS flags; // подумать нужен ли он здесь...
    uint16_t curEff = (uint16_t)EFF_NONE;     ///< энумератор текущего эффекта
    uint16_t selEff = (uint16_t)EFF_NONE;     ///< энумератор выбранного эффекта (для отложенного перехода)

    String originalName;    // имя эффекта дефолтное
    String effectName;      // имя эффекта (предварительно заданное или из конфига)
    String soundfile;       // имя/путь к звуковому файлу (DF Player Mini)
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
    EffectWorker(LAMPSTATE *_lampstate) : effects(), controls(), selcontrols() {
      lampstate = _lampstate;
      if (!LittleFS.begin()){
          //LOG(println, F("ERROR: Can't mount filesystem!"));
          return;
      }

      for(int8_t id=0;id<3;id++){
        controls.add(new UIControl(
            id,                                     // id
            CONTROL_TYPE::RANGE,                    // type
            id==0 ? String(FPSTR(TINTF_00D)) : id==1 ? String(FPSTR(TINTF_087)) : String(FPSTR(TINTF_088)),           // name
            String(127),                            // value
            String(1),                              // min
            String(255),                            // max
            String(1)                               // step
        ));
        // selcontrols.add(new UIControl(
        //     id,                                     // id
        //     CONTROL_TYPE::RANGE,                    // type
        //     id==0 ? String(FPSTR(TINTF_00D)) : id==1 ? String(FPSTR(TINTF_087)) : String(FPSTR(TINTF_088)),           // name
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
    void setEffSortType(SORT_TYPE type) {if(effSort != type) { effectsReSort(type); } effSort = type;}

    // конструктор копий эффектов
    EffectWorker(const EffectListElem* base, const EffectListElem* copy);
    // Конструктор для отложенного эффекта
    EffectWorker(uint16_t delayeffnb);
    // конструктор текущего эффекта, для fast=true вычитываетсяч только имя
    EffectWorker(const EffectListElem* eff, bool fast=false);

    // отложенная запись конфига текущего эффекта
    bool autoSaveConfig(bool force=false, bool reset=false);
    // удалить конфиг переданного эффекта
    void removeConfig(const uint16_t nb, const char *folder=nullptr);
    // пересоздает индекс с текущего списка эффектов
    void makeIndexFileFromList(const char *folder = nullptr);
    // пересоздает индекс с конфигов в ФС
    void makeIndexFileFromFS(const char *fromfolder = nullptr, const char *tofolder = nullptr);

    byte getModeAmount() {return effects.size();}

    const String &getEffectName() {return effectName;}
    void setEffectName(const String &name, EffectListElem*to) // если текущий, то просто пишем имя, если другой - создаем экземпляр, пишем, удаляем
        {if(to->eff_nb==curEff) effectName=name; else {EffectWorker *tmp=new EffectWorker(to); tmp->curEff=to->eff_nb; tmp->selEff=to->eff_nb; tmp->setEffectName(name,to); tmp->saveeffconfig(to->eff_nb); delete tmp;} }

    const String &getSoundfile() {return soundfile;}
    void setSoundfile(const String &_soundfile, EffectListElem*to) // если текущий, то просто пишем имя звукового файла, если другой - создаем экземпляр, пишем, удаляем
        {if(to->eff_nb==curEff) soundfile=_soundfile; else {EffectWorker *tmp=new EffectWorker(to); tmp->curEff=to->eff_nb; tmp->selEff=to->eff_nb; tmp->setSoundfile(_soundfile,to); tmp->saveeffconfig(to->eff_nb); delete tmp;} }
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

    /**
    * вычитать только имя\путь звука из конфиг-файла и записать в предоставленную строку
    * в случае отсутствия/повреждения возвращает пустую строку
    * @param effectName - String куда записать результат
    * @param nb  - айди эффекта
    * @param folder - какой-то префикс для каталога
    */
    void loadsoundfile(String& effectName, const uint16_t nb, const char *folder=nullptr);

    // текущий эффект или его копия
    const uint16_t getEn() {return curEff;}
    //const uint16_t

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