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
//#include <FastLed.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#endif
#include "effects_types.h"


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
EFF_CUBE2                                     // Куб 2D
#ifdef MIC_EFFECTS
,EFF_FREQ = (99U)                             // Частотный анализатор (служебный, смещаем в конец)
#endif
} EFF_ENUM;

void sparklesRoutine(CRGB*, const char*);
void fireRoutine(CRGB*, const char*);
void whiteColorStripeRoutine(CRGB*, const char*);
void fire2012WithPalette(CRGB*, const char*);
void pulseRoutine(CRGB*, const char*);
void rainbowDiagonalRoutine(CRGB*, const char*);
void colorsRoutine(CRGB*, const char*);
void matrixRoutine(CRGB*, const char*);
void snowRoutine(CRGB*, const char*);
void snowStormStarfallRoutine(CRGB*, const char*);
void lightersRoutine(CRGB*, const char*);
void ballsRoutine(CRGB*, const char*);
void lightBallsRoutine(CRGB*, const char*);
void ballRoutine(CRGB*, const char*);
void madnessNoiseRoutine(CRGB*, const char*);
void rainbowNoiseRoutine(CRGB*, const char*);
void rainbowStripeNoiseRoutine(CRGB*, const char*);
void zebraNoiseRoutine(CRGB*, const char*);
void forestNoiseRoutine(CRGB*, const char*);
void oceanNoiseRoutine(CRGB*, const char*);
void plasmaNoiseRoutine(CRGB*, const char*);
void cloudsNoiseRoutine(CRGB*, const char*);
void lavaNoiseRoutine(CRGB*, const char*);
void BBallsRoutine(CRGB*, const char*);
void Sinusoid3Routine(CRGB*, const char*);
void metaBallsRoutine(CRGB*, const char*);
void spiroRoutine(CRGB*, const char*);
void rainbowCometRoutine(CRGB*, const char*);
void rainbowComet3Routine(CRGB*, const char*);
void prismataRoutine(CRGB*, const char*);
void flockRoutine(CRGB*, const char*);
void swirlRoutine(CRGB*, const char*);
void incrementalDriftRoutine(CRGB*, const char*);
void incrementalDriftRoutine2(CRGB*, const char*);
void twinklesRoutine(CRGB*, const char*);
void radarRoutine(CRGB*, const char*);
void wavesRoutine(CRGB*, const char*);
void fire2012Routine(CRGB*, const char*);
void coloredRainRoutine(CRGB*, const char*);
void simpleRainRoutine(CRGB*, const char*);
void stormyRainRoutine(CRGB*, const char*);
void fire2018Routine(CRGB*, const char*);
void ringsRoutine(CRGB*, const char*);
void cube2dRoutine(CRGB*, const char *);
#ifdef MIC_EFFECTS
void freqAnalyseRoutine(CRGB*, const char*);
#endif
//-------------------------------------------------
const char _R255[] PROGMEM = "[{'R':'127'}]";

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
    void updateParam(const char *str) {
        if(param!=nullptr && param!=_R255) // херовая проверка, надобно будет потом выяснить как безопасно разпознать указатель на PROGMEM или на RAM
            delete [] param;
        param = new char[strlen(str)+1];
        strcpy(param, str);
    }
    String getValue(const char *src, const _PTR type){
        if(src==nullptr)
            return String(); // empty
        //LOG.printf_P(PSTR("TEST: %s\n"),src);
        DynamicJsonDocument doc(128);
        String tmp(FPSTR(src));
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
    void setValue(const char *src, const _PTR type, const _PTR val){
        DynamicJsonDocument doc(128);
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
        //LOG.println(tmp);
        updateParam(tmp.c_str());
    }
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

#ifdef MIC_EFFECTS
const char T_FREQ[] PROGMEM = "Частотный анализатор";
#endif

static EFFECT _EFFECTS_ARR[] = {
    {false, false, 127, 127, 127, EFF_NONE, nullptr, nullptr, nullptr},
    {true, true, 127, 127, 127, EFF_WHITE_COLOR, T_WHITE_COLOR, whiteColorStripeRoutine, nullptr},

    {true, true, 127, 127, 127, EFF_COLORS, T_COLORS, colorsRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW_2D, T_RAINBOW_2D, rainbowDiagonalRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SPARKLES, T_SPARKLES, sparklesRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SNOW, T_SNOW, snowRoutine, nullptr},    
    {true, true, 127, 127, 127, EFF_SNOWSTORMSTARFALL, T_SNOWSTORMSTARFALL, snowStormStarfallRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_MATRIX, T_MATRIX, matrixRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LIGHTERS, T_LIGHTERS, lightersRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LIGHTER_TRACES, T_LIGHTER_TRACES, ballsRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CUBE, T_CUBE, ballRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PULSE, T_PULSE, pulseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_EVERYTHINGFALL, T_EVERYTHINGFALL, fire2012WithPalette, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE, T_FIRE, fireRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PAINTBALL, T_PAINTBALL, lightBallsRoutine, nullptr},

    {true, true, 127, 127, 127, EFF_MADNESS, T_MADNESS, madnessNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CLOUDS, T_CLOUDS, cloudsNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_LAVA, T_LAVA, lavaNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_PLASMA, T_PLASMA, plasmaNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW, T_RAINBOW, rainbowNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOW_STRIPE, T_RAINBOW_STRIPE, rainbowStripeNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_ZEBRA, T_ZEBRA, zebraNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FOREST, T_FOREST, forestNoiseRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_OCEAN, T_OCEAN, oceanNoiseRoutine, nullptr},

    {true, true, 127, 127, 127, EFF_BBALS, T_BBALS, BBallsRoutine, nullptr}, 
    {true, true, 127, 127, 127, EFF_SINUSOID3, T_SINUSOID3, Sinusoid3Routine, nullptr},
    {true, true, 127, 127, 127, EFF_METABALLS, T_METABALLS, metaBallsRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SPIRO, T_SPIRO, spiroRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOWCOMET, T_RAINBOWCOMET, rainbowCometRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_RAINBOWCOMET3, T_RAINBOWCOMET3, rainbowComet3Routine, nullptr},
    {true, true, 127, 127, 127, EFF_PRIZMATA, T_PRIZMATA, prismataRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FLOCK, T_FLOCK, flockRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_SWIRL, T_SWIRL, swirlRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_DRIFT, T_DRIFT, incrementalDriftRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_DRIFT2, T_DRIFT2, incrementalDriftRoutine2, nullptr},
    {true, true, 127, 127, 127, EFF_TWINKLES, T_TWINKLES, twinklesRoutine, ((char *)_R255)}, // очень хреновое приведение типов, но дальше это разрулим :)
    {true, true, 127, 127, 127, EFF_RADAR, T_RADAR, radarRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_WAVES, T_WAVES, wavesRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE2012, T_FIRE2012, fire2012Routine, nullptr},
    {true, true, 127, 127, 127, EFF_RAIN, T_RAIN, simpleRainRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_COLORRAIN, T_COLORRAIN, coloredRainRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_STORMYRAIN, T_STORMYRAIN, stormyRainRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_FIRE2018, T_FIRE2018, fire2018Routine, nullptr},
    {true, true, 127, 127, 127, EFF_RINGS, T_RINGS, ringsRoutine, nullptr},
    {true, true, 127, 127, 127, EFF_CUBE2, T_CUBE2, cube2dRoutine, nullptr}
#ifdef MIC_EFFECTS
    ,{true, true, 127, 127, 127, EFF_FREQ, T_FREQ, freqAnalyseRoutine, nullptr}
#endif
};

#define bballsMaxNUM_BALLS     (16U)                // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
#define BALLS_AMOUNT           (7U)                 // максимальное количество "шариков"
#define LIGHTERS_AM            (64U)                // светлячки
#define NUM_LAYERS             (1U)                 // The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS2            (2U)                 // The coordinates for 3 16-bit noise spaces.
#define AVAILABLE_BOID_COUNT   (10U)                // стая, кол-во птиц
class SHARED_MEM {
public:
    union {
		struct { // spiroRoutine
            boolean spiroincrement;
            boolean spirohandledChange;
            byte spirohueoffset;
            uint8_t spirocount;
            float spirotheta1;
            float spirotheta2;
		};
        struct { // BouncingBalls2014
            uint8_t bballsCOLOR[bballsMaxNUM_BALLS] ;                   // прикручено при адаптации для разноцветных мячиков
            uint8_t bballsX[bballsMaxNUM_BALLS] ;                       // прикручено при адаптации для распределения мячиков по радиусу лампы
            int   bballsPos[bballsMaxNUM_BALLS] ;                       // The integer position of the dot on the strip (LED index)
            float bballsH[bballsMaxNUM_BALLS] ;                         // An array of heights
            float bballsVImpact[bballsMaxNUM_BALLS] ;                   // As time goes on the impact velocity will change, so make an array to store those values
            float bballsTCycle[bballsMaxNUM_BALLS] ;                    // The time since the last time the ball struck the ground
            float bballsCOR[bballsMaxNUM_BALLS] ;                       // Coefficient of Restitution (bounce damping)
            long  bballsTLast[bballsMaxNUM_BALLS] ;                     // The clock time of the last ground strike
		};
        struct {
            //-- 3D Noise эффектцы --------------
            uint8_t ihue;
            uint8_t colorLoop;
            uint16_t speed;                                        // speed is set dynamically once we've started up
            uint16_t scale;                                        // scale is set dynamically once we've started up
            uint16_t x;
            uint16_t y;
            uint16_t z;
            #if (WIDTH > HEIGHT)
            uint8_t noise[2*HEIGHT][WIDTH];
            #else
            uint8_t noise[2*WIDTH][HEIGHT];
            #endif
            char currentPalette[sizeof(CRGBPalette16)];
		};
        struct {
            int16_t ballColor;
            int8_t vectorB[2U];
            float coordB[2U];
		};
        struct {
            int8_t vector[BALLS_AMOUNT][2U];
            float coord[BALLS_AMOUNT][2U];
		};
        struct {
            uint16_t lightersIdx;
            int8_t lightersSpeed[2U][LIGHTERS_AM];
            uint8_t lightersColor[LIGHTERS_AM];
            float lightersPos[2U][LIGHTERS_AM];
		};
        struct { // радуги
            uint8_t hue;
		};
        struct { // pulse
            uint8_t pulse_hue;
            uint8_t pulse_step;
		};
        struct {
            // Array of temperature readings at each simulation cell
            byte heat[WIDTH][HEIGHT];
		};
        struct { // Огонь
            uint8_t pcnt;
            uint8_t shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
            uint8_t line[WIDTH];
            uint8_t shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)
            unsigned char matrixValue[8][16];
		};
        struct { // радужная комета
            uint8_t eNs_noisesmooth;
            uint8_t rhue;
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
            //uint8_t ledsbuff[sizeof(CRGB)* NUM_LEDS];
        };
        struct { // стая
            bool predatorPresent;
            uint8_t hueoffset;
            char predator[sizeof(Boid)];
            char wind[sizeof(PVector)];
            char boids[sizeof(Boid)*AVAILABLE_BOID_COUNT];
		};
        struct { // будильник "рассвет"
            uint8_t dawnCounter;                                           // счётчик первых шагов будильника
            time_t startmillis;
            CHSV dawnColorMinus[6];
		};
        struct { // дрифты
            uint8_t dri_phase;
		};
        struct { // мерцание
            uint8_t thue;
            CRGB ledsbuff[NUM_LEDS];
		};
        struct { // радар
            uint8_t eff_offset;        // глобальная переменная для работы эффектов (обычно применяется для циклического пересчета hue, количества кадров и др...)
            uint8_t eff_theta;         // глобальная переменная угла для работы эффектов
		};
        struct { // волны
            uint8_t whue;
            uint8_t waveTheta;
		};
        struct {
            int8_t peakX[2][WIDTH];
        };
        struct { // огонь2018
            uint32_t noise32_x[NUM_LAYERS2];
            uint32_t noise32_y[NUM_LAYERS2];
            uint32_t noise32_z[NUM_LAYERS2];
            uint32_t scale32_x[NUM_LAYERS2];
            uint32_t scale32_y[NUM_LAYERS2];
            uint8_t fire18heat[NUM_LEDS];
            uint8_t noise3dx[NUM_LAYERS2][WIDTH][HEIGHT];
        };
        struct { // кодовый замок
            uint8_t ringColor[HEIGHT]; // начальный оттенок каждого кольца (оттенка из палитры) 0-255
            uint8_t huePos[HEIGHT]; // местоположение начального оттенка кольца 0-WIDTH-1
            uint8_t shiftHueDir[HEIGHT]; // 4 бита на ringHueShift, 4 на ringHueShift2
            ////ringHueShift[ringsCount]; // шаг градиета оттенка внутри кольца -8 - +8 случайное число
            ////ringHueShift2[ringsCount]; // обычная скорость переливания оттенка всего кольца -8 - +8 случайное число
            uint8_t currentRing; // кольцо, которое в настоящий момент нужно провернуть
            uint8_t stepCount; // оставшееся количество шагов, на которое нужно провернуть активное кольцо - случайное от WIDTH/5 до WIDTH-3            
        };
        struct { // cube2d
            bool direction; // направление вращения в данный момент
            uint8_t pauseSteps; // осталось шагов паузы
            uint8_t currentStep; // текущий шаг сдвига (от 0 до GSHMEM.shiftSteps-1)
            uint8_t shiftSteps; // всего шагов сдвига (от 3 до 4)
            uint8_t gX, gY; // глобальный X и глобальный Y нашего "кубика"
            int8_t globalShiftX, globalShiftY; // нужно ли сдвинуть всё поле по окончаии цикла и в каком из направлений (-1, 0, +1)
            uint8_t storage[WIDTH][HEIGHT];
        };
		//uint8_t raw[1024];
	};
};

