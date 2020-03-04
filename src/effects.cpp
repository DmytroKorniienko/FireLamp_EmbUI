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

//----------------------------------------------------
void fadePixel(uint8_t i, uint8_t j, uint8_t step)          // новый фейдер
{
    int32_t pixelNum = myLamp.getPixelNumber(i, j);
    if (myLamp.getPixColor(pixelNum) == 0U) return;

    CRGB led = myLamp.getLeds(pixelNum);
    if (led.r >= 30U ||
        led.g >= 30U ||
        led.b >= 30U){
        myLamp.setLedsfadeToBlackBy(pixelNum,step);
    }
    else{
        myLamp.setLeds(pixelNum, 0U);
    }
}

// функция плавного угасания цвета для всех пикселей
void fader(uint8_t step)
{
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      fadePixel(i, j, step);
    }
  }
}
// ============= ЭФФЕКТЫ ===============

// ------------- конфетти --------------
#define EFF_FADE_OUT_SPEED        (70U)                         // скорость затухания
void sparklesRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    fader(5); return;
  } else {
    myLamp.setEffDelay(millis());
  }
  
  // static uint32_t chktime = millis();
  // LOG.println(millis()-chktime);
  // chktime = millis();
  
  for (uint8_t i = 0; i < myLamp.effects.getScale(); i++)
  {
    uint8_t x = random(0U, WIDTH);
    uint8_t y = random(0U, HEIGHT);
    if (myLamp.getPixColorXY(x, y) == 0U)
    {
      myLamp.setLeds(myLamp.getPixelNumber(x, y),CHSV(random(0U, 255U), random(0U, 255U), random(0U, 255U)));
    }
  }
  fader(EFF_FADE_OUT_SPEED);
}

// ------------- The Flame / Огонь -----------------
// The Flame Effect(beta) for GyverLamp (FastLED) by PalPalych from 27-02-2020
#define FIRE_UNIVERSE                        // универсальный огонь 2-в-1 Цветной+Белый
#define FIRE_WHITEVALUE           (255U)     // значение регулятора "Масштаб" при котором отображается белое пламя #100U
#define FIRE_OFFSET               (4U)       // сдвиг маски огня относительно края. для того, чтобы разместить очаг по центру = FIRE_FLAMEWIDTH/2 #4U

#define FIRE_SMOOTH               (3U)       // сглаживание за счет увеличения промежуточных кадров (0...255, 0 - выключено), #3U
                                             // зависимые параметры: FIRE_CORESTRENGTH, FIRE_CORERESIST, FIRE_FLAMESTRENGTH,FIRE_CORERESIST,
                                             // FIRE_BREATHSPEED, FIRE_SPARKDELAY

#define FIRE_CORESHIFT            (8U)       // допустимый сдвиг очага в пикселях относительно его центральной позиции #4U
#define FIRE_CORESTRENGTH         (36U)      // устойчивость очага от сдвига по строкам (1...255 , 0 = отключить колыхание) #12U
#define FIRE_CORERESIST           (16U)      // устойчивость очага от сдвига по кадрам (1...255) #2U
#define FIRE_COREAMPLITUDE        (1U)       // амплитуда сдвига очага (1...FIRE_COREWIDTH/2) #1U
#define FIRE_CORESEED             (16U)      // величина зерна огня #16U  

#define FIRE_FLAMESTRENGTH        (8U)       // устойчивость пламени от сдвига по строкам (1...255 , 0 = отключить колыхание) #2U
#define FIRE_FLAMERESIST          (3U)       // устойчивость пламени от сдвига по кадрам (1...255) #1U
#define FIRE_FLAMEAMPLITUDE       (1U)       // амплитуда сдвига пламени (1...FIRE_FLAMEWIDTH/4) #1U
#define FIRE_FLAMESEED            (4U)       // величина зерна оттенка соседних пикселей огня #8U

#define FIRE_COREEDGE             (24U)      // порог срабатывания от hueMask с градиентом +FIRE_FLAMESEED для ообозначения сердцевины огня (1...255 , 0 = отключить) #1U
#define FIRE_COREDEEP             (64U)      // глубина белизны сердцевины огня #64U

#define FIRE_FLAMECOEFF           (4U)       // делитель фактов поворота зеркала для некоторых режимов FIRE_FLAMEMIRROR 
#define FIRE_FLAMEMIRROR          (201U)     // отзеркаливание маски оттенка очага #201U
                                             //   0...99 - столб пламени, без смещения оттенка по строкам,
                                             //     0 - выключение отзеркливания / столб пламени
                                             //     1...49 - шаговая диффузия с амплитудой FIRE_FLAMEAMPLITUDE,
                                             //     50...99 - случайная диффузия с амплитудой FIRE_FLAMEAMPLITUDE,
                                             //  101...199 - колышащее пламя, с учетом смещения оттенка по строкам,
                                             //     100 - выключение отзеркливания / колышащее пламя (базовое отключение)
                                             //     101...149 - диффузия со смещением FIRE_FLAMEAMPLITUDE,
                                             //     150...199 - случайная диффузия со смещением FIRE_FLAMEAMPLITUDE,
                                             //   200...255 - колышащее пламя для каждого очага
                                             //     200 - зеркальное для каждого второго очага
                                             //     201 - колышащее пламя для каждого очага
                                             //     202...254 - случайное отзеркаливание со скоростью переключения, 

#define FIRE_PLUMEHEIGHTMIN       (3U)      // высота языков пламени минимальная, влияет на визуальную скорость подъема пламени (0...255) #3U
#define FIRE_PLUMEHEIGHTMAX       (9U)      // высота языков пламени максимальная (FIRE_PLUMEHEIGHTMIN...255) #9U
#define FIRE_PLUMEWIDTHMIN        (1U)      // ширина языков пламени минимальная (0...255) #31U
#define FIRE_PLUMEWIDTHMAX        (4U)      // ширина языков пламени максимальная (FIRE_PLUMEWIDTHMIN...255) #31U
#define FIRE_PLUMELIGHTMIN        (192U)    // минимальная яркость темной части языка пламени (0...255) #160U
#define FIRE_PLUMELIGHTMAX        (224U)    // максимальная яркость темной части языка пламени (0...255) #192U

#define FIRE_BREATHDEEP           (64U)     // дыхание пламени (0...255) 0 - выключено  #64U
#define FIRE_BREATHSPEED          (2U)      // скорость дыхание пламени за 1 кадр (0...255) #4U

#define FIRE_SPARKLES             (true)    // вылетающие искры вкл/выкл
#define FIRE_SPARKSBRIGHT         (128U)    // первоначальная яркость искр (255...0) #200U
#define FIRE_EVERYNSPARK          (48U)     // только каждая единичная из указанных искр будет выводится (0...255) #64U
#define FIRE_SPARKSTRENGTH        (3U)      // стойкость искр относительно напора ветра (1... , 0 = отключить колыхание) #3U
#define FIRE_SPARKBORNDEEP        (3U)      // глубина от края пламени, на которой зарождаются искры #3U
#define FIRE_SPARKDELAY           (4U)      // замедлитель скорости искр #2U

//#define FIRE_WINDCASE             (2U)      // случайные порывы ветра каждый 256 кадр (1...255 , 0 = отключить)  #2U

// ----- неперемещаемые/неизменные оперделения -----
#define FIRE_COREWIDTH            (8U)      // ширина очага (маски)
#define FIRE_FLAMEWIDTH           (8U)      // ширина пламени (маски)
#define FIRE_HEIGHT               (9U)      // высота очага (маски)

//these values are substracetd from the generated values to give a shape to the animation
#if ((FIRE_COREWIDTH == 16U) && (FIRE_HEIGHT == 8U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {32 , 0  , 0  , 16 , 16 , 0  , 0  , 32 , 32 , 0  , 0  , 16 , 16 , 0  , 0  , 32 },
  {64 , 0  , 0  , 8  , 8  , 0  , 0  , 64 , 64 , 0  , 0  , 8  , 8  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 8U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {32 , 0  , 0  , 16  , 16  , 0  , 0  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 192, 160, 128 , 128 , 160, 192, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 9U)) //PalPalych's Optimal Mod for 16x16
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {16 , 0  , 8  , 16  , 16  , 8  , 0  , 16 },
  {32 , 0  , 0  , 16  , 16  , 0  , 0  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 240, 192, 160 , 160 , 192, 240, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 10U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {24 , 0  , 8  , 16  , 16  , 8  , 0  , 24 },
  {32 , 4  , 0  , 12  , 12  , 0  , 4  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 192, 160, 128 , 128 , 160, 192, 255},
  {255, 240, 192, 160 , 160 , 192, 240, 255}
};
#else
#pragma message "Wrong value mask parameters!"
#endif

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
#if ((FIRE_FLAMEWIDTH == 16U) && (FIRE_HEIGHT == 8U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM =  {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1},
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1},
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1},
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1},
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1},
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0},
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0},
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0}
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 8U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 9U)) //PalPalych's Optimal Mod for 16x16
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {7 , 21, 25, 26, 26, 25, 21, 7 },
  {3 , 17, 24, 26, 26, 24, 17, 3 },
  {2 , 13, 21, 25, 25, 21, 13, 2 },
  {1 , 11, 18, 23, 23, 18, 11, 1 },
  {1 , 9 , 14, 20, 20, 14, 9 , 1 },
  {1 , 5 , 10, 16, 16, 10, 5 , 1 },
  {0 , 2 , 7 , 11, 11, 7 , 2 , 0 },
  {0 , 1 , 3 , 6 , 6 , 3 , 1 , 0 },
  {0 , 0 , 1 , 2 , 2 , 1 , 0 , 0 }
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 10U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {5 , 16, 25, 26, 26, 25, 16, 5 },
  {1 , 11, 22, 26, 26, 22, 11, 1 },
  {1 , 8 , 19, 25, 25, 19, 8 , 1 },
  {1 , 8 , 13, 19, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 3 , 6 , 6 , 3 , 0 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};