class EffectWorker {
private:
    const int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);

    EFF_ENUM curEff = EFF_NONE;
    unsigned int arrIdx = 0;
    unsigned int storedIdx = 0; // предыдущий эффект
    EFFECT* effects = _EFFECTS_ARR;

    EffectWorker(const EffectWorker&);  // noncopyable
    EffectWorker& operator=(const EffectWorker&);  // noncopyable
public:
    EffectWorker() {
        //loadConfig();       
    }

    ~EffectWorker() {}

    void loadConfig(const char *cfg = nullptr) {
        if(SPIFFS.begin()){
            File configFile;
            if(cfg == nullptr)
                configFile = SPIFFS.open(F("/eff_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
            else
                configFile = SPIFFS.open(cfg, "r"); // PSTR("r") использовать нельзя, будет исключение!
            String cfg_str = configFile.readString();

            if (cfg_str == F("")){
#ifdef LAMP_DEBUG
                LOG.println(F("Failed to open effects config file"));
#endif
                saveConfig();
                return;
            }

#ifdef LAMP_DEBUG
                LOG.println(F("\nStart desialization of effects\n\n"));
#endif

            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, cfg_str);
            if (error) {
#ifdef LAMP_DEBUG
                LOG.print(F("deserializeJson error: "));
                LOG.println(error.code());
#endif
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
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("(%d - %d - %d - %d - %d - %d - %s)\n"), nb, eff->brightness, eff->speed, eff->scale, eff->isFavorite, eff->canBeSelected, eff->param!=nullptr?FPSTR(eff->param):FPSTR(F("")));
#endif
            }
            // JsonArray::iterator it;
            // for (it=arr.begin(); it!=arr.end(); ++it) {
            //     const JsonObject& elem = *it;
            // }
#ifdef LAMP_DEBUG
            LOG.println(F("Effects config loaded"));
#endif
            doc.clear();
        }
    }

    void saveConfig(const char *cfg = nullptr) {
        if(SPIFFS.begin()){
            File configFile;
            if(cfg == nullptr)
                configFile = SPIFFS.open(F("/eff_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
            else
                configFile = SPIFFS.open(cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
            EFFECT *cur_eff;

            configFile.print("[");
            for(int i=1; i<MODE_AMOUNT; i++){ // EFF_NONE не сохраняем
                cur_eff = &(effects[i]);
                configFile.printf_P(PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?FPSTR(cur_eff->param):FPSTR(F(""))));
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (char*)(i>1?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?FPSTR(cur_eff->param):FPSTR(F(""))));
#endif
            }     
            configFile.print("]");
            configFile.flush();
            configFile.close();
            LOG.println(F("\nSave effects config"));
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

    void setBrightness(byte val) {effects[arrIdx].brightness = val;}
    void setSpeed(byte val) {effects[arrIdx].speed = val;}
    void setScale(byte val) {effects[arrIdx].scale = val;}
    byte getBrightness() { return effects[arrIdx].brightness; }
    byte getSpeed() { return effects[arrIdx].speed; }
    byte getScale() { return effects[arrIdx].scale; }
    byte getModeAmount() {return MODE_AMOUNT;}
    const char *getName() {return effects[arrIdx].eff_name;}
    const EFF_ENUM getEn() {return effects[arrIdx].eff_nb;}
    
    void moveNext() // следующий эффект, кроме canBeSelected==false
    {
        int i;

        for(i=arrIdx+1; i<MODE_AMOUNT; i++){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
        i = 1;
        for(; i<MODE_AMOUNT; i++){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
    }

    void movePrev() // предыдущий эффект, кроме canBeSelected==false
    { 
        int i;

        for(i=arrIdx-1; i>0; i--){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
        i = MODE_AMOUNT-1;
        for(; i>=0; i--){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
    }

    void moveBy(EFF_ENUM select){ // перейти по перечислению
        for(int i=MODE_AMOUNT-1; i>=0; i--){
            if(effects[i].eff_nb == select){
                arrIdx = i;
                curEff = effects[i].eff_nb;
            }
        }
    }

    void moveBy(byte cnt){ // перейти на количество шагов, к ближайшему большему (для DEMO)
        while(cnt){
            for(int i=arrIdx; i<MODE_AMOUNT; i++){
                if(cnt>0)
                    cnt--;
                if(effects[i].isFavorite && cnt==0){
                    arrIdx = i;
                    curEff = effects[i].eff_nb;
                    return;
                }
            }
            if(arrIdx == 0){ return; }
            else {
                arrIdx = 0;
                curEff = effects[0].eff_nb;
            }
        }
    }

    EFFECT *enumNextEffect(EFFECT *current){
        for(int i=0; i<MODE_AMOUNT; i++){
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
        return &(effects[arrIdx]);
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