#else
#pragma message "Wrong hue mask parameters!"
#endif

const uint8_t _coreshiftmin = FIRE_FLAMEWIDTH - FIRE_CORESHIFT ;
const uint8_t _coreshiftmax = FIRE_FLAMEWIDTH + FIRE_CORESHIFT ;
const uint8_t _flameshiftmin = FIRE_FLAMEWIDTH - FIRE_FLAMEAMPLITUDE ;
const uint8_t _flameshiftmax = FIRE_FLAMEWIDTH + FIRE_FLAMEAMPLITUDE ;
const uint8_t _corestrength = FIRE_CORESTRENGTH + 1U ;
const uint8_t _coreresist = FIRE_CORERESIST + 1U ;
const uint8_t _coreamplitude = FIRE_COREAMPLITUDE + 1U ;
const uint8_t _flamestrength = FIRE_FLAMESTRENGTH + 1U ;
const uint8_t _flameresist = FIRE_FLAMERESIST + 1U ;
const uint8_t _flameamplitude = FIRE_FLAMEAMPLITUDE + 1U ;
const uint8_t _sparkstrength = FIRE_SPARKSTRENGTH + 1U ;
const uint8_t _flameseed = FIRE_FLAMESEED ;
const uint8_t _coreoffset = FIRE_COREWIDTH + FIRE_OFFSET ;
const uint8_t _nofmasks = WIDTH / FIRE_COREWIDTH + (WIDTH % FIRE_COREWIDTH) ? 2U : 1U ;
const uint8_t _iseven = (FIRE_HEIGHT & 2U) ? 0U : 1U ;
const uint8_t _sparkdelay = FIRE_SPARKDELAY - 1U;
const uint8_t _plumestep = FIRE_HEIGHT + 1U;

uint8_t shiftValue[FIRE_HEIGHT];                          // массив дороожки горизонтального смещения пламени (hueValue)

#if FIRE_FLAMEMIRROR                                      // ***** FLAMEMIRROR Инициализация *****
#if FIRE_FLAMEMIRROR < 50U                                // FLAMEMIRROR = 1...49
uint8_t _shiftHue = FIRE_FLAMEWIDTH - 1U;
const uint8_t _flamecount = FIRE_FLAMEMIRROR ;
#elif FIRE_FLAMEMIRROR < 100U                             // FLAMEMIRROR = 50...99
uint8_t _shiftHue = FIRE_FLAMEWIDTH - 1U ;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 50U) * 2U ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
#elif FIRE_FLAMEMIRROR < 101U                             // FLAMEMIRROR = 100
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;
#elif FIRE_FLAMEMIRROR < 150U                             // FLAMEMIRROR = 101...149
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamecount = FIRE_FLAMEMIRROR - 100U ;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 200U                             // FLAMEMIRROR = 150...199
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 150U) * 2U  ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 201U                             // FLAMEMIRROR = 200
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 202U                             // FLAMEMIRROR = 201
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT][_nofmasks] ;                // массив дороожки горизонтального смещения пламени (hueMask)
#else
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 201U) * 2U ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
uint8_t shiftHue[FIRE_HEIGHT][_nofmasks] ;                // массив дороожки горизонтального смещения пламени (hueMask)
#endif
#endif

uint8_t matrixValue[FIRE_HEIGHT + 2U][WIDTH];		// <- ******* для всех прошивок!!! закомментить предыдцщие инициализации и строки с упоминанием. Проверить глобальным поискоом!  *******
uint8_t line[WIDTH][2U];                                // буфер теневой маски
uint8_t pcnt;
uint16_t _rowcounter;
uint16_t _framecounter;
bool _mirror;
uint8_t _mirrorstate [_nofmasks];
#if FIRE_SMOOTH
uint8_t _smooth = FIRE_SMOOTH;
uint8_t _smoothstep = 256.0 / FIRE_SMOOTH;
#endif
uint8_t _seedstep = 256.0 / _flameseed;
uint8_t _sdelay = 128U;
uint8_t _darkness;
uint8_t _value;
uint8_t _windspeed = WIDTH;


#if FIRE_FLAMEMIRROR < 201U                             // FLAMEMIRROR = 201
uint8_t _briefValue;
uint8_t _briefNewValue;
#else
uint8_t _briefValue[_nofmasks];
uint8_t _briefNewValue[_nofmasks];
#endif

void fire_drawFrame(bool isColored);

void fireRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }
  
  fire_drawFrame(true);
}

const uint8_t _plumeinc_min = (255.0 - FIRE_PLUMELIGHTMAX) / (float) FIRE_PLUMEHEIGHTMIN;
const uint8_t _plumeinc_max = (255.0 - FIRE_PLUMELIGHTMIN) / (float) FIRE_PLUMEHEIGHTMAX ;
const uint8_t _plumedec_min = (255.0 - FIRE_PLUMELIGHTMAX) / 2.0 ;
const uint8_t _plumedec_max = (255.0 - FIRE_PLUMELIGHTMIN) / 2.0 ;

void fire_shiftUp() {
  for (uint8_t y = FIRE_HEIGHT; y > 0U; y--) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      matrixValue[y][x] = matrixValue[y - 1U][x];
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++) {
    if (matrixValue[_plumestep][x]) {
      matrixValue[0U][x] = (matrixValue[_plumestep][x] == 1U )
                           ? qsub8(matrixValue[0U][x], random8(_plumedec_min, _plumedec_max))
                           : qadd8(matrixValue[0U][x], random8(_plumeinc_min, _plumeinc_max));
      matrixValue[_plumestep][x]--;
    } else {
      uint8_t x1max = random8(FIRE_PLUMEWIDTHMIN, FIRE_PLUMEWIDTHMAX);
      uint8_t _plumeheight = random8(FIRE_PLUMEHEIGHTMIN, FIRE_PLUMEHEIGHTMAX);
      uint8_t mValue = random8(FIRE_PLUMELIGHTMIN, FIRE_PLUMELIGHTMAX);

      for (uint8_t x1 = 0U; x1 < x1max; x1++) {
        uint8_t x2 = (x + x1) % WIDTH;
        matrixValue[0U][x2] = ((x1 == 0) || (x1 == x1max)) ? qsub8(mValue, 24U) : mValue;
        matrixValue[_plumestep][x2] = _plumeheight;
      }
      x += x1max;
    }
  }
}

void fire_drawFrame(bool isColored) {                                         // прорисовка нового кадра

#ifdef FIRE_UNIVERSE                                                          // если определен универсальный огонь вычисляем его базовый цвет  
  uint8_t baseHue = (myLamp.effects.getScale() * 2.57, _flameseed) ;
  uint8_t baseSat = (myLamp.effects.getScale() == FIRE_WHITEVALUE) ? 0U : 255U ;  // отключение насыщенности при определенном положении колеса
#else
  uint8_t baseHue = isColored ? 255U : 0U;                                    // старое определение цвета
  uint8_t baseSat = 255;
#endif

#if FIRE_SMOOTH
  uint8_t _framemod = _framecounter % FIRE_SMOOTH;
  if (!_framemod) fire_shiftUp();
#else
  fire_shiftUp();
#endif

#if FIRE_BREATHSPEED
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (_briefValue[i] == _briefNewValue[i]) {
      _briefNewValue[i]  = random(FIRE_BREATHDEEP);
    } else {
      _briefValue[i] = (_briefValue[i] < _briefNewValue[i])
                       ?  min(qadd8(_briefValue[i], FIRE_BREATHSPEED ), _briefNewValue[i])
                       : max(qsub8(_briefValue[i], FIRE_BREATHSPEED), _briefNewValue[i]);
    }
  }
#else
  if (_briefValue == _briefNewValue) {
    _briefNewValue  = random(FIRE_BREATHDEEP);
  } else {
    _briefValue = (_briefValue < _briefNewValue)
                  ?  min(qadd8(_briefValue, FIRE_BREATHSPEED ), _briefNewValue)
                  : max(qsub8(_briefValue, FIRE_BREATHSPEED), _briefNewValue);
  }
#endif



#if FIRE_FLAMEMIRROR                                  // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U                            // _mirror = 1...49
  if (!(_rowcounter % _flamecount) && (_rowcounter % FIRE_HEIGHT)) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
#elif FIRE_FLAMEMIRROR < 101U
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 100...149  
  if (!(_rowcounter % _flamecount) && (_rowcounter % FIRE_HEIGHT)) _mirror = !_mirror ;
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 150...199  
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201

#else                                                 // _mirror = 202...255
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
#endif
#endif

  //each row interpolates with the one before it
  for (uint8_t y = FIRE_HEIGHT - 1U; y < FIRE_HEIGHT; y--) {   // Work остальных строк
#if FIRE_FLAMEMIRROR                                  // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U                            // _mirror = 1...49
    if (!(_rowcounter % _flamecount) ) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
    if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 101U                         // _mirror = 100
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 101...149 
    if (!(_rowcounter % FIRE_FLAMEMIRROR)) _mirror = !_mirror ;
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 151U                         // _mirror = 150
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 151...199 
    if (!random8(_flamemirror) && (_rowcounter % _flamecount)) _mirror = !_mirror ;
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201
#else
    if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
      _mirror = !_mirror ;
    }
#endif
#endif

    for (uint8_t x = 0U; x < WIDTH; x++) {            // прорисовка пламени
#if FIRE_FLAMEMIRROR == 0U                            // ***** HUE_mirror *****
      uint8_t _offset = x + _coreoffset;
      uint8_t z = _offset % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 50U                          // _mirror = 1...49
      uint8_t _offset = x + _coreoffset;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
      uint8_t _offset = x + _coreoffset;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 101U                         // _mirror = 100
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset - _shiftHue;
      uint8_t z = _offset % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 101...149      
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 151U                         // _mirror = 150
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 151...199      
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _mirror = (x / FIRE_FLAMEWIDTH) % 2U;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _shiftHue = shiftHue[y][x / FIRE_FLAMEWIDTH];
      uint8_t z = qadd8(_offset, _shiftHue) % FIRE_FLAMEWIDTH;
#else                                                 // _mirror = 202...255
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _shiftHue = shiftHue[y][x / FIRE_FLAMEWIDTH];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#endif

#if FIRE_BREATHSPEED
      _darkness = qadd8(_briefValue[x / FIRE_FLAMEWIDTH], random8(FIRE_CORESEED));
#else
      _darkness = qadd8(_briefValue, random8(FIRE_CORESEED));
#endif

#if FIRE_SMOOTH
      uint8_t _scale = qmul8(_smoothstep, _framemod);
      _value = qadd8( scale8( matrixValue[y + 1U][x] , qsub8( 255, _scale)), scale8( matrixValue[y][x] , _scale ) )  ;
#elif FIRE_SMOOTH == 1
      _value = avg8( matrixValue[y][x], matrixValue[y + 1U][x]);
#else
      _value = matrixValue[y + 1U][x];
#endif
      uint8_t _hue =  qadd8(random8(_flameseed), pgm_read_byte(&hueMask[y][z]));
#if FIRE_COREEDGE
      uint8_t _white = qsub8(_hue, FIRE_COREEDGE);
      uint8_t _whiteness = qsub8( baseSat, scale8( FIRE_COREDEEP, qsub8( 255, qmul8(_seedstep , _white))));
      uint8_t _saturation = _white ?  _whiteness : baseSat;
#else
      uint8_t _saturation = baseSat;
#endif

      myLamp.drawPixelXY(x, y, CHSV(                    // определение цвета пикселя
                    qadd8(_hue, baseHue) , // H - смещение всполохов
                    _saturation,           // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                    qsub8(_value, qadd8(pgm_read_byte(&valueMask[y][_offset % FIRE_FLAMEWIDTH]), _darkness)))); // V
    }
    if (y) {
      shiftValue[y] = shiftValue[y - 1];                 // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем

#if FIRE_FLAMEMIRROR                                   // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U
#elif FIRE_FLAMEMIRROR < 100U                          // _mirror = 51...99   
#elif FIRE_FLAMEMIRROR < 200U                          // _mirror = 151...199    
      shiftHue[y] = shiftHue[y - 1];                     // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
#elif FIRE_FLAMEMIRROR < 201U                          // _mirror = 200
      shiftHue[y] = shiftHue[y - 1];
#elif FIRE_FLAMEMIRROR < 202U                          // _mirror = 201
      for (uint8_t i = 0; i < _nofmasks; i++) {
        shiftHue[y][i] = shiftHue[y - 1][i];
      }
#else
      for (uint8_t i = 0; i < _nofmasks; i++) {
        shiftHue[y][i] = shiftHue[y - 1][i];
      }
#endif
#endif
    }
    _rowcounter++;

  }
#if FIRE_CORESTRENGTH                                     // смещение очага 
  if (!random8(_corestrength) && !(_rowcounter % _coreresist)) {
    shiftValue[0] = constrain(qsub8(qadd8(shiftValue[0],  random8(_coreamplitude)),  random8(_coreamplitude)), _coreshiftmin, _coreshiftmax);
  }
#endif

#if FIRE_FLAMEMIRROR                                      // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U
#elif FIRE_FLAMEMIRROR < 100U                             // _mirror = 51...99
#elif FIRE_FLAMEMIRROR < 200U                             // _mirror = 151...199  
  if (!random8(_flamestrength) && !(_rowcounter % _flameresist)) {
    shiftHue[0] = constrain(qsub8(qadd8(shiftHue[0],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
  }
#elif FIRE_FLAMEMIRROR < 201U                             // _mirror = 200  
  if (!random8(_flamestrength)  && !(_rowcounter % _flameresist)) {
    shiftHue[0] = constrain(qsub8(qadd8(shiftHue[0],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
  }
#elif FIRE_FLAMEMIRROR < 202U                             // _mirror = 201
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (!random8(_flamestrength) && !((_rowcounter + i) % _flameresist) ) {
      shiftHue[0][i] = constrain(qsub8(qadd8(shiftHue[0][i],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
    }
  }
#else                                                     // _mirror = 202...255 
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (!random8(_flamestrength) && !((_rowcounter + i) % _flameresist) ) {
      shiftHue[0][i] = constrain(qsub8(qadd8(shiftHue[0][i],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
    }
  }
#endif
#endif

#if FIRE_SPARKLES  // если это самая нижняя строка искр - формитуем искорку из пламени
  if (!(_framecounter % _sparkdelay)) {
    for (uint8_t y = HEIGHT - 1U; y > FIRE_HEIGHT; y--) {
      for (uint8_t x = 0U; x < WIDTH; x++) {      // стираем строчку с искрами (очень не оптимально)
        myLamp.drawPixelXY(x, y, 0U);
      }
      for (uint8_t x = 0U; x < WIDTH; x++) {

        /*#if FIRE_WINDCASE  // TODO
                if  (!random8(_sparkstrength) && ((_framecounter >> 8) % FIRE_WINDCASE )) {
                  _windspeed = constrain(_windspeed + random8(2U) - random8(2U), WIDTH - 1U, WIDTH + 1U) % WIDTH;
                }
                _windspeed = 15U;
          #endif*/
#if FIRE_SPARKSTRENGTH
        uint8_t newX = (random8(_sparkstrength)) ? x : (x + WIDTH + random8(2U) - random8(2U)) % WIDTH ;   // с вероятностью 1/3 смещаем искорку влево или вправо
#else
        uint8_t newX = x;
#endif
        if (myLamp.getPixColorXY(x, y - 1U) > 0U) myLamp.drawPixelXY(newX, y, myLamp.getPixColorXY(x, y - 1U));    // рисуем искорку на новой строчке
      }
    }
    for (uint8_t x = 0U; x < WIDTH; x++) {                  // если это не самая нижняя строка искр - перемещаем искорку выше
      if (random8(FIRE_EVERYNSPARK) == 0 && myLamp.getPixColorXY(x, FIRE_HEIGHT - 1U) != 0U) {
        uint16_t px = myLamp.getPixelNumber (x, FIRE_HEIGHT);
        myLamp.setLeds(px, myLamp.getPixColorXY(x, FIRE_HEIGHT - FIRE_SPARKBORNDEEP));
        myLamp.setLeds(px, (unsigned int)myLamp.getLeds(px) % FIRE_SPARKSBRIGHT);
      } else myLamp.drawPixelXY(x, FIRE_HEIGHT, 0U);
    }
  }
#endif
  if (_rowcounter > 65521U) {
    _rowcounter = _rowcounter % 65521 + _iseven  ;
  }
  _framecounter++;
}

// ------------- белый свет -------------
void whiteColorRoutine(char *param)
{
  if (myLamp.isLoading())
  {
    FastLED.clear();

    for (uint16_t i = 0U; i < NUM_LEDS; i++)
    {
      myLamp.setLeds(i, CHSV(0U, 0U, 255U));
    }
  }
}

// ------------- белый свет (светится горизонтальная полоса по центру лампы; масштаб - высота центральной горизонтальной полосы; скорость - регулировка от холодного к тёплому; яркость - общая яркость) -------------
void whiteColorStripeRoutine(CRGB *leds, const char *param)
{
    if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
      return;
    } else {
      myLamp.setEffDelay(millis());
    }
    
    uint8_t centerY = max((uint8_t)round(HEIGHT / 2.0F) - 1, 0);
    uint8_t bottomOffset = (uint8_t)(!(HEIGHT & 1) && (HEIGHT > 1));                      // если высота матрицы чётная, линий с максимальной яркостью две, а линии с минимальной яркостью снизу будут смещены на один ряд
    for (int16_t y = centerY; y >= 0; y--)
    {
      CRGB color = CHSV(
        45U,                                                                              // определяем тон
        map(myLamp.effects.getSpeed(), 0U, 255U, 0U, 170U),                            // определяем насыщенность
        y == centerY                                                                                                    // определяем яркость
          ? BRIGHTNESS                                                                                                  // для центральной горизонтальной полосы (или двух) яркость всегда равна BRIGHTNESS
          : (myLamp.effects.getScale() / 100.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? BRIGHTNESS : 0);  // для остальных горизонтальных полос яркость равна либо BRIGHTNESS, либо 0 в зависимости от масштаба //BRIGHTNESS/((centerY + 1.0F)-y)

      for (uint8_t x = 0U; x < WIDTH; x++)
      {
        myLamp.drawPixelXY(x, y, color);                                                         // при чётной высоте матрицы максимально яркими отрисуются 2 центральных горизонтальных полосы
        myLamp.drawPixelXY(x, max((uint8_t)(HEIGHT - 1U) - (y + 1U) + bottomOffset, 0U), color); // при нечётной - одна, но дважды
      }
    }
}

// ============= водо/огне/лава/радуга/хренопад ===============
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80U // 50 // 30 // 120 // 90 // 60
void fire2012WithPalette(CRGB*leds, const char *param) {
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }


  uint8_t scale = myLamp.effects.getScale();
  uint8_t COOLINGNEW = constrain((uint16_t)(scale % 16) * 32 / HEIGHT + 16, 1, 255) ;
  // Array of temperature readings at each simulation cell
  static byte heat[WIDTH][HEIGHT];

  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Cool down every cell a little
    for (unsigned int i = 0; i < HEIGHT; i++) {
      //heat[x][i] = qsub8(heat[x][i], random8(0, ((COOLINGNEW * 10) / HEIGHT) + 2));
      heat[x][i] = qsub8(heat[x][i], random8(0, COOLINGNEW));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = HEIGHT - 1; k >= 2; k--) {
      heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKINGNEW) {
      int y = random8(2);
      heat[x][y] = qadd8(heat[x][y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (unsigned int j = 0; j < HEIGHT; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8(heat[x][j], 240);
      if  (scale < 16) {            // Lavafall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(LavaColors_p, colorindex));
      } else if (scale < 32) {      // Firefall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(HeatColors_p, colorindex));
      } else if (scale < 48) {      // Waterfall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(PartyColors_p, colorindex));
      } else if (scale < 64) {      // Skyfall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(CloudColors_p, colorindex));
      } else if (scale < 80) {      // Forestfall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(ForestColors_p, colorindex));
      } else if (scale < 96) {      // Rainbowfall
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(RainbowColors_p, colorindex));        
      } else {                      // Aurora
        myLamp.setLeds(myLamp.getPixelNumber(x, (HEIGHT - 1) - j), ColorFromPalette(RainbowStripeColors_p, colorindex));
      }
    }
  }
}

// --------------------------- эффект пульс ----------------------
// Stefan Petrick's PULSE Effect mod by PalPalych for GyverLamp 

void drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CRGB & color){
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    myLamp.drawPixelXY(x0, y0, color);
    return;
  }

  while (a >= b)  {
    myLamp.drawPixelXY(a + x0, b + y0, color);
    myLamp.drawPixelXY(b + x0, a + y0, color);
    myLamp.drawPixelXY(-a + x0, b + y0, color);
    myLamp.drawPixelXY(-b + x0, a + y0, color);
    myLamp.drawPixelXY(-a + x0, -b + y0, color);
    myLamp.drawPixelXY(-b + x0, -a + y0, color);
    myLamp.drawPixelXY(a + x0, -b + y0, color);
    myLamp.drawPixelXY(b + x0, -a + y0, color);
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

uint8_t pulse_hue;
uint8_t pulse_step = 0;
void pulse(CRGB *leds, const char *param) {
    if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
      return;
    } else {
      myLamp.setEffDelay(millis());
    }

  CRGBPalette16 palette;
  CRGB _pulse_color;
  uint8_t currentRadius = 4;
  uint8_t centerX = random8(WIDTH - 5U) + 3U;
  uint8_t centerY = random8(HEIGHT - 5U) + 3U;
  //uint16_t _rc;
  uint8_t _pulse_hue = 0;
  uint8_t _pulse_hueall = 0;
  uint8_t _pulse_delta = 0;

  palette = RainbowColors_p;
  uint8_t _scale = myLamp.effects.getScale();
  //const uint8_t limitsteps = 6U;
  //static const float fadeRate = 0.8;

  myLamp.dimAll(248U); // если эффект устанавливается с другими эффектами от Stefan Petrick, тогда  процедура должна называться dimAll (без двоечки)
  if (pulse_step <= currentRadius) {
    for (uint8_t i = 0; i < pulse_step; i++ ) {
      uint8_t _dark = qmul8( 2U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
      if (_scale == 1) {            // 1 - случайные диски
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);
      
      } else if (_scale <= 17) {    // 2...17 - перелив цвета дисков 
        _pulse_delta = (17U - _scale) ;
        _pulse_color = CHSV(_pulse_hueall, 255U, _dark);
     
      } else if (_scale <= 33) {    // 18...33 - выбор цвета дисков 
        _pulse_hue = (_scale - 18U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);
      
      } else if (_scale <= 50) {    // 34...50 - дискоцветы
        _pulse_hue += (_scale - 33U) * 5U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);
      
      } else if (_scale <= 67) {    // 51...67 - пузыри цветы
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
         _pulse_hue += (68U - _scale) * 7U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      
      } else if (_scale < 83) {     // 68...83 - выбор цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = (_scale - 68U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      
      } else if (_scale < 100) {    // 84...99 - перелив цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_delta = (_scale - 85U)  ;
        _pulse_color = CHSV(_pulse_hueall, _sat, _dark);
      
      } else { // 100 - случайные пузыри
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      }
      drawCircle(centerX, centerY, i, _pulse_color  );
    }
  } else {
    centerX = random8(WIDTH - 5U) + 3U;
    centerY = random8(HEIGHT - 5U) + 3U;
    _pulse_hueall += _pulse_delta;
    pulse_hue = random8(0U, 255U);
    currentRadius = random8(3U, 9U);
    pulse_step = 0;
  }
  pulse_step++;
}

// радуги 2D
// ------------- радуга вертикальная/горизонтальная ----------------
uint8_t hue;
void rainbowHorVertRoutine(bool isVertical)
{
  hue += 4;
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
    CHSV thisColor = CHSV((uint8_t)(hue + i * myLamp.effects.getScale()%170), 255, 255);
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      myLamp.drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
}

// ------------- радуга диагональная -------------
void rainbowDiagonalRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)(255-myLamp.effects.getSpeed())){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }

  if(myLamp.effects.getScale()<85){
    rainbowHorVertRoutine(false);
    return;
  } else if (myLamp.effects.getScale()>170){
    rainbowHorVertRoutine(true);
    return;
  }

  hue += 4;
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      float twirlFactor = 3.0F * (myLamp.effects.getScale() / 30.0F);      // на сколько оборотов будет закручена матрица, [0..3]
      CRGB thisColor = CHSV((uint8_t)(hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)myLamp.getmaxDim())), 255, 255);
      myLamp.drawPixelXY(i, j, thisColor);
    }
  }
}

// ------------- цвета -----------------
void colorsRoutine(CRGB *leds, const char *param)
{
  static unsigned int step = 0; // доп. задержка
  unsigned int delay = (myLamp.effects.getSpeed()==1)?4294967294:255-myLamp.effects.getSpeed(); // на скорости 1 будет очень долгое ожидание)))
  
  if (myLamp.isLoading()){ // начальная установка цвета
    hue = myLamp.effects.getScale(); myLamp.fillAll(CHSV(hue, 255U, 255U)); return;
  } else {
    step=(step+1)%(delay+1);
    if(step!=delay) return;
  }
  //LOG.println(hue);

  hue += myLamp.effects.getScale();
  myLamp.fillAll(CHSV(hue, 255U, 255U));
}
